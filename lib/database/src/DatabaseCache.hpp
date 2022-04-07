#pragma once


#include "DatabaseImplFile.hpp"


#include <array>


#ifndef DC_DATABASE_CAPACITY
#error Specify database Capacity in build flags
#endif


namespace DC {


    /**
     * @brief Stores NFC-Tokens in RAM.
     * 
     * @author David Gruen
     */
    class DatabaseCache {

    public:

        /**
         * @brief Construct a new Database Cache object.
         * 
         * @param maxSize The expected maximum size.
         */
        DatabaseCache(size_t maxSize);


        /**
         * @brief Load all NFC-Tokens from a file.
         * 
         * @param databaseFile 
         */
        void loadFromFile(const DatabaseImplFile& databaseFile);


        /**
         * @brief Insert a NFC-Token.
         * 
         * @param token 
         */
        void insert(const NFCToken& token);


        /**
         * @brief Remove a NFC-Token.
         * 
         * @param token 
         */
        void remove(const NFCToken& token);

        
        /**
         * @brief Check if a NFC-Token is contained.
         * 
         * @param token 
         * @return true 
         * @return false 
         */
        bool contains(const NFCToken& token) const;


        /**
         * @brief Return the amount of NFC-Tokens stored in the cache.
         * 
         * @return Return the amount of NFC-Tokens stored in the cache.
         */
        size_t size() const;


        /**
         * @brief Remove all NFC-Tokens.
         * 
         */
        void clear();


        /**
         * @brief Iterate all NFC-Tokens in lexicographical order.
         * 
         * @tparam Function Signature: bool f(const NFCToken&);
         * @param function 
         */
        template<typename Function>
        void foreach(Function function) const;

    private:

        std::array<NFCToken, DC_DATABASE_CAPACITY> container;
        size_t _size;
    };


    template<typename Function>
    void DatabaseCache::foreach(Function function) const {
        for (size_t i = 0; i < this->_size; i++) {
            if (function(this->container[i])) {
                return;
            }
        }
    }


}
