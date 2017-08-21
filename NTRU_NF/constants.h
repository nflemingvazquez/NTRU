#pragma once
//
// ees443ep1 set of parameters
// believed to be suitable for 128 bit pre/post-quantum security level
# define ees_N 443
# define ees_p 3
# define ees_q 2048
# define ees_s 2 // own parameter
# define ees_r 11 // q = s^r
# define ees_df1 9
# define ees_df2 8
# define ees_df3 5
# define ees_dg 148
#define ees_lLen 1 // number of bytes required to represent message length
#define ees_db 256
#define ees_maxMsgLenBytes 49
#define ees_bufferLenBits 656
#define ees_dm0 115
#define ees_dr1 9
#define ees_dr2 8
#define ees_dr3 5
#define ees_c 9
#define ees_minCallsR 8
#define ees_minCallsMask 5

#define ees_bLen 666 // lowest multiple of 3 larger than message size in bits