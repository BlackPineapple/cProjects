#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>
#include"thread.h"


int main(){
	pthread_t thrd1,thrd2;
	int ret;
	void *retval;

	ret = pthread_create(&thrd1,NULL,(void*)task1,(void *)&g1);
	printf("Im in main function!\n");

	ret = pthread_create(&thrd2,NULL,(void*)task2,(void *)&g2);

	cleanup(g1,g2);
	getchar();
	int tmp;
	if((tmp=pthread_join(thrd1,&retval))!=0){
		switch(tmp){
			case EINVAL:
				printf("pthread_join return value is EINVAL:%d\n",tmp);
				break;
			case ESRCH:
				printf("pthread_join return value is ESRCH:%d\n",tmp);
			default:
				printf("pthread_join return value is others:%d\n",tmp);
		}
	}
	printf("return val of task1 is %d\n",(int)retval);
	cleanup(g1,g2);
	exit(EXIT_SUCCESS);
}
