#ifndef FT_PING_H
 #define FT_PING_H

 #include <unistd.h>
 #include <stdarg.h>
 #include <stdio.h>
 #include "ft_ping_structs.h"

 //**** prints.c ****//
 void       print_help();
 void       print_usage(void);
 void       error_exit(int status, int errnum, const char *fmt, ...);
 void       print_summary(t_stats *stats);
 void       print_infof(int verbose, FILE *stream, const char *fmt, ...);

 //**** parse_args.c ****//
 void       parse_args(int argc, char *argv[], t_ping_options *opts);
 int        resolve_target(t_ping_options *opts, t_target *t_out);

 //**** main.c ****//
 void       handle_sigint(int signum);

 //**** packets.c *****//
 int        send_packet(int sockfd, const t_ping_options *opts, const t_target *target, t_stats *stats, uint16_t seq);
 int        receive_packet(int sockfd, uint16_t sent_seq, const t_ping_options *opts, t_stats *stats);
 uint64_t   ft_time_now_us(void);
 uint16_t   calc_checksum(const void *data, size_t len);
  
 #endif