#include <stdlib.h>

#include "Fparser.hpp"
#include "Fstruct.hpp"
#include "Fserver.hpp"

int run(int argc, char **argv);
int test(int argc, char **argv);
int debug_run(int argc, char **argv);
int debug_run_hard();


int main(int argc, char **argv)
{
//	test(argc, argv);
 //  debug_run(argc, argv);
 //  debug_run_hard();
	run(argc, argv);

}

int run(int argc, char **argv)
{
    printf("***Release***\n");
	struct input_arg arg;
    initialize_arg(&arg);
    parse_input( argc, argv, &arg);
    server(&arg,1);
    return 0;
}

int debug_run(int argc, char **argv)
{
    printf("***Debug_Run***\n");
	struct input_arg arg;
    initialize_arg(&arg);
    parse_input( argc, argv, &arg);
    server(&arg,0);
    return 0;
}

int debug_run_hard()
{
    printf("***Debug_Run_Hard***\n");
	struct input_arg arg;
    initialize_arg(&arg);
    arg.ip =(char*)"127.0.0.1";
    arg.port=12345;
    arg.dir = (char*)"/tmp";

    server(&arg,0);
    return 0;
}

int test(int argc, char **argv)
{
    printf("***Test***\n");
	struct input_arg arg;
	init_arg_test(&arg);

	testParserGet();
    return 0;
}

