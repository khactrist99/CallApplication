/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Network.cpp
 * Author: trink
 * 
 * Created on August 19, 2021, 2:36 PM
 */

#include "Network.h"

Network::Network() {
}

Network::Network(const Network& orig) {
}

Network::~Network() {
    close(sockfd);
}

void Network::Init() {
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
      
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
      
    // Filling server information
    server_addr.sin_family    = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
}

void Network::Bind() {
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&server_addr, 
            sizeof(server_addr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

}

int Network::recv(unsigned char * data, int data_size) {
    unsigned char buffer[MAXLINE];
    int offset = 0;
    int remain = data_size;
    int bytes_recv = 0;
    while ( (remain > 0) && ( ( bytes_recv = recvfrom(sockfd, buffer, 
            std::min(remain + NB_FLAG, MAXLINE), MSG_WAITALL, (struct sockaddr *) &client_addr, 
            &len) ) > 0 ) )
    {
        if (buffer[0] == END) {
            return END; 
        }
        memcpy(data + offset, buffer + NB_FLAG, bytes_recv - NB_FLAG);         
        offset += (bytes_recv - NB_FLAG);
        remain -= (bytes_recv - NB_FLAG);                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    }
    return VIDEO;
}

void Network::send(unsigned char * data, int data_size) {
    int bytes_sent = 0;
    int offset = 0;
    int remain = data_size;
    int n = 0;
    data[-NB_FLAG] = VIDEO << 14 | n; 
    while ((remain > 0) && ((bytes_sent = sendto(sockfd, data + offset - NB_FLAG, 
            std::min(remain + NB_FLAG, MAXLINE), MSG_CONFIRM, (struct sockaddr *) &server_addr, 
            sizeof(server_addr) )) > 0))
    {   
        n += 1;
        offset += (bytes_sent - NB_FLAG);
        remain -= (bytes_sent - NB_FLAG);
        data[offset - NB_FLAG] = VIDEO << 14 | n;
    }
}

void Network::Close() {
    unsigned char end_signal[NB_FLAG] = {END};
    if (sendto(sockfd, end_signal, NB_FLAG, MSG_CONFIRM, (struct sockaddr *) &server_addr, 
                sizeof(server_addr)) < 0) {
        printf("Close Network error!");
    }
}