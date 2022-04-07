#include "Batch.hpp"


namespace DC {


    void Batch::add(const NFCToken& token) {
        this->container.push_back(token);
        this->isSorted = false;
    }


    void Batch::sort() {
        if (this->isSorted) {
            return;
        }
        std::sort(this->container.begin(), this->container.end());
        this->isSorted = true;
    }


    size_t Batch::size() const {
        return this->container.size();
    }


    bool Batch::contains(const NFCToken& token) const {
        if (this->isSorted) {
            return std::binary_search(this->container.begin(), this->container.end(), token);
        }
        return std::find(this->container.begin(), this->container.end(), token) != this->container.end();
    }


    Batch::Iterator Batch::begin() const {
        return this->container.begin();
    }


    Batch::Iterator Batch::end() const {
        return this->container.end();
    }


}