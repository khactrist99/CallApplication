/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Interface.cpp
 * Author: trink
 * 
 * Created on August 19, 2021, 2:47 PM
 */

#include "Interface.h"

Interface::Interface() {
}

Interface::Interface(const Interface& orig) {
}

Interface::~Interface() {
}

bool Interface::Init() {
    if(SDL_Init(SDL_INIT_VIDEO)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return false;
	} 

	screen = SDL_CreateWindow("RECEIVER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if(!screen) {  
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
		return false;
	}
	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  

	sdlTexture = SDL_CreateTexture(sdlRenderer,pixformat, SDL_TEXTUREACCESS_STREAMING,pixel_w,pixel_h);
    return true;
}

void Interface::Render(unsigned char * buffer) {
        SDL_UpdateTexture( sdlTexture, NULL, buffer, pixel_w);  

        //FIX: If window is resize
        sdlRect.x = 0;  
        sdlRect.y = 0;  
        sdlRect.w = screen_w;  
        sdlRect.h = screen_h;  
        
        SDL_RenderClear( sdlRenderer );   
        SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, &sdlRect);  
        SDL_RenderPresent( sdlRenderer );  
}

