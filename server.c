#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SECTIONS 7
#define PORT 4210
#define SA struct sockaddr
static int MAX = SECTIONS*6;

struct sockaddr_in servaddr; 
//// Function designed for chat between client and server.
////void func(int connfd,char buff[MAX]){
//    // infinite loop for chat
//        //bzero(buff, MAX);
//        //n = 0;
//        // copy server message in the buffer
//        //while ((buff[n++] = getchar()) != '\n')
//        //    ;
//   
//        // and send that buffer to client
//   	sendto(socket, buff, strlen(buff)+1, 
//        0, (const struct sockaddr *) &servaddr,  
//            sizeof(servaddr)); 
//        // if msg contains "Exit" then server exit and chat ended.
//}
   
// Driver function
int main()
{
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
   
   
    // Binding newly created socket to given IP and verification
   
    // Now server is ready to listen and verification
   
    // Function for chatting between client and server
    //func(connfd,"000000000000000000000000000000000000000000");
    printf("Stripe nullen\n");
    char zahl[]="000000000000000000000000000000000000000000";
    sendto(sockfd, zahl, (strlen(zahl)+1), 0,
(struct sockaddr *)&servaddr, sizeof(servaddr)) ;
    printf("Stripe genullt... theoretisch\n");
    _Bool count[]={0,0,0,0,0,0,0};
    int tmp;
    char zahlen[][6]={"000000","ffffff","ff0000","00ff00","0000ff"};
    for(int h=0;h<5;h++){
	    for(int i=0;i<128;i++){
		    tmp=i;
		    for(int j=0;j<7;j++){
			    count[j]=tmp%2;
			    tmp=tmp/2;
		    }
		    for(int k=0;k<42;k++){
			    zahl[k]=zahlen[(count[k/6]+h)%5][k%6];
		    }
		    sendto(sockfd, zahl, (strlen(zahl)+1), 0,
                 (struct sockaddr *)&servaddr, sizeof(servaddr)) ;
		    printf("Schicke: %s\n", zahl);
		    sleep(1);
	    }
    }

    // After chatting close the socket
    close(sockfd);
}
