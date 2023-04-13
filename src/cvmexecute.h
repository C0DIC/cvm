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

#define CVM_STD
#include "cvmstd.h"

CataError cvm_execute(CataVM *cvm);

#endif // CVM_EXECUTE

#ifdef CVM_EXECUTE

void cata_exit(CataError err) {
    if (err != CATA_OK) exit(1);
}

CataError cvm_execute(CataVM *cvm) {
    while (cvm->instr_pos != cvm->instr_stack_size && !cvm->halted) {

        if (cvm->instr_pos < 0 || cvm->instr_pos > cvm->instr_stack_size) {
                cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                          ERR_BOUND_BREAK, "out of bounds");
                return ERR_BOUND_BREAK;
        }

        if (cvm->instr_stack[cvm->instr_pos].instr.length == 0) cvm->instr_pos += 1;
        else {
            if (cvm->instr_stack[cvm->instr_pos].warg) {
                if (castr_same(CS("push"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_push(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("wrt"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_wrt(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("wrtn"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_wrtn(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("if"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_if(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("ifnt"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_ifnt(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("cmp"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_cmp(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("goto"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_goto(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("swp"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_swp(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else if (castr_same(CS("dup"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_dup(cvm, cvm->instr_stack[cvm->instr_pos].arg));
                else {
                    fprintf (stderr,
                            "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                            cvm->filename, cvm_err_to_cstr(ERR_NAME_ERROR), cvm->instr_stack[cvm->instr_pos].line,
                            "unknown name", CS_FMT(cvm->instr_stack[cvm->instr_pos].instr)
                    );

                    return ERR_NAME_ERROR;
                }
            } else {
                if (castr_same(CS("add"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_add(cvm));
                else if (castr_same(CS("sub"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_sub(cvm));
                else if (castr_same(CS("mult"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_mult(cvm));
                else if (castr_same(CS("div"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_div(cvm));
                else if (castr_same(CS("mod"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_mod(cvm));
                else if (castr_same(CS("inc"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_inc(cvm));
                else if (castr_same(CS("dec"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_dec(cvm));
                else if (castr_same(CS("hlt"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_hlt(cvm));
                else if (castr_same(CS("pop"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_pop(cvm));
                else if (castr_same(CS("scn"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_scn(cvm));
                else if (castr_same(CS("dmp"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_dmp(cvm));
                else if (castr_same(CS("=="), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_eq(cvm));
                else if (castr_same(CS("!="), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_noteq(cvm));
                else if (castr_same(CS(">"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_greater(cvm));
                else if (castr_same(CS("<"), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_less(cvm));
                else if (castr_same(CS(">="), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_greateq(cvm));
                else if (castr_same(CS("<="), cvm->instr_stack[cvm->instr_pos].instr))
                    cata_exit(execute_lesseq(cvm));
                else if (castr_endswith  (":", cvm->instr_stack[cvm->instr_pos].instr) ||
                         castr_startswith(".", cvm->instr_stack[cvm->instr_pos].instr))
                                cvm->instr_pos += 1;
                else {
                    fprintf (stderr,
                        "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                        cvm->filename, cvm_err_to_cstr(ERR_NAME_ERROR), cvm->instr_stack[cvm->instr_pos].line,
                        "unknown name", CS_FMT(cvm->instr_stack[cvm->instr_pos].instr)
                    );

                    return ERR_NAME_ERROR;
                }
            }
        }
    }
    return CATA_OK;
}

#endif