// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
extern "C"
{
#include "SDL2/SDL.h"
};
#define PORT 8080
#define MAXLINE 1024

const int bpp=12;

int screen_w=640,screen_h=480;
const int pixel_w=640,pixel_h=480;

unsigned char buffer[pixel_w*pixel_h*bpp/8];

using namespace std;

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    int bytes_sent = 0;
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
  
    memset(&servaddr, 0, sizeof(servaddr));
      
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
      
    int n;
    socklen_t len = sizeof(servaddr);
    
    FILE *fp=NULL;
    fp=fopen("out.yuv","rb+");

	if(fp==NULL){
		printf("cannot open this file\n");
		return -1;
	}
    
    while(1){
        if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
            break;
        }
        int offset = 0;
        int remain = pixel_w*pixel_h*bpp/8;
        while ((remain > 0) && ((bytes_sent = sendto(sockfd, buffer + offset, MAXLINE, 
            MSG_CONFIRM, (struct sockaddr *) &servaddr, len )) > 0))
        {    
            
            offset += bytes_sent;
            remain -= bytes_sent;
            // printf("remain:%d\n", remain);
        }
    }
  
    close(sockfd);
    return 0;
}
