#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<netinet/in.h>
#include<ctype.h>
char host_name[20];
int port = 8000;

int main(void){
	struct sockaddr_in sin,pin;
	int sock_descriptor,temp_sock_descriptor,address_size;
	int i,len;
	char buf[16384];

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

	while(1){
		address_size = sizeof(pin);
		temp_sock_descriptor = accept(sock_descriptor,(struct sockaddr*)&pin,&address_size);
		if(temp_sock_descriptor == -1){
			perror("call to accept");
			exit(1);
		
		}

		inet_ntop(AF_INET,&pin.sin_addr,host_name,sizeof(host_name));
		printf("%s connected!\n",host_name);
		
		if(recv(temp_sock_descriptor,buf,16384,0) == -1){
			perror("call to recv");
			exit(1);
		}

		while(strcmp(buf,"quit")!=0){
			printf("received from client(%s):%s\n",host_name,buf);

			len = strlen(buf);
			for(i=0;i<len;i++){
				buf[i] = toupper(buf[i]);
			}
			send(temp_sock_descriptor,buf,len+1,0);
			recv(temp_sock_descriptor,buf,16384,0);
		}

		if(!close(temp_sock_descriptor)){ printf("%s disconnected!\n",host_name);	}
	}
}
