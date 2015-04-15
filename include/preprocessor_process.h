#ifndef PREPROCESSOR_PROCESS_H
#define PREPROCESSOR_PROCESS_H

#include <queue> 
#include <arpa/inet.h>//   u_long
#include <hash_map>
#include "decoder_dataCache.h"
#include "detect_acvl.h"

using namespace std;
using namespace __gnu_cxx;
// calculate tuple to hash index.
u_long cal_hash_index(u_long src_ip, u_long dst_ip, u_short src_port, u_short dst_port);
//==============  TCP ========================

struct TCP_CHECK_STRUCT
{
    struct ACVL *status;
	int time;
	u_long next_seq;
    
};
typedef struct TCP_CHECK_STRUCT tcp_check;

//void get_tcp_cache(queue <tcp_cache> &q, struct ACVL *root);
void * get_tcp_cache_thread(void *arg);

void goto_tcp_ac_detect(tcp_cache tc, struct ACVL *root);

void show_all_tcp_hash(hash_map<u_long, tcp_check> tcp_hash);

//==============  UDP ========================

struct UDP_CHECK_STRUCT
{

    struct ACVL *status;
	int time;
    
};
typedef struct UDP_CHECK_STRUCT udp_check;

void * get_udp_cache_thread(void *arg);

void goto_udp_ac_detect(udp_cache uc);


#endif