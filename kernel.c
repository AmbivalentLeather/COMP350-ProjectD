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
	char line[80];
	makeInterrupt21();
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);	

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

	while(1);
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

void printChar(char* c)
{
	char al = c;
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

void readFile(char* filename)
{
/*
 You should create a new function readFile that takes a character array
 containing a file name and reads the file into a buffer. After you complete
 your function, you should make it into an interrupt 0x21 call: 

 Read File: 
 AX = 3 
 BX = address (HINT: ADDRESS = &POINTER) of character array containing the file name 
 CX = address of a buffer to hold the file 
 DX = address of an integer to hold the number of sectors you read

 Your readFile function should work as follows: 

 1. Load the directory sector into a 512 byte character array using readSector 

 2. Go through the directory trying to match the file name. If you do not find
 it, set the number of sectors read to 0 and return. 

 3. Using the sector numbers in the directory, load the file, sector by sector,
 into the buffer array. You should add 512 to the buffer address every time you
 call readSector.  Make sure to increment the number of sectors read as you go.

 Hints This part is challenging.  Try using a for loop with an iterator
 fileentry to step through the directory in increments of 32.  Then for each
 file entry, look at the next six characters and compare them to your file name
 parameter.  For example, does filename[0]==dir[fileentry+0],
 filename[1]==dir[fileentry+1], and so on?  If not, move on to the next entry.
 You might use your printChar function to see how many times the for-loop is
 running.

 When you find your file, use another for-loop to load it.  Call readSector
 using dir[fileentry+6] as the sector number.  Add 512 to the buffer address.
 Call readSector with dir[fileentry+7].  Repeat this until you reach a sector
 number 0.
 */
}
