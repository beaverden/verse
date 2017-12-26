# Verse 
A semi-statically typed, semi-poetic, semi-language made by @beaverden and @ClaudiuIoanMaftei
as a final assignment for the [Formal Languages, Automata and Compilers](https://profs.info.uaic.ro/~otto/lfac.html) course.

It was made using LEX, YACC and works only thanks to unknown magic unicorn power.

  * Please note that this language is our first experience in parser and interpreter design and is not intended for any practical use.
  * Also consult the list of bugs (I wish there wasn't any).
  * Any string inside "[  ]" is meant to be taken as a regular expression. [type] means something like int or string and [identifier] would mean "_HolyHandGrenade" or "var".
  * Language lacks some important features that are not implemented now.

## Primitive types
There are four primitive types, any other types are composed of them.
  1. **Integer** (whole, discrete, full) 
  2. **Boolean** (doubting)
  3. **String**  (saying)
  4. **List**   (book)

Their default values are 0, untruth, "", book<> respectively.

## Variable declaration
Variables are only declared using `enter`.
```
enter whole var
enter saying name
enter saying Jesus
```
Constant variables are denoted with `fixed` keyword before type
```
enter fixed whole i
```
^ Which will be a interesting tactical move, because the constant variable has no value and it will be never assigned.

We can assign a value when declaring by using `telling`
```
enter fixed whole i telling 5
enter saying str telling "hello"
```
Wiil be equivallent to `const int i = 5` and `char* str = "hello"`

## Variable assignment and changing
There are ~just a few stupid~ multiple ways to change a variable.
  1. `let [id] ascend` `i++` and  for an integer type
  2. `let [id] descend` `i--`
  3. `let [id] be [expression]` `i = expression`
  4. `let there be [expression] added to [id]` `i += expr`
  5. `let there be [expression] conspired from [id]` `i -= expr`
  6. `let there be [expression] dividing [id]` `i /= expr`
  7. `let there be [expression] empowering to [id]` `i *= expr`
  8. `let there be [expression] reminding [id]` `i %= expr`

## Expression
A expression can be:
  1. Function call
  2. Number, string, list (book<1, 2, 3>), boolean value
  3. Variable identifiers and chained structure addressing (`from`)
  4. One of the predefined functions (if the return type is correct in the context of the expresssion)
  5. none (number 0 actually)
  6. one (number 1 actually)
  7. Expression enclosed in '< >'

Any of the above in a correct combination with operators:
  1. `adding` (+)
  2. `conspiring` (-)
  3. `dividing` (/)
  4. `reminding` (%)
  5. `empowering` (*)
  6. `is` (==)
  7. `is not` (!=)
  8. `lower than` (<)
  9. `greater than` (>)
  10. `up to lower than` (<=)
  11. `down to greater than` (>=)

What is a "correct combination"?:
  1. `adding` (numbers, strings, lists)
  2. `conspiring` (numbers)
  3. `dividing` (numbers)
  4. `reminding` (numbers)
  5. `empowering` (numbers, numbers+strings with give a pythonic result)
  6. Comparison operators and only be used with numeric types

Anything else can and will (I hope so) generate a runtime error

Some examples of correct expressions:

```
1 adding 5 // 6
2 empowering <5 adding 1> // 12
book<1, 2, 3> // list containing 1, 2, 3
book<1, 2, 3> + book<3, 2, 1> // book<1, 2, 3, 3, 2, 1>
"hey" + ", mate" // "hey, mate"
"hey" empowering 5 // "heyheyheyheyhey"
5 empowering "hey" // "heyheyheyheyhey"
untruth is indeed // untruth
```



## Complex types
A "struct" type declaration is done using **proclaim**
```
proclaim <type_name>
<
	[type] fixed? [identifier],
	...,
	[type] fixed? [identifier]
>
```
Members of a struct can be accessed with `[identifier] from [var_identifier]`

In this example, member `a` is accessed from `p` with `a from p`. `from` can be chained if more layers of complex types are present, like `a from b from c from var`.

```
proclaim pair
<
	whole a,
	whole b
>
enter pair p
quote<a from p>
quote<b from p>
```

## Predefined functions
`quote<[expr]>` Output an expression (`quote<1 adding 2>`)

`summon<[var]>` Inputs a value of the same type as var into var

`verbose<[expr]>` Simmilar to str() in python

`numeric<[expr]>` Simmilar to int() in python

## IF Statement
```
ask whether <[expression]> then
<
	...
>
otherwise
<
	...
>
```

## While
**To exit or continue any loop use `break free` and `continue`**
```
since [expression] 
<
	...
>
```

## For
```
for [assignment], since [expression], [assignment]
<
	...
>
```

## Function declaration
```
describe [func_id] as [type] adventure of < [type] [id], [type] [id], ... >
<
	...
	exeunt [expr]! // Return statement. Should be the same as function return type
>
```

## Lists
  1. Adding to back: `write chapter [expression] to [id]`
  2. Adding to position `write [expression] as chapter [expression] to [id]`
  3. Removing from position `erase chapter [expression] of [id]`
  4. Indexing `chapter [expression] of [id]` 
(`chapter 5 of arr` will return the element at position 5, starting from 0)

## Comments
There are no comments. Because f*ck you, that's why.

## Bugs and unimplemented stuff
  1. One should be able to assign a complex variable to another complex variable
  2. Implement &&, ||, ! for boolean expressions
  3. If you can declare a fixed variable in a struct, you should be able to assign it right away

If you made it to the end, take your cookie, you glorious reader!


