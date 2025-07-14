#ifndef FT_PING_STRUCTS_H
 #define FT_PING_STRUCTS_H

# include <stdint.h>
# include <netinet/in.h>   // struct sockaddr_in
# include <arpa/inet.h>    // INET_ADDRSTRLEN

 /**
  * @brief estructura para las flags. autoexplicativas
  * 
  */
 typedef struct s_ping_options {
	int 		verbose;
	const char	*target;
} t_ping_options;

/**
 * @brief Estructura para las estadísticas.
 * 
 * RTT significa Round-Trip Time: el tiempo que tarda un paquete en ir desde tu máquina al host remoto y volver con la respuesta.
 * Este valor se mide en milisegundos (ms).
 * 
 * rtt min/avg/max/mdev
 * mdev, desviación de latencia, cuanto menor más estable es la conexión
 */
typedef struct s_stats {
	int transmitted;         	// Nº total de paquetes enviados
	int received;            	// Nº total de paquetes recibidos correctamente
	double rtt_min;          	// Menor RTT observado (latencia más baja)
	double rtt_max;          	// Mayor RTT observado (latencia más alta)
	double rtt_total;        	// Suma acumulada de todos los RTT (para calcular la media)
	double rtt_squared_total;	// Suma de los RTT^2 (para calcular la desviación estándar)
} t_stats;

// Tamaño del payload ICMP (56 bytes es estándar en ping)
# define PAYLOAD_SIZE 56

/**
 * @brief Información del host de destino resuelta desde el nombre o IP.
 * Contiene:
 *  - la IP como string para imprimir
 *  - el hostname original (si no era una IP directa)
 *  - la estructura sockaddr_in que se usará en sendto()
 */
typedef struct s_target {
	char ip_str[INET_ADDRSTRLEN];   // Dirección IP como string (e.g. "8.8.8.8")
	char *hostname;                 // Hostname original (si aplica)
	struct sockaddr_in addr;        // Dirección IPv4 en formato binario
} t_target;

/**
 * @brief Cabecera ICMP tal y como debe enviarse en un paquete ICMP Echo.
 * Campos:
 *  - type: 8 para Echo Request, 0 para Echo Reply
 *  - code: normalmente 0
 *  - checksum: se calcula sobre toda la cabecera + payload
 *  - id: identificador único por proceso
 *  - sequence: número de secuencia creciente
 */
typedef struct s_icmphdr {
	uint8_t  type;
	uint8_t  code;
	uint16_t checksum;
	uint16_t id;
	uint16_t sequence;
} t_icmphdr;

/**
 * @brief Paquete completo ICMP, cabecera seguida de payload.
 * Este buffer es el que se enviará y recibirá con sendto()/recvfrom().
 */
typedef struct s_packet {
	t_icmphdr header;              // Cabecera ICMP
	char payload[PAYLOAD_SIZE];   // Payload arbitrario (p.ej., timestamp o relleno)
} t_packet;

#endif