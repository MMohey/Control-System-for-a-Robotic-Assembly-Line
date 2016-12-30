
#include <stdio.h>
#include<rt/rt.h>			// change the pathlist to rt.h to suit location on your disk
#include <iostream>
#include <string>



struct 	    mydatapooldata		// start of structure template " the data pool"
{
	string Producer_line;		// Variable to hold message to Producer line
	string Red_line;			// Variable to hold message to Red line
	string Green_line;			// Variable to hold message to Green line
	string Blue_line;			// Variable to hold message to Blue line
	string Yellow_line;		// Variable to hold message to Yellow line
};

int main()
{
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n";
	cout << "		#	       Mohamed Ahmed b5014297           # \n";
	cout << "		# Multitasking and Scheduling Systems           # \n";
	cout << "		# Contol System for a Robotic Assembly Line     # \n";
	cout << "		#		  control Panel			# \n";
	cout << "		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n \n";
	string cmd; //variable to hold the input comand
	CDataPool 		dp("IO", sizeof(struct mydatapooldata));	//create a data pool to send commands to other processes
	struct mydatapooldata 	 *MyDataPool = (struct mydatapooldata *)(dp.LinkDataPool());	//creat a pointer to the data pool

	//print the list of available commands
	cout << "List of commands:\n";
	cout << " WA to Increase speed of producer\n";
	cout << " QA to Decrease speed of producer\n";
	cout << " SA to Stop the producer\n";
	cout << " WB to Increase speed of Red line\n";
	cout << " QB to Decrease speed of Red\n";
	cout << " SB to Stop Red line\n";
	cout << " WC to Increase speed of Green line\n";
	cout << " QC to Decrease speed of Green\n";
	cout << " SC to Stop Green line\n";
	cout << " WD to Increase speed of Blue line\n";
	cout << " QD to Decrease speed of Blue\n";
	cout << " SD to Stop Blue line\n";
	cout << " WE to Increase speed of Yellow\n";
	cout << " QE to Decrease speed of Yellow\n";
	cout << " SE to Stop Yellow line\n";
	cout << " RS to Resume processing\n";
	cout << " XX to Halt the system\n";

	do //start spinng loop to read input drom the keyboard
	{
		cout << "Enter Your Command:\n";
		cin >> cmd;  //read the input command from the keyboard

		//validate the input command and then send it to the appropriate process
		do{

			if (cmd == "SA" || cmd == "sa" || cmd == "SB" || cmd == "sb" || cmd == "SC" || cmd == "sc" || cmd == "SD" || cmd == "sd" || cmd == "SE" || cmd == "se")
			{
				MyDataPool->Producer_line = "stop";
				break;
			}
			if (cmd == "WA" || cmd == "wa")
			{
				MyDataPool->Producer_line = "speed";
				break;
			}
			if (cmd == "QA" || cmd == "qa")
			{
				MyDataPool->Producer_line = "slow";
				break;
			}
			if (cmd == "WB" || cmd == "wb")
			{
				MyDataPool->Red_line = "speed";
				break;
			}
			if (cmd == "QB" || cmd == "qb")
			{
				MyDataPool->Red_line = "slow";
				break;
			}
			if (cmd == "WC" || cmd == "wc")
			{
				MyDataPool->Green_line = "speed";
				break;
			}
			if (cmd == "QC" || cmd == "qc")
			{
				MyDataPool->Green_line = "slow";
				break;
			}
			if (cmd == "WD" || cmd == "wd")
			{
				MyDataPool->Blue_line = "speed";
				break;
			}
			if (cmd == "QD" || cmd == "qd")
			{
				MyDataPool->Blue_line = "slow";
				break;
			}
			if (cmd == "WE" || cmd == "we")
			{
				MyDataPool->Yellow_line = "speed";
				break;
			}
			if (cmd == "QE" || cmd == "qe")
			{
				MyDataPool->Yellow_line = "slow";
				break;

			}
			if (cmd == "XX" || cmd == "xx")
			{
				MyDataPool->Producer_line = "kill";
				MyDataPool->Red_line = "kill";
				MyDataPool->Green_line = "kill";
				MyDataPool->Blue_line = "kill";
				MyDataPool->Yellow_line = "kill";
				return 0;
				break;
			}

			if (cmd == "RS" || cmd == "rs")
			{
				MyDataPool->Producer_line = "resume";
				break;
			}
			if (cmd != "SA" || cmd != "sa" || cmd != "SB" || cmd != "sb" || cmd != "SC" || cmd != "sc" ||
				cmd != "SD" || cmd != "sd" || cmd != "SE" || cmd != "se" || cmd != "WA" || cmd != "wa" ||
				cmd != "QB" || cmd != "qb" || cmd != "WC" || cmd != "wc" || cmd != "QC" || cmd != "qc" ||
				cmd != "WD" || cmd != "wd" || cmd != "QD" || cmd != "qd" || cmd != "WE" || cmd != "we" ||
				cmd != "QE" || cmd != "qe" || cmd != "XX" || cmd != "xx" || cmd != "RS" || cmd != "rs")
			{
				cout << "Wrong Command!! Try Again:\n";
				break;

			}
		} while (cmd != "SA" || cmd != "sa" || cmd != "SB" || cmd != "sb" || cmd != "SC" || cmd != "sc" ||
			cmd != "SD" || cmd != "sd" || cmd != "SE" || cmd != "se" || cmd != "WA" || cmd != "wa" ||
			cmd != "QB" || cmd != "qb" || cmd != "WC" || cmd != "wc" || cmd != "QC" || cmd != "qc" ||
			cmd != "WD" || cmd != "wd" || cmd != "QD" || cmd != "qd" || cmd != "WE" || cmd != "we" ||
			cmd != "QE" || cmd != "qe" || cmd != "XX" || cmd != "xx" || cmd != "RS" || cmd != "rs");

	} while (1);


	return 0;
}