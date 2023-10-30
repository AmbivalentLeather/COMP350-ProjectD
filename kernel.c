/* Written by: Nick Young, Chase Simao, starting code from Joe Matta
 * Date: October 2023 */

void printString(char*);
void printChar(char*);
char* readString(char*);
char* readSector(char*, int);
void handleInterrupt21(int ax, char* bx, int cx, int dx);
void readFile(char* filename);

void main()
{
	/* Final test code for projB
	char line[80];
	makeInterrupt21();
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);	
	*/

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
	*/

	/*
	char buffer[13312];   //this is the maximum size of a file
	int sectorsRead;
	makeInterrupt21(); 
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);   //read the file into buffer 
	if (sectorsRead>0)
		interrupt(0x21, 0, buffer, 0, 0);   //print out the file 
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);  //no sectors read? then print an error
	while(1);   //hang up
	*/

	//while(1);
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
	char al = inputChar;
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

char* readSector(char* buffer, int sector)
{
	int AH = 2;	// this number tells BIOS to read a sector as opposed to write
	int AL = 1;	// numbers of sectors to read
	int AX = AH * 256 + AL;

	char* BX = buffer; // address where the data should be stored to

	int CH = 0;	// track number
	int CL = sector + 1; // relative sector number
	int CX = CH * 256 + CL;

	int DH = 0;	// head number
	int DX = DH * 256 + 0x80;

	// interrupt(0x13, AX, BX, CX, DX);
	interrupt(0x13, AX, BX, CX, DX);  

	return buffer;
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

void readFile(char* filename) {
	int i;
	for (i = 0; i < 10; ++i){
		printChar("C");
	}
/*
	char* buffer[512];
	//char* buffer2[512];
	int* i;
	int AX = 3;

	int BX =  &filename;
	int CX = &buffer;
	int DX = &i;
	
	readSector(buffer, BX);

	int fileentry = 0;

	for (fileentry = 0; fileentry < 512; fileentry+=32){ //was incrementing i before and not fileentry
		if (filename[0] == dir[fileentry+0] && filename[1] == dir[fileentry+1] && filename[2] == dir[fileentry+2] && filename[3] == dir[fileentry+3] && filename[4] == dir[fileentry+4] && filename[5] == dir[fileentry+5]) {
			for (int i = 0; dir[fileentry+i] != 0; i++) {
				readSector(buffer, dir[fileentry+i]);
                		CX += 512;
           		} 
       		} 
	}
*/
	
}

