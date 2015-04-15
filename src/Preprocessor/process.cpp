
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <hash_map>

#include "preprocessor_process.h"
#include "detect_acvl.h"
#include   <unistd.h>
#include "init.h"
using namespace std;
using namespace __gnu_cxx;

void  test_write_tcp_content(int dst_port, unsigned char *content, int length);
//============================ TCP ============================

void * get_tcp_cache_thread(void *arg)//queue <tcp_cache> &q, struct ACVL *root
{

    struct ACVL *root = (struct ACVL *)arg;

    hash_map<u_long, tcp_check> tcp_hash;
    hash_map <u_long, tcp_check> :: iterator it;

    while(1){

      pthread_mutex_lock(&tcpMut);
      usleep(100);//0.1s
      if(tcp_queue.empty() )
      {
          pthread_cond_wait(&hasNode,&tcpMut);
      }
    // if(!tcp_queue.empty() )
    // {
      tcp_cache tempq= tcp_queue.front();

        // calculate tuple to hash index.
      u_long hash_index=cal_hash_index(tempq.src_ip ,tempq.dst_ip,tempq.src_port,tempq.dst_port);

      it=tcp_hash.find(hash_index);

        if(it == tcp_hash.end())//no index ; then insert into hash table
        {
            tcp_check new_tcp_hash_node;

            new_tcp_hash_node.time = 10;
            new_tcp_hash_node.status = root;//
            new_tcp_hash_node.next_seq = tempq.seq + tempq.length;

            tcp_hash[hash_index]=new_tcp_hash_node;
            //content  root  status
           ///// test_write_tcp_content(tempq.dst_port, tempq.content, tempq.length);
            new_tcp_hash_node.status=ac_tcp_query_content(tempq, root, new_tcp_hash_node.status);// send tcp_cache to the AC Automation



            if(tempq.fin==1)// fin=1, after detection, delete from hash
            {
                tcp_hash.erase(hash_index);
            }

        }

        else //already in the hash table; then check seq and fin
        {

            if(it->second.next_seq == tempq.seq)//check seq
            {
                it->second.next_seq= tempq.seq + tempq.length;
               ///// test_write_tcp_content(tempq.dst_port, tempq.content,tempq.length);
                it->second.status = ac_tcp_query_content(tempq, root, it->second.status);// send tcp_cache to the AC Automation
            }

            else //seq is not right; (delete from hash table)
            {
                tcp_hash.erase(hash_index);
            }

            if(tempq.fin==1)// fin=1, after detection, delete from hash
            {
                tcp_hash.erase(hash_index);
            }

        }


      //  pthread_mutex_lock(&tcpMut);
        pthread_mutex_unlock(&tcpMut);
        tcp_queue.pop();

        


    //}// end if
    }// end while 1

}
//D59B24371B66E7
/*write from sql into file, then test ac is right or not*/
void  test_write_tcp_content(int dst_port, unsigned char *content, int length)
{
    //char * temp_content = (char *) malloc (sizeof(char)*length);
    //  strcpy(temp_content,content);
    char fileName[10];
    sprintf(fileName,"%d",dst_port);

    FILE * pFile;
    pFile = fopen ( fileName , "ab" );//ab binary

    int i=0;
    while(i<length)
    {
        fwrite (&content[i] , 1, 1 , pFile );
        i++;
    }

    // fwrite (temp_content , length, 1 , pFile );

    fclose (pFile);

}


void goto_tcp_ac_detect( tcp_cache tc, ACNODE *root)
{

    printf("src_ip:%16s   dst_ip:%16s   src_port:%5d   dst_port:%5d  seq:%12lu\n",
     luToIpadd(tc.src_ip), luToIpadd(tc.dst_ip), tc.src_port, tc.dst_port, tc.seq);

    // test_write_tcp_content(tc.dst_port, tc.content, tc.length);
    /*go to ac_detect(in detect_acvl.cpp)*/
    //  ac_detect();

}




void show_all_tcp_hash(hash_map<u_long, tcp_check> tcp_hash)
{
    hash_map <u_long, tcp_check> :: iterator Iter1;
    Iter1 = tcp_hash.begin();

    while(Iter1 != tcp_hash.end())
    {
        //printf("src_ip: %16s  ; next_seq: %12lu\n", luToIpadd(Iter1->second.src_ip), Iter1->second.next_seq);
        Iter1++ ;
    }

}

//============================ UDP ====================================================
void * get_udp_cache_thread(void *arg)//queue <tcp_cache> &q, struct ACVL *root
{
    struct ACVL *root = (struct ACVL *)arg;

    hash_map<u_long, udp_check> udp_hash;
    hash_map <u_long, udp_check> :: iterator it;

    while(1){
      pthread_mutex_lock(&udpMut);
      usleep(100);//0.1s
      
      if(udp_queue.empty() )
      {
          pthread_cond_wait(&hasUdpNode,&udpMut);
      }

       // if(!udp_queue.empty() )
       // {
           
            udp_cache tempq= udp_queue.front();

        // calculate tuple to hash index.
            u_long hash_index=cal_hash_index(tempq.src_ip ,tempq.dst_ip,tempq.src_port,tempq.dst_port);

            it=udp_hash.find(hash_index);

        if(it == udp_hash.end())//no index ; then insert into hash table
        {
            udp_check new_udp_hash_node;

            new_udp_hash_node.status = root;//

            new_udp_hash_node.time = 10;

            udp_hash[hash_index]=new_udp_hash_node;

            /////test_write_tcp_content(tempq.dst_port, tempq.content, tempq.length);

            new_udp_hash_node.status=ac_udp_query_content(tempq, root, new_udp_hash_node.status);

            //  goto_udp_ac_detect(tempq);// send udp_cache to the AC Automation

        }

        else //already in the hash table
        {
            //goto_udp_ac_detect(tempq);// send udp_cache to the AC Automation
           ///// test_write_tcp_content(tempq.dst_port, tempq.content, tempq.length);
            it->second.status = ac_udp_query_content(tempq, root, it->second.status);

        }

       // pthread_mutex_lock(&udpMut);

        udp_queue.pop();

        pthread_mutex_unlock(&udpMut);


    //}// end if(!udp_queue.empty() )
}

}


void goto_udp_ac_detect(udp_cache uc)
{

    printf("src_ip:%16s   dst_ip:%16s   src_port:%5d   dst_port:%5d \n",
     luToIpadd(uc.src_ip), luToIpadd(uc.dst_ip), uc.src_port, uc.dst_port);

}



//============================ COMMON ==============================

u_long cal_hash_index(u_long src_ip, u_long dst_ip, u_short src_port, u_short dst_port)//plus; maybe better
{
    u_long hash_index=src_ip+dst_ip+src_port+dst_port;
    return hash_index;
}
