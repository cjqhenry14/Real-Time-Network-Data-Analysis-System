/***
Rational AC
g++ -o ac-big.o ac-big.cpp
***/


#include    <stdio.h>
#include    <malloc.h>
#include    <stdlib.h>
#include    <string.h>
#include    <time.h>
#include    <unistd.h>
/*#define keyFile "bigkey.txt"
#define stringFile "string.txt"
#define LineMax 6
#define queMax 30//节点数
#define keyNum 5*/

#define keyFile "bigKey.txt"
#define stringFile "bigString.txt"
#define LineMax 20
#define queMax 400005//
#define keyNum 20000//20000 40000 60000 80000 100000

#define charSize 256
struct node
{
    struct node *fail; //失败指针
    struct node *next[charSize];//一个节点拥有的子节点
    int count; //是否为该单词最后一个点；1表示是最后一个点，0表示初始化，－1表示已经统计过
};
typedef struct node Node;
Node *q[queMax];//关键词每行50个词，共10000行
Node *root;
int    *keyword;//关键词
int   head, tail,j;//队列的头、尾指针

/*插入节点到字典树中*/
void insert(int * str,Node *root,int num)
{
    Node *p = root;
    int i = 0, index;
    for(i=0; i<num; i++)
    {
        index = str[i];
        if (p->next[index] == NULL)
        {
            Node *newnode;
            newnode = (Node *)malloc(sizeof(Node));
            for (j = 0; j < charSize; j++)
                newnode->next[j] = 0;
            newnode->count = 0;
            p->next[index] = newnode;
        } /*如果不存在那个节点，首先得创建节点*/
        p = p->next[index];
    }
    p->count = 1;//表示此节点为最后一个词
}
/*失败指针的建立*/
void build_fail(Node *root)
{
    int i;
    root->fail = NULL;/*让root的失败指针指向NULL，作为一种条件判断*/
    q[head++] = root;
    while (head != tail)/*当遍历完所有的节点后，head==tail*/
    {
        Node *temp = q[tail++];/*首先让指针指向root*/
        Node *p = NULL;
        for (i=0; i < charSize; i++)
        {
            if (temp->next[i] != NULL)/*找到存在的节点，用的是遍历的方法*/
            {
                if (temp == root)
                {
                    temp->next[i]->fail = root;/*root所有下一级节点的失败指针全部指向root*/
                }
                else
                {
                    p = temp->fail;/*找到失配点父节点的失败指针,其父节点的字符与该失败指针指向字符相等*/
                    while (p != NULL)
                    {
                        if (p->next[i] != NULL)/*如果p->next[i]有节点，在p->next[i]就是temp->next[i]失败指针位置*/
                        {
                            temp->next[i]->fail = p->next[i];
                            break;
                        }
                        p = p->fail;/*如果上一个if不成立，则继续向上查找*/
                    }
                    if (p == NULL)
                    {
                        temp->next[i]->fail = root;/*如果没有找到，那么将失败指针指向root*/
                    }
                }
                q[head++] = temp->next[i];
            }
        }
    }
}
/*建立关键字树*/
void build_keyword(Node *root)
{
    FILE *fp;
    int i=0,j=0;
    if ((fp=fopen(keyFile, "r"))==NULL)
    {
        printf("open keyword.txt file error!!\n");
        return;
    }
    for(i=0; i<keyNum; i++)
    {
        for(j=0; j<LineMax; j++)
        {
            keyword[j]=fgetc(fp);
        }
        keyword[j]='\0';
        insert(keyword,root,j);//根据keyword建立字典树
    }
}

//=======================================
int queryStr(Node *root)
{
    FILE *fp;
    int cnt = 0, index;
    Node *p = root;/*从root开始匹配*/
    if ((fp=fopen(stringFile, "r"))==NULL)
    {
        printf("open string.txt file error!!\n");
        return 0;
    }
    while(!feof(fp))
    {
        index=fgetc(fp);
        while (p->next[index] == NULL && p != root)
            p = p->fail;
        p = p->next[index];
        if (p == NULL)
        {
            p = root;/*没有找到匹配点，则从root出开始匹配*/
        }
        Node *temp = p;
        while (temp != root && temp->count != -1)
        {
            cnt+=temp->count;
            temp->count = -1;
            temp = temp ->fail;
        }//统计匹配到的关键字
    }
    return cnt;
}

//=======================================

int main()
{
    printf("=================acRational start==================\n");
    clock_t start,finish,middle;
    double exeTime,preTime;
    start = clock();
    keyword=(int*)malloc(LineMax*sizeof(int));
    int  i;
    head = tail = 0;
    root = (Node *)malloc(sizeof(Node));
    for (i = 0; i < charSize; i++)
        root->next[i] = 0;
    root->count = 0;
    build_keyword(root);

    build_fail(root);//建立失败指针
    //预处理时间
    middle = clock();
    preTime= (double)(middle - start) / CLOCKS_PER_SEC;
    printf("               preTime :%f\n",preTime);
    printf("                 acRational %d\n",queryStr(root));
    finish = clock();
    exeTime = (double)(finish - middle) / CLOCKS_PER_SEC;
    printf("               exeTime :%f\n",exeTime);
    printf("=================acRational end==================\n");
    //sleep(3);
    return 0;
}
