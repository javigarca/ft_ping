#include "ft_ping.h"

void init_opts(t_ping_options *opts){
    opts->show_help = 0;
    opts->verbose = 0;
    opts->target = NULL;
}