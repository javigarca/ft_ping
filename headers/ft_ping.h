#ifndef FT_PING_H
 #define FT_PING_H

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 #include "ft_ping_structs.h"

 
 //**** extras.c ****//
 char	*ft_strjoin(char const *s1, char const *s2);

 //**** prints.c ****//
 void   print_help();
 void   usage();

 //**** parse_args.c ****//
 int parse_args(int argc, char *argv[], t_ping_options *opts);
 
 //**** init.c ****//
 void init_opts(t_ping_options *opts);
  


 #endif