
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include "ft_ping.h"
#include "ft_ping_definitions.h"
#include "ft_ping_structs.h"

int send_packet(int sockfd, const t_ping_options *opts, const t_target *target, t_stats *stats, uint16_t seq){
    t_packet    packet;
	uint64_t    now;
	ssize_t     sent_bytes;

	//  Rellenar cabecera
	memset(&packet, 0, sizeof(packet));
	packet.header.type = ICMP_ECHO;
	packet.header.code = ICMP_CODE_DEFAULT;
	packet.header.id = htons((uint16_t)getpid());
	packet.header.sequence = htons(seq);

	// Timestamp en payload
	now = ft_time_now_us();
	memcpy(packet.payload, &now, sizeof(now));

	// Rellenar resto del payload
	for (size_t i = sizeof(now); i < PAYLOAD_SIZE; i++)
		packet.payload[i] = 0x42; // patrón arbitrario

	//  Calcular checksum
	packet.header.checksum = 0;
	packet.header.checksum = calc_checksum(&packet, sizeof(packet));

    // imprimir cabecera y verboses
    /*
    ai->ai_family: AF_INET6, ai->ai_canonname: 'google.com'
PING google.com (2a00:1450:4003:803::200e) 56 data bytes

    */
    const char *family_str =
    (target->addr.sin_family == AF_INET) ? "AF_INET" :
    (target->addr.sin_family == AF_INET6) ? "AF_INET6" :
    "UNKNOWN";

    print_infof(opts->verbose,stdout, "ai->ai_family: %s, ai->ai-canonname: '%s'", family_str, target->hostname);
    print_infof(1, stdout, "PING %s (%s) %d data bytes", target->hostname, target->ip_str, PAYLOAD_SIZE);
	//  Enviar
	sent_bytes = sendto(sockfd, &packet, sizeof(packet), 0,
	                    (struct sockaddr *)&target->addr, sizeof(target->addr));

	if (sent_bytes < 0) {
		print_infof(opts->verbose, stderr, "sendto failed: %s", strerror(errno));
		return -1;
	}

	// 6. Verbose 
	print_infof(opts->verbose, stdout, "Sent ICMP echo seq=%d (%ld bytes)",
	            seq, sent_bytes);
    
    stats->transmitted++;

	return 0;

}
/*
int receive_packet(int sockfd, uint16_t sent_seq, const t_ping_options *opts, t_stats *stats){

    
    Escucha con recvfrom() (bloqueante o con timeout)

    Verifica que es un ICMP Echo Reply (type == 0)

    Verifica que:

        El id coincide (getpid())

        El sequence coincide con sent_seq

    Extrae el timestamp desde el payload y calcula el RTT

    Muestra la línea como:

    64 bytes from 8.8.8.8: icmp_seq=3 ttl=117 time=13.7 ms

    Actualiza las estadísticas:

    stats->received++

    RTT acumulado: rtt_total, rtt_min, rtt_max, etc.
    
    
}
*/

/**
 * @brief Timestamp del momento actual en microsegundos, 64bits
 * 
 * @return uint64_t 
 */
uint64_t    ft_time_now_us(void)
{
	struct timeval	time;
	gettimeofday(&time, NULL);
	return ((uint64_t)time.tv_sec * 1000000ULL) + time.tv_usec;
}

/**
 * @brief Calcula el checksum de ICMP seǵun the Internet checksum (RFC 1071).
 *
 * Usando el header y el payload, usando datos como palabras de 16 bits. 
 * uint8_t *bytes = ...	Convertir datos a array de bytes
 * uint32_t sum = 0;	Inicializar acumulador de 16-bit words
 * Bucle for	Sumar todas las palabras de 16 bits
 * if (len % 2)	Añadir último byte si el número es impar
 * while (sum >> 16)	Añadir acarreo si la suma sobrepasa 16 bits
 * ~sum & 0xFFFF	Complemento a uno → checksum final
 * uint8_t *bytes	--  8 bits	Acceso byte a byte seguro y portable
 * uint16_t word	--  16 bits	Para representar cada bloque de 2 bytes como palabra
 * uint32_t sum	    --  32 bits	Para acumular sin perder acarreo
 * Resultado final	--  16 bits	El valor ICMP checksum es siempre uint16_t
 *
 * @param data Puntero a los datos (por ejemplo t_packet)
 * @param len tamaño en bytes
 * @return uint16_t el checksum final, listo para incluir en header, htons(in host byte order)
 */
uint16_t calc_checksum(const void *data, size_t len)
{
	const uint8_t *bytes = (const uint8_t *)data;
	uint32_t sum = 0;

	// Sum all 16-bit words
	for (size_t i = 0; i + 1 < len; i += 2)
	{
		uint16_t word = (bytes[i] << 8) + bytes[i + 1];
		sum += word;
	}

	// If odd number of bytes, pad with 0
	if (len % 2 != 0)
	{
		uint16_t last = bytes[len - 1] << 8;
		sum += last;
	}

	// Fold carries
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	// One's complement and return
	return ~sum & 0xFFFF;
}
