//Function which prints the main menu.
void menu()
{
	system("clear");
	printf("--------------------------Banker's Alogrithm--------------------------\n");
	printf("1. Check Status\n");
	printf("2. Allocate Resources\n");
	printf("3. Terminate a process\n");
	printf("4. See Safe Sequence\n");
	printf("5. Exit\n");
	printf("Enter one of the option = ");
}

//Shows the current state of the simulation.
void status_module()
{
	printf("\n\n\n\tAvailable system resources:\n\t");
	for(int i=0;i<maxr;i++)
		printf("R%d ",i);
	printf("\nFree\t");
	for(int i=0;i<maxr;i++)
		printf("%d  ",available[i]);

	//Currently allocated resources
	printf("\n\n\n\tProcesses (currently allocated resources):\n\t");
	for(int i=0;i<maxr;i++)
		printf("R%d ",i);
	for(int i=0;i<maxn;i++)
	{
		printf("\nP%d\t",i);
		for(int j=0;j<maxr;j++)
			printf("%d  ",allocated[i][j]);
		if(state[i]==-1)
		printf("--Terminated");	
	}

	//Currently allocated resources
	printf("\n\n\n\tProcesses (maximum resources):\n\t");
	for(int i=0;i<maxr;i++)
		printf("R%d ",i);
	for(int i=0;i<maxn;i++)
	{
		printf("\nP%d\t",i);
		for(int j=0;j<maxr;j++)
			printf("%d  ",max[i][j]);	
		if(state[i]==-1)
		 	printf("--Terminated");
	}

	//Resources needed to complete execution by each process
	printf("\n\n\n\tNeed (maximum resources - currently allocated resources):\n\t");
	for(int i=0;i<maxr;i++)
		printf("R%d ",i);
	for(int i=0;i<maxn;i++)
	{
		printf("\nP%d\t",i);
		for(int j=0;j<maxr;j++)
			printf("%d  ",need[i][j]);	
		if(state[i]==-1)
		 	printf("--Terminated");
	}
}

//For Showing status.
void show_status()
{
	system("clear");
	//Total system resources
	printf("\tTotal system resources:\n\t");
	for(int i=0;i<maxr;i++)
		printf("R%d ",i);
	printf("\nFree\t");
	for(int i=0;i<maxr;i++)
		printf("%d  ",total_resources[i]);

	status_module();

	//Available system resources
		printf("\nPress enter to go back......");
	return;
}