cmake_minimum_required(VERSION 3.14)
project(Parce_Calculator)

set(CMAKE_CXX_STANDARD 14)

set(STRING_LIB "/home/denis/CLionProjects/String_Lib/String.cpp")

set(Exceptions "/home/denis/CLionProjects/MyExceptions/MyException/My_Exception.cpp" Obj.cpp Obj.hpp Symbol.cpp Symbol.hpp BinaryTree.cpp BinaryTree.hpp)


set(Alloc "/home/denis/CLionProjects/vec_lib/TrackingAllocator.hpp")

add_executable(Parce_Calculator main.cpp differentiator.hpp differentiator.cpp ${STRING_LIB}  ${Exceptions}
        ${Alloc}  Number.cpp Number.hpp)