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
#define queMax 30//�ڵ���
#define keyNum 5*/

#define keyFile "bigKey.txt"
#define stringFile "bigString.txt"
#define LineMax 20
#define queMax 400005//
#define keyNum 20000//20000 40000 60000 80000 100000

#define charSize 256
struct node
{
    struct node *fail; //ʧ��ָ��
    struct node *next[charSize];//һ���ڵ�ӵ�е��ӽڵ�
    int count; //�Ƿ�Ϊ�õ������һ���㣻1��ʾ�����һ���㣬0��ʾ��ʼ������1��ʾ�Ѿ�ͳ�ƹ�
};
typedef struct node Node;
Node *q[queMax];//�ؼ���ÿ��50���ʣ���10000��
Node *root;
int    *keyword;//�ؼ���
int   head, tail,j;//���е�ͷ��βָ��

/*����ڵ㵽�ֵ�����*/
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
        } /*����������Ǹ��ڵ㣬���ȵô����ڵ�*/
        p = p->next[index];
    }
    p->count = 1;//��ʾ�˽ڵ�Ϊ���һ����
}
/*ʧ��ָ��Ľ���*/
void build_fail(Node *root)
{
    int i;
    root->fail = NULL;/*��root��ʧ��ָ��ָ��NULL����Ϊһ�������ж�*/
    q[head++] = root;
    while (head != tail)/*�����������еĽڵ��head==tail*/
    {
        Node *temp = q[tail++];/*������ָ��ָ��root*/
        Node *p = NULL;
        for (i=0; i < charSize; i++)
        {
            if (temp->next[i] != NULL)/*�ҵ����ڵĽڵ㣬�õ��Ǳ����ķ���*/
            {
                if (temp == root)
                {
                    temp->next[i]->fail = root;/*root������һ���ڵ��ʧ��ָ��ȫ��ָ��root*/
                }
                else
                {
                    p = temp->fail;/*�ҵ�ʧ��㸸�ڵ��ʧ��ָ��,�丸�ڵ���ַ����ʧ��ָ��ָ���ַ����*/
                    while (p != NULL)
                    {
                        if (p->next[i] != NULL)/*���p->next[i]�нڵ㣬��p->next[i]����temp->next[i]ʧ��ָ��λ��*/
                        {
                            temp->next[i]->fail = p->next[i];
                            break;
                        }
                        p = p->fail;/*�����һ��if����������������ϲ���*/
                    }
                    if (p == NULL)
                    {
                        temp->next[i]->fail = root;/*���û���ҵ�����ô��ʧ��ָ��ָ��root*/
                    }
                }
                q[head++] = temp->next[i];
            }
        }
    }
}
/*�����ؼ�����*/
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
        insert(keyword,root,j);//����keyword�����ֵ���
    }
}

//=======================================
int queryStr(Node *root)
{
    FILE *fp;
    int cnt = 0, index;
    Node *p = root;/*��root��ʼƥ��*/
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
            p = root;/*û���ҵ�ƥ��㣬���root����ʼƥ��*/
        }
        Node *temp = p;
        while (temp != root && temp->count != -1)
        {
            cnt+=temp->count;
            temp->count = -1;
            temp = temp ->fail;
        }//ͳ��ƥ�䵽�Ĺؼ���
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

    build_fail(root);//����ʧ��ָ��
    //Ԥ����ʱ��
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
