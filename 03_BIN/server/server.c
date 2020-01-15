#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define MSG_LEN 3000
#define SRV_IP "127.0.0.1"
#define SOCK_PORT 31337

#define KB_SIZE 1024
#define MB_SIZE 1048576
#define GB_SIZE 1073741824

void parseFileLen(char* str, size_t *size){
	*size = 0;
	while (*str >= '0' && *str <= '9'){
		*size = *size * 10 + (*str - '0');
		str++;
		//printf("%zu\n",*size);
	}
	//printf("%zu\n",*size);
}

void parseFileMaxLen(char* str, size_t *size){
	*size = 0;
	while (*str >= '0' && *str <= '9'){
		*size = *size * 10 + (*str - '0');
		str++;
		//printf("%zu\n",*size);
	}
	if (*str == 'K'){
		*size *= KB_SIZE;
	} else if (*str == 'M'){
		*size *= MB_SIZE;
	} else if (*str == 'G'){
		*size *= GB_SIZE;
	}
	//printf("%zu\n",*size);
}

int main(){
    	struct sockaddr_in server_addr, client_addr;
	int sock, slen = sizeof(client_addr);
	char buf[MSG_LEN];
	char fileName[MSG_LEN];
	char* bytes;
	size_t maxsize;
	size_t rsize;

	printf("Server started!\n");
	printf("Input max file size: ");
	fgets(buf, MSG_LEN, stdin);
	parseFileMaxLen(buf,&maxsize);
	bytes = malloc(maxsize);
	
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
		if (recvfrom(sock, fileName, MSG_LEN, 0, (struct sockaddr *)&client_addr, &slen) == -1){
			perror("Error: Couldn't recieve filename!");
			exit(1);
		}

		printf("Recieved file name: %s",fileName);

		if (recvfrom(sock, buf, MSG_LEN, 0, (struct sockaddr *)&client_addr, &slen) == -1){
			perror("Error: Couldn't file size!");
			exit(1);
		}

		parseFileMaxLen(buf,&rsize);
		printf("Recieved file size: %ld\n",rsize);

		if (rsize <= maxsize){
			int i;
		
			for (i = 0; fileName[i] != 0 && fileName[i] != '\n'; i++);
			fileName[i] = 0;
			FILE *fp = fopen(fileName,"w");

			int packets = rsize / MSG_LEN;
			if (rsize%MSG_LEN != 0){
				packets++;
			}

			//printf("%d packets to recieve\n",packets);
			
			for (int k = 0; k < packets; k++){
				//printf("\tpacket %d recieved\n",k);
				if (recvfrom(sock, bytes, MSG_LEN, 0, (struct sockaddr *)&client_addr, &slen) == -1){
					perror("Error: Couldn't recieve message!");
					exit(1);
				}
				fwrite(bytes,1,rsize,fp);
			}
			
			fclose(fp);	
		} else {
			perror("Error: File is too big!");
			exit(1);
		}
		
	}
	free(bytes);
	close(sock);
	return(0);
}
