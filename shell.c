/* Program to run the shell
 * Step 4 of Project C
 */
void type(char* inputFileName);
void exec(char* inputFileName);
void findFileName(char* given_string, char* fileName);

int main()
{
	char* given_string;
	char testString[4];
	char* fileName = "None";

	char* cmdType = "type";
	char* cmdExec = "exec";
	
	int i = 0;

	while(1){
		syscall(0, "C> ");
		syscall(1, given_string);

		for (i = 0; i < 4; i++) {
			testString[i] = given_string[i];
		}
		
		// Find, in the line given by the user, the filename to look for
		// This is a temporary solution to a slightly larger problem
		findFileName(given_string, fileName);

//		/*
		if (syscall(6, testString, cmdType) != 0) {	// Should return false when testString == cmdExec
			type(fileName);
			syscall(5);
		}
		else if (syscall(6, testString, cmdExec) != 0) {
			exec(fileName);
			syscall(5);
		}
		// */
		syscall(5);
		
	}
}

void type(char* inputFileName)
{
	char buffer[13312]; /*this is the maximum size of a file*/
	int sectorsRead;
	syscall(3, inputFileName, buffer, &sectorsRead);

	syscall(0, "AAAAAAAAA");

	// /*
	if (sectorsRead > 0) {
		syscall(0, buffer);
		syscall(5);
	}	
	else {
		syscall(0, "File not found.\r\n");
		syscall(5);
	}
	// */
}

void exec(char* inputFileName)
{
	syscall(0, "BBBBBBBBB");
	syscall(4, inputFileName);
	syscall(5);
}

void findFileName(char* given_string, char* fileName)
{
	int i = 0;
	while(i < 12) {
		fileName[i] = given_string[i + 5];
		++i;
	}
}
