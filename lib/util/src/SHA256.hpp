#pragma once


#include <Arduino.h>
#include <mbedtls/md.h>


#include <types.hpp>


namespace DC {


    /**
     * @brief Wrapper for SHA-256 hash Algorithm.
     * 
     * @author David Gruen
     */
    class SHA256 {

    public:

        /**
         * @brief Construct a new SHA256 object.
         * 
         */
        SHA256();


        /**
         * @brief Update the hash.
         * 
         * @param str 
         */
        void update(const String& str);


        /**
         * @brief Get a hexadecimal representation of the computed hash.
         * 
         * @return String 
         */
        Hash digestToHex();

    private:

        mbedtls_md_context_t ctx;
        Hash result;
        bool finished;

    };


}