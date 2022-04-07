#include "HTTPDRequest.hpp"


#include "ServerConfig.hpp"


namespace DC {


    HTTPDRequest::HTTPDRequest(httpd_req_t* request)
        : request(request) {

    }


    Method HTTPDRequest::getMethod() const {
        return Method::GET;
    }


    Body HTTPDRequest::getBody() const {
        std::string payloadStdString;
        payloadStdString.resize(this->request->content_len);
        httpd_req_recv(this->request, &payloadStdString[0], this->request->content_len);
        return String(payloadStdString.c_str());
    }


    bool HTTPDRequest::hasHeader(const String& name) const {
        size_t len = httpd_req_get_hdr_value_len(this->request, name.c_str());
        return len != 0;
    }


    String HTTPDRequest::getHeader(const String& name) const {
        size_t len = httpd_req_get_hdr_value_len(this->request, name.c_str());
        if (len <= 0) {
            return emptyString;
        }
        std::string stdString;
        stdString.resize(len + 1);
        httpd_req_get_hdr_value_str(this->request, name.c_str(), &stdString[0], len + 1);
        return String(stdString.c_str());
    }


};
