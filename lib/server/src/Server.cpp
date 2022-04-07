#include "Server.hpp"


#include "HTTPDRequest.hpp"
#include "ServerConfig.hpp"


#if DC_SERVER_SECURE
#define CERT_BINARY_ADDRESS_BEGIN "_binary_certs_certificate_pem_crt_start"
#define CERT_BINARY_ADDRESS_END "_binary_certs_certificate_pem_crt_end"
#define KEY_BINARY_ADDRESS_BEGIN "_binary_certs_private_pem_key_start"
#define KEY_BINARY_ADDRESS_END "_binary_certs_private_pem_key_end"
#endif


#define ERROR_REQUEST_TOO_BIG "{\"msg\": \"Error: Request too big.\"}"


namespace DC {


    Server::Server() {
        this->config = HTTPD_SSL_CONFIG_DEFAULT();

#if DC_SERVER_SECURE
        extern const unsigned char certificateStart[] asm(CERT_BINARY_ADDRESS_BEGIN);
        extern const unsigned char certificateEnd[] asm(CERT_BINARY_ADDRESS_END);
        extern const unsigned char privateKeyStart[] asm(KEY_BINARY_ADDRESS_BEGIN);
        extern const unsigned char privateKeyEnd[] asm(KEY_BINARY_ADDRESS_END);
        this->config.cacert_pem = certificateStart;
        this->config.cacert_len = certificateEnd - certificateStart;
        this->config.prvtkey_pem = privateKeyStart;
        this->config.prvtkey_len = privateKeyEnd - privateKeyStart;
#else
        this->config.transport_mode = HTTPD_SSL_TRANSPORT_INSECURE;
#endif
        this->config.httpd.max_open_sockets = SERVER_MAX_OPEN_SOCKETS;
    }


    void Server::addHandler(Handler* handler) {
        this->uriHandlers.push_back({
            .uri = handler->getURI().c_str(),
            .method = toHTTPDMethod(handler->getMethod()),
            .handler = universalHandler,
            .user_ctx = (void*)handler,
        });
    }


    void Server::removeHandler(Handler* handler) {
        this->uriHandlers.remove_if([&](const httpd_uri_t& uri){
            return uri.user_ctx == (void*)handler;
        });
    }

    
    void Server::start() {
        if (this->serverHandle) {
            return;
        }
        esp_err_t err = httpd_ssl_start(&this->serverHandle, &this->config);
        if (err) {
            Serial.print("Server failed with code: ");
            Serial.println(esp_err_to_name(err));
            return;
        }
        for (auto& uriHandler : this->uriHandlers) {
            httpd_register_uri_handler(this->serverHandle, &uriHandler);
        }
    }


    void Server::stop() {
        httpd_ssl_stop(this->serverHandle);
        this->serverHandle = NULL;
    }


    /*static*/ esp_err_t Server::universalHandler(httpd_req_t* req) {
        if (req->content_len > SERVER_MAX_REQUEST_PAYLOAD_SIZE) {
            httpd_resp_set_status(req, HTTPD_400);
            httpd_resp_send(req, ERROR_REQUEST_TOO_BIG, HTTPD_RESP_USE_STRLEN);
        }
        Handler* handler = (Handler*)req->user_ctx;
        HTTPDRequest httpdReq = HTTPDRequest(req);
        Response response = handler->onRequest(httpdReq);


        String respCode = String(response.statusCode);
        httpd_resp_set_status(req, respCode.c_str());

        Body respBody = response.getBody();
        httpd_resp_send(req, respBody.c_str(), HTTPD_RESP_USE_STRLEN);

        return ESP_OK;
    };


    /*static*/ httpd_method_t Server::toHTTPDMethod(Method method) {
        static httpd_method_t T[] = {
            HTTP_GET,
            HTTP_POST,
            HTTP_PUT,
            HTTP_DELETE
        };
        return T[method];
    }


}
