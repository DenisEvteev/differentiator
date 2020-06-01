#include <iostream>
#include "src/differentiator.hpp"
#include "String_Lib/String.hpp"


void MyNewHandler();



int main()
{

	std::set_new_handler(MyNewHandler);
	String traditional_math_notation;
	try
	{
		std::cin >> traditional_math_notation;
		if (!traditional_math_notation.eng())
		{
			std::cerr << "Error : Input string should contain only ASCII symbols\n";
			std::exit(EXIT_FAILURE);
		}
		// create the short mathematical notation of the input expression
		String short_math_notation = traditional_math_notation.ParseShortMathNotation();

#ifdef DEBUG
		std::cout << short_math_notation << std::endl;
#endif
		differentiator parser(std::move(short_math_notation));
		parser.ShowResult();

	}
	catch (const std::exception& excpt)
	{
		std::cerr << excpt.what() << std::endl;
	}

	return 0;

}

void MyNewHandler(){
    std::cerr << "operator new cannot allocate more memory\n";
    std::abort();
}