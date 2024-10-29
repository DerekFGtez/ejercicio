#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procesador.h"

static int total_instructions = 0;

// Función para inicializar el procesador
void init_processor(Processor *cpu)
{
    cpu->PC = 0;  // Inicializamos el contador de programa a 0
    total_instructions = 0; // Reiniciamos el contador de instrucciones a 0
    
    // Inicializamos los registros A y B a 0
    for (int i = 0; i < NUM_REGISTERS; i++) {
        cpu->registers[i] = 0;
    }

    // Inicializamos las banderas Zero y Negative a 0
    for (int i = 0; i < NUM_FLAGS; i++) {
        cpu->flags[i] = 0;
    }

    // Limpiamos la memoria de código
    memset(cpu->code_memory, 0, sizeof(cpu->code_memory));

    // Limpiamos la memoria principal (256 bytes)
    memset(cpu->memory, 0, MEM_SIZE * sizeof(int));
}

// Función para cargar el programa desde un archivo
void load_program(Processor *cpu, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: No se pudo abrir el archivo %s\n", filename);
        exit(1);
    }

    total_instructions = 0;
    char line[100];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (total_instructions >= MAX_INSTRUCTIONS)
        {
            fprintf(stderr, "Error: Exceso de instrucciones en el archivo\n");
            break;
        }

        // Ignorar comentarios después del punto y coma
        char *comment_pos = strchr(line, ';');
        if (comment_pos)
            *comment_pos = '\0';

        char instruction[6];
        char operando1;
        int operando2 = 0;

        sscanf(line, "%5s %c %d", instruction, &operando1, &operando2);

        strtok(instruction, " \n\r\t");

        printf("Instrucción cargada: %s %c %d\n", instruction, operando1, operando2);

        // Almacena la instrucción en code_memory
        strcpy(cpu->code_memory[total_instructions].inst, instruction);
        cpu->code_memory[total_instructions].op1 = operando1;
        cpu->code_memory[total_instructions].op2 = operando2;

        total_instructions++;
    }

    fclose(file);
}

// Función para actualizar las banderas después de una operación aritmética
void update_flags(Processor *cpu, int result)
{
    cpu->flags[ZF] = (result == 0) ? 1 : 0;
    cpu->flags[NF] = (result < 0) ? 1 : 0;
}

//Función para ejecutar acciones
void alu_and_control_unity(Processor *cpu)
{
    int max_iterations = 100; // Límite de iteraciones para evitar bucles infinitos

    while (cpu->PC < total_instructions && max_iterations > 0)
    {
        Instruction inst = cpu->code_memory[cpu->PC];

        if (strcmp(inst.inst, "MOV") == 0)
        {
            if (inst.op1 == 'A')
                cpu->registers[0] = inst.op2;
            else if (inst.op1 == 'B')
                cpu->registers[1] = inst.op2;
        }
        else if (strcmp(inst.inst, "ADD") == 0)
        {
            if (inst.op1 == 'A')
                cpu->registers[0] += inst.op2;
            else if (inst.op1 == 'B')
                cpu->registers[1] += inst.op2;

            update_flags(cpu, (inst.op1 == 'A') ? cpu->registers[0] : cpu->registers[1]);
        }
        else if (strcmp(inst.inst, "SUB") == 0)
        {
            if (inst.op1 == 'A')
                cpu->registers[0] -= inst.op2;
            else if (inst.op1 == 'B')
                cpu->registers[1] -= inst.op2;

            update_flags(cpu, (inst.op1 == 'A') ? cpu->registers[0] : cpu->registers[1]);
        }
        else if (strcmp(inst.inst, "CMP") == 0)
        {
            int result = cpu->registers[inst.op1 == 'A' ? 0 : 1] - inst.op2;
            update_flags(cpu, result);
        }
        else if (strcmp(inst.inst, "JMP") == 0)
        {
            cpu->PC = inst.op2 - 1; // Ajuste para que el siguiente ciclo apunte correctamente
        }
        else if (strcmp(inst.inst, "JZ") == 0)
        {
            if (cpu->flags[ZF])
                cpu->PC = inst.op2 - 1;
            else
                cpu->PC++;

            continue;
        }
        else if (strcmp(inst.inst, "JN") == 0)
        {
            if (cpu->flags[NF])
                cpu->PC = inst.op2 - 1;
            else
                cpu->PC++;
            
            continue;
        }
        else if (strcmp(inst.inst, "LOAD") == 0)
        {
            cpu->registers[inst.op1 == 'A' ? 0 : 1] = cpu->memory[inst.op2];
        }
        else if (strcmp(inst.inst, "STORE") == 0)
        {
            cpu->memory[inst.op2] = cpu->registers[inst.op1 == 'A' ? 0 : 1];
        }
        else
        {
            printf("Error: Opcode no reconocido %s en la posición %d\n", inst.inst, cpu->PC);
        }

        // Avanzar el PC a la siguiente instrucción solo si no es un salto
        cpu->PC++;
        max_iterations--;

        if (cpu->PC >= total_instructions) {
            break;
        }
    }

    if (max_iterations == 0) {
        printf("Se alcanzó el límite máximo de iteraciones, posible bucle infinito\n");
    }
}

//Función para imprimir el estado del procesador en la pantalla
void fprint(Processor *cpu)
{
    printf("PC: %d\n", cpu->PC);
    printf("Registros: A = %d, B = %d\n", cpu->registers[0], cpu->registers[1]);
    printf("Zero Flag: %d, Negative Flag: %d\n", cpu->flags[ZF], cpu->flags[NF]);   
}