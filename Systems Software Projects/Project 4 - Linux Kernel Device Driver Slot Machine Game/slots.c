/*NAME: Ben Isenberg*/
/*DATE: 7/28/11*/


/*This is a program that implements a slot machine that the user can spin
as if he were in a casino.  User starts with 10 coins and he can win more if he
is lucky and gets spins 3 of the same symbol.  If he runs out of coins, the game ends.  
The spins are random and determined by reading from a char device driver in a linux 
kernel called /dev/slotmachine*/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

/*Socked file descriptor*/
int fd;

/*First slot machine reel*/
int spin1(){

	/*A number 0-6 representing a random spin*/
	char num[20];

	/*Open the device driver for use*/
	fd = open("/dev/slotmachine",O_RDONLY);

	/*Random number is placed in num[0]*/
	unsigned char spin = read(fd, num, 100);

	/*Display what symbol the user spun*/
	switch(num[0]){
		case 0: printf("Blank\n");
				return 0;
		case 1: printf("Cherry\n");
				return 1;
		case 2: printf("Orange\n");
				return 2;
		case 3: printf("Plum\n");
				return 3;
		case 4: printf("Bell\n");
				return 4;
		case 5: printf("Bar\n");
				return 5;
		case 6: printf("Seven\n");
				return 6;
		default: printf("Invalid number generated!\n");
				 return -9999;
	}

}

/*Second slot machine reel*/
int spin2(){

	/*A number 0-6 representing a random spin*/
	char num[20];
	
	/*Open the device driver for use*/
	fd = open("/dev/slotmachine",O_RDONLY);
	
	/*Random number is placed in num[0]*/
	unsigned char spin = read(fd, num, 100);

	/*Display what symbol the user spun*/
	switch(num[0]){
		case 0: printf("Blank\n");
				return 0;
		case 1: printf("Cherry\n");
				return 1;
		case 2: printf("Orange\n");
				return 2;
		case 3: printf("Plum\n");
				return 3;
		case 4: printf("Bell\n");
				return 4;
		case 5: printf("Bar\n");
				return 5;
		case 6: printf("Seven\n");
				return 6;
		default: printf("Invalid number generated!\n");
				 return -9999;
	}

}

/*Third slot machine reel*/
int spin3(){

	/*A number 0-6 representing a random spin*/
	char num[20];

	/*Open the device driver for use*/
	fd = open("/dev/slotmachine",O_RDONLY);
	
	/*Random number is placed in num[0]*/
	unsigned char spin = read(fd, num, 100);

	/*Display what symbol the user spun*/
	switch(num[0]){
		case 0: printf("Blank\n");
				return 0;
		case 1: printf("Cherry\n");
				return 1;
		case 2: printf("Orange\n");
				return 2;
		case 3: printf("Plum\n");
				return 3;
		case 4: printf("Bell\n");
				return 4;
		case 5: printf("Bar\n");
				return 5;
		case 6: printf("Seven\n");
				return 6;
		default: printf("Invalid number generated!\n");
				 return -9999;
	}

}


int main(int argc, char *argv[]){
	/*Variables*/
	int a, b, c;
	int x = 0;
	
	/*User's coins*/
	int coins = 10;
	
	/*The string 'yes'*/
	char yes[3];
	yes[0] = 'y';
	yes[1] = 'e';
	yes[2] = 's';

	/*The string 'no'*/
	char no[2];
	no[0] = 'n';
	no[1] = 'o';

	printf("Welcome to the casino!\n\n");

	/*Keep game going until user has no more coins*/
	while(coins > 0){

		char data[128];

		printf("You currently have %d coins.\n", coins);

		printf("Would you like to play slots (yes/no)? ");

		/*Get user's response*/
		fgets(data, 128, stdin);

		/*Spin machine if user said yes*/
		if(strncmp(data, yes, 3) == 0){
			
			/*Spin slot machine*/
			printf("The spin:\n");
			a = spin1();
			b = spin2();
			c = spin3();

			/*Check to see if the user got three of a kind*/
			/*Payout coins to the user if he did*/
			if(a == 0 && a == b && b == c){
				printf("\n3 Blanks! You win 1 coin!\n\n");
				coins++;
			}
			if(a == 1 && a == b && b == c){
				printf("\n3 Cherries! You win 2 coins!\n\n");
				coins = coins + 2;
			}
			if(a == 2 && a == b && b == c){
				printf("\n3 Oranges! You win 5 coins!\n\n");
				coins = coins + 5;
			}
			if(a == 3 && a == b && b == c){
				printf("\n3 Plums! You win 10 coins!\n\n");
				coins = coins + 10;
			}
			if(a == 4 && a == b && b == c){
				printf("\n3 Bells! You win 100 coins!\n\n");
				coins = coins + 100;
			}
			if(a == 5 && a == b && b == c){
				printf("\n3 Bars! You win 1000 coins!\n\n");
				coins = coins + 1000;
			}
			if(a == 6 && a == b && b == c){
				printf("\n3 Sevens! You win 10000 coins!\n\n");
				coins = coins + 10000;
			}

		}
		else{
			/*Exit loop if user said no*/
			if(strncmp(data, no, 2) == 0){
				break;
			}
			else{
				printf("Invalid command.\n");
				
				continue;
			}

		}
		/*Take a coin from user for spinning the slot machine*/
		coins--;
	}

	/*Tell user he ran out of coins*/
	if(coins == 0){
		printf("Sorry but you ran out of coins :(\n");
	}

	/*Tell user how many coins he has left*/
	printf("You leave the casino with %d coins\n", coins);

	/*Close device driver*/
	close(fd);
	
	return 0;
}


