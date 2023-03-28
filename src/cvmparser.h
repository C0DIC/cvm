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

#ifndef CVM_INSTR
    #include "cvminstr.h"
#endif

#include "catavm.h"

#ifndef CVM_ERROR
    #include "cvmerror.h"
#endif

CataError parse_insts(CataStr source, CataVM *cvm);

bool label_exist(CataVM *cvm, CataStr label);

#endif

#ifdef CVM_PARSER

bool label_exist(CataVM *cvm, CataStr label) {
    for (size_t i = 0; i < cvm->labels_stack_size; i++) {
        if (castr_same(label, castr_untilc(':', cvm->labels_stack[i].instr))) {
            return true;
        }
    }

    return false;
}

size_t label_indx(CataVM *cvm, CataStr label) {
    for (size_t i = 0; i < cvm->labels_stack_size; i++)
        if (castr_same(castr_untilc(':', cvm->labels_stack[i].instr), label)) {
            return cvm->labels_stack[i].line;
    }

    return -1;
}

CataError parse_insts(CataStr source, CataVM *cvm) {
    CataStr line         = {0};
    CataStr token        = {0};
    CataStr arg          = {0};
    size_t  line_count   = 1;

    CataStr cp_source = source;
    Token tok_list[18]; createTokenList(tok_list);

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
            } else {
                cvm_error(
                    cvm->filename, cvm->instr_stack[cvm->instr_pos].line,
                    ERR_SYNTAX_ERROR, "\" never closed");
                return ERR_SYNTAX_ERROR;
            }
        } else {
            arg    = castr_ltrim(' ', castr_untilc(' ', line));
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
            } else {
                for (size_t i = 0; i < TOKEN_COUNT; i++) {

                    if (castr_same(token, tok_list[i].value)) {
                        cvm->instr_stack[cvm->instr_stack_size].instr = token;

                        if (tok_list[i].is_arg) {
                            if (arg.length == 0) {
                                fprintf(stderr,
                                        "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                                        cvm->filename, cvm_err_to_cstr(ERR_NO_ARGUMENT), 
                                        line_count, "excepted value after", CS_FMT(token)
                                );
                                return ERR_NO_ARGUMENT;
                            }

                            if (castr_same(token, CS("goto"))){
                                cvm->instr_stack[cvm->instr_stack_size].instr = token;

                                if (isalpha(arg.data[0])) {
                                    if (label_exist(cvm, arg)) {
                                        cvm->instr_stack[cvm->instr_stack_size].arg.as_int = label_indx(cvm, arg);
                                        cvm->instr_stack[cvm->instr_stack_size].line       = line_count;
                                        cvm->instr_stack_size += 1;
                                        continue;
                                    } else {
                                        fprintf(stderr,
                                            "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                                            cvm->filename, cvm_err_to_cstr(ERR_UNKNOWN_LABEL), line_count,
                                            "unknown label", CS_FMT(arg)
                                        );

                                        return ERR_UNKNOWN_LABEL;
                                    }
                                } else {
                                    cvm->instr_stack[cvm->instr_stack_size].arg   = makeObject(arg);
                                    cvm->instr_stack[cvm->instr_stack_size].line   = line_count;
                                    cvm->instr_stack_size += 1;
                                    continue;
                                }
                            }

                            if (castr_startswith("\"", arg)) {
                                arg = castr_cut_by(1, arg);
                                cvm->instr_stack[cvm->instr_stack_size].arg.as_string   = castr_untilc('"', arg);
                            } else {
                                cvm->instr_stack[cvm->instr_stack_size].arg   = makeObject(arg);
                            }

                        } else {
                            cvm->instr_stack[cvm->instr_stack_size].arg = makeObject(arg);
                        }

                        cvm->instr_stack[cvm->instr_stack_size].line   = line_count;
                        cvm->instr_stack_size += 1;
                    } else {
                        if (i < TOKEN_COUNT) {
                            continue;
                        } else {
                            fprintf(stderr,
                                "%s:\n  |___%s: %lu: %s `"CS_PRI"`\n",
                                cvm->filename, cvm_err_to_cstr(ERR_NAME_ERROR), line_count,
                                "unknown name", CS_FMT(token)
                            );

                            return ERR_NAME_ERROR;
                        }
                    }
                }
            }
        }
        
        if (!castr_has('\n', source)) source = CS("");
            else source = castr_cutc('\n', source);

        line_count += 1;
        
    }

    return CATA_OK;
};

#endif