///*
//* http://ffmpeg.org/doxygen/trunk/index.html
//*
//* Main components
//*
//* Format (Container) - a wrapper, providing sync, metadata and muxing for the streams.
//* Stream - a continuous stream (audio or video) of data over time.
//* Codec - defines how data are enCOded (from Frame to Packet)
//*        and DECoded (from Packet to Frame).
//* Packet - are the data (kind of slices of the stream data) to be decoded as raw frames.
//* Frame - a decoded raw frame (to be encoded or filtered).
//*/
//
//extern "C"
//{
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswresample/swresample.h>
//#include <libavdevice/avdevice.h>
//#include <SDL2/SDL.h>
//#include <stdio.h>
//#include <stdarg.h>
//#include <stdlib.h>
//#include <string.h>
//#include <inttypes.h>
//}
//// #include "my_avutil.h"
//#include "AVModule.h"
//
//#include <stdio.h>
//// #include <stdlib.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <netinet/in.h>
//#include <iostream>
//#include <vector>
//
//#define PORT 8080
//
//using namespace std;
//
////THESE CODE FOR AUDIO-----------------------------------------
//#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
//
////Buffer:
////|-----------|-------------|
////chunk-------pos---len-----|
//static Uint8 *audio_chunk;
//static Uint32 audio_len;
//static Uint8 *audio_pos;
//
////sdl update texture
//void updateTexture(SDL_Renderer *renderer, SDL_Texture *texture, AVFrame *pFrame);
//
//void updateAudio(uint8_t *out_buffer, int out_buffer_size);
//
///* The audio function callback takes the following parameters: 
//* stream: A pointer to the audio buffer to be filled 
//* len: The length (in bytes) of the audio buffer 
//*/
//void audio_callback(void *udata, Uint8 *stream, int len);
//
///* The audio function decode packet to frame
//* then resample by au_convert_ctx 
//* then play audio by SDL
//*/
//
//bool Init(SDL_Window *&screen, SDL_Renderer *&renderer, SDL_Texture *&texture,
//          int screen_w, int screen_h)
//{
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
//    {
//        printf("Could not initialize SDL - %s\n", SDL_GetError());
//        return false;
//    }
//
//    screen = SDL_CreateWindow("Simplest FFmpeg Read Camera",
//                              SDL_WINDOWPOS_UNDEFINED,
//                              SDL_WINDOWPOS_UNDEFINED,
//                              screen_w, screen_h, 0);
//
//    if (!screen)
//    {
//        printf("SDL: could not set video mode - exiting:%s\n", SDL_GetError());
//        return false;
//    }
//
//    renderer = SDL_CreateRenderer(screen, -1, 0);
//    if (!renderer)
//    {
//        fprintf(stderr, "SDL: could not create renderer - exiting\n");
//        return false;
//    }
//
//    // Allocate a place to put our YUV image on that screen
//    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
//                                screen_w, screen_h);
//
//    if (!texture)
//    {
//        fprintf(stderr, "SDL: could not create texture - exiting\n");
//        return false;
//    }
//    return true;
//}
//
//// void Cleanup() {
//// 	SDL_DestroyTexture(texture);
////     SDL_DestroyRenderer(renderer);
////     SDL_DestroyWindow(screen);
////     SDL_CloseAudio();
////     SDL_Quit();
//// }
//
////AUDIO DEFINE END----------------------------------------
//
//int main(int argc, char *argv[])
//{   
//    
//    char *input_filename = NULL;
//    SDL_Event event;
//    //video vars
//    Uint8 *yPlane, *uPlane, *vPlane;
//    size_t yPlaneSz, uvPlaneSz;
//    int uvPitch;
//    //audio vars
//    SDL_AudioSpec audio_spec;
//    uint8_t *out_buffer;
//    int out_linesize;
//    AVModule* pAVModule;
//
//    if (argc < 2)
//    {
//        printf("You need to specify a media file.\n");
//        return -1;
//    }
//
//    input_filename = argv[1];
//    AVModule AVModule{input_filename};
//    pAVModule = &AVModule;
//    
//    if (pAVModule->open_media_and_get_format_context() < 0)
//    {
//        printf("open media fail");
//        return -1;
//    }
//    
//    if (pAVModule->prepare_decode() < 0)
//        return -1;
//
//    //
//    AVFrame *pFrame = av_frame_alloc();
//    if (!pFrame)
//    {
//        printf("failed to allocated memory for AVFrame");
//        return -1;
//    }
//    //
//    AVPacket *pPacket = av_packet_alloc();
//    if (!pPacket)
//    {
//        printf("failed to allocated memory for AVPacket");
//        return -1;
//    }
//
//    //Out Audio Param
//    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
//    int out_nb_samples = pAVModule->pAudioCodecContext->frame_size;
//    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
//    int out_sample_rate = 44100;
//
//    /* buffer is going to be directly written to a rawaudio file, no alignment */
//    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
//    //Out Buffer Size
//    int out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
//    out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
//
//    //SDL Screen
//    int screen_w = 0, screen_h = 0;
//    screen_w = pAVModule->pVideoCodecContext->width;
//    screen_h = pAVModule->pVideoCodecContext->height;
//    SDL_Window *screen;
//    SDL_Renderer *renderer;
//    SDL_Texture *texture;
//    Init(screen, renderer, texture, screen_w, screen_h);
//
//    //SDL_AudioSpec
//    audio_spec.freq = pAVModule->pAudioCodecContext->sample_rate;
//    audio_spec.format = AUDIO_S16SYS;
//    audio_spec.channels = pAVModule->pAudioCodecContext->channels;
//    audio_spec.silence = 0;
//    audio_spec.samples = out_nb_samples;
//    audio_spec.callback = audio_callback;
//    audio_spec.userdata = pAVModule->pAudioCodecContext;
//
//    if (SDL_OpenAudio(&audio_spec, NULL) < 0)
//    {
//        printf("can't open audio.\n");
//        return -1;
//    }
//
//    if (pAVModule->prepare_resample_audio(out_channel_layout, out_sample_fmt, out_sample_rate) < 0)
//    {
//        printf("Prepare_resample_audio fail!");
//        return -1;
//    }
//
//    SDL_PauseAudio(0); //Play audio
//    //SDL End------------------------
//
//    // fill the Packet with data from the Stream
//    while (pAVModule->read_frame(pPacket) >= 0)
//    {                      
//        if (!pAVModule->decode_video_packet(pPacket, pFrame)) {
//            printf("Error in decoding_video_packet");
//                return -1;
//        }
//        if (!pAVModule->decode_audio_packet(pPacket, pFrame, out_buffer, MAX_AUDIO_FRAME_SIZE)) {
//                printf("Error in decoding_audio_packet");
//                return -1;
//            }
//        if (pPacket->stream_index == pAVModule->video_stream_index) {
//            updateTexture(renderer, texture, pFrame);
//        } 
//        else if (pPacket->stream_index == pAVModule->audio_stream_index) {   
//            updateAudio(out_buffer, out_buffer_size);     
//        }
//
//        SDL_PollEvent(&event);
//        switch (event.type) {
//            case SDL_QUIT:
//                SDL_DestroyTexture(texture);
//                SDL_DestroyRenderer(renderer);
//                SDL_DestroyWindow(screen);
//                SDL_CloseAudio();
//                SDL_Quit();
//                break;
//            default:
//                break;
//        }
//    
//    }
//    av_packet_free(&pPacket);
//    av_frame_free(&pFrame);
//    av_free(out_buffer);
//    return 0;
//}
//
//void updateTexture(SDL_Renderer *renderer, SDL_Texture *texture, AVFrame *pFrame)
//{
//    SDL_UpdateYUVTexture(
//        texture,
//        NULL,
//        pFrame->data[0], pFrame->linesize[0],
//        pFrame->data[1], pFrame->linesize[1],
//        pFrame->data[2], pFrame->linesize[2]);
//
//    SDL_RenderClear(renderer);
//    SDL_RenderCopy(renderer, texture, NULL, NULL);
//    SDL_RenderPresent(renderer);
//}
//
//void audio_callback(void *udata, Uint8 *stream, int len)
//{
//    SDL_memset(stream, 0, len);
//    if (audio_len == 0)
//        return;
//
//    len = (len > audio_len ? audio_len : len); /*  Mix  as  much  data  as  possible  */
//
//    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
//    audio_pos += len;
//    audio_len -= len;
//}
//
//void updateAudio(uint8_t *out_buffer, int out_buffer_size) {
//    while (audio_len > 0) //Wait until finish
//            SDL_Delay(1);
//
//        //Set audio buffer (PCM data)
//        audio_chunk = (Uint8 *)out_buffer;
//        //Audio buffer length
//        audio_len = out_buffer_size;
//        audio_pos = audio_chunk;
//}
//
//void updateTexture(SDL_Renderer *renderer, SDL_Texture *texture, AVFrame *pFrame)
//{
//    SDL_UpdateYUVTexture(
//        texture,
//        NULL,
//        pFrame->data[0], pFrame->linesize[0],
//        pFrame->data[1], pFrame->linesize[1],
//        pFrame->data[2], pFrame->linesize[2]);
//
//    SDL_RenderClear(renderer);
//    SDL_RenderCopy(renderer, texture, NULL, NULL);
//    SDL_RenderPresent(renderer);
//}
//
//
//void updateAudio(uint8_t *out_buffer, int out_buffer_size) {
//    while (audio_len > 0) //Wait until finish
//            SDL_Delay(1);
//
//        //Set audio buffer (PCM data)
//        audio_chunk = (Uint8 *)out_buffer;
//        //Audio buffer length
//        audio_len = out_buffer_size;
//        audio_pos = audio_chunk;
//}
