#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/conf.h>


static const unsigned char gcm_key[] = {
	0xee,0xbc,0x1f,0x57,0x48,0x7f,0x51,0x92,0x1c,0x04,0x65,0x66,
	0x5f,0x8a,0xe6,0xd1,0x65,0x8b,0xb2,0x6d,0xe6,0xf8,0xa0,0x69,
	0xa3,0x52,0x02,0x93,0xa5,0x72,0x07,0x8f
};

static const unsigned char gcm_iv[] = {
	0x99,0xaa,0x3e,0x68,0xed,0x81,0x73,0xa0,0xee,0xd0,0x66,0x84
};

static const unsigned char gcm_pt[] = {
	0xf5,0x6e,0x87,0x05,0x5b,0xc3,0x2d,0x0e,0xeb,0x31,0xb2,0xea,
	0xcc,0x2b,0xf2,0xa5
};

static const unsigned char gcm_aad[] = {
	0x4d,0x23,0xc3,0xce,0xc3,0x34,0xb4,0x9b,0xdb,0x37,0x0c,0x43,
	0x7f,0xec,0x78,0xde
};

static const unsigned char gcm_ct[] = {
	0xf7,0x26,0x44,0x13,0xa8,0x4c,0x0e,0x7c,0xd5,0x36,0x86,0x7e,
	0xb9,0xf2,0x17,0x36
};

static const unsigned char gcm_tag[] = {
	0x67,0xba,0x05,0x10,0x26,0x2a,0xe4,0x87,0xd7,0x37,0xee,0x62,
	0x98,0xf7,0x7e,0x0c
};

void aes_gcm_encrypt(void)
	{
	EVP_CIPHER_CTX *ctx;
	int outlen, tmplen;
	unsigned char outbuf[1024];
	printf("AES GCM Encrypt:\n");
	printf("Plaintext:\n");
	BIO_dump_fp(stdout, gcm_pt, sizeof(gcm_pt));
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(gcm_iv), NULL);
	EVP_EncryptInit_ex(ctx, NULL, NULL, gcm_key, gcm_iv);
	EVP_EncryptUpdate(ctx, NULL, &outlen, gcm_aad, sizeof(gcm_aad));
	EVP_EncryptUpdate(ctx, outbuf, &outlen, gcm_pt, sizeof(gcm_pt));
	printf("Ciphertext:\n");
	BIO_dump_fp(stdout, outbuf, outlen);
	EVP_EncryptFinal_ex(ctx, outbuf, &outlen);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, outbuf);
	EVP_CIPHER_CTX_free(ctx);
	}
void aes_gcm_decrypt(void)
	{
	EVP_CIPHER_CTX *ctx;
	int outlen, tmplen, rv;
	unsigned char outbuf[1024];
	printf("AES GCM Derypt:\n");
	printf("Ciphertext:\n");
	BIO_dump_fp(stdout, gcm_ct, sizeof(gcm_ct));
	ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(gcm_iv), NULL);
	EVP_DecryptInit_ex(ctx, NULL, NULL, gcm_key, gcm_iv);
	
	EVP_DecryptUpdate(ctx, NULL, &outlen, gcm_aad, sizeof(gcm_aad));

	EVP_DecryptUpdate(ctx, outbuf, &outlen, gcm_ct, sizeof(gcm_ct));

	printf("Plaintext:\n");
	BIO_dump_fp(stdout, outbuf, outlen);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, sizeof(gcm_tag), gcm_tag);
	rv = EVP_DecryptFinal_ex(ctx, outbuf, &outlen);
		EVP_CIPHER_CTX_free(ctx);
	}

int main(int argc, char **argv)
	{
	aes_gcm_encrypt();
	aes_gcm_decrypt();
	}
