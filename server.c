/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

void dostuff(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    char buffer[256];

    //  struct sockaddr_in {
    //         short   sin_family;
    //         u_short sin_port;
    //         struct  in_addr sin_addr;
    //         char    sin_zero[8];
    // };
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

    // The function bzero() sets all values in a buffer to zero. 
    // It takes two arguments, the first is a pointer to the buffer and the second is the size of the buffer. 
    // Thus, this line initializes serv_addr to zeros.
     bzero((char *) &serv_addr, sizeof(serv_addr));

     // atoi() function to convert this from a string of digits to an integer.
     portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;

     // This field contains the IP address of the host. 
     // For server code, this will always be the IP address of the machine on which the server is running, 
     // and there is a symbolic constant INADDR_ANY which gets this address.
     serv_addr.sin_addr.s_addr = INADDR_ANY;

     // instead of simply copying the port number to this field, 
     // it is necessary to convert this to network byte order using the function htons() 
     // which converts a port number in host byte order to a port number in network byte order.
     serv_addr.sin_port = htons(portno);

     // It takes three arguments, 
     // the socket file descriptor, the address to which is bound, and the size of the address to which it is bound. 
     // The second argument is a pointer to a structure of type sockaddr, 
     // but what is passed in is a structure of type sockaddr_in, and so this must be cast to the correct type.
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

    // The listen system call allows the process to listen on the socket for connections. 
    // The first argument is the socket file descriptor, and the second is the size of the backlog queue, 
    // i.e., the number of connections that can be waiting while the process is handling a particular connection. 
    // This should be set to 5, the maximum size permitted by most systems. 
    // If the first argument is a valid socket, this call cannot fail, and so the code doesn't check for errors. 
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
    while (1) {
        newsockfd = accept(sockfd, 
            (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0)  {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
        else close(newsockfd);
    } /* end of while */
    return 0; /* we never get here */ 
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
      
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}