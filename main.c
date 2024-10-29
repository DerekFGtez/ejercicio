#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "procesador.h"  // Incluye la definición de Processor y las funciones

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);

    // Verificar si se ha proporcionado el nombre del archivo de instrucciones
    if (argc < 2)
    {
        fprintf(stderr, "Error: Se requiere el nombre del archivo de instrucciones.\n");
        fprintf(stderr, "Uso: %s <archivo de instrucciones>\n", argv[0]);
        return 1;
    }

    // Crear e inicializar el procesador
    Processor cpu;
    init_processor(&cpu);

    // Cargar el programa desde el archivo
    load_program(&cpu, argv[1]);

    // Ejecutar las instrucciones
    alu_and_control_unity(&cpu);

    // Imprimir el estado final del procesador
    fprint(&cpu);

    return 0;
}