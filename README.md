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

| Name  | Signature  | Description                 |
| ----- | ---------- | --------------------------- |
| `add` | `a b -> c` | Add two integers together   |
| `*`   | `a ->`     | Print as an integer         |
| `&`   | `a ->`     | Print as an ASCII character |
