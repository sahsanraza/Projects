/*Filename: encrypt.c
 *Name: Ben Isenberg
 *Date:
 */

/*Project Description:  This program can encrypt a message, decrypt a message, and print data about the ecnrypted message.
 *In order to have a file encrypted you must enter into the command line ./encrypt -e sourcefile outputfile key hint.
 *After this is entered, the program reads the data from the specified sourcefile, encrypts it by xoring it with the key,
 *then it prints the number 0449, the enrypted message, the word TAG, the hint, and then a time stamp saying when the encryption
 *occurred.  To decrypt a file you enter in the command line ./encrypt -d sourcefile outputfile key.  The program reads
 *the sourcefile and checks to make sure it has a number and tag.  Then it decrypts the message with the key and writes the
 *message into the outputfile.  To see the hint and time stamp printed on screen, enter into the command line 
 *./encrypt -r sourcefile.  It reads the encrypted file and prints out the hint and time stamp stored in it.*/

#include <stdio.h>
#include <string.h>
#include <time.h>


/*A struct used to store the tag information for encryption*/
struct tag{
	char* hint[64];
	char* time[26];
	char* t[4];
};

/*Function displays how to use program*/
void usage(){
	printf("Usage for encryption:\n");
	printf("\t./encrypt -e infile outfile key hint\n");
	printf("Usage for decryption:\n");
	printf("\t./encrypt -d infile outfile key\n");
	printf("Usage for tag reading:\n");
	printf("\t./encrypt -r infile\n");
}


/*Function that prints the tag hint and time stamp from a specified encrypted file*/
void printTag(FILE *sourceFile){
	
	/*Holds encrypted message*/
	char *string;
	
	char *ptr;

	/*int length;
	char hint[64];
	char timeStamp[26];
	char year[5] = "2011";*/


	/*Allocate memory for tag*/	
	char *myTag = malloc(sizeof(char)*256); 
	/*Magic number*/
	char magicNum[5] = "0449";
	/*Tag string*/
	char tag[4] = "TAG";

	string = malloc(sizeof(char)*256);

	/*If the sourcefile doesn't exist exit program*/
	if(sourceFile == NULL){
		printf("Source file not found.\n");
		return;
	}

	/*Read first line of sourcefile*/
	fread(string, sizeof(char), 256,  sourceFile);

	/*Check to see if the file is encrypted*/
	if(strncmp(magicNum, string, 4) != 0){
		printf("Source file is not an encrypted 0449 file.\n");
		return;
	}


	/*Check to see if the file has a tag*/
	if(strstr(string, tag) == NULL){
		printf("No tag was found.\n");
		return;
	}


	printf("Tag Contents:\n");

	/*Find where tag begins in encrypted message*/
	ptr = strstr(string, tag);

	/*copy hint and time stamp from tag into myTag*/
	strcpy(myTag, ptr+3);

	/*length = strcspn(myTag, year);

	memmove(hint, myTag, (length-22));*/
	
	/*Print hint and time stamp onto command terminal*/
	printf(myTag);
	printf("\nDone.\n");

}

/*This function encrypts a line of data from a specified source file using a specified key. 
*It also adds a magic number and tag to the encrypted file*/
void encrypt(FILE *sourceFile, FILE *outputFile, char *key[], char *hint[]){
	
	/*Holds string to be encrypted*/
	char *string;
	
	/*Holds encrypted message*/
	char message[256];
	
	/*Variable used in for loops*/
	int x;
	/*Stores the size of the message to be encrypted*/
	int size;
	/*Array holds the key*/
	char key2[sizeof(key)];
	/*Tag struct*/
	struct tag myTag;
	/*Array that stores first 64 bytes of the entered hint*/
	char myHint[64];
	/*Stores the time of encryption*/ 
	char *encryptTime;

	/*Counter used to repeat key in encryption*/
	int i = 0;
	/*Magic number*/
	char number[5] = "0449";

	/*Find the present time and store it in encryptTime*/
	time_t timer;

	time_t  time_raw_format;
	struct tm * ptr_time;

	time ( &time_raw_format);
	ptr_time = localtime ( &time_raw_format );

	encryptTime = asctime(ptr_time);

	/*Allocate memory for string*/	
	string = malloc(sizeof(char)*256);
	/*Copy entered key into key2 array*/
	strcpy(key2, *key);
	/*place first 64 bytes of entered hint into myHint array*/	
	strxfrm(myHint, *hint, 64);
	/*Allocate memory for tag struct hint*/	
	myTag.hint[64] = malloc(sizeof(char)*(64));
	/*Place hint into tag struct*/	
	strcpy(myTag.hint[64], myHint);


	/*If the sourcefile doesn't exist exit program*/
	if(sourceFile == NULL){
		printf("Source file not found.\n");
		return;
	}


	/*Read first line of sourcefile*/
	size = fread(string, sizeof(char), 256,  sourceFile);
		


	/*ENCRYPT MESSAGE WITH BITWISE XOR*/
	for(x=0; x < size; x++){
		/*Key repeats itself once every letter has been used*/
		if(i == strlen(key2)){
			i = 0;
		}

		message[x] = (key2[i] ^ string[x]);

		i++;

	}

	/*Allocate memory and add string TAG into tag struct*/
	myTag.t[4] = malloc(sizeof(char)*(strlen("TAG")+1));
	strcpy(myTag.t[4], "TAG");
	/*Allocate memory and add time stamp to tag struct*/
	myTag.time[26] = malloc(sizeof(char)*strlen(encryptTime));
	strcpy(myTag.time[26], encryptTime);
	
	/*Concatenate the magic number, encrypted message, and tag into 1 string*/
	strncat(number, message, size);
	strncat(number, myTag.t[4], 3);
	strcat(number, myTag.hint[64]);
	strcat(number, myTag.time[26]);

	/*Put encrypted message into output file*/
	fwrite(number, 1, strlen(number), outputFile);
	printf("Done.\n");

}

/*This function decrypts a message from a specified file using an entered key and writes it to a specified output file*/
void decrypt(FILE *sourceFile, FILE *outputFile, char *key[]){
	
	/*Holds string to be decrypted*/
	char *string;
	/*Holds string without tage*/
	char message[256];
	/*Holds decrypted message*/
	char decrypted[256];

	/*Variables used for loops and counting*/
	int x, i;
	/*Stores the key*/
	char key2[sizeof(key)];
	/*Stores length of message to be decrypted*/
	int length;
	/*Used to check that file has magic number*/
	char magicNum[5] = "0449";
	/*Used to check file has a tag*/
	char tag[4] = "TAG";

	string = malloc(sizeof(char)*256);
	/*Place key into key2*/
	strcpy(key2, *key);


	/*If the sourcefile doesn't exist exit program*/
	if(sourceFile == NULL){
		printf("Source file not found.\n");
		return;
	}

	/*Read first line of sourcefile*/
	fread(string, sizeof(char), 256,  sourceFile);

	/*Check to see if the file is encrypted*/
	if(strncmp(magicNum, string, 4) != 0){
		printf("Source file is not an encrypted 0449 file.\n");
		return;
	}
		

	/*Check to see if the file has a tag*/
	if(strstr(string, tag) == NULL){
		printf("No tag was found.\n");
		return;
	}
	
	/*Get message from file*/
	length = strcspn(string, tag);
	
	/*Place string without tag into message string*/	
	memmove(message, string, length);

	/*DECRYPT MESSAGE WITH BITWISE XOR*/
	for(x=4; x < length; x++){
		/*Key repeats itself once every letter has been used*/
		if(i == strlen(key2)){
			i = 0;
		}
		
		decrypted[x-4] = (key2[i] ^ message[x]);

		i++;

	}


	/*Put encrypted message into output file*/
	fwrite(decrypted, 1, length-4, outputFile);
	printf("Done.\n");

}




/*Main function*/
int main(int argc, char *argv[]){

	/*print usage*/
	if(argc == 1){
		usage();
		return 0;
	}

	/*Encryption*/
	if(argv[1][1] == 'e'){

		/*Get specified source file*/
		FILE *sourceFile = fopen(argv[2], "rb");

		/*Get specified output file*/
		FILE *outputFile = fopen(argv[3], "wb");

		/*Holds key for decryption*/
		char *key = argv[4];
		/*Holds hint*/
		char *hint = argv[5];

		encrypt(sourceFile, outputFile, &key, &hint);

		return 0;
	}
	else{

		/*Decryption*/
		if(argv[1][1] == 'd'){
		
			/*Get specified source file*/
			FILE *sourceFile = fopen(argv[2], "rb");
			/*Get specified output file*/
			FILE *outputFile = fopen(argv[3], "wb");

			/*Holds key for decryption*/
			char *key = argv[4];
		
			decrypt(sourceFile, outputFile, &key);

			return 0;
		}
		else{

			/*Reading*/
			if(argv[1][1] == 'r'){

				/*Get specified source file*/
				FILE *sourceFile = fopen(argv[2], "rb");

				printTag(sourceFile);

				return 0;
			}
			else{
				usage();
				return 0;
				
			}
		}
	}



		
}



