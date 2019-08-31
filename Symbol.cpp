//
// Created by denis on 22.08.19.
//

#include "Symbol.hpp"


Symbol::Symbol(int code, const char name) : Obj(code, nullptr, nullptr, nullptr), name_(name) {}


Symbol* Symbol::create(int code, char c)const {
    Symbol* sym = s_traits::allocate(Symbol::alloc_, 1);
    s_traits::construct(Symbol::alloc_, sym, code, c);
    return sym;
}

char Symbol::get_name() const {
    return name_;
}

void Symbol::print_info() const {
    std::cout << name_;
}



Number* Symbol::calc(float x, float y)const{
    Number* val = nullptr;
    switch(name_) {

        case '+' : {
            return (val->create(type(Value), y + x));
        }

        case '-' : {
            return (val->create(type(Value), y - x));
        }

        case '*' : {
            return (val->create(type(Value), y * x));
        }

        case '/': {
            return (val->create(type(Value), y / x));
        }

        case '^': {
            return (val->create(type(Value), pow(y,x)));
        }
        default:
            return val;
    }
}


Symbol::~Symbol(){
    if(right_)
        right_ = nullptr;
    if(left_)
        left_ = nullptr;
    if(parent_)
        parent_ = nullptr;
    name_ = '\0';
}

Obj* Symbol::copy() const {
    Symbol* copy_sym = s_traits::allocate(Symbol::alloc_, 1);
    s_traits::construct(Symbol::alloc_, copy_sym, type_, name_);
    return copy_sym;

}

void Symbol::set_name(const char c) {
    name_ = c;
}


Symbol::s_alloc Symbol::alloc_ = Symbol::s_alloc();