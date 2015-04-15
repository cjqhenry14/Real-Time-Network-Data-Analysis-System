/***
four elements hashtable test1.
g++  -o sqlToCache.o $(mysql_config --cflags) sqlToCache.cpp  $(mysql_config --libs)

g++ -I ../../include/ -o sqlToCache.o  $(mysql_config --cflags) sqlToCache.cpp ../Decoder/dataCache.cpp $(mysql_config --libs)

0      1     2       3      4          5          6        7      8       9  
id    type  src_ip  dst_ip  src_port   dst_port  content   seq   length   fin
***/

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <cstring>
#include <queue> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "decoder_dataCache.h"// thread 1-> put into cache

using namespace std;

int sniffer_char_to_num(char  str)
{
    if ((str>='0') && (str<='9'))
        return str-48;//字符0的ASCII值为48

    else if ((str>='a') && (str<='f'))
        return str-'a'+10;

    else if ((str>='A') && (str<='F'))
        return str-'A'+10;

    else
        return 0;

}

void* get_hex_data_from_sql(void *arg){

    MYSQL *my_conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    my_conn=mysql_init(NULL);

    if(!mysql_real_connect(my_conn,"localhost","root","","myIDS",0,NULL,0)) //连接test数据库
    {
        printf("Connect Error!n");
        exit(1);
    }

    if(mysql_query(my_conn,"select * from pcapData")) //top 20 limit 8,10 netDataTest testPcapHex
    {
        printf("Query Error!n");
        exit(1);
    }

    res=mysql_use_result(my_conn); //取得表中的数据并存储到res中

    while((row=mysql_fetch_row(res))!=NULL) //put into cache
    {
        int src_port=atoi(row[4]);
        int dst_port=atoi(row[5]);
        int length=atoi(row[8]);//length of content
        bool fin=atoi(row[9]);
        unsigned long seq= atol(row[7]);
        int templen=2*length;
       

 // row[6]is hex, content should be payload,one bit one word.
        unsigned char * content = (unsigned char *) malloc (sizeof(unsigned char)*length);
        //686F7374
        int i=0;
        int j=0;
       // printf("%c%c", row[6][i],row[6][i+1]);
         // char * sql_content = (char *) malloc (sizeof(char)*2*templen);         
         //     memcpy(sql_content, row[6], templen);

        while(i<templen)
        {
            content[j]= sniffer_char_to_num(row[6][i+1]) + sniffer_char_to_num(row[6][i])*16;
             
            i=i+2;
            j++;
        }

   // printf("%s", row[6]);
        if(strcmp(row[1],"0")==0)// TCP
        {
            pthread_mutex_lock(&tcpMut);

           sql_put_tcp_cache(inet_addr(row[2]),inet_addr(row[3]),src_port,dst_port,content,seq,length,fin);

            pthread_mutex_unlock(&tcpMut);
             pthread_cond_signal(&hasNode);

        }

        else if (strcmp(row[1],"1")==0)// UDP
        {
            pthread_mutex_lock(&udpMut);

           sql_put_udp_cache(inet_addr(row[2]),inet_addr(row[3]),src_port,dst_port,content,length);

            pthread_mutex_unlock(&udpMut);
            pthread_cond_signal(&hasUdpNode);
        }

    }

    mysql_free_result(res);
    mysql_close(my_conn);
}



//-===============================================================================


void* get_data_from_sql(void *arg){

    MYSQL *my_conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    my_conn=mysql_init(NULL);

    if(!mysql_real_connect(my_conn,"localhost","root","","myIDS",0,NULL,0)) //连接test数据库
    {
        printf("Connect Error!n");
        exit(1);
    }

    if(mysql_query(my_conn,"select * from netDataTest")) //top 20 limit 8,10 netDataTest testPcapHex
    {
        printf("Query Error!n");
        exit(1);
    }

    res=mysql_use_result(my_conn); //取得表中的数据并存储到res中
    /**
 0      1     2       3      4          5          6        7      8       9
id    type  src_ip  dst_ip  src_port   dst_port  content   seq   length   fin
***/


    while((row=mysql_fetch_row(res))!=NULL) //put into cache
    {
        int src_port=atoi(row[4]);
        int dst_port=atoi(row[5]);
        int length=atoi(row[8]);
        bool fin=atoi(row[9]);
        unsigned long seq= atol(row[7]);
        int templen=2*length;

        //printf("%s %s \n",row[1],row[2]);
       unsigned char * content = (unsigned char *) malloc (sizeof(unsigned char)*2*templen);
            // strcpy(content,row[6]);
             memcpy(content, row[6], templen);

        if(strcmp(row[1],"0")==0)// TCP
        {
            pthread_mutex_lock(&tcpMut);

           sql_put_tcp_cache(inet_addr(row[2]),inet_addr(row[3]),src_port,dst_port,content,seq,length,fin);

            pthread_mutex_unlock(&tcpMut);
             pthread_cond_signal(&hasNode);

        }

        else if (strcmp(row[1],"1")==0)// UDP
        {
            pthread_mutex_lock(&udpMut);

           sql_put_udp_cache(inet_addr(row[2]),inet_addr(row[3]),src_port,dst_port,content,length);

            pthread_mutex_unlock(&udpMut);
            pthread_cond_signal(&hasUdpNode);
        }

    }


    mysql_free_result(res);
    mysql_close(my_conn);
}
