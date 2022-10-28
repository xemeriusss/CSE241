#include <stdio.h>

/*Function for check to look behind one number and searching that is there a same number or not. */
int control_for_back(FILE *inFile, int first, int back_cursor); 

/*Function for sorting numbers and counts in ascending order. */
int sorting(int countarray[], int finarr[], int array_size);

int main(){

	/*-----------------------------------------------------------------------------*/
	int finarr[100]; /* array for different numbers */
	int countarray[100]; /* array for counts */

	int flag=0; /* flag for first number */
	int first; /* for holding and controlling numbers in loops */
	int f=0,c=0; /* array counters for number and count arrays */
	int i;
	int count=0; /* count for same numbers */

	int status1,status2,status3,status4; /* controlling status for fscanf */
	int num;
	int num_final;
	int temp_num;

	int cursor_after_num=0;
	int cursor_place;
	int back_cursor;

	int array_size=0; /* passing array size as a parameter for sorting array */
	/*-----------------------------------------------------------------------------*/


	FILE *inFile; /* declaring file pointers */
	FILE *outFile;

   inFile = fopen("input.txt", "r"); /* opening input.txt for reading */


   if(inFile == NULL){
        printf("Error Reading File\n");
        return 1;
   }


   else{

    	while(1){

    		/* making sure the cursor is in the right place after the round is over */
    		fseek(inFile, cursor_after_num, SEEK_SET); 

    		/* setting count to zero per loop */
    		count=0; 

    		/* saving cursor place before reading a number for control_for_back function */
    		back_cursor = ftell(inFile);  

    		/* reading number from input.txt */
    		status1 = fscanf(inFile, "%d,", &num); 

    		/* saving cursor place after reading a number */
    		if(status1 == 1)
    			cursor_after_num = ftell(inFile); 

    		/* assigning number to different value */
    		first = num; 

    		/* controlling that number is not the first number AND are there any same number before that number */
        	if(flag!=0 && control_for_back(inFile, first, back_cursor) == 1){ 

        		count++;

        		/* saving cursor place for going back to same place after reading number for exit */
        		cursor_place = ftell(inFile);

        		/* putting cursor after number for checking is it going to read number or not */
        		fseek(inFile, cursor_after_num, SEEK_SET);

        		status4 = fscanf(inFile, "%d", &num_final);

        		/* if it cannot read number, it is End of File, break the loop and finish the code */
        		if(status4 != 1)
        			break;
        		
        		/* if it can read number, put cursor to its old place */
				else{
					fseek(inFile, cursor_place, SEEK_SET);
        			continue;
        		}

        	}

        	/* if there is no same number at the back, make counting for new number */
        	else{

        		fseek(inFile, cursor_after_num, SEEK_SET);

        		/* loop for finding same number and counting them */
        		while(1){

        			cursor_place = ftell(inFile);

        			/* reading a temp num for compare numbers which are same */
        			status2 = fscanf(inFile, "%d", &temp_num); 

					if(status2 == 1){

						if(first == temp_num)
							count++;	 /* if there is same number make a count */
					}

					/* exiting from loop when finishing one round for one number */
					if(feof(inFile))  
						break;
			
        		}

        		countarray[c] = count;  /* assigning the final count in count array */
        		c++;  /* moving forward in array */

            finarr[f] = first;  /* assigning the final count in number array */
            f++;  

         }


        	flag++;  /* flag for checking is it first number in the file or not */

       	fseek(inFile, cursor_after_num, SEEK_SET);

       	status3 = fscanf(inFile, "%d", &num_final);

       	/* if there is no number after out while loop round, finish the code */
			if(status3 != 1) 
					break;

   	} /* end of While */
   } /* end of NULL else */

   fclose(inFile); /* closing input.txt */

   /*---------------------------------------------------------------------------*/

   outFile = fopen("output.txt", "w"); /* opening output.txt for writing numbers and its counts */

   if(outFile == NULL){
        	printf("Error Opening File\n");
        	return 1;
   }

   else{

    	if(c==f){ /* checking that count array and number array are have same size */

    		array_size = c;

    		/* sorting counts in ascending order */
    		sorting(countarray,finarr,array_size);

    		/* writing sorted numbers and counts in output.txt */
   		for(i = 0; i < array_size; ++i){

   			fprintf(outFile, "%d: %d\n",finarr[i],countarray[i]+1); /* adding +1 to count the number itself */
   		}
    	}
   }

   fclose(outFile); /* closing output.txt */

   return 0;

} /* End of Main */


/*==========================================================================*/


int sorting(int countarray[], int finarr[], int array_size){

	int i,j,temp;

	for(i = 0; i < array_size; ++i){

      for(j = i + 1; j < array_size; ++j){

      	/* if counts are same with the next number, just change the number places in ascending order */
         if(countarray[i] == countarray[j]){

            if(finarr[i] > finarr[j]){ 

               temp = finarr[i];
               finarr[i] = finarr[j];
               finarr[j] = temp;

            }
         }

         /* if counts are not same with the next number, both change the number and count places in ascending order */
         else if(countarray[i] > countarray[j]){

            temp = countarray[i];
            countarray[i] = countarray[j];
            countarray[j] = temp;

            temp = finarr[i];
            finarr[i] = finarr[j];
            finarr[j] = temp;

         }
      }
   }

   return 0;
}


/*==========================================================================*/


int control_for_back(FILE *inFile, int first, int back_cursor){

	int num;
	int status;
	int cursor_place=0;

	/* putting cursor to beginning of file for checking back of the number */
	fseek(inFile, 0, SEEK_SET); 

	while(back_cursor != cursor_place){ /* when cursor comes to the place which we sent, stop the loop */

		status = fscanf(inFile, "%d", &num);

		if(status == 1){

			cursor_place = ftell(inFile);

        	if(first == num){  /* if there is same number at the back, return 1 and break the loop */

            	return 1;
             	break;
        	} 
		}
	} 

	return 0;
}