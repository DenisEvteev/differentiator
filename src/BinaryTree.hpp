//
// Created by denis on 22.08.19.
//

#pragma once

#include "Number.hpp"
#include "Symbol.hpp"
#include "../String_Lib/String.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <functional>

enum SIZE{
	SIZE_ARRAY_PRINT =  4,
	SIZE_ARRAY_SIMPLIFY = 5,
	SIZE_ARRAY_FUNCTIONS = 7
};


#define CHECK_ONE(op) (op->get_type() == Value && static_cast<Number*>(op)->get_val() == 1)

#define CHECK_NOL_CHILD(op) (op->get_type() == Value && static_cast<Number*>(op)->get_val() == 0)

class BinaryTree final
{
 public:
	explicit BinaryTree(Obj* root);
	~BinaryTree();
	BinaryTree& operator=(BinaryTree&& rhs) noexcept;
	BinaryTree(const BinaryTree& copy) = delete;
	BinaryTree& operator=(const BinaryTree& copy) = delete;
	BinaryTree() = default;

	Obj* get_root() const { return root_; }
	void set_root(Obj* root);

	void clear(Obj* root);

	Obj* Calculate(Obj* root);

	Obj* MakeCount(Obj* root);

	bool CheckParentPriority(Obj* obj) const;

	void SimplifyTree(Obj* obj);

	void dump_tree(Obj* root, std::ofstream& out) const;

	Obj* WorkWithRightOne(Obj* copy_func, Obj* root);

	Obj* MakeNumericalNode(Obj* root, float value);

	//-----------------Functions for simplifying operators-------------------------//
	Obj* SimplifyExponent(Obj* root);
	Obj* SimplifyPlus(Obj* root);
	Obj* SimplifyMinus(Obj* root);
	Obj* SimplifyMultiplication(Obj* root);
	Obj* SimplifyDivision(Obj* root);
	//----------------------------------------------------------------------------//

	//____________________Functions for printing different types__________________//
	void PrintValue(std::ofstream& out, Obj* root) const;
	void PrintOperator(std::ofstream& out, Obj* root) const;
	void PrintFunc(std::ofstream& out, Obj* root) const;
	void PrintUnknown(std::ofstream& out, Obj* root) const;

 private:

	Obj* root_ = nullptr;

	typedef void (BinaryTree::*ptr_print)(std::ofstream&, Obj*) const;
	static ptr_print printer[SIZE_ARRAY_PRINT];

	typedef Obj* (BinaryTree::*ptr_funcs)(Obj*);
	static ptr_funcs simplifier[SIZE_ARRAY_SIMPLIFY];
	/*static field to convert short math notation into
	 * full mathematical notation for dumping the right representation
	 * of functions into a file stream*/
	static String func_names[SIZE_ARRAY_FUNCTIONS];
};

