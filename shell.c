/* Program to run the shell
 * Step 4 of Project C
 */
void type(char* inputFileName);
void exec(char* inputFileName);
void findFileName(char userInput[], char* fileName);
void findCommandName(char inputString[], char* outputString);
int stringCompare(char* given, char* compared_to);

int main()
{
	while(1) {
		char userInput[80];
		char cmdString[4];
		char fileName[12];
	
		char* cmdType = "type";
		char* cmdExec = "exec";
		
		syscall(0, "C> ");
		syscall(1, userInput);
	
		// Find the command used and the file name being referenced
		findCommandName(userInput, cmdString);
		findFileName(userInput, fileName);

//		/*
		if (stringCompare(cmdString, cmdType)) {
			type(fileName);
		} else if (stringCompare(cmdString, cmdExec)) {
			exec(fileName);
		} else {
			syscall(0, "Bad command!\n\r");
		}
		// */
	}
		
}

void type(char* inputFileName)
{
	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	// /*
	if (sectorsRead > 0) {
		syscall(0, buffer);
	}	
	else {
		syscall(0, "File not found.\r\n");
	}
	// */

	syscall(5);
}

void exec(char* inputFileName)
{

	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	// /*
	if (sectorsRead > 0) {
		syscall(4, inputFileName);
	}	
	else {
		syscall(0, "File not found.\r\n");
	}
	// */

	syscall(5);
}

void findFileName(char* inputString, char* fileName)
{
	int i = 0;
	while(i < 12) {
		fileName[i] = inputString[i + 5];
		++i;
	}
}

void findCommandName(char* inputString, char* outputString)
{
	int i;
	for (i = 0; i < 4; i++) {
		outputString[i] = inputString[i];
	}
}	

int stringCompare(char given[], char* compared_to)
{
	int i = 0;

	while(given[i] != '\0' && compared_to[i] != '\0') {
		if (given[i] != compared_to[i]) {
			return 0;
		}
		i++;
	}

	return 1;
}

