/* Written by: Nick Young, Chase Simao, starting code from Joe Matta
 * Date: October 2023 */

/* K&R Functions */
void itoa(int n, char s[]);

/* Function initialization */
void printString(char*);
void printChar(char*);
char* readString(char*);
void readSector(char*, int);
void handleInterrupt21(int ax, char* bx, int cx, int dx);

void readFile(char* filename, char* output_buffer, int* sectorsRead);
int string_compare(char* directory_buffer, int* file_entry, char* string_to_beat);


void main()
{
	/* Final test code for projB
	char line[80];
	makeInterrupt21();
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);	
	// */

	/* Old code for testing
	char* letters = "Enter a string: \0";
	char input[80];
	
	interrupt(0x21, 0, 0, 0, 0);

	printString(letters);
	readString(input);
	printString(input);

	char buffer[512];
	readSector(buffer, 30);
	printString(buffer);
	// */

	// /*
	char str[1];
	alph = (9, str);
	char buffer[13312];   //this is the maximum size of a file
	int sectorsRead;
	makeInterrupt21(); 
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);   //read the file into buffer 
	if (sectorsRead>0)
		interrupt(0x21, 0, buffer, 0, 0);   //print out the file 
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);  //no sectors read? then print an error
	// */

	while(1);	// PLEASE DON'T CHANGE THIS LINE
}

void printString(char* chars)
{	
	int i = 0;
	while (chars[i] != 0x0) {
		char al = chars[i];
		char ah = 0xe;
		int ax = ah * 256 + al;
		interrupt(0x10, ax, 0, 0, 0);
		++i;
	}
}

void printChar(char* inputChar)
{
	char al = inputChar[0];
	char ah = 0xe;
	int ax = ah * 256 + al;
	interrupt(0x10, ax, 0, 0, 0);
}

char* readString(char* inputArray)
{
	char keyboardInput = 0x0;

	int i = 0;
	while (i != 79) { // While the index is below 80
		keyboardInput = (char) interrupt(0x16, 0, 0, 0, 0);

		if (keyboardInput == 0xd)
			// Break out of the loop if the user presses enter
			break;
		if (keyboardInput == 0x8 && i == 0)
			// If the index is 0, don't print backspace
			continue;
		if (keyboardInput == 0x8){
			// Print backspace character, decrement array index
			// We use backspace twice so the character is deleted visually
			char space = ' ';
			interrupt(0x10, 0xe * 256 + keyboardInput, 0, 0, 0);
			interrupt(0x10, 0xe * 256 + space, 0, 0, 0);
			interrupt(0x10, 0xe * 256 + keyboardInput, 0, 0, 0);
			--i;
			continue;
		}
		// If conditions above are met, character prints normally

		inputArray[i] = keyboardInput;
		interrupt(0x10, 0xe * 256 + keyboardInput, 0, 0, 0);
		++i;
	}
	inputArray[i] = 0xa;
	inputArray[i + 1] = 0x0;

	interrupt(0x10, 0xe * 256 + 0xd, 0, 0, 0); // Print return character
	interrupt(0x10, 0xe * 256 + 0xa, 0, 0, 0); // Print line feed character

	return inputArray;
}

void readSector(char* address, int sector)
{
	int AH = 2;	// this number tells BIOS to read a sector as opposed to write
	int AL = 1;	// numbers of sectors to read
	int AX = AH * 256 + AL;

	char* BX = address; // address where the data should be stored to

	int CH = 0;	// track number
	int CL = sector + 1; // relative sector number
	int CX = CH * 256 + CL;

	int DH = 0;	// head number
	int DX = DH * 256 + 0x80;

	interrupt(0x13, AX, BX, CX, DX);  
}

void handleInterrupt21(int ax, char* bx, int cx, int dx)
{
	switch(ax)
	{
		case 0: printString(bx);
			break;
		case 1: readString(bx);
			break;
		case 2: readSector(bx, cx);
			break;
		case 3: readFile(bx, cx, dx);
			break;
		default: printString("Error AX is invalid");
			break;
	}
}

void readFile(char* filename, char* output_buffer, int* sectorsRead)
{	
	char directory_buffer[512];
	int i = 0;
	// I only declare the pointer like this so the logic is more clear
	int file_entry = 0;
	int* pfile_entry;

	pfile_entry = &file_entry;

	readSector(directory_buffer, 2);

	// /*
	if(string_compare(directory_buffer, pfile_entry, filename)){
		printString("String matched\r\n");
		while(directory_buffer[*pfile_entry + i] != 0) {
			printString("About to readSector\r\n");

			readSector(output_buffer, directory_buffer[*pfile_entry + 6 + i]);
			output_buffer += 512;

			printString("readSector to output buffer done\r\n");
			++*sectorsRead;
			++i;
		}
       	} else
		sectorsRead = 0;
	// */
}

int string_compare(char* directory_buffer, int* file_entry, char* string_to_beat)
{
	// /*
	int hope = 0;
	int i;

	for (*file_entry = 0; *file_entry < 512; *file_entry += 32){
		for(i = 0; i < 6; ++i) {
			if(directory_buffer[*file_entry + i] != string_to_beat[i])
				break;
			else 
				++hope;
		}
		if(hope == 6)	// Return true if all characters match
			return 1;
		else
			;	// Pass this loop
	}
	return 0; // Base case, if the loop above finds nothing, return false
	// */

}

/* itoa: convert n to characters in s TAKEN FROM K&R */
void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)  /* record sign */
		n = -n;           /* make n positive */
		i = 0;
	do {  /* generate digits in reverse order */
		s[i++] = n % 10 + '0';  /* get next digit */
	} while ((n /= 10) > 0);   /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}
