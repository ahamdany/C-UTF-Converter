#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include "wrappers.h"

#include "utf.h"


int
main(int argc, char *argv[]){
    int infile, outfile, in_flags, out_flags;
    parse_args(argc, argv);
    check_bom();
    print_state();
    in_flags = O_RDONLY;
    out_flags = O_WRONLY | O_CREAT;
    infile = Open(program_state->in_file, in_flags);
    outfile = Open(program_state->out_file, out_flags);
    lseek(infile,program_state->bom_length,SEEK_SET); /* Discard BOM */
    get_encoding_function(infile, outfile);
    close(infile);
    close(outfile);
    if(program_state != NULL) {
        close((long)program_state);
    }
    free(program_state);
    return EXIT_SUCCESS;
}
