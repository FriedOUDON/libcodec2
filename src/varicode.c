/*
  Lightweight text bitstream codec used by FreeDV framing paths.
  This project is distributed under LGPL-2.1 (see COPYING).
*/

#include "varicode.h"

enum {
    VARICODE_SYNC = 0,
    VARICODE_PAYLOAD = 1,
    VARICODE_STOP = 2
};

static int normalize_code_num(int code_num) {
    if (code_num == 2) {
        return 2;
    }
    return 1;
}

static void reset_decoder_state(struct VARICODE_DEC *dec_states) {
    dec_states->state = VARICODE_SYNC;
    dec_states->n_zeros = 0;
    dec_states->v_len = 0;
    dec_states->packed = 0;
    dec_states->n_in = 0;
    dec_states->in[0] = 0;
    dec_states->in[1] = 0;
}

int varicode_encode(short varicode_out[], char ascii_in[], int max_out, int n_in, int code_num) {
    int i;
    int n_out = 0;

    (void)code_num;

    if (!varicode_out || !ascii_in || max_out <= 0 || n_in <= 0) {
        return 0;
    }

    for (i = 0; i < n_in; i++) {
        int bit;
        unsigned char ch = (unsigned char)ascii_in[i];

        /* Frame format: 1 start + 8 data + 00 stop. */
        if ((n_out + 11) > max_out) {
            break;
        }

        varicode_out[n_out++] = 1;
        for (bit = 7; bit >= 0; bit--) {
            varicode_out[n_out++] = (short)((ch >> bit) & 1U);
        }
        varicode_out[n_out++] = 0;
        varicode_out[n_out++] = 0;
    }

    return n_out;
}

void varicode_decode_init(struct VARICODE_DEC *dec_states, int code_num) {
    if (!dec_states) {
        return;
    }

    dec_states->code_num = normalize_code_num(code_num);
    reset_decoder_state(dec_states);
}

void varicode_set_code_num(struct VARICODE_DEC *dec_states, int code_num) {
    if (!dec_states) {
        return;
    }

    dec_states->code_num = normalize_code_num(code_num);
    reset_decoder_state(dec_states);
}

static int decode_one_bit(struct VARICODE_DEC *dec_states, short varicode_in, char *single_ascii) {
    int bit = (varicode_in != 0) ? 1 : 0;

    switch (dec_states->state) {
    case VARICODE_SYNC:
        if (bit) {
            dec_states->state = VARICODE_PAYLOAD;
            dec_states->packed = 0;
            dec_states->v_len = 0;
        }
        break;

    case VARICODE_PAYLOAD:
        dec_states->packed = (unsigned short)((dec_states->packed << 1) | (unsigned short)bit);
        dec_states->v_len++;
        if (dec_states->v_len >= 8) {
            dec_states->state = VARICODE_STOP;
            dec_states->n_zeros = 0;
        }
        break;

    case VARICODE_STOP:
        if (!bit) {
            dec_states->n_zeros++;
            if (dec_states->n_zeros >= 2) {
                *single_ascii = (char)(dec_states->packed & 0xFFU);
                reset_decoder_state(dec_states);
                return 1;
            }
        } else {
            /* Framing error: treat this bit as the next frame start bit. */
            dec_states->state = VARICODE_PAYLOAD;
            dec_states->packed = 0;
            dec_states->v_len = 0;
            dec_states->n_zeros = 0;
        }
        break;

    default:
        reset_decoder_state(dec_states);
        break;
    }

    return 0;
}

int varicode_decode(struct VARICODE_DEC *dec_states, char ascii_out[], short varicode_in[], int max_out, int n_in) {
    int n_out = 0;

    if (!dec_states || !ascii_out || !varicode_in || max_out <= 0 || n_in <= 0) {
        return 0;
    }

    while (n_in && (n_out < max_out)) {
        char single_ascii = 0;
        if (decode_one_bit(dec_states, varicode_in[0], &single_ascii)) {
            ascii_out[n_out++] = single_ascii;
        }
        varicode_in++;
        n_in--;
    }

    return n_out;
}
