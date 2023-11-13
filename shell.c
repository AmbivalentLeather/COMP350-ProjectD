/* Program to run the shell
 * Step 4 of Project C
 */
void type(char* inputFileName);
void exec(char* inputFileName);
void findFileName(char* given_string, char* fileName);
int stringCompareType(char* given, char* compared_to);
int stringCompareExec(char* given, char* compared_to);
void cSpacing(char* file_name);

int main()
{
	char* given_string;
	char testString[4];
	char* fileName = "None";

	char* cmdType = "type";
	char* cmdExec = "exec";
	
	int i = 0;

	syscall(0, "C> ");
	syscall(1, given_string);

	for (i = 0; i < 4; i++) {
		testString[i] = given_string[i];
	}
	
	// Find, in the line given by the user, the filename to look for
	// This is a temporary solution to a slightly larger problem
	findFileName(given_string, fileName);

//	/*
	if (stringCompare(testString, cmdType)) {	// Should return false when testString == cmdExec
		type(fileName);
	} else {
        	exec(fileName);
    	}
//	if (stringCompareExec(testString, cmdExec)) {
//		exec(fileName);
//	}
	// */
	syscall(5);
		
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
	}	
	else {
		syscall(0, "File not found.\r\n");
	}
	// */
	syscall(0, "\n\r");
	syscall(5);
}

void exec(char* inputFileName)
{
	syscall(0, "BBBBBBBBB");
	syscall(0, "\n\r");
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

int stringCompare(char* given, char* compared_to)
{
    syscall(0, "STRING COMPARE WORKS");
	while (*given && *compared_to) {
		if (*given != *compared_to) {
			return 0;
		}
		
		given++;
		compared_to++;

	}

	return (*given == '\0' && *compared_to == '\0');
}








