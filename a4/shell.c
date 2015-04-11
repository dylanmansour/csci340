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
#include <sys/fcntl.h>
#include <sys/signal.h>

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};

// --------------------------------------------
// Implementation for each prototype below
// --------------------------------------------
// void parse( char* line, command_t* p_cmd );
// int execute( command_t* p_cmd );
// int find_fullpath( char* fullpath, command_t* p_cmd );
// int is_builtin( command_t* p_cmd );
// int do_builtin( command_t* p_cmd );
// void cleanup( command_t* p_cmd );

int find_fullpath( char* fullpath, command_t* p_cmd ){
	
	struct stat buffer;
	int partCount, i, start_pos, exists, collen, path_length, pos_of_next_collen, length_of_part;

	char* path_env_variable;
	path_env_variable = getenv( "PATH" );
	partCount = number_of_needle_in_hey_stack(path_env_variable, ':') + 1;

	start_pos = 0;
	for(i = 0; i < partCount; i++){
			
		//get the pos of the next collen
		pos_of_next_collen = pos_of_next_char(path_env_variable, start_pos, ':');
		
		//calculate the length of the current part
		length_of_part = pos_of_next_collen - start_pos;
		
		//compute the total length of this path
		path_length = string_length(p_cmd->name) + length_of_part + 1;
		
		//define a new char with this total length
		char full_path_test[path_length + 1];
		
		//copy in the path
		copy_string_part(path_env_variable, full_path_test, start_pos, pos_of_next_collen);
		
		//concat the path and trailing / before command
		string_concat(full_path_test, "/", full_path_test);
		
		//concat the path and the command
		string_concat(full_path_test, p_cmd->name, full_path_test);

		//check if the cmd is in the current path;
		exists = stat(full_path_test, &buffer); 
		
		if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) )  {
        	//Directory exists
        	return 0;
		} else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
          	// File exists
          	
          	//copy current full path to fullpath pointer
          	copy_string_part(full_path_test, fullpath, 0, string_length(full_path_test));
          	
        	return 1;
		} else {
			//calculate new starting pos
			start_pos = pos_of_next_collen + 1;
		}
	}
	
	
	return 0;
}

void string_concat(char* a, char* b, char* target){
	int i = 0;
	while (a[i] != '\0') {
    	target[i] = a[i];
    	i++; 	
	}
 
    int j = 0;
   	while (b[j] != '\0') {
    	target[i++] = b[j++];
   	}
 
   	target[i] = '\0';
}

int number_of_needle_in_hey_stack(char* str, char needle){
	int i = 0, counter = 0;
	while(str[i] !=  '\0'){
		if(str[i] == needle){
			counter++;
		
		}
		i++;
	}
	return counter;
}

int strings_equal(const char* a, const char* b){
	
	while(*a == *b){
      if (*a == '\0' || *b == '\0')
         break;
    
      a++;
      b++;
    }
         	
   if(*a == '\0' && *b == '\0'){
      return 1;
   }else{
      return 0;
    }
}

int do_builtin( command_t* p_cmd ){
	
	int i = 0;
	
	while(valid_builtin_commands[i] != NULL){
		
		//find the cmd in the array of known cmds
		if(strings_equal("cd", valid_builtin_commands[i])){
			//cd - check that is only one argv
			if(p_cmd->argc == 2){
				return chdir(p_cmd->argv[1]);
			}else{
				return 0;
			}
		}
		i++;
	}
	
	
	return 0;
}

int is_builtin( command_t* p_cmd ){

	int i = 0;
		
	while(valid_builtin_commands[i] != NULL){
		if(strings_equal(p_cmd->name, valid_builtin_commands[i])){
			return 1;
		}
		i++;
	}
	
	return 0;
}

void parse( char* line, command_t* p_cmd ){

	//declare vars
	int i, partCount, start_pos, pos_of_next_space, length_of_part;
	
	//remove the line break at the end of the line
	//line[string_length(line) - 1] = '\0';

	//get the number of of spaces and add one, this is the number of parts
	partCount = number_of_needle_in_hey_stack(line, ' ') + 1;
		
	//allocate space for all the parts (+1 for null)
	p_cmd->argv = (char **) malloc(sizeof(char *) * (partCount + 1));

	//set argc to the number of parts
	p_cmd->argc = partCount;

	
	//loop through all parts
	start_pos = 0;
	for(i = 0; i < partCount; i++){
		
		//get the pos of the next space
		pos_of_next_space = pos_of_next_char(line, start_pos, ' ');
		
		//calculate the length of the current part
		length_of_part = pos_of_next_space - start_pos;
		
		//allocate space for this part and copy the part in
		p_cmd->argv[i] = malloc(sizeof(char *) * (length_of_part));
		copy_string_part(line, p_cmd->argv[i], start_pos, pos_of_next_space);
		
		//check if first part
		if(i == 0){
			// allocate and copy first part to cmd name
			p_cmd->name = malloc(sizeof(char *) * (length_of_part));
			copy_string_part(p_cmd->argv[i], p_cmd->name, 0, length_of_part);
		}
		
		//calculate new starting pos
		start_pos = pos_of_next_space + 1;
	}
	
	//null terminate the argv array
	p_cmd->argv[partCount] = NULL;
	
}

int pos_of_next_char(char* str, int start_pos, char needle){
	int i = start_pos;
	while(str[i] != '\0'){
		if(str[i] == needle){
			if(str[i + 1] == '\0'){
				str[i] = '\0';
			}
			return i;
		}
		i++;
	}
	
	return i;
}

void copy_string_part(char* src, char* dest, int start, int stop){
	
	int i = 0;
	while((dest[i] = src[start])){
		if(start++ == stop)
			break;
		i++;
	}
	dest[i] = '\0';
}

int string_length(char* str){
	
	int i = 0;
	
	while(str[i] != '\0')
		i++;
		
	return i;
}


int execute( command_t* p_cmd ){

	int fnd = FALSE, fnd1 = FALSE, fnd2 = FALSE, i, currentCmd, start_pos, posOfNextPipe, newLineLength, j;
	char fullpath[255];
	char fullpath1[255];
	char fullpath2[255];
	int status;
	
	int pipeCount = number_of_element_in_array(p_cmd->argv, "|");
	int bgCount = number_of_element_in_array(p_cmd->argv, "&");
	int redirectCount = number_of_element_in_array(p_cmd->argv, ">");
	
	int numOfCmds = pipeCount + 1;
	
	int child_process_status[numOfCmds];
	int fds[2];
	pid_t cpid1;
	pid_t cpid2;
	
	if(pipeCount > 0){
		
		command_t p_cmd_array[numOfCmds];
		
		currentCmd = 0;
		start_pos = 0;
		
		while(currentCmd != numOfCmds){
			
			newLineLength = 0;
			posOfNextPipe = pos_of_element_in_argv(p_cmd, "|", start_pos);
			
			if(posOfNextPipe == start_pos){
				printf("Command is not valid.\n");
				return 0;
			}
			
			for(j = start_pos; j < posOfNextPipe; j++)
				newLineLength += string_length(p_cmd->argv[j]);
			
			char newLine[newLineLength + j + 1]; //length of all elements plus spaces in between
			newLine[0] = '\0';
			
			//build the new line
			for(j = start_pos; j < posOfNextPipe; j++){
				string_concat(newLine, p_cmd->argv[j], newLine);
				if(j != posOfNextPipe - 1)
				string_concat(newLine, " ", newLine); //add a space if not the last part
			}
			newLine[newLineLength + j] = '\0';
						
			parse(newLine, &p_cmd_array[currentCmd]);
			
			start_pos = posOfNextPipe + 1;
			currentCmd++;
		}
		
		
		if(numOfCmds == 2){
			
			pipe( fds );
			
			if ( (cpid1 = fork()) == 0 ) {
				
				close(1); /* close normal stdout */
				dup( fds[1] ); /* make stdout same as fds[1] */
				close( fds[0] ); /* we don’t need this */
				
				fnd1 = find_fullpath( fullpath1, &p_cmd_array[0] );
			
				if ( fnd1 ) {
					execv( fullpath1, p_cmd_array[0].argv );
				}
			}
			
			if ( (cpid2 = fork()) == 0 ) {
				
				close(0); /* close normal stdin */
				dup( fds[0] ); /* make stdin same as fds[0] */
				close( fds[1] ); /* we don’t need this */
				
				fnd2 = find_fullpath( fullpath2, &p_cmd_array[1] );
			
				if ( fnd2 ) {
					execv( fullpath2, p_cmd_array[1].argv );
				}
			}
			
			close( fds[0] );
			close( fds[1] );
			
			waitpid( cpid1, &child_process_status[0], 0 );
			waitpid( cpid1, &child_process_status[1], 0 );
			
		}
		
			
		
	}else if(bgCount > 0){
	
		pid_t pid;
		
		char *dbplaceholder = p_cmd->argv[p_cmd->argc - 1];
		p_cmd->argv[p_cmd->argc - 1] = NULL;
		
		pid = fork();
		if ( pid == 0 ) {
			
			fnd = find_fullpath( fullpath, p_cmd );
			
			if ( fnd ) {
				execv( fullpath, p_cmd->argv );		
			} else {
				printf("Command not found.\n");
				return 0;
			}
			perror("Child process terminated in error condition!");
			exit(1);
		}
		
		
		p_cmd->argv[p_cmd->argc - 1] = dbplaceholder;
		
		return 0;
	
	}else if(redirectCount > 0){
	
		int posOfRedrirect = pos_of_element_in_argv(p_cmd, ">", start_pos);
		char *pointerToRedirect = p_cmd->argv[posOfRedrirect];
		char *fileForRedirection = p_cmd->argv[posOfRedrirect + 1];
		
		//null the >
		p_cmd->argv[posOfRedrirect] = NULL;
		
		if(posOfRedrirect == 0){
			printf("Command is not valid.\n");
			return 0;
		}
		
		int child_process_status;
		int outfile;
		pid_t cpid1;
				
		outfile = open( fileForRedirection, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP );
		if ( outfile == -1 ) {
			printf("Failed to open file.\n");
		} else if ( (cpid1 = fork()) == 0 ) {
			dup2( outfile, 1 );
			
			fnd = find_fullpath( fullpath, p_cmd );
			
			if ( fnd ) {
				execv( fullpath, p_cmd->argv );		
			} else {
				printf("Command not found.\n");
				return 0;
			}
		}
		
		close( outfile );
		waitpid( cpid1, &child_process_status, 0 );
		
		
		p_cmd->argv[posOfRedrirect] = pointerToRedirect;
		
		return 0;
		
	}else{


		fnd = find_fullpath( fullpath, p_cmd );
			
		if ( fnd ) {
			if ( fork() == 0 ) {
				execv( fullpath, p_cmd->argv );
			}
			wait( &status );
		} else {
			printf("Command not found.\n");
			return 0;
		}
	}
	
	return 1;
}

void cleanup( command_t* p_cmd ){
	
	//free the name
	free(p_cmd->name);
	
	//free the argv array elements
	int i;
	for(i = 0; i < p_cmd->argc; i++){
		free(p_cmd->argv[i]);
	}
	
	//free the argv array itself
	free(p_cmd->argv);
}

int number_of_element_in_array(char** arr, char* element){
	
	int i = 0;
	int counter = 0;
	
	while(arr[i] != NULL){
		if(strings_equal(arr[i], element)){
			counter++;
		}
		i++;
	}
	
	return counter;
}

int pos_of_element_in_argv(command_t* p_cmd, char* element, int start_pos){
	
	int i;
	
	for(i = start_pos; i < p_cmd->argc; i++){
		if(strings_equal(p_cmd->argv[i], element)){
			return i;
		}
	}
	
	return p_cmd->argc;
}