/*#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "procesador.h"

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);

    if (argc < 2)
    {
        fprintf(stderr, "Error: Se requiere el nombre del archivo de instrucciones.\n");
        fprintf(stderr, "Uso: %s <archivo de instrucciones>\n", argv[0]);
        return 1;
    }

    Processor cpu;
    init_processor(&cpu); //Inicializa el procesador

    load_program(&cpu, argv[1]); //Carga el programa desde el archivo

    int max_iterations = 100;

    while (cpu.PC < cpu.total_instructions && max_iterations > 0) //Bucle de la ejecución de cada instrucción
    {
        alu_and_control_unity(&cpu); //Ejecuta cada instrucción del archivo de texto
        max_iterations--;
    }

    if (max_iterations == 0)
    {
        printf("Se alcanzó el límite máximo de iteraciones, posible bucle infinito\n");
    }

    fprint(&cpu); //Imprime el estado final del procesador

    return 0;
}*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "procesador.h"

FILE *output_file; // Variable global para el archivo de salida

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);

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
        fprintf(stderr, "Error al abrir el archivo de salida.\n");
        return 1;
    }

    load_program(&cpu, argv[1]);

    while (cpu.PC < cpu.total_instructions)
    {
        alu_and_control_unity(&cpu);
        fprint(&cpu); //Imprime en archivo
        //printf("Número de instrucción ejecutada: %d\n", cpu.PC + 1);

        if (cpu.PC >= cpu.total_instructions)
        {
            break;
        }

        cpu.PC++;
    }

    fclose(output_file); // Cierra el archivo de salida al final
    return 0;
}