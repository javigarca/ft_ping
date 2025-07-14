#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ft_ping.h"

/**
 * @brief Función de impresion de ayuda copleta del programa
 * 
 */
void    print_help(){
    printf("Usage: ft_ping [options] host\n");
    printf("Options:\n");
    printf("  -v, --verbose      verbose output\n");
    printf("  -c <count>         stop after <count> replies\n");
    printf("  -i <interval>      wait interval seconds between sending each packet\n");
    printf("  -t <ttl>           set the TTL (time to live)\n");
    printf("  -?, --help         display this help and exit\n");
}

/**
 * @brief Función de impresión del uso del programa y consejo de help
 * 
 */
void    print_usage(void){
    printf("Usage: ft_ping [options] host\n");
    printf("Try 'ft_ping --help' or 'ft_ping -?' for more information.\n");
}

 /**
 * @brief Función de impresión de error y salida del programa 
 * 
 * Imprime un mensaje de error personalizado, seguido opcionalmente 
 * del mensaje asociado a un código de error del sistema (usando strerror),
 * y termina la ejecución del programa con el código de salida especificado.
 * 
 * @param status Valor que tomará la función exit() al terminar el programa.
 * @param errnum Código de error del sistema (por ejemplo, errno). Si es 0, no se imprime mensaje de error del sistema.
 * @param fmt Cadena de formato estilo printf para el mensaje de error personalizado.
 * @param ... Argumentos variables usados junto con fmt para construir el mensaje de error.
  */
void    error_exit(int status, int errnum, const char *fmt, ...){
    fprintf(stderr, "ft_ping: ");

    if (fmt)
    {
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }

    if (errnum)
        fprintf(stderr, ": %s", strerror(errnum));

    fprintf(stderr, "\n");
    exit(status);
}