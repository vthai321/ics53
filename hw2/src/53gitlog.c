#include "constants2.h"
#include "helpers2.h"
#include "hw2.h"

int main(int argc, char* argv[]) {
	int D_flag = 0;
	int A_flag = 0;
    int NUM_arg = 0;
    int LEVEL_arg = 0;
    char ORDER_arg = 'n';   // default no order
    char* DATE_arg = NULL;
    char* OUTFILE = NULL;
    char* INFILE = NULL;

    // Use basic getopt to parse flags and respective arguments
    int option;
    while ((option = getopt(argc, argv, "HD:A:O:I:n:ad" )) >= 0) {
        switch (option) {
            case 'H':
				fprintf(stdout, USAGE_MSG);
				return EXIT_SUCCESS;
            case 'D':
        	    D_flag = 1;
                DATE_arg = optarg;
                break;
            case 'A':
				A_flag = 1;
                LEVEL_arg = atoi(optarg);
                break;
            case 'n':
				NUM_arg = atoi(optarg);
                break;
            case 'a':
            case 'd':
				ORDER_arg = option;
                break;
            case 'O':
				OUTFILE = optarg;
                break;
            case 'I':
				INFILE = optarg;
                break;
            default:
                fprintf(stderr, USAGE_MSG);
                return EXIT_FAILURE;
        }
    }

    // validate a required option was specified - Does not check for more than 1
    if ( ! (A_flag | D_flag) )
    {
        fprintf(stderr, "ERROR: Search mode was not specified.\n\n" USAGE_MSG);
        return EXIT_FAILURE;
    }
    
    // INSERT YOUR IMPLEMENTATION HERE
    // getopts only stored the arguments and performed basic checks. More error checking is still needed!!!!


    return 0;
}
