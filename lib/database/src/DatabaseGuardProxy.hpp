#pragma once


#include "Database.hpp"


namespace DC {


    /**
     * @brief Protects a Database from concurrent accesses.
     * 
     * @author David Gruen
     */
    template<class UnderlyingDatabase>
    class DatabaseGuardProxy : public Database {

    public:

        /**
         * @brief Construct a new Database Guard Proxy object.
         * 
         * @param filepath The path reserved for the database.
         */
        DatabaseGuardProxy(const Filepath& filepath)
            : database(filepath), mutex(xSemaphoreCreateMutex()) {

        }



        /**
         * @brief Inserts a batch of NFC-Tokens into the database.
         * 
         * @param batch A batch of NFC-Tokens
        */
        void insert(Batch& batch) override {
            this->protect([&](){
                this->database.insert(batch);
            });
        }


        /**
         * @brief Remove a Batch of NFC-Tokens from the database.
         * 
         * @param batch A batch of NFC-Tokens
         */
        void remove(Batch& batch) override {
            this->protect([&](){
                this->database.remove(batch);
            });
        }


        /**
         * @brief Checks if a batch fits into the database.
         * 
         * @param batch A batch of NFC-Tokens
         * @return true if the batch fits.
         * @return false if the batach does not fit.
         */
        bool fits(Batch& batch) const override {
            bool _fits;
            this->protect([&](){
                _fits = this->database.fits(batch);
            });
            return _fits;
        }


        /**
         * @brief Checks if a NFC-Token is in the database.
         * 
         * @param token 
         * @return true If token in database.
         * @return false Else.
         */
        bool contains(const NFCToken& token) const override {
            bool _contains;
            this->protect([&](){
                _contains = this->database.contains(token);
            });
            return _contains;
        }


        /**
         * @brief Return the amount of NFC-Tokens stored in the cache.
         * 
         * @return Return the amount of NFC-Tokens stored in the cache.
         */
        size_t size() const override {
            size_t size = 0;
            this->protect([&](){
                size = this->database.size();
            });
            return size;
        }


        /**
         * @brief Clears the database.
         * 
         */
        void clear() override {
            this->protect([&](){
                this->database.clear();
            });
        }


        /**
         * @brief Returns a SHA-256 hash of a concatenated string
         *        consisting of all tokens in lexicographical
         *        order.
         * 
         * @return Hash SHA-256 hash.
         */
        Hash hash() const override {
            Hash hash;
            this->protect([&](){
                hash = this->database.hash();
            });
            return hash;
        }

    private:

        template<typename Function>
        void protect(Function function) const {
            xSemaphoreTake(this->mutex, portMAX_DELAY);
            function();
            xSemaphoreGive(this->mutex);
        }

    private:

        UnderlyingDatabase database;
        SemaphoreHandle_t mutex;

    };


}
