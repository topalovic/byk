#include <stdio.h>
#include <ruby.h>
#include <ruby/encoding.h>

#ifndef rb_check_arity
#define rb_check_arity rb_check_arity

NORETURN(void rb_error_arity(int, int, int));

static inline void
rb_check_arity(int argc, int min, int max)
{
    if ((argc < min) || (max != -1 && argc > max))
	rb_error_arity(argc, min, max);
}
#endif

#define STR_ENC_GET(str) rb_enc_from_index(ENCODING_GET(str))

#define STR_CAT_COND_ASCII(ascii, dest, chr, ascii_chr, len, enc) \
    ascii ? rb_enc_str_buf_cat(dest, chr, len, enc)               \
          : str_cat_char(dest, ascii_chr, enc)

enum {
    LAT_CAP_TJ=262,
    LAT_TJ,
    LAT_CAP_CH=268,
    LAT_CH,
    LAT_CAP_DJ=272,
    LAT_DJ,
    LAT_CAP_SH=352,
    LAT_SH,
    LAT_CAP_ZH=381,
    LAT_ZH,
    CYR_CAP_DJ=1026,
    CYR_CAP_J=1032,
    CYR_CAP_LJ,
    CYR_CAP_NJ,
    CYR_CAP_TJ,
    CYR_CAP_DZ=1039,
    CYR_CAP_A,
    CYR_CAP_B,
    CYR_CAP_V,
    CYR_CAP_G,
    CYR_CAP_D,
    CYR_CAP_E,
    CYR_CAP_ZH,
    CYR_CAP_Z,
    CYR_CAP_I,
    CYR_CAP_K=1050,
    CYR_CAP_L,
    CYR_CAP_M,
    CYR_CAP_N,
    CYR_CAP_O,
    CYR_CAP_P,
    CYR_CAP_R,
    CYR_CAP_S,
    CYR_CAP_T,
    CYR_CAP_U,
    CYR_CAP_F,
    CYR_CAP_H,
    CYR_CAP_C,
    CYR_CAP_CH,
    CYR_CAP_SH,
    CYR_A=1072,
    CYR_B,
    CYR_V,
    CYR_G,
    CYR_D,
    CYR_E,
    CYR_ZH,
    CYR_Z,
    CYR_I,
    CYR_K=1082,
    CYR_L,
    CYR_M,
    CYR_N,
    CYR_O,
    CYR_P,
    CYR_R,
    CYR_S,
    CYR_T,
    CYR_U,
    CYR_F,
    CYR_H,
    CYR_C,
    CYR_CH,
    CYR_SH,
    CYR_DJ=1106,
    CYR_J=1112,
    CYR_LJ,
    CYR_NJ,
    CYR_TJ,
    CYR_DZ=1119
};

static inline unsigned int
is_upper_case(unsigned int c)
{
    return ((c >= 65 && c <= 90)
            || (c >= CYR_CAP_DJ && c <= CYR_CAP_SH)
            || c == LAT_CAP_TJ
            || c == LAT_CAP_CH
            || c == LAT_CAP_DJ
            || c == LAT_CAP_SH
            || c == LAT_CAP_ZH);
}

static void
str_cat_char(VALUE str, unsigned int c, rb_encoding *enc)
{
    char s[16];
    int n = rb_enc_codelen(c, enc);
    rb_enc_mbcput(c, s, enc);
    rb_enc_str_buf_cat(str, s, n, enc);
}

static VALUE
str_to_latin(int argc, VALUE *argv, VALUE str, int ascii, int bang)
{
    VALUE dest;
    long dest_len;
    char *pos, *end;
    rb_encoding *enc;
    int len, next_len;
    int seen_upper = 0;
    int force_upper = 0;
    unsigned int codepoint = 0;
    unsigned int next_codepoint = 0;

    rb_check_arity(argc, 0, 1);

    pos = RSTRING_PTR(str);
    if (!pos || RSTRING_LEN(str) == 0) return str;

    end = RSTRING_END(str);
    enc = STR_ENC_GET(str);
    dest_len = RSTRING_LEN(str) + 30;
    dest = rb_str_buf_new(dest_len);
    rb_enc_associate(dest, enc);

    codepoint = rb_enc_codepoint_len(pos, end, &len, enc);

    while (pos < end) {
        if (pos + len < end) {
            next_codepoint = rb_enc_codepoint_len(pos + len, end, &next_len, enc);
        }

        force_upper = seen_upper || is_upper_case(next_codepoint);
        seen_upper = is_upper_case(codepoint);

        /* Latin -> "ASCII Latin" conversion */
        if (ascii && codepoint >= LAT_CAP_TJ && codepoint <= LAT_ZH) {
            switch (codepoint) {
            case LAT_TJ:
            case LAT_CH:     rb_enc_str_buf_cat(dest, "c", 1, enc); break;
            case LAT_DJ:     rb_enc_str_buf_cat(dest, "dj", 2, enc); break;
            case LAT_SH:     rb_enc_str_buf_cat(dest, "s", 1, enc); break;
            case LAT_ZH:     rb_enc_str_buf_cat(dest, "z", 1, enc); break;
            case LAT_CAP_TJ:
            case LAT_CAP_CH: rb_enc_str_buf_cat(dest, "C", 1, enc); break;
            case LAT_CAP_SH: rb_enc_str_buf_cat(dest, "S", 1, enc); break;
            case LAT_CAP_ZH: rb_enc_str_buf_cat(dest, "Z", 1, enc); break;

            case LAT_CAP_DJ:
                force_upper ? rb_enc_str_buf_cat(dest, "DJ", 2, enc)
                            : rb_enc_str_buf_cat(dest, "Dj", 2, enc);
                break;
            default:
                rb_enc_str_buf_cat(dest, pos, len, enc);
            }
        }

        /* Non-Cyrillic codepoints */
        else if (codepoint < CYR_CAP_DJ || codepoint > CYR_DZ) {
            rb_enc_str_buf_cat(dest, pos, len, enc);
        }

        /* Cyrillic -> Latin conversion */
        else if (codepoint >= CYR_A) {
            switch (codepoint) {
            case CYR_A:      rb_enc_str_buf_cat(dest, "a",  1, enc); break;
            case CYR_B:      rb_enc_str_buf_cat(dest, "b",  1, enc); break;
            case CYR_V:      rb_enc_str_buf_cat(dest, "v",  1, enc); break;
            case CYR_G:      rb_enc_str_buf_cat(dest, "g",  1, enc); break;
            case CYR_D:      rb_enc_str_buf_cat(dest, "d",  1, enc); break;
            case CYR_E:      rb_enc_str_buf_cat(dest, "e",  1, enc); break;
            case CYR_Z:      rb_enc_str_buf_cat(dest, "z",  1, enc); break;
            case CYR_I:      rb_enc_str_buf_cat(dest, "i",  1, enc); break;
            case CYR_K:      rb_enc_str_buf_cat(dest, "k",  1, enc); break;
            case CYR_L:      rb_enc_str_buf_cat(dest, "l",  1, enc); break;
            case CYR_M:      rb_enc_str_buf_cat(dest, "m",  1, enc); break;
            case CYR_N:      rb_enc_str_buf_cat(dest, "n",  1, enc); break;
            case CYR_O:      rb_enc_str_buf_cat(dest, "o",  1, enc); break;
            case CYR_P:      rb_enc_str_buf_cat(dest, "p",  1, enc); break;
            case CYR_R:      rb_enc_str_buf_cat(dest, "r",  1, enc); break;
            case CYR_S:      rb_enc_str_buf_cat(dest, "s",  1, enc); break;
            case CYR_T:      rb_enc_str_buf_cat(dest, "t",  1, enc); break;
            case CYR_U:      rb_enc_str_buf_cat(dest, "u",  1, enc); break;
            case CYR_F:      rb_enc_str_buf_cat(dest, "f",  1, enc); break;
            case CYR_H:      rb_enc_str_buf_cat(dest, "h",  1, enc); break;
            case CYR_C:      rb_enc_str_buf_cat(dest, "c",  1, enc); break;
            case CYR_J:      rb_enc_str_buf_cat(dest, "j",  1, enc); break;
            case CYR_LJ:     rb_enc_str_buf_cat(dest, "lj", 2, enc); break;
            case CYR_NJ:     rb_enc_str_buf_cat(dest, "nj", 2, enc); break;
            case CYR_DJ:     STR_CAT_COND_ASCII(ascii, dest, "dj", LAT_DJ, 2, enc); break;
            case CYR_TJ:     STR_CAT_COND_ASCII(ascii, dest, "c", LAT_TJ, 1, enc); break;
            case CYR_CH:     STR_CAT_COND_ASCII(ascii, dest, "c", LAT_CH, 1, enc); break;
            case CYR_ZH:     STR_CAT_COND_ASCII(ascii, dest, "z", LAT_ZH, 1, enc); break;
            case CYR_SH:     STR_CAT_COND_ASCII(ascii, dest, "s", LAT_SH, 1, enc); break;
            case CYR_DZ:
                rb_enc_str_buf_cat(dest, "d", 1, enc);
                STR_CAT_COND_ASCII(ascii, dest, "z", LAT_ZH, 1, enc);
                break;
            default:
                rb_enc_str_buf_cat(dest, pos, len, enc);
            }
        }

        /* Cyrillic -> Latin conversion, caps */
        else {
            switch (codepoint) {
            case CYR_CAP_J:  rb_enc_str_buf_cat(dest, "J",  1, enc); break;
            case CYR_CAP_A:  rb_enc_str_buf_cat(dest, "A",  1, enc); break;
            case CYR_CAP_B:  rb_enc_str_buf_cat(dest, "B",  1, enc); break;
            case CYR_CAP_V:  rb_enc_str_buf_cat(dest, "V",  1, enc); break;
            case CYR_CAP_G:  rb_enc_str_buf_cat(dest, "G",  1, enc); break;
            case CYR_CAP_D:  rb_enc_str_buf_cat(dest, "D",  1, enc); break;
            case CYR_CAP_E:  rb_enc_str_buf_cat(dest, "E",  1, enc); break;
            case CYR_CAP_Z:  rb_enc_str_buf_cat(dest, "Z",  1, enc); break;
            case CYR_CAP_I:  rb_enc_str_buf_cat(dest, "I",  1, enc); break;
            case CYR_CAP_K:  rb_enc_str_buf_cat(dest, "K",  1, enc); break;
            case CYR_CAP_L:  rb_enc_str_buf_cat(dest, "L",  1, enc); break;
            case CYR_CAP_M:  rb_enc_str_buf_cat(dest, "M",  1, enc); break;
            case CYR_CAP_N:  rb_enc_str_buf_cat(dest, "N",  1, enc); break;
            case CYR_CAP_O:  rb_enc_str_buf_cat(dest, "O",  1, enc); break;
            case CYR_CAP_P:  rb_enc_str_buf_cat(dest, "P",  1, enc); break;
            case CYR_CAP_R:  rb_enc_str_buf_cat(dest, "R",  1, enc); break;
            case CYR_CAP_S:  rb_enc_str_buf_cat(dest, "S",  1, enc); break;
            case CYR_CAP_T:  rb_enc_str_buf_cat(dest, "T",  1, enc); break;
            case CYR_CAP_U:  rb_enc_str_buf_cat(dest, "U",  1, enc); break;
            case CYR_CAP_F:  rb_enc_str_buf_cat(dest, "F",  1, enc); break;
            case CYR_CAP_H:  rb_enc_str_buf_cat(dest, "H",  1, enc); break;
            case CYR_CAP_C:  rb_enc_str_buf_cat(dest, "C",  1, enc); break;
            case CYR_CAP_TJ: STR_CAT_COND_ASCII(ascii, dest, "C", LAT_CAP_TJ, 1, enc); break;
            case CYR_CAP_CH: STR_CAT_COND_ASCII(ascii, dest, "C", LAT_CAP_CH, 1, enc); break;
            case CYR_CAP_ZH: STR_CAT_COND_ASCII(ascii, dest, "Z", LAT_CAP_ZH, 1, enc); break;
            case CYR_CAP_SH: STR_CAT_COND_ASCII(ascii, dest, "S", LAT_CAP_SH, 1, enc); break;
            case CYR_CAP_LJ:
                rb_enc_str_buf_cat(dest, (force_upper ? "LJ" : "Lj"), 2, enc);
                break;
            case CYR_CAP_NJ:
                rb_enc_str_buf_cat(dest, (force_upper ? "NJ" : "Nj"), 2, enc);
                break;
            case CYR_CAP_DJ:
                STR_CAT_COND_ASCII(ascii, dest, (force_upper ? "DJ" : "Dj"), LAT_CAP_DJ, 2, enc);
                break;
            case CYR_CAP_DZ:
                rb_enc_str_buf_cat(dest, "D", 1, enc);
                if (force_upper) {
                    STR_CAT_COND_ASCII(ascii, dest, "Z", LAT_CAP_ZH, 1, enc);
                }
                else {
                    STR_CAT_COND_ASCII(ascii, dest, "z", LAT_ZH, 1, enc);
                }
                break;
            default:
                rb_enc_str_buf_cat(dest, pos, len, enc);
            }
        }
        pos += len;
        len = next_len;
        codepoint = next_codepoint;
        next_codepoint = 0;
    }

    if (bang) {
        rb_str_shared_replace(str, dest);
    }
    else {
	OBJ_INFECT(dest, str);
	str = dest;
    }

    return str;
}

static VALUE
rb_str_to_latin(int argc, VALUE *argv, VALUE str) {
    return str_to_latin(argc, argv, str, 0, 0);
}

static VALUE
rb_str_to_latin_bang(int argc, VALUE *argv, VALUE str) {
    return str_to_latin(argc, argv, str, 0, 1);
}

static VALUE
rb_str_to_ascii_latin(int argc, VALUE *argv, VALUE str) {
    return str_to_latin(argc, argv, str, 1, 0);
}

static VALUE
rb_str_to_ascii_latin_bang(int argc, VALUE *argv, VALUE str) {
    return str_to_latin(argc, argv, str, 1, 1);
}

void Init_byk_native(void)
{
    rb_define_method(rb_cString, "to_latin",  rb_str_to_latin, -1);
    rb_define_method(rb_cString, "to_latin!", rb_str_to_latin_bang, -1);
    rb_define_method(rb_cString, "to_ascii_latin", rb_str_to_ascii_latin, -1);
    rb_define_method(rb_cString, "to_ascii_latin!", rb_str_to_ascii_latin_bang, -1);
}
