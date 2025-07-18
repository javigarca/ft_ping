#include <stdint.h>
#include <unistd.h>
#include <string.h>     // memset, strerror
#include <netdb.h>      // getaddrinfo, freeaddrinfo, addrinfo
#include <netinet/in.h> // sockaddr_in
#include <stdio.h>      
#include <stdlib.h>     
#include <errno.h>
#include <limits.h>

#include "ft_ping_bonus.h"
#include "ft_ping_definitions_bonus.h"
#include "ft_ping_structs_bonus.h"

/**
 * @brief Extrae las flags y el host de los argumentos introducidos, evalúa y ejecuta según sea necesario.
 * 
 * @param argc número de argumentos de main
 * @param argv valor de los argumentos de main
 * @param opts estructura de opciones para dar valor según sea el caso
 */

void parse_args(int argc, char *argv[], t_ping_options *opts){

    if (argc < 2){
        print_usage();
        exit(EXIT_SUCCESS);
    }

    int i = 1;
    int hostcont = 0;
    char flag;
    char *val = NULL;

    while (argv[i]) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            if (argv[i][1] == '-'){
                if (!strcmp(argv[i], "--help")){
                        print_help();
                        exit(EXIT_SUCCESS);
                    }
                    if (!strcmp(argv[i], "--verbose")){
                        opts->verbose = 1;
                    }
                    if (!strcmp(argv[i], "--usage")){
                        print_usage();
                        exit(EXIT_SUCCESS);
                    }
                    error_exit(EXIT_FAILURE, 0, "Unknown option: %s", argv[i]);   
            } else {
                for (int j = 1; argv[i][j]; j++){
                    switch (argv[i][j]) {
                        case 'v': 
                            opts->verbose=1;
                            break;
                        case '?': 
                            print_help(); 
                            exit(EXIT_SUCCESS);
                        case 's':
                        case 'p':
                        case 'i':
                        case 'c':
                            flag = argv[i][j];
                            // Caso "-c123" (sin espacio)
                            if (argv[i][j+1] != '\0') {
                                val = &argv[i][j+1];
                            } else {
                                // Caso "-c 123" (espacio separado)
                                if (i + 1 >=argc)
                                    error_exit(EXIT_FAILURE, 0, "Option -%c requires an argument", argv[i][j]);
                                val = argv[++i];
                            }
                            validate_flag_arg(val, flag, opts);
                            // Salimos del inner-loop para no reexaminar los dígitos de "123"
                            j = strlen(argv[i]) - 1;
                            break;
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

    print_opts(opts);
    if (hostcont == 0) {
        error_exit(EXIT_FAILURE, 0, "Missing host operand");
    }
    if (hostcont != 1) {
        error_exit(EXIT_FAILURE, 0,"only one destination allowed");
    }
}

/**
 * @brief Valida el host introducido por el usuario y lo introudce en target. devuelve 0 si todo bien
 * 
 * @param opts 
 * @param t_out 
 * @return int 
 */
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
		error_exit(EXIT_FAILURE, 0, "%s: %s", opts->target, gai_strerror(ret));
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
	t_out->hostname = opts->target; 

	freeaddrinfo(result);
	return (0);
}

/**
 * @brief Devuelve la información de tipo de socket y familia. devulve 0 si todo bien
 * 
 * @param sockfd 
 * @param stats 
 * @return int 
 */
int get_socket_info(int sockfd, t_stats *stats) {
    socklen_t optlen = sizeof(int);

    // Obtener tipo real del socket
    if (getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &stats->socket_i.socktype, &optlen) < 0)
        error_exit(EXIT_FAILURE, errno, "getsockopt SO_TYPE failed");

    // Traducir tipo a string
    stats->socket_i.socktype_str = (stats->socket_i.socktype == SOCK_RAW) ? "SOCK_RAW" :
                        (stats->socket_i.socktype == SOCK_DGRAM) ? "SOCK_DGRAM" :
                        (stats->socket_i.socktype == SOCK_STREAM) ? "SOCK_STREAM" : "UNKNOWN";

    // Guardar familia (usada en hints o result) // de momento en mandatory con ip4 nos vale así
    stats->socket_i.family = AF_INET;
    stats->socket_i.family_str = (stats->socket_i.family == AF_INET) ? "AF_INET" :
                    (stats->socket_i.family == AF_INET6) ? "AF_INET6" : "UNSPEC";
    
    return(0);
}

/**
 * @brief Función para validar el argumento de las flag que aceptan un número de arg como -c e -i. Salimos con error en caso negativo
 * 
 * @param value valor que recibimos del argumento
 * 
 */
void validate_flag_arg(char *value, char flag, t_ping_options *opts){
    errno = 0;
    char *endptr;
    if (flag == 'c'){
        long cnt = strtol(value, &endptr, 10);
        if (*endptr != '\0') 
            error_exit(EXIT_FAILURE, 0,"invalid argument: '%s'", value);
        if (errno == ERANGE || cnt < 1)
            error_exit(EXIT_FAILURE, 0, "invalid argument: '%s': out of range: 1 <= value <= %ld", value, LONG_MAX);
        opts->count = (int)cnt;
    }
    if (flag == 'i'){
        double interval = strtod(value, &endptr);
        if (*endptr != '\0') 
            error_exit(EXIT_FAILURE, 0,"invalid argument: '%s'", value);
        if (errno == ERANGE || interval <=0.0)
            error_exit(EXIT_FAILURE, 0, "invalid argument: '%s': out of range: 1 <= value <= %ld", value, LONG_MAX);
        opts->interval = interval;
    }
    if (flag == 'p'){
        size_t hexlen = strlen(value);
        if (hexlen < 2 || hexlen > MAX_PATTERN_LEN*2 || hexlen%2 != 0)
            error_exit(EXIT_FAILURE, 0, "invalid argument: '%s'", value);
        opts->pattern_len = hexlen / 2; //de digitos a bytes
        for (size_t i = 0; i < opts->pattern_len; i++) {
            char byte_str[3] = {value[2*i], value[2*i+1], '\0'};
            long v = strtol(byte_str, &endptr, 16);
            if (*endptr != '\0')
                error_exit(EXIT_FAILURE, 0,"invalid argument: '%s'", value);
            opts->pattern[i] = (uint8_t)v;
        }
        opts->pattern_use = 1;
    }
    if (flag == 's'){
        long sz = strtol(value, &endptr, 10);
        if (*endptr != '\0') 
            error_exit(EXIT_FAILURE, 0,"invalid argument: '%s'", value);
        if (errno == ERANGE || sz < 0 || sz > MAX_PAYLOAD_SIZE)
            error_exit(EXIT_FAILURE, 0, "invalid argument: '%s': out of range: 1 <= value <= %ld", value, MAX_PAYLOAD_SIZE);
        opts->payload_size = (size_t)sz;
        opts->payload_size_use = 1;
    }
}