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
char host_name[16];
pthread_t recvThrd,sendThrd;
void recv_message(int socket_descriptor);
void send_message(int socket_descriptor);
int main(int argc,char* argv[]){
	int listen_socket,address_size;
	int socket_descriptor;
	struct sockaddr_in pin,sin;

	listen_socket=socket(AF_INET,SOCK_STREAM,0);
	memset(&sin,0,sizeof(sin));
	sin.sin_addr.s_addr=INADDR_ANY;
	sin.sin_port=htons(port);
	bind(listen_socket,(struct sockaddr*)&sin,sizeof(sin));
	listen(listen_socket,1);

	address_size=sizeof(pin);
	while(1){
		printf("\nI'm ready!\n");
		socket_descriptor=accept(listen_socket,(struct sockaddr*)&pin,&address_size);
		inet_ntop(AF_INET,&pin.sin_addr,host_name,sizeof(host_name));
		printf("\n%s connected!\n\n",host_name);
		pthread_create(&recvThrd,NULL,(void *)recv_message,(void *)socket_descriptor);
		pthread_create(&sendThrd,NULL,(void *)send_message,(void *)socket_descriptor);

		pthread_join(recvThrd,NULL);
		pthread_join(sendThrd,NULL);
		close(socket_descriptor);
		printf("\n%s disconnected!\n",host_name);
	}

	return 0;
}
void recv_message(int socket_descriptor){
	char buf[8192];
	while(1){
		if(read(socket_descriptor,buf,8192)==0){
			printf("\nClient shutdown!\n");
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
			break;
		}
		if(buf[0]!='\0'){write(socket_descriptor,buf,sizeof(buf));}
		memset(buf,0,sizeof(buf));
	}
}
