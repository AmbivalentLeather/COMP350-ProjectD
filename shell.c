/* Program to run the shell
 * Step 4 of Project C
 */
void type(char* inputFileName);
void findFileName(char* given_string, char* fileName);

int main()
{
	char* given_string;
	char testString[4];
	char* cmdType = "type";

	char* fileName;

	int i = 0;

	while(1){
		syscall(0, "C> ");
		syscall(1, given_string);

		for (i = 0; i < 4; i++)
			testString[i] = given_string[i];
		
		// Find, in the line given by the user, the filename to look for
		// This is a temporary solution to a slightly larger problem
		findFileName(given_string, fileName);
		type(fileName);

		if(testString == cmdType){
			syscall(0, "raaaaaaaaaaa");
		}

		syscall(5);
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

void findFileName(char* given_string, char* fileName)
{
	int i = 0;
	while(i < 79) {
		if(given_string[i + 5] == 0x0)
			break;

		fileName[i] = given_string[i + 5];
		++i;
	}
}
