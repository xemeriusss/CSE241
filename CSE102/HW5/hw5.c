#include <stdio.h>

#define MAX 250

int recursive_func(char str[], int cursor, FILE *outFile); /* function for making newick output with recursion */

int main(){

	char buffer[MAX];
	int cursor; /* for calculating - number */

	int i=0;

	FILE *inFile;
	FILE *outFile;

	inFile = fopen("input.txt", "r"); /* opening input.txt for reading */

    if(inFile == NULL)
        	return 1;
    

	else{

		fgets(buffer, 250, inFile);

		cursor = 0;

		outFile = fopen("output.txt", "w"); /* opening output.txt for writing */

		/* sending output file to the function because of not opening the file again and again with recursion */
		recursive_func(buffer, cursor, outFile);

	}

	fclose(inFile);

	fclose(outFile);

	return 0;

} /* end of main */


int recursive_func(char str[], int cursor, FILE *outFile){

	int i;


	if(str[0] == '\0')  /* if it's end of string, exit from function */
		return 1;
	

	else if(str[0] == '('){  /* if it's starting of a parentheses, increase the - number */

		cursor++;
		recursive_func(&str[1], cursor, outFile);  /* by giving &str[1] to the function, we're going forward in array */
	}

	else if(str[0] == ','){ 

		recursive_func(&str[1], cursor, outFile);
	}

	else if(str[0] == ')'){ /* if it's ending of a parentheses, decrease the - number */

		cursor--;
		recursive_func(&str[1], cursor, outFile);
	}

	else{ /* if it's other then upper characters, print that character to output */

		for(i=0 ; i<cursor ; i++){

			fputc('-', outFile);
		}

		fputc(str[0], outFile);
		fputc('\n', outFile);

		recursive_func(&str[1], cursor, outFile);
	}

} /* end of function */