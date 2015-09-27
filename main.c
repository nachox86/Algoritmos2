#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Show help here... */
void showHelp() {}

int main(int argc, char *argv[]) {
	int i, nargs = 0;
    char *inputDir = NULL, *outputFile = NULL, *logfile = NULL;
    
    if (argc < 7) {
    	printf("Not enough args\n"); /* TODO: Do something else with this */
        return -1; /* NOT ENOUGH ARGS */
    }
    
    for (i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            showHelp();
            break;
        } else if (strcmp(argv[i], "-i") == 0) {
            inputDir = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(inputDir, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i], "-l") == 0) {
            logfile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(logfile, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i], "-o") == 0) {
            outputFile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(outputFile, argv[i+1]);
            nargs++;
        }
    }
    
    if (nargs == 3) {
    	/* We got enough arguments to proceed */
    	/* TODO: Next step, process input */
    }
    
    free(inputDir);
    free(outputFile);
    free(logfile);
    
    return 0;
}
