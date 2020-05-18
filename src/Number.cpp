//
// Created by denis on 22.08.19.
//

#include "Number.hpp"


Number::Number(int code, float val) : Obj(code, nullptr, nullptr, nullptr), val_(val)
{}

Number::Number(Number&& num)noexcept : Obj(num.type_, num.parent_, num.right_, num.left_){

    val_= num.val_;

    //Destory the information in num
    num.val_ = 0;
    num.type_ = SALT;

    num.parent_ = nullptr;
    num.left_ = nullptr;
    num.right_ = nullptr;
}

Number& Number::operator=(Number &&num)noexcept {
    //Move data
    type_ = num.type_;
    parent_ = num.parent_;
    left_ = num.left_;
    right_ = num.right_;


    val_= num.val_;

    //Destory the information in num
    num.val_ = 0;
    num.type_ = SALT;

    num.parent_ = nullptr;
    num.left_ = nullptr;
    num.right_ = nullptr;
    return *this;
}

float Number::get_val() const {
    return val_;
}

Obj* Number::copy() const {
    Number* copy_val = n_traits::allocate(Number::alloc_, 1);
    n_traits::construct(Number::alloc_, copy_val, type_, val_);
    return copy_val;
}

void Number::print_info() const {
    std::cout << val_;
}

Obj* Number::create(Obj* obj){
    obj = n_traits::allocate(Number::alloc_, 1);
    auto num = dynamic_cast<Number*>(obj);
    if(num){
        n_traits::construct(Number::alloc_, num, std::move(*this));
        return num;
    }
	return nullptr;
}

Number* Number::create(int type, float val) const
{

	Number* ptr = n_traits::allocate(Number::alloc_, 1);
	n_traits::construct(Number::alloc_, ptr, type, val);
	return ptr;
}

bool Number::operator==(const Obj& obj) const
{
	auto rhs_ptr = dynamic_cast<const Number*>(&obj);
	return (rhs_ptr->type_ == type_ && rhs_ptr->val_ == val_);
}

Number::n_alloc Number::alloc_ = Number::n_alloc();

