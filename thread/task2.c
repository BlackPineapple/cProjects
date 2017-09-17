#include<stdio.h>

void task2(int *counter){
	while(*counter<5){
		printf("task2 count:%d\n",*counter);
		(*counter)++;
	}
}
