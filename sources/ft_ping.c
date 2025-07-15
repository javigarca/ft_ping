#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include "ft_ping.h"
#include "ft_ping_structs.h"

/**
 * @brief Flujo principal del programa
 * 
 * recibe los argumentos, inicializa estructuras, parseo de argumentos, creación de socket, envío de socket y respuesta. Tratamiento de señales, crtl+C. 
 *
 * varible global de puntero controlado a la dirección de la estructura de la estadística para solo usarla en el caso de imprimir el resumen de las estadísticas, 
 * y no andar cambiando glabales todo el tiempo.

 * @param argc 
 * @param argv 
 * @return int 
 */

static t_stats *g_stats_ref = NULL;

int main (int argc, char **argv)
{
    t_ping_options opts = {0};
    t_stats stats = {0};
    g_stats_ref = &stats;

    parse_args(argc, argv, &opts);   
    signal(SIGINT, handle_sigint);

    int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_fd < 0)
        error_exit(EXIT_FAILURE, errno, "socket" );

    print_infof(opts.verbose, stderr, "ft_ping: sock4.fd: %d (socktype: SOCK_RAW), sock6.fd: -1 (socktype: SOCK_RAW), hints.ai_family: AF_INET.\n", socket_fd);

    t_target host_target = {0};
    if (resolve_target(&opts, &host_target))
        error_exit(EXIT_FAILURE, 0, "Error resolving host.");
    
    int seq = 1;
    while(1){
        send_packet(socket_fd, &opts, &host_target, &stats, seq);
       //receive_packet(socket_fd, seq, &opts, &stats);
        seq++;
        sleep(1);
    }
   
    return (EXIT_SUCCESS);
}

void    handle_sigint(int signum){
    (void)signum;
    if (g_stats_ref)
		print_summary(g_stats_ref);
    exit(EXIT_SUCCESS);
}