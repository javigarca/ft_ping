#include "ft_ping.h"

int main (int argc, char **argv)
{
    t_ping_options opts;
    init_opts(&opts);

    if (parse_args(argc, argv, &opts)){
        usage();
    }

    if (argc < 2){
        usage();
        return (EXIT_FAILURE);
    }
    else {
        if(argv[1])
            fprintf(stderr, "%s", argv[1]);
        print_help();
    }
   
    return (EXIT_SUCCESS);
}