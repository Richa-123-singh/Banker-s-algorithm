//Function to make a copy of the global variables. 
void make_copy(int temp_available[maxr],int temp_allocated[maxn][maxr],int temp_need[maxn][maxr])
{
	for(int i=0;i<maxn;i++)
	{
		for(int j=0;j<maxr;j++)
			{
			temp_allocated[i][j]=allocated[i][j];
			temp_need[i][j]=need[i][j];
		}
	}
	for(int i=0;i<maxr;i++)
		temp_available[i]=available[i];
}

//Function to terminate a thread.
void terminate(int thread_id)
{
	pthread_mutex_lock(&m);
	for(int i=0;i<maxr;i++)
	{
		available[i]+=allocated[thread_id][i];
		allocated[thread_id][i]=0;
	}
	pthread_mutex_unlock(&m);
		return;
}

//Function to begin termination of a thread.
void begin_termination()
{
	system("clear");
	status_module();
	int thread_id;
	while(1)
	{
		printf("\nEnter the process id you want to eliminate:");
		scanf("%d",&thread_id);
		if(!(thread_id>=0 && thread_id<maxn) || state[thread_id]==-1)
			printf("Error! Thread doesn't exist!!!\n");
		else
			break;
	}
	state[thread_id]=2;  		//Change the state of the thread to initiate termination.
	while(state[thread_id]==2); //Wait while terminate() does it's work.
	printf("Process Terminated!!!");
	getch();
	return;
}

//Function to check after resource allocation if the thread is completed or not.
void check_termination(int thread_id)
{
	for(int i=0;i<maxr;i++)
	{
		if(need[thread_id][i]!=0)
			return;
	}
	printf("The process P%d has completed! It will be now terminate.",thread_id);
	getch();
	state[thread_id]=2;
	while(state[thread_id]==2);
	return;
}

//Algorithm to check system's safe state and to print safe sequence.
bool safety_algorithm(int temp_available[maxr],int temp_allocated[maxn][maxr],int temp_need[maxn][maxr])
{
	bool is_finished[maxn],found;
	int count=0,safe_sequence[maxn],terminated=0;
	for(int i=0;i<maxn;i++)
	{
		if(state[i]!=-1)
			is_finished[i]=false;
		else
			{
				is_finished[i]=true;
				safe_sequence[count++]=i;
			}
	}
	terminated=count;
	while(count<maxn)
	{
		found=false;
		for(int i=0;i<maxn;i++)
		{
			if(!is_finished[i])
			{
				int j;
                for (j=0;j<maxr;j++)
                    if (temp_need[i][j]>temp_available[j])
                        {
                        	break;
                        }
                if (j==maxr)
                {
                    for (int k=0;k<maxr;k++)
                        temp_available[k]+=temp_allocated[i][k];
                    safe_sequence[count++]=i;
                    is_finished[i]=true;
                    found=true;
				}
			}
		}
		if(found==false)
		{
			return false;
		}
	}
	printf("\nThe Safe Sequence is:\n");
	for(int i=terminated;i<maxn-1;i++)
	{
		printf("P%d ",safe_sequence[i]);
			printf("-> ");
	}
	printf("P%d",safe_sequence[maxn-1]);
	return true;
}

//Function invokes safety_algorithm to print safe sequence on copy of global variables.
void show_safe_state()
{
	int temp_available[maxr],temp_allocated[maxn][maxr],temp_need[maxn][maxr];
	make_copy(temp_available,temp_allocated,temp_need);	
	safety_algorithm(temp_available,temp_allocated,temp_need);
	getch();
	return;
}

//Calls safety_algorithm() and allocate resources only if it returns true.
void check_request(int thread_id,int res[])
{
	int temp_available[maxr],temp_allocated[maxn][maxr],temp_need[maxn][maxr];
	make_copy(temp_available,temp_allocated,temp_need);	
	for(int i=0;i<maxr;i++)
	{
		temp_allocated[thread_id][i]+=res[i];
		temp_need[thread_id][i]-=res[i];
		temp_available[i]-=res[i];
	}
	if(safety_algorithm(temp_available,temp_allocated,temp_need))
	{
		printf("\nResources allocated!!\n");
		pthread_mutex_lock(&m);
		for(int i=0;i<maxr;i++)
		{
			allocated[thread_id][i]+=res[i];
			need[thread_id][i]-=res[i];
			available[i]-=res[i];
	   }	
	   pthread_mutex_unlock(&m);
	}
	else
	{
		printf("\nThe Request is denied, as granting the request doesn't leave the system in a safe state!!");
	}
	return;
}

//Function used by thread to make a request for resource allocation
void request(int thread_id)
{
	int res[maxr];
	status_module();
	printf("\nEnter the values of resources for the process P%d = \n",thread_id);
	for(int i=0;i<maxr;i++)
	{
		while(1)
		{
		scanf("%d",&res[i]);
		if(res[i]>need[thread_id][i])
			printf("Error!! Process is requesting more resources than defined!!\nPlease Enter again!=\n");	
		else
			break;
		}
	}
	check_request(thread_id,res);
	getch();
	return;
}

//Funciton to initiate a resource request.
void initiate_request()
{
	system("clear");
	int thread_id,res[maxr];
	while(1)
	{
	printf("Enter the process number = ");
	scanf("%d",&thread_id);
	if(!(thread_id>=0 && thread_id<maxn) || state[thread_id]==-1)
	printf("Error! Thread doesn't exist!!!\n");
	else
	break;
	}
	state[thread_id]=1;
	while(state[thread_id]==1);
	check_termination(thread_id);
	return;
}

//The basic structure used to make all the threads.
void *generic_thread(void *data)
{
	int pthread_id=(int *)data;
	while(1)
	{
		if(state[pthread_id]==1)
		{
			request(pthread_id);
			state[pthread_id]=0;
		}
		else if(state[pthread_id]==2)
		{
			terminate(pthread_id);
			state[pthread_id]=-1;
			pthread_exit(NULL);
		}
	}
}