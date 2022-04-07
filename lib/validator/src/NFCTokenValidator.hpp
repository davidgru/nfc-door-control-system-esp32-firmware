#pragma once


#include "Validator.hpp"


#include <database.hpp>
#include <types.hpp>


namespace DC {


    /**
     * @brief Validates NFC-Tokens.
     * 
     * @author David Gruen
     */
    class NFCTokenValidator : public Validator<NFCToken> {

    public:

        /**
         * @brief Construct a new NFCTokenValidator object
         * 
         * @param tokenSeed NFC-Tokens are seeded with this seed.
         * @param database
         */
        NFCTokenValidator(const Seed& tokenSeed, const Database& database);


        /**
         * @brief Checks if a NFC-Token is valid.
         * 
         * @note A NFC-Token is valid if it's in the database.
         * 
         * @param token 
         * @return true 
         * @return false 
         */
        bool isValid(const NFCToken& token) override;

    private:
    
        Seed tokenSeed;
        const Database& database;

    };


}
