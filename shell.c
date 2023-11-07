/* Program to run the shell
 * Step 4 of Project C
 */

int main()
{
	char* given_string;
	char output_buffer[13312];
	int sectorsRead;
	while(1){
		syscall(0, "C> ");
		syscall(1, given_string);
		syscall(0, given_string);
		syscall(3, given_string, output_buffer, &sectorsRead);
		
		// /*
		if (sectorsRead == 0)
			syscall(0, "Bad Command");
		else
			syscall(4, given_string);
		// */

		syscall(5);
	}
}

void commandChecker(char* pGiven_String)
{
	int i = 0;
	char command[4];
	char* type[4];
	type[0] = "t";
	type[1] = "y";
	type[2] = "p";
	type[3] = "e";

	while(i < 4) {
		command[i] = pGiven_String[i];
		i++;
	}
	
	for (i = 0; i < 4; i++) {
		if(type[i] == command[i])
			;
		else
			break;
	}
}
