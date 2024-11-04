#include <stdbool.h>

#define MEM_SIZE 256 // Tamaño de la memoria
#define MAX_INSTRUCTIONS 100 // Número máximo de instrucciones
#define NUM_REGISTERS 2 // Número de registros (A y B)
#define NUM_FLAGS 2 // Número de banderas
// Definimos para las banderas.
#define ZF 0 // Zero Flag
#define NF 1 // Negative Flag

extern FILE *output_file; // Declaración del archivo de salida

typedef struct
{
    char inst[6];
    char op1;
    int op2;
} Instruction;

// Estructura que representa el procesador
typedef struct
{
    int memory[MEM_SIZE];
    int registers[NUM_REGISTERS];
    bool flags[NUM_FLAGS];
    int PC; // Contador de programa.
    int loaded_instructions;
    int total_instructions; //Una última variable extra para imprimir el número total de instrucciones ejecutadas
    Instruction code_memory[MAX_INSTRUCTIONS]; // Memoria de instrucciones
} Processor;

// Funciones del procesador
void init_processor(Processor *cpu);
void load_program(Processor *cpu, const char *filename);
void update_flags(Processor *cpu, int result[2]);
void alu_and_control_unity(Processor *cpu);
void print(Processor *cpu);
void fprint(Processor *cpu);