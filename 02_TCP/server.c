#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <strings.h>
#define MAX 1024 
#define PORT 3000 
#define SA struct sockaddr 

#define BYTE_TO_BYNARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void parse(char* buff, char* type, int* index){
	int i = 0;
	int current;
	if (strncmp(buff,"FILE",4) == 0){
		*type = 'F';
		return;
	}
	if (buff[0] == 'B' || buff[0] == 'H' || buff[0] == 'b'|| buff[0] == 'h' || buff[0] == 'V' || buff[0] == 'v'){
		*type = buff[0];
	} else {
		perror("Error! Wrong command!\n");
		exit(1);
	}
	buff++;
	while (*buff != '\n'){
		current = *buff - '0';

		if (current > 9 || current < 0){
			
			perror("111Error! Wrong command!\n");
			exit(1);
		}
		i = i*10 + current;
		buff++;
	}
	*index = i;
	printf("Type = %c Index = %d\n",*type,*index);
}

void getData(char* buff, char type, int index){
	FILE *fp = fopen("address.txt","r");
	int b1,b2,b3,b4;
	
	if (type == 'F'){
		char current[MAX];
		while(fgets(current,MAX,fp)!=NULL){
			buff = strcat(buff, current);
		}
	} else {
		for(int i = 0; i < index+1; i++){
			fscanf(fp,"%d.%d.%d.%d\n",&b1,&b2,&b3,&b4);
		}
		if (type == 'B'){
			sprintf(buff,"%c%c%c%c%c%c%c%c.%c%c%c%c%c%c%c%c.%c%c%c%c%c%c%c%c.%c%c%c%c%c%c%c%c\n",BYTE_TO_BYNARY(b1),BYTE_TO_BYNARY(b2),BYTE_TO_BYNARY(b3),BYTE_TO_BYNARY(b4));
		} else if (type == 'H'){
			sprintf(buff,"%x.%x.%x.%x\n",b1,b2,b3,b4);
		} else if (type == 'V'){
			sprintf(buff,"%o.%o.%o.%o\n",b1,b2,b3,b4);
		}else if (type == 'b') {
			FILE *fp1 = fopen("ad.txt","r");
			char curent[MAX];
			while(fgets(curent,MAX,fp1)!=NULL){
			buff = strcat(buff, curent);}
			for(int i = 0; i < index+1; i++){
			fscanf(fp,"%d.%d.%d.%d\n",&b1,&b2,&b3,&b4);}
		}
		else if (type == 'h') {
			FILE *fp1 = fopen("a.txt","r");
			char cuent[MAX];
			while(fgets(cuent,MAX,fp1)!=NULL){
			buff = strcat(buff, cuent);}
			for(int i = 0; i < index+1; i++){
			fscanf(fp,"%d.%d.%d.%d\n",&b1,&b2,&b3,&b4);}
		}
	}

	fclose(fp);
}

void communicate(int sockfd) 
{ 
    	char buff[MAX]; 
	int index;
	char type;
    	while (1) { 
        	bzero(buff, MAX); 
  
        	read(sockfd, buff, MAX); 

        	printf("From client: %s To client : \n", buff); 

		parse(buff, &type, &index);
        	bzero(buff, MAX); 
		getData(buff, type, index);
		printf("%s",buff);
  
        	write(sockfd, buff, MAX); 
  
        	if (strncmp("exit", buff, 4) == 0) { 
            	printf("Server Exit...\n"); 
            	break; 
        } 
    } 
} 
  
int main() 
{ 
    	int sockfd, connfd, len; 
    	struct sockaddr_in servaddr, cli; 
  
    	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    	if (sockfd == -1) { 
        	perror("Error! Failed to create socket\n"); 
        	exit(1); 
    	} 
    	
	printf("Server started!\n");	

    	bzero(&servaddr, sizeof(servaddr)); 
  
    	servaddr.sin_family = AF_INET; 
    	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    	servaddr.sin_port = htons(PORT); 
  
    	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        	perror("Error! Faild to bind socket!\n"); 
        	exit(1); 
    	} 
  
    	if ((listen(sockfd, 5)) != 0) { 
        	perror("Error! Failed to listen!\n"); 
        	exit(1); 
    	} 
    	len = sizeof(cli); 
  
    	connfd = accept(sockfd, (SA*)&cli, &len); 
    	if (connfd < 0) { 
        	perror("Error! Failed to accept to server!\n"); 
        	exit(1); 
    	} 
  
    	communicate(connfd); 
    	close(sockfd); 
} 
