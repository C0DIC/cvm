# cvm

Cata Virtual Machine

CVM is a full stack-based virtual machine. It means that all variables are stored in vm-stack. All operations and instructions also works with elements in vm's stack.

Developing virtual machine for [Cata](https://github.com/C0DIC/cata) programming language. When the CVM will be done, Cata development will continue.

!!Remember
Project is under heavy development

## Quick start

``` console
make
./cvm examples/03-write.cava -r
./cvm examples/08-fibonacci.cava -r
./cvm examples/09-numhello.cava -r
```

## Examples

All examples was written in [Cava](#docs) and working properly.
If you found a bug or an error, please open an issue

## Docs

- [Cava](cavadocs.md) (Cata Virtual Assembly)

## References

- [Virtual Machine](https://en.wikipedia.org/wiki/Virtual_machine)
- [JVM](https://en.wikipedia.org/wiki/Java_virtual_machine)
- [Cata](https://github.com/C0DIC/cata)
- [CataString](./catastring/) library for "strings" in C

## Uninstall 

``` console
make clean
```