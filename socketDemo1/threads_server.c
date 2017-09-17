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

int port = 8000;
struct sockaddr_in pin;
void thread_server(int temp_sock_descriptor);

int count=1;

int main(void){
    int sock_descriptor,temp_sock_descriptor,address_size;
    struct sockaddr_in sin;
	pthread_t thread;
    sock_descriptor = socket(AF_INET,SOCK_STREAM,0);
    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    if(bind(sock_descriptor,(struct sockaddr*)&sin,sizeof(sin))==-1){
        perror("call to bind");
        exit(1);
    }
    if(listen(sock_descriptor,100)==-1){
        perror("call to listen");
        exit(1);
    }
    printf("Accepting connections...\n");
    address_size = sizeof(struct sockaddr_in);
    
    while(1){
        temp_sock_descriptor = accept(sock_descriptor,(struct sockaddr*)&pin,&address_size);
        pthread_create(&thread,NULL,(void*)thread_server,(void*)temp_sock_descriptor);
        count++;
    }
    
}
void thread_server(int temp_sock_descriptor){
    pthread_detach(pthread_self());
    char buf[16384];
    char host_name[20];
    int len;
    inet_ntop(AF_INET,&pin.sin_addr,host_name,sizeof(host_name));
    printf("%s connected,thread[%lu] is servring for client(%s) !\n",host_name,pthread_self(),host_name);
    
    recv(temp_sock_descriptor,buf,16384,0);

    while(strcmp(buf,"quit")!=0){
        printf("received from client(%s):%s\n",host_name,buf);
        len = strlen(buf);
        for(int i=0;i<len;i++){
            buf[i] = toupper(buf[i]);
        }
        send(temp_sock_descriptor,buf,len+1,0);
        recv(temp_sock_descriptor,buf,16384,0);
    }
    if(!close(temp_sock_descriptor)){printf("%s disconnected!\n",host_name);}
}

