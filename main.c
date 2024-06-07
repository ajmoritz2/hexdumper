#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define PER_LINE 16

int fd  = -1;

void printText(FILE* file, FILE* stream, int* flags)
{
	char trans[PER_LINE+1];
	trans[PER_LINE] = 0;
	int c = fgetc(file), count = 0, line = 1;
	if (flags[3] == 0) 
	{
		fprintf(stream, "%#08x: ", line); // FIRST LINE NUMBER 
	}
	do {
		if (c > 32 && c < 127) {
			trans[count] = c;
		} else {
			trans[count] = '.';
		}
		if (flags[3] == 0) {	
			fprintf(stream, "%#04x ", c);
			count++;
			if (count >= PER_LINE)
			{
				fprintf(stream, "|%s|\n", trans); // ASCII
				line++;
				fprintf(stream, "%#08x: ", line); // LINE #
				count = 0;
			}
		} else {
			fprintf(stream, "%02x ", c);
			count++;
			if (count >= PER_LINE)
			{
				fprintf(stream, "\n");
				count = 0;
			}
		}
		c = fgetc(file);
	} while (c != EOF);
	
	if (count != 0 && flags[3] == 0) {
		for (int i = PER_LINE-1; i > count-1; i--) {
			fprintf(stream, "%#04x ", 3);
			trans[i] = '.';
		}
		
		fprintf(stream, "|%s|\n",trans);
	}	

}

int printDocs() {
	FILE* docs = fopen("hexdocs.txt", "r");
	int c = 0;

	if (docs == NULL) {
		printf("No documentation found!");
		return -1;
	}

	while ((c = fgetc(docs)) != EOF) {
		printf("%c",c);
	}

	fclose(docs);
}

void defineFlags(int flags[], FILE** stream, char *argv[], int argc) {
	// Function assumes file is real
	
	for (int i = 2; i < argc; i+=1) {
		if (strcmp("-e", argv[i]) == 0) {
			flags[0] = atoi(argv[i+1]);
			i++;
			continue;
		} else if (strcmp("-s", argv[i]) == 0){
			flags[1] = atoi(argv[i+1]);
			i++;
			continue;
		} else if (strcmp("-p", argv[i]) == 0) {
			*stream = fopen(argv[i+1], "w+");
			flags[2] = 1;
			i++;
			continue;
		} else if (strcmp("-r", argv[i]) == 0) {
			flags[3] = 1;
			i++;
			continue;
		} else {
			printf("%s is not a valid argument. Do -h for a list of valid flags!\n Flags include -p for output file (not sure why I made it -p and not -o)\n", argv[i]);
			exit(1);
		}
	
	}
	
}

/* Flag numbers as follows
 * -e flag 0 (end, means nothing)
 *  -s flag 1 (start, means nothing)
 *  -p flag 2 (print to file)
 *  -r flag 3 (raw data)
 */

int main (int argc, char *argv[])
{
	int fileParam = 1;
	int flags[4] = {-1, -1, 0, 0};

	FILE* stream = stdout;
	if (strcmp("-h", argv[1]) == 0){
		printDocs();
		return 1;
	}


	FILE* file; // The file we are going to load
	file = fopen(argv[fileParam], "rb");
	
	if (file == NULL)
	{
		printf("File %s not found!", argv[fileParam]);
		return -1;
	}
	printf("File found at %#x\n", file); 

	defineFlags(flags, &stream, argv, argc);

	printText(file, stream, flags);

	if (flags[2] == 1) {
		
	}

	printf("File binaries completed!");
	fclose(stream);
	fclose(file);

	return 0;	
}
