/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Interface.h
 * Author: trink
 *
 * Created on August 19, 2021, 2:47 PM
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "SDL2/SDL.h"

//Refresh Event
#define REFRESH_EVENT  (SDL_USEREVENT + 1)

#define BREAK_EVENT  (SDL_USEREVENT + 2)

class Interface {
public:
	Interface();
	Interface(const Interface& orig);
	virtual ~Interface();
	bool Init();
	void Run();
	void Render(unsigned char *);
private:
	int screen_w=640,screen_h=480;
	const int pixel_w=640,pixel_h=480;
	SDL_Window *screen; 
	SDL_Renderer* sdlRenderer;
	Uint32 pixformat= SDL_PIXELFORMAT_IYUV; //IYUV: Y + U + V  (3 planes) or YV12: Y + V + U  (3 planes)
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect;  

};

#endif /* INTERFACE_H */

