//
// Created by denis on 22.08.19.
//

#ifndef PARCE_CALCULATOR_OBJ_HPP
#define PARCE_CALCULATOR_OBJ_HPP

#include "TrackingAllocator.hpp"
#include <iostream>

#define SALT -666

/*This is the base class for different types of nodes These are:
 * 1) values will contain float value                 : 1, 10.02, -19, -1002.4
 * 2) operators will contain char field with it value :   +, -, *, /, ^
 * 3) functions will contain char field with it name  :   s, c, n, l
 * 4) unknown will contain char symbols like          :   x, y, z*/

enum Token {
    Value,
    Operator,
    Func,
    Unknown
};


class Obj {


public:

    typedef TrackingAllocator<Obj> o_alloc;
    typedef std::allocator_traits<o_alloc> o_traits;

    Obj(int type, Obj* par, Obj* right, Obj* left);

    Obj(Obj&& obj) = delete;
    Obj(const Obj& obj) = delete;


    virtual Obj* operator()(Obj* left, Obj* right);

    //------------setters---------------------//
    void set_parent(Obj* par);
    void set_right(Obj* right);
    void set_left(Obj* left);
    //---------------------------------------//

    void MakeChild(Obj* child);

    virtual void print_info() const { std::cout << "Type of this object is : " << type_ << std::endl; }

    virtual Obj* copy() const {};
    virtual bool operator==(const Obj& obj) const;
	void remove(Obj* root);

    Obj* create(int type)const;




    //------------getters---------------------//
    int get_type()const;
    Obj* get_left()const;
    Obj* get_right()const;
    Obj* get_parent()const;
    //---------------------------------------//

    static o_alloc alloc;


protected:
    int type_;
    Obj* parent_;
    Obj* right_;
    Obj* left_;

};


#endif //PARCE_CALCULATOR_OBJ_HPP
