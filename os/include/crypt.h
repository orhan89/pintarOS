#include <config.h>
#include <tea.h>

//! Length of key in octets.
#define CRYPT_KEY_LEN	TEA_KEY_LEN
//! Length of cipher block in octets.
#define CRYPT_BLOCK_LEN	TEA_BLOCK_LEN
//! Single block encryption function.
#define crypt_enc(v,k) tea_enc((iu32*)(v),(iu32*)(k))
//! Single block decryption function.
#define crypt_dec(v,k) tea_dec((iu32*)(v),(iu32*)(k))

