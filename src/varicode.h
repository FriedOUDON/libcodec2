/*
  Lightweight text bitstream codec used by FreeDV framing paths.
  This project is distributed under LGPL-2.1 (see COPYING).
*/

#ifndef __VARICODE__
#define __VARICODE__

#ifdef __cplusplus
extern "C" {
#endif

/*
  1 start bit + 8 data bits + 2 stop bits.
  Keep this constant for ABI compatibility with existing callers that size buffers
  using VARICODE_MAX_BITS.
*/
#define VARICODE_MAX_BITS (10 + 2)

struct VARICODE_DEC {
    int            state;
    int            n_zeros;
    int            v_len;
    unsigned short packed;
    int            code_num;
    int            n_in;
    int            in[2];
};

int varicode_encode(short varicode_out[], char ascii_in[], int max_out, int n_in, int code_num);
void varicode_decode_init(struct VARICODE_DEC *dec_states, int code_num);
int varicode_decode(struct VARICODE_DEC *dec_states, char ascii_out[], short varicode_in[], int max_out, int n_in);
void varicode_set_code_num(struct VARICODE_DEC *dec_states, int code_num);

#ifdef __cplusplus
}
#endif

#endif
