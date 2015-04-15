#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <queue> 
#include <string.h>
#include <arpa/inet.h>
#include "decoder_dataCache.h"

using namespace std;
/**
 0      1     2       3      4          5          6        7      8       9
id    type  src_ip  dst_ip  src_port   dst_port  content   seq   length   fin
***/
char * luToIpadd(u_long lu){// unsigned long -> xx.xx.xx.xx
    struct in_addr inaddr;
    inaddr.s_addr=lu;
    // char * ipaddr= inet_ntoa(inaddr);
    char * ipaddr = (char *) malloc (sizeof(char)*16);
    strcpy(ipaddr,inet_ntoa(inaddr));
    return ipaddr;
}
//=============================================  TCP  ====================================

queue <tcp_cache> sql_init_tcp_queue()
{
    queue <tcp_cache> tcp_queue;
    return tcp_queue;
}

void sql_show_tcp_queue(queue <tcp_cache> &q)
{
    printf("  **************************  TCP CACHE  **************************  \n");
    while(!q.empty() )
    {
        tcp_cache tempq= q.front();
        //printf("length:%5d   src_ip:%16lu   dst_ip:%16lu   src_port:%5d   dst_port:%5d\n",tempq.length, tempq.src_ip,tempq.dst_ip,tempq.src_port,tempq.dst_port);
        printf("length:%5d   src_ip:%16s   dst_ip:%16s   src_port:%5d   dst_port:%5d\n",tempq.length, luToIpadd(tempq.src_ip) ,luToIpadd(tempq.dst_ip),tempq.src_port,tempq.dst_port);
        q.pop();
    }
}

void sql_put_tcp_cache(u_long src_ip, u_long dst_ip,
                       u_short src_port, u_short dst_port,unsigned char* content, u_long seq, u_short length, bool fin)
{
    tcp_cache tc;
    tc.src_ip=src_ip;
    tc.dst_ip=dst_ip;

    tc.src_port =  src_port;
    tc.dst_port =  dst_port;

    tc.content = content;

    tc.seq =  seq;
    tc.length =  length;
    tc.fin =  fin;

    tcp_queue.push(tc);

}
//=================================================  UDP  ====================================

queue <udp_cache> sql_init_udp_queue()
{
    queue <udp_cache> udp_queue;
    return udp_queue;
}

void sql_show_udp_queue(queue <udp_cache> &q)
{
    printf("  **************************  UDP CACHE  **************************  \n");
    while(!q.empty() )
    {
        udp_cache tempq= q.front();

        printf("length:%5d   src_ip:%16s   dst_ip:%16s   src_port:%5d   dst_port:%5d\n",tempq.length, luToIpadd(tempq.src_ip),luToIpadd(tempq.dst_ip),tempq.src_port,tempq.dst_port);
        q.pop();
    }
}

void sql_put_udp_cache(u_long src_ip, u_long dst_ip,
                       u_short src_port, u_short dst_port,unsigned char* content, u_short length)
{
    udp_cache uc;
    uc.src_ip=src_ip;
    uc.dst_ip=dst_ip;

    uc.src_port =  src_port;
    uc.dst_port =  dst_port;

    uc.content = content;

    uc.length =  length;

    udp_queue.push(uc);
}
