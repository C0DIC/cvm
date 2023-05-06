/*
*
*
*       cvmparser.h
*
*       Created by C0DIC, 21/03/2023
*
*
*       Parser implementation for CataVM
*
*
*/

#ifndef CVM_PARSER_H
#define CVM_PARSER_H

#include "catavm.h"

CataError parse_insts(CataStr source, CataVM *cvm);

#endif

#ifdef CVM_PARSER

static bool label_exist(CataVM *cvm, CataStr label) {
    for (size_t pos = 0; pos < cvm->labels_stack_size; pos++) {
        if (castr_same(label, castr_untilc(':', cvm->labels_stack[pos].instr))) {
            return true;
        }
    }

    return false;
}

static size_t label_indx(CataVM *cvm, CataStr label) {
    for (size_t pos = 0; pos < cvm->labels_stack_size; pos++)
        if (castr_same(castr_untilc(':', cvm->labels_stack[pos].instr), label)) {
            return cvm->labels_stack[pos].line;
    }

    return -1;
}

static size_t token_exist(CataStr token, Token *tok_array) {
    for (size_t pos = 0; pos < TOKEN_COUNT; pos++) {
        if (castr_same(token, tok_array[pos].value))
            return pos;
    }

    return -1;
};

CataError parse_insts(CataStr source, CataVM *cvm) {
    CataStr line         = {0};
    CataStr token        = {0};
    CataStr arg          = {0};
    size_t  line_count   = 1;
    bool    is_str       = false;

    CataStr cp_source = source;
    Token tok_list[TOKEN_COUNT]; createTokenList(tok_list);

    while (cp_source.length != 0) {
        line   = castr_ltrim(' ', castr_untilc('\n', cp_source));

        token  = castr_untilc(' ', line);

        line   = castr_ltrim(' ', castr_untilc('\n', castr_cut_by(token.length, line)));
        arg    = castr_ltrim(' ', castr_untilc(' ', line));
        
        if (!castr_has('/', line))
             line   = castr_ltrim(' ', castr_untilc('\n', castr_cut_by(arg.length, line)));
        else line   = castr_ltrim(' ', castr_cutc('/', line));

        if (token.length != 0) {
            if (castr_endswith(":", token)) {
                cvm->labels_stack[cvm->labels_stack_size].instr     = token;
                cvm->labels_stack[cvm->labels_stack_size].line      = line_count;
                cvm->labels_stack_size += 1;
            }
        }
        
        if (!castr_has('\n', cp_source)) cp_source = CS("");
            else cp_source = castr_cutc('\n', cp_source);

        line_count += 1;
    }

    line_count = 1;

    while (source.length != 0) {
        line   = castr_ltrim(' ', castr_untilc('\n', source));

        if (castr_startswith("//", line)) {
            line_count += 1;
            if (!castr_has('\n', source)) source = CS("");
                else source = castr_cutc('\n', source);
            
            continue;
        }

        token  = castr_untilc(' ', line);
        line   = castr_ltrim(' ', castr_untilc('\n', castr_cut_by(token.length, line)));

        if (castr_startswith("\"", line)) {
            line = castr_cut_by(1, line);

            if (castr_has('"', line)) {
                arg = castr_untilc('"', line);
                line = castr_cut_by(1, line);
                is_str = true;
            } else {
                cvm_error(
                    cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                    ERR_SYNTAX_ERROR, "\" never closed");
                return ERR_SYNTAX_ERROR;
            }
        } else {
            arg    = castr_ltrim(' ', castr_untilc(' ', line));
            line = castr_cutc('\n', line);
        }
        
        if (!castr_has('/', line))
             line   = castr_ltrim(' ', castr_untilc('\n', castr_cut_by(arg.length, line)));
        else line   = castr_ltrim(' ', castr_cutc('/', line));

        if (!castr_startswith("/", line) && line.length != 0) {
            fprintf(stderr,
                    "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                    cvm->filename, cvm_err_to_cstr(ERR_NAME_ERROR), line_count,
                    "unknown name", CS_FMT(line)
            );
            return ERR_NAME_ERROR;
        }

        if (token.length != 0) {
            if (castr_endswith(":", token)) {
                cvm->instr_stack[cvm->instr_stack_size].instr   = token;
                cvm->instr_stack[cvm->instr_stack_size].arg     = makeEmptyObject();
                cvm->instr_stack[cvm->instr_stack_size].line    = line_count;
                cvm->instr_stack_size += 1;
            } else if (castr_startswith(".", token)) {
                if (var_exist(cvm, castr_cut_by(1, token))) {
                    fprintf(stderr,
                        "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                        cvm->filename, cvm_err_to_cstr(ERR_NAME_ERROR), line_count,
                        "reassignment of existing variable", CS_FMT(castr_cut_by(1, token))
                    );

                    return ERR_VAR_EXIST;
                }

                cvm->variables_stack[cvm->variables_stack_size].name   = castr_cut_by(1, token);
                cvm->variables_stack[cvm->variables_stack_size].ptr    = cvm->variables_stack_size;

                if (arg.length == 0) {
                    fprintf(stderr,
                                    "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                                    cvm->filename, cvm_err_to_cstr(ERR_NO_ARGUMENT), 
                                    line_count, "excepted value after", CS_FMT(token)
                            );
 
                    return ERR_NO_ARGUMENT;
                } else {
                    if (is_str) {
                        cvm->variables_stack[cvm->variables_stack_size].value   = makeObject(arg, "str");
                        cvm->variables_stack[cvm->variables_stack_size].type    = CS("str");
                    } else {
                        if (castr_has('.', arg)) {
                            cvm->variables_stack[cvm->variables_stack_size].value = makeObject(arg, "f64");
                            cvm->variables_stack[cvm->variables_stack_size].type    = CS("f64");  
                        } else { 
                            cvm->variables_stack[cvm->variables_stack_size].value = makeObject(arg, "i64");
                             cvm->variables_stack[cvm->variables_stack_size].type = CS("i64");
                        }
                    }
                }

                cvm->variables_stack_size += 1;
            } else {
                size_t pos = token_exist(token, tok_list);

                if (pos != -1) {
                    cvm->instr_stack[cvm->instr_stack_size].instr = token;
                    cvm->instr_stack[cvm->instr_stack_size].warg  = true;

                    if (tok_list[pos].is_arg) {
                        if (arg.length == 0) {
                            fprintf(stderr,
                                    "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                                    cvm->filename, cvm_err_to_cstr(ERR_NO_ARGUMENT), 
                                    line_count, "excepted value after", CS_FMT(token)
                            );
                            return ERR_NO_ARGUMENT;
                        }

                        if (castr_same(token, CS("goto"))) {
                            if (!is_str && !isdigit(arg.data[0])) {
                                if (label_exist(cvm, arg)) {
                                    cvm->instr_stack[cvm->instr_stack_size].arg.as_int = label_indx(cvm, arg);
                                } else {
                                    fprintf(stderr,
                                            "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                                            cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_LABEL), line_count,
                                            "unknown label", CS_FMT(arg)
                                    );

                                    return ERR_UNKNOWN_LABEL;
                                }
                            } else {
                                cvm->instr_stack[cvm->instr_stack_size].arg = makeObject(arg, "i64");
                            }
                        } else {
                            if (is_str || (!isdigit(arg.data[0]) && arg.data[0] != '-')) {
                                cvm->instr_stack[cvm->instr_stack_size].arg = makeObject(arg, "str");
                            } else {
                                if (castr_has('.', arg)) cvm->instr_stack[cvm->instr_stack_size].arg = makeObject(arg, "f64");
                                else if (isdigit(arg.data[0]) || (arg.data[0] == '-' && isdigit(arg.data[1]))) cvm->instr_stack[cvm->instr_stack_size].arg = makeObject(arg, "i64");
                            }
                        }
                    } else {
                        cvm->instr_stack[cvm->instr_stack_size].arg = makeEmptyObject();
                        cvm->instr_stack[cvm->instr_stack_size].warg = false;
                    }

                    cvm->instr_stack[cvm->instr_stack_size].line   = line_count;
                    cvm->instr_stack_size += 1;
                } else {
                    fprintf(stderr,
                        "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                        cvm->filename, cvm_err_to_cstr(ERR_NAME_ERROR), line_count,
                        "unknown name", CS_FMT(token)
                    );

                    return ERR_NAME_ERROR;
                }
            }
        } else {}
        
        if (!castr_has('\n', source)) source = CS("");
            else source = castr_cutc('\n', source);

        line_count += 1;       
        is_str = false;
    }

    return CATA_OK;
};

#endif