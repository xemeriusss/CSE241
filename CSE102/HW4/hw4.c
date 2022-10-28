#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void part_of_buffer(char buffer[], char* name[]); /* function for parsing without + sign */

double price_of_object(char arr[]); /* recursive function returning sum */

						   			/* Recursive part will calculate the sum until a number enters
						   			   and then it is going to pass another object. And the same 
						   			   steps will be repeated until finishing the all object price 
						   			   and then reaching back to the define object. */

int is_number(char buffer[]); /* function for checking buffer is number or not */


int main(){

	char define[100]; 
	double result;
	char c;

	printf("Please define the object:");

	scanf("%s",define);

	c = getchar(); /* for \n character */

	result = price_of_object(define);

	printf("Total cost of %s: %.2f\n",define, result);

	return 0;
}

/* ========================================================================= */

double price_of_object(char arr[]){

	char buffer[100]; 
	char *name_of_objects[25]; /* string array for objects with quantities */
 	char object_name[100]; /* char array for just object name */
 	double double_value; /* for quantity */
 	double buffer_double_value;

	double sum = 0.0;
	double price;

	int i;

 	char c; /* for * sign */


	printf("\nWhat is %s?\n",arr);

	fgets(buffer, 100, stdin); /* getting buffer from user with stdin */

	if(is_number(buffer)){ /* if buffer is not number */

		part_of_buffer(buffer, name_of_objects); /* will parse into name_of_objects string array */

		for(i=0 ; name_of_objects[i] != NULL ; i++){

			/* this will parce as for example 2 * wheel */

   			sscanf(name_of_objects[i], "%lf %c %s", &double_value, &c, object_name);

			price = price_of_object(object_name); /* recursive part */
 
			sum = sum + (double_value * price);
		}

		return sum;
	}

	else{ /* if buffer is number */

		sscanf(buffer, "%lf", &buffer_double_value); /* convert buffer to double */

		return buffer_double_value;
	}

}

/* ========================================================================= */


void part_of_buffer(char buffer[], char *name[]){

	/* parse the buffer with strtok */

	char *p;
	int i=0;

	p = strtok (buffer,"+");

	while (p!= NULL){

    	name[i] = p; /* assigning buffer parts to string array */
    	i++;

    	p = strtok (NULL, "+");
  	}

  	name[i] = NULL; /* for finding part number and using in for loop */
}

/* ========================================================================= */


int is_number(char buffer[]){

    int i;
    int size;

    size = strlen(buffer);

    for(i=0 ; i<size; i++){
        
        /* if there is letter in buffer, it is not a number */
        if(('a' <= buffer[i] && buffer[i] <= 'z') || ('A' <= buffer[i] && buffer[i] <= 'Z')){
            
            return 1;
   		}
   	}
   	return 0;
}