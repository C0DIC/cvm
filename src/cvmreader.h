/*
*
*
*       catareader.h
*
*       Created by C0DIC, 21/03/2023
*
*
*       Read and extract data from files
*
*
*/


#ifndef CVM_READER_H
#define CVM_READER_H

#include <stdio.h>
#include <stdlib.h>

char *readFile(FILE **cava_file, const char *filename);

#endif

#ifdef CVM_READER

char *readFile(FILE **cava_file, const char *filename) {
    if (!*cava_file) {
        fprintf(stderr, "<error>: no such file `%s`: %s\n", filename, strerror(errno));
        exit(1);
    }

    fseek(*cava_file, 0, SEEK_END);
    size_t file_size = ftell(*cava_file);
    fseek(*cava_file, 0, SEEK_SET);

    char *soure_cava = (char*)malloc(file_size + 1);
    fread(soure_cava, 1, file_size, *cava_file);

    return soure_cava;
};

#endif