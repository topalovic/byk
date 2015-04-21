#include <ruby.h>
#include <ruby/encoding.h>

#define STR_ENC_GET(str) rb_enc_from_index(ENCODING_GET(str))

#define STR_CAT_COND_ASCII(ascii, dest, chr, ascii_chr, len, enc)       \
    ascii ? rb_str_buf_cat(dest, chr, len)                              \
          : str_cat_char(dest, ascii_chr, enc)

enum {
    LAT_CAP_TJ = 0x106,
    LAT_TJ,
    LAT_CAP_CH = 0x10c,
    LAT_CH,
    LAT_CAP_DJ = 0x110,
    LAT_DJ,
    LAT_CAP_SH = 0x160,
    LAT_SH,
    LAT_CAP_ZH = 0x17d,
    LAT_ZH,
    CYR_CAP_DJ = 0x402,
    CYR_CAP_J  = 0x408,
    CYR_CAP_LJ,
    CYR_CAP_NJ,
    CYR_CAP_TJ,
    CYR_CAP_DZ = 0x40f,
    CYR_CAP_A,
    CYR_CAP_ZH = 0x416,
    CYR_CAP_C  = 0x426,
    CYR_CAP_CH,
    CYR_CAP_SH,
    CYR_A  = 0x430,
    CYR_ZH = 0x436,
    CYR_C  = 0x446,
    CYR_CH,
    CYR_SH,
    CYR_DJ = 0x452,
    CYR_J  = 0x458,
    CYR_LJ,
    CYR_NJ,
    CYR_TJ,
    CYR_DZ = 0x45f
};

static inline unsigned int
is_cyrillic(unsigned int c)
{
    return c >= CYR_CAP_DJ && c <= CYR_DZ;
}

static inline unsigned int
is_upper(unsigned int c)
{
    return (c >= 65 && c <= 90)
        || (c >= CYR_CAP_DJ && c <= CYR_CAP_SH)
        || c == LAT_CAP_TJ
        || c == LAT_CAP_CH
        || c == LAT_CAP_DJ
        || c == LAT_CAP_SH
        || c == LAT_CAP_ZH;
}

static inline unsigned int
maps_directly(unsigned int c)
{
    return c != CYR_ZH
        && c != CYR_CAP_ZH
        && ((c >= CYR_A && c <= CYR_C) || (c >= CYR_CAP_A && c <= CYR_CAP_C));
}

static void
str_cat_char(VALUE str, unsigned int c, rb_encoding *enc)
{
    char s[16];
    int n = rb_enc_codelen(c, enc);
    rb_enc_mbcput(c, s, enc);
    rb_str_buf_cat(str, s, n);
}

static VALUE
str_to_latin(VALUE str, int ascii, int bang)
{
    VALUE dest;
    long dest_len;
    int len, next_len;
    int seen_upper = 0;
    int force_upper = 0;
    char *pos = RSTRING_PTR(str);
    char *end, *seq_start = 0;
    char cyr;
    unsigned int codepoint = 0;
    unsigned int next_codepoint = 0;
    rb_encoding *enc;

    char CYR_MAP[] = {
        'a', 'b', 'v', 'g', 'd', 'e', '\0', 'z', 'i', '\0', 'k',
        'l', 'm', 'n', 'o', 'p', 'r', 's', 't', 'u', 'f', 'h', 'c'
    };

    char CYR_CAPS_MAP[] = {
        'A', 'B', 'V', 'G', 'D', 'E', '\0', 'Z', 'I', '\0', 'K',
        'L', 'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'F', 'H', 'C'
    };

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

        /* Latin -> "ASCII Latin" conversion */
        if (ascii && codepoint >= LAT_CAP_TJ && codepoint <= LAT_ZH) {
            if (seq_start) {
                rb_str_buf_cat(dest, seq_start, pos - seq_start);
                seq_start = 0;
            }

            switch (codepoint) {
            case LAT_TJ:
            case LAT_CH:     rb_str_buf_cat(dest, "c",  1); break;
            case LAT_DJ:     rb_str_buf_cat(dest, "dj", 2); break;
            case LAT_SH:     rb_str_buf_cat(dest, "s",  1); break;
            case LAT_ZH:     rb_str_buf_cat(dest, "z",  1); break;
            case LAT_CAP_TJ:
            case LAT_CAP_CH: rb_str_buf_cat(dest, "C",  1); break;
            case LAT_CAP_SH: rb_str_buf_cat(dest, "S",  1); break;
            case LAT_CAP_ZH: rb_str_buf_cat(dest, "Z",  1); break;
            case LAT_CAP_DJ:
                (seen_upper || is_upper(next_codepoint))
                    ? rb_str_buf_cat(dest, "DJ", 2)
                    : rb_str_buf_cat(dest, "Dj", 2);
                break;
            default:
                rb_str_buf_cat(dest, pos, len);
            }
        }

        /* Cyrillic coderange */
        else if (is_cyrillic(codepoint)) {
            if (seq_start) {
                rb_str_buf_cat(dest, seq_start, pos - seq_start);
                seq_start = 0;
            }

            if (codepoint >= CYR_A) {
                if (maps_directly(codepoint)) {
                    cyr = CYR_MAP[codepoint - CYR_A];
                    cyr ? rb_str_buf_cat(dest, &cyr, 1)
                        : rb_str_buf_cat(dest, pos, len);
                }
                else {
                    switch (codepoint) {
                    case CYR_J:  rb_str_buf_cat(dest, "j",  1); break;
                    case CYR_LJ: rb_str_buf_cat(dest, "lj", 2); break;
                    case CYR_NJ: rb_str_buf_cat(dest, "nj", 2); break;
                    case CYR_DJ: STR_CAT_COND_ASCII(ascii, dest, "dj", LAT_DJ, 2, enc); break;
                    case CYR_TJ: STR_CAT_COND_ASCII(ascii, dest, "c",  LAT_TJ, 1, enc); break;
                    case CYR_CH: STR_CAT_COND_ASCII(ascii, dest, "c",  LAT_CH, 1, enc); break;
                    case CYR_SH: STR_CAT_COND_ASCII(ascii, dest, "s",  LAT_SH, 1, enc); break;
                    case CYR_ZH: STR_CAT_COND_ASCII(ascii, dest, "z",  LAT_ZH, 1, enc); break;
                    case CYR_DZ:
                        rb_str_buf_cat(dest, "d", 1);
                        STR_CAT_COND_ASCII(ascii, dest, "z", LAT_ZH, 1, enc);
                        break;
                    default:
                        rb_str_buf_cat(dest, pos, len);
                    }
                }
            }
            else {
                if (maps_directly(codepoint)) {
                    cyr = CYR_CAPS_MAP[codepoint - CYR_CAP_A];
                    cyr ? rb_str_buf_cat(dest, &cyr, 1)
                        : rb_str_buf_cat(dest, pos, len);
                }
                else {
                    force_upper = seen_upper || is_upper(next_codepoint);

                    switch (codepoint) {
                    case CYR_CAP_J:  rb_str_buf_cat(dest, "J", 1); break;
                    case CYR_CAP_LJ: rb_str_buf_cat(dest, (force_upper ? "LJ" : "Lj"), 2); break;
                    case CYR_CAP_NJ: rb_str_buf_cat(dest, (force_upper ? "NJ" : "Nj"), 2); break;
                    case CYR_CAP_TJ: STR_CAT_COND_ASCII(ascii, dest, "C", LAT_CAP_TJ, 1, enc); break;
                    case CYR_CAP_CH: STR_CAT_COND_ASCII(ascii, dest, "C", LAT_CAP_CH, 1, enc); break;
                    case CYR_CAP_SH: STR_CAT_COND_ASCII(ascii, dest, "S", LAT_CAP_SH, 1, enc); break;
                    case CYR_CAP_ZH: STR_CAT_COND_ASCII(ascii, dest, "Z", LAT_CAP_ZH, 1, enc); break;
                    case CYR_CAP_DJ: STR_CAT_COND_ASCII(ascii, dest, (force_upper ? "DJ" : "Dj"), LAT_CAP_DJ, 2, enc); break;
                    case CYR_CAP_DZ:
                        rb_str_buf_cat(dest, "D", 1);
                        force_upper ? STR_CAT_COND_ASCII(ascii, dest, "Z", LAT_CAP_ZH, 1, enc)
                                    : STR_CAT_COND_ASCII(ascii, dest, "z", LAT_ZH, 1, enc);
                        break;
                    default:
                        rb_str_buf_cat(dest, pos, len);
                    }
                }
            }
        }
        else {
            /* Mark the start of a copyable sequence */
            if (!seq_start) seq_start = pos;
        }

        seen_upper = is_upper(codepoint);

        pos += len;
        len = next_len;

        codepoint = next_codepoint;
        next_codepoint = 0;
    }

    /* Flush the last sequence, if any */
    if (seq_start) {
        rb_str_buf_cat(dest, seq_start, pos - seq_start);
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
rb_str_to_latin(VALUE str)
{
    return str_to_latin(str, 0, 0);
}

static VALUE
rb_str_to_latin_bang(VALUE str)
{
    return str_to_latin(str, 0, 1);
}

static VALUE
rb_str_to_ascii_latin(VALUE str)
{
    return str_to_latin(str, 1, 0);
}

static VALUE
rb_str_to_ascii_latin_bang(VALUE str)
{
    return str_to_latin(str, 1, 1);
}

void Init_byk_native(void)
{
    rb_define_method(rb_cString, "to_latin", rb_str_to_latin, 0);
    rb_define_method(rb_cString, "to_latin!", rb_str_to_latin_bang, 0);
    rb_define_method(rb_cString, "to_ascii_latin", rb_str_to_ascii_latin, 0);
    rb_define_method(rb_cString, "to_ascii_latin!", rb_str_to_ascii_latin_bang, 0);
}
