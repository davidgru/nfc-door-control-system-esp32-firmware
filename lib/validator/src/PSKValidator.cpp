#include "PSKValidator.hpp"


namespace DC {


    PSKValidator::PSKValidator(const PSK& realPSK)
        : realPSK(realPSK) {

    }


    bool PSKValidator::isValid(const PSK& psk) {
        return psk == this->realPSK;
    }


}
