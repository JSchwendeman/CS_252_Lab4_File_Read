#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * README: Please complete all TODO and compile and Test it.
 * You can use `gcc lab4_starter_code.c -o lab4` to compile the code.
 * You can use `./lab4` to execute the program.
 * Note: size_t is the **unsigned integer** type of the result of sizeof.
 * size_t can store the maximum size of a theoretically possible 
 * object of any type (including array).
 * size_t is commonly used for array indexing and loop counting.
 */

/* Function prototypes */
void search_by_byte(unsigned char *buffer, size_t size);
void search_by_string(unsigned char *buffer, size_t size);
void search_by_address(unsigned char *buffer, size_t size);
void display_context(unsigned char *buffer, size_t size, size_t index,
                     size_t length);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    /* Open the file */
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    /* Determine file size */
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    /* Allocate memory to load the file */
    unsigned char *buffer = (unsigned char *)malloc(size * 
                                                    sizeof(unsigned char));
    if (!buffer) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    /* Load the file into memory */
    fread(buffer, sizeof(unsigned char), size, file);
    fclose(file);

    /* Main loop */
    int choice;
    do {
        printf("\nSearch options:\n");
        printf("1. Search by byte value\n");
        printf("2. Search by ASCII string\n");
        printf("3. Search by memory address\n");
        printf("4. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                search_by_byte(buffer, size);
                break;
            case 2:
                search_by_string(buffer, size);
                break;
            case 3:
                search_by_address(buffer, size);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 4);

    /* Free allocated memory */
    free(buffer);

    return 0;
}

void search_by_byte(unsigned char *buffer, size_t size) {
    /* 
     * Prompt the user to input a Byte value (0-255)
     * If found the byte value in the buffer, print the memory address and,
     *    call the display_context() function
     * You may find at multiple places, please print them all 
     * If not found, print not found the byte value, and return to main()
     */
	int byte = -1;

	while (byte < 0 || byte > 255) {
		printf("Input a byte value to search for (0-255): ");
		scanf("%d", &byte);

		if (byte < 0 || byte > 255) {
			printf("Invalid byte value!");	
		}
	}

	printf("Search Results --------------------------------------------------------------------\n");
	int found = 0;
	for (int i=0; i<size; i++) {
		int indexVal = *(buffer+i);
		if (indexVal == byte) {
			printf("Match found at address %p\n", (buffer+i));
			display_context(buffer, size, i, 1);
			found = 1;
		}	
	}


	if (!found) {
		printf("No Results Found.\n");
	}
	printf("End Results -----------------------------------------------------------------------\n");

}

void search_by_string(unsigned char *buffer, size_t size) { 
    /* Prompt the user to input an ASCII string
     * If found the ASCII string in the buffer, print the memory address, and
     *     call the display_context() function
     * You may find at multiple places, please print them all
     * If not found, print not found the string value, and return to main()
     */
    /*
     * You may use the following functions:
     * scanf() to get the string from the terminal
     * strlen() to get the length of the string
     * strcpy() or strncpy() to copy the substring
     * memcmp() or strncmp() to compare two strings.
     * You may need to Google the usage of memcmp() or strncmp() funtion.
     */

	// allocate memory for user input
	char* inputString = (char*)malloc(size*sizeof(unsigned char));
	
    if (!inputString) {
        printf("Error: Memory Allocation failed");
        return;
    }

	// Block until valid input string
	int len = -1;
	while(1) {
		printf("Enter a string [cannot be larger than input file.]: ");
		scanf("%s", inputString);
	
		len = strlen(inputString);

		if (len > size) {
			printf("String cannot be larger than file size!\n");
			continue;
		}
		break;
	}

	printf("Search Results --------------------------------------------------------------------\n");
	int found = 0;

	// loop through characters in buffer
	for (int i=0; i<size; i++) {
		
		// Check for match to first char, or early return
		if (buffer[i] != inputString[0]) {
			continue;
		
		} 

		// boolean to indicate if full string is a match
		// assume true, set to false otherwise
		int match = 1;
			
		// loop through to confirm all chars match
		for (int j=0; j<len; j++) {
			if (inputString[j] != buffer[i+j]) {
				match = 0;
				break;
			}
		}
		

		// if match, print stuff
		if (match) {
			printf("Match found at address %p\n", (buffer + i));
			display_context(buffer, size, i, len);
			found = 1;
		}
	}	

	if (!found) {
		printf("No results found.\n");
	}
	printf("End Results -----------------------------------------------------------------------\n");

	// free memory
	free(inputString);
}

void search_by_address(unsigned char *buffer, size_t size) {
    size_t address;
    printf("Enter a memory address: ");
    scanf("%zu", &address);

    // Ensure the address is within bounds
    if (address < (size_t)buffer || address >= (size_t)(buffer + size)) {
        printf("Invalid address. Please enter an address within the valid range.\n");
        return;
    }

    size_t index = address - (size_t)buffer;
    printf("Memory address: %zu\t", address);
    display_context(buffer, size, index, 1);
}

void display_context(unsigned char *buffer, size_t size, size_t index, size_t length) {
    /* Print context before the match */
    if (index < 5) {
        printf("START OF FILE ");
        for (size_t i = 0; i < index; i++) {
            printf("%c", buffer[i]);
        }
    } else {
        for (size_t i = index - 5; i < index; i++) {
            printf("%c", buffer[i]);
        }
    }

    /* Print the matching byte/string */
    printf("**"); /* To distinguish the matching byte/string */
    for (size_t i = 0; i < length; i++) {
        printf("%c", buffer[index + i]);
    }
    printf("**");

    /* Print context after the match */
    if (index + length + 5 > size) {
        for (size_t i = index + length; i < size; i++) {
            printf("%c", buffer[i]);
        }
        printf(" END OF FILE ");
    } else {
        for (size_t i = index + length; i < index + length + 5; i++) {
            printf("%c", buffer[i]);
        }
    }
    printf("\n");
}
