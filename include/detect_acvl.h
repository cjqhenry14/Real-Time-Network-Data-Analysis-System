#ifndef DETECT_ACVL_H
#define DETECT_ACVL_H

#include "preprocessor_process.h"

#define stringFile "42256"//community.rules  string.txt 53015  33399 35262 42256

#define queMax 45000//queue max num  all:41903(45000)  1-20:263(300)   53015:5000

#define charSize 256

struct ACVL
{
    struct ACVL *fail;
    struct ACVL *avlRoot;
    int count;
    int data;
    int bf;
    struct ACVL *lchild;
    struct ACVL *rchild;
};
typedef struct ACVL ACNODE,*BST;


int ac_query_file_test(ACNODE *root);

BST  buildAvl(int index,BST T,BST newnode);

BST  stepAvlNext(BST T,int index);

int  queryAvlExist(BST T,int index);

//build rules
bool ac_build_rules(ACNODE *root);

void show_tcp_detect_info(tcp_cache tc, int tcp_detect_num, ACNODE *temp);

// return detect_info to JNA | for test , just return tcp_detect_num
void return_tcp_detect_info(tcp_cache tc, int tcp_detect_num, ACNODE *temp);

// main interface; begin detect
void ac_detect(ACNODE *root);

ACNODE * ac_tcp_query_content(tcp_cache tc, ACNODE *root, ACNODE *status);

//==============  UDP ===============================

void show_udp_detect_info(udp_cache tc, int udp_detect_num, ACNODE *temp);

ACNODE * ac_udp_query_content(udp_cache tc, ACNODE *root, ACNODE *status);

void close_detect_sql_connection();// close_detect_sql_connection

void start_tcp_sql_conn();

void start_udp_sql_conn();

#endif

