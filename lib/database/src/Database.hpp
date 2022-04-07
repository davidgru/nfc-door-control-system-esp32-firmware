#pragma once


#include "Batch.hpp"


#include <types.hpp>


namespace DC {


    /**
     * @brief Stores unique NFC-Tokens.
     * 
     * @author David Gruen
     */
    class Database {

    public:

        /**
         * @brief Inserts a batch of NFC-Tokens into the database.
         * 
         * @param batch A batch of NFC-Tokens
        */
        virtual void insert(Batch& batch) = 0;


        /**
         * @brief Remove a Batch of NFC-Token from the database.
         * 
         * @param batch A batch of NFC-Tokens
         */
        virtual void remove(Batch& batch) = 0;


        /**
         * @brief Checks if a batch fits into the database.
         * 
         * @param batch A batch of NFC-Tokens
         * @return true if the batch fits.
         * @return false if the batach does not fit.
         */
        virtual bool fits(Batch& batch) const = 0;


        /**
         * @brief Checks if a NFC-Token is in the database.
         * 
         * @param token 
         * @return true If token in database.
         * @return false Else.
         */
        virtual bool contains(const NFCToken& token) const = 0;


        /**
         * @brief Return the amount of NFC-Tokens stored.
         * 
         * @return The amount of NFC-Tokens stored. 
         */
        virtual size_t size() const = 0;


        /**
         * @brief Clears the database.
         * 
         */
        virtual void clear() = 0;


        /**
         * @brief Returns a SHA-256 hash of a concatenated string
         *        consisting of all tokens in lexicographical
         *        order.
         * 
         * @return Hash SHA-256 hash.
         */
        virtual Hash hash() const = 0;

    };


}
