
/********************************************************************************************************************
 * 
 * My code is keeping the first number with fscanf. Then it compares the number it keeps with other numbers 
 * up to the end of file. If it's the same number, it replaces the number with a space. After coming to the 
 * end of file, it returns to the place where we kept the first number and if there is no subsequent number, 
 * it finishes the code. If there is, the code continues with the number after the first number. 
 * Thus, the code terminates when the outer loop reads the last number.
 * 
 * 
 * For efficiency, for example, if there are 5 numbers in file.txt, it will return 4 times when we looking at the
 * outer loop, and it will return 4-3-2-1 times (4 turns in total) in the inner loop, provided that it decreases 
 * by 1 each time. Therefore, we can give an approximate ratio such as (n.(n+1)/2)-n to the total number of turns. 
 * This; there will be 4950 (approximately 5000) rounds in 100 counts, 499500 (approximately 500000) rounds in 1000 counts. 
 * If we assume 100 numbers as 1 second and 1000 numbers as 10 seconds, this would be an efficient code compared 
 * to the increased number of rounds.
 * 
 * 
 ********************************************************************************************************************/

#include <stdio.h>

int digit(int num); /* function for calculating digit of number */

int main(){

	/*----------------------------------------------------------------*/
	int cursor_place;

	int len_of_num; /* digit num */

	int status;  /* status for controlling fscanf */
	int status2;
	int status3;
	
	int num;  /* numbers for fscanf */
	int temp_num;
	int num_final;

	int i;

	int cursor_after_num=0; /* cursor for holding number */
	/*----------------------------------------------------------------*/

	FILE *filep = fopen("file.txt","r+");  /* opening file with r+ (both reading and writing without deleting) */

	if(filep == NULL){
		printf("File not found.\n");
	}

	else{

		/* Beginning of loop */

		while(1){

			fseek(filep, cursor_after_num, SEEK_SET); /* putting cursor to second num after a round for one same num */

			status = fscanf(filep, "%d", &num); 

			if(status == 1){ /* controlling fscanf was able to read or not */

				cursor_after_num = ftell(filep); /* saving the place that we are going to back after a round */
			}


			/* Beginning of nested loop */

			while(1){  

				cursor_place = ftell(filep);

				status2 = fscanf(filep, "%d", &temp_num); /* reading a temp num for compare numbers which are same */

				if(status2 == 1){

					if(num == temp_num){

						len_of_num = digit(temp_num); /* finding digit of num for going back properly */

						fseek(filep, -(len_of_num), SEEK_CUR); /* going back as digit number */

						for(i=0 ; i<len_of_num ; i++){ /* loop for making spaces for the same number */
							fputs(" ",filep);
						}
					}
				}

				if(feof(filep))  /* exiting from loop when finishing one round for one number */
					break;
			
			}  /*end of while */


			fseek(filep, cursor_after_num, SEEK_SET); /* putting cursor to place which we determine
													 for passing another number to compare */

			status3 = fscanf(filep, "%d", &num_final);

			if(status3 != 1)  /* if there is no number after a whole and big round, we can finish the code */
				break;

		} /* end of while */

	} /* end of else */

} /* end of main */

/**********************************************************************/

int digit(int num){

	int count=0;

	if(num < 0)  /* if num is negative, there will be 1 more digit to delete */
		count++;
	
	while(num != 0){

		count++;
		num = num/10;
	}

	return count;
}

/**********************************************************************/

