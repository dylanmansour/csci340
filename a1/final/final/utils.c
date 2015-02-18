// -----------------------------------
// CSCI 340 - Operating Systems I 
// Spring 2015 (Tony Leclerc, Brent Munsell)
// utils.c implementation file
// Homework Assignment 1
//
// -----------------------------------

// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <stdio.h>	// IO functions
#include <stdlib.h> 	// atof function
#include <math.h>	// math functions
#include "utils.h"	// your own functions

// Now you put your implementation of the function prototypes here...

// -------------------------
// function read
// returns: int - number of v_structs read in from givin file
// params: file_name - char*, name of the file containing v_structs
//         p_vec_array - v_struct*, of v_structs that is to be populated
//
int read(char* file_name, v_struct* p_vec_array){
	
	//open file
	FILE * f = fopen(file_name, "r");
	
	//create space to store a line of the file
	char vec[100];
	
	//counter for number of vectors read in
	int counter = 0;

	//error reading the file
	if(f == NULL){
		printf("Error reading in file\n");
		exit(1);
	}	

	//loop through the file and read in each line
	while(fgets(vec, 100, f) != NULL){
		
		int pos = 0;
		double r, theta;
		
		//find position of the comma		
		while(vec[pos] != ',' && vec[pos] != '\0')
			pos++;
		
		if(vec[pos] == ','){
			
			//set the common position to the null value
			vec[pos] = '\0';
			
			//read in the just part of the string until the null value and convert to double
			r = atof(vec);
			
			//read in second half of the line and convert to double
			theta = atof(&vec[pos+1]);			
			
			//set values of the v_struct
			p_vec_array[counter].r = r;
			p_vec_array[counter].theta = theta;
			
			//increment counter
			counter++;
		}	
	
	}
	
	//close file
	fclose(f);

	//return the number of vectors read in
	return counter;
}


// -----------------------------
// function x_component
// returns: double - represents x component value of vector
// params: p_vector = pointer to v_struct
// 
double x_component(v_struct* p_vector){
	
	//compute and return x value
	return p_vector->r * cos(p_vector->theta);
}

// -----------------------------
// function y_component
// returns: double - represents y component value of vector
// params: p_vector = pointer to v_struct
// 
double y_component(v_struct* p_vector){
	
	//compute and return y value
	return p_vector->r * sin(p_vector->theta);
}

// -----------------------------
// function degrees_to_radians
// returns: void
// params: p_vector = pointer to v_struct
// 
void degrees_to_radians( v_struct* p_vector ){
	
	//get the degree value of the v_struct
	double degrees = p_vector->theta;
	
	if(degrees > 360){
		degrees -= 360;
	}else if(degrees < -360){
		degrees += 360;
	}
	
	//convert and set back to theta
	p_vector->theta = degrees * PI/180;
	 
}
