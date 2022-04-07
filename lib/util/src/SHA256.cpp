#include "SHA256.hpp"


namespace DC {


    SHA256::SHA256()
        : finished(false) {
        mbedtls_md_init(&this->ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 0);
        mbedtls_md_starts(&ctx);
    }


    void SHA256::update(const String& str) {
        if (this->finished) {
            return;
        }
        mbedtls_md_update(&ctx, (unsigned char*)str.c_str(), str.length());
    }


    Hash SHA256::digestToHex() {
        if (!this->finished) {
            byte result[32];
            mbedtls_md_finish(&ctx, result);
            mbedtls_md_free(&ctx);
            for (byte b : result) {
                char str[3];
                sprintf(str, "%02x", b);
                this->result += str;
            }
            this->finished = true;
        }
        return this->result;
    }


}
