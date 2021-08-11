#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio_c.h>

#define PORT 8080

using namespace std;
using namespace cv;

using namespace std;
using namespace cv;

#define PORT 8080
#define MAXLINE 1024
  
// Driver code
int main() {
    int sockfd;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int buffer_size, bytes_recv = 0;
    vector<uchar> buffer;
    Mat image;

    memset(&client_addr, 0, sizeof(client_addr));
    // Filling server information
    client_addr.sin_family    = AF_INET; // IPv4
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(PORT);

    namedWindow("Video Playback", WINDOW_AUTOSIZE); //set name and size of window

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
      
    // Bind the socket with the server address
    if ( bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
      

    while (true)
    {   
        addr_len = sizeof(client_addr);
        //Recieves the byte size of the client file.
        if (bytes_recv = recvfrom(sockfd, &buffer_size, sizeof(int), 
            MSG_WAITALL, (struct sockaddr *) &client_addr, &addr_len) < 0)  
            {
                cout << "receive error" << endl;
            }
        else {
            try {
            buffer.resize(buffer_size);
            if (bytes_recv = recvfrom(sockfd, &buffer[0], buffer.size(), 
                MSG_WAITALL, (struct sockaddr *) &client_addr, &addr_len) > 0)
                {  
                // cout << bytes_sent << ";" << bytes_sent - buffer_size << endl; //print for debug    
                
                //decode buffer to Mat, then show the image
                image= imdecode(buffer, 1);
                imshow("Video Playback", image);
                }
            }
            catch (...) {
                // cerr << e.msg << endl;
            }
            if(waitKey(30) >= 0) break;
        }
    }
         
      
    return 0;
}