//
// Created by denis on 22.08.19.
//

#include "BinaryTree.hpp"

BinaryTree::ptr_print BinaryTree::printer[SIZE_ARRAY_PRINT] = { &BinaryTree::PrintValue,
																&BinaryTree::PrintOperator,
																&BinaryTree::PrintFunc,
																&BinaryTree::PrintUnknown };

BinaryTree::ptr_funcs BinaryTree::simplifier[SIZE_ARRAY_SIMPLIFY] = { &BinaryTree::SimplifyPlus,
																	  &BinaryTree::SimplifyMinus,
																	  &BinaryTree::SimplifyMultiplication,
																	  &BinaryTree::SimplifyDivision,
																	  &BinaryTree::SimplifyExponent };

String                BinaryTree::func_names[SIZE_ARRAY_FUNCTIONS] = { String("\\sin"),
																	   String("\\cos"),
																	   String("\\arcsin"),
																	   String("\\log"),
																	   String("\\arccos"),
																	   String("\\tg"),
																	   String("\\ctg") };

BinaryTree::~BinaryTree()
{
	clear(root_);
}

BinaryTree::BinaryTree(Obj *root) : root_(root) {
	assert(root_);
}

BinaryTree& BinaryTree::operator=(BinaryTree&& rhs) noexcept {
	root_ = rhs.root_;
	rhs.root_ = nullptr;
	return *this;
}

void BinaryTree::set_root(Obj* root){
	assert(root_ == nullptr); //otherwise this operation seems to be very strange
    root_ = root;
}

void BinaryTree::PrintValue(std::ofstream& out, Obj* root)const{
	assert(root);
    out << static_cast<Number*>(root)->get_val();
    if (root->get_parent() && (root->get_parent())->get_right() != root
		&& static_cast<Symbol*>(root->get_parent())->get_name() != '^')
		out << "{}";
    out.flush();
}

void BinaryTree::PrintOperator(std::ofstream &out, Obj *root) const {
    //this case is only for operator with name -
    if(!root->get_left()){
        out << "\\left(";
        out << static_cast<Symbol*>(root)->get_name();
        dump_tree(root->get_right(), out);
        out << "\\right)";
        out.flush();
    }

    else{
        if(CheckParentPriority(root))
            out << "\\left(";
        auto op = static_cast<Symbol*>(root);

        switch(op->get_name()){
            case '*':{
                dump_tree(root->get_left(), out);
                if(root->get_left()->get_type() != Value)
                    out << "{}";

                out << "\\cdot" << "{}";

                dump_tree(root->get_right(), out);
                break;
            }


            case '^':{

                if(root->get_right()->get_type() == Value
                && static_cast<Number*>(root->get_right())->get_val() == 0.5){
                    out << "\\sqrt{";
                    dump_tree(root->get_left(), out);
                    out << "}";
                    break;
                }
                if(root->get_left()->get_type() != Value && root->get_left()->get_type() != Unknown){
                    out << "\\left(";
                    dump_tree(root->get_left(), out);
                    out << "\\right)";
                }
                else
                    dump_tree(root->get_left(), out);

                out << "^{";

                dump_tree(root->get_right(), out);

                out << "}";
                break;

            }

            case '/':{

                out << "\\frac{";
                dump_tree(root->get_left(), out);
                out << "}{";
                dump_tree(root->get_right(), out);
                out << '}';
                break;

            }
            default:{
                dump_tree(root->get_left(), out);
                if(root->get_left()->get_type() != Value)
                    out << "{}";

                out << op->get_name() << "{}";

                dump_tree(root->get_right(), out);
                break;
            }
        }

        if(CheckParentPriority(root))
            out << "\\right)";

        out.flush();
    }
}

void BinaryTree::PrintFunc(std::ofstream &out, Obj *root) const {
    auto name_f = static_cast<Symbol*>(root);

    if(name_f->get_name() == 'l'){
        auto base = static_cast<Symbol*>(name_f->get_left());
        if(base && base->get_name() == 'e'){
            out << "\\ln \\left(";
            dump_tree(root->get_right(), out);
            out << "\\right)";
        }
        else if(root->get_left()->get_type() == Value &&
        static_cast<Number*>(root->get_left())->get_val() == 10){
            out << "\\lg \\left(";
            dump_tree(root->get_right(), out);
            out << "\\right)";
        }
        else{
            out << "\\log_{";
            dump_tree(root->get_left(), out);
            out << "}{\\left(";
            dump_tree(root->get_right(), out);
            out << "\\right)}";
        }

    }
    else{
    	int id = name_f->get_id() - BOUND;
    	assert(id >= 0 && id <= 6);
        out << func_names[id].GetStr() << "{}\\left(";
        dump_tree(root->get_right(), out);
        out << "\\right)";
    }

}

void BinaryTree::PrintUnknown(std::ofstream &out, Obj *root) const {
    out << static_cast<Symbol*>(root)->get_name();
    out.flush();
}

void BinaryTree::dump_tree(Obj* root,  std::ofstream& out) const{
    if(!root)
        return;
    int type = root->get_type();
    assert(type >= 0 && type <= 3);
    std::invoke(printer[type], *this, out, root);
}

bool BinaryTree::CheckParentPriority(Obj* obj)const{
	assert(obj);
    auto cur_tok = static_cast<Symbol*>(obj);
    auto prev_tok = static_cast<Symbol*>(obj->get_parent());

    if(!prev_tok)
        return false;

    if((prev_tok->get_name() == '*' || prev_tok->get_name() == '/')
    && (cur_tok->get_name() == '+' || cur_tok->get_name() == '-'))
        return true;

    if(prev_tok->get_name() == '^')
        return true;

    return false;
}

void BinaryTree::clear(Obj* root) {
    /*Will clean all the allocated memory for this tree
     * and i can determine the start position for this operation
     * pointing to the appropriate root */
    root->remove(root);
}

Obj* BinaryTree::WorkWithRightOne(Obj* copy_func, Obj* root){
    copy_func->set_parent(root->get_parent());

    if(root->get_parent())
        root->MakeChild(copy_func);

    root->remove(root);

    return copy_func;
}

Obj* BinaryTree::Calculate(Obj* root) {

    if (!root)
        return nullptr;

    switch (root->get_type()) {

        case Value   :
		case Unknown :
		case Func    :
            return root;

		case Operator : {
            auto op = static_cast<Symbol *>(root);
            int id = op->get_id();
            assert(id <= 4 && id >= 0);
            return std::invoke(simplifier[id], *this, op);
		}
        default : {
            std::cout << "No such type of nodes\n";
            std::cout << "__ERROR__ in function : " << __PRETTY_FUNCTION__ << std::endl;
            return nullptr;
        }
    }
}


Obj* BinaryTree::SimplifyDivision(Obj *root) {
    //cases (0/smth)
    if (CHECK_NOL_CHILD(root->get_left())) {
        return MakeNumericalNode(root, 0);
    }

    //cases (smth/1)
    if (CHECK_ONE(root->get_right())) {
        Obj *copy_func = root->get_left();
        root->set_left(nullptr);
        return WorkWithRightOne(copy_func, root);
    }

    return MakeCount(root);
}

Obj* BinaryTree::SimplifyMultiplication(Obj *root) {
    //cases (smth*0) || (0*smth)
    if (CHECK_NOL_CHILD(root->get_left()) || CHECK_NOL_CHILD(root->get_right())) {
        return MakeNumericalNode(root, 0);
    }

    //cases (1*smth)
    if (CHECK_ONE(root->get_left())) {
        Obj *copy_func = root->get_right();
        root->set_right(nullptr);
        return WorkWithRightOne(copy_func, root);
    }
    //cases (smth*1)
    if (CHECK_ONE(root->get_right())) {
        Obj *copy_func = root->get_left();
        root->set_left(nullptr);
        return WorkWithRightOne(copy_func, root);
    }


    return MakeCount(root);

}

Obj* BinaryTree::SimplifyMinus(Obj *root) {
    //cases (0-smth)
    if(root->get_left() && CHECK_NOL_CHILD(root->get_left())){
        root->remove(root->get_left());
		root->set_left(nullptr);
		return root;
	}

	//cases (smth-0)
	if (CHECK_NOL_CHILD(root->get_right()))
	{
		Obj* copy_func = root->get_left();
		root->set_left(nullptr);
		return WorkWithRightOne(copy_func, root);
	}

	//case (smth - smth) this node is 0
	if (root->get_left() && root->get_right() &&
		root->get_left()->get_type() == root->get_right()->get_type() && *(root->get_left()) == *(root->get_right()))
	{
		return MakeNumericalNode(root, 0);
	}

	return MakeCount(root);
}

Obj* BinaryTree::SimplifyPlus(Obj *root) {
    //cases (smth+0)
    if(CHECK_NOL_CHILD(root->get_right())){
        Obj* copy_func = root->get_left();
        root->set_left(nullptr);
        return WorkWithRightOne(copy_func, root);
    }


    //cases (0+smth)
    if(CHECK_NOL_CHILD(root->get_left())){
        Obj* copy_func = root->get_right();
        root->set_right(nullptr);
        return WorkWithRightOne(copy_func, root);
    }


    return MakeCount(root);
}


Obj* BinaryTree::SimplifyExponent(Obj* root){
    //cases (0^smth)
    if(CHECK_NOL_CHILD(root->get_left())){
        return MakeNumericalNode(root, 0);
    }

    //cases (smth^0)
    if(CHECK_NOL_CHILD(root->get_right())){
        return MakeNumericalNode(root, 1);
    }

    //cases (1^smth)
    if(CHECK_ONE(root->get_left())){
        return MakeNumericalNode(root, 1);
    }

    //cases (smth^1)
    if(CHECK_ONE(root->get_right())){
        Obj *copy_func = root->get_left();
        root->set_left(nullptr);
        return WorkWithRightOne(copy_func, root);
    }

    return MakeCount(root);
}

Obj* BinaryTree::MakeNumericalNode(Obj* root, float value){
    Number* num = nullptr;
    Obj* new_root = num->create(Value, value);

    if(root->get_parent())
        root->MakeChild(new_root);

    new_root->set_parent(root->get_parent());

    root->remove(root);
    return new_root;
}

Obj* BinaryTree::MakeCount(Obj* root){
    if(root->get_right() &&
    root->get_right()->get_type() == Value
    && ((root->get_left() && root->get_left()->get_type() == Value) || !root->get_left())){

        auto symbol = static_cast<Symbol*>(root);

        Number* pivotal = nullptr;

        if(!root->get_left())
            pivotal = symbol->calc(static_cast<Number*>(root->get_right())->get_val(), 0);
        else
            pivotal = symbol->calc(static_cast<Number*>(root->get_right())->get_val(),
                    static_cast<Number*>(root->get_left())->get_val());


        if(!root->get_parent()) {
            root_ = pivotal;
        }else {
            pivotal->set_parent(root->get_parent());
            root->MakeChild(pivotal);
        }

        root->remove(root);
        return pivotal;
    }

    return root;
}

void BinaryTree::SimplifyTree(Obj* obj){

    if(!obj)
        return;

    SimplifyTree(obj->get_left());

    SimplifyTree(obj->get_right());


    Obj* pivotal = Calculate(obj);

    if(pivotal == obj || pivotal->get_parent())
        return;

    root_ = pivotal;

}