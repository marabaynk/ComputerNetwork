#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1024 
#define PORT 3000 
#define SA struct sockaddr
 
void communicate(int sockfd) 
{ 
    	char buff[MAX]; 
    	int n; 
    	while(1) { 
        	bzero(buff, sizeof(buff)); 
        	printf("Enter the string : "); 
        	n = 0; 
        	while ((buff[n++] = getchar()) != '\n') 
		;
		if ((strncmp(buff, "exit", 4)) == 0) { 
            		printf("Client Exit...\n"); 
            		break; 
        	}  
        	write(sockfd, buff, MAX); 
        	bzero(buff, sizeof(buff)); 
        	read(sockfd, buff, MAX); 
        	printf("From Server :\n%s", buff); 
        	
    	} 
} 
  
int main() 
{ 
    	int sockfd, connfd; 
    	struct sockaddr_in servaddr, cli; 
  
    	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    	if (sockfd == -1) { 
        	perror("Error! Failed to create socket!\n"); 
        	exit(1); 
    	} 
    	
	printf("Client started!\n");
    	bzero(&servaddr, sizeof(servaddr)); 
  
    	servaddr.sin_family = AF_INET; 
    	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    	servaddr.sin_port = htons(PORT); 
  
    	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        	perror("Error! Faild to connect server!\n"); 
        	exit(1); 
    	} 

	printf("[B/H/V][Number]\n");
	printf("or input [FILE]\n");
  
    	communicate(sockfd); 
    	close(sockfd); 
} 
