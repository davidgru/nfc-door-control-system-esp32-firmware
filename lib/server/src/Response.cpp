#include "Response.hpp"


namespace DC {
    

    Response::Response(StatusCode statusCode, const Body& body)
        : statusCode(statusCode), body(body) {

    }


    StatusCode Response::getStatusCode() const {
        return this->statusCode;
    }


    const Body& Response::getBody() const {
        return this->body;
    }


}