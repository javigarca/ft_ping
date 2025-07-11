#include "ft_ping.h"

void    print_help() 
{
    usage();
    printf("Options:\n");
    printf("  -v                 verbose output\n");
    printf("  -c <count>         stop after <count> replies\n");
    printf("  -i <interval>      wait interval seconds between sending each packet\n");
    printf("  -t <ttl>           set the TTL (time to live)\n");
    printf("  -h, --help         display this help and exit\n");
}

void    usage()
{
    printf("Usage: ft_ping [options] host\n");
}