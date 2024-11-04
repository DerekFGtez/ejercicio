#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procesador.h"

//Inicializar el procesador
void init_processor(Processor *cpu)
{
    cpu->PC = 0;
    cpu->loaded_instructions = 0;
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

    cpu->loaded_instructions = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (cpu->loaded_instructions >= MAX_INSTRUCTIONS)
        {
            fprintf(stderr, "Error: Exceso de instrucciones en el archivo\n");
            break;
        }
        
        Instruction *current_instruction = &cpu->code_memory[cpu->loaded_instructions];

        //Leer instrucción en 'current_instruction->inst'
        sscanf(line, "%5s", current_instruction->inst);

        if (current_instruction->inst[0] == 'J') //Verifica si una instrucción inicia con 'J'
        {
            sscanf(line, "%5s %d", current_instruction->inst, &current_instruction->op2); //Para instrucciones de salto, lee solo 'op2'
            current_instruction->op1 = '\0'; //Establece 'op1' a un valor nulo en estos casos
        }
        else
        {
            //Para el resto de las instrucciones, lee de manera normal 'op1' y 'op2'
            sscanf(line, "%5s %c %d", current_instruction->inst, &current_instruction->op1, &current_instruction->op2);
        }

        //Imprimimos la instrucción que ya ha cargado correctamente
        printf("Instrucción cargada: %s %c %d\n", current_instruction->inst, current_instruction->op1, current_instruction->op2);

        cpu->loaded_instructions++;
    }

    fclose(file);
}

void update_flags(Processor *cpu, int result[2])
{
    if (result[0] == 1) //Esto indica si se debe usar el valor de result[1]
    {
        cpu->flags[ZF] = (result[1] == 0); //Activa la Zero Flag si el resultado es 0
        cpu->flags[NF] = (result[1] < 0); //Activa la Negative Flag si el resultado es negativo
    }
    else //Si no, usa los valores de los registros A y B para actualizar las banderas
    {
        cpu->flags[ZF] = (cpu->registers[0] == 0 || cpu->registers[1] == 0);
        cpu->flags[NF] = (cpu->registers[0] < 0 || cpu->registers[1] < 0);
    }
}

void alu_and_control_unity(Processor *cpu)
{
    cpu->total_instructions++;

    Instruction inst = cpu->code_memory[cpu->PC];
    int resultado[2] = {0, 0}; //Inicializa resultado para indicar que no hay resultado específico al inicio
    int registroActual = (inst.op1 == 'A') ? 0 : 1; //Variable para determinar a qué registro aplicar la operación

    printf("\nNúmero de instrucción ejecutada: %d\n", cpu->total_instructions);
    printf("Ejecutando instrucción en PC: %d -> %s %c %d\n", cpu->PC, inst.inst, inst.op1, inst.op2);

    bool actualizar_flags = false; //Bandera para controlar si se deben actualizar las banderas, un poco irónico xd

    if (strcmp(inst.inst, "MOV") == 0)
    {
        cpu->registers[registroActual] = inst.op2;
        actualizar_flags = true;
    }
    else if (strcmp(inst.inst, "ADD") == 0)
    {
        cpu->registers[registroActual] += inst.op2;
        actualizar_flags = true;
    }
    else if (strcmp(inst.inst, "SUB") == 0)
    {
        cpu->registers[registroActual] -= inst.op2;
        actualizar_flags = true;
    }
    else if (strcmp(inst.inst, "CMP") == 0)
    {
        resultado[0] = 1; //Se actualiza result[0] para indicar que result[1] contiene un valor válido
        resultado[1] = cpu->registers[registroActual] - inst.op2;
        actualizar_flags = true;
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
        cpu->registers[registroActual] = cpu->memory[inst.op2];
        printf("Cargando en %c desde memory[%d]: %d\n", (registroActual == 0) ? 'A' : 'B', inst.op2, cpu->memory[inst.op2]);
        resultado[0] = 1;
        resultado[1] = cpu->registers[registroActual];
        actualizar_flags = true;
    }
    else if (strcmp(inst.inst, "STORE") == 0)
    {
        cpu->memory[inst.op2] = cpu->registers[registroActual];
        printf("Almacenando en memory[%d] desde %c: %d\n", inst.op2, (registroActual == 0) ? 'A' : 'B', cpu->registers[registroActual]);
    }
    else
    {
        printf("Error: Instrucción '%s' no válida en la posición %d\n", inst.inst, cpu->PC);
        return;
    }

    if (actualizar_flags)
        update_flags(cpu, resultado);

    print(cpu); //Imprime en pantalla
}

//Imprimir el estado del procesador en la pantalla
void print(Processor *cpu)
{
    printf("PC: %d\n", cpu->PC);
    printf("Registros: A = %d, B = %d\n", cpu->registers[0], cpu->registers[1]);
    printf("Zero Flag: %d, Negative Flag: %d\n", cpu->flags[ZF], cpu->flags[NF]);
}

//Imprimir el estado del procesador en el archivo de salida
void fprint(Processor *cpu)
{
    if (output_file)
    {
        fprintf(output_file, "\nNúmero de instrucción ejecutada: %d\n", cpu->total_instructions);
        fprintf(output_file, "Ejecutando instrucción en PC: %d -> %s %c %d\n", cpu->PC, cpu->code_memory[cpu->PC].inst, cpu->code_memory[cpu->PC].op1, cpu->code_memory[cpu->PC].op2);
        fprintf(output_file, "PC: %d\n", cpu->PC);
        fprintf(output_file, "Registros: A = %d, B = %d\n", cpu->registers[0], cpu->registers[1]);
        fprintf(output_file, "Zero Flag: %d, Negative Flag: %d\n", cpu->flags[ZF], cpu->flags[NF]);
    }
}