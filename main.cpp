#include <iostream>
#include "differentiator.hpp"
#include "/home/denis/CLionProjects/MyExceptions/MyException/My_Exception.hpp"


void MyNewHandler();

int main() {

    std::set_new_handler(MyNewHandler);


    try{

        differentiator parser;

        parser.ShowResult();

    }catch(My_Exception* ex){
        std::cout << ex->what() << std::endl;
        ex->dump_info_in_console();
        ex->clear();
    }
    return 0;

}

void MyNewHandler(){
    std::cout << "operator new cannot allocate more memory\n";
    std::abort();
}