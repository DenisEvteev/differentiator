//
// Created by denis on 22.08.19.
//

#include "BinaryTree.hpp"


const BinaryTree::map BinaryTree::func_name = {{'s', "\\sin"}, {'c', "\\cos"}, {'n', "\\arcsin"},
                                         {'l', "\\log"}, {'a', "\\arccos"}, {'t', "\\tg"},
                                         {'g', "\\ctg"}};

const BinaryTree::ar_simpl BinaryTree::simpl_funcs = {{'^', &BinaryTree::SimplifyExponent},
                                                      {'+', &BinaryTree::SimplifyPlus},
                                                      {'-', &BinaryTree::SimplifyMinus},
                                                      {'*', &BinaryTree::SimplifyMultiplication},
                                                      {'/', &BinaryTree::SimplifyDivision}};


BinaryTree::al_ptr_f BinaryTree::alloc_print = BinaryTree::al_ptr_f();

BinaryTree::BinaryTree(Obj *root) : __root(root) {
    func_print_type = b_traits::allocate(BinaryTree::alloc_print, SIZE_ARRAY_PRINT);
    b_traits::construct(BinaryTree::alloc_print, func_print_type, &BinaryTree::PrintValue);
    b_traits::construct(BinaryTree::alloc_print, func_print_type + 1, &BinaryTree::PrintOperator);
    b_traits::construct(BinaryTree::alloc_print, func_print_type + 2, &BinaryTree::PrintFunc);
    b_traits::construct(BinaryTree::alloc_print, func_print_type + 3, &BinaryTree::PrintUnknown);
}


void BinaryTree::set_root(Obj* root){
    __root = root;
}

BinaryTree::~BinaryTree(){
    for(int i = 0; i < SIZE_ARRAY_PRINT; ++i)
        b_traits::destroy(BinaryTree::alloc_print, func_print_type + i);

    b_traits::deallocate(BinaryTree::alloc_print, func_print_type, SIZE_ARRAY_PRINT);
    __root = nullptr;
}

void BinaryTree::PrintValue(std::ofstream& out, Obj* root)const{
    out << dynamic_cast<Number*>(root)->get_val();
    if(root->get_parent() && (root->get_parent())->get_right() != root
       && dynamic_cast<Symbol*>(root->get_parent())->get_name() != '^')
        out << "{}";

    out.flush();
}

void BinaryTree::PrintOperator(std::ofstream &out, Obj *root) const {
    //this case is only for operator with name -
    if(!root->get_left()){
        out << "\\left(";
        out << dynamic_cast<Symbol*>(root)->get_name();
        dump_tree(root->get_right(), out);
        out << "\\right)";
        out.flush();
    }

    else{
        if(CheckParentPriority(root))
            out << "\\left(";
        auto op = dynamic_cast<Symbol*>(root);

        switch(op->get_name()){
            case '*':{
                dump_tree(root->get_left(), out);
                if(root->get_left()->get_type() != type(Value))
                    out << "{}";

                out << "\\cdot" << "{}";

                dump_tree(root->get_right(), out);
                break;
            }


            case '^':{

                if(root->get_right()->get_type() == type(Value)
                && dynamic_cast<Number*>(root->get_right())->get_val() == 0.5){
                    out << "\\sqrt{";
                    dump_tree(root->get_left(), out);
                    out << "}";
                    break;
                }
                if(root->get_left()->get_type() != type(Value) && root->get_left()->get_type() != type(Unknown)){
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
                if(root->get_left()->get_type() != type(Value))
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
    auto name_f = dynamic_cast<Symbol*>(root);

    if(name_f->get_name() == 'l'){
        auto base = dynamic_cast<Symbol*>(name_f->get_left());
        if(base && base->get_name() == 'e'){
            out << "\\ln \\left(";
            dump_tree(root->get_right(), out);
            out << "\\right)";
        }
        else if(root->get_left()->get_type() == type(Value) &&
        dynamic_cast<Number*>(root->get_left())->get_val() == 10){
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
        out << BinaryTree::func_name.at(name_f->get_name()) << "{}\\left(";
        dump_tree(root->get_right(), out);
        out << "\\right)";
    }

}

void BinaryTree::PrintUnknown(std::ofstream &out, Obj *root) const {
    out << dynamic_cast<Symbol*>(root)->get_name();
    out.flush();
}

void BinaryTree::dump_tree(Obj* root,  std::ofstream& out) const{
    if(!root)
        return;

    (this->*func_print_type[root->get_type()])(out, root);
}

bool BinaryTree::CheckParentPriority(Obj* obj)const{
    auto cur_tok = dynamic_cast<Symbol*>(obj);
    auto prev_tok = dynamic_cast<Symbol*>(obj->get_parent());

    if(!prev_tok)
        return false;

    if((prev_tok->get_name() == '*' || prev_tok->get_name() == '/')
    && (cur_tok->get_name() == '+' || cur_tok->get_name() == '-'))
        return true;

    if(prev_tok->get_name() == '^')
        return true;

    return false;
}

Obj* BinaryTree::get_root() const {
    return __root;
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

        case type(Value): {
            return root;
        }

        case type(Unknown):{
            return root;
        }

        case type(Func): {
            return root;
        }

        case type(Operator): {
            auto op = dynamic_cast<Symbol *>(root);
            try{
                return (this->*simpl_funcs.at(op->get_name()))(op);
            }catch(const std::out_of_range& ex){

                std::cout << ex.what() << '\n';
                std::cout << "__ERROR__ at line :" << __LINE__ << std::endl;
                std::cout << "Function : " << __PRETTY_FUNCTION__ << std::endl;
            }

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
    if(CHECK_NOL_CHILD(root->get_right())){
        Obj* copy_func = root->get_left();
        root->set_left(nullptr);
        return WorkWithRightOne(copy_func, root);
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
    Obj* new_root = num->create(type(Value), value);

    if(root->get_parent())
        root->MakeChild(new_root);

    new_root->set_parent(root->get_parent());

    root->remove(root);
    return new_root;
}

Obj* BinaryTree::MakeCount(Obj* root){
    if(root->get_right() &&
    root->get_right()->get_type() == type(Value)
    && ((root->get_left() && root->get_left()->get_type() == type(Value)) || !root->get_left())){

        auto symbol = dynamic_cast<Symbol*>(root);

        Number* pivotal = nullptr;

        if(!root->get_left())
            pivotal = symbol->calc(dynamic_cast<Number*>(root->get_right())->get_val(), 0);
        else
            pivotal = symbol->calc(dynamic_cast<Number*>(root->get_right())->get_val(),
                    dynamic_cast<Number*>(root->get_left())->get_val());


        if(!root->get_parent()) {
            __root = pivotal;
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

    __root = pivotal;

}