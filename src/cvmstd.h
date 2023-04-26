/*
*
*
*       cvmstd.h
*
*       Created by C0DIC, 09/04/2023
*
*
*       File where instuctions for CataVM implemented
*
*
*/

#ifndef CVM_STD_H
#define CVM_STD_H

#include "catavm.h"

CataError execute_push(CataVM *cvm, Object arg);

CataError execute_wrt(CataVM *cvm, Object arg);
CataError execute_wrtn(CataVM *cvm, Object arg);

CataError execute_if(CataVM *cvm, Object arg);
CataError execute_ifnt(CataVM *cvm, Object arg);

CataError execute_cmp(CataVM *cvm, Object arg);

CataError execute_goto(CataVM *cvm, Object arg);

CataError execute_swp(CataVM *cvm, Object arg);

CataError execute_dup(CataVM *cvm, Object arg);

//////////////////////////////////////////////////

CataError execute_add(CataVM *cvm);

CataError execute_inc(CataVM *cvm);
CataError execute_dec(CataVM *cvm);

CataError execute_pop(CataVM *cvm);

CataError execute_hlt(CataVM *cvm);

CataError execute_scn(CataVM *cvm);

CataError execute_dmp(CataVM *cvm);

CataError execute_eq(CataVM *cvm);
CataError execute_noteq(CataVM *cvm);
CataError execute_greater(CataVM *cvm);
CataError execute_less(CataVM *cvm);
CataError execute_greateq(CataVM *cvm);
CataError execute_lesseq(CataVM *cvm);

#endif


#define CVM_STD
#ifdef CVM_STD

// Cava Push
CataError execute_push(CataVM *cvm, Object arg) {
    if (cvm->stack_size >= STACK_SIZE) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_OVERFLOW, "stack overflow"
        );
        return ERR_STACK_OVERFLOW;
    }

    cvm->stack[cvm->stack_size++] = arg;
    cvm->instr_pos += 1;
    return CATA_OK;
};


// Math
// Add two i64
CataError execute_add(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    cvm->stack[cvm->stack_size - 2].as_int += cvm->stack[cvm->stack_size - 1].as_int;
    cvm->stack_size -= 1;
    cvm->instr_pos += 1;

    return CATA_OK;
};

// Substruct two i64
CataError execute_sub(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    cvm->stack[cvm->stack_size - 2].as_int -= cvm->stack[cvm->stack_size - 1].as_int;
    cvm->stack_size -= 1;
    cvm->instr_pos += 1;

    return CATA_OK;
};

// Div two i64
CataError execute_div(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    cvm->stack[cvm->stack_size - 2].as_int /= cvm->stack[cvm->stack_size - 1].as_int;
    cvm->stack_size -= 1;
    cvm->instr_pos += 1;

    return CATA_OK;
};

// Mult two i64
CataError execute_mult(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    cvm->stack[cvm->stack_size - 2].as_int *= cvm->stack[cvm->stack_size - 1].as_int;
    cvm->stack_size -= 1;
    cvm->instr_pos += 1;

    return CATA_OK;
};

// Modula
CataError execute_mod(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    cvm->stack[cvm->stack_size - 2].as_int %= cvm->stack[cvm->stack_size - 1].as_int;
    cvm->stack_size -= 1;
    cvm->instr_pos += 1;

    return CATA_OK;
};


//////////////////////////////////

// Printing functions

static CataError printi64(int64_t arg, bool new_line) {
    if (new_line) printf("%zi\n", arg);
    else printf("%zi", arg);

    return CATA_OK;
};

static CataError printf64(double arg, bool new_line) {
    if (new_line) printf("%lf\n", arg);
    else printf("%lf", arg);
    
    return CATA_OK;
};

static CataError printstr(CataStr arg, bool new_line) {
    if (new_line) printf(CS_PRI"\n", CS_FMT(arg));
    else printf(CS_PRI, CS_FMT(arg));

    return CATA_OK;
};

static bool var_exist(CataVM *cvm, CataStr name) {
    for (size_t pos = 0; pos < cvm->variables_stack_size; pos++) {
        if (castr_same(name, cvm->variables_stack[pos].name)) {
            return true;
        }
    }

    return false;
};

static size_t var_indx(CataVM *cvm, CataStr name) {
    if (var_exist(cvm, name)) {
        for (size_t pos = 0; pos < cvm->variables_stack_size; pos++) {
            if (castr_same(name, cvm->variables_stack[pos].name)) {
                return pos;
            }
        }   
    }

    return -1;
}

static CataStr getType(CataVM *cvm, CataStr name) {
    if (var_exist(cvm, name)) {
        for (size_t pos = 0; pos < cvm->variables_stack_size; pos++) {
            if (castr_same(name, cvm->variables_stack[pos].name)) {
                return cvm->variables_stack[pos].type;
            }
        }   
    }

    return CS("");
}

/////////////

// Standart print without new line
CataError execute_wrt(CataVM *cvm, Object arg) {
    if (cvm->stack_size < 1) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (castr_same(arg.as_string, CS("i64")))
        printi64(cvm->stack[cvm->stack_size - 1].as_int, false);
    else if (castr_same(arg.as_string, CS("f64")))
        printf64(cvm->stack[cvm->stack_size - 1].as_float, false);
    else if (castr_same(arg.as_string, CS("str")))
        printstr(cvm->stack[cvm->stack_size -1].as_string, false);
    else if (castr_startswith("$", arg.as_string)) {
        if (var_exist(cvm, castr_cut_by(1, arg.as_string))) {
            if (castr_same(CS("str"), getType(cvm, castr_cut_by(1, arg.as_string))))
                printstr(cvm->variables_stack[var_indx(cvm, castr_cut_by(1, arg.as_string))].value.as_string, false);
            else if (castr_same(CS("i64"), getType(cvm, castr_cut_by(1, arg.as_string))))
                printi64(cvm->variables_stack[var_indx(cvm, castr_cut_by(1, arg.as_string))].value.as_int, false);
            else if (castr_same(CS("f64"), getType(cvm, castr_cut_by(1, arg.as_string))))
                printf64(cvm->variables_stack[var_indx(cvm, castr_cut_by(1, arg.as_string))].value.as_float, false);
        } else {
            fprintf(stderr,
                "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_VARIABLE), cvm->instr_stack[cvm->instr_pos].line,
                "unknown variable", CS_FMT(castr_cut_by(1, arg.as_string))
            );

            return ERR_UNKNOWN_VARIABLE;
        }
    } else {
        fprintf(stderr,
            "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
            cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_TYPE), cvm->instr_stack[cvm->instr_pos].line,
            "unexcepted value. excepted type or variable", CS_FMT(arg.as_string)
        );

        return ERR_UNKNOWN_TYPE;
    }

    cvm->instr_pos += 1;
    return CATA_OK;
};

// Standart print with new line
CataError execute_wrtn(CataVM *cvm, Object arg) {
    if (cvm->stack_size < 1) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (castr_same(arg.as_string, CS("i64")))
        printi64(cvm->stack[cvm->stack_size - 1].as_int, true);
    else if (castr_same(arg.as_string, CS("f64")))
        printf64(cvm->stack[cvm->stack_size - 1].as_float, true);
    else if (castr_same(arg.as_string, CS("str")))
        printstr(cvm->stack[cvm->stack_size - 1].as_string, true);
    else if (castr_startswith("$", arg.as_string)) {
        if (var_exist(cvm, castr_cut_by(1, arg.as_string))) {
            if (castr_same(CS("str"), getType(cvm, castr_cut_by(1, arg.as_string))))
                printstr(cvm->variables_stack[var_indx(cvm, castr_cut_by(1, arg.as_string))].value.as_string, true);
            else if (castr_same(CS("i64"), getType(cvm, castr_cut_by(1, arg.as_string))))
                printi64(cvm->variables_stack[var_indx(cvm, castr_cut_by(1, arg.as_string))].value.as_int, true);
            else if (castr_same(CS("f64"), getType(cvm, castr_cut_by(1, arg.as_string))))
                printf64(cvm->variables_stack[var_indx(cvm, castr_cut_by(1, arg.as_string))].value.as_float, true);
        } else {
            fprintf(stderr,
                "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_VARIABLE), cvm->instr_stack[cvm->instr_pos].line,
                "unknown variable", CS_FMT(castr_cut_by(1, arg.as_string))
            );

            return ERR_UNKNOWN_VARIABLE;
        }
    } else {
        fprintf(stderr,
            "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
            cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_TYPE), cvm->instr_stack[cvm->instr_pos].line,
            "unexcepted value. excepted type or variable, but found", CS_FMT(arg.as_string)
        );

        return ERR_UNKNOWN_TYPE;
    }

    cvm->instr_pos += 1;
    return CATA_OK;
};


// If condition
// TODO: check conditions with variables
CataError execute_if(CataVM *cvm, Object arg) {
    if (cvm->stack[cvm->stack_size - 1].as_int == arg.as_int) cvm->instr_pos += 1;
    else cvm->instr_pos += 2;

    return CATA_OK;
};

// If NOT condition
// TODO: check conditions with variables
CataError execute_ifnt(CataVM *cvm, Object arg) {
    if (cvm->stack[cvm->stack_size - 1].as_int != arg.as_int) cvm->instr_pos += 1;
    else cvm->instr_pos += 2;

    return CATA_OK;
};

// Compare condition
// TODO: cmp with variables
CataError execute_cmp(CataVM *cvm, Object arg) {
    if (cvm->instr_pos < 0 || cvm->instr_pos > cvm->instr_stack_size) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_BOUND_BREAK, "out of bounds"
        );
        return ERR_BOUND_BREAK;
    }

    if (cvm->stack[cvm->stack_size - 1].as_int == cvm->stack[arg.as_int].as_int) cvm->instr_pos += 1;
    else cvm->instr_pos += 2;

    return CATA_OK;
};


// Goto 
CataError execute_goto(CataVM *cvm, Object arg) {
    for (size_t i = 0; i < cvm->instr_stack_size; i++) {
        if (cvm->instr_stack[i].line == arg.as_int)  {
            cvm->instr_pos = i;
        }
    }

    return CATA_OK;
}


// Swap elements
CataError execute_swp(CataVM *cvm, Object arg) {
    if (cvm->instr_pos < 0 || cvm->instr_pos > cvm->instr_stack_size) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_BOUND_BREAK, "out of bounds");
        return ERR_BOUND_BREAK;
    }

    Object temp = cvm->stack[cvm->stack_size - 1];

    cvm->stack[cvm->stack_size - 1] = cvm->stack[arg.as_int];
    cvm->stack[arg.as_int] = temp;
    cvm->instr_pos  += 1;  

    return CATA_OK;
};

CataError execute_dup(CataVM *cvm, Object arg) {
    if (cvm->stack_size >= STACK_SIZE) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_OVERFLOW, "stack overflow"
        );
        return ERR_STACK_OVERFLOW;
    }

    if (arg.as_int < 0) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow"    
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (arg.as_int > cvm->stack_size) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_OVERFLOW, "stack overflow"
        );
        return ERR_STACK_OVERFLOW;
    }

    cvm->stack[cvm->stack_size] = cvm->stack[cvm->stack_size - 1 - arg.as_int];
    cvm->stack_size += 1;
    cvm->instr_pos  += 1;

    return CATA_OK;
};

////////////////////////////////////////////////////////////////////////////////////

// Increment value
CataError execute_inc(CataVM *cvm) {
    if (cvm->stack_size < 1) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow");
        return ERR_STACK_UNDERFLOW;
    }
    
    cvm->stack[cvm->stack_size - 1].as_int++;
    cvm->instr_pos += 1;

    return CATA_OK;
};

// Decrement value
CataError execute_dec(CataVM *cvm) {
    if (cvm->stack_size < 1) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow");
        return ERR_STACK_UNDERFLOW;
    }
    
    cvm->stack[cvm->stack_size - 1].as_int--;
    cvm->instr_pos += 1;

    return CATA_OK;
};

// "Power off" vm
CataError execute_hlt(CataVM *cvm) {
    cvm->halted = 1;
    return CATA_OK;
};

CataError execute_pop(CataVM *cvm) {
    if (cvm->stack_size < 1) {
        cvm_error(cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack underflow");
        return ERR_STACK_UNDERFLOW;
    }

    cvm->stack_size -= 1;
    cvm->instr_pos += 1;

    return CATA_OK;
};


// Get input from user (i64)
// TODO: make `scn` with argument, to get input with type. Needed?
CataError execute_scn(CataVM *cvm) {
    if (cvm->stack_size >= STACK_SIZE) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_OVERFLOW, "stack overflow"
        );
        return ERR_STACK_OVERFLOW;
    }

    scanf("%zi", &cvm->stack[cvm->stack_size++].as_int);
    cvm->instr_pos  += 1;

    return CATA_OK;
};

CataError execute_dmp(CataVM *cvm) {

    for (size_t pos = 0; pos < cvm->stack_size; ++pos)
        printf("Stack: ['str',"CS_PRI"] | ['i64', %ld] | ['f64', %lf]\n",
            CS_FMT(cvm->stack[pos].as_string),
            cvm->stack[pos].as_int,
            cvm->stack[pos].as_float
        );

    cvm->instr_pos  += 1;

    return CATA_OK;
};

// Boolean operations

// ==
// 5
// 5
// ==
// This is true
CataError execute_eq(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack unferflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (cvm->stack[cvm->stack_size - 2].as_int == cvm->stack[cvm->stack_size - 1].as_int)
        cvm->stack[cvm->stack_size++].as_int = 1;
    else cvm->stack[cvm->stack_size++].as_int = 0;
    
    cvm->instr_pos  += 1;

    return CATA_OK;
};

// != operation
//
// 1
// 5
// !=
// This is true
CataError execute_noteq(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack unferflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (cvm->stack[cvm->stack_size - 2].as_int != cvm->stack[cvm->stack_size - 1].as_int)
        cvm->stack[cvm->stack_size++].as_int = 1;
    else cvm->stack[cvm->stack_size++].as_int = 0;

    cvm->instr_pos  += 1;

    return CATA_OK;
};

// > operation
//
// 5
// 1
// >
// This is true
CataError execute_greater(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack unferflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (cvm->stack[cvm->stack_size - 2].as_int > cvm->stack[cvm->stack_size - 1].as_int)
        cvm->stack[cvm->stack_size++].as_int = 1;
    else cvm->stack[cvm->stack_size++].as_int = 0;

    cvm->instr_pos  += 1;

    return CATA_OK;
};


// < operation
//
// 1
// 5
// <
// This is true
CataError execute_less(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack unferflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (cvm->stack[cvm->stack_size - 2].as_int < cvm->stack[cvm->stack_size - 1].as_int)
        cvm->stack[cvm->stack_size++].as_int = 1;
    else cvm->stack[cvm->stack_size++].as_int = 0;

    cvm->instr_pos  += 1;

    return CATA_OK;
};

// <= operation
//
// 5       1
// 5       5
// <=      <=
// Both is true
CataError execute_lesseq(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack unferflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (cvm->stack[cvm->stack_size - 2].as_int <= cvm->stack[cvm->stack_size - 1].as_int)
        cvm->stack[cvm->stack_size++].as_int = 1;
    else cvm->stack[cvm->stack_size++].as_int = 0;

    cvm->instr_pos  += 1;

    return CATA_OK;
};

// >= operation
//
// 5       7
// 5       5
// <=      <=
// Both is true
CataError execute_greateq(CataVM *cvm) {
    if (cvm->stack_size < 2) {
        cvm_error(
            cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
            ERR_STACK_UNDERFLOW, "stack unferflow"
        );
        return ERR_STACK_UNDERFLOW;
    }

    if (cvm->stack[cvm->stack_size - 2].as_int >= cvm->stack[cvm->stack_size - 1].as_int)
        cvm->stack[cvm->stack_size++].as_int = 1;
    else cvm->stack[cvm->stack_size++].as_int = 0;

    cvm->instr_pos  += 1;

    return CATA_OK;
};

///////////////////////////////////////

#endif