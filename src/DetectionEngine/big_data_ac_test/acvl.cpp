/***
acvl
g++ -o acvl.o acvl.cpp
***/

#include    <stdio.h>
#include    <malloc.h>
#include    <stdlib.h>
#include    <time.h>
#include    <unistd.h>
/*#define keyFile "bigkey.txt"
#define stringFile "string.txt"
#define LineMax 6
#define queMax 30//½ÚµãÊý
#define keyNum 5*/

#define keyFile "bigKey.txt"
#define stringFile "bigString.txt"
#define LineMax 20
#define queMax 2000005//
#define keyNum 100000//20000 40000 60000 80000 100000
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

ACNODE *q[queMax];
ACNODE *root;
int   *keyword;
int    head, tail;
BST  buildAvl(int index,BST T,BST newnode);
BST  stepAvlNext(BST T,int index);
int  queryAvlExist(BST T,int index);

int input;
BST R = NULL;
void insertACVL(int * str,ACNODE *root,int num)
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
    p->count = 1;
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

void build_keyword(ACNODE *root)
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
        insertACVL(keyword,root,j);
    }
}

int queryStr(ACNODE *root)
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
            cnt+=temp->count;
            temp->count = -1;
            temp = temp ->fail;
        }
    }
    return cnt;
}

int main()
{
    printf("================acvl start==================\n");
    clock_t start,finish,middle;
    double exeTime,preTime;
    start = clock();
    keyword=(int*)malloc(LineMax*sizeof(int));
    head = tail = 0;
    root = (ACNODE *)malloc(sizeof(ACNODE));
    root->avlRoot=NULL;
    root->data=-2;
    root->count = 0;
    build_keyword(root);
    build_fail(root);
    root->fail=root;
    middle = clock();
    preTime= (double)(middle - start) / CLOCKS_PER_SEC;
    printf("               preTime :%f\n",preTime);
     printf("                 ACVL %d\n",queryStr(root));
    finish = clock();
    exeTime = (double)(finish - middle) / CLOCKS_PER_SEC;
    printf("               exeTime :%f\n",exeTime);
    printf("============acvl end==================\n");
    //sleep(3);
    return 0;
}



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
