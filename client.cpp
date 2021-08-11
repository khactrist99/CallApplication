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

int main(int argc, char** argv ) {
    //declare for socket
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t addr_len;
    int buffer_size, bytes_sent = 0;
    //declare for camera, from Opencv
    Mat image;
    VideoCapture videoCapture;
    vector<uchar> buffer;

    memset(&servaddr, 0, sizeof(servaddr));  
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
      
    
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    namedWindow("Video Capture", WINDOW_AUTOSIZE ); 
    videoCapture.open(0); //open camera

    while (true) 
    {
        videoCapture.read(image);
        imshow("Video Capture", image);

        //encode Mat to vector
        imencode(".jpg", image, buffer);

        buffer_size = buffer.size();
        
        //send buffer size to Server
        addr_len = sizeof(servaddr);
        if (bytes_sent = sendto(sockfd, &buffer_size, sizeof(buffer_size),
            MSG_CONFIRM, (struct sockaddr *) &servaddr, addr_len) < 0) 
            {
                cout << "send error" << endl;
            }
        else
            {   
                bytes_sent = sendto(sockfd, &buffer[0], buffer.size(),
                    MSG_CONFIRM, (struct sockaddr *) &servaddr, addr_len);
                // cout << bytes_sent << ";" << bytes_sent - buffer_size << endl; //print for debug
            }      
        if(waitKey(30) >= 0) break;
    }     
    
    close(sockfd);
    return 0;
}