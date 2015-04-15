/***
level arr->avl
g++ -o levelAc.o levelAc.cpp
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
#define queMax 30//½ÚµãÊý
#define keyNum 5*/

#define keyFile "bigKey.txt"
#define stringFile "bigString.txt"
#define LineMax 20
#define queMax  400005//1799536   43533
#define keyNum  20000//20000 40000 60000 80000 100000

#define arrQueMax  60000
#define charSize 256 

#define avlStartLevel 2 //<avlStartLevel Ö®Ç°¶ŒÊÇarray²ã

//========================
struct ACVL
{
    struct ACVL *avlFail;
    struct arrnode *avlToArrFail; //avl->arrayµÄÊ§°ÜÖžÕë
    struct ACVL *avlRoot;
    int count;
    int level;//²ãºÅ,0¿ªÊŒ
    int data;
    int bf;
    struct ACVL *lchild;
    struct ACVL *rchild;
};
typedef struct ACVL ACNODE,*BST;
BST  buildAvl(int index,BST T,BST newnode);
BST  stepAvlNext(BST T,int index);
int  queryAvlExist(BST T,int index);
int input;
BST R = NULL;
//=================

struct arrnode
{
    struct arrnode *fail; //Ê§°ÜÖžÕë
    struct arrnode *next[charSize];//Ò»žöœÚµãÓµÓÐµÄ×ÓœÚµã
    struct ACVL *arrToAvlNext[charSize];//ÖžÏò¶ù×ÓÊÇavlÊ÷œá¹¹
    int count; //ÊÇ·ñÎªžÃµ¥ŽÊ×îºóÒ»žöµã£»1±íÊŸÊÇ×îºóÒ»žöµã£¬0±íÊŸ³õÊŒ»¯£¬£­1±íÊŸÒÑŸ­Í³ŒÆ¹ý
    int level;//²ãºÅ,0¿ªÊŒ
};
typedef struct arrnode arrNode;

arrNode *keyque[arrQueMax];//¹ØŒüŽÊÃ¿ÐÐ50žöŽÊ£¬¹²10000ÐÐ
ACNODE *avlKeyQue[queMax];//avl que
int   *keyword;//¹ØŒüŽÊ
int   head,tail;//¶ÓÁÐµÄÍ·¡¢Î²ÖžÕë
//---------------------
    ACNODE *avlStartTemp,*avltemp;
arrNode *arrtemp;
void printEach(arrNode *nn);
//--------------------
/*²åÈëœÚµãµœ×ÖµäÊ÷ÖÐ*/
void insert(int * str,arrNode *root,int num)
{
    arrNode *arrtemp = root;
    int i = 0, index,j;
    for(i=0; i<num; i++)
    {
        //printf("%c\n",str[i]);//eeceb deace
        index = str[i];
        if(i<avlStartLevel)//Ö®Ç°¶ŒÊÇarray²ã
        {
            if (arrtemp->next[index] == NULL)/*Èç¹û²»ŽæÔÚÄÇžöœÚµã£¬Ê×ÏÈµÃŽŽœšœÚµã*/
            {
                arrNode *newArrNode = (arrNode *)malloc(sizeof(arrNode));
                for (j = 0; j < charSize; j++)
                {
                    newArrNode->next[j] = NULL;
                    newArrNode->arrToAvlNext[j]=NULL;
                }
                newArrNode->count = 0;
                newArrNode->level=i;//²ãºÅž³Öµ
                arrtemp->next[index] = newArrNode;
            }
            arrtemp = arrtemp->next[index];
        }
        //------------------------------------------
         else if(i==avlStartLevel)//ŒÇÂŒavl²ã¿ªÍ·µÚÒ»žö
            {
                if (arrtemp->arrToAvlNext[index] == NULL)//ÏÖÔÚŽŠÓÚ¹ý¶Èœ×¶Î
                {
                    ACNODE * newAvlNode = (ACNODE *)malloc(sizeof(ACNODE));
                    newAvlNode->lchild=NULL;
                    newAvlNode->rchild=NULL;
                    newAvlNode->avlFail=NULL;
                    newAvlNode->avlToArrFail=NULL;
                    newAvlNode->bf=0;
                    newAvlNode->data=index;
                    newAvlNode->count = 0;
                    newAvlNode->level = i;
                    newAvlNode->avlRoot=NULL;
                    arrtemp->arrToAvlNext[index] = newAvlNode;
                }
                avltemp=arrtemp->arrToAvlNext[index];//Ïàµ±ÓÚp=stepAvlNext(p->avlRoot,index);
            }//end if i==avlStartLevel
          else
            {
                if (queryAvlExist(avltemp->avlRoot,index)==0)//avlœ×¶Î
                {
                    ACNODE * newAvlNode2 = (ACNODE *)malloc(sizeof(ACNODE));
                    newAvlNode2->lchild=NULL;
                    newAvlNode2->rchild=NULL;
                    newAvlNode2->avlFail=NULL;
                    newAvlNode2->avlToArrFail=NULL;
                    newAvlNode2->bf=0;
                    newAvlNode2->data=index;
                    newAvlNode2->count = 0;
                    newAvlNode2->level = i;
                    newAvlNode2->avlRoot=NULL;
                    avltemp->avlRoot= buildAvl(index,avltemp->avlRoot,newAvlNode2);
                }
              //  printf("\n ==== %c",i,str[i]);
                avltemp=stepAvlNext(avltemp->avlRoot,index);
            }

    }//end for
    avltemp->count = 1;//IMPORTANT!!±íÊŸŽËœÚµãÎª×îºóÒ»žöŽÊ,ÊÇavlÀàÐÍµÄÖžÕë,ÒòÎª²ãÊý×îºóÊÇavl,±ä³€ºÍlistÐèÒªÁíÍâ¿ŒÂÇ
}

/*Ê§°ÜÖžÕëµÄœšÁ¢*/
void build_fail(arrNode *root)
{
    int i,m;
    root->fail = NULL;/*ÈÃrootµÄÊ§°ÜÖžÕëÖžÏòNULL£¬×÷ÎªÒ»ÖÖÌõŒþÅÐ¶Ï*/
    keyque[head++] = root;//++ Ö®ºóhead=1
    while (head != tail)/*µ±±éÀúÍêËùÓÐµÄœÚµãºó£¬head==tail*/
    {
        if(keyque[tail]->level==(avlStartLevel-1))
            break;//Èç¹ûÓöµœ¹ý¶È²ã-1.ÔòÌø³ö
        arrNode *temp = keyque[tail++];/*Ê×ÏÈÈÃÖžÕëÖžÏòroot£¬keyqueµÄ×÷ÓÃÏàµ±ÓÚ¹ã¶ÈËÑË÷ÀïÃæŒÇÂŒ¶ù×ÓµÄ¶ÓÁÐ*/
        arrNode *p = NULL;
        for (i=0; i < charSize; i++)//±éÀúÒ»žöœÚµãµÄ¶ù×Ó
        {
            if (temp->next[i] != NULL)/*ÕÒµœŽæÔÚµÄœÚµã£¬ÓÃµÄÊÇ±éÀúµÄ·œ·š*/
            {
                if (temp == root)
                {
                    temp->next[i]->fail = root;/*rootËùÓÐÏÂÒ»Œ¶œÚµãµÄÊ§°ÜÖžÕëÈ«²¿ÖžÏòroot*/
                }
                else
                {
                    p = temp->fail;/*ÕÒµœÊ§ÅäµãžžœÚµãµÄÊ§°ÜÖžÕë,ÆäžžœÚµãµÄ×Ö·ûÓëžÃÊ§°ÜÖžÕëÖžÏò×Ö·ûÏàµÈ*/
                    while (p != NULL)
                    {
                        if (p->next[i] != NULL)/*Èç¹ûp->next[i]ÓÐœÚµã£¬ÔÚp->next[i]ŸÍÊÇtemp->next[i]Ê§°ÜÖžÕëÎ»ÖÃ*/
                        {
                            temp->next[i]->fail = p->next[i];
                            break;
                        }
                        p = p->fail;/*Èç¹ûÉÏÒ»žöif²»³ÉÁ¢£¬ÔòŒÌÐøÏòÉÏ²éÕÒ*/
                    }
                    if (p == NULL)
                    {
                        temp->next[i]->fail = root;/*Èç¹ûÃ»ÓÐÕÒµœ£¬ÄÇÃŽœ«Ê§°ÜÖžÕëÖžÏòroot*/
                    }
                }//end else
                keyque[head++] = temp->next[i];//ÓÐ¶ù×Ó,ŸÍhead++,Žæ·Å¶ù×ÓœÚµã£¬ÒÔ±ž¹ã¶ÈËÑË÷Ìø×ª
            }
        }// end for
    }//end while1  for array ÏÖÔÚnum=2²ãµÄfail¶ŒÒÑŸ­œšºÃ
//-------------µÚÈý²ã¿ªÊŒ--------------------------
    //µÚÈý²ãÍøÉÏÕÒžžÇ×µÄÊ±ºòÖ»ÄÜÊÇarrayÀàÐÍ¡£
    arrNode *arrToAvlp = NULL;
    int avlHead=0, avlTail=0;
    for(m=tail; m<head; m++) //µ¥¶ÀœšµÚÈý²ãµÄÊ§°ÜÖžÕë£¬œ«µÚËÄ²ã¶ù×ÓŽæÈëavlKeyQue[];
    {
        for (i=0; i < charSize; i++)//±éÀúÒ»žöœÚµãµÄ¶ù×Ó
        {
            if (keyque[m]->arrToAvlNext[i] != NULL)/*ÕÒµœŽæÔÚµÄœÚµã£¬ÓÃµÄÊÇ±éÀúµÄ·œ·š*/
            {
                arrToAvlp= keyque[m]->fail;
                while (arrToAvlp != NULL)
                {
                    if (arrToAvlp->next[i] != NULL)
                    {
                        keyque[m]->arrToAvlNext[i]->avlToArrFail = arrToAvlp->next[i];
                        break;
                    }
                    arrToAvlp = arrToAvlp->fail;
                }
                if (arrToAvlp == NULL)
                {
                    keyque[m]->arrToAvlNext[i]->avlToArrFail = root;
                }
                avlKeyQue[avlHead++] = keyque[m]->arrToAvlNext[i];//œ«µÚ3²ãŽæÈëavlKeyQue
            }
        }
    }
    //printf("  ***********************   %d\n", head);
    /*  for(m=0;m<5;m++){
     printf("\n *%c level:%d",avlKeyQue[m]->data,avlKeyQue[m]->avlToArrFail->level);
      }*/
//-------------µÚÈý²ãover--------------------------

//-------------ºóÐøavl²ã¿ªÊŒ--------------------------

    while (avlHead != avlTail)/*µ±±éÀúÍêËùÓÐµÄœÚµãºó£¬head==tail*/
    {
        ACNODE *avltemp = avlKeyQue[avlTail++];/*Ê×ÏÈÈÃÖžÕëÖžÏòroot£¬keyqueµÄ×÷ÓÃÏàµ±ÓÚ¹ã¶ÈËÑË÷ÀïÃæŒÇÂŒ¶ù×ÓµÄ¶ÓÁÐ*/
        ACNODE *avlp = NULL;
        arrNode *arrp = NULL;
        for (i=0; i < charSize; i++)//±éÀúÒ»žöœÚµãµÄ¶ù×Ó
        {
            if(queryAvlExist(avltemp->avlRoot,i)==1)/*ÕÒµœŽæÔÚµÄœÚµã£¬ÓÃµÄÊÇ±éÀúµÄ·œ·š*/
            {
                avlp=avltemp->avlFail;//·Ö2ÖÖÇé¿ö,žžÇ×ÊÇarrayºÍavl£¬avltempµÄfail±Ø¶šÒÑŸ­œšÁ¢
                arrp=avltemp->avlToArrFail;
                if(avlp!=NULL) //con1.žžÇ×œÚµãµÄfailÖžÏòavl
                {
                    while (avlp != NULL)
                    {
                        if (queryAvlExist(avlp->avlRoot,i)==1)/*Èç¹ûp->next[i]ÓÐœÚµã£¬ÔÚp->next[i]ŸÍÊÇtemp->next[i]Ê§°ÜÖžÕëÎ»ÖÃ*/
                        {
                            //avltemp->next[i]->fail = avlp->next[i];
                            stepAvlNext(avltemp->avlRoot,i)->avlFail=stepAvlNext(avlp->avlRoot,i);
                            break;
                        }
                        if(avlp->level==avlStartLevel)//
                        {
                            arrp = avlp->avlToArrFail;/*Èç¹ûÉÏÒ»žöif²»³ÉÁ¢£¬ÔòŒÌÐøÏòÉÏ²éÕÒ*/
                            break;
                        }
                        else if(avlp->level>avlStartLevel)
                        {
                            avlp = avlp->avlFail;
                        }
                    }
                    if (avlp == NULL)
                    {
                        stepAvlNext(avltemp->avlRoot,i)->avlToArrFail = root;
                    }
                }//end con1
                if(arrp!=NULL) //con2.žžÇ×œÚµãµÄfailÖžÏòarray
                {
                    while (arrp != NULL)
                    {
                        if(arrp->level<(avlStartLevel-1)) //Ž¿array
                        {
                            if (arrp->next[i] != NULL)
                            {
                                stepAvlNext(avltemp->avlRoot,i)->avlToArrFail = arrp->next[i];
                                break;
                            }
                            arrp = arrp->fail;
                        }

                        else                             //×ÔŒºÊÇarray, ¶ù×ÓÊÇavl
                        {
                            if (arrp->arrToAvlNext[i] != NULL)
                            {
                                stepAvlNext(avltemp->avlRoot,i)->avlFail = arrp->arrToAvlNext[i];//failÖžÏòarrp¶ù×Ó,±Ø¶šÊÇavl
                                break;
                            }
                            arrp = arrp->fail;
                        }
                    }
                    if (arrp == NULL)
                    {
                        stepAvlNext(avltemp->avlRoot,i)->avlToArrFail = root;/*Èç¹ûÃ»ÓÐÕÒµœ£¬ÄÇÃŽœ«Ê§°ÜÖžÕëÖžÏòroot*/
                    }
                }//end con2

                avlKeyQue[avlHead++] = stepAvlNext(avltemp->avlRoot,i);//ÓÐ¶ù×Ó,ŸÍhead++,Žæ·Å¶ù×ÓœÚµã£¬ÒÔ±ž¹ã¶ÈËÑË÷Ìø×ª
            }
        }// end for
    }//end while
   // printf("  ***********************   %d\n", avlHead);

}
/*œšÁ¢¹ØŒü×ÖÊ÷*/
void build_keyword(arrNode *root)
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
        insert(keyword,root,j);//žùŸÝkeywordœšÁ¢×ÖµäÊ÷
    }
}

//=======================================
int queryStr(arrNode *root)
{
    FILE *fp;
    int cnt = 0, index;
    int avlSwitch=0, u=0;//avlSwitch 0¿ªÊŒŽÓarrayœÚµã¿ªÊŒÕÒ£¬1ŽÓavlœÚµã¿ªÊŒÕÒ
    arrNode *arrQueryP = root;/*ŽÓroot¿ªÊŒÆ¥Åä*/
    ACNODE  *avlQueryP = NULL;
    if ((fp=fopen(stringFile, "r"))==NULL)
    {
        printf("open string.txt file error!!\n");
        return 0;
    }
    while(!feof(fp))
    {
        index=fgetc(fp);
        while(1)
        {
            if(avlSwitch==0)
            {
                if(arrQueryP->level<(avlStartLevel-1))//1.Ž¿array
                {
                    while(arrQueryP->next[index]==NULL&&arrQueryP!=root)
                    {
                        arrQueryP = arrQueryP->fail;
                    }
                    arrQueryP = arrQueryP->next[index];//ÏÖÔÚ¿ÉÄÜµœŽïÁË¹ý¶É²ã£¬œøÐÐÏÂÒ»žöif
                    avlSwitch=0;
                    if (arrQueryP == NULL)
                    {
                        arrQueryP = root;
                    }
                    break;
                }
//------------------------------------------
                else if(arrQueryP->level==(avlStartLevel-1)) //2.¹ý¶É²ã
                {
                    if(arrQueryP->arrToAvlNext[index]==NULL)
                    {
                        arrQueryP = arrQueryP->fail;
                        while(arrQueryP->next[index]==NULL&&arrQueryP!=root)
                        {
                            arrQueryP = arrQueryP->fail;
                        }//ŒÌÐøµœÉÏ²ãarrayÕÒfail
                         arrQueryP = arrQueryP->next[index];
                    }
                    else
                    {
                        avlQueryP=arrQueryP->arrToAvlNext[index];
                        avlSwitch=1;
                    }
                    if (arrQueryP == NULL)
                    {
                        arrQueryP = root;
                        avlSwitch=0;
                    }
                    break;
                }
            }//switch = 0 end
//------------------------------------------
            else if (avlSwitch==1)
            {
                //if (avlQueryP->level == 3)
                  //  printf("\n 3: %c",avlQueryP->data);
                while(queryAvlExist(avlQueryP->avlRoot,index)!=1)
                {      //printf("eee");//ËÀÑ­»·
                    if(avlQueryP->avlFail!=NULL) //avlµÄfailÖžÏòavl
                    {
                        avlQueryP=avlQueryP->avlFail;
                    }
                    else if(avlQueryP->avlToArrFail!=NULL) //avlµÄfailÖžÏòarray
                    {
                        arrQueryP=avlQueryP->avlToArrFail;
                        avlSwitch=0;
                        break;
                    }
                }
                if(queryAvlExist(avlQueryP->avlRoot,index)==1) //ÊôÓÚÕý³£Ìø³ö£¬ÕÒµœÁËavlµÄnext
                {
                    //if (avlQueryP->level == 3)
                    //printf(" && %c && ",avlQueryP->data);
                    avlQueryP=stepAvlNext(avlQueryP->avlRoot,index);
                    //if (avlQueryP->level == 4)
                    //printf(" ++ %c ++ ",avlQueryP->data);
                    avlSwitch=1;
                    break;
                }
                if (avlQueryP == NULL)
                {
                    arrQueryP = root;
                    avlSwitch=0;
                    break;
                }

            }//end  avlSwitch==1
        }//end while
//----------------------------
        if (avlSwitch==1)//Èç¹ûÊÇarray²ãÓÐœáÊø×Ö·û£¬ÐèÒªœøÒ»²œžÄœø
        {
            ACNODE *avlCntTemp = avlQueryP;
            arrNode *arrCntTemp2 =NULL;
           while (avlCntTemp->count != -1)
            {
                cnt+=avlCntTemp->count;
                avlCntTemp->count = -1;
                if(avlCntTemp->avlFail!=NULL)
                    avlCntTemp = avlCntTemp ->avlFail;
                else if (avlCntTemp->avlToArrFail!=NULL)
                {
                    arrCntTemp2=avlCntTemp->avlToArrFail;
                   // break;
                }//end while
                while (arrCntTemp2 != NULL && arrCntTemp2 != root && arrCntTemp2->count != -1)
                {
                    cnt+=arrCntTemp2->count;
                    arrCntTemp2->count = -1;
                    arrCntTemp2 = arrCntTemp2 ->fail;
                }//ŒÆÆ¥ÅäµœµÄ¹ØŒü×Ö
            }
        }
    }
    return cnt;
}
void printEach(arrNode *nn)
{
    int j;
    for (j = 0; j < charSize; j++)
        if(nn->next[j]!=NULL)
            printf("%c",j);
}
void printEachArrToAvl(arrNode *nn)
{
    int j;
    for (j = 0; j < charSize; j++)
        if(nn->arrToAvlNext[j]!=NULL)
            printf("%c",j);
}
void printAvlFail(ACNODE *avln)
{
    int j;
    if(avln->avlFail!=NULL)
        printf("%d",avln->avlFail->level);
    if(avln->avlToArrFail!=NULL)
    {
        printf("%d",avln->avlToArrFail->level);
    }
}

//=======================================
int main()
{
    avlStartTemp = (ACNODE *)malloc(sizeof(ACNODE));
    arrtemp = (arrNode *)malloc(sizeof(arrNode));
    avltemp = (ACNODE *)malloc(sizeof(ACNODE));
    //========
    printf("=================levelAc start==================\n");
    clock_t start,finish,middle;
    double exeTime,preTime;
    start = clock();
    keyword=(int*)malloc(LineMax*sizeof(int));
    int  i;
    head = tail = 0;
    arrNode *root = (arrNode *)malloc(sizeof(arrNode));
    for (i = 0; i < charSize; i++)
        root->next[i] = NULL;
    root->count = 0;
    root->level=-1;

    build_keyword(root);

    build_fail(root);//œšÁ¢Ê§°ÜÖžÕë
    //showTrie(root);
    //Ô€ŽŠÀíÊ±Œä
    middle = clock();
    preTime= (double)(middle - start) / CLOCKS_PER_SEC;
    printf("               preTime :%f\n",preTime);
    printf("                 arrToAvl %d\n",queryStr(root));
    finish = clock();
    exeTime = (double)(finish - middle) / CLOCKS_PER_SEC;
    printf("               exeTime :%f\n",exeTime);
    printf("=================levelAc end==================\n");
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
