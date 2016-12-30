

#include	<stdio.h>
#include	<rt/rt.h>			// change the pathlist to rt.h to suit location on your disk
#include <string>
#include <vector>

int SchedulerToPipe;
struct 	    mydatapooldata		// start of structure template " the data pool"
{
	string Producer_line;		// Variable to hold message to Producer line
	string Red_line;			// Variable to hold message to Red line
	string Green_line;			// Variable to hold message to Green line
	string Blue_line;			// Variable to hold message to Blue line
};



int temp; 		//variable to hold the value read from the pipline
int counter = 0;//varible to hold number of processed items
int no = 10;//variable to hold the number of processed items per second intially set to 10 items
int t;//variable to hold the sleeping time



//////////
int processes, time_period = 0, id = 0;
float Avg_TA = 0, Avg_Wait = 0;
int quantum; // for RR
bool cont = true;
////////
struct PCBFCFS{
	int process_id, burst, wait, turn_around, arrival_time, surf_time, colour;
	PCBFCFS *nextFCFS;
}*nextFCFS = NULL, *rearFCFS = NULL;
void printVectorFCFS(const vector<PCBFCFS*> newMyVector)
{
	unsigned int size = newMyVector.size();
	int remainingTime;
	for (unsigned int i = 0; i < size; i++)
	{
		remainingTime = newMyVector[i]->surf_time;
		printf("\n Received object with ID: %d \tAT: %d \tBurst: %d \tWait: %d \tTA: %d \n", newMyVector[i]->process_id, newMyVector[i]->arrival_time, newMyVector[i]->surf_time, newMyVector[i]->wait, newMyVector[i]->turn_around);
		while (remainingTime - 6 >0){
			if (newMyVector[i]->surf_time > 2){
				//remainingTime = newMyVector[i]->surf_time;
				remainingTime = remainingTime - 6;
				printf("Object with ID: %d \t Remaining time %d \n", newMyVector[i]->process_id, remainingTime);
				Sleep(100);
			}
		}
		printf("Object with ID: %d is executed \n", newMyVector[i]->process_id);
	}
}


struct PCBFCFS d11_FCFS;
vector<PCBFCFS*> myVector_FCFS;

int	main()
{
	SchedulerToPipe = 1;
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n";
	cout << "		#	       Mohamed Ahmed b5014297           # \n";
	cout << "		# Multitasking and Scheduling Systems           # \n";
	cout << "		# Contol System for a Robotic Assembly Line     # \n";
	cout << "		#		  Line D			# \n";
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n \n";
	CPipe	pipe3("MyPipe3");		// Create a pipe to receive items
	//CPipe	pipe4("MyPipe4");		// Create a pipe to receive items

	CDataPool 		dp("IO", sizeof(struct mydatapooldata)); //create a data pool to recive commands from I/O process
	struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool());
	TEXT_COLOUR(12, 0);//change printing colour to red

	while (1)//start spinning loop
	{
		//check the data pool for a command and then execute that command 

		if (MyDataPool->Blue_line == "kill")
		{
			return 0;
		}
		if (MyDataPool->Blue_line == "speed")
		{
			no = no + 1;
			MyDataPool->Blue_line = "";
		}
		if (MyDataPool->Blue_line == "slow")
		{
			no = no - 1;
			MyDataPool->Blue_line = "";
		}

		if (pipe3.TestForData() != 0)//test the pipeline befor reading
		{
			//pipe4.Read(&SchedulerToPipe, sizeof(SchedulerToPipe));	// Read the structure from the pipeline
			//printf("-zzzzzzzz-----------%d------------hhhhhhhhhhhhhh-------\n", SchedulerToPipe);
			if (SchedulerToPipe == 1)
			{
				pipe3.Read(&d11_FCFS, sizeof(d11_FCFS));	// Read the structure from the pipeline
				//pipe1.Read(&SchedulerToPipe, sizeof(SchedulerToPipe));	// Read the structure from the pipeline
				//printf("-zzzzzzzz-----------%d------------hhhhhhhhhhhhhh-------\n", SchedulerToPipe);

				myVector_FCFS.push_back(&d11_FCFS);
				TEXT_COLOUR(9, 0);						//Change the printing colour to red
				printf("\n Id: %d \tAT: %d \tBurst: %d \tWait: %d \tTA: %d", d11_FCFS.process_id, d11_FCFS.arrival_time, d11_FCFS.surf_time, d11_FCFS.wait, d11_FCFS.turn_around);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)

				counter++;//increase the counter of the processed items
				printf("Processed Red Item NO.%d \n", counter); //print the item
				//printf("		Line Speed= %d", no); //print the speed of the line in object per secon
				//printf(" OPS\n");
				t = 1000 / no; //calculate sleeping time
				Sleep(t);	//sleep t time
			}
		}
	}


	return 0;//terminate the line process

}