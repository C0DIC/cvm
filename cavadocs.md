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
- [wrt](#wrt)
- etc.

### push

Push element into the stack. Pushes it to the last free cell.

_[Throws an error if the stack if full](Notes.md#vms-stack)_

``` text
push (int | float)
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

Print the element above
Throws an error when it's noting to print or stack is empty

``` nasm
push 56
wrt       // Prints 56
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

Duplicate an element by its index in stack.

- Elements counting starts with __0__

Throws an error if the argument is less | bigger than stack size.

```text
dup (int)
```

Example:

``` nasm
push 90
push 45

dup 1   // Duplicating 45. Stack size is 3 now
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
    wrt  // Prints it
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

cmp 0       // If the element above is equal to Stack[0]
    wrt 5   // Prints 5
hlt         // Otherwise hlt
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
    wrt        // Prints -15
    push 50
    wrt        // Prints 50
```

## Comments

Coments starts with `//`

``` nasm
// This is a comment
hlt  // Side comments can be used too
```
