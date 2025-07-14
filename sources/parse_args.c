#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "ft_ping.h"

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
                    write(2,"verbose ON\n", 11);
                }
            } else {
                for (int j = 1; argv[i][j]; j++){
                    switch (argv[i][j]) {
                        case 'v': 
                            opts->verbose=1;
                            write(2,"verbose ON\n", 11);
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