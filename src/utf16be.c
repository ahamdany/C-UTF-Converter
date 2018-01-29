#include <unistd.h>
#include "utf.h"
#include "wrappers.h"

int
from_utf16be_to_utf16le(int infile, int outfile){
    int bom;
    utf16_glyph_t buf;
    ssize_t bytes_read;
    ssize_t bytes_to_write;
    int ret = 0;

    bom = UTF16LE;
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    reverse_bytes(&bom, 2);//3
    #endif
    write_to_bigendian(outfile, &bom, 2);

    while ((bytes_read = read_to_bigendian(infile, &(buf.upper_bytes), 2)) > 0) {
        bytes_to_write = 2;//4 /* utf-32 future compatibility */
        reverse_bytes(&(buf.upper_bytes), 2);
        if(is_lower_surrogate_pair(buf)) {
            if((bytes_read = read_to_bigendian(infile, &(buf.lower_bytes), 2) < 0)) {//>
                break;
            }
            reverse_bytes(&(buf.lower_bytes), 2);
            bytes_to_write += 2;
        }
        write_to_bigendian(outfile, &buf, bytes_to_write);
    }
    ret = bytes_read;
    return ret;
}

int
from_utf16be_to_utf8(int infile, int outfile){
    int bom;
    utf16_glyph_t buf;
    ssize_t bytes_read;
    int ret = 0;
    utf8_glyph_t utf8_buf;
    size_t size_of_glyph;

    bom = UTF8;

    write_to_bigendian(outfile, &bom, 3);

    while((bytes_read = read_to_bigendian(infile, &(buf.upper_bytes), 2)) > 0) {
        reverse_bytes(&(buf.upper_bytes), 2);
        if(!is_upper_surrogate_pair(buf)) {
          ret = buf.upper_bytes;
          utf8_buf = code_point_to_utf8_glyph(ret, &size_of_glyph);
          write_to_bigendian(outfile, &utf8_buf, size_of_glyph);

        } else {
          read_to_bigendian(infile, &(buf.lower_bytes), 2);
          reverse_bytes(&(buf.lower_bytes), 2);
            if(is_lower_surrogate_pair(buf)) {
                ret = ((buf.upper_bytes - 0xD800) << 10  |
                       ((buf.lower_bytes - 0xDC00) & 0x3FF)) + 0x10000;
                utf8_buf = code_point_to_utf8_glyph(ret, &size_of_glyph);
                write_to_bigendian(outfile, &utf8_buf, size_of_glyph);
            }
        }
    }
    ret = bytes_read;
    return ret;
}

utf16_glyph_t
code_point_to_utf16be_glyph(code_point_t code_point, size_t *size_of_glyph){
  
    utf16_glyph_t ret;

    memset(&ret, 0, sizeof ret);
    if(is_code_point_surrogate(code_point)) {
        code_point -= 0x10000;
        ret.upper_bytes = (code_point >> 10) + 0xD800;
        ret.lower_bytes = (code_point & 0x3FF) + 0xDC00;
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        reverse_bytes(&ret.upper_bytes, 2);
        reverse_bytes(&ret.lower_bytes, 2);
        #endif
        *size_of_glyph = 4;
    }
    else {
        ret.upper_bytes |= code_point;
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        reverse_bytes(&ret.upper_bytes, 2);
        #endif
        *size_of_glyph = 2;
    }
    return ret;
}

