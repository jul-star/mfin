#pragma once
#include <string.h>

/* HTTP response and header for a successful request.  */

static const char* ok_response =
    "HTTP/1.0 200 OK\n"
    "Content-type: text/html\n"
    "\r\n\r\n";
static const int okLen = strlen(ok_response);

/* HTTP response, header, and body indicating that the we didn't
   understand the request.  */

static const char* bad_request_response =
    "HTTP/1.0 400 Bad Request\n"
    "Content-type: text/html\n"
    "\n"
    "<html>\n"
    " <body>\n"
    "  <h1>Bad Request</h1>\n"
    "  <p>This server did not understand your request.</p>\n"
    " </body>\n"
    "</html>\n"
    "\r\n\r\n";
static const int badLen = strlen(bad_request_response);

int STUB()
{
    printf("%s",ok_response);
    printf("%s",bad_request_response);
    return 0;
}
