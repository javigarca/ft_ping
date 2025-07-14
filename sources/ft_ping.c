#include <stdlib.h>
#include "ft_ping.h"

int main (int argc, char **argv)
{
    t_ping_options opts;
    init_opts(&opts);

    parse_args(argc, argv, &opts); 
   
    return (EXIT_SUCCESS);
}