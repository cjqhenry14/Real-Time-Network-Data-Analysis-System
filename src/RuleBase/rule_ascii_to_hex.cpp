#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>  
#include <mysql.h>  
using namespace std;
/*
g++ -o rule_ascii_to_hex.o $(mysql_config --cflags) rule_ascii_to_hex.cpp $(mysql_config --libs)  
*/

#define MAX_LINE 2048

string string_to_hex(char * str, int len)
{
   string hex_content;

   int i;
   int leftNum= 0;
   for(i=0;i<len;i++)
   {
      if(str[i]=='|')// meet |
      {

         if(leftNum%2==0)
         {
            char *mm =&str[i+1];
            while(*mm!='|')
            {
               if(*mm==' ')
               {
                  mm++;
                  i++;
               }
               hex_content.push_back(*mm);
               mm++;
               i++;
               hex_content.push_back(*mm);
               mm++;
               i++;
            }
         }

         leftNum++;
      }

      else
      {
        char val[2] = {0};
        sprintf(val, "%2X", str[i]); //小写则为%x
        hex_content.append(string(val));
     }

  }

  return hex_content;
}

void rules_ascii_to_hex()
{
   MYSQL *my_conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   my_conn=mysql_init(NULL);

   if(!mysql_real_connect(my_conn,"localhost","root","","myIDS",0,NULL,0)) //连接test数据库
   {
      printf("Connect Error!n");
      exit(1);
   }

  // if(mysql_query(my_conn,"select * from ruleBase limit 0,20")) //top 20
  if(mysql_query(my_conn,"select * from ruleBase where id = 630 or id = 2081")) //all   630 2081
  {
   printf("Query Error!n");
   exit(1);
  }

  // res=mysql_use_result(my_conn); //取得表中的数据并存储到res中
/*
2|00 00 00 06 00 00 00|Drives|24 00|
327c30302030302030302030362030302030302030307c4472697665737c32342030307c
qazwsx.hsq 
71617a7773782e687371
*/
    //UPDATE `ruleBase` SET `hex_content`= "hehe" WHERE id= 1
   do
   {
        res = mysql_store_result(my_conn);//获得多个结果
        
      while((row=mysql_fetch_row(res))!=NULL) //打印结果
      {
         int len =  strlen(row[2]);//length of content
         string hex_content= string_to_hex(row[2], len);

        // cout<<hex_content<<endl;

         string insert_sql ="UPDATE ruleBase set hex_content='"+ hex_content +"' where id ='"+row[0]+"';";

         int insertRes =mysql_real_query( my_conn , insert_sql.c_str() , insert_sql.length() ); 
         if (!insertRes)
         {
           //printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(my_conn));
         } 

         else 
         {
           fprintf(stderr, "Insert error %d: %s\n", mysql_errno(my_conn),
           mysql_error(my_conn));
         }

    }
        mysql_free_result(res);//释放一个结果

    }while (!mysql_next_result(my_conn));//整个程序结束后所有的结果集中的数据被释放

   mysql_close(my_conn); //关闭与数据库的连接

}


int main()
{

   rules_ascii_to_hex();
   return 0;
}