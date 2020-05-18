//
// Created by denis on 22.08.19.
//

#include "Symbol.hpp"

void Symbol::deduce_id(const char c)
{
	switch (c)
	{
	case '+' :
		id_ = Plus;
		break;
	case '-' :
		id_ = Minus;
		break;
	case '*' :
		id_ = Multip;
		break;
	case '/' :
		id_ = Division;
		break;
	case '^' :
		id_ = Exponen;
		break;
	case 's' :
		id_ = Sinus;
		break;
	case 'c' :
		id_ = Cosine;
		break;
	case 'n' :
		id_ = Arcsine;
		break;
	case 'l' :
		id_ = Log;
		break;
	case 'a' :
		id_ = Arccosine;
		break;
	case 't' :
		id_ = Tg;
		break;
	case 'g' :
		id_ = Ctg;
		break;
	case 'x' :
		id_ = Variable;
		break;
	default :
		std::cerr << "ERROR !!!! Unrecognized symbol : " << name_ << std::endl;
		throw std::invalid_argument("Constructor cannot generate an object "
									"of Symbol Token due to unrecognized Token");
	}
}

Symbol::Symbol(int code, const char name) : Obj(code, nullptr, nullptr, nullptr), name_(name)
{
	deduce_id(name_);
}

Symbol* Symbol::create(int code, char c)const {
    Symbol* sym = s_traits::allocate(Symbol::alloc_, 1);
    s_traits::construct(Symbol::alloc_, sym, code, c);
    return sym;
}

Number* Symbol::calc(float x, float y)const{
    Number* val = nullptr;
    switch(name_) {

        case '+' : {
            return (val->create(Value, y + x));
        }

        case '-' : {
            return (val->create(Value, y - x));
        }

        case '*' : {
            return (val->create(Value, y * x));
        }

        case '/': {
            return (val->create(Value, y / x));
        }

        case '^': {
            return (val->create(Value, pow(y,x)));
        }
        default:
            return val;
    }
}

Obj* Symbol::copy() const {
    Symbol* copy_sym = s_traits::allocate(Symbol::alloc_, 1);
    s_traits::construct(Symbol::alloc_, copy_sym, type_, name_);
    return copy_sym;

}

void Symbol::set_name(const char c) {
	name_ = c;
	deduce_id(name_);
}

Symbol::s_alloc Symbol::alloc_ = Symbol::s_alloc();