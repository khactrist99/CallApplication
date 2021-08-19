/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Capture.h
 * Author: trink
 *
 * Created on August 19, 2021, 3:51 PM
 */

#ifndef CAPTURE_H
#define CAPTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

class Capture {
public:
    Capture();
    Capture(const Capture& orig);
    virtual ~Capture();
    bool open(const char * );
    int read(unsigned char *, int);
    
private:
    FILE *fp=nullptr;
    unsigned char buffer[];
};

#endif /* CAPTURE_H */

