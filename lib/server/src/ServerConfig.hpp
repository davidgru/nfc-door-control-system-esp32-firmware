#pragma once

#define SERVER_JSON_DOC_SIZE 10240

#define SERVER_MAX_REQUEST_PAYLOAD_SIZE 8192


// increasing this leads to memory leak
#define SERVER_MAX_OPEN_SOCKETS 1


#define SERVER_PSK_HEADER_KEY "psk"

#define SERVER_TOKEN_URI "/token/"
#define SERVER_DOOR_URI "/door/"

#ifndef DC_SERVER_SECURE
#define DC_SERVER_SECURE false
#endif



#define EMPTY_RESPONSE "{}"
