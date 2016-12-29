//	A Win32 C program to demonstrate the creation and use of named Pipes. Error checking has 
//	been ommitted to aid clarity

#include <rt/rt.h>			// change the pathlist to rt.h to suit location on your disk
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <ctime> 
#include <dos.h>
#include <vector>
#include <string>
#include <conio.h>
#include <assert.h>

using namespace std;

#ifndef NDEBUG
#define SUBDIR "Debug"
#else
#define SUBDIR "Release"
#endif
typedef int queuetype;
const int maxsize = 10000000; //max size of the queue


//queue Ready_Queue;     //create a ready queue							
string Scheduler_type;//variable to hold the schedulig algorithm chosen by the user "enters by the user"
int nopitems;		//vaiable to hold how many items the produser will produce "enters by the user"
string Input_method; //vaiable to hold  the input metthod(either munually or automatically) "enters by the user"
int no = 100;			//variable to hold the number of produced items per second intially set to 100 item per second
float t;			//variable to hold the sleep time of producer thread
int complet_produce = 0; //variable set to 1 when the producer finish producing items


//////////
int processes, time_period = 0, id = 0;
float Avg_TA = 0, Avg_Wait = 0;
int quantum; // for RR
bool cont = true;
////////

int random(){
	int max = 100, min = 1;
	int range = (max - min) + 1;
	return (min + int(range*rand() / (RAND_MAX + 1.0)));
}



struct 	    mydatapooldata		// start of structure template " the data pool"
{
	string Producer_line;		// Variable to hold message to Producer line
	string Red_line;			// Variable to hold message to Red line
	string Green_line;			// Variable to hold message to Green line
	string Blue_line;			// Variable to hold message to Blue line
};



int randomFCFS(){
	int max = 100, min = 1;
	int range = (max - min) + 1;
	return (min + int(range*rand() / (RAND_MAX + 1.0)));
}


struct PCBFCFS{
	int process_id, burst, wait, turn_around, arrival_time, surf_time, colour;
	PCBFCFS *nextFCFS;
}*nextFCFS = NULL, *rearFCFS = NULL;

struct PCBFCFS d11_FCFS;
vector<PCBFCFS*> myVector_FCFS;

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

void enqueueFCFS(int val){
	if (nextFCFS == NULL){
		nextFCFS = new PCBFCFS;
		rearFCFS = nextFCFS;
	}
	else{
		rearFCFS->nextFCFS = new PCBFCFS;
		rearFCFS = rearFCFS->nextFCFS;
	}
	rearFCFS->nextFCFS = NULL;
	rearFCFS->process_id = ++id;
	rearFCFS->arrival_time = time_period;
	rearFCFS->burst = val;
	rearFCFS->surf_time = val;
	rearFCFS->colour = (1 + (rand() % 3));		//generate random item

}

void dequeueFCFS(){
	struct PCBFCFS *temp;
	temp = NULL;
	int empty = 0;

	if (nextFCFS->process_id == processes){
		cont = false;
	}
	if (nextFCFS == NULL){
		empty = 1;

	}
	else if (nextFCFS == rearFCFS){
		temp = nextFCFS;
		nextFCFS = rearFCFS = NULL;
	}
	else{
		temp = nextFCFS;
		nextFCFS = nextFCFS->nextFCFS;
	}
	if (empty != 1){
		if (temp == NULL)
		{
			return;
		}
		temp->wait = (time_period)-(temp->arrival_time) - (temp->surf_time);
		temp->turn_around = (temp->wait) + (temp->surf_time);
		myVector_FCFS.push_back(temp);

		/////////////
		Avg_TA = Avg_TA + (temp->turn_around);
		Avg_Wait = Avg_Wait + (temp->wait);

		if (temp->colour == 1)	//if the coulour of item is red
		{
			TEXT_COLOUR(12, 0);						//Change the printing colour to red
			printf("\n Id: %d \tAT: %d \tBurst: %d \tWait: %d \tTA: %d", temp->process_id, temp->arrival_time, temp->surf_time, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 2)	//if the coulour of item is red
		{
			TEXT_COLOUR(10, 0);						//Change the printing colour to red
			printf("\n Id: %d \tAT: %d \tBurst: %d \tWait: %d \tTA: %d", temp->process_id, temp->arrival_time, temp->surf_time, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 3)	//if the coulour of item is red
		{
			TEXT_COLOUR(9, 0);						//Change the printing colour to red
			printf("\n Id: %d \tAT: %d \tBurst: %d \tWait: %d \tTA: %d", temp->process_id, temp->arrival_time, temp->surf_time, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}
	}
}
void surfFCFS(){
	if (nextFCFS != NULL){
		if (nextFCFS->burst>0){
			nextFCFS->burst--;


		}
		else{
			dequeueFCFS();
			time_period--; // neglecting realistic situation
		}
	}
	time_period++;
}

void displayFCFS(){
	//myVector.

	printf("\n\nAverage Waiting Time is : %.3f\nAverage Turn Around Time is : %.3f ", Avg_Wait / id, Avg_TA / id);

	printf("\n -------------------------///////// \n");
	//printVectorFCFS(myVector_FCFS);
	//for (int i = 0; i < tempRRIncrementer; i++)
	//{
	//	//	printf("\nid: %d \tAT: %d \tBurst: %d \tWait: %d \tTA: %d", tempRR[i]->process_id, tempRR[i]->arrival_time, tempRR[i]->surf_time, tempRR[i]->wait, tempRR[i]->turn_around);

	//}
}

/////////////////////// Priority \\\\\\\\\\\\\\\\\\\\\\\\


//--------------random number---------
int randomPriority(int min, int max){
	//int max=4,min=1;
	int range = (max - min) + 1;
	return (min + int(range*rand() / (RAND_MAX + 1.0)));
}

//--------------PCB---------
struct PCBPriority{
	int process_id, burst, wait, turn_around, arrival_time, surf_time, colour;
	int priority;
	PCBPriority *nextPriority;
}*frontPriority = NULL, *rearPriority = NULL;
struct PCBPriority d11Priority;
vector<PCBPriority*> myVectorPriority;

//--------------Enqueue---------
void enqueuePriority(int val){
	if (frontPriority == NULL){
		frontPriority = new PCBPriority;
		rearPriority = frontPriority;
	}
	else{
		rearPriority->nextPriority = new PCBPriority;
		rearPriority = rearPriority->nextPriority;
	}
	rearPriority->nextPriority = NULL;
	rearPriority->process_id = ++id;
	rearPriority->arrival_time = time_period;
	rearPriority->burst = val;
	rearPriority->surf_time = val;
	rearPriority->priority = randomPriority(1, 10);
	rearPriority->colour = (1 + (rand() % 3));		//generate random item
	printf("\nProcess %d created, has AT: %d and Burst : %d priority : %d\n", rearPriority->process_id, rearPriority->arrival_time, rearPriority->surf_time, rearPriority->priority);
}
//--------------Dequeue---------
void dequeuePriority(){
	struct PCBPriority *temp;
	temp = NULL;
	int empty = 0;
	if (frontPriority == NULL){
		empty = 1;

	}
	else if (frontPriority->nextPriority == NULL){ //'front==rear' didnt suitable in this case
		temp = frontPriority;
		frontPriority = rearPriority = NULL;
	}
	else{
		temp = frontPriority;
		frontPriority = frontPriority->nextPriority;
	}
	if (empty != 1){
		if (temp == NULL){
			return;
		}
		temp->wait = (time_period)-(temp->arrival_time) - (temp->surf_time);
		temp->turn_around = (temp->wait) + (temp->surf_time);

		myVectorPriority.push_back(temp);

		Avg_TA = Avg_TA + (temp->turn_around);
		Avg_Wait = Avg_Wait + (temp->wait);
		if (temp->colour == 1)	//if the coulour of item is red
		{
			TEXT_COLOUR(12, 0);						//Change the printing colour to red
			printf("\nProcess %d executed completely, its Wait: %d and TA: %d \n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 2)	//if the coulour of item is red
		{
			TEXT_COLOUR(10, 0);						//Change the printing colour to red
			printf("\nProcess %d executed completely, its Wait: %d and TA: %d \n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 3)	//if the coulour of item is red
		{
			TEXT_COLOUR(9, 0);						//Change the printing colour to red
			printf("\nProcess %d executed completely, its Wait: %d and TA: %d \n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}
	}
}
//--------------Place---------
//Place process of higher priority at frontPriorityof queue

void placePriority(){
	struct PCBPriority *curr, *prev, *low, *back_low;
	if (frontPriority != NULL){
		back_low = frontPriority;
		prev = frontPriority;
		curr = frontPriority;
		low = frontPriority;

		do{
			if (curr->priority < low->priority){ //lower the value of higher the priority based
				low = curr;
				back_low = prev;
			}
			prev = curr;
			curr = curr->nextPriority;
		} while (curr != NULL);

		if (low != frontPriority){// place in frontPriorityif it is not in front
			back_low->nextPriority = low->nextPriority;
			low->nextPriority = frontPriority;
			frontPriority = low;
			if (low == rearPriority) rearPriority = back_low;
		}

	}
}

//-------------- display--------------
void displayPriority(){
	printf("\n\t\t\tDisplay Queue \n \n");
	PCBPriority *cursor = frontPriority;
	if (frontPriority == NULL) printf("\nError : Queue is empty.");
	else {
		do{
			if (cursor->colour == 1)	//if the coulour of item is red
			{
				TEXT_COLOUR(12, 0);						//Change the printing colour to red
				printf("\nId : %d \t Priority: %d \t burst :  %d \t Add : %d \t nextPriority:\t %d", cursor->process_id,  cursor->priority, cursor->burst, cursor, cursor->nextPriority);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}

			if (cursor->colour == 2)	//if the coulour of item is red
			{
				TEXT_COLOUR(10, 0);						//Change the printing colour to red
				printf("\nId : %d \t Priority: %d \t burst :  %d \t Add : %d \t nextPriority:\t %d", cursor->process_id,  cursor->priority, cursor->burst, cursor, cursor->nextPriority);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}

			if (cursor->colour == 3)	//if the coulour of item is red
			{
				TEXT_COLOUR(9, 0);						//Change the printing colour to red
				printf("\nId : %d \t Priority: %d \t burst :  %d \t Add : %d \t nextPriority:\t %d", cursor->process_id,  cursor->priority, cursor->burst, cursor, cursor->nextPriority);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			cursor = cursor->nextPriority;
		} while (cursor != NULL);

	}
}

//--------------Execution---------
void surfPriority(){

	if (frontPriority != NULL){

		if (frontPriority->burst>0){
			placePriority();// place in frontPriorityof queue
			frontPriority->burst--;
		}
		else{
			dequeuePriority();
			if (frontPriority == NULL && processes == id){
				cont = false;
			}
			time_period--; // neglecting realistic situation
		}
		displayPriority();
	}
	time_period++;

}
//--------------Display AWT and TA---------
void display_chartPriority(){
	printf("\n\n\nAverage Waiting Time is \t: \t %.3f\nAverage Turn Around Time is \t: \t %.3f ", Avg_Wait / id, Avg_TA / id);
}

/////////////////////// Shortest Job Non Preemptive \\\\\\\\\\\\\\\\\\\\\\\\

//--------------random number---------
int randomSJFNonPre(){
	int max = 4, min = 1;
	int range = (max - min) + 1;
	return (min + int(range*rand() / (RAND_MAX + 1.0)));
}

////--------------PCB---------
struct PCBSJFNonPre{
	int process_id, burst, wait, turn_around, arrival_time, surf_time, colour;
	PCBSJFNonPre *nextSJFNonPre;
}*SJFNonPre = NULL, *rearSJFNonPre = NULL;
struct PCBSJFNonPre d11_SJFNonPre;
vector<PCBSJFNonPre*> myVector_SJFNonPre;


//--------------Enqueue---------
void enqueueSJFNonPre(int val){
	if (SJFNonPre == NULL){
		SJFNonPre = new PCBSJFNonPre;
		rearSJFNonPre = SJFNonPre;
	}
	else{
		rearSJFNonPre->nextSJFNonPre = new PCBSJFNonPre;
		rearSJFNonPre = rearSJFNonPre->nextSJFNonPre;
	}
	rearSJFNonPre->nextSJFNonPre = NULL;
	rearSJFNonPre->process_id = ++id;
	rearSJFNonPre->arrival_time = time_period;
	rearSJFNonPre->burst = val;
	rearSJFNonPre->surf_time = val;
	rearSJFNonPre->colour = (1 + (rand() % 3));		//generate random item
	printf("\n\nProcess %d created, has AT: %d and Burst : %d\n", rearSJFNonPre->process_id, rearSJFNonPre->arrival_time, rearSJFNonPre->surf_time);
}
//--------------Dequeue---------
void dequeueSJFNonPre(){
	struct PCBSJFNonPre *temp;
	temp = NULL;
	int empty = 0;
	if (SJFNonPre == NULL){
		empty = 1;

	}
	else if (SJFNonPre->nextSJFNonPre == NULL){ //'SJFNonPre==rearSJFNonPre' didnt suitable in this case
		temp = SJFNonPre;
		SJFNonPre = rearSJFNonPre = NULL;
	}
	else{
		temp = SJFNonPre;
		SJFNonPre = SJFNonPre->nextSJFNonPre;
	}
	if (empty != 1){
		if (temp == NULL){
			return;
		}
		temp->wait = (time_period)-(temp->arrival_time) - (temp->surf_time);
		temp->turn_around = (temp->wait) + (temp->surf_time);
		myVector_SJFNonPre.push_back(temp);

		Avg_TA = Avg_TA + (temp->turn_around);
		Avg_Wait = Avg_Wait + (temp->wait);
		if (temp->colour == 1)	//if the coulour of item is red
		{
			TEXT_COLOUR(12, 0);						//Change the printing colour to red
			printf("\n\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 2)	//if the coulour of item is red
		{
			TEXT_COLOUR(10, 0);						//Change the printing colour to red
			printf("\n\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 3)	//if the coulour of item is red
		{
			TEXT_COLOUR(9, 0);						//Change the printing colour to red
			printf("\n\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}
	}
}
//--------------Place---------
//Place process of lowest burst at SJFNonPre of queue
void placeSJFNonPre(){
	struct PCBSJFNonPre *curr, *prev, *low, *back_low;
	if (SJFNonPre != NULL){
		back_low = SJFNonPre;
		prev = SJFNonPre;
		curr = SJFNonPre;
		low = SJFNonPre;

		do{
			if (curr->colour == 1)	//if the coulour of item is red
			{
				TEXT_COLOUR(12, 0);						//Change the printing colour to red
				printf("\nid :  %d  burst : %d ", curr->process_id, curr->burst);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}

			if (curr->colour == 2)	//if the coulour of item is red
			{
				TEXT_COLOUR(10, 0);						//Change the printing colour to red
				printf("\nid :  %d  burst : %d ", curr->process_id, curr->burst);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}

			if (curr->colour == 3)	//if the coulour of item is red
			{
				TEXT_COLOUR(9, 0);						//Change the printing colour to red
				printf("\nid :  %d  burst : %d ", curr->process_id, curr->burst);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			if (curr->burst < low->burst){
				low = curr;
				back_low = prev;
			}
			prev = curr;
			curr = curr->nextSJFNonPre;
		} while (curr != NULL);
		printf("\n\nProcess %d has lowest burst (%d) : ",low->process_id,low->burst);

		if (low != SJFNonPre){// place in SJFNonPre if it is not in SJFNonPre
			back_low->nextSJFNonPre = low->nextSJFNonPre;
			low->nextSJFNonPre = SJFNonPre;
			SJFNonPre = low;
		}

	}
}
//--------------Execution---------
void surfSJFNonPre(){
	if (SJFNonPre == NULL && processes == id){
		cont = false;
	}
	if (SJFNonPre != NULL){
		if (SJFNonPre->burst>0){
			SJFNonPre->burst--;


		}
		else{
			dequeueSJFNonPre();
			placeSJFNonPre();
			time_period--; // neglecting realistic situation
		}
	}
	time_period++;
}
//--------------Display AWT and TA---------
void display_chartSJFNonPre(){
	printf("\n\n\nAverage Waiting Time is \t: \t %.3f\nAverage Turn Around Time is \t: \t %.3f ", Avg_Wait / id, Avg_TA / id);
}

/////////////////////// Shortest Job Preemptive \\\\\\\\\\\\\\\\\\\\\\\\

//--------------random number---------
int randomSJFPre(){
	int max = 4, min = 1;
	int range = (max - min) + 1;
	return (min + int(range*rand() / (RAND_MAX + 1.0)));
}

//--------------PCB---------
struct PCBSJFPre{
	int process_id, burst, wait, turn_around, arrival_time, surf_time, colour;
	PCBSJFPre *nextSJFPre;
}*frontSJFPre = NULL, *rearSJFPre = NULL;
struct PCBSJFPre d11_SJFPre;
vector<PCBSJFPre*> myVector_SJFPre;
//--------------Enqueue---------
void enqueueSJFPre(int val){
	if (frontSJFPre == NULL){
		frontSJFPre = new PCBSJFPre;
		rearSJFPre = frontSJFPre;
	}
	else{
		rearSJFPre->nextSJFPre = new PCBSJFPre;
		rearSJFPre = rearSJFPre->nextSJFPre;
	}
	rearSJFPre->nextSJFPre = NULL;
	rearSJFPre->process_id = ++id;
	rearSJFPre->arrival_time = time_period;
	rearSJFPre->burst = val;
	rearSJFPre->surf_time = val;
	rearSJFPre->colour = (1 + (rand() % 3));		//generate random item
	printf("\n\nProcess %d created, has AT: %d and Burst : %d\n", rearSJFPre->process_id, rearSJFPre->arrival_time, rearSJFPre->surf_time);
}
//--------------Dequeue---------
void dequeueSJFPre(){
	struct PCBSJFPre *temp;
	temp = NULL;
	int empty = 0;

	if (frontSJFPre->nextSJFPre == NULL){
		cont = false;
	}
	if (frontSJFPre == NULL){
		empty = 1;

	}
	else if (frontSJFPre->nextSJFPre == NULL){ //'frontSJFPre==rearSJFPre' didnt suitable in this case
		temp = frontSJFPre;
		frontSJFPre = rearSJFPre = NULL;
	}
	else{
		temp = frontSJFPre;
		frontSJFPre = frontSJFPre->nextSJFPre;
	}
	if (empty != 1){
		if (temp == NULL){
			return;
		}
		temp->wait = (time_period)-(temp->arrival_time) - (temp->surf_time);
		temp->turn_around = (temp->wait) + (temp->surf_time);

		myVector_SJFPre.push_back(temp);

		Avg_TA = Avg_TA + (temp->turn_around);
		Avg_Wait = Avg_Wait + (temp->wait);
		if (temp->colour == 1)	//if the coulour of item is red
		{
			TEXT_COLOUR(12, 0);						//Change the printing colour to red
			printf("\n\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 2)	//if the coulour of item is red
		{
			TEXT_COLOUR(10, 0);						//Change the printing colour to red
			printf("\n\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 3)	//if the coulour of item is red
		{
			TEXT_COLOUR(9, 0);						//Change the printing colour to red
			printf("\n\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}
	}
}
//--------------Place---------
//Place process of lowest burst at frontSJFPre of queue
void placeSJFPre(){
	struct PCBSJFPre *curr, *prev, *low, *back_low;
	if (frontSJFPre != NULL){
		back_low = frontSJFPre;
		prev = frontSJFPre;
		curr = frontSJFPre;
		low = frontSJFPre;

		do{

			if (curr->colour == 1)	//if the coulour of item is red
			{
				TEXT_COLOUR(12, 0);						//Change the printing colour to red
				printf("\nid :  %d  burst : %d ", curr->process_id, curr->burst);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			if (curr->colour == 2)	//if the coulour of item is red
			{
				TEXT_COLOUR(10, 0);						//Change the printing colour to red
				printf("\nid :  %d  burst : %d ", curr->process_id, curr->burst);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			if (curr->colour == 3)	//if the coulour of item is red
			{
				TEXT_COLOUR(9, 0);						//Change the printing colour to red
				printf("\nid :  %d  burst : %d ", curr->process_id, curr->burst);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}

			if (curr->burst < low->burst){
				low = curr;
				back_low = prev;
			}
			prev = curr;
			curr = curr->nextSJFPre;
		} while (curr != NULL);
		printf("\n\nProcess %d has lowest burst (%d) : ",low->process_id,low->burst);

		if (low != frontSJFPre){// place in frontSJFPre if it is not in frontSJFPre
			back_low->nextSJFPre = low->nextSJFPre;
			low->nextSJFPre = frontSJFPre;
			frontSJFPre = low;
		}

	}
}

//-------------- display--------------
void displaySJFPre(){
	printf("\n\t\t\tDisplay Queue\n\n");
	PCBSJFPre *cursor = frontSJFPre;
	if (frontSJFPre == NULL) printf("\nError : Queue is empty.");
	else {
		do{
			if (cursor->colour == 1)	//if the coulour of item is red
			{
				TEXT_COLOUR(12, 0);						//Change the printing colour to red
				printf("\nId : \t %d \t burst :  %d \t Add : %d \t nextSJFPre :\t %d", cursor->process_id, cursor->burst, cursor, cursor->nextSJFPre);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			if (cursor->colour == 2)	//if the coulour of item is red
			{
				TEXT_COLOUR(10, 0);						//Change the printing colour to red
				printf("\nId : \t %d \t burst :  %d \t Add : %d \t nextSJFPre :\t %d", cursor->process_id, cursor->burst, cursor, cursor->nextSJFPre);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			if (cursor->colour == 3)	//if the coulour of item is red
			{
				TEXT_COLOUR(9, 0);						//Change the printing colour to red
				printf("\nId : \t %d \t burst :  %d \t Add : %d \t nextSJFPre :\t %d", cursor->process_id, cursor->burst, cursor, cursor->nextSJFPre);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			cursor = cursor->nextSJFPre;
		} while (cursor != NULL);

	}
}

//--------------Execution---------
void surfSJFPre(){
	if (frontSJFPre != NULL){
		displaySJFPre();
		placeSJFPre();
		displaySJFPre();
		if (frontSJFPre->burst>0){
			frontSJFPre->burst--;
		}
		else{
			dequeueSJFPre();
			time_period--; // neglecting realistic situation
		}
	}
	time_period++;

}
//--------------Display AWT and TA---------
void display_chartSJFPre(){
	printf("\n\n\nAverage Waiting Time is \t: \t %.3f\nAverage Turn Around Time is \t: \t %.3f ", Avg_Wait / id, Avg_TA / id);
}

/////////////////////// Round Robin \\\\\\\\\\\\\\\\\\\\\\\\

//--------------random number---------
int random_RR(int min, int max){
	//int max=4,min=1;
	int range = (max - min) + 1;
	return (min + int(range*rand() / (RAND_MAX + 1.0)));
}

//--------------PCB---------
struct PCB_RR{
	int process_id, burst, wait, turn_around, arrival_time, surf_time, colour;
	int priority;
	PCB_RR *next_RR;
}*front_RR = NULL, *rear_RR = NULL;
struct PCB_RR d11_RR;
vector<PCB_RR*> myVector_RR;
//--------------Enqueue---------
void enqueue_RR(int val){
	if (front_RR == NULL){
		front_RR = new PCB_RR;
		rear_RR = front_RR;
	}
	else{
		rear_RR->next_RR = new PCB_RR;
		rear_RR = rear_RR->next_RR;
	}
	rear_RR->next_RR = NULL;
	rear_RR->process_id = ++id;
	rear_RR->arrival_time = time_period;
	rear_RR->burst = val;
	rear_RR->surf_time = val;
	rear_RR->priority = random_RR(1, 10);
	rear_RR->colour = (1 + (rand() % 3));		//generate random item

	printf("\nProcess %d created, has AT: %d and Burst : %d\n", rear_RR->process_id, rear_RR->arrival_time, rear_RR->surf_time);
}
//--------------Dequeue---------
void dequeue_RR(){
	struct PCB_RR *temp;
	temp = NULL;
	int empty = 0;
	if (front_RR == NULL){
		empty = 1;

	}
	else if (front_RR->next_RR == NULL){ //'front_RR==rear_RR' didnt suitable in this case
		temp = front_RR;
		front_RR = rear_RR = NULL;
	}
	else{
		temp = front_RR;
		front_RR = front_RR->next_RR;
	}
	if (empty != 1){
		if (temp == NULL){
			return;
		}
		temp->wait = (time_period)-(temp->arrival_time) - (temp->surf_time);
		temp->turn_around = (temp->wait) + (temp->surf_time);

		myVector_RR.push_back(temp); // HELLLOOOOOOOOOOOOOOOOOOOOOOO

		Avg_TA = Avg_TA + (temp->turn_around);
		Avg_Wait = Avg_Wait + (temp->wait);
		if (temp->colour == 1)	//if the coulour of item is red
		{
			TEXT_COLOUR(12, 0);						//Change the printing colour to red
			printf("\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 2)	//if the coulour of item is red
		{
			TEXT_COLOUR(10, 0);						//Change the printing colour to red
			printf("\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}

		if (temp->colour == 3)	//if the coulour of item is red
		{
			TEXT_COLOUR(9, 0);						//Change the printing colour to red
			printf("\nProcess %d executed completely, its Wait: %d and TA: %d\n", temp->process_id, temp->wait, temp->turn_around);
			TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
		}
	}
}
//--------------Place---------
//Place process of higher priority at front_RR of queue

void place_RR(){
	struct PCB_RR *curr, *prev, *low, *back_low;
	if (front_RR != NULL){
		back_low = front_RR;
		prev = front_RR;
		curr = front_RR;
		low = front_RR;

		do{
			if (curr->burst< low->burst){ //lower the value of higher the priority based
				low = curr;
				back_low = prev;
			}
			prev = curr;
			curr = curr->next_RR;
		} while (curr != NULL);

		if (low != front_RR){// place in front_RR if it is not in front_RR
			back_low->next_RR = low->next_RR;
			low->next_RR = front_RR;
			front_RR = low;
			if (low == rear_RR) rear_RR = back_low;
		}

	}
}

//-------------- display--------------
void display_RR(){
	printf("\n\t\t\tDisplay Queue");
	PCB_RR *cursor = front_RR;
	if (front_RR == NULL) printf("\nError : Queue is empty.");
	else {
		do{
			if (cursor->colour == 1)	//if the coulour of item is red
			{
				TEXT_COLOUR(12, 0);						//Change the printing colour to red
				printf("\nId : \t %d \t burst :  %d \t Add : %d \t next_RR :\t %d", cursor->process_id, cursor->burst, cursor, cursor->next_RR);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}

			if (cursor->colour == 2)	//if the coulour of item is red
			{
				TEXT_COLOUR(10, 0);						//Change the printing colour to red
				printf("\nId : \t %d \t burst :  %d \t Add : %d \t next_RR :\t %d", cursor->process_id, cursor->burst, cursor, cursor->next_RR);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}

			if (cursor->colour == 3)	//if the coulour of item is red
			{
				TEXT_COLOUR(9, 0);						//Change the printing colour to red
				printf("\nId : \t %d \t burst :  %d \t Add : %d \t next_RR :\t %d", cursor->process_id, cursor->burst, cursor, cursor->next_RR);
				TEXT_COLOUR(15, 0);						//return the to the default printing colour(whight colour)
			}
			cursor = cursor->next_RR;
		} while (cursor != NULL);

	}
}

//--------------Execution---------
void surf_RR(){
	PCB_RR *cursor = front_RR;
	if (front_RR == NULL){
		printf("\nError : Queue is empty.");
		if (front_RR == NULL && processes == id){
			cont = false;
		}
	}
	else {
		do{
			for (int i = 0; i<quantum; i++){
				if (cursor->burst > 0) cursor->burst--;
				else{
					place_RR();
					dequeue_RR();
				}
				time_period++;
			}
			display_RR();
			cursor = cursor->next_RR;
		} while (cursor != NULL);

	}


}
//--------------Display AWT and TA---------
void display_chart_RR(){
	printf("\n\n\nAverage Waiting Time is \t: \t %.3f\nAverage Turn Around Time is \t: \t %.3f ", Avg_Wait / id, Avg_TA / id);
}


int	main()									//the main robotic thread 
{
	int ditem;			//variable to hold the read item from the queue
	char endOfProgram;
	int objectNum;
	//Choose the method for produsin items. Type 1 for manually or 2 for automatically
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n";
	cout << "		#	       Mohamed Ahmed b5014297           # \n";
	cout << "		# Multitasking and Scheduling Systems           # \n";
	cout << "		# Contol System for a Robotic Assembly Line     # \n";
	cout << "		#		  Line A			# \n";
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n \n";

	cout << "Please enter your preferred production method (1 or 2)\n";	//Choose the method for produsin items
	cout << " 1 for Manual Production\n";
	cout << " 2 for Automatic Production\n";

	
	//Cheeck user selection either 1 or 2 otherwise asking him to re-try
	do{
		cin >> Input_method;					//save user selection in the variable "Scheduler_type
		if (Input_method == "1" || Input_method == "2"){
			break;
		}
		else
			cout << "Wrong Entry!! try again:\n";
	} while (Input_method != "1" || Input_method != "2");
	if (Input_method == "1"){
	cout << "type the code of the item to produce  (Red=1  Green=2   Blue=3  ):\n";
	//Validate the item code entered by the user
	do{
		cin >> objectNum;					//save user selection in the variable "Scheduler_type
		if (objectNum == 1 || objectNum == 2 || objectNum == 3)
			break;
		else
			cout << "Wrong Entry!! try again:\n";
	} while (objectNum != 1 || objectNum != 2 || objectNum != 3);
	}
	else
	{


		do{
			cout << "\n Please choose one of the following Scheduling algorithm: \n \n";	//print a message" coose Scheduling algorithm
			cout << "Type 1 for Fitst Come First Served\n";
			cout << "Type 2 for Priorty Base Scheduling \n";
			cout << "Type 3 for Shortest Job First Non Preemptive \n";
			cout << "Type 4 for Shortest Job First Preemptive \n";
			cout << "Type 5 for Round Robin \n";
			do{
				cin >> Scheduler_type;					//save user selection in the variable "Scheduler_type
				if (Scheduler_type == "1" || Scheduler_type == "2" || Scheduler_type == "3" || Scheduler_type == "4" || Scheduler_type == "5")
					break;
				else
					cout << "Wrong Entry!! try again:\n";
			} while (Scheduler_type != "1" || Scheduler_type != "2" || Scheduler_type != "3" || Scheduler_type != "4" || Scheduler_type != "5");


			if (Scheduler_type == "1") // FCFS
			{
				int SchedulerToPipe = 1;
				srand((unsigned)time(0));
				char end;
				int i;
				//do{
				printf("\nEnter number of objects you want to create : ");
				cin >> processes;

				//reseting if user run program again
				id = time_period = 0;
				cont = true;

				while (cont){ // continue the program until given number of processes excute completely.
					int val = randomFCFS();
					if (val % 2 == 0 && val < 100 && id < processes){// create randomly with burst less than 10 and create processes how the user wants
						enqueueFCFS(val);
					}
					surfFCFS();
				}

				displayFCFS();

				CPipe	pipe1("MyPipe1");						// Create a pipe 'pipe1' with the name "MyPipe1"
				CPipe	pipe2("MyPipe2");						// Create a pipe 'pipe2' with the name "MyPipe2"
				CPipe	pipe3("MyPipe3");						// Create a pipe 'pipe3' with the name "MyPipe3"
				//CPipe	pipe4("MyPipe4");						// Create a pipe 'pipe3' with the name "MyPipe3"

				CProcess p1("Q7Child\\"SUBDIR"\\Q7Child.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p2("Q7Child2\\"SUBDIR"\\Q7Child2.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p3("Q7Child3\\"SUBDIR"\\Q7Child3.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				//CProcess p4("Q7Child4\\"SUBDIR"\\Q7Child4.exe",	// pathlist to child program executable				
				//		NORMAL_PRIORITY_CLASS,			// priority
				//		OWN_WINDOW,						// process has its own window					
				//		ACTIVE							// process is active immediately
				//) ;
				CProcess p5("Q7ChildIO\\"SUBDIR"\\Q7ChildIO.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);


				CDataPool 		dp("IO", sizeof(struct mydatapooldata));
				struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool());

				for (int j = 0; j < myVector_FCFS.size(); j++)
					//while (1)// start spinning loop to read from the ready queue
				{
					if (MyDataPool->Producer_line == "stop")//check if the command from the data pool is stop
					{
						p1.Suspend();
						p2.Suspend();
						p3.Suspend();
						//p4.Suspend();
						if (complet_produce != 1)	//to avoid suspend the producer thread when it already terminated
						{
							//					c1.Suspend();
						}

						while (1)
						{
							if (MyDataPool->Producer_line == "resume")//check if the command from the data pool is resume
							{
								MyDataPool->Producer_line = "";
								p1.Resume();
								p2.Resume();
								p3.Resume();
								break;
							}
							if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
							{
								p1.Resume();
								p2.Resume();
								p3.Resume();
								//p4.Resume();
								MyDataPool->Red_line = "kill";
								MyDataPool->Green_line = "kill";
								MyDataPool->Blue_line = "kill";
								return 0;
							}
						}
					}

					if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
					{
						return 0;
					}
					if (MyDataPool->Producer_line == "speed")//check if the command from the data pool is speed
					{
						no = no + 1;//increase the number of the produced items
						MyDataPool->Producer_line = "";
					}
					if (MyDataPool->Producer_line == "slow")//check if the command from the data pool is slow
					{
						no = no - 1;//decrease the number of the produced items
						MyDataPool->Producer_line = "";
					}

					if (myVector_FCFS[j]->colour == 1)									//if the coulour of item is red
					{
						d11_FCFS = *myVector_FCFS[j];
						pipe1.Write(&d11_FCFS, sizeof(d11_FCFS));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						//pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_FCFS[j]->colour == 2)									//if the coulour of item is red
					{
						d11_FCFS = *myVector_FCFS[j];
						pipe2.Write(&d11_FCFS, sizeof(d11_FCFS));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						//pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_FCFS[j]->colour == 3)									//if the coulour of item is red
					{
						d11_FCFS = *myVector_FCFS[j];
						pipe3.Write(&d11_FCFS, sizeof(d11_FCFS));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						//pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}
					Sleep(50);											//wait some time (the dispatcher sleep time)
				}

				p1.WaitForProcess();			// wait for child Process to die
				p2.WaitForProcess();			// wait for child Process to die
				p3.WaitForProcess();			// wait for child Process to die
				//p4.WaitForProcess() ;			// wait for child Process to die
				p5.WaitForProcess();			// wait for child Process to die
			}

			if (Scheduler_type == "2") // FCFS
			{
				int SchedulerToPipe = 2;

				srand((unsigned)time(0));
				char end;
				int i;
				//do{
				printf("\nEnter number of objects you want to create : ");
				cin >> processes;
				if (processes < 1){
					printf("Can not create processes.");
				}
				else{
					//reseting if user run program again
					id = time_period = Avg_TA = Avg_Wait = 0;
					cont = true;

					for (i = 0; cont; i++){ //// continue the program until given number of processes excute completely
						int val = randomPriority(3, 7);
						if (val % 2 == 0 && val < 10 && id < processes){// create randomly with burst less than 10 and create processes how the user wants
							enqueuePriority(val);

						}
						surfPriority();
					}
					display_chartPriority();

				}

				CPipe	pipe1("MyPipe1");						// Create a pipe 'pipe1' with the name "MyPipe1"
				CPipe	pipe2("MyPipe2");						// Create a pipe 'pipe2' with the name "MyPipe2"
				CPipe	pipe3("MyPipe3");						// Create a pipe 'pipe3' with the name "MyPipe3"
				CPipe	pipe4("MyPipe4");						// Create a pipe 'pipe3' with the name "MyPipe3"

				//Creating five processes to represent the four line and the I/O process

				CProcess p1("Q7Child\\"SUBDIR"\\Q7Child.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p2("Q7Child2\\"SUBDIR"\\Q7Child2.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p3("Q7Child3\\"SUBDIR"\\Q7Child3.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p5("Q7ChildIO\\"SUBDIR"\\Q7ChildIO.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);


				CDataPool 		dp("IO", sizeof(struct mydatapooldata));
				struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool());

				for (int j = 0; j < myVectorPriority.size(); j++)
					//while (1)// start spinning loop to read from the ready queue
				{

					if (MyDataPool->Producer_line == "stop")//check if the command from the data pool is stop
					{
						p1.Suspend();
						p2.Suspend();
						p3.Suspend();

						while (1)
						{
							if (MyDataPool->Producer_line == "resume")//check if the command from the data pool is resume
							{
								MyDataPool->Producer_line = "";
								p1.Resume();
								p2.Resume();
								p3.Resume();
								break;
							}
							if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
							{
								p1.Resume();
								p2.Resume();
								p3.Resume();
								MyDataPool->Red_line = "kill";
								MyDataPool->Green_line = "kill";
								MyDataPool->Blue_line = "kill";
								return 0;
							}
						}
					}

					if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
					{
						return 0;
					}
					if (MyDataPool->Producer_line == "speed")//check if the command from the data pool is speed
					{
						no = no + 1;//increase the number of the produced items
						MyDataPool->Producer_line = "";
					}
					if (MyDataPool->Producer_line == "slow")//check if the command from the data pool is slow
					{
						no = no - 1;//decrease the number of the produced items
						MyDataPool->Producer_line = "";
					}

					if (myVectorPriority[j]->colour == 1)									//if the coulour of item is red
					{
						d11Priority = *myVectorPriority[j];
						pipe1.Write(&d11Priority, sizeof(d11Priority));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVectorPriority[j]->colour == 2)									//if the coulour of item is red
					{
						d11Priority = *myVectorPriority[j];
						pipe2.Write(&d11Priority, sizeof(d11Priority));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVectorPriority[j]->colour == 3)									//if the coulour of item is red
					{
						d11Priority = *myVectorPriority[j];
						pipe3.Write(&d11Priority, sizeof(d11Priority));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}
					Sleep(50);											//wait some time (the dispatcher sleep time)
				}

				p1.WaitForProcess();			// wait for child Process to die
				p2.WaitForProcess();			// wait for child Process to die
				p3.WaitForProcess();			// wait for child Process to die
				p5.WaitForProcess();			// wait for child Process to die
			}

			if (Scheduler_type == "3") // FCFS
			{
				int SchedulerToPipe = 3;
				srand((unsigned)time(0));
				char end;
				int i;
				//do{
				printf("\nEnter number of objects you want to create : ");
				cin >> processes;
				if (processes < 1){
					printf("Can not create processes.");
				}
				else{
					//reseting if user run program again
					id = time_period = Avg_TA = Avg_Wait = 0;
					cont = true;

					for (i = 0; cont; i++){ //// continue the program until given number of processes excute completely
						int val = randomSJFNonPre();
						if (val % 2 == 0 && val < 10 && id < processes){// create randomly with burst less than 10 and create processes how the user wants
							enqueueSJFNonPre(val);
						}
						surfSJFNonPre();
					}
					display_chartSJFNonPre();

				}

				CPipe	pipe1("MyPipe1");						// Create a pipe 'pipe1' with the name "MyPipe1"
				CPipe	pipe2("MyPipe2");						// Create a pipe 'pipe2' with the name "MyPipe2"
				CPipe	pipe3("MyPipe3");						// Create a pipe 'pipe3' with the name "MyPipe3"
				CPipe	pipe4("MyPipe4");						// Create a pipe 'pipe3' with the name "MyPipe3"

				CProcess p1("Q7Child\\"SUBDIR"\\Q7Child.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p2("Q7Child2\\"SUBDIR"\\Q7Child2.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p3("Q7Child3\\"SUBDIR"\\Q7Child3.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p5("Q7ChildIO\\"SUBDIR"\\Q7ChildIO.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);


				CDataPool 		dp("IO", sizeof(struct mydatapooldata));
				struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool());

				for (int j = 0; j < myVector_SJFNonPre.size(); j++)
					//while (1)// start spinning loop to read from the ready queue
				{
					if (MyDataPool->Producer_line == "stop")//check if the command from the data pool is stop
					{
						p1.Suspend();
						p2.Suspend();
						p3.Suspend();
						if (complet_produce != 1)	//to avoid suspend the producer thread when it already terminated
						{
							//					c1.Suspend();
						}

						while (1)
						{
							if (MyDataPool->Producer_line == "resume")//check if the command from the data pool is resume
							{
								MyDataPool->Producer_line = "";
								p1.Resume();
								p2.Resume();
								p3.Resume();
								break;
							}
							if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
							{
								p1.Resume();
								p2.Resume();
								p3.Resume();
								MyDataPool->Red_line = "kill";
								MyDataPool->Green_line = "kill";
								MyDataPool->Blue_line = "kill";
								return 0;
							}
						}
					}

					if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
					{
						return 0;
					}
					if (MyDataPool->Producer_line == "speed")//check if the command from the data pool is speed
					{
						no = no + 1;//increase the number of the produced items
						MyDataPool->Producer_line = "";
					}
					if (MyDataPool->Producer_line == "slow")//check if the command from the data pool is slow
					{
						no = no - 1;//decrease the number of the produced items
						MyDataPool->Producer_line = "";
					}


					if (myVector_SJFNonPre[j]->colour == 1)									//if the coulour of item is red
					{
						d11_SJFNonPre = *myVector_SJFNonPre[j];
						pipe1.Write(&d11_SJFNonPre, sizeof(d11_SJFNonPre));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_SJFNonPre[j]->colour == 2)									//if the coulour of item is red
					{
						d11_SJFNonPre = *myVector_SJFNonPre[j];
						pipe2.Write(&d11_SJFNonPre, sizeof(d11_SJFNonPre));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_SJFNonPre[j]->colour == 3)									//if the coulour of item is red
					{
						d11_SJFNonPre = *myVector_SJFNonPre[j];
						pipe3.Write(&d11_SJFNonPre, sizeof(d11_SJFNonPre));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}
					Sleep(50);											//wait some time (the dispatcher sleep time)
				}

				p1.WaitForProcess();			// wait for child Process to die
				p2.WaitForProcess();			// wait for child Process to die
				p3.WaitForProcess();			// wait for child Process to die
				//p4.WaitForProcess() ;			// wait for child Process to die
				p5.WaitForProcess();			// wait for child Process to die
			}

			if (Scheduler_type == "4") // FCFS
			{
				int SchedulerToPipe = 4;
				srand((unsigned)time(0));
				char end;
				int i;
				//do{
				printf("\nEnter number of objects you want to create : ");
				cin >> processes;
				if (processes < 1){
					printf("Can not create processes.");
				}
				else{
					//reseting if user run program again
					id = time_period = Avg_TA = Avg_Wait = 0;
					cont = true;

					for (i = 0; cont; i++){ //// continue the program until given number of processes excute completely
						int val = randomSJFPre();
						if (val % 2 == 0 && val < 10 && id < processes){// create randomly with burst less than 10 and create processes how the user wants
							enqueueSJFPre(val);
						}
						surfSJFPre();
					}
					display_chartSJFPre();

				}

				CPipe	pipe1("MyPipe1");						// Create a pipe 'pipe1' with the name "MyPipe1"
				CPipe	pipe2("MyPipe2");						// Create a pipe 'pipe2' with the name "MyPipe2"
				CPipe	pipe3("MyPipe3");						// Create a pipe 'pipe3' with the name "MyPipe3"
				CPipe	pipe4("MyPipe4");						// Create a pipe 'pipe3' with the name "MyPipe3"

				CProcess p1("Q7Child\\"SUBDIR"\\Q7Child.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p2("Q7Child2\\"SUBDIR"\\Q7Child2.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p3("Q7Child3\\"SUBDIR"\\Q7Child3.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p5("Q7ChildIO\\"SUBDIR"\\Q7ChildIO.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);


				CDataPool 		dp("IO", sizeof(struct mydatapooldata));
				struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool());

				for (int j = 0; j < myVector_SJFPre.size(); j++)
				{
					if (MyDataPool->Producer_line == "stop")//check if the command from the data pool is stop
					{
						p1.Suspend();
						p2.Suspend();
						p3.Suspend();
						if (complet_produce != 1)	//to avoid suspend the producer thread when it already terminated
						{
						}

						while (1)
						{
							if (MyDataPool->Producer_line == "resume")//check if the command from the data pool is resume
							{
								MyDataPool->Producer_line = "";
								p1.Resume();
								p2.Resume();
								p3.Resume();
								break;
							}
							if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
							{
								p1.Resume();
								p2.Resume();
								p3.Resume();
								MyDataPool->Red_line = "kill";
								MyDataPool->Green_line = "kill";
								MyDataPool->Blue_line = "kill";
								return 0;
							}
						}
					}

					if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
					{
						return 0;
					}
					if (MyDataPool->Producer_line == "speed")//check if the command from the data pool is speed
					{
						no = no + 1;//increase the number of the produced items
						MyDataPool->Producer_line = "";
					}
					if (MyDataPool->Producer_line == "slow")//check if the command from the data pool is slow
					{
						no = no - 1;//decrease the number of the produced items
						MyDataPool->Producer_line = "";
					}

					if (myVector_SJFPre[j]->colour == 1)									//if the coulour of item is red
					{
						d11_SJFPre = *myVector_SJFPre[j];
						pipe1.Write(&d11_SJFPre, sizeof(d11_SJFPre));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_SJFPre[j]->colour == 2)									//if the coulour of item is red
					{
						d11_SJFPre = *myVector_SJFPre[j];
						pipe2.Write(&d11_SJFPre, sizeof(d11_SJFPre));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_SJFPre[j]->colour == 3)									//if the coulour of item is red
					{
						d11_SJFPre = *myVector_SJFPre[j];
						pipe3.Write(&d11_SJFPre, sizeof(d11_SJFPre));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}
					Sleep(50);											//wait some time (the dispatcher sleep time)
				}

				p1.WaitForProcess();			// wait for child Process to die
				p2.WaitForProcess();			// wait for child Process to die
				p3.WaitForProcess();			// wait for child Process to die
				p5.WaitForProcess();			// wait for child Process to die
			}

			if (Scheduler_type == "5") // FCFS
			{
				int SchedulerToPipe = 5;
				srand((unsigned)time(0));
				char end;
				int i;
				//do{
				printf("\nEnter number of objects you want to create : ");
				cin >> processes;
				printf("\nEnter quantum : ");
				cin >> quantum;
				if (processes < 1){
					printf("Can not create processes.");
				}
				else{
					//reseting if user run program again
					id = time_period = Avg_TA = Avg_Wait = 0;
					cont = true;

					for (i = 0; cont; i++){ //// continue the program until given number of processes excute completely
						int val = random_RR(3, 7);
						if (val % 2 == 0 && val < 10 && id < processes){// create randomly with burst less than 10 and create processes how the user wants
							enqueue_RR(val);

						}
						surf_RR();
					}
					display_chart_RR();

				}

				CPipe	pipe1("MyPipe1");						// Create a pipe 'pipe1' with the name "MyPipe1"
				CPipe	pipe2("MyPipe2");						// Create a pipe 'pipe2' with the name "MyPipe2"
				CPipe	pipe3("MyPipe3");						// Create a pipe 'pipe3' with the name "MyPipe3"
				CPipe	pipe4("MyPipe4");						// Create a pipe 'pipe3' with the name "MyPipe3"

				CProcess p1("Q7Child\\"SUBDIR"\\Q7Child.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p2("Q7Child2\\"SUBDIR"\\Q7Child2.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p3("Q7Child3\\"SUBDIR"\\Q7Child3.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);
				CProcess p5("Q7ChildIO\\"SUBDIR"\\Q7ChildIO.exe",	// pathlist to child program executable				
					NORMAL_PRIORITY_CLASS,			// priority
					OWN_WINDOW,						// process has its own window					
					ACTIVE							// process is active immediately
					);


				CDataPool 		dp("IO", sizeof(struct mydatapooldata));
				struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool());

				for (int j = 0; j < myVector_RR.size(); j++)
					//while (1)// start spinning loop to read from the ready queue
				{
					if (MyDataPool->Producer_line == "stop")//check if the command from the data pool is stop
					{
						p1.Suspend();
						p2.Suspend();
						p3.Suspend();
						if (complet_produce != 1)	//to avoid suspend the producer thread when it already terminated
						{
						}

						while (1)
						{
							if (MyDataPool->Producer_line == "resume")//check if the command from the data pool is resume
							{
								MyDataPool->Producer_line = "";
								p1.Resume();
								p2.Resume();
								p3.Resume();
								break;
							}
							if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
							{
								p1.Resume();
								p2.Resume();
								p3.Resume();
								MyDataPool->Red_line = "kill";
								MyDataPool->Green_line = "kill";
								MyDataPool->Blue_line = "kill";
								return 0;
							}
						}
					}

					if (MyDataPool->Producer_line == "kill")//check if the command from the data pool is halt
					{
						return 0;
					}
					if (MyDataPool->Producer_line == "speed")//check if the command from the data pool is speed
					{
						no = no + 1;//increase the number of the produced items
						MyDataPool->Producer_line = "";
					}
					if (MyDataPool->Producer_line == "slow")//check if the command from the data pool is slow
					{
						no = no - 1;//decrease the number of the produced items
						MyDataPool->Producer_line = "";
					}

					if (myVector_RR[j]->colour == 1)									//if the coulour of item is red
					{
						d11_RR = *myVector_RR[j];
						pipe1.Write(&d11_RR, sizeof(d11_RR));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_RR[j]->colour == 2)									//if the coulour of item is red
					{
						d11_RR = *myVector_RR[j];
						pipe2.Write(&d11_RR, sizeof(d11_RR));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}

					if (myVector_RR[j]->colour == 3)									//if the coulour of item is red
					{
						d11_RR = *myVector_RR[j];
						pipe3.Write(&d11_RR, sizeof(d11_RR));			// write the structure to the pipeline, PRINTS TWICE THE ITEMS
						pipe4.Write(&SchedulerToPipe, sizeof(SchedulerToPipe));
					}
					Sleep(50);											//wait some time (the dispatcher sleep time)
				}

				p1.WaitForProcess();			// wait for child Process to die
				p2.WaitForProcess();			// wait for child Process to die
				p3.WaitForProcess();			// wait for child Process to die
				//p4.WaitForProcess() ;			// wait for child Process to die
				p5.WaitForProcess();			// wait for child Process to die
			}
			printf("\n\nDo you want to end program? Enter 'y' or 'n' :  \n");
			cin >> endOfProgram;
		} while (endOfProgram != 'y');
	}
	return 0;						//the program has executed successfully
}
