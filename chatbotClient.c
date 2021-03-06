/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        fgets(message, 1000, stdin);
         
        //Send some data
        send(sock , message , strlen(message) , 0);
        
        // stay in sync, wait for server to proccess
        usleep(100);
        
        // wipe the memory for the server reply
        memset(server_reply, 0, sizeof server_reply);
        
        //Receive a reply from the server
        recv(sock , server_reply , 2000 , 0);

        printf("Server reply : %s", server_reply);

    }
     
    //close(sock);
    return 0;
}