/***
acvl
g++ -o detect_acvl.o $(mysql_config --cflags) detect_acvl.cpp $(mysql_config --libs) 
***/

#include    <stdio.h>
#include    <malloc.h>
#include    <stdlib.h>
#include    <time.h>
#include    <string.h>
#include    <unistd.h>
#include    <mysql.h>  
#include    <iostream>
//------ my heads ---------
#include  "detect_acvl.h"
#include "init.h"
#include "attack_store.h"

using namespace std;

//=====================
MYSQL *my_conn;// connect in build_sql_rules; close in close_detect_sql_connection();
MYSQL_RES *res;
MYSQL_ROW row;

MYSQL *tcp_conn;
MYSQL_RES *tcp_res;
MYSQL_ROW tcp_row;

MYSQL *udp_conn;
MYSQL_RES *udp_res;
MYSQL_ROW udp_row;
//=====================
ACNODE *q[queMax];

int head, tail;

int input;

int allDetectNum=0;
int udp_detect_num = 0;
int tcp_detect_num = 0;

BST R = NULL;
//=========================

void insertACVL(int * str,ACNODE *root,int num, int rule_id)
{
    int i = 0, index;
    ACNODE *p = root;
    for(i=0; i<num; i++)
    {
        index = str[i];
        if(queryAvlExist(p->avlRoot,index)==0)
        {
            ACNODE * newnode = (ACNODE *)malloc(sizeof(ACNODE));
            newnode->lchild=NULL;
            newnode->rchild=NULL;
            newnode->fail=NULL;
            newnode->bf=0;
            newnode->data=index;
            newnode->count = 0;
            newnode->avlRoot=NULL;
            p->avlRoot= buildAvl(index,p->avlRoot,newnode);
        }
        p=stepAvlNext(p->avlRoot,index);
    }
    p->count = rule_id;
}

void build_fail(ACNODE *root)
{
    ACNODE *pNext;
    int t;
    root->fail = NULL;
    q[head++] = root;
    while (head != tail)
    {
        ACNODE *temp = q[tail++];
        ACNODE *p = NULL;
        ACNODE *tt;
        for (t=0; t <charSize; t++)
        {
            tt=temp;
            if(queryAvlExist(tt->avlRoot,t)==1)
            {
                if (temp == root)
                {
                    tt=temp;
                    temp=stepAvlNext(temp->avlRoot,t);
                    temp->fail = root;
                    temp=tt;
                }
                else
                {
                    p = temp->fail;
                    while (p != NULL)
                    {
                        tt=temp;
                        pNext=p;
                        if(queryAvlExist(pNext->avlRoot,t)==1)
                        {
                            tt=temp;
                            pNext=p;
                            temp=stepAvlNext(temp->avlRoot,t);
                            p=stepAvlNext(p->avlRoot,t);
                            temp->fail =p;
                            p=pNext;
                            temp=tt;
                            break;
                        }
                        p = p->fail;
                    }
                    if (p == NULL)
                    {
                        tt=temp;
                        temp=stepAvlNext(temp->avlRoot,t);
                        temp->fail = root;
                        temp=tt;
                    }
                }
                tt=temp;
                q[head++]=stepAvlNext(tt->avlRoot,t);
            }
        }
    }
}


void build_sql_rules(ACNODE *root)
{
    //// MYSQL *my_conn;
    //// MYSQL_RES *res;
    //// MYSQL_ROW row;
    my_conn=mysql_init(NULL);


    if(!mysql_real_connect(my_conn,"localhost","root","","myIDS",0,NULL,0)) //连接test数据库
    {
        printf("Connect Error!n");
        exit(1);
    }

    // if(mysql_query(my_conn,"select * from ruleBase limit 0,20")) //top 20
    if(mysql_query(my_conn,"select * from ruleBase")) //all
    {
        printf("Query Error!n");
        exit(1);
    }

    res=mysql_use_result(my_conn); //取得表中的数据并存储到res中

    while((row=mysql_fetch_row(res))!=NULL) //打印结果
    {

        int len =  strlen(row[2]);//length of content
        int * sql_rule=(int*)malloc(len*sizeof(int));
        int rule_id = atoi(row[0]);

        int i;
        for(i=0;i<len;i++)
        {
            sql_rule[i]=row[2][i];
        }

        insertACVL(sql_rule,root,i, rule_id);

        free(sql_rule);

    }//end while

    mysql_free_result(res); //关闭结果集
    //// mysql_close(my_conn); //关闭与数据库的连接

}

int char_to_num(char  str)
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

void build_hex_sql_rules(ACNODE *root)
{
    //// MYSQL *my_conn;
    //// MYSQL_RES *res;
    //// MYSQL_ROW row;
    my_conn=mysql_init(NULL);


    if(!mysql_real_connect(my_conn,"localhost","root","","myIDS",0,NULL,0)) //连接test数据库
    {
        printf("Connect Error!n");
        exit(1);
    }

    // if(mysql_query(my_conn,"select * from ruleBase limit 0,20")) //top 20
    if(mysql_query(my_conn,"select * from ruleBase")) //all
    {
        printf("Query Error!n");
        exit(1);
    }

    res=mysql_use_result(my_conn); //取得表中的数据并存储到res中

    while((row=mysql_fetch_row(res))!=NULL) //打印结果
    {

        int len =  strlen(row[3]);//length of content
        int hex_len=len/2;
        int * sql_rule=(int*)malloc(hex_len*sizeof(int));
        int rule_id = atoi(row[0]);

        //686F7374
        int i=0;
        int j=0;
        while(i<len)
        {
            sql_rule[j]= char_to_num(row[3][i+1]) + char_to_num(row[3][i])*16;
            //
            i=i+2;
            j++;
        }

        insertACVL(sql_rule,root,hex_len, rule_id);

        free(sql_rule);///

    }//end while

    mysql_free_result(res); //关闭结果集
    mysql_close(my_conn); //关闭与数据库的连接

}

void close_detect_sql_connection()
{
    mysql_close(tcp_conn);
    mysql_close(udp_conn);
}

//================================ TCP and UDP ===============================

void return_tcp_detect_info(tcp_cache tc, int tcp_detect_num, ACNODE *temp)
{
    detect_info di;

    di.rule_id=temp->count;
    di.attack_id=tcp_detect_num;
    di.pro_type=1;

    detect_info_queue.push(di);
}

void start_tcp_sql_conn(){

    tcp_conn=mysql_init(NULL);

    if(!mysql_real_connect(tcp_conn,"localhost","root","","myIDS",0,NULL,0)) //连接test数据库
    {
        printf("Connect Error!n");
        exit(1);
    }
}

void start_udp_sql_conn(){

    udp_conn=mysql_init(NULL);

    if(!mysql_real_connect(udp_conn,"localhost","root","","myIDS",0,NULL,0)) //连接test数据库
    {
        printf("Connect Error!n");
        exit(1);
    }
}


void show_tcp_detect_info(tcp_cache tc, ACNODE *temp)
{
       time_t rawtime;
       struct tm * timeinfo;

       time ( &rawtime );
       timeinfo = localtime ( &rawtime );
       string datetime=asctime (timeinfo);
        //printf ( "The current date/time is: %s\n", asctime (timeinfo) );

       /*start store in queue*/
       store_put_queue(0, tc.src_ip, tc.dst_ip, tc.src_port, tc.dst_port,
          temp->count, asctime (timeinfo));

        cout<<" =======   TCP ATTACK "<<tcp_detect_num<<"    rule_id "<<temp->count<<endl;
        char ch_did[5];
        sprintf(ch_did,"%d",tcp_detect_num);
        string str_did = ch_did;

        /*tcp count label update*/
        gdk_threads_enter();
        gtk_label_set_text(GTK_LABEL(tcpCountLabel),ch_did);
        gdk_threads_leave();

        char ch_rule_id[5];
        sprintf(ch_rule_id,"%d",temp->count);
        string str_rule_id = ch_rule_id;

        string srcip =luToIpadd(tc.src_ip);
        string dstip =luToIpadd(tc.dst_ip);

        char ch_src_port[7];
        sprintf(ch_src_port,"%d",tc.src_port);
        string srcport =ch_src_port;

        char ch_dst_port[7];
        sprintf(ch_dst_port,"%d",tc.dst_port);
        string dstport =ch_dst_port;

       //====== get rule info from sql ===============
       // msg-1 content-2 type-4
       string get_rule_info_sql ="select * from ruleBase where id = '"+ str_rule_id +"';";

       if(mysql_real_query(tcp_conn, get_rule_info_sql.c_str(), get_rule_info_sql.length() )) //连接WordC表
       {
           printf("TCP Query Error!n");
           exit(1);
       }

       tcp_res=mysql_use_result(tcp_conn); //取得表中的数据并存储到res中

        string msg,content,type;

       while((tcp_row=mysql_fetch_row(tcp_res))!=NULL) //打印结果
       {
         msg=tcp_row[1];

         content=tcp_row[2];

         type=tcp_row[4];

       }
/*No:62rule_id: 982.  [67.215.253.139] [80] -> [192.168.1.107] [56831]  ---  TIME: Sun Apr 13 13:24:58 2014
TYPE: misc-attack   |   MSG: PROTOCOL-SNMP null community string attempt
*/
      // string show_str ="# "+ str_did+"rule_id: "+str_rule_id+
          string show_str ="# "+ str_did+
      ".    ["+ srcip +"] ["+srcport+"] -> ["+ dstip +"] ["+dstport+"]"+"  ---  攻击时间: "+datetime+
      "攻击类型: "+type+ "   |   攻击特征: "+ content +
    "\n报警信息: "+msg+"\n\n";
      
  gdk_threads_enter();
        gtk_text_buffer_insert (tcpBuffer, &tcpIter, show_str.c_str(),-1);
    gdk_threads_leave();
        

       mysql_free_result(tcp_res); //关闭结果集

    //====== end of get rule info from sql ===============

}

void show_udp_detect_info(udp_cache tu, ACNODE *temp)
{

       time_t rawtime;
       struct tm * timeinfo;

       time ( &rawtime );
       timeinfo = localtime ( &rawtime );
       string datetime=asctime (timeinfo);

    cout<<" ==========  UDP ATTACK "<<udp_detect_num<<" rule_id "<<temp->count<<endl;

    store_put_queue(1, tu.src_ip, tu.dst_ip, tu.src_port, tu.dst_port,
          temp->count, asctime (timeinfo));

    //cout<< luToIpadd(tu.src_ip) << luToIpadd (tu.dst_ip) << tu.src_port << tu.dst_port <<endl;
        char ch_did[5];
        sprintf(ch_did,"%d",udp_detect_num);
        string str_did = ch_did;

         /*udp count label update*/
        gdk_threads_enter();
        gtk_label_set_text(GTK_LABEL(udpCountLabel),ch_did);
        gdk_threads_leave();

        char ch_rule_id[5];
        sprintf(ch_rule_id,"%d",temp->count);
        string str_rule_id = ch_rule_id;

        string srcip =luToIpadd(tu.src_ip);
        string dstip =luToIpadd(tu.dst_ip);

        char ch_src_port[7];
        sprintf(ch_src_port,"%d",tu.src_port);
        string srcport =ch_src_port;

        char ch_dst_port[7];
        sprintf(ch_dst_port,"%d",tu.dst_port);
        string dstport =ch_dst_port;

       //====== get rule info from sql ===============
       // msg-1 content-2 type-4
       string get_rule_info_sql ="select * from ruleBase where id = '"+ str_rule_id +"';";

       if(mysql_real_query(udp_conn, get_rule_info_sql.c_str(), get_rule_info_sql.length() )) //连接WordC表
       {
           printf("UDP Query Error!n");
           exit(1);
       }

       udp_res=mysql_use_result(udp_conn); //取得表中的数据并存储到res中

        string msg,content,type;

       while((udp_row=mysql_fetch_row(udp_res))!=NULL) //打印结果
       {
         msg=udp_row[1];

         content=udp_row[2];

         type=udp_row[4];

       }

      //  string show_str ="No:"+ str_did+"rule_id: "+str_rule_id+
      // ".  ["+ srcip +"] ["+srcport+"] -> ["+ dstip +"] ["+dstport+"]"+"  ---  TIME: "+datetime+
      // "TYPE: "+type+ "   |   MSG: "+ msg +"\n\n";
      string show_str ="# "+ str_did+
      ".    ["+ srcip +"] ["+srcport+"] -> ["+ dstip +"] ["+dstport+"]"+"  ---  攻击时间: "+datetime+
      "攻击类型: "+type+ "   |   攻击特征: "+ content +
    "\n报警信息: "+msg+"\n\n";
    
        gdk_threads_enter();

        gtk_text_buffer_insert (udpBuffer, &udpIter, show_str.c_str(),-1);

        gdk_threads_leave();

       mysql_free_result(udp_res); //关闭结果集

    //====== end of get rule info from sql ===============

}

ACNODE * ac_tcp_query_content(tcp_cache tc, ACNODE *root, ACNODE *status)// query content
{
    unsigned char * content = tc.content;
  
    ACNODE *tt;
    //int tcp_detect_num = 0;
    int index,i=0;
    ACNODE *p = status;
    
   // while(content[i])// should be hex 07 b2 87
    while(i<tc.length)
    {
        index= tc.content[i];
        tt=p;
        while ((queryAvlExist(tt->avlRoot,index)==0) &&tt!= root)
        {
            p = p->fail;
            tt=p;
        }
        tt=p;
        p=stepAvlNext(tt->avlRoot,index);
        if (p == NULL)
            p = root;
        
        ACNODE *temp = p;
        ///while (temp != root && temp->count != -1)
        while (temp != root)
        {
            if(temp->count>0)
            {
                tcp_detect_num++;
                allDetectNum++;
                show_tcp_detect_info(tc, temp);
                //return_tcp_detect_info(tc, tcp_detect_num, temp);

            }

            //temp->count = -1;
            temp = temp ->fail;
        }
        i++;
    }

    return p;// status

}

ACNODE * ac_udp_query_content(udp_cache tu, ACNODE *root, ACNODE *status)// query content
{
    unsigned char * content = tu.content;
    ACNODE *tt;


    int index,i=0;
    ACNODE *p = status;
    
    while(i<tu.length)
    {
        index=content[i];
        tt=p;
        while ((queryAvlExist(tt->avlRoot,index)==0) &&tt!= root)
        {
            p = p->fail;
            tt=p;
        }
        tt=p;
        p=stepAvlNext(tt->avlRoot,index);
        if (p == NULL)
            p = root;
        
        ACNODE *temp = p;
        ///while (temp != root && temp->count != -1)
        while (temp != root)
        {
            if(temp->count>0)
            {
                udp_detect_num++;
                allDetectNum++;
                show_udp_detect_info(tu, temp);

            }

            //temp->count = -1;
            temp = temp ->fail;
        }
        i++;
    }

    return p;// status

}
//================================END of  TCP and UDP ===============================


int ac_query_file_test(ACNODE *root)
{
    FILE *fp;
    ACNODE *tt;
    int cnt = 0, index;
    ACNODE *p = root;
    if ((fp=fopen(stringFile, "r"))==NULL)
    {
        printf("open string.txt file error!!\n");
        return 0;
    }
    while(!feof(fp))
    {
        index=fgetc(fp);
        tt=p;
        while ((queryAvlExist(tt->avlRoot,index)==0) &&tt!= root)
        {
            p = p->fail;
            tt=p;
        }
        tt=p;
        p=stepAvlNext(tt->avlRoot,index);
        if (p == NULL)
            p = root;

        ACNODE *temp = p;
        while (temp != root && temp->count != -1)
        {
            if(temp->count>0)
            {
                printf("--  rule_id -- %d\n", temp->count);
                cnt++;
            }

            temp->count = -1;
            temp = temp ->fail;
        }
    }

    fclose(fp);
    return cnt;
}

bool ac_build_rules(ACNODE *root)
{
    head = tail = 0;
    root->avlRoot=NULL;
    root->data=-2;
    root->count = 0;

    //build_sql_rules(root);
    build_hex_sql_rules(root);

    build_fail(root);

    root->fail=root;

    return true;
    
}


void ac_detect(ACNODE *root)
{
    // printf("          ACVL %d\n",ac_query_file_test(root));
}


//====================   avl   =========================


int Height(BST T)
{
    if (T == NULL)
        return -1;
    else
        return T->bf;
}


int Max(int A, int B)
{
    return ((A > B) ? A:B);
}


BST SingleRotateWithRight(BST K2,BST K1)
{
    K1 = K2->rchild;
    K2->rchild = K1->lchild;
    K1->lchild = K2;
    K2->bf = Max(Height(K2->lchild), Height(K2->rchild)) + 1;
    K1->bf = Max(Height(K1->lchild), Height(K1->rchild)) + 1;
    return K1;
}


BST SingleRotateWithLeft(BST K2,BST K1)
{
    K1 = K2->lchild;
    K2->lchild = K1->rchild;
    K1->rchild = K2;
    K2->bf = Max(Height(K2->lchild), Height(K2->rchild)) + 1;
    K1->bf = Max(Height(K1->lchild), Height(K1->rchild)) + 1;
    return K1;
}


BST DoubleRotateWithLeft(BST K3,BST newnode)
{
    K3->lchild = SingleRotateWithRight(K3->lchild,newnode);
    return SingleRotateWithLeft(K3,newnode);
}


BST DoubleRotateWithRight(BST K3,BST newnode)
{
    K3->rchild = SingleRotateWithLeft(K3->rchild,newnode);
    return SingleRotateWithRight(K3,newnode);
}


BST AVLInsert(BST T,BST newnode)
{
    if (T == NULL)
    {
        newnode->bf = Max(Height(newnode->lchild), Height(newnode->rchild)) + 1;
        return newnode;

    }
    else if (input < T->data)
    {

        T->lchild = AVLInsert(T->lchild,newnode);
        if (Height(T->lchild) - Height(T->rchild) == 2)
        {
            if (input < T->lchild->data)
            {
                T = SingleRotateWithLeft(T,newnode);
            }
            else
            {
                T = DoubleRotateWithLeft(T,newnode);
            }
        }
    }
    else if (input > T->data)
    {
        T->rchild  = AVLInsert(T->rchild,newnode);
        if (Height(T->rchild) - Height(T->lchild) == 2)
        {
            if (input > T->rchild->data)
            {
                T = SingleRotateWithRight(T,newnode);
            }
            else
            {
                T = DoubleRotateWithRight(T,newnode);
            }
        }
    }
    T->bf = Max(Height(T->lchild), Height(T->rchild)) + 1;
    return T;
}


int  queryAvlExist(BST T,int index)
{
    while(T!=NULL)
    {
        if(T->data==index)
            return 1;
        else if(T->data>index)
        {
            T=T->lchild;
        }
        else if(T->data<index)
        {
            T=T->rchild;
        }
    }
    return 0;
}


BST  stepAvlNext(BST T,int index)
{
    while(T!=NULL)
    {
        if(T->data==index)
        {
            return T;
        }
        else if(T->data>index)
            T=T->lchild;
        else T=T->rchild;
    }
    return NULL;
}

BST buildAvl(int index,BST T,BST newnode)
{
    input=index;
    R = AVLInsert(T,newnode);
    return R;
}
