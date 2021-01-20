# GrLang
A really bad programming language.

# Introduction

This programming language is heavily inspired by C++, Haskell and Jai. It is an imperative static/strongly typed C-style language. Here is a brief list of features that are 
borrowed from the before-mentioned languages or conceptualized by me. 

*Directly copied from [Unofficial Jai Github](https://github.com/BSVino/JaiPrimer/blob/master/JaiPrimer.md)*:
- Arbitrary compile-time code execution – Any function of the program can be made to run at compile time with #run
- Syntax-facilitated code refactoring – The language syntax facilitates code reuse by making it easy to move code from local block → local function → global function
- Integrated build process – The build process and parameters are specified by the source code itself, for consistency
- Reflection and run-time type information – Static type information for every structure available at runtime

*Inspired by Haskell*:
- function composition
- functions as parameters

*My own ideas*:
- allow different files to be interpreted at runtime individually whilst the rest of the program is compiled, this would allow even faster iteration on non performance heavy tasks such as UI.
- alternative way of handling multiple return values using the `final` operator.

# Syntax

### A basic program

```
MAIN _ => code:Z {
    $ this is a comment
	print "Hello World!"
	final code = 0
}
```
- function signature: 
`MAIN` is the function name, "_" signifies no arguments being passed, `code` is the name of the return value, and `Z` signifies an integer value, which is the type of code.
- function body:
    - print is a basic IO function which outputs given structure to the console. In this case it is a string.
    - final signifies that the `code` value will be no longer modified, when all return values are finalized, the function returns. 

#### Program Output:
```
Hello World!
```

### Conditionals

#### If statements:

```
if $ some condition $ {
    $ some code
}
elif $ some different condition
    $ some different code
}
```
*Note: the `final` operator is not allowed inside conditionals*

```
final variable := 
    if $ condition 1 $ set "yes"
    elif $ condition 2 $ set "maybe"
    else set "no"
```
*Note: the else statement in value conditionals is mandatory*
```
switch x:
    is 1 do $ some code $
    is 5 do $ some code $
    is 9 do {
        $ some code
    }
    else do $ some code $
```

```
final value := switch x:
    is 1 set 42
    is 5 set 87
    is 9 set 49
    else set 68
```

(... to be continued ...)




# Build

LLVM v10.0.1 required.
