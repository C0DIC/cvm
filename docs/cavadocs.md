# Cava

Cava is `assembly language` for cvm (cata virtual machine)

## Instructions

__One__ line is the __one__ instruction. Except the empty | [comment](#comments) line.
There are two types of instructions: `with argument` and `no argument`.

For example, instructions `with argument`:

- [push](#push)
- [goto](#goto)
- [cmp](#cmp)

- etc.

For example, instructions `with no argument`:

- [add](#add)
- [hlt](#hlt)
- [sub](#sub)
- etc.

### push

Push element into the stack. Pushes it to the last free cell.

_[Throws an error if the stack if full](Notes.md#vms-stack)_

``` text
push (i64 (int) | f64 (double) | string)
```

Example:

``` nasm
push 10
```

### add

Plus two elements above `add` instruction and decrementing stack by one.
Throws an error, if the stack size is less than 2

``` nasm
push 10
push 10   // Stack size is 2 now
add       // 10 + 10. Stack size is 1 now and element is 20
```

### sub

From the element that 2 above substructs the element above.
Throws an error, if the stack size is less than 2

``` nasm
push 5
push 10   // Stack size is 2 now
sub       // 5 - 10. Size is 1 now and element is -5
```

### mult

Multiplies two elements above.
Throws an error, if the stack size is less than 2

``` nasm
push 5
push 10   // Stack size is 2 now
mult       // 5 * 10. Size is 1 now and element is 50
```

### div

Devide element that 2 above by the element above.
Throws an error, if the stack size is less than 2

``` nasm
push 45
push 5    // Stack size is 2 now
div       // 45 / 5. Size is 1 now and element is 9
```

### mod

Devide element that 2 above by the element above and get the remainder.
Throws an error, if the stack size is less than 2

``` nasm
push 5
push 10   // Stack size is 2 now
mod       // Substructs 10 from 5. Size is 1 now and element is -5
```

### wrt

Print the element above without new line
Throws an error when it's noting to print or stack is empty

```text
wrt (str | i64 | f64)
```

``` nasm
push 56
wrt i64       // Prints 56
```

### wrtn

Print the element above with new line
Throws an error when sit's noting to print or stack is empty

```text
wrt (str | i64 | f64)
```

``` nasm
push 56
wrtn i64     // Prints 56
```

### hlt

Stops the vm immediately. Like exit() in `C` or `Python`.

``` nasm
hlt
push 5 // Dead code
```

### goto

Go to the instruction by its index or [label](#labels).

- Instructions are the lines with instruction only
- Lines with comments or empty lines not counts as the instruction.
- Counting of instructions starts with __0__

``` text
goto (int | block name)
```

Example:

``` nasm
goto 1    // Instruction #0

push 45   // Instruction #1
```

### dup

Duplicate an element by the "step" from instruction

- Elements counting starts with __0__

Throws an error if the argument is less | bigger than stack size.

```text
dup (int)
```

Example:

``` nasm
push 90   // dup 1
push 45   // dup 0

// 1 is showing `step` from current pos in stack
dup 1   // Duplicating 90. Stack size is 3 now
```

### swp

Swap element above with the element by index after `swp`.

- Elements counting starts with __0__

Throws an error if the argument is less | bigger than stack size.

``` text
swp (int)
```

Example:

``` nasm
push 45
push 40

swp  0    // Swaps 45 and 40
```

## Conditions

### if

Checks the argument with the element above.
If its `true` executing the next line of code, otherwise skips it.

```text
if (int)
```

Example:

``` nasm
push 1

if 1     // If the element above is 1
    wrtn i64  // Prints it
hlt      // Otherwise hlt
```

### cmp

Compares element above with other element by index
Throws an error if the argument is less | bigger than stack size.

```text
cmp (int)
```

Example:

``` nasm
push 5
push 1
push 5

cmp 0          // If the element above is equal to Stack[0]
    wrtn i64   // Prints 5
hlt            // Otherwise hlt
```

## Labels

Creating the block of code to manage code execution.

```text
[name]:
```

Example:

``` nasm
goto main // Will go to main block

exit:     // "exit" block will be skipped, because of goto
    hlt

main:
    push -15
    wrtn i64       // Prints -15
    push 50
    wrt  i64      // Prints 50
```

## Comments

Coments starts with `//`

``` nasm
// This is a comment
hlt  // Side comments can be used too
```

Also available in [Russian](%D0%BA%D0%B0%D0%B2%D0%B0_%D0%B4%D0%BE%D0%BA%D1%83%D0%BC%D0%B5%D0%BD%D1%82%D0%B0%D1%86%D0%B8%D1%8F.md)
