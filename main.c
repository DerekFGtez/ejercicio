#include <stdio.h>
#include <stdlib.h>
#include "procesador.h"

FILE *output_file; //Archivo de salida

int main(int argc, char *argv[])
{
    Processor cpu;
    init_processor(&cpu);

    if (argc < 3)
    {
        fprintf(stderr, "Uso: %s <archivo_instrucciones> <archivo_salida>\n", argv[0]);
        return 1;
    }

    output_file = fopen(argv[2], "w");
    if (!output_file)
    {
        fprintf(stderr, "Error al abrir el archivo de salida\n");
        return 1;
    }

    load_program(&cpu, argv[1]);

    fprintf(output_file, "---------------------------------------------");

    while (cpu.PC < cpu.loaded_instructions)
    {
        alu_and_control_unity(&cpu);
        fprint(&cpu); //Imprime el estado en el archivo

        cpu.PC++; //Incrementa el contador del programa después de cada instrucción
    }

    printf("\nNúmero total de instrucciones ejecutadas: %d", cpu.total_instructions);
    fprintf(output_file, "\nNúmero total de instrucciones ejecutadas: %d\n", cpu.total_instructions);
    fprintf(output_file, "---------------------------------------------");

    fclose(output_file);
    return 0;
}