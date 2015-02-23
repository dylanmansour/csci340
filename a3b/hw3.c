// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main( int argc, char** argv ) {

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while ( !done )
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
    //      if (parsed command is "exit")
    //      	done = true
	//  	else if ( is _builtin( ... ) )
	//			do_builtin( ... )
	//		else if ( find_fullpath( ... ) )
	//  		execute( ... )
	//		else
	//			error()
	// 		cleanup( ... )
	//

	printf( "CSCI 340 Homework Assignment 2 - Have Fun!\n" );
	
	int done = 0;
	char line[255];
	
	command_t p_cmd;
	
	while(!done){
		//print the prompt
		printf("$$ ");
		
		//read the line 
		fgets(line, sizeof(line), stdin);
						
		if(*line == '\n')
			continue;	
			
		line[string_length(line)-1] = '\0';
			
		if(strings_equal(line, "exit")){
			exit(1);
		}		
						
		//parse the input
		parse(line, &p_cmd);
		
		if(is_builtin(&p_cmd))
			do_builtin(&p_cmd);
		else
			execute(&p_cmd);
		
		cleanup(&p_cmd);
		
	}
	
	

	return 0;

} // end main function
