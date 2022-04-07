#pragma once


#ifndef DC_DATABASE_CAPACITY
#error Specify database capacity
#endif


#define DC_SORTED_DB_TOKEN_SIZE_BYTES (DC_MAX_TOKEN_SIZE_BYTES + 1)


#include "Database.hpp"


#include <types.hpp>
#include <util.hpp>


#include <FS.h>
#include <SPIFFS.h>


namespace DC {


    /**
     * @brief Stores NFC-Tokens in the SPIFFS filesystem.
     * 
     * @author David Gruen
     */
    class SortedDatabase : public Database {

    public:

        /**
         * @brief Construct a new Database Impl object.
         * 
         * @param filepath The path reserved for the database.
         */
        SortedDatabase(const Filepath& filepath);
        
        
        /**
         * @brief Insert a NFC-Token into the database.
         * 
         * @param batch 
         */
        void insert(Batch& batch) override;


        /**
         * @brief Remove a Batch of NFC-Tokens from the database.
         * 
         * @param batch 
         */
        void remove(Batch& batch) override;


        /**
         * @brief Checks if a batch fits into the database.
         * 
         * @param batch A batch of NFC-Tokens
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

        void initSpiffs() const;


        void writeToken(File& f, const NFCToken& token) const;


        NFCToken readToken(File& f) const;


        template<typename Function>
        void foreach(Function function) const;

    private:
        
        const Filepath filepath;

    };


    template<typename Function>
    void SortedDatabase::foreach(Function function) const {
        File f = SPIFFS.open(this->filepath, FILE_READ);
        NFCToken token = this->readToken(f);
        while(token.getLengthInBytes() > 0) {
            function(token);
            token = this->readToken(f);
        }
        f.close();
    }


}
