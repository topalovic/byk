#include <ruby.h>
#include <ruby/encoding.h>

#define STR_ENC_GET(str) rb_enc_from_index(ENCODING_GET(str))

static inline void
_str_cat_char(VALUE str, unsigned c, rb_encoding *enc)
{
    char s[16];
    int n = rb_enc_codelen(c, enc);
    rb_enc_mbcput(c, s, enc);
    rb_str_buf_cat(str, s, n);
}

enum {
    LAT_CAP_TJ=262, LAT_TJ, LAT_CAP_CH=268, LAT_CH,
    LAT_CAP_DJ=272, LAT_DJ, LAT_CAP_SH=352, LAT_SH,
    LAT_CAP_ZH=381, LAT_ZH, CYR_CAP_DJ=1026, CYR_CAP_J=1032,
    CYR_CAP_LJ, CYR_CAP_NJ, CYR_CAP_TJ, CYR_CAP_DZ=1039,
    CYR_CAP_A, CYR_CAP_B, CYR_CAP_V, CYR_CAP_G,
    CYR_CAP_D, CYR_CAP_E, CYR_CAP_ZH, CYR_CAP_Z,
    CYR_CAP_I, CYR_CAP_K=1050, CYR_CAP_L, CYR_CAP_M,
    CYR_CAP_N, CYR_CAP_O, CYR_CAP_P, CYR_CAP_R,
    CYR_CAP_S, CYR_CAP_T, CYR_CAP_U, CYR_CAP_F,
    CYR_CAP_H, CYR_CAP_C, CYR_CAP_CH, CYR_CAP_SH,
    CYR_A=1072, CYR_B, CYR_V, CYR_G, CYR_D,
    CYR_E, CYR_ZH, CYR_Z, CYR_I, CYR_K=1082,
    CYR_L, CYR_M, CYR_N, CYR_O, CYR_P,
    CYR_R, CYR_S, CYR_T, CYR_U, CYR_F,
    CYR_H, CYR_C, CYR_CH, CYR_SH, CYR_DJ=1106,
    CYR_J=1112, CYR_LJ, CYR_NJ, CYR_TJ, CYR_DZ=1119
};

static inline unsigned
is_cap(unsigned codepoint)
{
    if (codepoint >= 65 && codepoint <= 90) return 1;
    if (codepoint >= CYR_CAP_DJ && codepoint <= CYR_CAP_SH) return 1;

    switch(codepoint) {
    case LAT_CAP_TJ:
    case LAT_CAP_CH:
    case LAT_CAP_DJ:
    case LAT_CAP_SH:
    case LAT_CAP_ZH:
        return 1;
    default:
        return 0;
    }
}

static inline unsigned
is_digraph(unsigned codepoint)
{
    switch(codepoint) {
    case CYR_LJ:
    case CYR_NJ:
    case CYR_DZ:
    case CYR_CAP_LJ:
    case CYR_CAP_NJ:
    case CYR_CAP_DZ:
        return 1;
    default:
        return 0;
    }
}

static unsigned
digraph_to_cyr(unsigned codepoint, unsigned codepoint2, unsigned capitalize, unsigned *next_out)
{
    static unsigned CYR_MAP[] = {
        CYR_A, CYR_B, CYR_C, CYR_D, CYR_E, CYR_F,
        CYR_G, CYR_H, CYR_I, CYR_J, CYR_K, CYR_L,
        CYR_M, CYR_N, CYR_O, CYR_P, 0, CYR_R,
        CYR_S, CYR_T, CYR_U, CYR_V, 0, 0, 0, CYR_Z
    };

    static unsigned CYR_CAPS_MAP[] = {
        CYR_CAP_A, CYR_CAP_B, CYR_CAP_C, CYR_CAP_D, CYR_CAP_E, CYR_CAP_F,
        CYR_CAP_G, CYR_CAP_H, CYR_CAP_I, CYR_CAP_J, CYR_CAP_K, CYR_CAP_L,
        CYR_CAP_M, CYR_CAP_N, CYR_CAP_O, CYR_CAP_P, 0, CYR_CAP_R,
        CYR_CAP_S, CYR_CAP_T, CYR_CAP_U, CYR_CAP_V, 0, 0, 0, CYR_CAP_Z
    };

    if (codepoint2 == LAT_CAP_ZH || codepoint2 == LAT_ZH) {
        switch (codepoint) {
        case 'd': return CYR_DZ;
        case 'D': return CYR_CAP_DZ;
        }
    }

    if (codepoint2 == 'j' || codepoint2 == 'J') {
        switch (codepoint) {
        case 'l': return CYR_LJ;
        case 'n': return CYR_NJ;
        case 'L': return CYR_CAP_LJ;
        case 'N': return CYR_CAP_NJ;
        }
    }

    if (codepoint >= 'a' && codepoint <= 'z') return CYR_MAP[codepoint - 'a'];
    if (codepoint >= 'A' && codepoint <= 'Z') return CYR_CAPS_MAP[codepoint - 'A'];

    switch (codepoint) {
    case LAT_CH: return CYR_CH;
    case LAT_DJ: return CYR_DJ;
    case LAT_SH: return CYR_SH;
    case LAT_TJ: return CYR_TJ;
    case LAT_ZH: return CYR_ZH;
    case LAT_CAP_CH: return CYR_CAP_CH;
    case LAT_CAP_DJ: return CYR_CAP_DJ;
    case LAT_CAP_SH: return CYR_CAP_SH;
    case LAT_CAP_TJ: return CYR_CAP_TJ;
    case LAT_CAP_ZH: return CYR_CAP_ZH;
    }

    return 0;
}

static unsigned
digraph_to_latin(unsigned codepoint, unsigned codepoint2, unsigned capitalize, unsigned *next_out)
{
    static char LAT_MAP[] = {
        'a', 'b', 'v', 'g', 'd', 'e', 0, 'z', 'i', 0, 'k', 'l',
        'm', 'n', 'o', 'p', 'r', 's', 't', 'u', 'f', 'h', 'c'
    };

    static char LAT_CAPS_MAP[] = {
        'A', 'B', 'V', 'G', 'D', 'E', 0, 'Z', 'I', 0, 'K', 'L',
        'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'F', 'H', 'C'
    };

    if (codepoint < CYR_CAP_DJ || codepoint > CYR_DZ) return 0;

    switch (codepoint) {
    case CYR_ZH: return LAT_ZH;
    case CYR_CAP_ZH: return LAT_CAP_ZH;
    }

    if (codepoint >= CYR_A && codepoint <= CYR_C)
        return LAT_MAP[codepoint - CYR_A];

    if (codepoint >= CYR_CAP_A && codepoint <= CYR_CAP_C)
        return LAT_CAPS_MAP[codepoint - CYR_CAP_A];

    if (codepoint >= CYR_A) {
        switch (codepoint) {
        case CYR_J:  return 'j';
        case CYR_TJ: return LAT_TJ;
        case CYR_CH: return LAT_CH;
        case CYR_SH: return LAT_SH;
        case CYR_DJ: return LAT_DJ;
        case CYR_LJ: *next_out = 'j'; return 'l';
        case CYR_NJ: *next_out = 'j'; return 'n';
        case CYR_DZ: *next_out = LAT_ZH; return 'd';
        }
    }
    else {
        switch (codepoint) {
        case CYR_CAP_J:  return 'J';
        case CYR_CAP_TJ: return LAT_CAP_TJ;
        case CYR_CAP_CH: return LAT_CAP_CH;
        case CYR_CAP_SH: return LAT_CAP_SH;
        case CYR_CAP_DJ: return LAT_CAP_DJ;
        case CYR_CAP_LJ: *next_out = (capitalize || is_cap(codepoint2)) ? 'J' : 'j'; return 'L';
        case CYR_CAP_NJ: *next_out = (capitalize || is_cap(codepoint2)) ? 'J' : 'j'; return 'N';
        case CYR_CAP_DZ: *next_out = (capitalize || is_cap(codepoint2)) ? LAT_CAP_ZH : LAT_ZH; return 'D';
        }
    }

    return 0;
}

static unsigned
digraph_to_ascii(unsigned codepoint, unsigned codepoint2, unsigned capitalize, unsigned *next_out)
{
    switch (codepoint) {
    case LAT_TJ:
    case LAT_CH:
    case CYR_TJ:
    case CYR_CH: return 'c';
    case LAT_SH:
    case CYR_SH: return 's';
    case LAT_ZH:
    case CYR_ZH: return 'z';
    case LAT_DJ:
    case CYR_DJ: *next_out = 'j'; return 'd';
    case LAT_CAP_TJ:
    case LAT_CAP_CH:
    case CYR_CAP_TJ:
    case CYR_CAP_CH: return 'C';
    case LAT_CAP_SH:
    case CYR_CAP_SH: return 'S';
    case LAT_CAP_ZH:
    case CYR_CAP_ZH: return 'Z';
    case LAT_CAP_DJ:
    case CYR_CAP_DJ:
        *next_out = (capitalize || is_cap(codepoint2)) ? 'J' : 'j'; return 'D';
    case CYR_DZ:
        *next_out = (capitalize || is_cap(codepoint2)) ? 'Z' : 'z'; return 'd';
    case CYR_CAP_DZ:
        *next_out = (capitalize || is_cap(codepoint2)) ? 'Z' : 'z'; return 'D';
    default:
        return digraph_to_latin(codepoint, codepoint2, capitalize, next_out);
    }
}

static VALUE
str_to_srb(VALUE str, int strategy, int bang)
{
    VALUE dest;
    rb_encoding *enc;

    int len, next_len;
    unsigned in, in2, out, out2, seen_cap = 0;
    char *pos, *end, *seq_start = 0;

    unsigned (*method)(unsigned, unsigned, unsigned, unsigned*);

    switch(strategy) {
    case 0:  method = &digraph_to_cyr;   break;
    case 1:  method = &digraph_to_latin; break;
    default: method = &digraph_to_ascii;
    }

    StringValue(str);
    pos = RSTRING_PTR(str);
    if (!pos || RSTRING_LEN(str) == 0) return str;

    end = RSTRING_END(str);
    enc = STR_ENC_GET(str);
    dest = rb_str_buf_new(RSTRING_LEN(str) + 30);
    rb_enc_associate(dest, enc);

    in = rb_enc_codepoint_len(pos, end, &len, enc);

    while (pos < end) {
        in2 = out2 = 0;

        if (pos + len < end)
            in2 = rb_enc_codepoint_len(pos + len, end, &next_len, enc);

        out = (*method)(in, in2, seen_cap, &out2);

        if (out) {
            /* flush previous untranslatable sequence */
            if (seq_start) {
                rb_str_buf_cat(dest, seq_start, pos - seq_start);
                seq_start = 0;
            }

            _str_cat_char(dest, out, enc);
            if (out2) _str_cat_char(dest, out2, enc);
        }
        else if (!seq_start) {
            /* mark the beginning of an untranslatable sequence */
            seq_start = pos;
        }

        /* for cyrillic output, skip the second half of an input digraph */
        if (strategy == 0 && is_digraph(out)) {
            pos += next_len;
            if (pos + len < end)
                in2 = rb_enc_codepoint_len(pos + len, end, &next_len, enc);
        }

        seen_cap = is_cap(in);

        pos += len;
        len = next_len;
        in = in2;
    }

    /* flush final sequence */
    if (seq_start) rb_str_buf_cat(dest, seq_start, pos - seq_start);

    if (bang) {
        rb_str_shared_replace(str, dest);
    }
    else {
        str = dest;
    }

    return str;
}

/**
 * Returns a copy of <i>str</i> with Latin characters transliterated
 * into Serbian Cyrillic.
 *
 * @overload to_cyrillic(str)
 *   @param  [String] str text to be transliterated
 *   @return [String] transliterated text
 */
static VALUE
rb_str_to_cyrillic(VALUE self, VALUE str)
{
    return str_to_srb(str, 0, 0);
}

/**
 * Performs transliteration of <code>Byk.to_cyrillic</code> in place,
 * returning <i>str</i>, whether any changes were made or not.
 *
 * @overload to_cyrillic!(str)
 *   @param  [String] str text to be transliterated
 *   @return [String] transliterated text
 */
static VALUE
rb_str_to_cyrillic_bang(VALUE self, VALUE str)
{
    return str_to_srb(str, 0, 1);
}

/**
 * Returns a copy of <i>str</i> with Serbian Cyrillic characters
 * transliterated into Latin.
 *
 * @overload to_latin(str)
 *   @param  [String] str text to be transliterated
 *   @return [String] transliterated text
 */
static VALUE
rb_str_to_latin(VALUE self, VALUE str)
{
    return str_to_srb(str, 1, 0);
}

/**
 * Performs transliteration of <code>Byk.to_latin</code> in place,
 * returning <i>str</i>, whether any changes were made or not.
 *
 * @overload to_latin!(str)
 *   @param  [String] str text to be transliterated
 *   @return [String] transliterated text
 */
static VALUE
rb_str_to_latin_bang(VALUE self, VALUE str)
{
    return str_to_srb(str, 1, 1);
}

/**
 * Returns a copy of <i>str</i> with Serbian characters transliterated
 * into ASCII Latin.
 *
 * @overload to_ascii_latin(str)
 *   @param  [String] str text to be transliterated
 *   @return [String] transliterated text
 */
static VALUE
rb_str_to_ascii_latin(VALUE self, VALUE str)
{
    return str_to_srb(str, 2, 0);
}

/**
 * Performs transliteration of <code>Byk.to_ascii_latin</code> in
 * place, returning <i>str</i>, whether any changes were made or not.
 *
 * @overload to_ascii_latin!(str)
 *   @param  [String] str text to be transliterated
 *   @return [String] transliterated text
 */
static VALUE
rb_str_to_ascii_latin_bang(VALUE self, VALUE str)
{
    return str_to_srb(str, 2, 1);
}

void Init_byk_native(void)
{
    VALUE Byk = rb_define_module("Byk");
    rb_define_singleton_method(Byk, "to_cyrillic", rb_str_to_cyrillic, 1);
    rb_define_singleton_method(Byk, "to_cyrillic!", rb_str_to_cyrillic_bang, 1);
    rb_define_singleton_method(Byk, "to_latin", rb_str_to_latin, 1);
    rb_define_singleton_method(Byk, "to_latin!", rb_str_to_latin_bang, 1);
    rb_define_singleton_method(Byk, "to_ascii_latin", rb_str_to_ascii_latin, 1);
    rb_define_singleton_method(Byk, "to_ascii_latin!", rb_str_to_ascii_latin_bang, 1);
}
