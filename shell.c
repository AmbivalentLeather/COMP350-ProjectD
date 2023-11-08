/* Program to run the shell
 * Step 4 of Project C
 */

int main()
{
	char* given_string;
	char output_buffer[13312];
	int sectorsRead;
	char testString[4];
	char* stringTester = "type";
	int i = 0;

	while(1){
		syscall(0, "C> ");
		syscall(1, given_string);
		syscall(3, given_string, output_buffer, &sectorsRead);

		while(i < 4) {
			testString[i] = given_string[i];
			i++;
		}
		
		syscall(0, testString);

		if(syscall(6, testString, stringTester)){
			syscall(0, "WOOHOO");
		}

		/*
		if (sectorsRead == 0)
			syscall(0, "Bad Command");
		else
			syscall(4, given_string);
		// */

		syscall(5);
	}
}
