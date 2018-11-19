/*
    C socket server example updated
	it can now handle multiple clients using thread
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading
 
//the thread function
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    
    // wipe the memory, sanity wipe, not needed
    memset(client_message, 0, sizeof client_message);
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        // check if the memory for the length of the string Hello
        // is the same as Hello
        if(memcmp(client_message, "Hello", strlen("Hello")) == 0){
            message = "Hello human, how are you doing today?\n";
        }

        // following else ifs are the same as above with different
        // strings
        else if(memcmp(client_message, "Goodbye", strlen("Goodbye")) == 0){
            message = "Goodbye Human, come back to chat anytime!\n";
        }

        else if(memcmp(client_message, "Good and you?", strlen("Good and you?")) == 0){
            message = "Im doing well, Thanks for asking!\n";
        }

        else if(memcmp(client_message, "High five", strlen("High five")) == 0){
            message = "I guess you can high five the keyboard, mouse or monitor but I would not recommend it\n";
        }

        else if(memcmp(client_message, "Are robots going to take over the world?", strlen("Are robots going to take over the world?")) == 0){
            message = "Maybe, but I think you are safe from chatbots like myself!\n";
        }

        // and Im only responding to five inputs, this is not a robust chat bot
        else {
            message = "Sorry but Alexander was lazy and has not programmed me to respond to that\n";
        }

        // send the message to the client
        send(sock , message , strlen(message), 0);

        // clear the memory for the next message
        memset(client_message, 0, sizeof client_message);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}