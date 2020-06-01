//
// Created by denis on 21.08.19.
//

#ifndef PARCE_CALCULATOR_PARSER_HPP
#define PARCE_CALCULATOR_PARSER_HPP

#include "../String_Lib/String.hpp"
#include <cassert>
#include <utility>
#include "Number.hpp"
#include "Symbol.hpp"
#include "BinaryTree.hpp"

enum {
	SIZE_ARRAY_PTR_FUNC = 4,
	SIZE_ARRAY_EXACT_SYMBOL_DIFF = 12
};

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

class differentiator final
{
 public:
	template<class StringType>
	explicit differentiator(StringType&& str);
	differentiator(const differentiator& copy) = delete;
	differentiator& operator=(const differentiator& copy) = delete;
	void ShowResult();

 private:
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

	Obj* PlusMinusDiff(Obj* obj, const char type) const;

	Obj* Create(int type, const char c) const;
	Obj* Create(float v) const;

	void LaTeX() const;

 private :
	BinaryTree primary_expression;
	BinaryTree differentiate_expression;
	String _str;

	typedef Obj* (differentiator::*ptr_array_funcs)(Obj* obj) const;

	ptr_array_funcs diff_type[SIZE_ARRAY_PTR_FUNC] = { &differentiator::Diff_Value,
													   &differentiator::Diff_Operator,
													   &differentiator::Diff_Func,
													   &differentiator::Diff_Unknown };
	const char* _cur = nullptr;
	ptr_array_funcs func[SIZE_ARRAY_EXACT_SYMBOL_DIFF] = { &differentiator::Plus,
														   &differentiator::Minus,
														   &differentiator::Multiplication,
														   &differentiator::Division,
														   &differentiator::Exponentiation,
														   &differentiator::Sinus,
														   &differentiator::Cosinus,
														   &differentiator::Arcsin,
														   &differentiator::Logarithm,
														   &differentiator::Arccos,
														   &differentiator::Tg,
														   &differentiator::Ctg };

};

template<class StringType>
differentiator::differentiator(StringType&& str) : _str(std::forward<StringType>(str))
{
	_cur = _str.GetStr();
}

#endif //PARCE_CALCULATOR_PARSER_HPP
