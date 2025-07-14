#include "ft_ping.h"
#include "ft_ping_structs.h"

int send_packet(int sockfd, const t_ping_options *opts, const t_target *target, t_stats *stats, uint16_t seq){

/* 
t_packet packet;
	struct timeval now;
	ssize_t sent_bytes;

	// 1. Rellenar cabecera
	memset(&packet, 0, sizeof(packet));
	packet.header.type = 8;
	packet.header.code = 0;
	packet.header.id = htons(getpid() & 0xFFFF);
	packet.header.sequence = htons(seq);

	// 2. Timestamp en payload
	gettimeofday(&now, NULL);
	memcpy(packet.payload, &now, sizeof(now));

	// 3. Rellenar resto del payload
	for (size_t i = sizeof(now); i < PAYLOAD_SIZE; i++)
		packet.payload[i] = 0x42; // patrón arbitrario

	// 4. Calcular checksum
	packet.header.checksum = 0;
	packet.header.checksum = calc_checksum(&packet, sizeof(packet));

	// 5. Enviar
	sent_bytes = sendto(sockfd, &packet, sizeof(packet), 0,
	                    (struct sockaddr *)&target->addr, sizeof(target->addr));

	if (sent_bytes < 0) {
		print_infof(opts->verbose, stderr, "sendto failed: %s", strerror(errno));
		return -1;
	}

	// 6. Verbose 
	print_infof(opts->verbose, stdout, "Sent ICMP echo seq=%d (%ld bytes)",
	            seq, sent_bytes);

	return 0;


*/

}

int receive_packet(int sockfd, uint16_t sent_seq, const t_ping_options *opts, t_stats *stats){

    /*

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
    
    */
}