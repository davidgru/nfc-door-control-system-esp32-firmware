#include "SortedDatabase.hpp"


#define TMP_EXTENSION "tmp"

namespace DC {


    SortedDatabase::SortedDatabase(const Filepath& filepath)
        : filepath(filepath) {
    
    }
        

    void SortedDatabase::insert(Batch& batch) {
        if (!batch.size()) {
            return;
        }
        batch.sort();
        this->initSpiffs();
        File f = SPIFFS.open(this->filepath, FILE_READ);
        File tmp = SPIFFS.open(this->filepath + TMP_EXTENSION, FILE_WRITE);
        NFCToken fromFile = this->readToken(f);
        auto batchIt = batch.begin();
        while(fromFile.getLengthInBytes() > 0 || batchIt != batch.end()) {
            if (fromFile.getLengthInBytes() == 0) {
                this->writeToken(tmp, *(batchIt++));
            } else if (batchIt == batch.end()) {
                this->writeToken(tmp, fromFile);
                fromFile = this->readToken(f);
            } else if (fromFile == *batchIt) {
                this->writeToken(tmp, fromFile);
                fromFile = this->readToken(f);
                batchIt++;
            } else if (fromFile < *batchIt) {
                this->writeToken(tmp, fromFile);
                fromFile = this->readToken(f);    
            } else {
                this->writeToken(tmp, *(batchIt++));
            }
        }
        f.close();
        tmp.close();
        SPIFFS.remove(this->filepath);
        SPIFFS.rename(this->filepath + TMP_EXTENSION, this->filepath);
    }


    void SortedDatabase::remove(Batch& batch) {
        if (!batch.size()) {
            return;
        }
        this->initSpiffs();
        File f = SPIFFS.open(this->filepath, FILE_READ);
        File tmp = SPIFFS.open(this->filepath + TMP_EXTENSION, FILE_WRITE);
        this->foreach([&](const NFCToken& token){
            if (!batch.contains(token)) {
                this->writeToken(tmp, token);
            }
        });
        tmp.close();
        SPIFFS.remove(this->filepath);
        SPIFFS.rename(this->filepath + TMP_EXTENSION, this->filepath);
    }
    

    bool SortedDatabase::fits(Batch& batch) const {
        size_t sizeIncluding = this->size();
        for (const NFCToken& token : batch) {
            if (!this->contains(token)) {
                sizeIncluding++;
            }
        }
        return sizeIncluding <= DC_DATABASE_CAPACITY;
    }


    bool SortedDatabase::contains(const NFCToken& token) const {
        this->initSpiffs();
        File f = SPIFFS.open(this->filepath, FILE_READ);
        int64_t l = 0;
        int64_t r = f.size() / DC_SORTED_DB_TOKEN_SIZE_BYTES;
        while(l <= r) {
            int64_t m = l + (r - l) / 2;
            f.seek(m * DC_SORTED_DB_TOKEN_SIZE_BYTES, fs::SeekSet);
            NFCToken read = this->readToken(f);
            if (read == token) {
                f.close();
                return true;
            }
            if (read < token) {
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        f.close();
        return false;
    }


    size_t SortedDatabase::size() const {
        this->initSpiffs();
        File f = SPIFFS.open(this->filepath, FILE_READ);
        size_t _size = f.size() / DC_SORTED_DB_TOKEN_SIZE_BYTES;
        f.close();
        return _size;
    }


    void SortedDatabase::clear() {
        this->initSpiffs();
        File f = SPIFFS.open(this->filepath, FILE_WRITE);
        f.close();
    }


    Hash SortedDatabase::hash() const {
        this->initSpiffs();
        SHA256 hashFunction;
        this->foreach([&](const NFCToken& token){
            hashFunction.update(token.toHexRepr());
        });
        return hashFunction.digestToHex();
    }
    

    void SortedDatabase::initSpiffs() const {
        static bool inited = false;
        if (!inited) {
            SPIFFS.begin(true);
            if (!SPIFFS.exists(this->filepath)) {
                File f = SPIFFS.open(this->filepath, FILE_WRITE);
                f.close();
            }
            inited = true;
        }
    }


    void SortedDatabase::writeToken(File& f, const NFCToken& token) const {
        size_t tokenSize = token.getLengthInBytes();
        const byte* tokenBuffer = token.getBuffer();
        f.write((byte)tokenSize);
        f.write(tokenBuffer, tokenSize);
        for (size_t i = tokenSize; i < DC_MAX_TOKEN_SIZE_BYTES; i++) {
            f.write(0);
        }
    }


    NFCToken SortedDatabase::readToken(File& f) const {
        byte tokenLength;
        byte tokenBuffer[DC_MAX_TOKEN_SIZE_BYTES];
        if (f.readBytes((char*)&tokenLength, 1) != 1) {
            return {};
        }
        if (f.readBytes((char*)tokenBuffer, DC_MAX_TOKEN_SIZE_BYTES) != DC_MAX_TOKEN_SIZE_BYTES) {
            return {};
        }
        return NFCToken(tokenBuffer, tokenLength);
    }


}
