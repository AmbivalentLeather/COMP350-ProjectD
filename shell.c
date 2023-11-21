/* Program to run the shell
 * Step 4 of Project C
 * Nicholas Young & Chase Simao
 */
void type(char* inputFileName);
void exec(char* inputFileName);
int stringCompare(char given[], char goal[]);
void argFinder(char*, char*, int);
int numberOfArguments(char* input);
void argErrorHandler(int givenNumOfArgs, int expectedNumOfArgs);
void dir();
void del(char* address);
void copy(char* file1, char* file2);
void create(char* filename);

#define LINE_SIZE 80
#define SECTOR_SIZE 512
#define MAX_SECTORS 26

/* syscall KEY
 * 0, printString
 * 1, readString
 * 2, readSector
 * 3, readFile
 * 4, executeProgram
 * 5, terminate (return to shell)
 * 6, writeSector
 * 7, deleteFile
 * 8, writeFile
 */

int main()
{
	while(1) {
		char userInput[LINE_SIZE];
		char cmdString[12];
		char arg1[12], arg2[12];

		int argNum;

		char* cmdType = "type";
		char* cmdExec = "exec";
		char* cmdDir = "dir";
		char* cmdDel = "del";
		char* cmdCopy = "copy";
		char* cmdCreate = "create";

		syscall(0, "\rC> ");
		syscall(1, userInput);

		argNum = numberOfArguments(userInput);

		argFinder(userInput, cmdString, 0);
		// NOTE: I want to eventually use numOfArgs to report an error if the user
		// enters more arguments than are expected of the command AND when the user
		// enters more characters in the command than are checked

		if (stringCompare(cmdString, cmdType)) {
			argErrorHandler(argNum, 1);
			argFinder(userInput, arg1, 1);
			type(arg1);
		}
		else if (stringCompare(cmdString, cmdExec)) {
			argErrorHandler(argNum, 1);
			argFinder(userInput, arg1, 1);
			exec(arg1);
		}
		else if (stringCompare(cmdString, cmdDir)) {
            		dir();
        	}
		else if (stringCompare(cmdString, cmdDel)) {
			argErrorHandler(argNum, 1);
			argFinder(userInput, arg1, 1);
			del(arg1);
		}
		else if (stringCompare(cmdString, cmdCopy)) {
			argErrorHandler(argNum, 2);
			argFinder(userInput, arg1, 1);
			argFinder(userInput, arg2, 2);
			copy(arg1, arg2);
		}
		else if (stringCompare(cmdString, cmdCreate)) {
			argErrorHandler(argNum, 1);
			argFinder(userInput, arg1, 1);
			create(arg1);
		}
		else {
			syscall(0, "Bad command!\n\r");
		}
		
	}
		
}

void type(char* inputFileName)
{
	char buffer[SECTOR_SIZE * MAX_SECTORS]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	if (sectorsRead > 0)
		syscall(0, buffer);
	else
		syscall(0, "File not found.\r\n");
}

void exec(char* inputFileName)
{
	char buffer[SECTOR_SIZE * MAX_SECTORS]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	if (sectorsRead > 0)
		syscall(4, inputFileName);
	else
		syscall(0, "File not found.\r\n");
}

void dir()
{
	char directory_buffer[SECTOR_SIZE], file_buffer[12];
	int i = 0;

	int file_size = 0;
	int file_entry = 0;

	syscall(2, directory_buffer, 2);

	for (file_entry = 0; file_entry < SECTOR_SIZE; file_entry += 32){
		if (directory_buffer[file_entry] != '\0') {
			while(i < 6){
				file_buffer[i] = directory_buffer[file_entry + i];
				i++;
			}
			/* The problem with this part is that I don't know how to print a number.
			 * Also, this would only print up the size of all the sectors, not size of file
			 */
			while (directory_buffer[file_entry + i] != '\0') {
				file_size += SECTOR_SIZE;
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
	char buffer[SECTOR_SIZE * MAX_SECTORS]; /*this is the maximum size of a file*/
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
	char buffer[SECTOR_SIZE * MAX_SECTORS]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, file1, buffer, &sectorsRead);
	
	if (sectorsRead > 0)
		syscall(8, buffer, file2, sectorsRead);
	else
		syscall(0, "File not found.\r\n");
}

void create(char* filename)
{
	char stringStore[LINE_SIZE];
	char buffer[SECTOR_SIZE * MAX_SECTORS];
	int sectorNumber = 1;
	int sectorIndex = 0;
	int i;

	// We need to continue reading a line until the user doesn't enter anything
	while (1) {
		syscall(1, stringStore);

		if (stringStore[0] == '\r')
			break;

		// Iterate through the line until the carraige return
		for (i = 0; stringStore[i] != '\r'; i++) {
			buffer[sectorIndex] = stringStore[i];
			sectorIndex++;
		}

		// Add new line characters so the lines are actually lines
		buffer[sectorIndex] = '\r';
		buffer[sectorIndex + 1] = '\n';
		sectorIndex += 2;

		// If we've read 512 characters, increment the sectorIndex
		if (sectorIndex % SECTOR_SIZE)
			sectorNumber++;
	}
	syscall(8, buffer, filename, sectorNumber);
}

// I don't like that we're mixing [] and * since in this context they mean the same thing
int stringCompare(char given[], char goal[])
{
	int i = 0;
	/*
	int givenLength, goalLength;

	for (i = 0; given[i] != '\0'; i++)
		givenLength++;
	for (i = 0; goal[i] != '\0'; i++)
		goalLength++;

	// If the two strings are different lengths, return 0
	if (givenLength != goalLength)
		return 0;
	*/

	// If any characters don't match, return 0
	for (i = 0; given[i] != '\0' && goal[i] != '\0'; i++)
		if (given[i] != goal[i])
			return 0;

	return 1;
}


int numberOfArguments(char* input)
{
	int argumentIndex, args;
	for (argumentIndex = 0; input[argumentIndex] != '\0'; argumentIndex++)
		if (input[argumentIndex] == ' ')
			++args;
	return args;
}

void argErrorHandler(int givenNumOfArgs, int expectedNumOfArgs)
{
	// Prints "Too many arguments" every time for some reason
	if (givenNumOfArgs > expectedNumOfArgs)
		syscall(0, "Too many arguments\r\n");
	else if (givenNumOfArgs < expectedNumOfArgs)
		syscall(0, "Not enough arguments\r\n");
	syscall(5);
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

		if (input[inputIndex] == '\r') {
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

