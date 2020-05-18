#include <iostream>
#include "src/differentiator.hpp"


void MyNewHandler();



int main() {

    std::set_new_handler(MyNewHandler);

    differentiator parser;

	parser.ShowResult();
    return 0;

}

void MyNewHandler(){
    std::cerr << "operator new cannot allocate more memory\n";
    std::abort();
}