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

void Network::recv(unsigned char * buffer, int buffer_size) {
    int offset = 0;
    int remain = buffer_size;
    int bytes_recv = 0;
    while ((remain > 0) && ((bytes_recv = recvfrom(sockfd, buffer + offset, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &client_addr, &len)) > 0))
    {
        // if (bytes_recv == 1) {
        //     running = false;
        //     printf("\n offset: %d, buffer value: %x stop recv.\n", offset, buffer[offset]);
        //     break; 
        // }  
        offset += bytes_recv;
        remain -= bytes_recv;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    }
}

void Network::send(unsigned char * buffer, int buffer_size) {
    int bytes_sent = 0;
    int offset = 0;
    int remain = buffer_size;
    while ((remain > 0) && ((bytes_sent = sendto(sockfd, buffer + offset, MAXLINE, 
        MSG_CONFIRM, (struct sockaddr *) &server_addr, sizeof(server_addr) )) > 0))
    {   
        offset += bytes_sent;
        remain -= bytes_sent;
    }
}