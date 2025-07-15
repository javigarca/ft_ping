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
    //Activar IP_RECVTTL en el socket:
    int opt = 1;
    setsockopt(socket_fd, IPPROTO_IP, IP_RECVTTL, &opt, sizeof(opt));
    
    if (resolve_target(&opts, &stats.target))
        error_exit(EXIT_FAILURE, 0, "Error resolving host.");
    get_socket_info(socket_fd, &stats);
    ////impresión cabeceras
    print_infof(opts.verbose, stderr, "ft_ping: sock4.fd: %d (socktype: %s), sock6.fd: -1 (not used), hints.ai_family: %s.\n", socket_fd, stats.socket_i.socktype_str, stats.socket_i.family_str);
    print_infof(opts.verbose,stdout, "ai->ai_family: %s, ai->ai-canonname: '%s'", stats.socket_i.family_str, stats.target.hostname);
    print_infof(1, stdout, "PING %s (%s) %d data bytes", stats.target.hostname, stats.target.ip_str, PAYLOAD_SIZE);

    int seq = 1;
    while(1){
        send_packet(socket_fd, &opts, &stats, seq);
       // receive_packet(socket_fd, seq, &opts, &stats);
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