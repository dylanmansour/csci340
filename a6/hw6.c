#include <stdio.h>
#include "dpsim.h"

/**************************************************

Main Function:
	int main( int argc, char** argv )

------------------------------------------------
In this function perform the following steps:
------------------------------------------------
1. Create the following variables:
	- main_thread (pthread_t)
	- status (join status value)
2. Create a main_thread 
	- If the return value != 0, then display an error message and 
	  immediately exit program with status value 1.
3. Join the main_thread
	- If the return value != 0, then display an error message and
	  immediately exit the program with status value 2.
4. Display join status value.
5. Exit program.

*/

int main( int argc, char** argv ) {


	// ---------------------------------------
	// TODO: you add your implementation here
	
	pthread_t main_thread;
	int status, createResut;
	
	createResut = pthread_create(&main_thread, NULL, th_main, NULL);
	
	if(createResut != 0){
		printf("Failed to create the main thread in main.\n");
		exit(1);
	}
	
	status = pthread_join(main_thread, NULL);
	
	if(status != 0){
		printf("Failed to join main thread in main - %d.\n", status);
		exit(2);
	}
	
	return 0;

} // end main function