#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define CVM_READER
#include "cvmreader.h"

#define CVM_ERROR
#include "cvmerror.h"

#define CVM_INSTR
#include "cvminstr.h"

#define CVM_EXECUTE
#include "cvmexecute.h"

#define CVM_PARSER
#include "cvmparser.h"

#include "catavm.h"

// Function to get arguments when running program
// Example:
// ./catavm file.cava
//
// First using of get_arg will return ./catavm
static char *get_arg(int *argc, char ***argv) {
    assert(*argc > 0);
    char *arg = **argv;
    *argv += 1;
    *argc -= 1;

    return arg;
}

// Get file extension (should be `.cava`)
static const char *get_ext(const char *file_path) {
    const char *ext = strrchr(file_path, '.');

    if (!ext) return ""; else return ext + 1;
}

// Get only file name, without full_path
/*
static const char *lazy_filename(const char *file_path) {
    const char *slash = strrchr(file_path, '/');

    if (!slash) return file_path; else return slash + 1;
}
*/

CataVM CVM = {0};

int main(int argc, char **argv) {
    const char *const program = get_arg(&argc, &argv);

    if (argc < 2) {
        printf("Usage: %s <file> <flags>\n\n", program);
        printf("Flags:\n");
        printf("     -r                 Run  .cava-file\n");
        printf("     -o   <output.cabc> Save .cava-file to byte-code file\n");
        exit(1);
    }

    const char *input_file_path  = NULL;
    //const char *in_filename      = NULL;
    const char *ouput_file_path  = NULL;

    __u_short run                = 0;

    while (argc > 0) {
        const char *arg = get_arg(&argc, &argv);

        if (!strcmp(get_ext(arg), "cava")) {
            input_file_path = arg;
            //in_filename  = lazy_filename(input_file_path);
        } 

        arg = get_arg(&argc, &argv);

        if (!strcmp(arg, "-o")) {
            if (argc != 0) {
                ouput_file_path = get_arg(&argc, &argv);

                if (strcmp(get_ext(ouput_file_path), "cabc")) {
                    fprintf(stderr, "<error>: no .cabc file provided\n");
                    exit(1);
                }
            } else {
                fprintf(stderr, "<error>: no output file provided after `-o` flag\n");
                exit(1);
            }
        } else if (!strcmp(arg, "-r")) {
            run = 1;
        } else {
            fprintf(stderr, "<error>: unknown flag `%s`\n", arg);
            exit(1);
        }
    }

    if (!input_file_path) {
        fprintf(stderr, "<error>: no .cava file provided\n");
        exit(1);
    }
    
    FILE *cava_file   = fopen(input_file_path, "r");
    char *cava_source = readFile(&cava_file, input_file_path);

    CVM.filename      = input_file_path;

    CataError exec_prog = parse_insts(CS(cava_source), &CVM);

    if (exec_prog == CATA_OK) {
        if (ouput_file_path) {
            assert(0 && "<error>: not implemented yet");
        }
        else {
            if (run) {
                cvm_execute(&CVM);
            }
        }
    }

    free(cava_file);

    return 0;
}