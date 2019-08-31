//
// Created by denis on 22.08.19.
//

#pragma once

#include "Obj.hpp"
#include "Number.hpp"
#include "Symbol.hpp"
#include "iostream"
#include "fstream"
#include <cstdio>
#include <map>


#define SIZE_ARRAY_PRINT 4

#define CHECK_ONE(op) (op->get_type() == type(Value)\
&& dynamic_cast<Number*>(op)->get_val() == 1)\


#define CHECK_NOL_CHILD(op) (op->get_type() == type(Value) && dynamic_cast<Number*>(op)->get_val() == 0)

class BinaryTree {
public:
    explicit BinaryTree(Obj* root);
    ~BinaryTree();

    BinaryTree(const BinaryTree& copy) = delete;
    BinaryTree&operator=(const BinaryTree& copy) = delete;
    BinaryTree() = delete;

    Obj* get_root()const;
    void set_root(Obj* root);

    void clear(Obj* root);

    Obj* Calculate(Obj* root);

    Obj* MakeCount(Obj* root);

    bool CheckParentPriority(Obj* obj)const;

    void SimplifyTree(Obj* obj);

    void dump_tree(Obj* root, std::ofstream& out)const;

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
    void PrintValue(std::ofstream& out, Obj* root)const;
    void PrintOperator(std::ofstream& out, Obj* root)const;
    void PrintFunc(std::ofstream& out, Obj* root)const;
    void PrintUnknown(std::ofstream& out, Obj* root)const;
private:

    Obj* __root;

    //________Pointer to array of functions for printing different types of nodes________//
    typedef void (BinaryTree::*ptr_print)(std::ofstream&, Obj*)const;
    ptr_print* func_print_type = nullptr;
    typedef TrackingAllocator<ptr_print> al_ptr_f;
    typedef std::allocator_traits<al_ptr_f> b_traits;
    static al_ptr_f alloc_print;
    //__________________________________________________________________________________//


    typedef Obj* (BinaryTree::*ptr_funcs)(Obj*);
    typedef std::map<char, const char*> map;

    typedef std::map<char, ptr_funcs> ar_simpl;

    static const map func_name;
    static const ar_simpl simpl_funcs;


};
