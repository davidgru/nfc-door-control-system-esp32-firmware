#pragma once


#include "Validator.hpp"


#include <types.hpp>


namespace DC {


    /**
     * @brief Validates PSKs
     * 
     * @author David Gruen
     */
    class PSKValidator : public Validator<PSK> {

    public:

        /**
         * @brief Construct a new PSKValidator object.
         * 
         * @param realPSK The real psk.
         */
        PSKValidator(const PSK& realPSK);


        /**
         * @brief Checks if a psk is a valid PSK
         * 
         * @param psk 
         */
        bool isValid(const PSK& psk);

    private:

        const PSK realPSK;

    };


}
