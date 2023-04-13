/*
*
*
*       catavm.h
*
*       Created by C0DIC, 21/03/2023
*
*
*       General implementation of Cata Virtual Machine
*
*
*/

#ifndef CVM_VIRTUAL_MACHINE_H
#define CVM_VIRTUAL_MACHINE_H

#include <stdio.h>

#define STACK_SIZE 8192

// Cata Virtual Machine structure
typedef struct CataVirtualMachine {
    Object  stack[STACK_SIZE];
    size_t  stack_size;

    CVMInst instr_stack[STACK_SIZE];
    size_t  instr_stack_size;
    size_t  instr_pos;

    CVMLabel labels_stack[STACK_SIZE];
    size_t   labels_stack_size;

    Variable variables_stack[STACK_SIZE];
    size_t   variables_stack_size;

    __u_short halted;

    const char *filename;
} CataVM;

#endif
