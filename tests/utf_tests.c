#include <criterion/criterion.h>
#include "utf.h"
#include "wrappers.h"

Test(utf16_suite, code_point_surrogate_pair) {
    code_point_t cp1 = 0x21, cp2 = 0xe9, cp3 = 0x77e5, cp4 = 0x1F913;

    cr_assert_not(is_code_point_surrogate(cp1));

    cr_assert_not(is_code_point_surrogate(cp2));

    cr_assert_not(is_code_point_surrogate(cp3));

    cr_assert(is_code_point_surrogate(cp4));


}

Test(utf16_suite, glyph_surrogate_check) {
    utf16_glyph_t g1, g2, g3, g4;

    g1.upper_bytes = 0x0021;
    g1.lower_bytes = 0x0000;

    g2.upper_bytes = 0x00e9;
    g2.lower_bytes = 0x0000;

    g3.upper_bytes = 0x77e5;
    g3.lower_bytes = 0x0000;

    g4.upper_bytes = 0xd83e;
    g4.lower_bytes = 0xdd13;

    cr_assert_not(is_upper_surrogate_pair((g1)));
    cr_assert_not(is_lower_surrogate_pair(g1));

    cr_assert_not(is_upper_surrogate_pair(g2));
    cr_assert_not(is_lower_surrogate_pair(g2));

    cr_assert_not(is_upper_surrogate_pair(g3));
    cr_assert_not(is_lower_surrogate_pair(g3));

    cr_assert(is_upper_surrogate_pair(g4));
    cr_assert(is_lower_surrogate_pair(g4));


}

Test(utf8_suite, code_point_size) {
    code_point_t cp0 = 0x21, cp1 = 0xe9, cp2 = 0x77e5, cp3 = 0x1F913;
    size_t s0, s1, s2, s3;

    s0 = utf8_glyph_size_of_code_point(cp0);
    s1 = utf8_glyph_size_of_code_point(cp1);
    s2 = utf8_glyph_size_of_code_point(cp2);
    s3 = utf8_glyph_size_of_code_point(cp3);

    cr_assert_eq(s0, 1);
    cr_assert_eq(s1, 2);
    cr_assert_eq(s2, 3);
    cr_assert_eq(s3, 4);


}

 Test(utf8_suite, encode_funcs) {


     utf8_glyph_t exp1, exp2, exp3, exp4;
     utf8_glyph_t g1, g2, g3, g4;
     code_point_t cp1 = 0x21, cp2 = 0xe9, cp3 = 0x77e5, cp4 = 0x1F913;


     g1 = utf8_one_byte_encode(cp1);
     g2 = utf8_two_byte_encode(cp2);
     g3 = utf8_three_byte_encode(cp3);
     g4 = utf8_four_byte_encode(cp4);


     exp1.bytes[0].byte = 0x21;

     exp2.bytes[1].byte = 0xa9;
     exp2.bytes[0].byte = 0xc3;

     exp3.bytes[2].byte = 0xa5;
     exp3.bytes[1].byte = 0x9f;
     exp3.bytes[0].byte = 0xe7;

     exp4.bytes[3].byte = 0x93;
     exp4.bytes[2].byte = 0xa4;
     exp4.bytes[1].byte = 0x9f;
     exp4.bytes[0].byte = 0xf0;




    cr_assert_eq(memcmp(&g1, &exp1, 1), 0);
    cr_assert_eq(memcmp(&g2, &exp2, 2), 0);
    cr_assert_eq(memcmp(&g3, &exp3, 3), 0);
    cr_assert_eq(memcmp(&g4, &exp4, 4), 0);


 }

Test(wrappers_suite, reverse_bytes) {
    unsigned char c = 0x10;
    unsigned short s = 0xbeef;
    unsigned int i = 0xdeadbeef;
    unsigned long l = 0xdeadbeefdeadcafe;

    reverse_bytes(&c, sizeof c);
    reverse_bytes(&s, sizeof s);
    reverse_bytes(&i, sizeof i);
    reverse_bytes(&l, sizeof l);

    cr_assert_eq(c, 0x10);
    cr_assert_eq(s, 0xefbe);
    cr_assert_eq(i, 0xefbeadde);
    cr_assert_eq(l, 0xfecaaddeefbeadde);


}

Test(args_suite, join_string_array) {
    char *a[] = {"hello", "world", NULL};

    char *s = join_string_array(2, a);

    cr_assert_str_eq(s, "hello world ");
    free(s);


}

Test(args_suite, determine_format) {
    char *sutf16le = "UTF16LE";
    char *sutf16be = "UTF16BE";
    char *sutf8 = "UTF8";

    format_t utf8 = determine_format(sutf8);
    format_t utf16le = determine_format(sutf16le);
    format_t utf16be = determine_format(sutf16be);

    cr_assert_eq(utf8, UTF8);
    cr_assert_eq(utf16le, UTF16LE);
    cr_assert_eq(utf16be, UTF16BE);


}

//############################################
// STUDENT UNIT TESTS SHOULD BE WRITTEN BELOW
// DO NOT DELETE THESE COMMENTS
//############################################

Test(hw1_tests_suite, helpTest1) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -h NULL";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCESS", return_code);
}

Test(hw1_tests_suite, helpTest2) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -h";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCESS", return_code);
}

Test(hw1_tests_suite, helpTest3) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -hello";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, helpTest4) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -h -e";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, helpTest5) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e -h";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, helpTest6) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -h -e UTF8";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, helpTest7) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -h -e UTF8 rsrc/UTF8.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, helpTest8) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -h -e UTF8 rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, helpTest9) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e -h UTF8 rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, helpTest10) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF8 -h rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, helpTest11) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF8 rsrc/UTF8.txt -h rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, helpTest12) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF8 rsrc/UTF8.txt rsrc/helptest.txt -h";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, helpTest13) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF8 rsrc/UTF8.txt -h rsrc/helptest.txt -h";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, helpTest14) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF8 rsrc/UTF8.txt -hrsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, invalidArgs0) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -i UTF8 rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs1) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs2) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 -e rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs3) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt -e rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs4) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt -e";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs5) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt UTF8";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs6) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt -";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs7) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt e-";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs8) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt e-h";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs10) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt -e-h";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, invalidArgs11) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf UTF8 rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs12) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, invalidArgs13) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf rsrc/UTF8.txt -e UTF16BE rsrc/testing.txt -h";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_FAILURE, "Program exited with %d instead of EXIT_FAILURE", return_code);
}

Test(hw1_tests_suite, validArgs0) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf rsrc/UTF8.txt rsrc/helptest.txt -e UTF16LE";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, validArgs1) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf rsrc/UTF8.txt -e UTF16LE rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, validArgs2) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF16LE rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, validArgs3) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF16LE rsrc/UTF8.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}

Test(hw1_tests_suite, validArgs4) {
    //int argc = 2;
    //char *argv[] = {"bin/utf", "-h", NULL};
    char *cmd = "bin/utf -e UTF8 rsrc/UTF16LE.txt rsrc/helptest.txt";
    int return_code = WEXITSTATUS(system(cmd));

    cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCCESS", return_code);
}







