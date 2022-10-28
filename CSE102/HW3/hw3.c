#include <stdio.h>
#include <string.h>
#include <math.h>

/* function for making operations for polynomial */
double evaluations(double coeff_arr[], int power_arr[], double value, int a_ind);

int main(){

	FILE *fp;
	FILE *fp_value;
	FILE *fp_output;

	/*---------------------------------------------------------------------------------------*/

	char str[1000];
	int count=0; /* for counting digit of the coeff */
	int index=0; 
	int num_start;
	int i,j,k=0;

	char a[10]; /* array to convert char to float for coeff */
	char b[10]; /* power convert */

	double coeff_arr[100]; /* array to hold coefficients */
	int power_arr[100]; /* array to hold powers */
	int a_ind=0,b_ind=0; /* values to count size of coeff and power arrays */

	double coeff;
	int power;

	int size;

	int flag=0; /* flag for first element */

	double value_arr[100];
	double value,temp_value;
	int v=0;

	int n;

	double result=0;

	/*---------------------------------------------------------------------------------------*/

	fp = fopen("polynomial.txt" , "r");

	if(fp == NULL) {
      printf("Error opening file");
      return 1;
    }

   else {

   		fgets(str, 1000, fp); /* putting whole polynomial into array as char */

   		size = strlen(str);

   		for(i=0 ; i < size ; i++){ /* until end of the polynomial array */

   			if(str[i] == 'x'){

   				index = i;

   				/* power is 0 if after char is +,- sign or it is end of array */
   				if(str[index+1] == 43 || str[index+1] == 45 || (index+1) == size){ /* 43 is + sign, 45 is - sign */

   					power_arr[b_ind] = 0;
   					b_ind++;
   				}

   				/* coeff is 1 if the first char is x or before char is +,- sign */
   				if(index == 0 || str[index-1] == '+' || str[index-1] == '-'){

   					if(i == 0)
   						coeff = 1;
   					
   					else if(str[index-1] == 43)
   						coeff = 1;

   					else if(str[index-1] == 45)
   						coeff = -1;
   				}

   				else{ /* going back in the array for finding the coeff and making integer */

   					num_start = index - count;

   					for(j= 0 ; j<count ; j++){

   						a[j] = str[num_start];
   						num_start++;
   					}

   					a[j] = '\0'; /* putting \0 to end of temp array to make proper sscanf */

   					sscanf(a, "%lf", &coeff); /* converting char to integer */

   				}

  				coeff_arr[a_ind] = coeff; 
   				a_ind++;

   				count=0;
   				index=0;
   				flag=1;
   			}

   			else if(str[i] == 43 || str[i] == 45){ 

   				if(i == 0){ /* if i=0 and char is + or -, it is the first coeff and it is negative */

   					count++;
   				}

   				else{

   					index = i;

   					while(str[index] != 'x'){ /* increase the count (digit of coeff) until come to x */

   						count++;
   						index++;
   					}

   					index=0;
   				}

   			}

   			else if(str[i] == 94){ /* 94 is power symbol */

   				index = i;

   				while(str[index] != 43 || str[index] != 45){ 

   					b[k] = str[index+1]; /* assigning power digits to temp array for converting */
   					k++;
   					index++;

   					if((index+1) == size || str[index+1] != 43 || str[index+1] != 45) /* end of array */
   						break;
   				}

   				b[k] = '\0';

   				sscanf(b, "%d", &power);

   				power_arr[b_ind] = power;
   				b_ind++;

   				k=0;
   				index=0;
   			}

   			else{ 

   				if(flag == 0){ /* if str[i] is number and it is the first element, increase count */

   					count++;
   				}
   			}

   		} /* end of for loop */

   } /* else of not NULL */

   	fclose(fp);


   	fp_value = fopen("values.txt" , "r");

   	if(fp_value == NULL) {
      printf("Error opening file");
      return 1;
    }

    else {

    	while(!feof(fp_value)){

    		fscanf(fp_value, "%lf", &value); /* reading values from values.txt */

    		value_arr[v] = value; /* assigning values to array */
    		v++;

    	}
    }

    fclose(fp_value);


    fp_output = fopen("evaluations.txt" , "w");

    if(fp_output == NULL) {
      printf("Error opening file");
      return 1;
    }

    else{

    	if(a_ind == b_ind){

   			for(n=0 ; n<(v-1) ; n++){ /* loop for sending values one by one */

   				temp_value = value_arr[n];

   				result = evaluations(coeff_arr, power_arr, temp_value, a_ind); 

   				fprintf(fp_output, "%.2f\n", result);

   			}
   		}
    } 
	
	fclose(fp_output);
	return 0;

} /* end of main */

/* ====================================================================================== */

double evaluations(double coeff_arr[], int power_arr[], double value, int a_ind){

	double part=0,result=0;
	int i;

	for(i=0 ; i<a_ind ; i++){

		if(power_arr[i] == 0) /* if there is no power, multiply coeff with value */
			part = coeff_arr[i] * value;
		
		else
			part = coeff_arr[i] * pow(value,power_arr[i]);

		result = result + part; /* calculating the result part by part */
	}

	return result;
}