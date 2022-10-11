#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SECTIONS 7
#define PORT 4210
#define SA struct sockaddr

static int MAX = SECTIONS*6;

struct sockaddr_in servaddr;

int main(){
	int sockfd, connfd, len;
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
	    printf("socket creation failed...\n");
	    exit(0);
	}
	else
	    printf("Socket successfully created..\n");
	//bzero(&servaddr, sizeof(servaddr));
	// assign IP, PORT
	printf("Server Adresse festlegen\n");
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("192.168.2.11");



	char chars[16]="0123456789abcdef";
	char String[]="000000000000000000000000000000000000000000";
	int rgb[]={0,0,0};
	for(int h = 0; h<3;h++){
		for(int i = 0; i<256;i++){
			rgb[h]=i;
			rgb[(h+2)%3]=255-i;
			for(int k=0;k<7;k++){
				String[0+k*6]=chars[(rgb[0]&0b11110000)>>4];
				String[1+k*6]=chars[(rgb[0]&0b00001111)];
				String[2+k*6]=chars[(rgb[1]&0b11110000)>>4];
				String[3+k*6]=chars[(rgb[1]&0b00001111)];
				String[4+k*6]=chars[(rgb[2]&0b11110000)>>4];
				String[5+k*6]=chars[(rgb[2]&0b00001111)];
			}
			sendto(sockfd, String, (strlen(String)+1), 0,
			 (struct sockaddr *)&servaddr, sizeof(servaddr)) ;
			printf("Schicke: %s\n", String);
			usleep(100000);
		}
	}
}

