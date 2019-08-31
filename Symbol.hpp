//
// Created by denis on 22.08.19.
//

#ifndef PARCE_CALCULATOR_SYMBOL_HPP
#define PARCE_CALCULATOR_SYMBOL_HPP

#include "Obj.hpp"
#include "Number.hpp"
#include <cmath>


class Symbol : public Obj{


    typedef TrackingAllocator<Symbol> s_alloc;
    typedef std::allocator_traits<s_alloc> s_traits;

public:
    Symbol(int code, const char name);

    ~Symbol()override;

    Obj* copy()const override;

    Symbol* create(int code, char c = '\0')const;

    char get_name()const;

    void print_info()const;

    Number* calc(float x, float y)const;

    static s_alloc alloc_;

    void set_name(const char c);

private:
    char name_;

};


#endif //PARCE_CALCULATOR_SYMBOL_HPP
