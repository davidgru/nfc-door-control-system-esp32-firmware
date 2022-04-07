#pragma once


#include <types.hpp>


namespace DC {


    /**
     * @brief A Batch of NFC-Tokens.
     * 
     * @author David Gruen
     */
    class Batch {

    public:

        using Container = std::vector<NFCToken>;
        using Iterator = Container::const_iterator;

    public:

        /**
         * @brief Add a NFC-Token.
         * 
         * @param token 
         */
        void add(const NFCToken& token);


        /**
         * @brief Add a range of NFC-Tokens.
         * 
         * @tparam _It
         * @param first Iterator to first NFC-Token.
         * @param last Iterator to last NFC-Token.
         */
        template<typename _It>
        void add(_It first, _It last);


        /**
         * @brief Sorts the NFC-Tokens lexicographical.
         * 
         */
        void sort();


        /**
         * @brief Return the size of the batch.
         * 
         * @return The size of the batch.
         */
        size_t size() const;


        /**
         * @brief Checks if a NFC-Token is in the batch.
         * 
         * @return true 
         * @return false 
         */
        bool contains(const NFCToken& token) const;


        /**
         * @brief Return iterator to first element of batch.
         * 
         * @return BatchIterator 
         */
        Iterator begin() const;


        /**
         * @brief Return iterator to last element of batch.
         * 
         * @return BatchIterator 
         */
        Iterator end() const;

    private:

        Container container;
        bool isSorted = true;

    };


    template<typename _It>
    void Batch::add(_It first, _It last) {
        this->container.insert(this->container.begin(), first, last);
        this->isSorted = false;
    }


}
