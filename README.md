**Note: This documentation is unfinished.**

# Traverse

Traverse is a stack-based, two-dimensional programming language, inspired by [Porth](https://gitlab.com/tsoding/porth).

## Goals

-   [ ] Turing Complete
-   [ ] Visual Interpreter
-   [ ] Compiler

## Language Reference

### Paths

\- | \ /

### Comments

Anything not connected to a path is comment.

### Integer

When an integer is encounted, it is pushed onto the stack.

### Intrinsics

Note that in the signature, the top of the stack is towards the right.

| Name  | Signature         | Description                                                  |
| ----- | ----------------- | ------------------------------------------------------------ |
| `add` | `b a -> (a + b)`  | Add two integers together                                    |
| `sub` | `b a -> (b - a)`  | Subtract two integers                                        |
| `mul` | `b a -> (a * b)`  | Multiply two integers                                        |
| `div` | `b a -> (b / a)`  | Divide two integers (C integer division)                     |
| `mod` | `b a -> (b % a)`  | Perform modulo on two integers                               |
| `=`   | `b a -> (b == a)` | Test whether two integers are equal                          |
| `!=`  | `b a -> (b != a)` | Test whether two integers are not equal                      |
| `>`   | `b a -> (b > a)`  | Test whether an integers is greater than another             |
| `<`   | `b a -> (b < a)`  | Test whether an integers is less than another                |
| `>=`  | `b a -> (b >= a)` | Test whether an integers is greater than or equal to another |
| `<=`  | `b a -> (b <= a)` | Test whether an integers is less than or equal to another    |
| `*`   | `a ->`            | Print as an integer                                          |
| `&`   | `a ->`            | Print as an ASCII character                                  |
| `%`   | `a -> a a`        | Duplicate the top of the stack                               |
