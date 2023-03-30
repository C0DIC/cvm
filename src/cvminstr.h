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

#define TOKEN_COUNT 19

#include <stdlib.h>
#include <stdint.h>
#include "../catastring/catastring.h"

typedef struct {
    int64_t  as_int;
    double   as_float;
    CataStr  as_string;
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

typedef struct {
    CataStr   value;
    bool      is_arg;
} Token;


Object makeEmptyObject();
Object makeObject(const CataStr obj);

Token createToken(CataStr value, bool is_arg);
void createTokenList(Token *token_list);

#endif

#ifdef CVM_INSTR

Object makeObject(const CataStr obj) {
    return (Object) {
        .as_int = castr_to_lld(obj),
        .as_float = atof(obj.data),
        .as_string = obj
    };
}

Object makeEmptyObject() {
    return (Object) {0};
}

Token createToken(CataStr value, bool is_arg) {
    return (Token) {
        .value = value,
        .is_arg = is_arg
    };
}

void createTokenList(Token *token_list) {
    token_list[0] = createToken(CS("push"), true);
    token_list[1] = createToken(CS("if"), true);
    token_list[2] = createToken(CS("ifnt"), true);
    token_list[3] = createToken(CS("dup"), true);
    token_list[4] = createToken(CS("cmp"), true);
    token_list[5] = createToken(CS("wrt"), true);
    token_list[6] = createToken(CS("wrtn"), true);
    token_list[7] = createToken(CS("swp"), true);
    token_list[8] = createToken(CS("goto"), true);

    token_list[9] = createToken(CS("add"), false);
    token_list[10] = createToken(CS("sub"), false);
    token_list[11] = createToken(CS("mult"), false);
    token_list[12] = createToken(CS("div"), false);
    token_list[13] = createToken(CS("hlt"), false);
    token_list[14] = createToken(CS("scn"), false);
    token_list[15] = createToken(CS("inc"), false);
    token_list[16] = createToken(CS("dec"), false);
    token_list[17] = createToken(CS("dmp"), false);
    token_list[18] = createToken(CS("pop"), false);
}

#endif