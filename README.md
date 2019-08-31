![text](https://github.com/DenisEvteev/differentiator/blob/master/picture/diff_image.svg)

This is my pivotal project of the summer.
It took me about ten days to write it

----------------------------------------
In input my program gets string with math expression and at this moment it must 
have the definite form which contain such tokens :
* float values
* unknown variables (only 'x', 'y', 'z')
* functions (sin -> 's', cos -> 'c', tg -> 't', ctg -> 'g', arcsin -> 'n', 
arccos -> 'a', log -> 'l')
* binary operators ('-', '+', '*', '/', '^')

The math expression cannot has any spaces

For example : 
---------------------------------


if you want to get derivative from function `f(x) = sin(x)*arcsin(x^2)`
in this syntax you must write like `s(x)*n(x^2)`
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


And for the future of the project TODO list :
-------------------------------------------

* [ ] Examine the input expression for errors
* [ ] Convert input expression with correct math form of functions to tokens name (sin -> 's', arccos -> 'c')
* [ ] Print exponent in right position for function 
* [ ] To make it work in browser
-----------------------------------------------------

This list shows imperfections of my differentiator
 




