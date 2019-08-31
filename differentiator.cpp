//
// Created by denis on 21.08.19.
//

#include "differentiator.hpp"


differentiator::differentiator(){
    std::cout << "Enter the expresion : ";
    std::cin >> _str;

    if(_str.size() != _str.GetNumBytes() - 1){
        std::cout << "Bad expression! The program will over!" << std::endl;
        return;
    }
    _cur = _str.GetStr();

    primary_expression = nullptr;
    differentiate_expression = nullptr;

    diff_type = f_traits::allocate(differentiator::alloc, SIZE_ARRAY_FOR_STR);
    f_traits::construct(differentiator::alloc, diff_type, &differentiator::Diff_Value);
    f_traits::construct(differentiator::alloc, diff_type + 1, &differentiator::Diff_Operator);
    f_traits::construct(differentiator::alloc, diff_type + 2, &differentiator::Diff_Func);
    f_traits::construct(differentiator::alloc, diff_type + 3, &differentiator::Diff_Unknown);


    //operators
    funcs['-'] = &differentiator::Minus;
    funcs['+'] = &differentiator::Plus;
    funcs['*'] = &differentiator::Multiplication;
    funcs['/'] = &differentiator::Division;
    funcs['^'] = &differentiator::Exponentiation;

    //functions
    funcs['s'] = &differentiator::Sinus;
    funcs['c'] = &differentiator::Cosinus;
    funcs['l'] = &differentiator::Logarithm;
    funcs['n'] = &differentiator::Arcsin;
    funcs['a'] = &differentiator::Arccos;
    funcs['t'] = &differentiator::Tg;
    funcs['g'] = &differentiator::Ctg;

}

Obj* differentiator::Create(int type, const char c) const{
    Symbol* symbol = nullptr;

    return symbol->create(type, c);
}

Obj* differentiator::Create(float v)const {
    Number* num = nullptr;

    return num->create(type(Value), v);
}

differentiator::alloc_ptr_f differentiator::alloc = differentiator::alloc_ptr_f();

differentiator::~differentiator() {
    for(int i = 0; i < SIZE_ARRAY_FOR_STR; ++i)
        f_traits::destroy(differentiator::alloc, diff_type + i);

    f_traits::deallocate(differentiator::alloc, diff_type, SIZE_ARRAY_FOR_STR);

    diff_type = nullptr;
    _cur = nullptr;
    primary_expression = nullptr;
    differentiate_expression = nullptr;

}

void differentiator::CreatePrimaryTree() {
    primary_expression = new BinaryTree(GetG());
}

void differentiator::CreateDiffTree(){
    differentiate_expression = new BinaryTree(Differentiate(primary_expression->get_root()));
}

void differentiator::ShowResult(){

    CreatePrimaryTree();
    primary_expression->SimplifyTree(primary_expression->get_root());

    CreateDiffTree();
    differentiate_expression->SimplifyTree(differentiate_expression->get_root());

    LaTeX();

    primary_expression->clear(primary_expression->get_root());

    differentiate_expression->clear(differentiate_expression->get_root());

    delete differentiate_expression;

    delete primary_expression;
}

void differentiator::LaTeX()const{
    std::ofstream output("/home/denis/CLionProjects/Parce_Calculator/res/output.tex");
    if(!output){
        std::cerr << "output.tex cannot be made or opened\n";
        return;
    }

    output << "\\documentclass[a4paper,12pt]{article}\n";

    output << "\\usepackage[T2A]{fontenc}\n"
              "\\usepackage[utf8]{inputenc}\n"
              "\\usepackage[english,russian]{babel}\n";

    output << "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n";

    output << "\\title{Дифференциатор с выводом производной в \\LaTeX}\n"
              "\\author{Евтеев Денис}"
              "\\date{\\today}\n";

    output << "\\begin{document}\n";
    output << "\\maketitle\n\n";

    output << "\\flushleft{\\textbf{\\large{Исходная функция :}}}\n"
              "\\center{\\fbox{$f(x) = ";

    primary_expression->dump_tree(primary_expression->get_root(), output);

    output << "$}}\\\\[1cm]\n";

    output << "\\flushleft{\\textbf{\\large{Производная исходной функции :}}}\\\\[2mm]"
              "\\center{\\fbox{$f'(x) = ";

    differentiate_expression->dump_tree(differentiate_expression->get_root(), output);
    output << "$}}\n\n\n";

    output << "\\end{document}\n";


    output.close();

}

Obj* differentiator::GetG() {


    Obj* root = GetE();

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

        pivotal = Create(type(Operator), symbol);

        (*pivotal)(left_ch, right_ch);
    }

    return pivotal;
}



//all data in _str is very correct !!!

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
            left_ch = Create(type(Operator), symbol);
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

        pivotal = Create(type(Operator), symbol);

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
    Obj* func = nullptr;

    if(!unknown){
        func = GetF();
        if(!func){
            pivotal = GetN();
            return pivotal;
        }
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

        pivotal = Create(type(Func), *_cur);
        ++_cur;
        right_ch = GetP();
        (*pivotal)(left_ch, right_ch);
        return pivotal;
    }

    else if (*_cur == func(LOG)){
        pivotal = Create(type(Func), *_cur);
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

    if(*_cur == 'x' || *_cur == 'y' || *_cur == 'z' || (*_cur == 'e' && *(_cur - 2) == 'l')){
        unknown = Create(type(Unknown), *_cur);
        ++_cur;
        return unknown;
    }
    else
        return nullptr;
}

Obj* differentiator::Differentiate(Obj* root)const{
    if(!root)
        return nullptr;

    Obj* diff_root_ = (this->*diff_type[root->get_type()])(root);
    
    return diff_root_;
}

Obj* differentiator::Diff_Value(Obj* obj)const{

    return Create(0);
}

Obj* differentiator::Diff_Func(Obj* obj)const{
    auto name_f = dynamic_cast<Symbol*>(obj);
    try{
        return (this->*funcs.at(name_f->get_name()))(name_f);
    }catch(const std::out_of_range& ex){
        std::cout << ex.what() << '\n';
        std::cout << "__ERROR__ at line :" << __LINE__ << std::endl;
        std::cout << "Function : " << __PRETTY_FUNCTION__ << std::endl;
        return nullptr;
    }
}

Obj* differentiator::Diff_Operator(Obj* obj)const{
    return Diff_Func(obj);
}

Obj* differentiator::Diff_Unknown(Obj* obj)const{

    return Create(1);
}

Obj* differentiator::Multiplication(Obj *obj)const {
    return MakeOperatorForMulDiv(obj, '+');
}

Obj* differentiator::MakeOperatorForMulDiv(Obj* obj, const char link)const{

    Obj* op = Create(type(Operator), link);

    Obj* left_diff = Differentiate(obj->get_left());
    Obj* right_diff = Differentiate(obj->get_right());

    Obj* left_multiplication = Create(type(Operator), '*');
    Obj* right_multiplication = Create(type(Operator), '*');

    Obj* left_copy = make_copy(obj->get_left());
    Obj* right_copy = make_copy(obj->get_right());

    (*left_multiplication)(left_diff, right_copy);
    (*right_multiplication)(left_copy, right_diff);
    (*op)(left_multiplication, right_multiplication);

    return op;

}

Obj* differentiator::Division(Obj *obj) const{
    Obj* pivotal = Create(type(Operator), '/');
    Obj* left_ch = MakeOperatorForMulDiv(obj, '-');
    Obj* right_ch = Create(type(Operator), '^');
    (*right_ch)(make_copy(obj->get_right()), Create(2));
    (*pivotal)(left_ch, right_ch);
    return pivotal;
}

Obj* differentiator::Exponentiation(Obj *obj) const{
    Obj* pivotal = Create(type(Operator), '*');


    //this logarithm will be the left child of multiplication operator root
    Obj* logarithm = Create(type(Func), 'l');


    Obj* left_ch = make_copy(obj);

    Obj* copy_left_child_obj = make_copy(obj->get_left());
    (*logarithm)(Create(type(Unknown), 'e'), copy_left_child_obj);
    Obj* multip = Create(type(Operator), '*');
    (*multip)(make_copy(obj->get_right()), logarithm);

    Obj* right_ch = Differentiate(multip);

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
    Obj* op = Create(type(Operator), type);

    Obj* left_ch = Differentiate(obj->get_left());
    Obj* right_ch = Differentiate(obj->get_right());

    (*op)(left_ch, right_ch);

    return op;
}

Obj* differentiator::Sinus(Obj* obj)const{

    Obj* left_ch = make_copy(obj);

    dynamic_cast<Symbol*>(left_ch)->set_name('c');

    Obj* right_ch = Differentiate(obj->get_right());

    Obj* pivotal = Create(type(Operator), '*');

    (*pivotal)(left_ch, right_ch);

    return pivotal;
}

Obj* differentiator::make_copy(Obj *obj) const {
    if(!obj)
        return nullptr;

    if(!obj->get_right() && !obj->get_left()){
        Obj* pivotal = obj->copy();
        return pivotal;
    }

    Obj* left_ch = make_copy(obj->get_left());
    Symbol* op = dynamic_cast<Symbol*>(obj);
    Obj* pivotal = Create(op->get_type(), op->get_name());
    Obj* right_ch = make_copy(obj->get_right());
    (*pivotal)(left_ch, right_ch);
    return pivotal;
}

Obj* differentiator::Cosinus(Obj* obj)const{
    Obj* right_ch = Differentiate(obj->get_right());
    Obj* left_ch = Create(type(Operator), '-');
    Obj* cope_cos_f = make_copy(obj);
    dynamic_cast<Symbol*>(cope_cos_f)->set_name('s');
    (*left_ch)(nullptr, cope_cos_f);
    Obj* pivotal = Create(type(Operator), '*');
    (*pivotal)(left_ch, right_ch);
    return pivotal;
}

Obj* differentiator::Logarithm(Obj* obj)const{
    if(obj->get_left()->get_type() == type(Unknown) &&
    dynamic_cast<Symbol*>(obj->get_left())->get_name() == 'e') {
        Obj *pivotal = Create(type(Operator), '/');
        (*pivotal)(Differentiate(obj->get_right()), make_copy(obj->get_right()));
        return pivotal;
    }
    else{
        Obj* pivotal = Create(type(Operator), '/');
        Obj* left_ln = Create(type(Func), 'l');
        Obj* right_ln = Create(type(Func), 'l');
        (*left_ln)(Create(type(Unknown), 'e'), make_copy(obj->get_right()));
        (*right_ln)(Create(type(Unknown), 'e'), make_copy(obj->get_left()));
        (*pivotal)(left_ln, right_ln);
        Obj* diff_log = Differentiate(pivotal);
        pivotal->remove(pivotal);
        return diff_log;
    }

}

Obj* differentiator::Arcsin(Obj* obj)const{
    Obj* pivotal = Create(type(Operator), '/');
    Obj* left_ch = Differentiate(obj->get_right());
    Obj* right_ch = Create(type(Operator), '^');
    Obj* copy_func_expression = make_copy(obj->get_right());

    //here i've changed the return type of operator () in Obj class


    //It's a very strange syntax, but i liked it a bit (it's better to understand it than eating salt)

    (*right_ch)((*Create(type(Operator), '-'))(Create(1),
            (*Create(type(Operator), '^'))(copy_func_expression, Create(2)) ),
            (*Create(type(Operator), '/'))(Create(1), Create(2)));

    return (*pivotal)(left_ch, right_ch);
}

Obj* differentiator::Arccos(Obj* obj)const{
    Obj* pivotal = Create(type(Operator), '-');
    return (*pivotal)(nullptr, Arcsin(obj));
}

Obj* differentiator::Tg(Obj* obj)const{
    Obj* pivotal = Create(type(Operator), '/');
    Obj* diff_func = Differentiate(obj->get_right());
    Obj* copy_func = make_copy(obj->get_right());
    Obj* denominator = Create(type(Operator), '^');
    (*denominator)((*Create(type(Func), func(COS)))(nullptr, copy_func), Create(2));
    return (*pivotal)(diff_func, denominator);
}

Obj* differentiator::Ctg(Obj *obj) const{
    Obj* pivotal = Create(type(Operator), '-');
    Obj* right_ch = Create(type(Operator), '/');
    Obj* diff_func = Differentiate(obj->get_right());
    Obj* copy_func = make_copy(obj->get_right());

    //the same
    (*right_ch)(diff_func, (*Create(type(Operator), '^'))((*Create(type(Func), func(SIN)))(nullptr, copy_func), Create(2)));
    return (*pivotal)(nullptr, right_ch);
}