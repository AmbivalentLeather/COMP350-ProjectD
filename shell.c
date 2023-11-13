/* Program to run the shell
 * Step 4 of Project C
 */
void type(char* inputFileName);
void exec(char* inputFileName);
void findFileName(char* given_string, char* fileName);
void findCommandName(char* inputString, char* outputString);
int stringCompare(char* given, char* compared_to);

int main()
{
	char* userInput;
	char testString[4];
	char* fileName = "None";

	char* cmdType = "type";
	char* cmdExec = "exec";
	
	syscall(0, "C> ");
	syscall(1, userInput);

	findCommandName(userInput, testString);
	
	// Find, in the line given by the user, the filename to look for
	// This is a temporary solution to a slightly larger problem
	findFileName(userInput, fileName);

//	/*

	if (stringCompare(testString, cmdType)) {
		syscall(0, "Running type()\n\r");
		type(fileName);
	}
	if (stringCompare(testString, cmdExec)) {
		syscall(0, "Running exec()\n\r");
		exec(fileName);
	}

	// */

	syscall(5);
		
}

void type(char* inputFileName)
{
	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	syscall(0, "AAAAAAAAA");
	syscall(0, "\n\r");

	// /*
	if (sectorsRead > 0) {
		syscall(0, buffer);
	}	
	else {
		syscall(0, "File not found.\r\n");
	}
	syscall(0, "\n\r");
	// */
	syscall(5);
}

void exec(char* inputFileName)
{
	syscall(0, "BBBBBBBBB");
	syscall(0, "\n\r");
	syscall(4, inputFileName);
	syscall(0, "\n\r");
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

int stringCompare(char* given, char* compared_to)
{
	syscall(0, "STRING COMPARE WORKS");
	syscall(0, "\n\r");
	// Loop through each character in given strings
	while (*given && *compared_to) {
		// If the characters are not equal, strings are not equal
		if (*given != *compared_to) {
			return 0;
		}
		
		// Move to the next character in each string
		given++;
		compared_to++;
	}

	// Check if both strings have reached an end, return 1 if true
	return (*given == '\0' && *compared_to == '\0');
}

