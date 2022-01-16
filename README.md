**Note: This documentation is unfinished.**

# Traverse

Traverse is a stack-based, two-dimensional programming language, inspired by [Porth](https://gitlab.com/tsoding/porth).

## Goals

-   [x] [Turing Complete](https://github.com/kowasaur/traverse/blob/main/examples/rule110.trav)
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

| Name  | Signature         | Description                                |
| ----- | ----------------- | ------------------------------------------ |
| `add` | `b a -> (a + b)`  | Addition                                   |
| `sub` | `b a -> (b - a)`  | Subtraction                                |
| `mul` | `b a -> (a * b)`  | Multiplication                             |
| `div` | `b a -> (b / a)`  | Division                                   |
| `mod` | `b a -> (b % a)`  | Modulo                                     |
| `=`   | `b a -> (b == a)` | Equal                                      |
| `!=`  | `b a -> (b != a)` | Not Equal                                  |
| `>`   | `b a -> (b > a)`  | Greater Than                               |
| `<`   | `b a -> (b < a)`  | Less Than                                  |
| `>=`  | `b a -> (b >= a)` | Greater Than or Equal                      |
| `<=`  | `b a -> (b <= a)` | Less Than or Equal                         |
| `and` | `b a -> (a & b)`  | Bitwise And                                |
| `or`  | `b a -> (a \| b)` | Bitwise Or                                 |
| `not` | `a -> ~a`         | Bitwise Not                                |
| `xor` | `b a -> (a ^ b)`  | Bitwise Xor                                |
| `<<`  | `b a -> (b << a)` | Bitshift Left                              |
| `>>`  | `b a -> (b >> a)` | Bitshift Right                             |
| `*`   | `a ->`            | Print as an integer                        |
| `&`   | `a ->`            | Print as an ASCII character                |
| `?`   | `-> a`            | Get a single character of input            |
| `%`   | `a -> a a`        | Duplicate the top of the stack             |
| `!`   | `a ->`            | Drop the top of the stack                  |
| `$`   | `b a -> a b`      | Swap the two top elements of the stack     |
| `@`   | `c b a -> b a c`  | Rotate the top three elements of the stack |
