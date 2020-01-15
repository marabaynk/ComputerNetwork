#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define MSG_LEN 3000
#define SRV_IP "127.0.0.1"
#define SOCK_PORT 31337

int main(){
	struct sockaddr_in server_addr;
	int sock, slen = sizeof(server_addr);
	const char *server_name = "localhost";
	int msize = 0;
	char buf[MSG_LEN];
	char* bytes = malloc(MSG_LEN);

	printf("Client started!\n");
	printf("Input file name or 'exit' to close connection.\n\n");
	
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
		if (current > msize){
			msize = strlen(buf);
		}

		if(strcmp(buf,"exit\n")==0){
			printf("Client exited!\n");
			break;
		}
		

		if (sendto(sock, buf, MSG_LEN, 0, (struct sockaddr *)&server_addr, slen) == -1){
			perror("Error: Couldn't send file name!");
			exit(1);
		}

		printf("\tFile name sended\n");

		size_t i,j;
		j = 0;
		char byte;
		size_t size;
		for (i = 0; buf[i] != 0 && buf[i] != '\n'; i++);
		buf[i] = 0;
		FILE *fp = fopen(buf,"r");

		fseek(fp,0L,SEEK_END);
		size = ftell(fp);
		rewind(fp);

		bytes = (char*)realloc(bytes,size+1);

		printf("\tFile size = %zu\n",size);
		
		printf("\tFile descriptor created\n");

		for (i = 0; i < MSG_LEN; i++){
			buf[i] = 0;
		}

		//while ((byte = getc(fp))!=EOF){
		while (j < size){
			byte = getc(fp);
			bytes[j] = byte;
			j++;
		}
			bytes[j] = '\n';

		printf("\t%ld bytes sended\n",j);
		sprintf(buf,"%zu\n",j);
		int packets = size / MSG_LEN;
		if (size%MSG_LEN != 0){
			packets++;
		}
		
		if (sendto(sock, buf, MSG_LEN, 0, (struct sockaddr *)&server_addr, slen) == -1){
			perror("Error: Couldn't send file length!");
			exit(1);
		}

		for (int k = 0; k < packets; k++){
			printf("\t packet %d sended that starts with %x\n",k+1,*(bytes+MSG_LEN*k));
			char *current = bytes+MSG_LEN*k;
			if (sendto(sock, current, MSG_LEN, 0, (struct sockaddr *)&server_addr, slen) == -1){
				perror("Error: Couldn't send file content!");
				exit(1);
			}
			sleep(10000);
		}

		fclose(fp);
	}	
	free(bytes);
	close(sock);
	return 0;
}	
