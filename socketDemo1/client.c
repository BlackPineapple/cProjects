#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

char *host_name = "127.0.0.1";
char *client_name="client";
int port = 8000;

int main(int argc,char* argv[]){
	
	char buf[8192];
	int socket_descriptor;
	struct sockaddr_in pin;
	//char *str;
	char input[8192]={0};
	//str=input;
	memset(&pin,0,sizeof(pin));
	pin.sin_family = AF_INET;
	inet_pton(AF_INET,host_name,&pin.sin_addr);
	pin.sin_port = htons(port);
	socket_descriptor = socket(AF_INET,SOCK_STREAM,0);
	if(connect(socket_descriptor,(struct sockaddr *)&pin,sizeof(pin))==-1){
		perror("Error connecting to socket\n");
		exit(1);
	}
	
	printf("connected!\n");
	
	while(1){
		printf("\n%s:\n",client_name);
		//scanf("%s",input);
		gets(input);
		write(socket_descriptor,input,sizeof(input));
		if(strcmp(input,"quit")==0){break;}
		read(socket_descriptor,buf,8192);
		printf("\nResponse from server:\n\n%s\n",buf);
		memset(input,0,sizeof(input));
	}

	close(socket_descriptor);
	printf("Bye!\n");
	return 1;

}
