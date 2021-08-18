#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <queue>
#include <vector>
extern "C"
{
#include <SDL2/SDL_mutex.h>
#include "SDL2/SDL.h"
};

#define PORT 8080
#define MAXLINE 1024

 using namespace std;

const int bpp=12;

int screen_w=640,screen_h=480;
const int pixel_w=640,pixel_h=480;

#define BUFFER_SIZE = pixel_w*pixel_h*bpp/8;

// unsigned char buffer[pixel_w*pixel_h*bpp/8];


struct FrameQueue
{
    FrameQueue()
    {
        eventNumber = SDL_RegisterEvents(1);
        mutex = SDL_CreateMutex();
    }
    ~FrameQueue()
    {
        SDL_DestroyMutex( mutex );
    }

    void Lock()
    {
        SDL_LockMutex( mutex );
    }

    void Unlock()
    {
        SDL_UnlockMutex( mutex );
    }

    Uint32 eventNumber;
    queue< vector< unsigned char > > frames;
	unsigned char *pBuffer;
private:
    SDL_mutex* mutex;
};

int thread_exit=0;

int recv_data(void *ptr){
	FrameQueue& fq = *((FrameQueue*)ptr);
	int sockfd;
    
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;

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
    server_addr.sin_port = htons(PORT);
      
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&server_addr, 
            sizeof(server_addr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    addr_len = sizeof(client_addr); 
    vector <unsigned char> buffer;
	buffer.resize(pixel_w*pixel_h*bpp/8);
	int offset = 0;
	int bytes_recv = 0;
	int remain = pixel_w*pixel_h*bpp/8;
	for (int i = 0; i < 10; i++) //Testing, 10 loop, crash when using while
	// while (1)
	{
		
		while ((remain > 0) && ((bytes_recv = recvfrom(sockfd, &buffer[offset]  , MAXLINE, 
				MSG_WAITALL, (struct sockaddr *) &client_addr, &addr_len)) > 0))
		{
			offset += bytes_recv;
			remain -= bytes_recv;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
		}
	
		fq.Lock();
		fq.frames.push( buffer );
		fq.Unlock();
		SDL_Event event;
		event.type = fq.eventNumber;
		SDL_PushEvent( &event );
	}
	
	return 0;
}

// Driver code
int main() {
    if(SDL_Init(SDL_INIT_VIDEO)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	} 

	SDL_Window *screen; 
	//SDL 2.0 Support for multiple windows
	screen = SDL_CreateWindow("RECEIVER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screen_w, screen_h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if(!screen) {  
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
		return -1;
	}
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  

	Uint32 pixformat=0;

	//IYUV: Y + U + V  (3 planes)
	//YV12: Y + V + U  (3 planes)
	pixformat= SDL_PIXELFORMAT_IYUV;  

	SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer,pixformat, SDL_TEXTUREACCESS_STREAMING,pixel_w,pixel_h);

	SDL_Rect sdlRect;  

    FrameQueue fq;
	SDL_Thread *refresh_thread = SDL_CreateThread(recv_data,NULL,(void*)&fq);
	
	// SDL_Event event;
    bool running = true;
    SDL_Event event;
	while(running){
		//Wait
		SDL_WaitEvent(&event);
		if(event.type==fq.eventNumber){
			fq.Lock();
			vector< unsigned char > frame = fq.frames.front();
			fq.frames.pop();			
			fq.Unlock();
			SDL_UpdateTexture( sdlTexture, NULL, &frame[0], pixel_w);  

			//FIX: If window is resize
			sdlRect.x = 0;  
			sdlRect.y = 0;  
			sdlRect.w = screen_w;  
			sdlRect.h = screen_h;  
			
			SDL_RenderClear( sdlRenderer );   
			SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, &sdlRect);  
			SDL_RenderPresent( sdlRenderer );  
			SDL_Delay(33);
		}else if(event.type==SDL_WINDOWEVENT){
			//If Resize
			SDL_GetWindowSize(screen,&screen_w,&screen_h);
		}else if(event.type==SDL_QUIT){
			running=false;
		}
	}
	SDL_Quit();

    return 0;
}