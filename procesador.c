#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procesador.h"

void init_processor(Processor *cpu) {
    cpu->PC = 0;
    for (int i=0; i < NUM_REGISTERS; i++) {
        cpu->registers[i] = 0;
    }

    for (int i=0; i < NUM_FLAGS; i++){
        cpu->flags[i] = 0;
    }

    memset(cpu->code_memory, 0, sizeof(cpu->code_memory));
    memset(cpu->memory, 0, MEM_SIZE);
}

void load_program(Processor *cpu, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file){
        fprintf(stderr, "Error: El archivo no se pudo abrir  %s\n", filename);
        exit(1);
    }

    int opcode, operando1, operando2;
    int instruction_count = 0;

    while (fscanf(file, %d %d  %d, &opcode, &operando1; &operando2) == 3) {
        if (instruction_count >= MAX MAX_INSTRUCTIONS) {
            fprintf(stderr, "Error: Son demasiadas instrucciones para el archivo\n");
            break;
        }
        cpu->code_memory[instruction_count].opcode = opcode;
        cpu->code_memory[instruction_count].operando1 = operando1;
        cpu->code_memory[instruction_count].operando2 = operando2;
        instruction_count++;
    }
    fclose(file);
}

void update_flags(Processor *cpu, int result){
    cpu->flags[ZF] = (result == 0) ? 1 : 0;
    cpu->flags[NF] = (result < 0) ? 1 : 0;
}
//archivo