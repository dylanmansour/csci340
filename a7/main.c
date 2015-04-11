#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

/* minimum and maximum duration of use for an allocated block of memory */
#define MIN_DURATION      3
#define MAX_DURATION     25

/* minimum and maximum allocation request size */
#define MIN_REQUEST_SIZE    3
#define MAX_REQUEST_SIZE  100

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  number of times to repeat each experiment (ie. number of runs).  The
  third parameter is the duration of the each simulation run. The
  forth parameter is a random number seed. Here is an example command
  line:

  ./hw7 10000 100 2000 1234

  This means that your program should initialize physical memory to
  10,000 units, perform 100 runs with each run taking 2000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1234.
*/

int main(int argc, char** argv)
{
	
	//check number of args
	if(argc != 5){
		printf("Invalid input.\n");
		return 1;
	}
	
	int size_memory = atoi(argv[1]), 
		num_runs = atoi(argv[2]), 
		time_steps = atoi(argv[3]), 
		seed = atoi(argv[4]), 
		i,
		j,
		type_run,
		duration,
		size,
		result,
		avg_probe = 0, 
		fail_count = 0;
	
	mem_init(size_memory);
	
	srand(seed);
	
	
	for(type_run = 0; type_run < 1; type_run++){

		mem_clear();

		for(i = 0; i < num_runs; i++){

			for(j = 0; j < time_steps; j++){
			
				printf("time_step = %d\n", j);
				
				duration = (rand() % (MAX_DURATION - MIN_DURATION)) + MIN_DURATION;
				size = (rand() % (MAX_REQUEST_SIZE - MIN_REQUEST_SIZE)) + MIN_REQUEST_SIZE;
		
				switch(type_run){
					case 0: result = mem_allocate(FIRST, size, duration);
						break;
					case 1: result = mem_allocate(NEXT, size, duration);
						break;
					case 2: result = mem_allocate(BEST, size, duration);
						break;
				}
							
				if(result >= 0)
					avg_probe += result;
				else
					fail_count++;
			
				mem_single_time_unit_transpired();
			}
			
			
		}
	}
	
	mem_free();

	return 0;
}
