/*

  DrvFltIp.H

  Author: Jesús Oliva García
  Last Updated: 06/04/03 18:54
  
*/


//
// Define the various device type values.  Note that values used by Microsoft
// Corporation are in the range 0-32767, and 32768-65535 are reserved for use
// by customers.
//
// Tipo del dispositivo. De 0-32767 reservado por Microsoft.
#define FILE_DEVICE_DRVFLTIP  0x00654322


// Macro para definir IOCTL
#define DRVFLTIP_IOCTL_INDEX  0x830


// Defino los IOCTLs
#define START_IP_HOOK CTL_CODE(FILE_DEVICE_DRVFLTIP, DRVFLTIP_IOCTL_INDEX,METHOD_BUFFERED, FILE_ANY_ACCESS)

#define STOP_IP_HOOK CTL_CODE(FILE_DEVICE_DRVFLTIP, DRVFLTIP_IOCTL_INDEX+1, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define ADD_FILTER CTL_CODE(FILE_DEVICE_DRVFLTIP, DRVFLTIP_IOCTL_INDEX+2, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define CLEAR_FILTER CTL_CODE(FILE_DEVICE_DRVFLTIP, DRVFLTIP_IOCTL_INDEX+3, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define OUT_PRINTSTR CTL_CODE(FILE_DEVICE_DRVFLTIP, DRVFLTIP_IOCTL_INDEX+4, METHOD_BUFFERED, FILE_ANY_ACCESS)


// Estructura para definir una regla de filtrado
typedef struct filter
{
	USHORT protocol;		// Protocolo

	ULONG sourceIp;			// Dirección IP fuente
	ULONG destinationIp;	// Dirección IP destino

	ULONG sourceMask;		// Mascara de dirección IP fuente
	ULONG destinationMask;	// Mascara de dirección IP destino

	USHORT sourcePort;		// Puerto fuente
	USHORT destinationPort; // Puerto destino
	
	BOOLEAN drop;			// Si TRUE, el paquete sera tirado en caso de coincidencia
}IPFilter, *PIPFilter;



// Estructura para definir la lista enlazada de reglas.
struct filterList
{
	IPFilter ipf;

	struct filterList *next;
};


// Cabecera IP
typedef struct IPHeader 
{
    UCHAR     iphVerLen;      // Version y longitud cabecera
    UCHAR     ipTOS;          // Tipo de servicio
    USHORT    ipLength;       // Longitud total del datagrama
    USHORT    ipID;		      // Identificacion 
    USHORT    ipFlags;	      // Flags
    UCHAR     ipTTL;	      // TTL
    UCHAR     ipProtocol;	  // Protocolo de nivel superior 
    USHORT    ipChecksum;     // Checksum de la cabecera
    ULONG     ipSource;       // Direccion fuente
    ULONG     ipDestination;  // Direccion destino
} IPPacket, *PIPPacket; 


// Cabecera TCP
typedef struct _TCPHeader
{
	USHORT			sourcePort;			// Puerto fuente
	USHORT			destinationPort;	// Puerto destino
	ULONG			sequenceNumber;		// Numero de secuencia
	ULONG			acknowledgeNumber;	// Numero de reconocimiento
	UCHAR			dataoffset;			// Puntero a los datos
	UCHAR			flags;				// Flags
	USHORT			windows;			// Tamaño de la ventana TCP
	USHORT			checksum;			// Checksum del paquete
	USHORT			urgentPointer;		// Puntero a los datos "urgentes"
} TCPHeader, *PTCPHeader;


// Cabecera UDP
typedef struct _UDPHeader
{
	USHORT			sourcePort;			// Puerto fuente
	USHORT			destinationPort;	// Puerto destino
	USHORT			len;				// Longitud del datagrama
	USHORT			checksum;			// Checksum del datagrama
} UDPHeader, *PUDPHeader;


