// Client side implementation of UDP client-server model
#include "Network.h"
#include "Capture.h"

#define BUFFER_SIZE 460800

unsigned char buffer[BUFFER_SIZE];

int main(int argc, char* argv[]) {
    Network network;
    network.Init();

    const char * filename = "out.yuv";
    Capture capture;
    capture.open(filename);
    while (capture.read(buffer, BUFFER_SIZE) > 0)
    {
        network.send(buffer, BUFFER_SIZE);
    }
    network.Close();
    return 0;
}
