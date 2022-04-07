#pragma once


#ifndef DC_DATABASE_CAPACITY
#error Define database capacity
#endif


#include "Database.hpp"
#include "DatabaseImplFile.hpp"
#include "DatabaseCache.hpp"


#include <types.hpp>


namespace DC {


    /**
     * @brief Stores NFC-Tokens in the SPIFFS filesystem.
     * 
     * @author David Gruen
     */
    class CachedDatabase : public Database {

    public:

        /**
         * @brief Construct a new Database Impl object.
         * 
         * @param filepath The path reserved for the database.
         */
        CachedDatabase(const Filepath& filepath);
        

        /**
         * @brief Inserts a batch of NFC-Tokens into the database.
         * 
         * @param batch A batch of NFC-Tokens
        */
        void insert(Batch& batch) override;


        /**
         * @brief Remove a Batch of NFC-Token from the database.
         * 
         * @param batch A batch of NFC-Tokens
         */
        void remove(Batch& batch) override;


        /**
         * @brief Checks if a batch fits into the database.
         * 
         * @param batch A batch of NFC-Tokens
         * @return true if the batch fits.
         * @return false if the batach does not fit.
         */
        bool fits(Batch& batch) const override;


        /**
         * @brief Checks if a NFC-Token is in the database.
         * 
         * @param token 
         * @return true If token in database.
         * @return false Else.
         */
        bool contains(const NFCToken& token) const override;


        /**
         * @brief Return the amount of NFC-Tokens stored.
         * 
         * @return The amount of NFC-Tokens stored. 
         */
        size_t size() const override;


        /**
         * @brief Clears the database.
         * 
         */
        void clear() override;


        /**
         * @brief Returns a SHA-256 hash of a concatenated string
         *        consisting of all tokens in lexicographical
         *        order.
         * 
         * @return Hash SHA-256 hash.
         */
        Hash hash() const override;

    private:

        void loadCacheIf() const;

    private:

        DatabaseImplFile file;
        mutable DatabaseCache cache;
        mutable bool cacheLoaded;
    };


}
