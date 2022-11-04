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


struct sockaddr_in servaddr;
const char* generateJWT(char String[]);
int main(){
	char hexCodes[]="000000000000000000000000000000000000000000";
	int sockfd;
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
	int rgb[]={0,0,0};
	char * jwt;
	unsigned char pendel=0;
	_Bool up=1;
	for(int h = 0; 1;h=(h+1)%3){
		for(int i = 0; i<256;i++){
			rgb[h]=i;
			rgb[(h+2)%3]=255-i;
			for(int k=0;k<7;k++){
				if(k<pendel){
					hexCodes[0+k*6]=chars[(rgb[0]&0b11110000)>>4];
					hexCodes[1+k*6]=chars[(rgb[0]&0b00001111)];
					hexCodes[2+k*6]=chars[(rgb[1]&0b11110000)>>4];
					hexCodes[3+k*6]=chars[(rgb[1]&0b00001111)];
					hexCodes[4+k*6]=chars[(rgb[2]&0b11110000)>>4];
					hexCodes[5+k*6]=chars[(rgb[2]&0b00001111)];
				}
				else{
					hexCodes[0+k*6]=chars[(rgb[2]&0b11110000)>>4];
					hexCodes[1+k*6]=chars[(rgb[2]&0b00001111)];
					hexCodes[2+k*6]=chars[(rgb[0]&0b11110000)>>4];
					hexCodes[3+k*6]=chars[(rgb[0]&0b00001111)];
					hexCodes[4+k*6]=chars[(rgb[1]&0b11110000)>>4];
					hexCodes[5+k*6]=chars[(rgb[1]&0b00001111)];
				}
			}
			if(!(i%10)){
				up?pendel++:pendel--;
				if(pendel==7) up=0;
				if(pendel==0) up=1;
			}
			jwt=generateJWT(hexCodes);
			sendto(sockfd, jwt, (strlen(jwt)+1), 0,
			 (struct sockaddr *)&servaddr, sizeof(servaddr)) ;
			printf("Schicke: %s\n",jwt );
			usleep(100000);
		}
	}
}

