/* Program to run the shell
 * Step 4 of Project C
 * Nicholas Young & Chase Simao
 */
void type(char* inputFileName);
void exec(char* inputFileName);
int stringCompare(char* given, char* compared_to);
void argFinder(char*, char*, int);
void numOfArgs(char*, int*);

int main()
{
	while(1) {
		char userInput[80];
		char cmdString[10];
		char fileName[12];

		char* cmdType = "type";
		char* cmdExec = "exec";
		
		syscall(0, "\rC> ");
		syscall(1, userInput);
	
		argFinder(userInput, cmdString, 0);
		// NOTE: I want to eventually use numOfArgs to report an error if the user
		// enters more arguments than are expected of the command

		if (stringCompare(cmdString, cmdType)) {
			argFinder(userInput, fileName, 1);
			type(fileName);
		}
		else if (stringCompare(cmdString, cmdExec)) {
			argFinder(userInput, fileName, 1);
			exec(fileName);
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
