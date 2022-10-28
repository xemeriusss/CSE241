#include <stdio.h>

#define ROW 512
#define COL 512

int find_island(int arr[ROW][COL], int island, int i, int k, int len);

int main(){

	FILE *inFile;
	FILE *outFile;

	int arr[ROW][COL];
	int len; 
	int island = -1;

	int island_num = -1; /* for printing to output */
	int num=1;

	int i,j;


	inFile = fopen("input.txt","r"); 

	if(inFile == NULL){
		return 1;
	}

	fscanf(inFile, "%d", &len); /* finding island lenght */

	/* making array from file */
	for(i=0 ; i<ROW ; i++){

		for(j=0 ; j<COL ; j++){

			fscanf(inFile, "%d", &arr[i][j]);
		}
	}

	fclose(inFile);


	/* calling recursive function for every lenght */
	for(i = 0; i < ROW; i++){

		for(j = 0; j < COL; j++){

			island = find_island(arr, island, i, j, len);
		}
	}
	

	outFile = fopen("output.txt","w");

	if(outFile == NULL){
		return 1;
	}

	for(i = 0; i < ROW; i++){

		for(j = 0; j < COL; j++){

			if(arr[i][j] == island_num){ 

				/* x_coordinate1 y_coordinate1 */
				fprintf(outFile, "%d_coordinate%d %d_coordinate%d\n", i, num, j, num);

				num++;
				island_num--; /* if it is equeal to current island num, it is going to change next island num */
			}
		}	
	}

	fclose(outFile);

} /* End of Main */

/*===============================================================================================*/

int find_island(int arr[ROW][COL], int island, int i, int j, int len){

	int a,b;

	/* if it is out of border */
	if(i<0 || j<0 || i>(ROW-1) || j>(COL-1)){ 

		return island;
	}

	if(arr[i][j] > len){ /* if it is island */

		arr[i][j] = island; /* will change the array as island number */

		for(a=-1 ; a<2 ; a++){ /* for looking all sides of number except crosses */

			for(b=-1 ; b<2 ; b++){

				/* for not looking crosses */
				if(!((a == -1 && b == -1) || (a == -1 && b == 1) || (a == 1 && b == -1) || (a == 1 && b == 1))){

					find_island(arr, island, a+i, b+j, len);
				}
			}
		}

		island--; /* if it finishes the current island, it changes the island number for different islands */
		return island;
	}

	return island; /* if it is inside the borders but not an island, returns the current island number */
}

