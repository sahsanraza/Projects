//Name: Ben Isenberg
//Email: bji6@pitt.edu
//Date: 1/28/12

/*This program is an implementation of a simple UNIX-like shell.  It can run UNIX commands, change the current working directory, redirect 
output to a specified file, and pipe one command into another.  It handles multiple pipes but not multiple redirections or any combination 
of output redirection and piping.  Also it exits when 'exit' is typed into the terminal.*/


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>		//For strlen() function	

extern char **getline();

main() {
  int i;
  char **args; 
  
  //flag used to say whether command uses piping or not
  int num_of_pipes = 0;

  int loop_num = -1;

  while(1){
	loop_num++;

    args = getline();
	
	//reset number of pipes value
	num_of_pipes = 0;

	//If ctrl-D is typed with a command then exit shell after executing command
	if((loop_num > 0 && feof(stdin) != 0)){
		exit(1);
	}
	
	//EXIT internal shell command
	if(strlen(args[0]) == 4){
		//if first argument is exit then exit the shell program
		if(args[0][0] == 'e' && args[0][1] == 'x' && args[0][2] == 'i' && args[0][3] == 't'){
			_exit(1);
		}
	  }

	//CHANGE DIRECTORY internal shell comand
	if(strlen(args[0]) == 2){
		//if first argument is ch then call change directory syscall and go to specified directory
		if(args[0][0] == 'c' && args[0][1] == 'd'){
			//chdir() return value		
			int return_val;

			return_val = chdir(args[1]);
			//Error handling
			if(return_val == -1){
				fprintf(stderr, "Error occured in an attempt to change directories, Error Number: %d\n", errno);
			}
			
			continue;
		}

	}

	//Counter
	int x = 0;

	//For error handling
	int closeval;
	int dup2val;

	//Make the array to hold each pipe command
	char *cmd1[x];

	//Count the number of pipes in the command
	for(i = 0; args[i] != NULL; i++) {

		if(args[i][0] == '|'){
			num_of_pipes++;
			continue;
		}
    }

	//Do this if command requires piping
	if(num_of_pipes > 0){

		//array of indexes of pipes in args command list
		int pipe_indexes[num_of_pipes];

		//Initialize first value to zero
		pipe_indexes[0] = 0;

		x = 1;
		
		//Enter the index values for each pipe in the command list into the pipe_indexes array
		for(i = 0; args[i] != NULL; i++) {
			if(args[i][0] == '|'){
				pipe_indexes[x] = i;
				x++;
			}
		}

		//Process id for forking
		int pid;
		
		//Keep two arrays of file descriptors and alternate between them for multiple pipe commands
		int new_fd[2];
		int old_fd[2];
		
		//Counters
		i = 0;
		x = 0;
		int counter;

		//Loop (number of pipes + 1)
		for(counter = 0; counter <= num_of_pipes; counter++){

			//Reset x so that args from previous pipe are overwritten by args for next pipe
			x = 0;
			//get args for the next pipe
			while(1){
				//if you reached end of args list or a pipe -> break
				if(args[i] == NULL || args[i][0] == '|'){
					i++;
					break;
				}
				
				cmd1[x] = args[i];
				i++;
				x++;
			}
			//Make array null terminating so it works with execvp() syscall
			cmd1[x] = NULL;

			//If you are not on the last loop then make a new pipe
			if(counter < num_of_pipes){
				int pipeval = pipe(new_fd);
				//Error handling
				if(pipeval == -1){
					fprintf(stderr, "pipe() Error:%d\n", errno);
				}
			}

			pid = fork();

			if(pid == -1){
				fprintf(stderr, "Fork() Error:%d\n", errno);
			}

			//Child code
			if(pid == 0){

				//Do this only if a previous pipe has been written to by a child process in the past
				//Read data from old pipe to use as input for executing next command
				if(counter > 0){
					//Close write end of old pipe, wont need it
					closeval = close(old_fd[1]);
					//Error handling
					if(closeval == -1){
						fprintf(stderr, "close() Error:%d\n", errno);
					}
					//Take as input data from the read end of the old pipe
					dup2val = dup2(old_fd[0], 0);
					//Error handling
					if(dup2val == -1){
						fprintf(stderr, "dup2() Error:%d\n", errno);
					}
					//Close read end of old pipe
					closeval = close(old_fd[0]);
					//Error handling
					if(closeval == -1){
						fprintf(stderr, "close() Error:%d\n", errno);
					}
				}

				//Do this for every loop except for the last one
				//Prepare to write output into the new pipe
				if(counter < num_of_pipes){
					//close read end of new pipe, dont need it
					closeval = close(new_fd[0]);
					//Error handling
					if(closeval == -1){
						fprintf(stderr, "close() Error:%d\n", errno);
					}
					//redirect output into the write end of the new pipe
					dup2val = dup2(new_fd[1], 1);
					//Error handling
					if(dup2val == -1){
						fprintf(stderr, "dup2() Error:%d\n", errno);
					}
					//close write end of new pipe
					closeval = close(new_fd[1]);
					//Error handling
					if(closeval == -1){
						fprintf(stderr, "close() Error:%d\n", errno);
					}
				}
				
				//Execute command and die if sucessful
				execvp(cmd1[0], cmd1);
				
				//If child process doesn't die, print error number and manually kill child
				fprintf(stderr, "execvp() Error:%d\n", errno);
				exit(1);
					
			}
			//Parent code
			else{
				//Do this for every loop except the first loop
				//The old pipe is not in use yet at the first loop
				if(counter > 0){
					//Closes both ends of the old pipe
					closeval = close(old_fd[0]);
					//Error handling
					if(closeval == -1){
						fprintf(stderr, "close() Error:%d\n", errno);
					}
					closeval = close(old_fd[1]);
					//Error handling
					if(closeval == -1){
						fprintf(stderr, "close() Error:%d\n", errno);
					}
				}
				
				//Do this for every loop but the last
				//Don't need this on last loop because the last child process displays its results 
				//to the terminal, so no new data will be written into a new pipe
				if(counter < num_of_pipes){
					//The old pipe = the last new pipe used by a child process
					//store data outputted by last child process into the old pipe for the next child to read in
					old_fd[0] = new_fd[0];
					old_fd[1] = new_fd[1];

				}
				
				//On last loop only
				//Wait for last child to display its results to the terminal and then exit loop 
				if(counter == num_of_pipes){
					int status;
					int waitval;
					waitval = wait(&status);
					//Error handling
					if(waitval == -1){
						fprintf(stderr, "wait() Error:%d\n", errno);
					}
					break;
				}
			}
		}
	}

	//For non-piping commands
	if(num_of_pipes == 0){
		//UNIX commands with or without arguments
		pid_t process_ID;
		  
		process_ID = fork();
		//Error occured, child process was not created!
		if(process_ID == -1){
			fprintf(stderr, "Error occured, no child process was created. Error number: %d\n", errno);
			exit(1);
		}
			
		//Child process code
		if(process_ID == 0){
			//variable that stores file opening mode
			char mode = 'w';
			//pointer to mode
			char *point = &mode;
			int index = 0;
			//return value for execvp()
			int re_val_exec;
			//return value for freopen()
			int re_val_fopen;
			
			for(i = 0; args[i] != NULL; i++) {
				//Get filename for output redirection as long as one is specified by user
				if(args[i][0] == '>' && args[i+1] != NULL){
					index = i;
					//syscall for output redirection
					re_val_fopen = freopen(args[i+1], point, stdout);
					//Error handling
					if(re_val_fopen == NULL){
						fprintf(stdout, "Error occured during output redirection, exiting child process. Error num:%d\n", errno);
						_exit(1);
					}
				}
				//Exit if no file name is given for redirection
				if(args[i][0] == '>' && args[i+1] == NULL){
					fprintf(stdout, "No file name specified for output redirection, exiting child process\n");
					_exit(1);
				}
			}
		
			//Only do this if redirection is involved in command
			if(index > 0){
				//Get rid of '>' argument and following args before executing command
				for(i = 0; args[i] != NULL; i++){
					if(i >= index){
						args[i] = NULL;
					}
				}
			}

			re_val_exec = execvp(args[0], args);
			//Error handling
			if(re_val_exec == -1){
				fprintf(stdout, "Error occured, manually exiting child process. Error num:%d\n", errno);
				_exit(1);
			}


		}
		//Parent process code
		else{
			int status;
			//wait() return value
			int re_val_wait;
			//Wait until the status of child process has changed
			re_val_wait = wait(&status);
			//Error handling
			if(re_val_wait == -1){
				fprintf(stdout, "Error occured during wait, Error num:%d\n", errno);
				_exit(1);
			}

			continue;
		}
	}

//End of while loop
}	
  return 0;
}
