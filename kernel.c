/* Written by: Nick Young, Chase Simao, starting code from Joe Matta
 * Date: October 2023 */

void printString(char*);
void printChar(char*);
char* readString(char*);
void readSector(char*, int);
void handleInterrupt21(int ax, char* bx, int cx, int dx);
void readFile(char* filename);
int string_matcher(char* directory_buffer, int current_string, char* string_to_beat);

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

	/*
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

	// interrupt(0x13, AX, BX, CX, DX);
	interrupt(0x13, AX, BX, CX, DX);  

	// return address; // I don't think this needs to return anything at all
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
		default: printString("Error AX is invalid");
			 break;
	}
}

void readFile(char* filename)
{	
	int file_entry = 0;
	char* file_storage_buffer[512];
	char* directory_buffer[512];
	int* sectors_read;

	int AX = 3;
	int BX =  &filename;
	int CX = &file_storage_buffer;
	int DX = &sectors_read;
	
	readSector(*directory_buffer, 2);

	if(string_matcher(*directory_buffer, file_entry, filename)){
		int i;
		for (i = 0; directory_buffer[file_entry + 6 + i] != 0; i++) {
			// WHAT FUCKING BUFFER SHOULD I WRITE TO? CX???
			readSector(CX , directory_buffer[file_entry + 6 + i]);
                	CX += 512;
          	} 
       	} 
	
}

int string_matcher(char* directory_buffer, int file_entry, char* string_to_beat)
{
	int hope = 0;
	for (file_entry = 0; file_entry < 512; file_entry += 32){
		// /*
		int i;
		for(i = 0; i < 5; ++i) {
			if(directory_buffer[file_entry + i] != string_to_beat[i])
				break;
			else
				++hope;
		}
		// */
		if(hope == 6)
			return 1;
		else
			;
	}

}
