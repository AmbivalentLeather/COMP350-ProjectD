/* Program to run the shell
 * Step 4 of Project C
 * Nicholas Young & Chase Simao
 */
void type(char* inputFileName);
void exec(char* inputFileName);
int stringCompare(char* given, char* compared_to);
void argFinder(char*, char*, int);
void numOfArgs(char*, int*);
void dir();
void del(char* address);
void copy(char* file1, char* file2);

int main()
{
	while(1) {
		char userInput[80];
		char cmdString[10];
		char fileName[12];
		char fileName1[12], fileName2[12];

		char* cmdType = "type";
		char* cmdExec = "exec";
		char* cmdDir = "dir";
		char* cmdDel = "del";
		char* cmdCopy = "copy";
		
		syscall(0, "\rC> ");
		syscall(1, userInput);
	
		argFinder(userInput, cmdString, 0);
		// NOTE: I want to eventually use numOfArgs to report an error if the user
		// enters more arguments than are expected of the command AND when the user
		// enters more characters in the command than are checked

		if (stringCompare(cmdString, cmdType)) {
			argFinder(userInput, fileName, 1);
			type(fileName);
		}
		else if (stringCompare(cmdString, cmdExec)) {
			argFinder(userInput, fileName, 1);
			exec(fileName);
		}
		else if (stringCompare(cmdString, cmdDir)) {
            		dir();
        	}
		else if (stringCompare(cmdString, cmdDel)) {
			argFinder(userInput, fileName, 1);
			del(fileName);
		}
		else if (stringCompare(cmdString, cmdCopy)) {
			argFinder(userInput, fileName1, 1);
			argFinder(userInput, fileName2, 2);
			copy(fileName1, fileName2);
		}
		else {
			syscall(0, "Bad command!\n\r");
		}
		
	}
		
}

void type(char* inputFileName)
{
	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	if (sectorsRead > 0)
		syscall(0, buffer);
	else
		syscall(0, "File not found.\r\n");
}

void exec(char* inputFileName)
{
	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	if (sectorsRead > 0)
		syscall(4, inputFileName);
	else
		syscall(0, "File not found.\r\n");
}

void dir()
{
	char directory_buffer[512];
	char file_buffer[10];
	int i = 0;

	int file_size = 0;

	int file_entry = 0;

	syscall(2, directory_buffer, 2);

	for (file_entry = 0; file_entry < 512; file_entry += 32){
		if (directory_buffer[file_entry] != '\0') {
			while(i < 6){
				file_buffer[i] = directory_buffer[file_entry + i];
				i++;
			}
			/* The problem with this part is that I don't know how to print a number.
			 * Also, this would only print up the size of all the sectors, not size of file
			 */
			while (directory_buffer[file_entry + i] != '\0') {
				file_size += 512;
				//syscall(0, "H");
				i++;
			}
			//syscall(0, "\n\r");
			// */
			syscall(0, file_buffer);
			//syscall(0, "\t");
			//syscall(0, 30);
			syscall(0, "\n\r");
			i = 0;
		}
		// file_size = 0;
	}

}

void del(char* filename)
{
	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, filename, buffer, &sectorsRead);

	if (sectorsRead > 0) {
		syscall(7, filename);
		syscall(0, "File deleted.\n\r");
	}
	else
		syscall(0, "File not found.\r\n");

}

void copy(char* file1, char* file2)
{
	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, file1, buffer, &sectorsRead);
	
	if (sectorsRead > 0)
		syscall(8, buffer, file2, sectorsRead);
	else
		syscall(0, "File not found.\r\n");
}

// I don't like that we're mixing [] and * since in this context they mean the same thing
int stringCompare(char given[], char* compared_to)
{
	int i = 0;
	while(given[i] != '\0' && compared_to[i] != '\0') {
		if (given[i] != compared_to[i])
			return 0;
		i++;
	}

	return 1;
}

void numOfArgs(char* input, int* args)
{
	int argumentIndex;
	*args = 0;
	for (argumentIndex = 0; input[argumentIndex] != '\0'; argumentIndex++)
		if (input[argumentIndex] == ' ')
			++*args;
}

void argFinder(char* input, char* output, int whichArg)
{
	// outputIndex is needed alongside inputIndex for this function to work
	int argumentIndex = 0;
	int outputIndex = 0;
	int inputIndex = 0;

	while (input[inputIndex] != '\0') {
		if (input[inputIndex] == ' ') {
			argumentIndex++;
			inputIndex++;
			continue;
		}

		if (whichArg == argumentIndex) {
			output[outputIndex] = input[inputIndex];
			outputIndex++;
		}

		inputIndex++;
	}

	output[outputIndex] = '\0';
}

