#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdbool.h>
#include<stdlib.h>
#define MAXR 100
#define MAXN 100
const int maxn,maxr;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; //Initialize Mutex lock.
int total_resources[MAXR]={0};
int available[MAXR]={0}; 	//To store the available instances of the ith resource.
int max[MAXN][MAXR]; 		//To store the max number of resources of a given type,the process will allocate. 
int allocated[MAXN][MAXR]; 	//To store the allocated instance of each resource for the following process.
int need[MAXN][MAXR];		//To store the required number of instances of a resource a process needs in order to get completed.
int state[MAXN];			//To control each thread.
void getch();				//Self defined getch().
#include "ui.c"				//File contains funcitons related to UI
#include "thread.c"			//File contains functions implementation related to threads.
void init()
{
	//Initialize all the shared data to 0
	for(int i=0;i<MAXN;i++)
	{
		for(int j=0;j<MAXR;j++)
		{
			max[i][j]=0;
			allocated[i][j]=0;
			need[i][j]=0;
		}
	}
	//All the processes in the beginning are unfinished.
	for(int i=0;i<MAXN;i++)
		state[i]=0;
}

//unix doesn't support getch(), self defined getch().
void getch()
{	
	char c;
	fflush(stdout);
	fflush(stdin);
	read(0, &c, 1);
		return;
}
int main()
{
	int i,j,ch;
	init();
	printf("Enter the number of resources = ");
	scanf("%d",&maxr);
	printf("Enter the number of processes = ");
	scanf("%d",&maxn);
	printf("Enter the maximum instances of each resources type = ");
	for(i=0;i<maxr;i++)
	{
		scanf("%d",&total_resources[i]);
		available[i]=total_resources[i];
	}
	printf("Enter the maximum number of instance's of each resource the processes require = \n");
	for(i=0;i<maxn;i++)
	{
		printf("Enter the resources requirement for process P%d = ",i);
		for(j=0;j<maxr;j++)
			{
				scanf("%d",&max[i][j]);
				need[i][j]=max[i][j];
				if(max[i][j]>available[j])
				{
					printf("Error!! Maximum required instance of a resource cann't be greater than available instance's\n");
					getch();
					return -1;
				}
			}
	}
	pthread_t threads[maxn];
	//Creating maxn threads.
	for(i=0;i<maxn;i++)
		pthread_create(&threads[i],NULL,generic_thread,(void *)i);
	while(1)
	{
		menu();
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				show_status();
				getch();
				break;
			case 2:
				initiate_request();
				break;
			case 3:
				begin_termination();
				break;
			case 4:
				show_safe_state();
				break;
			case 5:
				printf("Simulation Terminated, press enter to exit.........\n");
				getch();
				return 0;
			default:
				printf("Invalid choice press enter to continue..........\n");
				getch();
				break;
		}
		int i;
		for(i=0;i<maxn;i++)
		{
			if(state[i]!=-1)
				break;
		}
		if(i==maxn)
		break;
	}
	system("clear");
	printf("There are no more process left in the system. Press enter key to exit simulation....");
	getch();
	return 0;
}