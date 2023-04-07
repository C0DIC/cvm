/*
*
*
*       cvmexecute.h
*
*       Created by C0DIC, 24/03/2023
*
*
*       Execution of the instructions for CVM
*
*
*/


#ifndef CVM_EXECUTE_H
#define CVM_EXECUTE_H

#include "../catastring/catastring.h"

#ifndef CVM_INSTR
    #include "cvminstr.h"
#endif

#include "catavm.h"

#ifndef CVM_ERROR
    #include "cvmerror.h"
#endif

#ifdef CVM_PARSER
    #include "cvmparser.h"
#endif

CataError cvm_execute(CataVM *cvm);

#endif // CVM_EXECUTE

#ifdef CVM_EXECUTE

CataError cvm_execute(CataVM *cvm) {
    while (cvm->instr_pos != cvm->instr_stack_size && !cvm->halted) {
        CataStr instr = cvm->instr_stack[cvm->instr_pos].instr;
        Object  arg   = cvm->instr_stack[cvm->instr_pos].arg;

        if (cvm->instr_pos < 0 || cvm->instr_pos > cvm->instr_stack_size) {
                cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                          ERR_BOUND_BREAK, "out of bounds");
                return ERR_BOUND_BREAK;
        }

        if (instr.length == 0) cvm->instr_pos += 1;
        else {
            if (castr_same(instr, CS("push"))) {
                if (cvm->stack_size >= STACK_SIZE) {
                    cvm_error(
                        cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                        ERR_STACK_OVERFLOW, "stack overflow");
                    return ERR_STACK_OVERFLOW;
                }

                cvm->stack[cvm->stack_size++] = arg;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("add"))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                cvm->stack[cvm->stack_size - 2].as_int += cvm->stack[cvm->stack_size - 1].as_int;
                cvm->stack_size -= 1;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("sub"))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                cvm->stack[cvm->stack_size - 2].as_int -= cvm->stack[cvm->stack_size - 1].as_int;
                cvm->stack_size -= 1;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("mult"))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                cvm->stack[cvm->stack_size - 2].as_int *= cvm->stack[cvm->stack_size - 1].as_int;
                cvm->stack_size -= 1;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("div"))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (cvm->stack[cvm->stack_size - 1].as_int == 0) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_DIVISION_BY_ZERO, "division by zero found"
                    );
                    return ERR_DIVISION_BY_ZERO;
                }

                cvm->stack[cvm->stack_size - 2].as_int /= cvm->stack[cvm->stack_size - 1].as_int;
                cvm->stack_size -= 1;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("mod"))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (cvm->stack[cvm->stack_size - 1].as_int == 0) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_DIVISION_BY_ZERO, "division by zero found"
                    );
                    return ERR_DIVISION_BY_ZERO;
                }

                cvm->stack[cvm->stack_size - 2].as_int %= cvm->stack[cvm->stack_size - 1].as_int;
                cvm->stack_size -= 1;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("inc"))) {
                if (cvm->stack_size < 1) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                cvm->stack[cvm->stack_size - 1].as_int++;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("dec"))) {
                if (cvm->stack_size < 1) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                cvm->stack[cvm->stack_size - 1].as_int--;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("pop"))) {
                if (cvm->stack_size < 1) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                cvm->stack_size -= 1;
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("cmp"))) {
                if (cvm->instr_pos < 0 || cvm->instr_pos > cvm->instr_stack_size) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_BOUND_BREAK, "out of bounds");
                    return ERR_BOUND_BREAK;
                }

                if (cvm->stack[cvm->stack_size - 1].as_int == cvm->stack[arg.as_int].as_int) {
                    cvm->instr_pos += 1;
                } else {
                    cvm->instr_pos += 2;
                }
            } else if (castr_same(instr, CS("hlt"))) {
                cvm->halted = 1;
            } else if (castr_same(instr, CS("dmp"))) {
                for (size_t i = 0; i < cvm->stack_size; ++i) {
                    printf("Stack[%ld]: i64: %ld | f64: %lf | str:"CS_PRI"\n", i, cvm->stack[i].as_int, cvm->stack[i].as_float, CS_FMT(cvm->stack[i].as_string));
                }
                cvm->instr_pos += 1;
            } else if (castr_same(instr, CS("goto"))) {
                for (size_t i = 0; i < cvm->instr_stack_size; i++) {
                    if (cvm->instr_stack[i].line == arg.as_int)  {
                        cvm->instr_pos = i;
                    }
                }
            } else if (castr_same(instr, CS("if"))) {
                if (cvm->stack[cvm->stack_size - 1].as_int == arg.as_int) {
                    cvm->instr_pos += 1;
                } else {
                    cvm->instr_pos += 2;
                }
            } else if (castr_same(instr, CS("ifnt"))) {
                if (cvm->stack[cvm->stack_size - 1].as_int != arg.as_int) {
                    cvm->instr_pos += 1;
                } else {
                    cvm->instr_pos += 2;
                }
            } else if (castr_same(instr, CS("dup"))) {
                if (cvm->stack_size >= STACK_SIZE) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                    ERR_STACK_OVERFLOW, "stack overflow");
                    return ERR_STACK_OVERFLOW;
                }

                if (arg.as_int < 0) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                    ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (arg.as_int > cvm->stack_size) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                    ERR_STACK_OVERFLOW, "stack overflow");
                    return ERR_STACK_OVERFLOW;
                }

                cvm->stack[cvm->stack_size] = cvm->stack[cvm->stack_size - 1 - arg.as_int];
                cvm->stack_size += 1;
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS("wrt"))) {
                if (castr_same(CS("i64"), arg.as_string)) {
                    printf("%ld", cvm->stack[cvm->stack_size - 1].as_int);
                } else if (castr_same(CS("f64"), arg.as_string)) {
                    printf("%lf", cvm->stack[cvm->stack_size - 1].as_float);
                } else if (castr_same(CS("str"), arg.as_string)) {
                    printf(CS_PRI"", CS_FMT(cvm->stack[cvm->stack_size - 1].as_string));
                } else {
                    fprintf(stderr,
                        "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                        cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_TYPE), cvm->instr_stack[cvm->instr_pos].line,
                        "unknown type", CS_FMT(arg.as_string)
                    );

                    return ERR_UNKNOWN_TYPE;
                }

                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS("wrtn"))) {
                if (castr_same(CS("i64"), arg.as_string)) {
                    printf("%ld\n", cvm->stack[cvm->stack_size - 1].as_int);
                } else if (castr_same(CS("f64"), arg.as_string)) {
                    printf("%lf\n", cvm->stack[cvm->stack_size - 1].as_float);
                } else if (castr_same(CS("str"), arg.as_string)) {
                    printf(CS_PRI"\n", CS_FMT(cvm->stack[cvm->stack_size - 1].as_string));
                } else {
                    fprintf(stderr,
                        "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                        cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_TYPE), cvm->instr_stack[cvm->instr_pos].line,
                        "unknown type", CS_FMT(arg.as_string)
                    );

                    return ERR_UNKNOWN_TYPE;
                }

                cvm->instr_pos  += 1;
            } else if (castr_endswith(":", instr)) {
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS("scn"))) {
                if (cvm->stack_size >= STACK_SIZE) {
                    cvm_error(
                        cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                        ERR_STACK_OVERFLOW, "stack overflow");
                    return ERR_STACK_OVERFLOW;
                }

                scanf("%ld", &cvm->stack[cvm->stack_size++].as_int);
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS("swp"))) {
                if (cvm->instr_pos < 0 || cvm->instr_pos > cvm->instr_stack_size) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_BOUND_BREAK, "out of bounds");
                    return ERR_BOUND_BREAK;
                }

                Object temp = cvm->stack[cvm->stack_size - 1];

                cvm->stack[cvm->stack_size - 1] = cvm->stack[arg.as_int];
                cvm->stack[arg.as_int] = temp;
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS("=="))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (cvm->stack[cvm->stack_size - 2].as_int == cvm->stack[cvm->stack_size - 1].as_int)
                    cvm->stack[cvm->stack_size - 2].as_int = 1;
                else cvm->stack[cvm->stack_size - 2].as_int = 0;

                cvm->stack_size -= 1;
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS("<"))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (cvm->stack[cvm->stack_size - 2].as_int < cvm->stack[cvm->stack_size - 1].as_int)
                    cvm->stack[cvm->stack_size - 2].as_int = 1;
                else cvm->stack[cvm->stack_size - 2].as_int = 0;

                cvm->stack_size -= 1;
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS(">"))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (cvm->stack[cvm->stack_size - 2].as_int > cvm->stack[cvm->stack_size - 1].as_int)
                    cvm->stack[cvm->stack_size - 2].as_int = 1;
                else cvm->stack[cvm->stack_size - 2].as_int = 0;

                cvm->stack_size -= 1;
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS("<="))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (cvm->stack[cvm->stack_size - 2].as_int <= cvm->stack[cvm->stack_size - 1].as_int)
                    cvm->stack[cvm->stack_size - 2].as_int = 1;
                else cvm->stack[cvm->stack_size - 2].as_int = 0;

                cvm->stack_size -= 1;
                cvm->instr_pos  += 1;
            } else if (castr_same(instr, CS(">="))) {
                if (cvm->stack_size < 2) {
                    cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                            ERR_STACK_UNDERFLOW, "stack underflow");
                    return ERR_STACK_UNDERFLOW;
                }

                if (cvm->stack[cvm->stack_size - 2].as_int >= cvm->stack[cvm->stack_size - 1].as_int)
                    cvm->stack[cvm->stack_size - 2].as_int = 1;
                else cvm->stack[cvm->stack_size - 2].as_int = 0;

                cvm->stack_size -= 1;
                cvm->instr_pos  += 1;
            } else {
                fprintf(stderr,
                        "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                        cvm->filename, cvm_err_to_cstr(ERR_NAME_ERROR), cvm->instr_stack[cvm->instr_pos].line,
                        "unknown name", CS_FMT(instr)
                    );

                return ERR_NAME_ERROR;
            }
        }
    }

    return CATA_OK;
}

#endif