
#include "sockutil.h"
#include <stdlib.h>
#include <string.h>
#pragma warning( disable: 4996 )
/*++

Descripción:

    Convierte una Ip de cadena a formato de red

Argumentos:

    ip - cadena que contiene la direccion ipstring that represent a ip address
    
    Si la cadena no tiene el formato correcto(x.x.x.x) devuelve -1. Si los octetos
    son menores de 0 o mayores de 255 devuelve direccion 0.0.0.0.

Valores devueltos:

	ip en formato de red

--*/
int inet_addr(const char *sIp, unsigned long *lIp)
{
	int octets[4];
	int i;
	const char * auxCad = sIp;
	*lIp = 0;
	
	// Extraigo cada uno de los octetos. Atoi extrae caracteres hasta encontrar
	// un caracter no numerico, en nuestro el '.'
	for(i = 0; i < 4; i++)
	{
		octets[i] = atoi(auxCad);

		if(octets[i] < 0 || octets[i] > 255)
			return -1;

		*lIp |= (octets[i] << (i*8));

		// Acualizo auxCad para que apunte al siguiente octeto
		auxCad = strchr(auxCad, '.');

		if(auxCad == NULL && i!=3)
			return -1;

		auxCad++;
	}


	return 0;
}



/*++

Descripción:

    Cambia el orden de los octetos. Mayor orden a menor orden y viceversa.

Argumentos:

    port - Numero a convertir


Valores devueltos:

	puerto convertido

--*/
unsigned short htons(unsigned short port)
{
	unsigned short portRet;

	portRet = ((port << 8) | (port >> 8));

	return portRet;
}

char *IpToString(char *ip, unsigned long lIp)
{
	char octeto[4];

	ip[0] = 0;

	itoa(lIp & 0xff, octeto, 10);

	strcat(ip, octeto);
	strcat(ip, ".");


	itoa((lIp >> 8) & 0xff, octeto, 10);

	strcat(ip, octeto);
	strcat(ip, ".");


	itoa((lIp >> 16) & 0xff, octeto, 10);

	strcat(ip, octeto);
	strcat(ip, ".");

	itoa((lIp >> 24) & 0xff, octeto, 10);

	strcat(ip, octeto);
	

	return ip;
}
	