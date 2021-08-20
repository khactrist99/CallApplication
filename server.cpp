#include "Render.h"
#include "Network.h"

#define BUFFER_SIZE 460800

unsigned char buffer[BUFFER_SIZE];
bool running = true;

int recv_data(void *ptr){
<<<<<<< Updated upstream
	Network network;
	network.Init();

	bool running = true;
	while(running)
	{
		network.recv(buffer, BUFFER_SIZE);
=======
    Network network;
    network.Init();
	network.Bind();
    while(1)
    {
        if (network.recv(buffer, BUFFER_SIZE) == END) {
			running = false;
			break;
		}
>>>>>>> Stashed changes
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent( &event );
	}
	return 0;
}

int main() {
<<<<<<< Updated upstream
	Interface UI;
	UI.Init();
=======
    Interface UI;
    UI.Init("RECEIVER");
>>>>>>> Stashed changes
	SDL_Thread *refresh_thread = SDL_CreateThread(recv_data,NULL, NULL);

	// SDL_Event event;
<<<<<<< Updated upstream
	bool running = true;
	SDL_Event event;
=======
    
    SDL_Event event;
>>>>>>> Stashed changes
	while(running){
		//Wait
		SDL_WaitEvent(&event);
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
