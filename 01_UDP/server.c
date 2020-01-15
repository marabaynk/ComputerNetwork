#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define MSG_LEN 512
#define SRV_IP "127.0.0.1"
#define SOCK_PORT 31337

int main(){
    	struct sockaddr_in server_addr, client_addr;
	int sock, slen = sizeof(client_addr);
	char buf[MSG_LEN];

	printf("Server started!\n");
	
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("Error: Couldn't get socket!");
		exit(1);
	}

	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SOCK_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
		perror("Error: Couldn't bind!");
		exit(1);
	}

	while(1){
		if (recvfrom(sock, buf, MSG_LEN, 0,(struct sockaddr *) &client_addr, &slen) == -1){
			perror("Error: Couldn't recieve message!");
			exit(1);
		}

		printf("recieved: %s",buf);
	}

	close(sock);
	return(0);
}
