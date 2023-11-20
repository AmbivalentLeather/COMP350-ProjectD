/* Written by: Nick Young, Chase Simao, starting code from Joe Matta
 * Date: October 2023 */

void printString(char*);
void printChar(char*);
char* readString(char*);
void readSector(char*, int);
void handleInterrupt21(int ax, char* bx, int cx, int dx);
void readFile(char* filename, char* output_buffer, int* sectorsRead);
int directoryLineCompare(char* directory_buffer, int* file_entry, char* string_to_beat);
void executeProgram(char* name);
void writeSector(char*, int);
void deleteFile(char* filename);

void writeBFile(char*, char*, int);

void terminate();

int main()
{
	makeInterrupt21();
	interrupt(0x21, 8, "This is a test message", "alphab", 1);
	interrupt(0x21, 4, "shell", 0, 0);
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
		case 6: writeSector(bx, cx);
			break;
		case 7: deleteFile(bx);
			break;
		case 8: writeBFile(bx, cx, dx);
			break;
		default: printString("Error AX is invalid");
			break;
	}
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
	inputArray[i] = 0xd;	// Add return character to character array
	inputArray[i + 1] = 0x0;	// Add null character to character array

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

void writeSector(char* address, int sector)
{
	// this number tells BIOS to read a sector as opposed to write (changed to 3 for writeSector)
	int AH = 3;
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

void readFile(char* filename, char* output_buffer, int* sectorsRead)
{
	char directory_buffer[512];
	int i = 0;

	int file_entry = 0;
	int* pfile_entry;
	pfile_entry = &file_entry;
	*sectorsRead = 0;

	// Reads directory (sector 2) into directory buffer
	readSector(directory_buffer, 2);

	// Checks if filename exists in directory
	if (directoryLineCompare(directory_buffer, pfile_entry, filename)) {
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

// This is chase's writeFile
void writeFile(char* buffer, char* filename, int numberOfSectors) {
	char dir[512];
	char map[512];
	int file_entry = 96; // variable for loop that reads through map checking for \0
	int fillin = 0; // variable for loop that fills rest of directory with 0s
	int sectorCounter = 0; // keeps track of the number of sectors the loop has gone through
	int addSectorNum = 7; // keeps track of where the sector number should be added to directory entry

	readSector(map, 1); // reads map sector 1 into map buffer
	readSector(dir, 2); //reads directory sector 2 into directory buffer
    
	for (file_entry = 96; file_entry < 512; file_entry += 32) { 
		// loops through map, we start at ninety six because we do not want to overwrite the bootloader
		if (map[file_entry] == '\0') { // checks for 0 entry
			// sets that sector to 0xFF in the map
			map[file_entry + 2] = "F";
			map[file_entry + 3] = "F";

            
            	//I am confused on how to add the sector number to the files directory entry
            	// after the sixth character in the directory buffer is where you add the sector numbers
            
            	dir[addSectorNum] = sectorCounter; // adds sector number to files directory entry

            	writeSector(buffer, sectorCounter); // writes 512 bytes from the buffer holding the file to that sector

            	for(fillin = addSectorNum + 1; fillin < 512; fillin++) { // loops through and fills in the remaining bytes in directory entry to 0
			dir[fillin] = "0"; // fill in the remaining bytes in the directory entry to 0
		}
            
		addSectorNum++; // this increments so that next time a sector number is added to directory entry it does not overwrite the last
		numberOfSectors--; // number of sectors represents how many should be written to the disk, everytime a sector is written this decrements

		writeSector(map, 1); // writes map sector back to disk
		writeSector(dir, 2); // writes directory sector back to disk
            
		}
        sectorCounter++;
	} 
}

// This is Nick's writeFile
void writeBFile(char* buffer, char* filename, int numberOfSectors)
{
	// NOTE: THIS CODE IS BAD AND SLIGHTLY BUGGY
	// FIX THE BAD BEFORE THE BUGS
	char dir[512];
	char map[512];
	char genericSectorBuffer[512];
	int file_entry; // variable for loop that reads through map checking for \0

	// Simple iterator variables
	int i, j;

	int antiSectorCounter, directoryColumn, sectorCounter, currentSector, totalSectors;

	readSector(map, 1); // reads map sector 1 into map buffer
	readSector(dir, 2); //reads directory sector 2 into directory buffer
    
	// Find a free sector in map
	for (totalSectors = 3; totalSectors < 512 && sectorCounter < numberOfSectors; totalSectors++) {
		// Set empty sector(s) to 0xFF in map
		if (map[totalSectors] == '\0') {
			map[totalSectors] = 0xFF;
			sectorCounter++;
		}
	}

	antiSectorCounter = (totalSectors - sectorCounter);

	// Find free directory entry and append sector numbers
	for (file_entry = 0; file_entry < 512; file_entry += 32) { 
		if (dir[file_entry] == '\0') {
			// Copy filename into free directory entry
			for (i = 0; i < 6; i++) {
				 dir[file_entry + i] = filename[i];
			}
			// Store the location in dir for the sector numbers
			directoryColumn = file_entry + 6;

			// Write the numbers of the sectors being used by
			// the file immediately after the filename
			for (i = 0; i < sectorCounter; i++) {
				dir[directoryColumn + i] = antiSectorCounter++;
				i++;
			}
			// Break out of the loop after finding the
			// correct entry
			break;
		}
	}
	
	// This loop reads the buffer into its assigned sectors
	// This is the most disgusting part of the code
	antiSectorCounter = (totalSectors - sectorCounter);
	for (i = 0; i < sectorCounter; i++) {
		currentSector = antiSectorCounter + i;
		readSector(genericSectorBuffer, currentSector);
		// This loop writes 512 bytes from the buffer into the sector i
		for (j = 0; j < 512; j++) {
			// genericSectorBuffer increments by 512
			// buffer increments by the NEXT 512 bytes starting at 0, then 1, then 2 etc
			genericSectorBuffer[j] = buffer[j + (i * 512)];
		}
		writeSector(genericSectorBuffer, currentSector);
	}

	writeSector(map, 1); // writes map sector back to disk
	writeSector(dir, 2); // writes directory sector back to disk
}

void deleteFile(char* filename)
{
	char dir[512];
	char map[512];
	int i = 0;
	int mapIndex = 0;

	int file_entry = 0;
	int* pfile_entry = &file_entry;

	// Reads map (sector 1) into map buffer
	readSector(map, 1);
	// Reads directory (sector 2) into directory buffer
	readSector(dir, 2);

	// Checks if filename exists in directory
	if (directoryLineCompare(dir, pfile_entry, filename)) {
		dir[*pfile_entry] = '\0';
		while(dir[*pfile_entry + 6 + i] != '\0') {
			mapIndex = dir[*pfile_entry + 6 + i];
			map[mapIndex] = 0;
			i++;
		}
       	}

	writeSector(map, 1);
	writeSector(dir, 2);

}

int directoryLineCompare(char* directory_buffer, int* file_entry, char* filename_to_beat)
{
	int correctIndex = 0;
	int i = 0;

	// Check every line in directory_buffer, incrementing 32 to move to the next line
	for (*file_entry = 0; *file_entry < 512; *file_entry += 32){
		// Compare the first 6 characters to the given filename_to_beat
		while (i < 6) {
			if(directory_buffer[*file_entry + i] != filename_to_beat[i])
				break;
			else 
				correctIndex++;
			++i;
		}
		// Return true if all characters match, this works regardless of string length
		if(correctIndex == 6)
			return 1;
	}
	// Base case, if the loop above finds nothing, return false
	return 0;
}

void executeProgram(char* program_name)
{
    	char buffer[13312];
    	int sectorsRead;
	int offset = 0;

	// Read program_name into buffer
    	readFile(program_name, buffer, &sectorsRead);

    	for (offset = 0; offset < sectorsRead * 512; offset++) { 
        	// putInMemory(int segment, int address, char character)
         	putInMemory(0x2000, offset, buffer[offset]); 
    	}

	launchProgram(0x2000); // will not return, sets of registers and jumps to the program located at 0x2000
}

void terminate()
{
	char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';

	executeProgram(shellname);
}

