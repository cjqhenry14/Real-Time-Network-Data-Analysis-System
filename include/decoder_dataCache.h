#ifndef DECODER_DATACACHE_H
#define DECODER_DATACACHE_H

#include <queue> 
#include <arpa/inet.h>//   u_long
using namespace std;

char * luToIpadd(u_long lu);
//==============  TCP ========================
/*1.结构体成员指针需要初始化*/ 

struct TCP_CACHE_STRUCT
{

   // char src_ip[16];
   // char dst_ip[16];//unsigned long dst_ip = inet_addr("132.241.5.10");将IP地址从 点数格式转换成无符号长整型。
	u_long src_ip;
	u_long dst_ip;
    u_short src_port;
    u_short dst_port;
     unsigned char * content;
    u_long  seq;
    u_short length;
    bool fin;

    //struct TCP_CACHE_STRUCT *next;
    
};
typedef struct TCP_CACHE_STRUCT tcp_cache;

void sql_put_tcp_cache( u_long src_ip, u_long dst_ip, u_short src_port, u_short dst_port,unsigned char* content, u_long seq, u_short length, bool fin);
 
queue <tcp_cache> sql_init_tcp_queue();

void sql_show_tcp_queue(queue <tcp_cache> &q);

//==============  UDP ========================
struct UDP_CACHE_STRUCT
{

	u_long src_ip;
	u_long dst_ip;
    u_short src_port;
    u_short dst_port;
    unsigned char * content;
    u_short length;

    //struct UDP_CACHE_STRUCT *next;
    
};
typedef struct UDP_CACHE_STRUCT udp_cache;

void sql_put_udp_cache(u_long  src_ip, u_long dst_ip, u_short src_port, u_short dst_port, unsigned char* content, u_short length);

queue <udp_cache> sql_init_udp_queue();

void sql_show_udp_queue(queue <udp_cache> &q);
/*
struct CACHE_QUEUE
{

   queue <tcp_cache> tcp_queue ;//TCP INIT
   queue <udp_cache> udp_queue ;//UDP INIT
    
};
typedef struct CACHE_QUEUE cache_queue;*/
struct DETECT_INFO_STRUCT
{
/*    u_long src_ip;
    u_long dst_ip;
    u_short src_port;
    u_short dst_port;*/
    int  attack_id;
    int  rule_id;
    bool pro_type;//0-udp   1-tcp

};
typedef struct DETECT_INFO_STRUCT detect_info;
// externs are init in sqlToCache.cpp
  extern queue <tcp_cache> tcp_queue ;//TCP INIT
  extern queue <udp_cache> udp_queue ;//UDP INIT

  extern pthread_mutex_t tcpMut;
  extern pthread_mutex_t udpMut;

  extern queue <detect_info> detect_info_queue ;//UDP INIT

  extern pthread_cond_t hasNode;
  extern pthread_cond_t hasUdpNode;

#endif

