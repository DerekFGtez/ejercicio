#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procesador.h"

void init_processor(Processor *cpu)
{
    memset(cpu->memory, 0, MEM_SIZE);

    /*cpu->registers[0] = 0;
    cpu->registers[1] = 0;*/

    memset(cpu->registers, 0, NUM_REGISTERS);

    /*cpu->flags[ZF] = false;
    cpu->flags[NF] = false;*/

    memset(cpu->flags, 0, NUM_FLAGS);    

    cpu->PC = 0;
}

void load_program(Processor *cpu, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        exit(1);
    }
}