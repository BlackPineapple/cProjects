#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
void task1(int *counter){
	while(*counter<5){
		printf("task1 count:%d\n",*counter);
		(*counter)++;
		sleep(1);
	}
	pthread_exit((void*)100);
}
