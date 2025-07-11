#include "ft_ping.h"

int main (int argc, char **argv)
{
    char *msg;

    if (argc != 2){
        msg = ft_strjoin("Using ping with a parameter, please.", "\n");
        write(2, msg, strlen(msg));
    }
    else {
        msg = ft_strjoin(argv[1], " port tested. \n");
        write(1,msg, strlen(msg));
    }
    free(msg);

    return (0);
}