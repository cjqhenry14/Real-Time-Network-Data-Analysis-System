#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>  
#include <queue> 
#include <string> 
#include <string.h> 
#include <arpa/inet.h>

#include "attack_store.h"

using namespace std;


void store_save_queue()
{
     MYSQL *insert_conn;

     insert_conn=mysql_init(NULL);

     int res=0;

     //连接test数据库
     if(!mysql_real_connect(insert_conn,"localhost","root","","myIDS",0,NULL,0)) 
     {
        printf("Connect Error!n");
        exit(1);
     }

     while(!store_queue.empty() )
     {
        store_cache sca= store_queue.front();
       // printf("id: %d  src_ip:%16s   dst_ip:%16s   src_port:%5d   dst_port:%5d\n",
       //i,  store_luToIpadd(tempq.src_ip) ,store_luToIpadd(tempq.dst_ip),tempq.src_port,tempq.dst_port);

        char ch_type[2];
        sprintf(ch_type,"%d",sca.type);
        string str_type = ch_type;

        char ch_rule_id[10];
        sprintf(ch_rule_id,"%d",sca.rule_id);
        string str_rule_id = ch_rule_id;

        char ch_src_port[8];
        sprintf(ch_src_port,"%d",sca.src_port);
        string str_src_port = ch_src_port;

        char ch_dst_port[8];
        sprintf(ch_dst_port,"%d",sca.dst_port);
        string str_dst_port = ch_dst_port;

        string str_src_ip = store_luToIpadd(sca.src_ip);
        string str_dst_ip = store_luToIpadd(sca.dst_ip);
        string str_attack_time = sca.attack_time;

       string insert_sql ="INSERT INTO attackStore VALUES( NULL, '"+ str_type +
     	"' , '"+ str_src_ip +"' , '"+ str_dst_ip +"' , '"+ str_src_port +
        "' , '"+ str_dst_port +"' , '"+ str_rule_id +"' , '"+ str_attack_time +"');";
  
       res =mysql_real_query( insert_conn , insert_sql.c_str() , insert_sql.length() ); 

       if (!res) 
       {
         // printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(insert_conn));
       }

       else 
       {
         fprintf(stderr, "Insert error %d: %s\n", mysql_errno(insert_conn),mysql_error(insert_conn));
       }

       store_queue.pop();

     }// end while

      mysql_close(insert_conn); //关闭与数据库的连接

}


void store_put_queue(int type, u_long src_ip, u_long dst_ip, short src_port, short dst_port,
   int rule_id, char* attack_time)
{
	store_cache sc;

	sc.type = type;

    sc.src_ip = src_ip;
    sc.dst_ip = dst_ip;

    sc.src_port = src_port;
    sc.dst_port = dst_port;

    sc.rule_id = rule_id;

    sc.attack_time = attack_time;

    store_queue.push(sc);
}

char * store_luToIpadd(u_long lu){// unsigned long -> xx.xx.xx.xx
    struct in_addr inaddr;
    inaddr.s_addr=lu;
    // char * ipaddr= inet_ntoa(inaddr);
    char * ipaddr = (char *) malloc (sizeof(char)*16);
    strcpy(ipaddr,inet_ntoa(inaddr));
    return ipaddr;
}

void store_show_all_queue()
{
	
    int i=0;
    while(!store_queue.empty() )
    {
    	i++;
        store_cache tempq= store_queue.front();
        //printf("length:%5d   src_ip:%16lu   dst_ip:%16lu   src_port:%5d   dst_port:%5d\n",tempq.length, tempq.src_ip,tempq.dst_ip,tempq.src_port,tempq.dst_port);
        printf("id: %d  src_ip:%16s   dst_ip:%16s   src_port:%5d   dst_port:%5d\n",i,  store_luToIpadd(tempq.src_ip) ,store_luToIpadd(tempq.dst_ip),tempq.src_port,tempq.dst_port);
        store_queue.pop();
    }
}