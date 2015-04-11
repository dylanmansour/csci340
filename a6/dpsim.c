#include "dpsim.h"


static const unsigned int NUM_PHILOSPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosphers[5];


void* th_main( void* th_main_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here

    long i;
	int chopsticksTemp[NUM_CHOPSTICKS], deadlock = 1;
	
	
	//init the chopstick array
	for(i = 0; i < NUM_CHOPSTICKS; i++){
		chopsticks[i] = -1;
	}
	
	//spin up phil threads
	for(i = 0; i < NUM_PHILOSPHERS; i++){
		if(pthread_create(&(philosphers[i]), NULL, th_phil, (void *)i)){
			printf("Error creating philospher thread %ld\n", i);
			exit(1);
		}
	}
	
	//begin
	while(TRUE){
		
		//copy the chopsticks to temp
		for(i = 0; i < NUM_CHOPSTICKS; i++){
			chopsticksTemp[i] = chopsticks[i];
		}
		
		//check for deadlock
		for(i = 0; i < NUM_CHOPSTICKS; i++){
			if(chopsticksTemp[i] == -1 || chopsticksTemp[i] == chopsticksTemp[(i + NUM_CHOPSTICKS + 1) % NUM_CHOPSTICKS]){
				deadlock = 0;
			}
		}
		if(deadlock){
			printf("Deadlock condition (0,1,2,3,4) ... terminating\n");
			break;
		}
		
		//print out eaters
		printf("Philopsher(s) ");
		for(i = 0; i < NUM_CHOPSTICKS; i++){
			if(chopsticksTemp[i] == chopsticksTemp[(i + NUM_CHOPSTICKS + 1) % NUM_CHOPSTICKS] && chopsticksTemp[i] != -1){
				printf("%d, ", chopsticksTemp[i]);
			}
		}
		printf("are eating\n");
		
		//think
		delay(1000);
		
		//reset deadlock assumption
		deadlock = 1;
	}
	
	//kill phil threads
	for(i = 0; i < NUM_PHILOSPHERS; i++){
		pthread_kill(philosphers[i], 9);
	}
	
	//done
	pthread_exit(0);

} // end th_main function


void* th_phil( void* th_phil_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here
	
	int id = (int)th_phil_args;
	
	while(TRUE){
		delay(1000);
		eat(id);
	}


} // end th_phil function


void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {

	// ---------------------------------------
	// TODO: you add your implementation here

	//lock right chockstick
	pthread_mutex_lock(&(mutex[phil_id]));
	
	//pick up right chopstick
	chopsticks[phil_id] = phil_id;
	
	delay(10);
	
	//lock left chopstick
	pthread_mutex_lock(&(mutex[(phil_id + NUM_PHILOSPHERS + 1) % NUM_PHILOSPHERS]));
	
	//pick up left chopstick
	chopsticks[(phil_id + NUM_PHILOSPHERS + 1) % NUM_PHILOSPHERS] = phil_id;
	
	//eat
	delay(20000);
	
	//put down left chopstick
	chopsticks[(phil_id + NUM_PHILOSPHERS + 1) % NUM_PHILOSPHERS] = -1;
	
	//unlock left chopstick
	pthread_mutex_unlock(&(mutex[(phil_id + NUM_PHILOSPHERS + 1) % NUM_PHILOSPHERS]));
	
	//delay(10);
	
	//put down right chopstick
	chopsticks[phil_id] = -1;
	
	//unlock right chopstick
	pthread_mutex_unlock(&(mutex[phil_id]));


} // end eat function
