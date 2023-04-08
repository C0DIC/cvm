/*
*
*
*       cvmerror.h
*
*       Created by C0DIC, 22/03/2023
*
*
*       File with CataVM errors
*
*
*/

#ifndef CVM_ERROR_H
#define CVM_ERROR_H


#include "../catastring/catastring.h"

typedef enum {
    CATA_OK,
    ERR_NO_ARGUMENT,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_NAME_ERROR,
    ERR_BOUND_BREAK,
    ERR_DIVISION_BY_ZERO,
    ERR_UNKNOWN_LABEL,
    ERR_UNKNOWN_TYPE,
    ERR_UNKNOWN_VARIABLE,
    ERR_SYNTAX_ERROR
} CataError;

char *cvm_err_to_cstr(CataError error);
void cvm_error(const char *filename, size_t line_cnt, CataError error, char *text);

#endif

#define CVM_ERROR
#ifdef CVM_ERROR

char *cvm_err_to_cstr(CataError error) {
    switch (error) {
        case CATA_OK:                   return "OK"; break;
        case ERR_NO_ARGUMENT:           return "Missing Argument"; break;
        case ERR_STACK_OVERFLOW:        return "Stack Overflow"; break;
        case ERR_STACK_UNDERFLOW:       return "Stack Underflow"; break;
        case ERR_NAME_ERROR:            return "Name Error"; break;
        case ERR_SYNTAX_ERROR:          return "Syntax Error"; break;
        case ERR_DIVISION_BY_ZERO:      return "Zero Division"; break;
        case ERR_BOUND_BREAK:           return "Bound Error"; break;
        case ERR_UNKNOWN_LABEL:         return "Unknown Label"; break;
        case ERR_UNKNOWN_TYPE:          return "Type Error"; break;
        case ERR_UNKNOWN_VARIABLE:      return "Unknown Variable"; break;
        default: break;
    }
}

void cvm_error(const char *filename, size_t line_cnt, CataError error, char *text) {
    fprintf(stderr,
            "%s:\n  |___%s: %lu: %s\n",
            filename, cvm_err_to_cstr(error), line_cnt,
            text
    );
}

#endif