#include <unistd.h>
#include <string.h>     // memset, strerror
#include <netdb.h>      // getaddrinfo, freeaddrinfo, addrinfo
#include <netinet/in.h> // sockaddr_in
#include <stdio.h>      
#include <stdlib.h>     
#include <errno.h>

#include "ft_ping.h"
#include "ft_ping_structs.h"

/**
 * @brief Extrae las flags y el host de los argumentos introducidos, evalúa y ejecuta según sea necesario.
 * 
 * @param argc número de argumentos de main
 * @param argv valor de los argumentos de main
 * @param opts estructura de opciones para dar valor según sea el caso
 */

void parse_args(int argc, char *argv[], t_ping_options *opts){

    if (argc < 2)
        error_exit(EXIT_FAILURE, 0, "Usage: ft_ping [options] host.");

    int i = 1;
    int hostcont = 0;

    while (argv[i]) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-'){
                if (!strcmp(argv[i], "--help")){
                        print_help();
                        exit(EXIT_SUCCESS);
                    }
                if (!strcmp(argv[i], "--verbose")){
                    opts->verbose = 1;
                }
            } else {
                for (int j = 1; argv[i][j]; j++){
                    switch (argv[i][j]) {
                        case 'v': 
                            opts->verbose=1;
                            break;
                        case '?': 
                            print_help(); 
                            exit(EXIT_SUCCESS);
                        default: 
                            error_exit(EXIT_FAILURE, 0, "Unknown option: -%c", argv[i][j]);
                    }
                }
            }
            
        }
        else {
            hostcont++;
            opts->target = argv[i]; 
        }
        i++;
    }

    if (hostcont == 0) {
        error_exit(EXIT_FAILURE, 0, "Missing host operand");
    }
    if (hostcont != 1) {
        error_exit(EXIT_FAILURE, 0,"only one destination allowed");
    }
}

int resolve_target(t_ping_options *opts, t_target *t_out){
    if (!opts->target || strlen(opts->target) == 0)
	    error_exit(EXIT_FAILURE, 0, "Empty host name");

    if (strlen(opts->target) > 255)
	    error_exit(EXIT_FAILURE, 0, "Host name too long");
    
    struct addrinfo hints;
	struct addrinfo *result;
	int ret;

	// Preparar estructura de filtro
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // Solo IPv4
	hints.ai_socktype = SOCK_RAW;    // Tipo de socket (aunque no importa mucho aquí)
	hints.ai_protocol = IPPROTO_ICMP;

	// Resolver
	ret = getaddrinfo(opts->target, NULL, &hints, &result);
	if (ret != 0) {
		//printinfof(1,stderr, "ft_ping: cannot resolve %s: %s\n", opts->target, gai_strerror(ret));
        error_exit(EXIT_FAILURE, 0, "%s: %s\n", opts->target, gai_strerror(ret));
        //exit(EXIT_FAILURE);
		//return -1;
	}

	// Extraer sockaddr_in
	struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
	t_out->addr = *addr;

	// Convertir IP a string
	if (!inet_ntop(AF_INET, &addr->sin_addr, t_out->ip_str, INET_ADDRSTRLEN)) {
		freeaddrinfo(result);
		error_exit(EXIT_FAILURE, errno, "inet_ntop");
	}

	// Guardar hostname original si lo vas a usar (opcional)
	t_out->hostname = strdup(opts->target); 

	freeaddrinfo(result);
	return 0;
}