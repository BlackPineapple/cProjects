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
char *host_name;
//char *client_name="client[1]";
void recv_message(int socket_descriptor);
void send_message(int socket_descriptor);

int main(int argc,char* argv[]){
	int socket_descriptor;
	struct sockaddr_in pin;
	pthread_t recv,send;
	
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

	pthread_create(&recv,NULL,(void *)recv_message,(void *)socket_descriptor);
	pthread_create(&send,NULL,(void *)send_message,(void *)socket_descriptor);

	pthread_join(send,NULL);
	pthread_cancel(recv);
	pthread_join(recv,NULL);

	close(socket_descriptor);
	printf("\nYou were disconnected , see you !\n");

	return 0;
}
void recv_message(int socket_descriptor){
	char buf[8192];
	while(1){
		if(read(socket_descriptor,buf,8192)==0){
			printf("\nServer brokedown ! Please enter 'quit' to end!\n");
			break;
		}
		printf("response:%s\n\n",buf);
		memset(buf,0,sizeof(buf));
	}
}
void send_message(int socket_descriptor){
	char buf[8192];

	while(1){
		//printf("\n%s:\n:",client_name);		
		gets(buf);
		putchar('\n');
		if(strncmp(buf,"quit",4)==0){break;}
		if(buf[0]!='\0'){write(socket_descriptor,buf,sizeof(buf));}
		memset(buf,0,sizeof(buf));
	}
}
