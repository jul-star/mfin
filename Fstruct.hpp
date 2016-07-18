#pragma once

struct input_arg
{
    int h;
    char *ip;
    int p;
    int port;
    int d;
    char *dir;
};
void initialize_arg(struct input_arg *arg);
void init_arg_test(struct input_arg *arg);

void initialize_arg(struct input_arg *arg)
{
    arg->h = arg->p = arg->d = 0;
}

void init_arg_test(struct input_arg *arg)
{
	arg->h = arg->p = arg->d = 1;
	arg->ip = (char *)malloc(sizeof(char)*(strlen("127.0.0.1")+1));
	strcpy(arg->ip, "127.0.0.1");
	arg->port = 100;
	arg->dir = (char *)malloc(sizeof(char)*(strlen("tmp")+1));
	strcpy(arg->dir, "tmp");
}
