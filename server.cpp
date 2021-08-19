#include "Interface.h"
#include "Network.h"
//#include "SDL2/SDL.h"

#define BUFFER_SIZE 460800

unsigned char buffer[BUFFER_SIZE];

int recv_data(void *ptr){
    Network network;
    network.Init();

    bool running = true;
    while(running)
    {
        network.recv(buffer, BUFFER_SIZE);
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent( &event );
	}
	return 0;
}

int main() {
    Interface UI;
    UI.Init();
	SDL_Thread *refresh_thread = SDL_CreateThread(recv_data,NULL, NULL);
	
	// SDL_Event event;
    bool running = true;
    SDL_Event event;
	while(running){
		//Wait
		SDL_PollEvent(&event);
		if(event.type==REFRESH_EVENT){
            UI.Render(buffer);
			SDL_Delay(33);
		}else if(event.type==SDL_QUIT){
			running=false;
		}
	}
	SDL_Quit();
    return 0;
}