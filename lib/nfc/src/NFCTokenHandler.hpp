#pragma once


#include <door.hpp>
#include <types.hpp>
#include <validator.hpp>


namespace DC {


    /**
     * @brief Opens the door if invoked with a valid NFC-Token.
     * 
     * @author David Gruen
     */
    class NFCTokenHandler {

    public:

        /**
         * @brief Construct a new NFCTokenHandler object.
         * 
         * @param doorOpener 
         * @param validator A NFC-Token validator.
         */
        NFCTokenHandler(DoorOpener& doorOpener, Validator<NFCToken>& validator);


        /**
         * @brief Opens the door if token is valid.
         * 
         * @param token 
         */
        void processToken(const NFCToken& token);

    private:

        DoorOpener& doorOpener;
        Validator<NFCToken>& validator;

    };


}
