#include <gtk/gtk.h>
#include <mysql.h>  
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include "record.h"

using namespace std;
/*
g++  $(mysql_config --cflags)  12-history.cpp $(mysql_config --libs) -o 12-history.o `pkg-config --cflags --libs gtk+-2.0` 
*/

/*sql to record window*/
void record_show_all(GtkWidget *record_clist)
{
   MYSQL *record_show_conn;
   MYSQL_RES *record_show_res;
   MYSQL_ROW record_show_row;
   record_show_conn=mysql_init(NULL);

   //连接test数据库
   if(!mysql_real_connect(record_show_conn,"localhost","root","","myIDS",0,NULL,0)) 
   {
     printf("Connect Error!n");
     exit(1);
   }

   string query_sql="select * from attackStore, ruleBase where attackStore.rule_id = ruleBase.id";

   if( mysql_real_query( record_show_conn , query_sql.c_str() , query_sql.length() ) )
   {
     printf("Query Error!n");
     exit(1);
   }

   record_show_res=mysql_use_result(record_show_conn); 

   while((record_show_row=mysql_fetch_row(record_show_res))!=NULL) 
   {
    // sql  "编号",type "源IP地址" "目的IP地址" 源端口 目的端口 ruleid 攻击时间 id msg content type
    //list  "编号","源IP地址","源端口","目的IP地址","目的端口","报警信息","攻击特征","攻击类型","攻击时间"
    char * str[9];
    str[0]=record_show_row[0];
    str[1]=record_show_row[2];
    str[2]=record_show_row[4];
    str[3]=record_show_row[3];
    str[4]=record_show_row[5];
    str[5]=record_show_row[9];
    str[6]=record_show_row[10];
    if(strcmp(record_show_row[1], "1")==0)
      str[7]="UDP";
    else
      str[7]="TCP";
    string datetime = record_show_row[7];
    string datetime2 =  "\n"+datetime;
    char* c;
   const int len = datetime2.length();
   c = new char[len+1];
   strcpy(c,datetime2.c_str());
    str[8]=c;
    gtk_clist_append(GTK_CLIST(record_clist),str);

  }

   mysql_free_result(record_show_res); //关闭结果集
   mysql_close(record_show_conn); //关闭与数据库的连接
}

/*ui_open_record_window*/
 void record_show()
 {

  GtkWidget *record_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(record_window), "Attack Record");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(record_window), 960, 600);

  gtk_window_set_position(GTK_WINDOW(record_window), GTK_WIN_POS_CENTER);

  GtkWidget *record_vbox;  
        /*创建一个盒装容器并添加到窗口中*/
  record_vbox = gtk_vbox_new(FALSE, 0);

    //---------------manager hbox2-------------------------------------------

  GtkWidget *record_clist;
  GtkWidget *scrolled_window;
//-------------
  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), 
  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
gtk_box_pack_start(GTK_BOX(record_vbox), scrolled_window, TRUE, TRUE, 0);
gtk_widget_show (scrolled_window);
//-------------
  char *manager_clist_title[] = {
    "编号","源IP地址","源端口","目的IP地址","目的端口","报警信息","攻击特征","攻击类型","攻击时间"
  };

  record_clist = gtk_clist_new_with_titles(9,manager_clist_title);

  gtk_clist_set_column_width(GTK_CLIST(record_clist),0,30);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),1,110);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),2,50);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),3,110);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),4,50);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),5,190);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),6,120);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),7,70);
  gtk_clist_set_column_width(GTK_CLIST(record_clist),8,100);

gtk_clist_set_row_height( GTK_CLIST(record_clist), 30 );

gtk_container_add(GTK_CONTAINER(scrolled_window), record_clist);
gtk_widget_show (record_clist);
//----------------------------------------------------------

  gtk_container_add(GTK_CONTAINER(record_window), record_vbox);

record_show_all(record_clist);

  gtk_widget_show_all(record_window);

  
}
