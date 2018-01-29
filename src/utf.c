#include <sys/sendfile.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include "utf.h"
#include "debug.h"
#include "wrappers.h"

convertion_func_t
get_encoding_function(int infile,int outfile){
    encoding_from_to_t translate =
    (program_state->encoding_from - program_state->encoding_to);

    switch (translate) {
        case utf8_to_utf16le:
            return (convertion_func_t)(unsigned long)from_utf8_to_utf16le(infile,outfile);
        case utf8_to_utf16be:
            return (convertion_func_t)(unsigned long)from_utf8_to_utf16be(infile,outfile);
        case utf16le_to_utf16be:
            return (convertion_func_t)(unsigned long)from_utf16le_to_utf16be(infile,outfile);
        case utf16be_to_utf16le:
            return (convertion_func_t)(unsigned long)from_utf16be_to_utf16le(infile,outfile);
        case utf16be_to_utf8:
            return (convertion_func_t)(unsigned long)from_utf16be_to_utf8(infile,outfile);
        case utf16le_to_utf8:
            return (convertion_func_t)(unsigned long)from_utf16le_to_utf8(infile,outfile);
        case transcribe_file:
            return (convertion_func_t)(unsigned long)transcribe(infile,outfile);
    }
  return NULL;
}

void
check_bom(){
    int fd;
    ssize_t bytes_read;
    format_t bom = 0;

    if (program_state->in_file == NULL) {
        error("%s\n", "In file not specified");
        exit(EXIT_FAILURE);
    }
    fd = Open(program_state->in_file, O_RDONLY);

    if ((bytes_read = read_to_bigendian(fd, &bom, 3)) < 3) {
        fprintf(stderr, "%s\n", "File contains invalid BOM or is empty beyond BOM");
        exit(EXIT_FAILURE);
    }
    debug("BOM: %x", bom);
    if(bom == UTF8){
        info("Source BOM: %s", STR_UTF8);
        program_state->encoding_from = UTF8;
        program_state->bom_length = 3;
        close(fd);
        return;
    }
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        reverse_bytes(&bom, 2);
    #endif
    debug("BOM AFTER SWAP: %x", bom);
    if (LOWER_TWO_BYTES(bom) == UTF16LE) {
        info("Source BOM: %s", STR_UTF16LE);
        program_state->encoding_from = UTF16LE;
        program_state->bom_length = 2;
    }
    else if(LOWER_TWO_BYTES(bom) == UTF16BE)
    {
        info("Source BOM: %s", STR_UTF16BE);
        program_state->encoding_from = UTF16BE;
        program_state->bom_length = 2;
    }
    else
    {
        fprintf(stderr, "%s\n", "Unrecognized BOM");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

int
transcribe(int infile, int outfile){
  int ret = 0;
  struct stat infile_stat;
  if (fstat(infile, &infile_stat) < 0) {
    perror("Could not stat infile");
    exit(EXIT_FAILURE);
  }
  ret = sendfile(outfile, infile, NULL, infile_stat.st_size);
  return ret;
}

bool
is_upper_surrogate_pair(utf16_glyph_t glyph){
    return ((glyph.upper_bytes > 0xD800) && (glyph.upper_bytes < 0xDBFF));
}

bool
is_lower_surrogate_pair(utf16_glyph_t glyph){
  return ((glyph.lower_bytes > 0xDC00) && (glyph.lower_bytes < 0xDFFF));
}

code_point_t
utf16_glyph_to_code_point(utf16_glyph_t *glyph){
    
    code_point_t ret = 0;

    if(!is_upper_surrogate_pair(*glyph)) {
        ret = glyph->upper_bytes;
    }
    else {
        ret = ((glyph->upper_bytes - 0xD800) << 8  |
        ((glyph->lower_bytes - 0xDC00) & 0x3FF)) +
        0x10000;// 0xD800) << 1
    }
    return ret;
}

bool
is_code_point_surrogate(code_point_t code_point){
    return (code_point >= 0x10000);
}
