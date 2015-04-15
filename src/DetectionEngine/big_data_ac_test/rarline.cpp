/***
rarline
g++ -o rarline.o rarline.cpp
***/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

/*#define keyFile "bigkey.txt"
#define stringFile "string.txt"
#define LineMax 6
#define queMax 30//½ÚµãÊý
#define keyNum 5*/

#define keyFile "bigKey.txt"
#define stringFile "bigString.txt"
#define LineMax 20
#define queMax  2000005//
#define keyNum  100000//20000 40000 60000 80000 100000
#define charSize 256

int ** map;
int fail[queMax];
int que[queMax];
int cnt[queMax];
int idx,ff,m;
int w,r,t,i,p;
int *keyword;

void init()
{
    map = (int **) malloc(sizeof(int *)*queMax);
    for(i=0; i<=queMax; i++)
    {
        map[i]=(int *) malloc(sizeof(int));
        map[i][0]=0;
    }
    fail[0]=0;
    idx=1;
}
bool isInMap(int i ,int j)
{
    if(0==map[i][0])
        return 0;
    for(w=1; w<=map[i][0]; w++)
        if(j==map[i][w])
            return 1;
    return 0;
}
int getMapNext(int i,int j)
{
    t=map[i][0];
    if(t==0)
        return 0;
    for(w=1; w<=t; w++)
        if(j==map[i][w])
            return map[i][w+t];

    return 0;
}
void insert(int *s,int val)
{
    int p=0,c,m;
    for(; *s; s++)
    {
        c=*s;
        if(isInMap(p,c)==0)
        {
            cnt[idx]=0;
            map[p]=(int *)realloc(map[p],sizeof(int )*(2*(map[p][0]+1)+1));
            for(m=2*map[p][0]+1; m>map[p][0]+1; m--)
                map[p][m]=map[p][m-1];
            map[p][map[p][0]+1]=c;
            map[p][0]=map[p][0]+1;
            t=2*map[p][0];
            map[p][t]=idx;
            idx++;
        }
        p=getMapNext(p,c);
    }
    cnt[p]+=val;
}
void AC()
{
    int *s=que,*e=que,l;
    int i;
    for(i=0; i<charSize; i++)
        if(isInMap(0,i))
        {
            r=getMapNext(0,i);
            fail[r]=0;
            *e++=r;
        }
    while(s!=e)
    {
        p=*s++;
        for(i=0; i<charSize; i++)
            if(isInMap(p,i))
            {

                r=getMapNext(p,i);
                *e++=r;
                fail[r]=getMapNext(fail[p],i);
            }
    }
}
int queryStr()
{
    FILE *fp;
    int ans = 0, c,pp;
    if ((fp=fopen(stringFile, "r"))==NULL)
    {
        printf("open string.txt file error!!\n");
        return 0;
    }
    while(!feof(fp))
    {
        c=fgetc(fp);

        while(p!=0&&isInMap(p,c)==0)
            p=fail[p];
        p=getMapNext(p,c);

        pp=p;
        while(pp && cnt[pp]!=-1)
        {
            ans+=cnt[pp];
            cnt[pp]=-1;
            pp=fail[pp];
        }
    }
    return ans;
}
void build_keyword()
{
    FILE *fp;
    int i=0,jmax,j=0;
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
        insert(keyword,1);
    }
}

void countRarOut(){
    int c=0;
    for(i=0; i<=queMax; i++)
    {

       c+= map[i][0];
    }
    printf(" RarOut: %d  \n",c);
}
void showMap()
{
    int i=0,j=0;
    for(i=0; i<9; i++)
    {
        for(j=0; j<8; j++)
            printf("  %d  ",map[i][j]);
        printf("\n");
    }
}
int maxLine()
{
    int y=0;
    for(r=0; r<=queMax; r++)
    {
        if(map[r][0]!=0)
            y=r;
    }
    return y;
}
int main()
{
    printf("=================rarline start==================\n");
    clock_t start,finish,middle;
    double exeTime,preTime;
    start = clock();
    keyword=(int*)malloc(LineMax*sizeof(int));
    init();
    build_keyword();
    AC();
    middle = clock();
    preTime= (double)(middle - start) / CLOCKS_PER_SEC;
    printf("               preTime :%f\n",preTime);
    printf("          rarline    %d\n",queryStr());
    finish = clock();
    exeTime = (double)(finish - middle) / CLOCKS_PER_SEC;
    printf("               exeTime :%f\n",exeTime);
    free(map);
    printf("=================rarline end==================\n");
    return 0;
}
