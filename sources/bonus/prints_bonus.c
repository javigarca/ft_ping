#include <bits/types/struct_timeval.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <inttypes.h>
#include "ft_ping_bonus.h"

/**
 * @brief Función de impresion de ayuda copleta del programa
 * 
 */
void    print_help(){
    printf("\nUsage:\n  ft_ping [options] <destination>\n");
    printf("Options:\n");
    printf("  --usage            display usage and exit\n");
    printf("  -v, --verbose      verbose output\n");
    printf("  -c <count>         stop after <count> replies\n");
    printf("  -i <interval>      wait interval seconds between sending each packet\n");
    printf("  -p <pattern>       specify up to 16 pad bytes to fill out the packet you send\n");
    printf("  -s <packetsize>    specifies the number of data bytes to be sent (default 56)\n");
    printf("  -t <ttl>           set the TTL (time to live)\n");
    printf("  -r, --route        Bypass routing tables");
    printf("  -?, --help         display this help and exit\n");
}

/**
 * @brief Función de impresión del uso del programa y consejo de help
 * 
 */
void    print_usage(void){
    printf("\nUsage: ft_ping [options] host\n");
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

/**
 * @brief Función para imprimir el resumen de las estadísticas que recibe como parámetro
 * 
 * @param stats estructura de las esteadisticas 
 */
void    print_summary(t_stats *stats){
	double loss = 0.0;
    struct timeval end_ping;
    gettimeofday(&end_ping, NULL);
    long elapsed_time = (end_ping.tv_sec - stats->start_ping.tv_sec) * 1000 + (end_ping.tv_usec - stats->start_ping.tv_usec) / 1000;

	if (stats->transmitted > 0)
		loss = 100.0 * (stats->transmitted - stats->received) / stats->transmitted;

	printf("\n--- %s ping statistics ---\n", stats->target.hostname);
	printf("%d packets transmitted, %d received, %.0f%% packet loss, time %ldms\n",
		stats->transmitted, stats->received, loss, elapsed_time);

	if (stats->received > 0) {
		double avg = stats->rtt_total / stats->received;
		double mdev = 0;
		if (stats->received > 1) {
			double mean_sq = stats->rtt_squared_total / stats->received;
			mdev = sqrt(mean_sq - avg * avg);
		}
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			stats->rtt_min, avg, stats->rtt_max, mdev);
	}
}

/**
 * @brief Función para imprimir mensajes en función de verbose
 * 
 * @param verbose valor del flag  
 * @param fmt Cadena de formato estilo printf para el mensaje personalizado.
 * @param ... Argumentos variables usados junto con fmt para construir el mensaje.
 */

 void   print_infof(int verbose, FILE *stream, const char *fmt, ...){
    if (!verbose)
        return;
    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt,args);
	va_end(args);
	fprintf(stream, "\n");
 }
/**
 * @brief Función debug para imprimir el pattern introducido, en decimal y byte a byte
 * 
 * @param opts la estructura con los datos de las flags
 */
 void print_pattern(const t_ping_options *opts) {
    if (!opts->pattern_use) {
        printf("No pattern set\n");
        return;
    }
    for (size_t i = 0; i < opts->pattern_len; i++) 
        printf("Pattern valor byte(%zu): %u\n", i, (unsigned)opts->pattern[i]);
    printf("Pattern (%zu bytes):", opts->pattern_len);
    for (size_t i = 0; i < opts->pattern_len; i++) {
        printf(" %02x", opts->pattern[i]);
    }
    printf("\n");    
}
/**
 * @brief Función debug para imprimir las ping options
 * 
 * @param opts estructura de ping options
 */
void print_opts(const t_ping_options *opts) {
    printf("=== Parsed options ===\n");
    printf("  verbose       = %d\n", opts->verbose);
    printf("  count         = %d\n", opts->count);
    printf("  interval      = %.6f\n", opts->interval);
    printf("  target        = %s\n", opts->target ? opts->target : "(null)");
    printf("  route         = %d\n", opts->route);
    printf("  use_pattern   = %d\n", opts->pattern_use);
    if (opts->pattern_use) {
        printf("  pattern_len   = %zu\n", opts->pattern_len);
        printf("  pattern bytes =");
        for (size_t i = 0; i < opts->pattern_len; i++) {
            printf(" %02" PRIx8, opts->pattern[i]);
        }
        printf("\n");
    }

    printf("  use_size      = %d\n", opts->payload_size_use);
    if (opts->payload_size_use)
        printf("  packetsize    = %zu\n", opts->payload_size);

    printf("======================\n");
}