//
// Created by denis on 22.08.19.
//

#ifndef PARCE_CALCULATOR_SYMBOL_HPP
#define PARCE_CALCULATOR_SYMBOL_HPP

#include "Obj.hpp"
#include "Number.hpp"
#include <cmath>
#include <iostream>

enum Index {
	Plus,  //0
	Minus,
	Multip,
	Division,
	Exponen,  //4
	Sinus, //5
	Cosine,
	Arcsine,
	Log,
	Arccosine,
	Tg,
	Ctg, //11
	Variable, //12
	Exponenta,          // this
	BOUND = Sinus
};

class Symbol : public Obj{


    typedef TrackingAllocator<Symbol> s_alloc;
    typedef std::allocator_traits<s_alloc> s_traits;

public:
	Symbol(int code, const char name);

	Obj* copy() const override;

	Symbol* create(int code, char c = '\0') const;

	char get_name() const
	{
		return name_;
	}
	int get_id() const
	{
		return id_;
	}

	void print_info() const
	{
		std::cout << name_;
	}

	bool operator==(const Obj& obj) const override;

	Number* calc(float x, float y) const;

	static s_alloc alloc_;  // the fact that this field is static make me feel much better!!!

	void set_name(const char c);

 private:
	void deduce_id(const char c);
	char name_;
    /*This index field is used to invoke a function via pointer-to-member
     * using array of pointers in the classes of high layer.
     * It was added to the implementation on the 18.05.2020 due to
     * this project was reorganized to reduce all the usage of STL library
     *
     * [ Notice ] : this value will be set directly in the constructor of Symbol object
     * via switch construction where the default value represents the case of error*/
    Index id_;

};


#endif //PARCE_CALCULATOR_SYMBOL_HPP
