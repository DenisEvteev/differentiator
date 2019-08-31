//
// Created by denis on 21.08.19.
//

#ifndef PARCE_CALCULATOR_PARSER_HPP
#define PARCE_CALCULATOR_PARSER_HPP

#include "/home/denis/CLionProjects/String_Lib/String.hpp"
#include <cassert>
#include "Number.hpp"
#include "Symbol.hpp"
#include "BinaryTree.hpp"

#include "Obj.hpp"
#include <map>

#define SIZE_ARRAY_PTR_FUNC 4

#include <fstream>

#define func(type) static_cast<const char>(FUNC::type)

enum class FUNC: const char{
    SIN = 's',
    COS = 'c',
    ARCSIN = 'n',
    ARCCOS = 'a',
    LOG = 'l',
    TG = 't',
    CTG = 'g'
};

class differentiator {
public:
    differentiator();
    differentiator(const differentiator& copy) = delete;
    differentiator&operator=(const differentiator& copy) = delete;
    void ShowResult();
    ~differentiator();


private:

    typedef Obj* (differentiator::*ptr_array_funcs)(Obj* obj)const;
    typedef TrackingAllocator<ptr_array_funcs> alloc_ptr_f;
    typedef std::allocator_traits<alloc_ptr_f> f_traits;

    ptr_array_funcs* diff_type = nullptr;

    static alloc_ptr_f alloc;

    void CreatePrimaryTree();
    void CreateDiffTree();


    String _str;
    const char* _cur = nullptr;
    Obj* GetG();
    Obj* GetN();
    Obj* GetE();
    Obj* GetT();
    Obj* GetP();
    Obj* GetF();
    Obj* GetU();     // get unknown

    Obj* Differentiate(Obj* root)const;
    Obj* make_copy(Obj* obj)const;

    //-------------------Functions for differentiating different types of tokens------------------//
    Obj* Diff_Value(Obj* obj)const;
    Obj* Diff_Func(Obj* obj)const;
    Obj* Diff_Unknown(Obj* obj)const;
    Obj* Diff_Operator(Obj* obj)const;
    //____________________________________________END________________________________________//

    Obj* Minus(Obj* obj)const;
    Obj* Plus(Obj* obj)const;
    Obj* Sinus(Obj* obj)const;
    Obj* Cosinus(Obj* obj)const;
    Obj* Logarithm(Obj* obj)const;
    Obj* Arcsin(Obj* obj)const;
    Obj* Arccos(Obj* obj)const;
    Obj* Tg(Obj* obj)const;
    Obj* Ctg(Obj* obj)const;
    Obj* Multiplication(Obj* obj)const;
    Obj* Division(Obj* obj)const;
    Obj* Exponentiation(Obj* obj)const;
    //________________________________________________________________________________________//

    Obj* MakeOperatorForMulDiv(Obj* obj, const char link)const;

    Obj* PlusMinusDiff(Obj* obj, const char type)const;

    Obj* Create(int type, const char c)const;
    Obj* Create(float v)const;

    void LaTeX()const;
    std::map<const char, ptr_array_funcs> funcs;

    BinaryTree* primary_expression;
    BinaryTree* differentiate_expression;

};


#endif //PARCE_CALCULATOR_PARSER_HPP
