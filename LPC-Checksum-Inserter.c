#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_FILE_SIZE (1024 * 1024)

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	char *input_file, *output_file;
	int i;
	unsigned int checksum, *buffer;
	size_t file_size;
	
	// Check for arguments passed
	if(argc < 2)
	{
		printf("Error: Input file required\n");
		printf("Usage:\n\tLPC-Checksum-Inseter InputFile.bin\nor\n\tLPC-Checksum-Inseter InputFile.bin OutputFile.bin");
		return(1);
	}
	
	// Onput filename
	input_file = (char *)argv[1];
	
	// If no output file is specified then the output over the input file.
	if(argc == 2)
	{
		output_file = input_file;
	}
	else
	{
		output_file = (char *)argv[2];
	}
	
	// Create a buffer to store the file
	buffer = (unsigned int *) malloc(MAX_FILE_SIZE);
	if(!buffer)
	{
		printf("Error: unable to allocate memory for buffer\n");
		return(1);
	}
	
	// Open the input file
	fp = fopen((const char *) input_file, "rb");
	if(fp == NULL)
	{
		printf("Error: unable to open file %s for input\n", input_file);
		free(buffer);
		return(1);
	}
	
	// Read the input file to the buffer
	file_size = fread((void *) buffer, 1, MAX_FILE_SIZE, fp);
	if(ferror(fp))
	{
		printf("Error: failed to read file: %s\n", input_file);
		close(fp);
		free(buffer);
		return(1);
	}
	
	// Check the file has been fully read into our buffer and is not to long
	if(!feof(fp))
	{
		printf("The input file %s is too large for this application. The maximum size supported is %d bytes\n", input_file, MAX_FILE_SIZE);
		close(fp);
		free(buffer);
		return(1);
	}
	
	// File has been read into memory
	printf("Input file %s has been read. Size %d bytes\n", input_file, (int)file_size);
	
	// Close the input file
	close(fp);
	
	// Compute 2's complement checksum
	checksum = 0;
	for(i = 0; i < 7; i++)
	{
		checksum += buffer[i];
		printf("%d: 0x%08x\n", i, buffer[i]);
	}

	if(buffer[7] == (0xFFFFFFFF - checksum) + 1)
	{
		printf("Checksum already set to correct value.  Exiting without changing file\n");
		free(buffer);
		return(1);
	}
	
	buffer[7] = (0xFFFFFFFF - checksum) + 1;
	printf("7: 0x%08x\n", buffer[7]);
	printf("checksum total: 0x%08x\n", checksum + buffer[7]);
	
	// Open output file
	fp = fopen(output_file, "wb");
	if(!fp)
	{
		printf("Error: unable to open file %s for output\n", output_file);
		free(buffer);
		return(1);
	}
	
	// Write data back to file
	if(fwrite(buffer, 1, file_size, fp) != file_size)
	{
		printf("Error: unable to write to file %s\n", output_file);
		close(fp);
		free(buffer);
		return(1);
	}
	
	printf("File %s written with %d bytes\n", output_file,(int) file_size);
	
	free(buffer);
	close(fp);

	return(1);
}
