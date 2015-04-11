#include <stdlib.h>   // for malloc() and free()
#include <stdio.h>
#include "mem.h"

/*
  Physical memory array. This is a static global array for all functions in this file.  
  An element in the array with a value of zero represents a free unit of memory.
*/
static unsigned int* memory;

/*
 The size (i.e. number of units) of the physical memory array. This is a static global 
 variable used by functions in this file.  

 */
static unsigned int mem_size;


/*
 The last_placement_position variable contains the end position of the last 
 allocated unit used by the next fit placement algorithm.  
 */
static unsigned int last_placement_position;



int index_of_next_probe(int index){
	
	int i, not_zero = 0;
	
	//see if first index is a free chunk
	if(index == 0 && memory[index] == 0)
		return 0;
	
	for(i = index; i < mem_size; i++){
		if(not_zero && memory[i] == 0)
			return i;
		else if(memory[i] != 0)
			not_zero = 1;
			
	}
	
	return -1;
	
}

int size_of_probe(int probe_index){

	int i;
	
	for(i = probe_index; i < mem_size; i++){
		if(memory[i] != memory[probe_index])
			break;
	}
	
	return i - probe_index;
}

void insert_data(int start_index, unsigned int size, unsigned int duration){
	
	int i;
	
	for(i = start_index; i < size + start_index; i++){
		memory[i] = duration;
	}
}


int first_and_next_fit(unsigned int size, unsigned int duration, int start_index){
	
	int probe_count = 0, probe_size, probe_index = start_index, count = 0;
	
	while(1){
	
		probe_index = index_of_next_probe(probe_index);
		
		if(probe_index == -1)
			return -1;
		
		probe_size = size_of_probe(probe_index);
		
		probe_count++;
		
		if(probe_size >= size){
			
			insert_data(probe_index, size, duration);
			
			printf("index = %d, size = %d\n", probe_index, probe_size);
			
			printf("data_size = %d\n", size);
			
			//done
			last_placement_position = probe_index;
			return probe_count;
			
		}	
		
		printf("probe_count = %d\n", probe_count);
		printf("\n");
		
		if(count++ == 10)
			exit(1);
	}
}

int best_fit(unsigned int size, unsigned int duration){
	
	int index_of_best_fit = index_of_next_probe(0), 
		size_of_best_fit = size_of_probe(index_of_best_fit),
		probe_count = size_of_best_fit,
		candidate_index = index_of_best_fit,
		candidate_size,
		chunk_count = mem_fragment_count(mem_size),
		i;
		
	if(size_of_best_fit < size){
		index_of_best_fit = -1;
	}		
	
		
	for(i = 0; i < chunk_count; i++){
		
		if(size_of_best_fit == size)
			break;
		
		//fix for index 0 bug
		if(candidate_index == 0)
			candidate_index++;
			
		candidate_index = index_of_next_probe(candidate_index);
		candidate_size = size_of_probe(candidate_index);
		
		probe_count++;
		
		if(candidate_size < size_of_best_fit && candidate_size >= size){
			index_of_best_fit = candidate_index;
			size_of_best_fit = candidate_size;
		}
	}
	
	if(index_of_best_fit > -1)
		return probe_count;
	else
		return 0;

}

/*
  Using the memory placement algorithm, strategy, allocate size
  units of memory that will reside in memory for duration time units.

  If successful, this function returns the number of contiguous blocks
  (a block is a contiguous "chuck" of units) of free memory probed while 
  searching for a suitable block of memory according to the placement 
  strategy specified.  If unsuccessful, return -1.

  If a suitable contiguous block of memory is found, the first size
  units of this block must be set to the value, duration.
 */
int mem_allocate(mem_strategy_t strategy, unsigned int size, unsigned int duration)
{
	 if(strategy == FIRST)
	 	return first_and_next_fit(size, duration, 0);
	 else if(strategy == NEXT)
	 	return first_and_next_fit(size, duration, last_placement_position);
	 else
	 	return best_fit(size, duration);
	 
}

/*
  Go through all of memory and decrement all positive-valued entries.
  This simulates one unit of time having transpired.
 */
int mem_single_time_unit_transpired()
{
	int i;
	
	for(i = 0; i < mem_size; i++){
		if(memory[i] > 0)
			memory[i]--;
	}
	
	printf("decrementing\n");
	
	return 0;
}

/*
  Return the number of fragments in memory.  A fragment is a
  contiguous free block of memory of size less than or equal to
  frag_size.
 */
int mem_fragment_count(int frag_size)
{
	int i, count = 0, j, length;
	
	for(i = 0; i < mem_size; i++){
		if(memory[i] == 0){
			length = 0;
			j = i;
			while(memory[j] == 0){
				length++;
				j++;
			}
			
			if(length <= frag_size)
				count++;
				
			i = j;
		}
	}
	
	return count;
}

/*
  Set the value of zero to all entries of memory.
 */
void mem_clear()
{
	int i;
	
	for(i = 0; i < mem_size; i++){
		memory[i] = 0;
	}
}

/*
 Allocate physical memory to size. This function should 
 only be called once near the beginning of your main function.
 */
void mem_init( unsigned int size )
{
	last_placement_position = 0;

	memory = malloc( sizeof(unsigned int)*size );
	mem_size = size;
}

/*
 Deallocate physical memory. This function should 
 only be called once near the end of your main function.
 */
void mem_free()
{
	free( memory );
}
