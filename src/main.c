#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE 1024


void send_response(int client_fd, const char *status, const char *content_type, const char *body) {
    char response[SIZE];
    snprintf(response, SIZE, "HTTP/1.1 %s\r\nContent-Type: %s\r\n\r\n%s", status, content_type, body);
    write(client_fd, response, strlen(response));
}

int main(int argc, char *argv[]){
    //sockfd for listening and accepting client's connection, newsockfd for response to client, 
    int sockfd, newsockfd, clilen;
    char method[10], route[100];


    //SOCKET()
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket, and store the return value for error checking
    if(sockfd < 0){
        perror("Error creating socket");
        exit(1);
    }

    //BIND()
    //Forcefully attaching socket to port 8080  
    struct sockaddr_in server_addr;

    //memset helps us to fill a part of memory with specific value, and help with reseting
    memset(&server_addr, 0, sizeof(server_addr)); //bzero can be used as well

    // assignments
    server_addr.sin_family = AF_INET;            // IPv4 will be used
    server_addr.sin_port = htons(8080);          // htons() function makes sure that numbers are stored in memory in network byte order,
    server_addr.sin_addr.s_addr = INADDR_ANY;    // binding for all interfaces

    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind Error");
        exit(1);
    }

    //LISTEN()
    //listen function listens to connection that comes over sockfd, with 2nd argument it limits thw queue
    if(listen(sockfd, 5) < 0){   
        perror("Listen Error");  
        exit(1);  
    }

    //ACCEPTING     
    struct sockaddr_in client_addr;
    clilen = sizeof(client_addr);
    while(1){

        newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &clilen);
        if (newsockfd < 0) {
            perror("Accept Error");
            exit(1);
        }
        printf("OMG! We got a connector\n");

        char *request = (char *)malloc(SIZE * sizeof(char));
        read(newsockfd, request, SIZE);
        sscanf(request, "%s %s", method, route);

        if(strcmp(method, "GET") != 0){
            perror("Request Error");
            exit(1);
        }

        printf("Method: %s\nRoute: %s\n", method, route);
        free(request); 

        char resourcePath[SIZE];
        sprintf(resourcePath, ".%s", route);
        char *first_line = "HTTP/1.1 200 OK";
        char responseHeader[SIZE];
        if (strstr(resourcePath, ".html")) {
            snprintf(responseHeader, SIZE,first_line, "Content-Type: text/html\r\n");
        } 
        else if (strstr(resourcePath, ".css")) {
            snprintf(responseHeader, SIZE, first_line,"Content-Type: text/css\r\n");
        }

        FILE *file = fopen(resourcePath, "r");
        if (file == NULL) {
            perror("Resource not found");
            exit(1);
        }


        /////sending http
        send_response(newsockfd, responseHeader, "text/html", ""); // Send initial response header

        
        char response[SIZE];
        size_t bytesRead;

        while ((bytesRead = fread(response, 1, SIZE, file)) > 0) {
            write(newsockfd, response, bytesRead); // Send file content
        }
        fclose(file);
        close(newsockfd);
        

    }


    

    

    return 0;
}

