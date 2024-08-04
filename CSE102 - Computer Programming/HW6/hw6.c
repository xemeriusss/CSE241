#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum type{INPUT, AND, OR, NOT, FLIPFLOP}; /* making type names in number with enum*/

struct gate{

	enum type type;
	char type_name[10];

	char output_name[10];
	char in_name1[10];
	char in_name2[10];

	struct gate *input1; /* for linked list */
	struct gate *input2;

	int output;
	int memory; /* for flipflop old values */
};

void link_in_to_out(struct gate **gates, struct gate *g); /* this function finds the same
															 strings in structure with the 
															 help of struct function 
														  */

int evaluation(struct gate *g); /* Recursive function starts from the 
								   end of the circuit and returns the
								   calculations of types. When it 
								   comes to INPUT it returns the number
								   in input.txt 
								*/

int main(){

	int i;
	char ch; /* for finding number of input */
	int line=0; /* for finding the total line of circuit.txt */
	int input_count=0;


	char garbage_input[10]; /* for taking the word of INPUT */ 


	int index;
	int gate_count;

	char temptypename[10];

	int n;

	FILE *inputFile;
	FILE *circuitFile;

	circuitFile = fopen("circuit.txt","r"); /* opening circuit.txt for finding input and line number */

	if(circuitFile == NULL){
		printf("Error reading file\n");
		return 1;
	}

	for(ch = getc(circuitFile) ; ch != EOF ; ch = getc(circuitFile)){

		if(line == 0){
			if(ch == ' ')
				input_count++;
		}

		if(ch == '\n')  
			line++;
	}

	fclose(circuitFile);

	gate_count = (line-1)+input_count;

	/* allocating total of input number and line number (except input) space for struct pointer */
	struct gate** gates = (struct gate**)malloc((gate_count+1) * sizeof(struct gate*));

	gates[gate_count] = NULL;

	if(gates == NULL){

		printf("Memory not allocated.\n");
        return 1;
	}


	circuitFile = fopen("circuit.txt","r"); /* opening circuit.txt second time for put into the structure */

	if(circuitFile == NULL){
		printf("Error reading file\n");
		return 1;
	}

	for(i=0; i<(input_count) ; i++){ 

		if(i==0)
			fscanf(circuitFile, "%s", garbage_input); /* taking the word of INPUT first */

		gates[i] = malloc(sizeof(struct gate)); /* allocating space for every struct array */

		if(gates == NULL){

			printf("Memory not allocated.\n");
        	return 1;
		}

		gates[i]->type = INPUT; /* making every input structure's type 0 */

		fscanf(circuitFile, "%s", gates[i]->output_name); /* taking input values and assigning to structure */
	}

	for(index=input_count; index < gate_count; index++){ /* until it reachs end of the struct which we allocate */

		gates[index] = malloc(sizeof(struct gate)); /* continue allocating space for every struct array */


		/* it's fixed that the first two element is type name and output */
		if(fscanf(circuitFile, "%s %s", temptypename, gates[index]->output_name) != 2){

			free(gates[index]);
			break;
		}

		/* after we get type we compare them */

		if(strcmp(temptypename, "AND")==0){

			gates[index]->type = AND;

			fscanf(circuitFile, "%s %s", gates[index]->in_name1, gates[index]->in_name2);
		}

		else if(strcmp(temptypename, "OR")==0){

			gates[index]->type = OR;

			fscanf(circuitFile, "%s %s", gates[index]->in_name1, gates[index]->in_name2);
		}

		else if(strcmp(temptypename, "NOT")==0){

			gates[index]->type = NOT;

			fscanf(circuitFile, "%s", gates[index]->in_name1);
		}

		else if(strcmp(temptypename, "FLIPFLOP")==0){

			gates[index]->type = FLIPFLOP;
			gates[index]->memory = 0; /* first memory will be 0 */

			fscanf(circuitFile, "%s", gates[index]->in_name1);
		}
	}

	fclose(circuitFile);

	for(i=0 ; i<gate_count ; i++){ /* passing whole structure to function */

		link_in_to_out(gates, gates[i]); 
	}

	inputFile = fopen("input.txt","r");

	if(inputFile == NULL){
		printf("Error reading file\n");
		return 1;
	}

	while(1){

		for(i=0; i < input_count; i++){

			if (fscanf(inputFile, "%d", &n) != 1) {
				break;
			}

			gates[i]->output = n;
		}

		if(i == input_count){
			printf("%d\n", evaluation(gates[gate_count-1])); /* passing the last element of structure to recursion */
		} else {
			break;
		}

	}

	fclose(inputFile);

	return 0;

} /* End of main */

/*=================================================================================*/

struct gate *find_out(struct gate **gates, char *out){  

	for(; *gates != NULL; gates++){ 

		if(strcmp((*gates)->output_name, out) == 0){ /* finding same string */

			return *gates;
		}
	}
	return NULL;
}

/*=================================================================================*/

void link_in_to_out(struct gate **gates, struct gate *g){

	switch(g->type){

		case AND:

		case OR:
			g->input1 = find_out(gates, g->in_name1); /* assiging the same first input strings to input1 structure  */
			g->input2 = find_out(gates, g->in_name2); /* assiging the same second input strings to input2 structure  */
			break;

		case NOT:

		case FLIPFLOP:
			g->input1 = find_out(gates, g->in_name1); /* assiging the same first input strings to input1 structure  */
			break;

		case INPUT:

		default:
			break;
	}
}

/*=================================================================================*/

int evaluation(struct gate *g){

	switch(g->type){

		case INPUT:
			return g->output; /* if it reaches INPUT, return the value inside of input.txt */

		case AND:
			g->output = evaluation(g->input1) && evaluation(g->input2);
			return g->output;

		case OR:
			g->output = evaluation(g->input1) || evaluation(g->input2);
			return g->output;

		case NOT:
			g->output = !evaluation(g->input1);
			return g->output;

		case FLIPFLOP:
			g->output = evaluation(g->input1) - g->memory != 0; /* making xor operation with calculation */
			g->memory = g->output;
			return g->output;
	}
}
