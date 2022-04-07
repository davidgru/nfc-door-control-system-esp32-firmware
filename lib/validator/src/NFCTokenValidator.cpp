#include "NFCTokenValidator.hpp"


#include <util.hpp>

#define SEEDED_NFC_TOKEN_SIGNIFICANT_BYTES_BEGIN 0
#define SEEDED_NFC_TOKEN_SIGNIFICANT_BYTES_NUM 32


namespace DC {


    NFCTokenValidator::NFCTokenValidator(const Seed& tokenSeed, const Database& database)
        : tokenSeed(tokenSeed), database(database) {

    }


    bool NFCTokenValidator::isValid(const NFCToken& token) {
        SHA256 hashFunction;

        hashFunction.update(this->tokenSeed);
        hashFunction.update(token.toHexRepr());

        Hash hash = hashFunction.digestToHex();
        NFCToken seededToken = hash.substring(
            SEEDED_NFC_TOKEN_SIGNIFICANT_BYTES_BEGIN * 2,
            SEEDED_NFC_TOKEN_SIGNIFICANT_BYTES_BEGIN * 2 + SEEDED_NFC_TOKEN_SIGNIFICANT_BYTES_NUM * 2
        );

        return this->database.contains(seededToken);
    }


}
