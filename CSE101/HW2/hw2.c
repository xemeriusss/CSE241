#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main(void) {

	HANDLE hComm;                       
    char ComPortName[] = "\\\\.\\COM5"; // Name of Port
    BOOL Status;
    DWORD dwEventMask;
    char buffer[256];
    char bufferButton[256];
    DWORD NoBytesRead;

    char sel[10];
    char selSquare[10];
    char readChar;
    char selButton[10];
    char readButton;

    int i = 0;

    /*------------------------------------------ Defining the Menu -----------------------------------------*/

    printf("## WELCOME TO GTU ARDUINO LAB       ##\n");
	printf("## STUDENT NAME: BERRU LAFCI        ##\n");
	printf("## PLEASE SELECT FROM THE FOLLOWING ##\n");
	printf("## MENU :                           ##\n");
	printf("(1) TURN ON LED ON ARDUINO\n");
	printf("(2) TURN OFF LED ON ARDUINO\n");
	printf("(3) FLASH ARDUINO LED 3 TIMES\n");
	printf("(4) SEND A NUMBER TO ARDUINO TO COMPUTE SQUARE BY ARDUINO\n");
	printf("(5) BUTTON PRESS COUNTER\n");
	printf("(0) EXIT\n");

    /*---------------------------------- Opening the Serial Port -------------------------------------------*/

    hComm = CreateFile(ComPortName,                  
                       GENERIC_READ | GENERIC_WRITE,
                       0,                            
                       NULL,                         
                       OPEN_EXISTING,               
                       0,                            
                       NULL);             


    if (hComm == INVALID_HANDLE_VALUE)
        printf("\n   Error! - Port %s can't be opened", ComPortName);
    else
        printf("\n   Port %s Opened\n ", ComPortName);


    /*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

    DCB dcbSerialParams = {0}; 
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(hComm, &dcbSerialParams); 


    if (Status == FALSE)
        printf("\n   Error! in GetCommState()");



    dcbSerialParams.BaudRate = CBR_9600;   
    dcbSerialParams.ByteSize = 8;          
    dcbSerialParams.StopBits = ONESTOPBIT; 
    dcbSerialParams.Parity = NOPARITY;     

    Status = SetCommState(hComm, &dcbSerialParams);

    if (Status == FALSE) {
        printf("\n   Error! in Setting DCB Structure");
    }

    else {
        printf("\n   Setting DCB Structure Successfull\n");
        printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
        printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
        printf("\n       StopBits = %d", dcbSerialParams.StopBits);
        printf("\n       Parity   = %d", dcbSerialParams.Parity);
    }


    /*------------------------------------ Setting Timeouts --------------------------------------------------*/

    COMMTIMEOUTS timeouts = {0};

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
        printf("\n   Error! in Setting Time Outs");
    else
        printf("\n\n   Setting Serial Port Timeouts Successfull");
    


    /*------------------------------------ Setting Receive Mask ----------------------------------------------*/

    Status = SetCommMask(hComm, EV_RXCHAR); 

    if (Status == FALSE)
        printf("\n\n    Error! in Setting CommMask");
    else
        printf("\n\n    Setting CommMask successfull");


    /*----------------------------- Writing a Character to Serial Port----------------------------------------*/

    while(1) {

    	printf("\nYOUR CHOICE: ");
    	scanf("%s", &sel[0]); // sending the address of array

    	DWORD dNoOFBytestoWrite;    
    	DWORD dNoOfBytesWritten = 0; 
    	dNoOFBytestoWrite = sizeof(sel);

    	Status = WriteFile(hComm,  // first selection for writing to serial port a character         
                       	sel,          
                       	dNoOFBytestoWrite,  
                       	&dNoOfBytesWritten, 
                       	NULL);

    	if (Status == TRUE)
        	printf("\n\n    %s - Written to %s\n\n", sel, ComPortName);
    	else
        	printf("\n\n   Error in Writing to Serial Port");

        //======================================================================================

        if(sel[0] == '0'){
        	printf("GOODBYE~~\n");
			CloseHandle(hComm);
        	break;
        }

        //======================================================================================

        else if(sel[0] == '4'){

        	i=0; 

        	printf("NUMBER TO CALCULATE SQUARE: ");
			scanf("%s",&selSquare[0]);

			Status = WriteFile(hComm,    // square selection     
                       	selSquare,          
                       	dNoOFBytestoWrite,  
                       	&dNoOfBytesWritten, 
                       	NULL);

			do {

	            Status = ReadFile(
	            	hComm, 
	            	&readChar, 
	            	sizeof(readChar), 
	            	&NoBytesRead, 
	            	NULL);

	            buffer[i] = readChar;
	            i++;

	        } while (NoBytesRead > 0);


	        printf("\n");

	        int j = 0;

	        for (j = 0; j < i - 1; j++) {  
	            printf("%c", buffer[j]);
	        }

        }

        //======================================================================================

        else if(sel[0]=='5'){


    		while(1){

                i=0;

    			printf("(1) SEE BUTTON COUNTER\n");
                printf("(0) BACK TO MAIN MENU\n");
                printf("YOUR CHOICE FOR BUTTON: ");
            	scanf("%s",&selButton[0]);
            
            	if(selButton[0]=='0')
					break; 
			
    			do{
            		Status = ReadFile(
                        hComm, 
                        &readButton, 
                        sizeof(readButton), 
                        &NoBytesRead, 
                        NULL);

            		bufferButton[i] = readButton;
            		i++;
        		} while (NoBytesRead > 0);

        		int j = 0;
        		for (j = 0; j < i - 1; j++){
            		printf("%c",bufferButton[j]);
                }        		
			} 
   		}

    }

	return 0;
}