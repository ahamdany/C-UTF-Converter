#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <utf.h>
#include <wrappers.h>
#include <debug.h>

int opterr;
int optopt;
int optind;
char *optarg;

state_t *program_state;

void
parse_args(int argc, char *argv[]){
    int i;
    int option;
    char *joined_argv;
    char** arguments = argv;
    int iterator = 0;
    if(argc<2){
        USAGE(argv[0]);
        exit(EXIT_FAILURE);
    }
    if(argc>5){
        USAGE(argv[0]);
        exit(EXIT_FAILURE);
    }


    joined_argv = join_string_array(argc, arguments);
    info("argc: %d argv: %s", argc, joined_argv);

    program_state = Calloc(1, sizeof(state_t));
    for (i = 0; iterator < argc; ++i) {
        debug("%d opterr: %d", i, opterr);
        debug("%d optind: %d", i, optind);
        debug("%d optopt: %d", i, optopt);
        debug("%d argv[optind]: %s", i, argv[optind]);
        if ((option = getopt(argc, argv, "h+e:")) != -1) {
          switch (option) {
            case 'e':
              if((strcmp(optarg,"-h"))==0){
                USAGE(argv[0]);
                free(program_state);
                exit(EXIT_SUCCESS);
              }
              info("Encoding Argument: %s", optarg);
              if ((program_state->encoding_to = determine_format(optarg)) == 0){
                print_state();  //errorcase;
              }

              if((argc - (optind)) == 2){
                program_state->in_file = argv[optind];
                program_state->out_file = argv[optind + 1];
              }
              else if((argc - (optind)) == 1){
                program_state->in_file = argv[optind - 3];
                program_state->out_file = argv[optind];
              }
              else if((argc - (optind)) == 0){
                program_state->in_file = argv[optind - 4];
                program_state->out_file = argv[optind - 3];
              }
              break;

            case '?':
              if (optopt != 'h'){
                USAGE(argv[0]);
                fprintf(stderr, KRED "-%c is not a supported argument\n" KNRM,optopt);
                free(joined_argv);

                free(program_state);
                exit(EXIT_FAILURE);
              }
              else{
                USAGE(argv[0]);
                free(joined_argv);

                free(program_state);
                exit(EXIT_SUCCESS);
              }
              break;

            case 'h':    //"errorcase"[0]
              USAGE(argv[0]);
              free(joined_argv);
              free(program_state);
              exit(EXIT_SUCCESS);
              break;

            default: {
              break;
            }
          }
    }
    iterator++;
  }
    free(joined_argv);
}

format_t
determine_format(char *argument){
  
  STR_UTF16BE = "UTF16BE";
  STR_UTF16LE = "UTF16LE";
  STR_UTF8 = "UTF8";

  if (strcmp(argument, STR_UTF16LE) == 0)
    return UTF16LE;
  if (strcmp(argument, STR_UTF16BE) == 0)
    return UTF16BE;
  if (strcmp(argument, STR_UTF8) == 0)
    return UTF8;
  return 0;
}

const char*
bom_to_string(format_t bom){

  switch(bom){
    case UTF8: return STR_UTF8;
    case UTF16BE: return STR_UTF16BE;
    case UTF16LE: return STR_UTF16LE;
  }
  return "UNKNOWN";
}

char*
join_string_array(int count, char *array[])
{
    char *ret;
    char charArray[count];
    int i;
    int len = 0, str_len, cur_str_len;
    str_len = array_size(count, array);
    ret = (char*)Calloc(str_len,sizeof(char));
    *ret = *charArray;
    str_len+=0;

    for (i = 0; i < count; ++i) {
        cur_str_len = strlen(array[i]);
        memcpy(ret + len, array[i],cur_str_len);
        len += cur_str_len;
        memcpy(ret + len, " ",1);
        len += 1;
    }
    return ret;
}

int
array_size(int count, char *array[]){
    
  int i, sum = 1; /* NULL terminator */
  for (i = 0; i < count; ++i) {
    sum += strlen(array[i]);
    ++sum; /* For the spaces */
  }
  return sum+1;
}

void
print_state(){
    
  if (program_state == NULL) {
    error("program_state is %p", (void*)program_state);
    exit(EXIT_FAILURE);
  }
  info("program_state {\n"
         "  format_t encoding_to = 0x%X;\n"
         "  format_t encoding_from = 0x%X;\n"
         "  char *in_file = '%s';\n"
         "  char *out_file = '%s';\n"
         "};\n",
         program_state->encoding_to, program_state->encoding_from,
         program_state->in_file, program_state->out_file);
}
