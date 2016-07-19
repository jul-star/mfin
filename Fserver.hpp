#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <omp.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h> // daemon

#include "Fhttp.hpp"


int server(struct input_arg *arg,int runType);
int set_nonblock(int sct);
int socket_up(int skt);
int timeout_recv(int skt);
int timeout_send(int skt);
int check_errors(struct epoll_event *ee);
void test_parallel();
void printHostInfo(struct sockaddr *Addr, int Len, int fd);

int server(struct input_arg *arg,int runType)
{
    const int bSize = 4096;
    test_parallel();

    if (runType) { daemon(0,0); }
// domain, type, protocol
    int sServer = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (sServer == -1 )
    {
        perror("socket");
        exit (0);
    }

    // bind
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(arg->port);
    inet_pton(AF_INET, arg->ip, &(sa.sin_addr));
    int bnd = bind(sServer, (struct sockaddr *)(&sa), sizeof(sa));
    if (bnd == -1)
    {
        perror("bind");
        exit (0);
    }

    //
    set_nonblock(sServer);



    // listen
    int listenRes = listen(sServer, SOMAXCONN);  // SOMAXCONN = 128
    if (listenRes == -1)   {perror("Server:listen"); exit(EXIT_FAILURE);}


    // create EPoll
    int EPoll = epoll_create1(0);   // create(1);
    if(EPoll == -1) { perror("epoll_create"); exit (EXIT_FAILURE); }

    struct epoll_event EventServer;

    // add server
    EventServer.data.fd = sServer;
    EventServer.events = EPOLLIN; // | EPOLLET | EPOLLONESHOT; // | EPOLLET;  /// | EPOLLET; ///TODO: FIND OUT

    // register Server in EPoll
    int RegServ = epoll_ctl(EPoll, EPOLL_CTL_ADD, sServer, &EventServer);
    if (RegServ == -1) { perror("RegServ epoll_ctl"); exit (EXIT_FAILURE); }

    // return events from kernel
    const int MAX_EVENTS = 32;

    int N,i;
    while(true) // (sClient = accept(sServer,&sClientAddr, &sClientLen)) != -1
    {
        struct epoll_event Events[MAX_EVENTS];
        // *** WAIT ***
        N = epoll_wait(EPoll, Events, MAX_EVENTS, -1); // -1 infinitive wait

        //TODO: check max_threads
#pragma omp parallel shared(EPoll, Events, sServer)
        {
//          int maxThread = omp_get_max_threads();
//          int threads;
//          threads = (N>maxThread) ? maxTread : N
        #pragma omp for num_threads(N)
        for(i = 0; i < N; i++)
        {
//           printf("th-n:%d\n",omp_get_thread_num());
            if (check_errors(&Events[i])!=-1)
            { // error->connect close
            if (Events[i].data.fd == sServer)
            {  // adding / removing new connections
                // if event on server then someone trying connects to the Server
                struct sockaddr sClientAddr;
                socklen_t sClientLen;
                struct epoll_event Event;
                int sClient;
                sClientLen = sizeof sClientAddr;

                //***ACCEPT*** CLIENT
                sClient = accept(sServer, &sClientAddr, &sClientLen);
                if (sClient == -1)
                {
                	if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
					{
                		printf(" error accept ");
						 // processed all incoming connection
					}
					else { perror("accept");  }
                }
                printHostInfo(&sClientAddr, sClientLen, sClient);


                set_nonblock(sClient);
                // *** REGISTER CLIENT ***
                Event.data.fd = sClient;
                Event.events = EPOLLIN; // | EPOLLET | EPOLLONESHOT;
                int epollClient = epoll_ctl(EPoll, EPOLL_CTL_ADD, sClient, &Event);
                if (epollClient == -1 ){ perror ("epoll_ctl_Client");}
            }
            else  // try Read
            {
                static char buf[bSize];
                int RecvRes = recv(Events[i].data.fd, &buf, bSize, MSG_NOSIGNAL);
                if (RecvRes == -1) { perror("recv4Clien"); }
                // *** READ & WRITE
                if ((RecvRes == 0 ) && (errno != EAGAIN))
                {
                    shutdown(Events[i].data.fd,SHUT_RDWR);
                    close(Events[i].data.fd);
                }
                else if(RecvRes > 0 )   // print & send back
                {
                    char *answer = (char*)getResponse(buf,arg->dir);
                    int answerLen = strlen(answer);
                    int SendRes = send(Events[i].data.fd, answer, answerLen, MSG_NOSIGNAL);
                    if (SendRes == -1) { perror ("send2Client"); }
                    int epollClientModify = epoll_ctl(EPoll, EPOLL_CTL_MOD, Events[i].data.fd, &Events[i]);
                    if (epollClientModify  == -1) { perror("EPOLL_CTL_MOD");}
                } // else if(RecvRes > 0 )

            } // else  // try Read
            } // if EBADF
        } // for
       }  //parallel
        // signal -> r/w flags MSG_NOSIGNAL
        //   size_t sClientRecv = recv (sClient, &rbuf, rlen, rflags);
        //   size_t sClientSend = send(sClient, &wbuf, wlen, wflag);
    } // while


    int shutRes = shutdown(sServer,SHUT_RDWR);
    if(shutRes == -1) { perror("shutdownServer"); }
    int closeRes = close(sServer);
    if (closeRes == -1) { perror("closeServer"); }
    return 0;
}

int check_errors(struct epoll_event *ee)
{
	if((ee->events & EPOLLERR) ||
			(ee->events & EPOLLHUP) ||
			(!(ee->events & EPOLLIN)))
	{
		if (errno != EBADF)
		{
		perror("events error");
		shutdown(ee->data.fd,SHUT_RDWR);
		close (ee->data.fd);
			return 1;
		}
		else
		{
				perror("events error");
				return -1;
		//	shutdown(ee->data.fd,SHUT_RDWR);
		//	close (ee->data.fd);
		}
	}
	if (errno == EBADF) return -1;

	return 0;
}

 void printHostInfo(struct sockaddr *Addr, int Len, int fd)
 {
                char hostBuf[NI_MAXHOST];
                char serverBuf[NI_MAXSERV];
                int NameRes = getnameinfo(Addr, Len,
                		hostBuf, sizeof hostBuf,
						serverBuf, sizeof serverBuf,
						NI_NUMERICHOST | NI_NUMERICSERV);
                if( NameRes == 0)
                {
                	printf ("Connected: %d, host=%s, port=%s\n", fd, hostBuf, serverBuf);
                }
}
//l 3.1 slide 9
int set_nonblock(int sct)
{
    int flags;
#if defined(O_NONBLOCK)
    if (-1 ==(flags = fcntl(sct,F_GETFL,0)))
        flags =0;
    return fcntl(sct, F_SETFL,flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(sct,FIONBIO, &flags);
#endif // defined
}
// up socket
int socket_up(int skt)
{
    int optval =1;
    int ret;
    ret = setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ret == -1)
    {
        perror("setsockopt");
        return 1;
    }
    return 0;
}

// timeout recieve
int timeout_recv(int skt)
{
    struct timeval tv;
    tv.tv_sec = 16;
    int ret;
    ret = setsockopt(skt, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    if (ret == -1)
    {
        perror("setsockopt");
        return 1;
    }
    return 0;
}
// timeout send
int timeout_send(int skt)
{
    struct timeval tv;
    tv.tv_sec = 16;
    int ret;
    ret = setsockopt(skt, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));
    if (ret == -1)
    {
        perror("setsockopt");
        return 1;
    }
    return 0;
}

void test_parallel()
{
#pragma omp parallel
  {
  printf ("mx:th:%d\n",omp_get_max_threads());
  printf ("th:%d(%d)\n",omp_get_num_threads(),omp_get_thread_num());
  }
}
