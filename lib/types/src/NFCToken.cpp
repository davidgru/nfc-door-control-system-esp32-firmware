#include "NFCToken.hpp"


namespace DC {


    NFCToken::NFCToken(const String& hexRepr)
        : NFCToken(hexRepr.c_str()) {
        
    }


    NFCToken::NFCToken(const char* hexRepr) {
        if (!isValidTokenRepr(hexRepr)) {
            this->tokenLengthInBytes = 0;
            return;
        }
        this->tokenLengthInBytes = strlen(hexRepr) / 2;
        for (size_t i = 0; i < this->tokenLengthInBytes; i++) {
            byte b = 0;
            b += hexDigitToInt(hexRepr[2 * i + 1]);
            b += hexDigitToInt(hexRepr[2 * i]) << 4;
            this->tokenBuffer[i] = b;
        }   
    }


    NFCToken::NFCToken(const byte* buf, size_t size) {
        this->tokenLengthInBytes = size;
        memcpy(this->tokenBuffer, buf, sizeof(byte) * size);
    }


    size_t NFCToken::getLengthInBytes() const {
        return this->tokenLengthInBytes;
    }

    
    const byte* NFCToken::getBuffer() const {
        return this->tokenBuffer;
    }


    String NFCToken::toHexRepr() const {
        String hexRepr;
        hexRepr.reserve(this->tokenLengthInBytes * 2);
        for (size_t i = 0; i < this->tokenLengthInBytes; i++) {
            byte b = this->tokenBuffer[i];
            hexRepr += upperHexDigitFromByte(b);
            hexRepr += lowerHexDigitFromByte(b);
        }
        return hexRepr;
    }


    bool NFCToken::operator==(const NFCToken& rhs) const {
        if (this->tokenLengthInBytes != rhs.tokenLengthInBytes) {
            return false;
        }
        for (size_t i = 0; i < this->tokenLengthInBytes; i++) {
            if (this->tokenBuffer[i] != rhs.tokenBuffer[i]) {
                return false;
            }
        }
        return true;
    }


    bool NFCToken::operator!=(const NFCToken& rhs) const {
        return !(*this == rhs);
    }


    bool NFCToken::operator<(const NFCToken& rhs) const {
        if (this->tokenLengthInBytes != rhs.tokenLengthInBytes) {
            return this->tokenLengthInBytes < rhs.tokenLengthInBytes;
        }
        for (size_t i = 0; i < this->tokenLengthInBytes; i++) {
            if (this->tokenBuffer[i] != rhs.tokenBuffer[i]) {
                return this->tokenBuffer[i] < rhs.tokenBuffer[i];
            }
        }
        return false;
    }


    /*static*/ bool NFCToken::isValidTokenRepr(const String& str) {
        return isValidTokenRepr(str.c_str());
    }


    /*static*/ bool NFCToken::isValidTokenRepr(const char* str) {
        size_t len = strlen(str);
        if (len % 2 != 0) {
            return false;
        }
        for (size_t i = 0; i < len; i++) {
            char digit = str[i];
            if (!((digit >= '0' && digit <= '9') ||
                  (digit >= 'a' && digit <= 'f') ||
                  (digit >= 'A' && digit <= 'F'))) {
                return false;
            }
        }
        return true;
    }


    /*static*/ int NFCToken::hexDigitToInt(char digit) {
        if (digit >= '0' && digit <= '9') {
            return digit - '0';
        }
        if (digit >= 'a' && digit <= 'f') {
            return digit + 10 - 'a';
        }
        if (digit >= 'A' && digit <= 'F') {
            return digit + 10 - 'A';
        }
        return -1;
    }


    /*static*/ char NFCToken::upperHexDigitFromByte(byte value) {
        byte upper = value >> 4;
        return lowerHexDigitFromByte(upper);
    }

    /*static*/ char NFCToken::lowerHexDigitFromByte(byte value) {
        byte lower = value & 0xF;
        if (lower >= 0 && lower <= 9) {
            return lower + '0';
        }
        return lower + 'a' - 10;
    }

}