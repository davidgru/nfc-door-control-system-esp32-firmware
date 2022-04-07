#include "CachedDatabase.hpp"


#include <SPIFFS.h>
#include <algorithm>
#include <vector>


#include <util.hpp>


namespace DC {


    CachedDatabase::CachedDatabase(const Filepath& filepath)
        : file(filepath), cache(DC_DATABASE_CAPACITY), cacheLoaded(false) {

    }


    void CachedDatabase::insert(Batch& batch) {
        for (const NFCToken& token : batch) {
            this->loadCacheIf();
            if (this->contains(token)) {
                return;
            }
            if (this->size() + 1 > DC_DATABASE_CAPACITY) {
                return;
            }
            this->cache.insert(token);
            this->file.insert(token);
        }
    }


    void CachedDatabase::remove(Batch& batch) {
        for (const NFCToken& token : batch) {
            this->loadCacheIf();
            if (!this->contains(token)) {
                return;
            }
            this->cache.remove(token);
            this->file.remove(token);
        }
    }


    bool CachedDatabase::fits(Batch& batch) const {
        size_t _size = this->size();
        for (const NFCToken& token: batch) {
            if (!this->contains(token)) {
                _size++;
            }
        }
        return _size <= DC_DATABASE_CAPACITY;
    }


    bool CachedDatabase::contains(const NFCToken& token) const {
        this->loadCacheIf();
        return this->cache.contains(token);
    }


    size_t CachedDatabase::size() const {
        this->loadCacheIf();
        return this->cache.size();
    }


    void CachedDatabase::clear() {
        this->cache.clear();
        this->file.clear();
    }


    Hash CachedDatabase::hash() const {
        this->loadCacheIf();
        SHA256 hashFunction;
        this->cache.foreach([&](const NFCToken& token){
            hashFunction.update(token.toHexRepr());
            return 0;
        });
        return hashFunction.digestToHex();
    }


    void CachedDatabase::loadCacheIf() const {
        if (!this->cacheLoaded) {
            this->cache.loadFromFile(this->file);
            this->cacheLoaded = true;
        }
    }


}
