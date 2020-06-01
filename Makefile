CXX = g++
CXXFLAGS = -Wall -std=c++17 -O3


#All objective files in the project without external libraries [String_Lib]
OBJS = src/BinaryTree.o src/differentiator.o src/Obj.o src/Number.o src/Symbol.o main.o
#Objective file connected with a String_Lib
OBJ_STRING = String_Lib/String.o
OBJS += $(OBJ_STRING)

STRING_HEADER  = String_Lib/String.hpp
COMMON_HEADERS = src/Obj.hpp src/Symbol.hpp src/Number.hpp
BINARY_TREE_HEADERS := $(COMMON_HEADERS) src/BinaryTree.hpp $(STRING_HEADER)
ALLOCATOR = src/TrackingAllocator.hpp

.PHONY : all
all : AcramAlpha

AcramAlpha : $(OBJS)
	$(CXX) -o $@ $^

main.o : src/differentiator.hpp

src/BinaryTree.o : $(BINARY_TREE_HEADERS)

src/Obj.o : $(ALLOCATOR) src/Obj.hpp

src/Symbol.o : $(COMMON_HEADERS)

src/Number.o : src/Obj.hpp src/Number.hpp

src/differentiator.o : $(BINARY_TREE_HEADERS) src/differentiator.hpp

String_Lib/String.o : String_Lib/String.hpp

.PHONY : clean
clean :
	-rm src/*.o
	-rm String_Lib/*.o
	rm AcramAlpha