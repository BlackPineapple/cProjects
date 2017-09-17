#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<netinet/in.h>
#include<ctype.h>
#include<pthread.h>
#include<errno.h>

int port=8000;
pthread_t recvThrd,sendThrd;
char *host_name;
void recv_message(int socket_descriptor);
void send_message(int socket_descriptor);

int main(int argc,char* argv[]){
    int socket_descriptor;
    struct sockaddr_in pin;
    host_name=argv[1];
    port=atoi(argv[2]);
    memset(&pin,0,sizeof(pin));
    pin.sin_family = AF_INET;
    inet_pton(AF_INET,host_name,&pin.sin_addr);
    pin.sin_port=htons(port);
    socket_descriptor = socket(AF_INET,SOCK_STREAM,0);
    
    if(connect(socket_descriptor,(struct sockaddr *)&pin,sizeof(pin))==-1){
        perror("Error connecting to socket\n");
        exit(1);
    }
    printf("\nConnected with %s !\n\n",host_name);
    
    pthread_create(&recvThrd,NULL,(void *)recv_message,(void *)socket_descriptor);
    pthread_create(&sendThrd,NULL,(void *)send_message,(void *)socket_descriptor);
    
    pthread_join(sendThrd,NULL);
    pthread_join(recvThrd,NULL);
    
    close(socket_descriptor);
    printf("\nYou were disconnected , see you !\n");
    
    return 0;
}
void recv_message(int socket_descriptor){
    char buf[8192];
    while(1){
        if(read(socket_descriptor,buf,8192)==0){
            printf("\nServer shutdown ! \n");
            pthread_cancel(sendThrd);
            break;
        }
        printf("response:%s\n\n",buf);
        memset(buf,0,sizeof(buf));
    }
}
void send_message(int socket_descriptor){
    char buf[8192];
    while(1){
        gets(buf);
        putchar('\n');
        if(strcmp(buf,"quit")==0){
            pthread_cancel(recvThrd);
            break;}
        if(buf[0]!='\0'){write(socket_descriptor,buf,sizeof(buf));}
        memset(buf,0,sizeof(buf));
    }
}
