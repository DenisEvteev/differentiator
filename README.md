![text](https://github.com/DenisEvteev/differentiator/blob/master/picture/diff_image.svg)

The second name of this project is _**`Acram Alpha`**_. The reason for it is that this program was sent as
a task for entering the Acronis company as the base department in MIPT.
This is my pivotal project of the summer 2019.
It took me about ten days to write it.
The main thing of this project that there isn't any usage of STL library. A short version of String_Lib is used in here.
And all the containers were substituted with common arrays. ( It was a restriction from the Acronis company )

----------------------------------------
In input this program gets string with traditional mathematical notation 
taking into account the natural priority of operations and without extra brackets. For output the program 
gives a derivative of the input function dumping it into PDF-file format. 

The input this moment must have a strict representation. The rules for it as follows : 
1. Form of input functions : `sin()` `cos()` `tg()` `ctg()` `log(smth, smth)` `arcsin()` `arccos()` **without any spaces**
2. e ( aka exponent ) can be used only as a base for log() function
3. All the float values should be entered without spaces
4. ',' can be used only as a separator for logarithm function

Simple parser convert the input traditional math notation into a short one. It has the definite form which contain such tokens :
* float values
* unknown variables (only 'x', 'e')
* functions (sin -> 's', cos -> 'c', tg -> 't', ctg -> 'g', arcsin -> 'n', 
arccos -> 'a', log -> 'l')
* binary operators ('-', '+', '*', '/', '^')

The short math expression cannot has any spaces

-------------------------------------------------------------

Then the program will parse this expression and make binary tree of this tokens;

>And some words about type of nodes in the tree :
The base class is `Obj` -- contains the common data about each node such as : parent, left_ch, right_ch pointers
and the type of node.
One of derivative class is `Number` -- contains the extra information for nodes which 
represent us the float values only.
Another derivative class is `Symbol` -- contains the extra information for nodes of functions,
>operators, unknown names;


After it the program will create the derivative_tree using the certain rules for different nodes

>To say the truth the design of program is using array of pointers to member functions for dealing 
>with different type of nodes by calling the match function based on its type

After it differentiator calls the function for simplifying tree and deal with situations such as :
`(left_ch * 0) -> 0`, `(left_ch * 1) -> left_ch`, `(0 / right_ch) -> 0`,
`(1 ^ right_ch) -> 1` etc.

The last function which differentiator calls is LaTeX() - which dump all the results in Latex script;

----------------------------------------------------------------------------

And finally some examples of working differentiator :
-------------------------------------------------

1. [func_1](https://github.com/DenisEvteev/differentiator/blob/master/out/func_1.pdf)

2. [func_2](https://github.com/DenisEvteev/differentiator/blob/master/out/func_2.pdf)

3. [func_3](https://github.com/DenisEvteev/differentiator/blob/master/out/func_3.pdf)


Some updated examples in the form the Acronis company wanted me :
---------------------------------------------------
1. [all_functions](https://github.com/DenisEvteev/differentiator/blob/master/new_tests/all_functions.pdf)

2. [smth_minus_smth_simplification](https://github.com/DenisEvteev/differentiator/blob/master/new_tests/smth_minus_smth.pdf)

And for the future of the project TODO list :
-------------------------------------------

* [x] Examine the input expression for errors
* [x] Convert input expression with correct math form of functions to tokens name (sin -> 's', arccos -> 'c')
* [ ] Print exponent in right position for function 
* [ ] To make it work in browser
 




