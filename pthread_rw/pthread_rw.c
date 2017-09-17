#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#define MAX_THRD 5
#define BUF_SIZE 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int count=0;
struct rw_file{
	FILE *srcfile;
	FILE *desfile;
};

void thread_rw(struct rw_file *rw_file);

int main(int argc,char *argv[]){
	
	pthread_t thrd[MAX_THRD];
	int ret;
	struct rw_file rw_file;

	if((rw_file.srcfile=fopen("./source_file","rb"))==NULL){
		printf("can not open source file!\n");
		exit(-1);	
	}

	if((rw_file.desfile=fopen("./dest_file","wb"))==NULL){
		printf("can not open destination file!\n");
		exit(-1);	
	}
	
	for(int i=0;i<MAX_THRD;i++){
		ret = pthread_create(&thrd[i],NULL,(void *)thread_rw,(void *)&rw_file);
	}
	for(int i=0;i<MAX_THRD;i++){pthread_join(thrd[i],NULL);}
	
	printf("This is main process :%x:All threads have done!\n",(int)pthread_self());	
	
	fclose(rw_file.srcfile);
	fclose(rw_file.desfile);

	return 0;
}
void thread_rw(struct rw_file *rw_file){
	char buf[BUF_SIZE];
	int id=++count;
	while(1){
		pthread_mutex_lock(&mutex);
		memset(buf,0,BUF_SIZE);
		if(fgets(buf,BUF_SIZE,rw_file->srcfile)!=NULL){
			fputs(buf,rw_file->desfile);
			printf("This is thread %d:%x:%s\n",id,(int)pthread_self(),buf);
		}	
		else{pthread_mutex_unlock(&mutex);break;}
		pthread_mutex_unlock(&mutex);
		sleep(0.1);
	}
}

