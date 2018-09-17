
#include "common/RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoFile.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "common/Tokenizer.h"

#include "ruby/ext/rho/rhoruby.h"
#include <algorithm>

using namespace rho;
using namespace rho::common;

#include "openssl/evp.h"
#include "openssl/aes.h"
#include "openssl/err.h"
#include "openssl/sha.h"


#include <stdio.h>
#include <string.h>
#include "openssl/bio.h"
#include "openssl/evp.h"
#include <stdint.h>
#include <assert.h>

#include "logging/RhoLog.h"


#define DEFAULT_LOGCATEGORY "Development_HTTPServer"

size_t calcDecodeLength(const char* b64input) {
    size_t len = strlen(b64input),
    padding = 0;

    if (b64input[len-1] == '=' && b64input[len-2] == '=')
        padding = 2;
    else if (b64input[len-1] == '=')
        padding = 1;

    return (len*3)/4 - padding;
}

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length) {
    BIO *bio, *b64;

    int decodeLen = calcDecodeLength(b64message);
    *buffer = (unsigned char*)malloc((decodeLen + 1));
    (*buffer)[decodeLen] = '\0';

    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    if (strlen(b64message) <= 60) {
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    }
    *length = BIO_read(bio, *buffer, strlen(b64message));
    //assert(*length == decodeLen);
    BIO_free_all(bio);

    return (0); //success
}

int checkHashSHA1(const unsigned char* data, const unsigned char* in_hash, size_t len)
{
    SHA_CTX sha1h;
    unsigned char out_hash[SHA_DIGEST_LENGTH] = { 0 };
    SHA1_Init(&sha1h);
    SHA1_Update(&sha1h, data, len);
    SHA1_Final(out_hash, &sha1h);
	
    return !memcmp(in_hash, out_hash, SHA_DIGEST_LENGTH);
}




void handleErrors(void)
{
    unsigned long errCode;

    printf("OpenSSL decryption error occurred\n");
    while(errCode = ERR_get_error())
    {
        char *err = ERR_error_string(errCode, NULL);

        const char* err_lib = ERR_lib_error_string(ERR_GET_LIB(errCode));
        const char* err_func = ERR_func_error_string(ERR_GET_FUNC(errCode));
        const char* err_reason = ERR_reason_error_string(ERR_GET_REASON(errCode));

        printf("%s\n", err);
        RAWTRACE("OPenSSL decryption error:");
        RAWTRACE(err);
        //LOG(TRACE) + "OpenSSL ERROR: " +err;
    }
    //abort();
}


void handleOpenSSLErrors() {
    handleErrors();
}

int tau_decrypt_AES(unsigned char *ciphertext,
                    int ciphertext_len,
                    unsigned char *key,
                    unsigned char *iv,
                    unsigned char* decoded_buf,
                    size_t* decoded_len) {

    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    bzero(decoded_buf,ciphertext_len+1);

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleOpenSSLErrors();

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleOpenSSLErrors();

    EVP_CIPHER_CTX_set_key_length(ctx, EVP_MAX_KEY_LENGTH);

    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_DecryptUpdate(ctx, decoded_buf, &len, ciphertext, ciphertext_len))
        handleOpenSSLErrors();

    plaintext_len = len;

    /* Finalize the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    int pad_len;
    if(1 != EVP_DecryptFinal_ex(ctx, decoded_buf + len, &len)) handleOpenSSLErrors();
    plaintext_len += len;

    /* Add the null terminator */
    (decoded_buf)[plaintext_len] = 0;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    //rho::String ret = (char*)plaintext;
    //delete [] plaintext;
    //return ret;
    *decoded_len = plaintext_len;
    return 0;
}



extern "C" int tau_decrypt_file(const char* filebuf, int filebuf_len, char* decrypted_buf, int maxlen, const char* key) {
    String str_filebuf = filebuf;

    unsigned char* iv = (unsigned char*)filebuf;
	const unsigned char* hash = (unsigned char*)filebuf + 16;
    unsigned char* data = (unsigned char*)filebuf + 16 + SHA_DIGEST_LENGTH;

    unsigned char* unpacked_key = NULL;
    size_t unpacked_key_len = 0;
    Base64Decode((char*)key, &unpacked_key, &unpacked_key_len);

    size_t decrypted_data_len = 0;

    //int res = tau_decrypt_AES(data, filebuf_len - 16 - SHA_DIGEST_LENGTH, unpacked_key, iv, (unsigned char*)decrypted_buf, &decrypted_data_len);
	int res = tau_decrypt_AES(data, filebuf_len - 16 - SHA_DIGEST_LENGTH, unpacked_key, iv, (unsigned char*)decrypted_buf, &decrypted_data_len);
	
	if(!checkHashSHA1((const unsigned char*)decrypted_buf, hash, decrypted_data_len))	
		RAWTRACE("Integrity check failed!\n");
		

    if (unpacked_key != NULL) {
        free(unpacked_key);
    }

    return decrypted_data_len;

}

/*extern "C" void tau_encrypt_file_simple(unsigned char *indata, int size, char * outputFileName, const char * ckey)
{
    int outLen1 = 0; 
    int outLen2 = 0;

    unsigned char *outdata = new unsigned char[size*2];
    //unsigned char ivec[] = "dontusethisinput";

    FILE * output = fopen(outputFileName, "wb");;

    //Set up encryption
    EVP_CIPHER_CTX *ctx = NULL;
    if(!(ctx = EVP_CIPHER_CTX_new())) handleOpenSSLErrors();

    EVP_EncryptInit(ctx, EVP_aes_256_cbc(), ckey, NULL);
    EVP_EncryptUpdate(ctx, outdata, &outLen1, indata, size);
    EVP_EncryptFinal(ctx, outdata + outLen1, &outLen2);

    fwrite(outdata, sizeof(char), outLen1 + outLen2, output);
    fclose(output);
    delete [] outdata;
}
*/


//extern "C" void Init_Decrypt_extension() {
//
//}
