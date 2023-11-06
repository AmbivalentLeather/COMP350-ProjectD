/* Written by: Nick Young, Chase Simao, starting code from Joe Matta
 * Date: October 2023 */

void printString(char*);
void printChar(char*);
char* readString(char*);
void readSector(char*, int);
void handleInterrupt21(int ax, char* bx, int cx, int dx);
/* Step 1 */
void readFile(char* filename, char* output_buffer, int* sectorsRead);
int stringCompare(char* directory_buffer, int* file_entry, char* string_to_beat);
/* Step 2 */
void executeProgram(char* name);
/* Step 3 */
void terminate();

void main()
{
//	/*
	makeInterrupt21();
	interrupt(0x21, 4, "tstpr2", 0, 0);
	// */

 	/*	Step 1 test code
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
        	case 4: executeProgram(bx);
            		break;
		case 5: terminate();
			break;
		default: printString("Error AX is invalid");
			break;
	}
}

void readFile(char* filename, char* output_buffer, int* sectorsRead)
{	
	char directory_buffer[512];
	int i = 0;

	int file_entry = 0;
	int* pfile_entry;
	pfile_entry = &file_entry;

	// Reads directory (sector 2) into directory buffer
	readSector(directory_buffer, 2);

	// Checks if filename exists in directory
	if(stringCompare(directory_buffer, pfile_entry, filename) == 1){
		// Reads the sectors with filename file into given output_buffer
		while(directory_buffer[*pfile_entry + i] != 0) {
			readSector(output_buffer, directory_buffer[*pfile_entry + 6 + i]);
			output_buffer += 512;
			++*sectorsRead;
			++i;
		}
       	} else
		*sectorsRead = 0;
}

int stringCompare(char* directory_buffer, int* file_entry, char* filename_to_beat)
{
	int correct_letters = 0;
	int i = 0;

	// Check every line in directory_buffer, incrementing 32 to move to the next line
	for (*file_entry = 0; *file_entry < 512; *file_entry += 32){
		// Compare the first 6 characters to the given filename_to_beat
		while(i < 6){
			if(directory_buffer[*file_entry + i] != filename_to_beat[i])
				break;
			else 
				++correct_letters;
			++i;
		}
		// Return true if all characters match, this works regardless of string length
		if(correct_letters == 6)
			return 1;
		else
			;	// Pass this loop
	}
	// Base case, if the loop above finds nothing, return false
	return 0;
}


void executeProgram(char* program_name)
{
    	char buffer[13312];
    	int sectorsRead;
	int j = 0;

	// Read program_name into buffer
    	readFile(program_name, buffer, &sectorsRead);

    	for (j = 0; j < sectorsRead * 512; j++) { 
        	// putInMemory(int segment, int address, char character)
         	putInMemory(0x2000, j, buffer[j]); 
    	}

	launchProgram(0x2000); // will not return, sets of registers and jumps to the program located at 0x2000
}

void terminate()
{
	while(1);
}
