#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main (argc, char *argv[])
{
		int sock_conn, sock_listen, ret;
		struct sockaddr_in serv_adr;
		char peticion[512];
		char respuesta[512];
		//abrimos socket
		if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				printf("Error creant socket");
		//Fem el bind al port
		//Inicalitza a zero serv_adr
		memset(&serv_adr, 0, sizeof(serve_adr));
		serv_adr.sin_family = AF_INET;
		
		//Asocia el socket a qualsevol IP de la màquina
		//htonl formatea el numero que recibe al formato necesario
		serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
		//escuchamos en el puerto 9050
		serv_adr.sin_port = htons(9050);
		if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
				printf ("Error al bind");

		//Maximo de peticiones en la cola es de 3
		if (listen(sock_listen, 3) < 0)
				printf ("Error en el Linsten");


		int i;
		//Atenderemos 5 peticiones
		for (i=0;i<5;i++){
				printf ("Escuchando\n");

				sock_conn = accept(sock_listen, NULL, NULL);
				printf ("He recibido conexión\n");
				
				//Ahora recibimos su nombre, que dejamos en el buf
				ret=read(sock_conn,peticion, sizeof(peticion));
				printf ("Recibido\n");

				//Tenemos que añadir la marca de fin de string para que no escriba lo que hay en el buffer
				peticion[ret]='\0';

				//Escribinos el nombre en consola

				printf("Petición: %s\n", peticion);
				
				//Vamos a ver que nos pide la peticion
				char *p = strtok( peticion, "/");
				int codigo = atoi(p);
				p = strtok (NULL, "/");
				char nombre[20];
				strcpy(nombre, p);
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
				
				if(codigo == 1) //piden la longitud del nombre
						sprintf(respuesta,"%d",strlen(nombre));
				else
						// saber si el nombre es bonito
						if ((nombre[0]=='M') || (nombre[0]=='S'))
							strcpy(repuesta, "SI");
						else
							strcpy(respuesta, "NO");

					printf ("Respuesta: %s\n", respuesta);
					//lo enviamos
					write (sock_conn, respuesta, strlen(respuesta));

					//se acabo el servicio para este cliente
					close(sock_conn);
		}
}