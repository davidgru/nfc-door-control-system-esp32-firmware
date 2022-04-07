#pragma once


namespace DC {


    /**
     * @brief Decides if a key is valid.
     * 
     * @tparam Key
     * 
     * @author David Gruen
     */
    template<typename Key>
    class Validator {

    public:

        /**
         * @brief Decides if a key is valid.
         * 
         * @param key 
         * @return true if key valid.
         * @return false if key invalid.
         */
        virtual bool isValid(const Key& key) = 0;

    };


}
