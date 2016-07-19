#pragma once
#include <string.h>


/* HTTP response, header, and body indicating that the we didn't
   understand the request.  */

static const char* bad_request_response =
    "HTTP/1.0 404 Not Found\r\n"
    "Content-length: 0\r\n"
    "Content-Type: text/html"
    "\r\n\r\n";
static const int badLen = strlen(bad_request_response);

int STUB()
{
    printf("%s",bad_request_response);
    return 0;
}
