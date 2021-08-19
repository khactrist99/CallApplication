/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Network.h
 * Author: trink
 *
 * Created on August 19, 2021, 2:30 PM
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 1024

class Network {
public:
    Network();
    Network(const Network& orig);
    virtual ~Network();
    void Init();
    void Bind();
    void recv(unsigned char * buffer, int data_size);
    void send(unsigned char * buffer, int data_size);
private:
    int port = 8080;
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len; 
};

#endif /* NETWORK_H */

