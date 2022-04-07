#pragma once


#include <Arduino.h>


#ifndef DC_MAX_TOKEN_SIZE_BYTES
#define DC_MAX_TOKEN_SIZE_BYTES 32
#endif


namespace DC {


    /**
     * @brief Stores a NFC-Token.
     * 
     * @author David Gruen
     */
    class NFCToken {

    public:

        /**
         * @brief Construct a new NFCToken object.
         * 
         * @param hexRepr Hexadecimal representation.
         */
        NFCToken(const String& hexRepr = F(""));


        /**
         * @brief Construct a new NFCToken object.
         * 
         * @param hexRepr Hexadecimal representation
         */
        NFCToken(const char* hexRepr);


        /**
         * @brief Construct a new NFCToken object
         * 
         * @param buf UID buffer
         * @param size UID length
         */
        NFCToken(const byte* buf, size_t size);


        /**
         * @brief Return the length of the token in bytes.
         * 
         * @return The length of the token.
         */
        size_t getLengthInBytes() const;


        /**
         * @brief Return the token buffer
         * 
         * @return const byte* 
         */
        const byte* getBuffer() const;


        /**
         * @brief Return a hexadecimal representation.
         * 
         * @return A hexadecimal representation.
         */
        String toHexRepr() const;


        bool operator==(const NFCToken& rhs) const;
        bool operator!=(const NFCToken& rhs) const;
        bool operator<(const NFCToken& rhs) const;


        /**
         * @brief Check if a String can be interpreted as a NFC-Token.
         * 
         * @param str 
         * @return true 
         * @return false 
         */
        static bool isValidTokenRepr(const String& str);
        static bool isValidTokenRepr(const char* str);

    private:

        static int hexDigitToInt(char digit);
        static char upperHexDigitFromByte(byte value);
        static char lowerHexDigitFromByte(byte value);

    private:

        size_t tokenLengthInBytes;
        byte tokenBuffer[DC_MAX_TOKEN_SIZE_BYTES];

    };

}