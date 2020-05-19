//
// Created by denis on 21.08.19.
//

#include "differentiator.hpp"
#define DEBUG

differentiator::differentiator(){
    std::cout << "Enter the expresion : ";
    std::cin >> _str;  //initial string in traditional mathematical notation

    if(_str.size() != _str.GetNumBytes() - 1)
    	throw std::invalid_argument("not only ASCII symbols in the input string");

    _str = _str.ParseShortMathNotation(); // create the short mathematical notation of the input expression
#ifdef DEBUG
	std::cout << _str << std::endl;
#endif
    _cur = _str.GetStr();

    diff_type = new ptr_array_funcs[SIZE_ARRAY_PTR_FUNC];
    diff_type[0] = &differentiator::Diff_Value;
	diff_type[1] = &differentiator::Diff_Operator;
	diff_type[2] = &differentiator::Diff_Func;
	diff_type[3] = &differentiator::Diff_Unknown;

	func = new ptr_array_funcs[SIZE_ARRAY_EXACT_SYMBOL_DIFF];
	func[0] = &differentiator::Plus;
	func[1] = &differentiator::Minus;
	func[2] = &differentiator::Multiplication;
	func[3] = &differentiator::Division;
	func[4] = &differentiator::Exponentiation;

	func[5] = &differentiator::Sinus;
	func[6] = &differentiator::Cosinus;
	func[7] = &differentiator::Arcsin;
	func[8] = &differentiator::Logarithm;
	func[9] = &differentiator::Arccos;
	func[10] = &differentiator::Tg;
	func[11] = &differentiator::Ctg;

}

Obj* differentiator::Create(int type, const char c) const{
    Symbol* symbol = nullptr;
    return symbol->create(type, c);
}

Obj* differentiator::Create(float v)const {
    Number* num = nullptr;
    return num->create(Value, v);
}

differentiator::~differentiator() {

	delete [] diff_type;
	delete [] func;
}

void differentiator::ShowResult(){

	BinaryTree::create_static_objects();

	/*In my realization in constructor of tree */
	primary_expression = BinaryTree(GetG());

    primary_expression.SimplifyTree(primary_expression.get_root());

    differentiate_expression = BinaryTree(Differentiate(primary_expression.get_root()));

    differentiate_expression.SimplifyTree(differentiate_expression.get_root());

    LaTeX();

    BinaryTree::clean_static_storage();

}

void differentiator::LaTeX()const{
    std::ofstream output("new_tests/output.tex");
    if(output.fail()){
        std::cerr << "output.tex cannot be made or opened\n";
        return;
    }

    output << "\\documentclass[a4paper,12pt]{article}\n";

    output << "\\usepackage[T2A]{fontenc}\n"
              "\\usepackage[utf8]{inputenc}\n"
              "\\usepackage[english,russian]{babel}\n";

    output << "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n";

	output << "\\title{Утрем нос Стивену Вольфраму!}\n"
			  "\\date{\\today}\n";

    output << "\\begin{document}\n";
    output << "\\maketitle\n\n";

    output << "\\flushleft{\\textbf{\\large{Исходная функция :}}}\n"
              "\\center{\\fbox{$f(x) = ";

    primary_expression.dump_tree(primary_expression.get_root(), output);

    output << "$}}\\\\[1cm]\n";

    output << "\\flushleft{\\textbf{\\large{Производная исходной функции :}}}\\\\[2mm]"
              "\\center{\\fbox{$f'(x) = ";

    differentiate_expression.dump_tree(differentiate_expression.get_root(), output);
    output << "$}}\n\n\n";

    output << "\\end{document}\n";


    output.close();

}

Obj* differentiator::GetG() {


    auto root = GetE();

    assert(_cur == _str.GetStr() + _str.size());

    ++_cur;

    return root;

}

//We are going to work with float values

Obj* differentiator::GetN() {
    float res_value{0};

    float after_dot{1};

    const char* save_cur = _cur;

    bool is_dot{false};

    while((*_cur >= '0' && *_cur <= '9') || *_cur == '.'){
        if(*_cur == '.'){
            ++_cur;
            is_dot = true;
            continue;
        }
        if(!is_dot)
            res_value = res_value * 10 + static_cast<float>(*_cur - '0');
        else{
            res_value += static_cast<float>(*_cur - '0') * 0.1f / after_dot;
            after_dot *= 10;
        }

        ++_cur;
    }

    if(save_cur == _cur)
        return nullptr;


    return Create(res_value);

}

Obj* differentiator::GetT(){
    Obj* pivotal = GetP();
    Obj* right_ch{nullptr};
    Obj* left_ch{nullptr};

    char symbol{'\0'};

    while(*_cur == '*' || *_cur == '/' || *_cur == '^'){
        symbol = *_cur;
        ++_cur;

        left_ch = pivotal;

        right_ch = GetP();

        pivotal = Create(Operator, symbol);

        (*pivotal)(left_ch, right_ch);
    }

    return pivotal;
}

Obj* differentiator::GetE(){

    Obj* pivotal = GetT();
    Obj* right_ch{nullptr};
    Obj* left_ch{nullptr};
    Obj* extra_obj{nullptr};

    char symbol{'\0'};


    while(*_cur == '-' || *_cur == '+'){
        symbol = *_cur;

        ++_cur;

        left_ch = pivotal;

        if(!left_ch) {
            extra_obj = GetP();
            left_ch = Create(Operator, symbol);
            (*left_ch)(nullptr, extra_obj);
            symbol = *_cur;
            ++_cur;

            if(_cur == _str.GetStr() + _str.GetNumBytes()){
                --_cur;
                pivotal = left_ch;
                return pivotal;

            }
        }

        right_ch = GetT();

        pivotal = Create(Operator, symbol);

        //this operator will make children for pivotal;
        (*pivotal)(left_ch, right_ch);

    }
    return pivotal;
}

Obj* differentiator::GetP(){
    Obj* pivotal = nullptr;

    if(*_cur == '('){
        ++_cur;
        pivotal = GetE();
        assert(*_cur == ')');
        ++_cur;
        return pivotal;
    }

    Obj* unknown = GetU();
    Obj* function = nullptr;

    if(!unknown){
        function = GetF();
        if(!function){
            pivotal = GetN();
            return pivotal;
        }
		return function;
    }
    else
        return unknown;
}

Obj* differentiator::GetF(){

    Obj* pivotal{nullptr};
    Obj* right_ch{nullptr};
    Obj* left_ch{nullptr};

    if(*_cur == func(SIN) || *_cur == func(COS) || *_cur == func(ARCCOS) || *_cur == func(ARCSIN)
    || *_cur == func(TG) || *_cur == func(CTG)){

        pivotal = Create(Func, *_cur);
        ++_cur;
        right_ch = GetP();
        (*pivotal)(left_ch, right_ch);
        return pivotal;
    }

    else if (*_cur == func(LOG)){
        pivotal = Create(Func, *_cur);
        _cur += 2;
        left_ch = GetE();
        assert(*_cur == ',');
        ++_cur;
        right_ch = GetE();
        ++_cur;
        (*pivotal)(left_ch, right_ch);
        return pivotal;
    }

    return nullptr;

}

Obj* differentiator::GetU(){
    Obj* unknown = nullptr;

    if(*_cur == 'x' || (*_cur == 'e' && *(_cur - 2) == 'l')){
        unknown = Create(Unknown, *_cur);
        ++_cur;
        return unknown;
    }
    else
        return nullptr;
}

Obj* differentiator::Differentiate(Obj* root)const{
    if(!root)
        return nullptr;

    int type = root->get_type();
    assert(type >= 0 && type <= 3);
    return std::invoke(diff_type[type], *this, root);;
}

Obj* differentiator::Diff_Value(Obj* obj)const{
	return Create(0);
}

Obj* differentiator::Diff_Func(Obj* obj)const{
    auto name_f = static_cast<Symbol*>(obj);

    int id = name_f->get_id();
    assert(id >= 0 && id <= 11);
    return std::invoke(func[id], *this, name_f);
}

Obj* differentiator::Diff_Operator(Obj* obj)const{
    return Diff_Func(obj);
}

Obj* differentiator::Diff_Unknown(Obj* obj)const
{

	/*Here I must look at the case of Unknown [e]
	 * its derivative won't be just 1 but 0*/
	int id = static_cast<Symbol*>(obj)->get_id();
	assert(id == Variable || id == Exponenta);
	return (id == Variable) ? Create(1) : Create(0);
}

Obj* differentiator::Multiplication(Obj *obj)const {
    return MakeOperatorForMulDiv(obj, '+');
}

Obj* differentiator::MakeOperatorForMulDiv(Obj* obj, const char link)const{

    auto op = Create(Operator, link);

    auto left_diff = Differentiate(obj->get_left());
    auto right_diff = Differentiate(obj->get_right());

    auto left_multiplication = Create(Operator, '*');
    auto right_multiplication = Create(Operator, '*');

    auto left_copy = make_copy(obj->get_left());
    auto right_copy = make_copy(obj->get_right());

    (*left_multiplication)(left_diff, right_copy);
    (*right_multiplication)(left_copy, right_diff);
    (*op)(left_multiplication, right_multiplication);

    return op;

}

Obj* differentiator::Division(Obj *obj) const{
    auto pivotal = Create(Operator, '/');
    auto left_ch = MakeOperatorForMulDiv(obj, '-');
    auto right_ch = Create(Operator, '^');
    (*right_ch)(make_copy(obj->get_right()), Create(2));
    (*pivotal)(left_ch, right_ch);
    return pivotal;
}

Obj* differentiator::Exponentiation(Obj *obj) const{
    auto pivotal = Create(Operator, '*');

    //this logarithm will be the left child of multiplication operator root
    auto logarithm = Create(Func, 'l');
    auto left_ch = make_copy(obj);

    auto copy_left_child_obj = make_copy(obj->get_left());
    (*logarithm)(Create(Unknown, 'e'), copy_left_child_obj);
    auto multip = Create(Operator, '*');
    (*multip)(make_copy(obj->get_right()), logarithm);

    auto right_ch = Differentiate(multip);

    (*pivotal)(left_ch, right_ch);
    multip->remove(multip);

    return pivotal;
}

Obj* differentiator::Minus(Obj* obj)const{
    return PlusMinusDiff(obj, '-');
}

Obj* differentiator::Plus(Obj* obj)const{
   return PlusMinusDiff(obj, '+');
}

Obj* differentiator::PlusMinusDiff(Obj* obj, const char type)const{
    auto op = Create(Operator, type);

    auto left_ch = Differentiate(obj->get_left());
    auto right_ch = Differentiate(obj->get_right());

    (*op)(left_ch, right_ch);

    return op;
}

Obj* differentiator::Sinus(Obj* obj)const{

    auto left_ch = make_copy(obj);

    static_cast<Symbol*>(left_ch)->set_name('c');

    auto right_ch = Differentiate(obj->get_right());

    auto pivotal = Create(Operator, '*');

    (*pivotal)(left_ch, right_ch);

    return pivotal;
}

Obj* differentiator::make_copy(Obj *obj) const {
    if(!obj)
        return nullptr;

    if(!obj->get_right() && !obj->get_left()){
        auto pivotal = obj->copy();
        return pivotal;
    }

    auto left_ch = make_copy(obj->get_left());
    auto op = static_cast<Symbol*>(obj);
    auto pivotal = Create(op->get_type(), op->get_name());
    auto right_ch = make_copy(obj->get_right());
    (*pivotal)(left_ch, right_ch);
    return pivotal;
}

Obj* differentiator::Cosinus(Obj* obj)const{
    auto right_ch = Differentiate(obj->get_right());
    auto left_ch = Create(Operator, '-');
    auto cope_cos_f = make_copy(obj);
    static_cast<Symbol*>(cope_cos_f)->set_name('s');
    (*left_ch)(nullptr, cope_cos_f);
    Obj* pivotal = Create(Operator, '*');
    (*pivotal)(left_ch, right_ch);
    return pivotal;
}

Obj* differentiator::Logarithm(Obj* obj)const{
    if(obj->get_left()->get_type() == Unknown &&
    static_cast<Symbol*>(obj->get_left())->get_name() == 'e') {
        auto pivotal = Create(Operator, '/');
        (*pivotal)(Differentiate(obj->get_right()), make_copy(obj->get_right()));
        return pivotal;
    }
    else{
    	auto pivotal = Create(Operator, '/');
        auto left_ln = Create(Func, 'l');
        auto right_ln = Create(Func, 'l');
        (*left_ln)(Create(Unknown, 'e'), make_copy(obj->get_right()));
        (*right_ln)(Create(Unknown, 'e'), make_copy(obj->get_left()));
        (*pivotal)(left_ln, right_ln);
        auto diff_log = Differentiate(pivotal);
        pivotal->remove(pivotal);
        return diff_log;
    }

}

Obj* differentiator::Arcsin(Obj* obj)const{
    auto pivotal = Create(Operator, '/');
    auto left_ch = Differentiate(obj->get_right());
    auto right_ch = Create(Operator, '^');
    auto copy_func_expression = make_copy(obj->get_right());



    auto last =  (*Create(Operator, '/'))(Create(1), Create(2));
    auto mid = (*Create(Operator, '^'))(copy_func_expression, Create(2));

    (*right_ch)((*Create(Operator, '-'))(Create(1), mid ), last);

    return (*pivotal)(left_ch, right_ch);
}

Obj* differentiator::Arccos(Obj* obj)const{
    auto pivotal = Create(Operator, '-');
    return (*pivotal)(nullptr, Arcsin(obj));
}

Obj* differentiator::Tg(Obj* obj)const{
	auto pivotal = Create(Operator, '/');
    auto diff_func = Differentiate(obj->get_right());
    auto copy_func = make_copy(obj->get_right());
    auto denominator = Create(Operator, '^');
    auto left = (*Create(Func, func(COS)))(nullptr, copy_func);
    (*denominator)(left, Create(2));
    return (*pivotal)(diff_func, denominator);
}

Obj* differentiator::Ctg(Obj *obj) const{
    auto pivotal = Create(Operator, '-');
    auto right_ch = Create(Operator, '/');
    auto diff_func = Differentiate(obj->get_right());
    auto copy_func = make_copy(obj->get_right());

    auto left = (*Create(Func, func(SIN)))(nullptr, copy_func);
    auto right = (*Create(Operator, '^'))(left, Create(2));
    (*right_ch)(diff_func, right);
    return (*pivotal)(nullptr, right_ch);
}