#include "DatabaseCache.hpp"


namespace DC {

    
    DatabaseCache::DatabaseCache(size_t maxSize)
        : _size(0) {
    
    }


    void DatabaseCache::loadFromFile(const DatabaseImplFile& databaseFile) {
        databaseFile.foreach([&](const NFCToken& token){
            this->container[this->_size] = token;
            this->_size++;
            return false;
        });
        std::sort(this->container.begin(), this->container.begin() + this->_size);
    }


    void DatabaseCache::insert(const NFCToken& token) {
        if (this->contains(token)) {
            return;
        }
        this->container[this->_size] = token;
        this->_size++;
        for (int32_t i = this->_size - 1; i >= 1; i--) {
            if (this->container[i] < this->container[i - 1]) {
                std::swap(this->container[i], this->container[i - 1]);
            } else {
                break;
            }
        }
    }


    void DatabaseCache::remove(const NFCToken& token) {
        auto new_end = std::remove(this->container.begin(), this->container.begin() + this->_size, token);
        this->_size = std::distance(this->container.begin(), new_end);
    }


    bool DatabaseCache::contains(const NFCToken& token) const {
        return std::binary_search(
            this->container.begin(),
            this->container.begin() + this->_size, 
            token
        );
    }


    size_t DatabaseCache::size() const {
        return this->_size;
    }


    void DatabaseCache::clear() {
        this->_size = 0;
    }

}