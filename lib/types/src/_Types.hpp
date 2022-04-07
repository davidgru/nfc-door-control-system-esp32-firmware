#pragma once


#include <Arduino.h>
#include <cstdint>


namespace DC {


    /**
     * @brief Represents a GPIO pin.
     * 
     */
    using Pin = uint8_t;


    /**
     * @brief Duration in milliseconds.
     * 
     */
    using Duration = uint32_t;


    /**
     * @brief URL
     * 
     */
    using URL = String;


    /**
     * @brief URI
     * 
     */
    using URI = String;


    /**
     * @brief A pre-shared key.
     * 
     */
    using PSK = String;


    /**
     * @brief 
     * 
     */
    using IPAddress = String;


    /**
     * @brief
     * 
     */
    using Hash = String;


    /**
     * @brief 
     * 
     */
    using Filepath = String;


    /**
     * @brief 
     * 
     */
    using Filename = String;

    
    /**
     * @brief HTTP-Methods
     * 
     */
    enum Method {
        GET,
        POST,
        PUT,
        DELETE,
    };


    /**
     * @brief Payload data.
     * 
     */
    using Body = String;


    /**
     * @brief 
     * 
     */
    using Seed = String;


}
