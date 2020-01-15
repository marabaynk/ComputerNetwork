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
	struct sockaddr_in server_addr;
	int sock, slen = sizeof(server_addr);
	int min = 0;
	int max = 0;
	int k = 1;
	char buf[MSG_LEN];
	const char *server_name = "localhost";
	const int server_port = 31337;
	printf("Client started!\n");
	printf("Input message or 'exit' to close connection.\n\n");
	
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("Error: Couldn't get socket!");
		exit(1);
	}	

	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SOCK_PORT);

 	inet_pton(AF_INET, server_name, &server_addr.sin_addr);

	while (1){
		fgets(buf, MSG_LEN, stdin);
		int current = strlen(buf);
		int b = strcmp(buf,"exit\n");
		if (current > max && b!=0){
			max = current-1;
			if (k == 1){
				k = 0;
				min = current-1;
			}
		}
		else if (min > current-1){
			min = current-1;
		}
		

		else if(strcmp(buf,"exit\n")==0 ){
			sprintf(buf,"massage line of client: min = %d  max = %d\n",min,max);
			if (sendto(sock, buf, MSG_LEN, 0, (struct sockaddr *)&server_addr, slen) == -1){
				perror("Error: Couldn't send message!");
				exit(1);
			}
			break;
		}
		printf("min = %d  max = %d\n",min,max);
		if (sendto(sock, buf, MSG_LEN, 0, (struct sockaddr *)&server_addr, slen) == -1){
			perror("Error: Couldn't send message!");
			exit(1);
		}
	}	

	close(sock);
	return 0;
}	
