//
// Created by denis on 22.08.19.
//

#pragma once

#include "Obj.hpp"

class Number : public Obj{

    typedef TrackingAllocator<Number> n_alloc;
    typedef std::allocator_traits<n_alloc> n_traits;

public:
    Number(int code, float val);

    //move constructor
    Number(Number&& num)noexcept;

    //move assignment operator
	Number& operator=(Number&& num) noexcept;
	bool operator==(const Obj& obj) const override;

    Number(const Number& num) = delete;

    Number* create(int type, float val = 0)const;
    Obj* create(Obj* obj);

    void print_info()const;

    Obj* copy()const override;

    //-------getters-----------//
    float get_val()const;

    static n_alloc alloc_;


private:
    float val_;
};

