#pragma once
#include "Fconst.hpp"
#include <string.h>



const char * getResponse(const char *v, const char *dir);
int testParserGet();
const char* test0(char *v, char *dir);
int test1();
int test2();
int test3();
int test4();
int test5();
int test6();
int test7();
int test8();
int test9();

const char * getResponse(const char *v, const char *dir)
{
   // printf("getResponse:get=%s\n", v);
    int vSize = strlen(v);// sizeof(*v);
    if (vSize == 0)
    {
        return bad_request_response;
    }

    const char *get = strstr(v,"GET");
    // scanf(v, "%s %s %s", method, url, protocol);
    // *** CHECK PROTOCOL ***
    if (get == NULL)
    {
        return bad_request_response;

    }
    // *** CHECK PROTOCOL ***
    const char *http10 = strstr(v,"HTTP 1.0");
    const char *http101 = strstr(v,"HTTP/1.0");
    if ((http10 == NULL) && (http101 == NULL))
    {
        return bad_request_response;
    }
    char* http = (http10 == NULL)? (char*)http101: (char*)http10;
  //  printf("http protocol=%s\n", http);

     // *** CHECK URL ***
    char* rawUrl = (char*)malloc(sizeof(char)*(http-get-4));
    memset(rawUrl,'\0', http-get-4);
    strncpy(rawUrl,(get+4), http-get-4);
    char* url = (char*)malloc(sizeof(char)*(http-get-4));
    memset(url,'\0', http-get-4);
    int i,j;
    j=0;
    for (i = 0; i<http-get-4;++i)
    {
        if (rawUrl[i] != ' ') { url[j]=rawUrl[i];++j;}
    }
 //   printf("url=!%s!\n",url);
    // *** NO PAGE ***
    if (strlen(url) < 2)
    {
        return bad_request_response;
    }

    // *** GET PAGE ***
    char* fullname = (char*)malloc(sizeof(char)*(strlen(dir)+strlen(url)+2));
    memset(fullname,'\0',(strlen(dir)+strlen(url)+2));
    if (strlen(dir) == 0){
        if (strstr(url,"/") != NULL)
        strcpy(fullname,(url+1));
        else
        strcpy(fullname,url);
    }
    else{
        strcpy(fullname, dir);
        strcpy(fullname+strlen(dir),url);
    }
    FILE *fd;
    fd = fopen(fullname,"r");
    if (fd == NULL)
    {
        return bad_request_response;
    }
                    // ***The only OK***
    fseek(fd,0L,SEEK_END);
    int fileSize = ftell(fd);
    rewind(fd);
    char* ok=(char*)"HTTP/1.0 200 OK\r\nContent-length: ";
    int counter = strlen(ok);
    char *cnt = (char*)"\r\nContent-Type: text/html\r\n\r\n";
    char fsz[32];
    memset(fsz,'\0',32);
    sprintf(fsz, "%d",fileSize);
   // "Content-type: text/html"
    // "\r\n\r\n";

    char* response = (char*)malloc(sizeof(char)*(fileSize+strlen(ok)+ strlen(fsz) +strlen(cnt)+1));
    memcpy(response, ok, counter);
    memcpy(response+counter, fsz, strlen(fsz));
    counter += strlen(fsz);
    memcpy(response+counter, cnt, strlen(cnt));
    counter += strlen(cnt);
    fread(response+counter,fileSize, sizeof(char),fd);
    fclose(fd);

//  printf("parseHttpGet:Answer: %s\t(%d)\n", answer, len);
    return response;
}

int testParserGet()
{
    /*    test1();
    	test2();
     	test3();
    	test4();

    	test5();
    	test6();
    	test7();
    	    */
    	//test8();
    	test9();
    // MIME-Version: 1.0
    return 0;
}
const char* test0(char *v, char *dir)
{
//    int len;
//    answer = (char*)bad_request_response;
//    answer = (char*)malloc(sizeof(char)*badLen);
//    memset(answer,'\0',badLen);
//    strcpy(answer,bad_request_response);
//		len = strlen(answer);
//		printf("test0:Answer: %s\t(%d)\n", answer, len);
    return bad_request_response;
}
int test1()
{
    // test 1
    const char *in  = "";
    const char *dir = "";
    printf("*** Test_01_0 ***	Empty data \n in=%s,  dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("*** Test_01_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}
int test2()
{
    // test 2
    const char *in = "GET HTTP 1.1 / \r\r";
    const char *dir = "";
    printf("*** Test_02_0 ***\n in=%s, dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_02_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}
int test3()
{
    // test 3
     char *in  = (char*)"POST HTTP 1.0 / \r\r";
     char *dir = (char*)"";
    printf("*** Test_03_0 ***\n in=%s, dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_03_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}
int test4()
{
    // test 4
    char *in  = (char*)"GET HTTP 1.0 / \r\r";
    char *dir = (char*)"";
    printf("*** Test_04_0 ***\n in=%s,  dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_04_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}

int test5()
{
    // test 5
    char *in  = (char*)"GET /index.html HTTP/1.0 \r\r";
    char *dir = (char*)"";
    printf("*** Test_04_0 ***\n in=%s,  dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_04_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}

int test6()
{
    // test 6
    char *in  = (char*)"GET index.html HTTP/1.0 \r\r";
    char *dir = (char*)"";
    printf("*** Test_04_0 ***\n in=%s,  dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_04_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}

int test7()
{
    // test 7
    char *in  = (char*)"GET /index.html HTTP/1.0 \r\r";
    char *dir = (char*)"tmp";
    printf("*** Test_04_0 ***\n in=%s,  dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_04_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}

int test8()
{
    // test 7
    char *in  = (char*)"GET /index.html HTTP/1.1"
                        "Host: 127.0.0.1"
                        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0"
                        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
                        "Accept-Language: en-US,en;q=0.5"
                        "Accept-Encoding: gzip, deflate"
                        "Connection: keep-alive"
                        "Cache-Control: max-age=0";
      char *dir = (char*)"tmp";
    printf("*** Test_04_0 ***\n in=%s,  dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_04_1 ***\n in=%s, out=%s, dir=%s\n\n",in, out,dir);
    return len;
}

int test9()
{
    // test 9
    char *in  = (char*)"GET /index.html HTTP/1.0"
                        "Host: 127.0.0.1"
                        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0"
                        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
                        "Accept-Language: en-US,en;q=0.5"
                        "Accept-Encoding: gzip, deflate"
                        "Connection: keep-alive"
                        "Cache-Control: max-age=0";
      char *dir = (char*)"tmp";
    printf("*** Test_04_0 ***\n in=%s,  dir=%s\n",in, dir);
    int len = 0;
    char *out =(char*)getResponse(in,dir);
    len = strlen(out);
    printf("***Test_04_1 ***\n out=%s, dir=%s\n\n", out,dir);
    return len;
}
