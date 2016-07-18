#pragma once
#include <unistd.h> // getopt
#include <stdio.h>
#include <string.h> //strlen
#include "Fstruct.hpp"

// tested:
// ./final -h 10 -p 200 -d hello
// ./final -h 10.10.10.10 -p 200 -d hello
// ./final -h 10.10.10.10:123 -p 200 -d hello
int parse_input(int argc, char **argv, struct input_arg *arg)
{

    int c;
    while( (c= getopt(argc, argv, "h:p:d:")) != -1)
        switch(c)
        {
        case 'h':
            arg->h = 1;
            arg->ip = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
            strcpy(arg->ip,optarg);
            break;
        case 'p':
            arg->p =1;
            arg->port = atoi(optarg);
            break;
        case 'd':
            arg->d = 1;
            arg->dir = (char*)malloc(sizeof(char)*(strlen(optarg)+1));
            strcpy(arg->dir,optarg);
            break;
        case '?':
            break;
        default:
            exit(0);
        }
    printf("\t h=%d, ip=%s\n\t p=%d, port=%d\n\t d=%d, dir=%s\n",
           arg->h,arg->ip,
           arg->p ,arg->port,
           arg->d,arg->dir);
    return 0;
}



