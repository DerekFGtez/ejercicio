#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procesador.h"

//Inicializar el procesador
void init_processor(Processor *cpu)
{
    cpu->PC = 0;
    cpu->total_instructions = 0;
    
    //Iniciando los registros A y B
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu->registers[i] = 0;
    }

    //Inicializando las banderas
    for (int i = 0; i < NUM_FLAGS; i++) {
        cpu->flags[i] = 0;
    }

    memset(cpu->code_memory, 0, sizeof(cpu->code_memory));

    memset(cpu->memory, 0, MEM_SIZE * sizeof(int));
}

//Cargar el programa desde un archivo
void load_program(Processor *cpu, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", filename);
        exit(1);
    }

    cpu->total_instructions = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (cpu->total_instructions >= MAX_INSTRUCTIONS)
        {
            fprintf(stderr, "Error: Exceso de instrucciones en el archivo\n");
            break;
        }
        
        char instruction[6];
        char operando1;
        int operando2 = 0;

        sscanf(line, "%5s %c %d", strcpy(cpu->code_memory[cpu->total_instructions].inst, instruction), &cpu->code_memory[cpu->total_instructions].op1, &cpu->code_memory[cpu->total_instructions].op2);

        //printf("Instrucción cargada: %s %c %d\n", instruction, operando1, operando2);

        cpu->total_instructions++;
    }

    fclose(file);
}

void update_flags(Processor *cpu, int result)
{
    cpu->flags[ZF] = (result == 0) ? 1 : 0;
    cpu->flags[NF] = (result < 0) ? 1 : 0;
}

void alu_and_control_unity(Processor *cpu)
{
    Instruction inst = cpu->code_memory[cpu->PC];
    int resultado = 0;  //Variable para almacenar el resultado de la operación actual

    if (strcmp(inst.inst, "MOV") == 0)
    {
        if (inst.op1 == 'A')
        {
            cpu->registers[0] = inst.op2;
            resultado = cpu->registers[0];
        }
        else if (inst.op1 == 'B')
        {
            cpu->registers[1] = inst.op2;
            resultado = cpu->registers[1];
        }
    }
    else if (strcmp(inst.inst, "ADD") == 0)
    {
        if (inst.op1 == 'A')
        {
            cpu->registers[0] += inst.op2;
            resultado = cpu->registers[0];
        }
        else if (inst.op1 == 'B')
        {
            cpu->registers[1] += inst.op2;
            resultado = cpu->registers[1];
        }
    }
    else if (strcmp(inst.inst, "SUB") == 0)
    {
        if (inst.op1 == 'A')
        {
            cpu->registers[0] -= inst.op2;
            resultado = cpu->registers[0];
        }
        else if (inst.op1 == 'B')
        {
            cpu->registers[1] -= inst.op2;
            resultado = cpu->registers[1];
        }
    }
    else if (strcmp(inst.inst, "CMP") == 0)
    {
        resultado = cpu->registers[inst.op1 == 'A' ? 0 : 1] - inst.op2;
    }
    else if (strcmp(inst.inst, "JZ") == 0)
    {
        if (cpu->flags[ZF])
        {
            cpu->PC = inst.op2 - 1;
            return;
        }
    }
    else if (strcmp(inst.inst, "JN") == 0)
    {
        if (cpu->flags[NF])
        {
            cpu->PC = inst.op2 - 1;
            return;
        }
    }
    else if (strcmp(inst.inst, "JMP") == 0)
    {
        cpu->PC = inst.op2 - 1;
        return;
    }
    else if (strcmp(inst.inst, "LOAD") == 0)
    {
        if (inst.op1 == 'A') {
            cpu->registers[0] = cpu->memory[inst.op2];
            resultado = cpu->registers[0];
        }
        else if (inst.op1 == 'B') {
            cpu->registers[1] = cpu->memory[inst.op2];
            resultado = cpu->registers[1];
        }
    }
    else if (strcmp(inst.inst, "STORE") == 0)
    {
        cpu->memory[inst.op2] = cpu->registers[inst.op1 == 'A' ? 0 : 1];
        return;
    }
    else
    {
        printf("Error: Opcode no reconocido %s en la posición %d\n", inst.inst, cpu->PC);
        return;
    }

    update_flags(cpu, resultado);

    print(cpu); //Imprime en pantalla
}

//Imprimir el estado del procesador en la pantalla
void print(Processor *cpu)
{
    printf("PC: %d\n", cpu->PC);
    printf("Registros: A = %d, B = %d\n", cpu->registers[0], cpu->registers[1]);
    printf("Zero Flag: %d, Negative Flag: %d\n\n", cpu->flags[ZF], cpu->flags[NF]);
}

//Imprimir el estado del procesador en el archivo de salida
void fprint(Processor *cpu)
{
    if (output_file) {
        fprintf(output_file, "PC: %d\n", cpu->PC);
        fprintf(output_file, "Registros: A = %d, B = %d\n", cpu->registers[0], cpu->registers[1]);
        fprintf(output_file, "Zero Flag: %d, Negative Flag: %d\n\n", cpu->flags[ZF], cpu->flags[NF]);
    }
}