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
