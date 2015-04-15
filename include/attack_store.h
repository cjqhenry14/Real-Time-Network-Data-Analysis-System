#ifndef ATTACK_STORE_H
#define ATTACK_STORE_H

#include <arpa/inet.h>
#include <queue> 

using namespace std;

char * store_luToIpadd(u_long lu);
/*1.结构体成员指针需要初始化*/ 

struct ATTACK_STORE_STRUCT
{

    u_short type;
   	u_long  src_ip;
	  u_long  dst_ip;
    u_short src_port;
    u_short dst_port;

    int rule_id;
    char * attack_time;

};
typedef struct ATTACK_STORE_STRUCT store_cache;


extern queue <store_cache> store_queue ;

void store_put_queue(int type, u_long src_ip, u_long dst_ip, short src_port, short dst_port,
   int rule_id, char* attack_time);

void store_show_all_queue();

void store_save_queue();



#endif

