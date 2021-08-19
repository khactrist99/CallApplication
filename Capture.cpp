/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Capture.cpp
 * Author: trink
 * 
 * Created on August 19, 2021, 3:51 PM
 */

#include "Capture.h"

Capture::Capture() {
}

Capture::Capture(const Capture& orig) {
}

Capture::~Capture() {
}

bool Capture::open(const char * filename) {
    fp=fopen(filename,"rb+");

	if(fp==NULL){
		printf("cannot open this file\n");
		return false;
	}
}

int Capture::read(unsigned char * buffer, int buffer_size) {
    int bytes_read = 0;
    if (( bytes_read = fread(buffer, 1, buffer_size, fp) ) != buffer_size){
        return 0;
    }
    return bytes_read;
}