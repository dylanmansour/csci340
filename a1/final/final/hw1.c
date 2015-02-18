// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <stdio.h>	// IO functions
#include <stdlib.h> 	// atof function
#include <math.h>	// math functions
#include "utils.h"	// your own functions

// ----------------------------------------------
// Define constants
#define TOTAL_VECS 10	// constant that defines the total number 
			// of vectors, i.e. magnitude,direction 
			// pairs, in the vectors text file.

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//		argv = array of argument values
//
//
int main( int argc, char** argv ) {
	
	// --------------------------------
	// Steps to be performed
	//	1. Define variables (including arrays)
	//	2. Using read() function in utils.h, read vectors defined in text file 
	//		- the location of the text file (i.e. vectors.txt) must be defined in argv[1]
	//  	3. Loop through array of vector structs and print to console:
	//		- vector magnitude (using only 2 decimal places)
	//		- vector direction (in radians using only 2 decimal places)
	//		- vector direction (in degrees using only 2 decimal places)
	//		- Using compute_x() function in utils.h, x component value (using only 2 decimal places)
	//		- Using compute_y() function in utils.h, y component value (using only 2 decimal places)
	//	    in the following format
	//		r = <value>, theta = <value> degrees, <value> radians, x_comp = <value>, y_comp = <value>
	//	    For example:
	//		r = 10.00, theta = 180.00 degrees, theta = 3.14 radians, x_comp = -10.00, y_comp = 0.00 
	//
	
	//create v_struct array with given space
	v_struct p_vec_array[TOTAL_VECS];		

	//read in the file of vectors and fill the array
	int count = read(argv[1], p_vec_array);	
	
	//check that number read in is as expected
	if(count != TOTAL_VECS){
		printf("Error: the total number of vectors read was %d, should have been %d\n", count, TOTAL_VECS);
		exit(1);
	}
	
	//loop through the array and print out results
	int i;
	for(i = 0; i < TOTAL_VECS; i++){
		
		//print r and theta
		printf("r = %.2f, theta = %.2f degrees, ", p_vec_array[i].r, p_vec_array[i].theta);
		
		//convert from degrees to radians
		degrees_to_radians(&p_vec_array[i]);
		
		//print theta (radians)
		printf("theta = %.2f radians, ", p_vec_array[i].theta);
		
		//print x and y
		printf("x = %.2f, y = %.2f\n", x_component(&p_vec_array[i]), y_component(&p_vec_array[i]));
	}
	
	//done!
	return 0;

} // end main() function
