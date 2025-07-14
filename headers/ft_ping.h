#ifndef FT_PING_H
 #define FT_PING_H

 #include <unistd.h>
 #include <stdarg.h>
 #include "ft_ping_structs.h"

 
 //**** extras.c ****//
 char	*ft_strjoin(char const *s1, char const *s2);

 //**** prints.c ****//
 void   print_help();
 void   print_usage(void);
 void   error_exit(int status, int errnum, const char *fmt, ...);

 //**** parse_args.c ****//
 void   parse_args(int argc, char *argv[], t_ping_options *opts);
 
 //**** init.c ****//
 void init_opts(t_ping_options *opts);
  


 #endif