/*
*
*
*       cvminst.h
*
*       Created by C0DIC, 21/03/2023
*
*
*       File where instuctions for CataVM implemented
*
*
*/

#ifndef CVM_INSTRUCTION_H
#define CVM_INSTRUCTION_H

#include <stdlib.h>
#include <stdint.h>
#include "../catastring/catastring.h"

typedef union {
    int64_t  as_int;
    double   as_float;
} Object;

typedef struct {
    CataStr     instr;
    Object      arg;
    size_t      line;
} CVMInst;

typedef struct {
    CataStr     instr;
    size_t      line;
} CVMLabel;

Object makeObject(const CataStr obj);

#endif

#ifdef CVM_INSTR

Object makeObject(const CataStr obj) {
    return (Object) {
        .as_float = atof(obj.data),
        .as_int = castr_to_lld(obj)
    };
}

#endif