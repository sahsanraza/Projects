/*FileName: mystrings.c
 *Name: Ben Isenberg
 *Date: 6/22/11
 */


#include<stdio.h>
#include<string.h>

/*This program takes a file and prints strings of at least 4 characters long.  It creates the strings by reading the file character by character
and checking to see if the character is printable or not. It prints 1 string per line.  The maximum string size starts at 5 and doubles whenever
more space is needed.*/



int main(int argc, char *argv[]){
		
	int size = 5;	/*size of array*/
	char *str = malloc(sizeof(char) * size);		/*array to hold strings*/
	char *str2 = malloc(sizeof(char) * size);		/*array to hold string during array resizing process*/
	int x = 0;			/*counter*/
	int c;				/*character read from file*/

	FILE *sourceFile = fopen(argv[1], "r");		/*Get file name from command line*/

	/*If file doesn't exist exit program*/
	if(sourceFile == NULL){
		printf("File does not exist.");
		return 0;
	}

	
	/*While not at the end of the file*/
	while(c != EOF){
		
		c = fgetc(sourceFile);		/*c = next character in file*/
		
		/*if c is a non-printable character*/
		if(c > 126 || (c < 32 && c != 9)){
			/*if your current string is greater than 3 characters print it*/
			if(x > 3){
				printf("%s\n", str);
			}

			/*Clear array for next string to be read*/				
			for(x = 0; x < size; x++){
				str[x] = 0;
			}
				
			x = 0;
		}
		else{
			/*Resize array if full*/
			if(x >= size){
				strcpy(str2, str);	/*Copy array contents over to another array*/
				size = size*2;		/*Double array size*/
				str = malloc(sizeof(char) * (size));	/*Allocate memory for array*/
				strcpy(str, str2);						/*Copy user's string back into original array*/
				str2 = malloc(sizeof(char) * (size));	/*Double size of temporary storage array for any potential future resizing*/
				

			}
			/*add character to string and increment counter*/
			str[x] = c;
			x++;
		}
	}
		
	/*close file and exit program*/
	fclose(sourceFile);  

	return 0;


}
