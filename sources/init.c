#include "ft_ping.h"

/**
 * @brief Función para iniciar la estructura t_ping_options
 * 
 * @param opts la estructura en sí
 */
void init_opts(t_ping_options *opts){
    opts->verbose = 0;
    opts->target = NULL;
}