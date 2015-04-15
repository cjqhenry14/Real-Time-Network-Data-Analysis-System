#include <gtk/gtk.h>
#include <mysql.h>  
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include "rulebase.h"

using namespace std;
/*
g++  $(mysql_config --cflags)  11-rulewindow.cpp $(mysql_config --libs) -o 11-rulewindow.o `pkg-config --cflags --libs gtk+-2.0` 
*/
/*manager open function; open manager_window*/

GtkWidget *rules_window;
GtkWidget *rules_clist;

GtkWidget *rules_look_window;
GtkWidget *rules_look_id_entry;

GtkWidget *rules_delete_window;
GtkWidget *rules_delete_id_entry;

GtkWidget *rules_add_window;
GtkWidget *add_type_entry;
GtkWidget *add_content_entry;
GtkWidget *add_msg_entry;
//gtk_clist_clear( GTK_CLIST(rules_clist) );

GtkWidget *rules_modify_window;
GtkWidget *modify_rule_id_entry;
GtkWidget *modify_type_entry;
GtkWidget *modify_content_entry;
GtkWidget *modify_msg_entry;
string rule_modify_id;

//********************   rules_modify_window   *******************************************
/*query modify and show in the entry*/
void rules_modify_query_sql()
{
  if(strlen(gtk_entry_get_text(GTK_ENTRY(modify_rule_id_entry)))<=10)
  {
     char ch_rule_id[10];

     strcpy(ch_rule_id, gtk_entry_get_text(GTK_ENTRY(modify_rule_id_entry)) );

      rule_modify_id = ch_rule_id;

     if(rule_modify_id=="" || rule_modify_id==" ")
     {
       rules_modify_id_null_warning();
     }
    
    else
    {

    MYSQL *rules_show_conn;
    MYSQL_RES *rules_show_res;
    MYSQL_ROW rules_show_row;

    rules_show_conn=mysql_init(NULL);

     //连接test数据库
   if(!mysql_real_connect(rules_show_conn,"localhost","root","","myIDS",0,NULL,0)) 
   {
     printf("Connect Error!n");
     exit(1);
   }
   
   string query_sql="select * from ruleBase where id ='"+ rule_modify_id+"';";

   if( mysql_real_query( rules_show_conn , query_sql.c_str() , query_sql.length() ) )
   {
     printf("Query Error!n");
     exit(1);
   }

   rules_show_res=mysql_use_result(rules_show_conn);

   bool isGet=false;

   while((rules_show_row=mysql_fetch_row(rules_show_res))!=NULL) //打印结果
   {
    isGet=true;

    gtk_entry_set_text(GTK_ENTRY(modify_type_entry),rules_show_row[4]);
    gtk_entry_set_text(GTK_ENTRY(modify_content_entry),rules_show_row[2]);
    gtk_entry_set_text(GTK_ENTRY(modify_msg_entry),rules_show_row[1]);
    // str[1]=rules_show_row[1];//msg
    // str[2]=rules_show_row[2];//content
    // str[4]=rules_show_row[4];//type
   }

   if(! isGet)//no such id
   {
     rules_modify_not_exist_warning();
   }

   mysql_free_result(rules_show_res); //关闭结果集
   mysql_close(rules_show_conn); //关闭与数据库的连接

  }//end else

 }//end check length <=10
 else
 {
   rules_modify_id_length_warning();
 }

}

/*update rulebase*/
void rules_modify_sql()
{
   int type_len=strlen(gtk_entry_get_text(GTK_ENTRY(modify_type_entry) )  );
   int content_len=strlen(gtk_entry_get_text(GTK_ENTRY(modify_content_entry) )  );
   int msg_len=strlen(gtk_entry_get_text(GTK_ENTRY(modify_msg_entry) )  );

   if( type_len == 0 || type_len == 0 || msg_len == 0 )
   {
     rules_modify_entry_null_warning();
   }

   else
   {
         string rule_type;
         string rule_content;
         string rule_msg;

         char * ch_rule_type = (char*)malloc(type_len*sizeof(char));
         char * ch_rule_content = (char*)malloc(content_len*sizeof(char));
         char * ch_rule_msg = (char*)malloc(msg_len*sizeof(char));

        strcpy(ch_rule_content, gtk_entry_get_text(GTK_ENTRY(modify_content_entry)) );
        strcpy(ch_rule_type, gtk_entry_get_text(GTK_ENTRY(modify_type_entry)) );
        strcpy(ch_rule_msg, gtk_entry_get_text(GTK_ENTRY(modify_msg_entry)) );

        string hex_content = rules_string_to_hex(ch_rule_content, content_len);

        rule_msg = ch_rule_msg;
        rule_content = ch_rule_content;
        rule_type = ch_rule_type;

         MYSQL *rules_update_conn;

         rules_update_conn=mysql_init(NULL);

         int res=0;
          //连接test数据库
         if(!mysql_real_connect(rules_update_conn,"localhost","root","","myIDS",0,NULL,0)) 
         {
             printf("Connect Error!n");
             exit(1);
         } 
        //  //rule_modify_id

         string update_sql ="UPDATE ruleBase set msg = '"+ rule_msg + 
         "' , content = '"+rule_content+ "'  , hex_content = '"+hex_content+ "'  , type = '"+rule_type+ 
         "' where id = '"+rule_modify_id+"' ";
        // cout<< update_sql <<endl;
         //string update_sql= "UPDATE ruleBase set msg = 'MALWARE-CNC Win.Worm.Neeris IRCbot outbound connection222' , content = '33D7BFA522'  , hex_content = '33334437424641353232'  , type = 'trojan-activity2' where id = '3001';";
          // string update_sql= "UPDATE ruleBase set content = '33D7BFA5'  , hex_content = '33334437424641353232' where id = '3001' ";

         res =mysql_real_query( rules_update_conn , update_sql.c_str() , update_sql.length() ); 

        if (!res) 
        {
          printf("UPDATE %lu rows\n", (unsigned long)mysql_affected_rows(rules_update_conn));
          /*change ok*/
          rules_modify_ok_info();
        }

        else 
        {
          rules_modify_fail_info();
          fprintf(stderr, "UPDATE error %d: %s\n", mysql_errno(rules_update_conn),mysql_error(rules_update_conn));
        }

         mysql_close(rules_update_conn); //关闭与数据库的连接

  }//end else entry null
}

/*rules_close_modify_window */
void rules_close_modify_window()
{
    gtk_widget_destroy(rules_modify_window);
}

/*rules_open_modify_window*/
void rules_open_modify_window()
{

  gtk_clist_clear( GTK_CLIST(rules_clist) );

  rules_modify_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(rules_modify_window), "Modify Rule");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(rules_modify_window), 300, 260);

  gtk_window_set_position(GTK_WINDOW(rules_modify_window), GTK_WIN_POS_CENTER);

  GtkWidget *create_frame;
  create_frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(rules_modify_window), create_frame);

  //---------------- query ---------------------------------------

  GtkWidget *rule_id_label;
  rule_id_label = gtk_label_new("规则号:");
  gtk_fixed_put(GTK_FIXED(create_frame), rule_id_label, 40, 20);

      //GtkWidget *modify_rule_id_entry;
  modify_rule_id_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), modify_rule_id_entry, 110, 20);
  gtk_widget_set_size_request(modify_rule_id_entry, 70, 30);

    GtkWidget *look_yes_btn;
  look_yes_btn = gtk_button_new_with_label("查询");
  gtk_widget_set_size_request(look_yes_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(create_frame), look_yes_btn, 190, 20);
  g_signal_connect(look_yes_btn, "clicked", G_CALLBACK (rules_modify_query_sql), NULL);


    //--------------   input ----------------------------------------------
  GtkWidget *type_label;
  type_label = gtk_label_new("类型:");
  gtk_fixed_put(GTK_FIXED(create_frame), type_label, 40, 80);

  GtkWidget *content_label;
  content_label = gtk_label_new("内容:");
  gtk_fixed_put(GTK_FIXED(create_frame), content_label, 40, 120);

  GtkWidget *msg_label;
  msg_label = gtk_label_new("报警信息:");
  gtk_fixed_put(GTK_FIXED(create_frame), msg_label, 40, 160);

      //GtkWidget *modify_type_entry;
  modify_type_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), modify_type_entry, 110, 80);

     //GtkWidget *modify_content_entry;
  modify_content_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), modify_content_entry, 110, 120);

     //GtkWidget *modify_msg_entry;
  modify_msg_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), modify_msg_entry, 110, 160);

    //--------------  btn  ----------------------------------------------
  GtkWidget *create_yes_btn;
  create_yes_btn = gtk_button_new_with_label("确定");
  gtk_widget_set_size_request(create_yes_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(create_frame), create_yes_btn, 50, 200);
  g_signal_connect(create_yes_btn, "clicked", G_CALLBACK (rules_modify_sql), NULL);

  GtkWidget *create_no_btn;
  create_no_btn = gtk_button_new_with_label("取消");
  gtk_widget_set_size_request(create_no_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(create_frame), create_no_btn, 180, 200);
  g_signal_connect(create_no_btn, "clicked", 
   G_CALLBACK (rules_close_modify_window), NULL);

    //------------------------------------------------------------
  gtk_widget_show_all(rules_modify_window);

}




//********************   rules_add_window   *******************************************

bool rules_add_check()
{
  if(strcmp(gtk_entry_get_text(GTK_ENTRY(add_type_entry)), "") == 0)
  {
    rules_no_type_warning();
    return false;
  }
  if(strcmp(gtk_entry_get_text(GTK_ENTRY(add_content_entry)), "") == 0)
  {
    rules_no_content_warning();
    return false;
  }
  if(strcmp(gtk_entry_get_text(GTK_ENTRY(add_msg_entry)), "") == 0)
  {
    rules_no_msg_warning();
    return false;
  }

  return true;
}

/*rules_add_sql*/
void rules_add_sql()
{
    bool isCheck=false;
    isCheck=rules_add_check();

    if(isCheck)// check ok ; start insert
    {
       // rules_add_ok_info();
       MYSQL *rules_insert_conn;

      rules_insert_conn=mysql_init(NULL);

      int res=0;

      //连接test数据库
     if(!mysql_real_connect(rules_insert_conn,"localhost","root","","myIDS",0,NULL,0)) 
     {
        printf("Connect Error!n");
        exit(1);
     }

         string rule_type;
         string rule_content;
         string rule_msg;

         int content_len=strlen(gtk_entry_get_text(GTK_ENTRY(add_content_entry)));
         printf("    content_len:      %d \n", content_len);
         int type_len=strlen(gtk_entry_get_text(GTK_ENTRY(add_type_entry)));
         int msg_len=strlen(gtk_entry_get_text(GTK_ENTRY(add_msg_entry)));

         char * ch_rule_type = (char*)malloc(type_len*sizeof(char));
         char * ch_rule_content = (char*)malloc(content_len*sizeof(char));
       //  char * ch_hex_rule_content = (char*)malloc(2*content_len*sizeof(char));
         char * ch_rule_msg = (char*)malloc(msg_len*sizeof(char));

        strcpy(ch_rule_content, gtk_entry_get_text(GTK_ENTRY(add_content_entry)) );
        strcpy(ch_rule_type, gtk_entry_get_text(GTK_ENTRY(add_type_entry)) );
        strcpy(ch_rule_msg, gtk_entry_get_text(GTK_ENTRY(add_msg_entry)) );

         string hex_content = rules_string_to_hex(ch_rule_content, content_len);
         char * ch_hex_rule_content = (char*)malloc(hex_content.length()*sizeof(char));
         memcpy(ch_hex_rule_content, hex_content.c_str() , hex_content.length());

        rule_msg = ch_rule_msg;
        rule_content = ch_rule_content;
        rule_type = ch_rule_type;
 
      string insert_sql ="INSERT INTO ruleBase VALUES( NULL, '"+ 
        rule_msg +"', '" + rule_content + "', '" + hex_content+"', '" + rule_type+"') ";
  
      res =mysql_real_query( rules_insert_conn , insert_sql.c_str() , insert_sql.length() ); 

      if (!res) 
      {
      printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(rules_insert_conn));
      /*create ok*/
      rules_add_ok_info();
      int new_id = mysql_insert_id(rules_insert_conn);
      char ch_new_id[10];
      sprintf(ch_new_id,"%d",new_id);

      char * str[5];
      str[0]=ch_new_id;
      str[1]=ch_rule_msg;
      str[2]=ch_rule_content;
      str[3]=ch_hex_rule_content;
      str[4]=ch_rule_type;
      gtk_clist_append(GTK_CLIST(rules_clist),str);

      } 

      else 
      {
        rules_add_fail_info();
        fprintf(stderr, "Insert error %d: %s\n", 
          mysql_errno(rules_insert_conn),mysql_error(rules_insert_conn));
      }

      mysql_close(rules_insert_conn); //关闭与数据库的连接

    }// end if(isCheck)
}

/*rules_close_add_window */
void rules_close_add_window()
{
    gtk_widget_destroy(rules_add_window);
}

/*rules_open_delete_window*/
void rules_open_add_window()
{

  gtk_clist_clear( GTK_CLIST(rules_clist) );

  rules_add_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(rules_add_window), "Add Rule");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(rules_add_window), 300, 190);

  gtk_window_set_position(GTK_WINDOW(rules_add_window), GTK_WIN_POS_CENTER);

  GtkWidget *create_frame;
  create_frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(rules_add_window), create_frame);

    //--------------   input ----------------------------------------------
  GtkWidget *type_label;
  type_label = gtk_label_new("类型:");
  gtk_fixed_put(GTK_FIXED(create_frame), type_label, 40, 20);

  GtkWidget *content_label;
  content_label = gtk_label_new("内容:");
  gtk_fixed_put(GTK_FIXED(create_frame), content_label, 40, 60);

  GtkWidget *msg_label;
  msg_label = gtk_label_new("报警信息:");
  gtk_fixed_put(GTK_FIXED(create_frame), msg_label, 40, 100);

      //GtkWidget *add_type_entry;
  add_type_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), add_type_entry, 110, 20);

     //GtkWidget *add_content_entry;
  add_content_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), add_content_entry, 110, 60);

     //GtkWidget *add_msg_entry;
  add_msg_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), add_msg_entry, 110, 100);

    //--------------  btn  ----------------------------------------------
  GtkWidget *create_yes_btn;
  create_yes_btn = gtk_button_new_with_label("新建");
  gtk_widget_set_size_request(create_yes_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(create_frame), create_yes_btn, 50, 150);
  g_signal_connect(create_yes_btn, "clicked", G_CALLBACK (rules_add_sql), NULL);

  GtkWidget *create_no_btn;
  create_no_btn = gtk_button_new_with_label("取消");
  gtk_widget_set_size_request(create_no_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(create_frame), create_no_btn, 180, 150);
  g_signal_connect(create_no_btn, "clicked", 
   G_CALLBACK (rules_close_add_window), NULL);

    //------------------------------------------------------------
  gtk_widget_show_all(rules_add_window);

}


//********************   rules_delete_window   *******************************************


/*rules_delete_sql*/
void rules_delete_sql()
{
     char ch_rule_id[10];

     strcpy(ch_rule_id, gtk_entry_get_text(GTK_ENTRY(rules_delete_id_entry)) );

     string rule_id = ch_rule_id;

     if(rule_id=="" || rule_id==" ")
     {
       rules_look_id_null_warning();
     }
    
    else
    {

    MYSQL *rules_show_conn;
    MYSQL_RES *rules_show_res;
    MYSQL_ROW rules_show_row;

    rules_show_conn=mysql_init(NULL);

     //连接test数据库
   if(!mysql_real_connect(rules_show_conn,"localhost","root","","myIDS",0,NULL,0)) 
   {
     printf("Connect Error!n");
     exit(1);
   }
   
   string query_sql="select * from ruleBase where id ='"+ rule_id+"';";

   if( mysql_real_query( rules_show_conn , query_sql.c_str() , query_sql.length() ) )
   {
     printf("Query Error!n");
     exit(1);
   }

   rules_show_res=mysql_use_result(rules_show_conn);

   bool isExist=false;

   while((rules_show_row=mysql_fetch_row(rules_show_res))!=NULL) //打印结果
   {
    isExist=true;
   }

   mysql_free_result(rules_show_res); //关闭结果集

   //--------------- query over ---------------------

     if(! isExist)//no such id
     {
       rules_look_not_exist_warning();
     }

     else//id is exist, start delete
     {
         string delete_sql ="DELETE from ruleBase where id ='"+rule_id+"';";
  
         int res =mysql_real_query( rules_show_conn , delete_sql.c_str() , delete_sql.length() ); 

        if (!res) 
        {
          /*delete ok*/
          rules_delete_ok_info();
        }

        else 
        {
          rules_delete_fail_info();
          fprintf(stderr, "UPDATE error %d: %s\n", 
            mysql_errno(rules_show_conn),mysql_error(rules_show_conn));
        }

     } //end delete

 //--------------- delete over ---------------------
   
   mysql_close(rules_show_conn); //关闭与数据库的连接

 }//end else

}

/*rules_close_delete_window */
void rules_close_delete_window()
{
    gtk_widget_destroy(rules_delete_window);
}

/*rules_open_delete_window*/
void rules_open_delete_window()
{

  rules_delete_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(rules_delete_window), "Delete Rule");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(rules_delete_window), 200, 160);

  gtk_window_set_position(GTK_WINDOW(rules_delete_window), GTK_WIN_POS_CENTER);

  GtkWidget *delete_frame;
  delete_frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(rules_delete_window), delete_frame);

    //--------------   input ----------------------------------------------
  GtkWidget *rules_id_label;
  rules_id_label = gtk_label_new("规则号:");
  gtk_fixed_put(GTK_FIXED(delete_frame), rules_id_label, 20, 33);

   //GtkWidget *rules_delete_id_entry;
  rules_delete_id_entry = gtk_entry_new();
  gtk_widget_set_size_request(rules_delete_id_entry, 95, 30);
  gtk_fixed_put(GTK_FIXED(delete_frame), rules_delete_id_entry, 70, 30);


    //--------------  btn  ----------------------------------------------
  GtkWidget *look_yes_btn;
  look_yes_btn = gtk_button_new_with_label("确认");
  gtk_widget_set_size_request(look_yes_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(delete_frame), look_yes_btn, 20, 100);
  g_signal_connect(look_yes_btn, "clicked", G_CALLBACK (rules_delete_sql), NULL);

  GtkWidget *look_no_btn;
  look_no_btn = gtk_button_new_with_label("取消");
  gtk_widget_set_size_request(look_no_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(delete_frame), look_no_btn, 110, 100);
  g_signal_connect(look_no_btn, "clicked", G_CALLBACK (rules_close_delete_window), NULL);

    //------------------------------------------------------------
  gtk_widget_show_all(rules_delete_window);

}


//********************   rules_look_window   *******************************************

/*rules_look_sql*/
void rules_look_sql()
{
     char ch_rule_id[10];

     strcpy(ch_rule_id, gtk_entry_get_text(GTK_ENTRY(rules_look_id_entry)) );

     string rule_id = ch_rule_id;

     if(rule_id=="" || rule_id==" ")
     {
       rules_look_id_null_warning();
     }
    
    else
    {

    MYSQL *rules_show_conn;
    MYSQL_RES *rules_show_res;
    MYSQL_ROW rules_show_row;

    rules_show_conn=mysql_init(NULL);

     //连接test数据库
   if(!mysql_real_connect(rules_show_conn,"localhost","root","","myIDS",0,NULL,0)) 
   {
     printf("Connect Error!n");
     exit(1);
   }
   
   string query_sql="select * from ruleBase where id ='"+ rule_id+"';";

   if( mysql_real_query( rules_show_conn , query_sql.c_str() , query_sql.length() ) )
   {
     printf("Query Error!n");
     exit(1);
   }

   rules_show_res=mysql_use_result(rules_show_conn);

   bool isGet=false;

   while((rules_show_row=mysql_fetch_row(rules_show_res))!=NULL) //打印结果
   {
    isGet=true;

    char * str[5];
    str[0]=rules_show_row[0];
    str[1]=rules_show_row[1];
    str[2]=rules_show_row[2];
    str[3]=rules_show_row[3];
    str[4]=rules_show_row[4];
    gtk_clist_append(GTK_CLIST(rules_clist),str);

   }

   if(! isGet)//no such id
   {
     rules_look_not_exist_warning();
   }

   mysql_free_result(rules_show_res); //关闭结果集
   mysql_close(rules_show_conn); //关闭与数据库的连接

 }//end else

}

/*rules_close_look_window */
void rules_close_look_window()
{
    gtk_widget_destroy(rules_look_window);
}

/*rules_create window*/
void rules_open_look_window()
{
  /*clean rules_clist */
  gtk_clist_clear( GTK_CLIST(rules_clist) );

  rules_look_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(rules_look_window), "Find Rule");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(rules_look_window), 200, 160);

  gtk_window_set_position(GTK_WINDOW(rules_look_window), GTK_WIN_POS_CENTER);

  GtkWidget *look_frame;
  look_frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(rules_look_window), look_frame);

    //--------------   input ----------------------------------------------
  GtkWidget *rules_id_label;
  rules_id_label = gtk_label_new("规则号:");
  gtk_fixed_put(GTK_FIXED(look_frame), rules_id_label, 20, 33);

   //GtkWidget *rules_look_id_entry;
  rules_look_id_entry = gtk_entry_new();
  gtk_widget_set_size_request(rules_look_id_entry, 95, 30);
  gtk_fixed_put(GTK_FIXED(look_frame), rules_look_id_entry, 70, 30);


    //--------------  btn  ----------------------------------------------
  GtkWidget *look_yes_btn;
  look_yes_btn = gtk_button_new_with_label("确认");
  gtk_widget_set_size_request(look_yes_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(look_frame), look_yes_btn, 20, 100);
  g_signal_connect(look_yes_btn, "clicked", G_CALLBACK (rules_look_sql), NULL);

  GtkWidget *look_no_btn;
  look_no_btn = gtk_button_new_with_label("取消");
  gtk_widget_set_size_request(look_no_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(look_frame), look_no_btn, 110, 100);
  g_signal_connect(look_no_btn, "clicked", G_CALLBACK (rules_close_look_window), NULL);

    //------------------------------------------------------------
  gtk_widget_show_all(rules_look_window);

}

//****************************  rules_main_window  **********************************

/*rules_return_main window*/
void rules_return_main()
{
    gtk_widget_destroy(rules_window);
}

 void ui_open_rules_window()
 {

  rules_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(rules_window), "Rules");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(rules_window), 500, 300);

  gtk_window_set_position(GTK_WINDOW(rules_window), GTK_WIN_POS_CENTER);

  GtkWidget *rules_vbox;  
        /*创建一个盒装容器并添加到窗口中*/
  rules_vbox = gtk_vbox_new(FALSE, 0);

    //---------------rules_menu-------------------------------------------
  GtkWidget *rules_hbox1;
  rules_hbox1=gtk_hbox_new(FALSE,0);

  GtkWidget *rules_menu;
    rules_menu=gtk_menu_bar_new();/*创建菜单条*/

  GtkWidget *rules_look_item;
  rules_look_item=gtk_menu_item_new_with_label("查看规则");
  gtk_menu_shell_append(GTK_MENU_SHELL(rules_menu),rules_look_item);
  g_signal_connect(rules_look_item, "activate", G_CALLBACK (rules_open_look_window), NULL);

  GtkWidget *rules_add_item;
  rules_add_item=gtk_menu_item_new_with_label("新建规则");
  gtk_menu_shell_append(GTK_MENU_SHELL(rules_menu),rules_add_item);
  g_signal_connect(rules_add_item, "activate", G_CALLBACK (rules_open_add_window), NULL);

    GtkWidget *rules_modify_item;
  rules_modify_item=gtk_menu_item_new_with_label("修改规则");
  gtk_menu_shell_append(GTK_MENU_SHELL(rules_menu),rules_modify_item);
  g_signal_connect(rules_modify_item, "activate", G_CALLBACK (rules_open_modify_window), NULL);

    GtkWidget *rules_delete_item;
  rules_delete_item=gtk_menu_item_new_with_label("删除规则");
  gtk_menu_shell_append(GTK_MENU_SHELL(rules_menu),rules_delete_item);
 g_signal_connect(rules_delete_item, "activate", G_CALLBACK (rules_open_delete_window), NULL);

  GtkWidget *rules_return_item;
  rules_return_item=gtk_menu_item_new_with_label("返回");
  gtk_menu_shell_append(GTK_MENU_SHELL(rules_menu),rules_return_item);
  g_signal_connect(rules_return_item, "activate", G_CALLBACK (rules_return_main), NULL);

    /*把菜单条加入组合框*/
  gtk_box_pack_start(GTK_BOX(rules_hbox1),rules_menu,TRUE,TRUE,0);

  gtk_box_pack_start(GTK_BOX(rules_vbox),rules_hbox1,FALSE,FALSE,0);

  //---------------rules hbox2-------------------------------------------
  GtkWidget *rules_hbox2;
  rules_hbox2=gtk_hbox_new(FALSE,0);


  char *rules_clist_title[] = {
    "id","msg","content","hex_content","type"
  };

  rules_clist = gtk_clist_new_with_titles(5,rules_clist_title);
  gtk_clist_set_column_width(GTK_CLIST(rules_clist),0,20);
  gtk_clist_set_column_width(GTK_CLIST(rules_clist),1,80);
  gtk_clist_set_column_width(GTK_CLIST(rules_clist),2,130);
  gtk_clist_set_column_width(GTK_CLIST(rules_clist),3,130);
  gtk_clist_set_column_width(GTK_CLIST(rules_clist),4,80);


  gtk_box_pack_start(GTK_BOX(rules_hbox2),rules_clist,TRUE,TRUE,0);

  gtk_box_pack_start(GTK_BOX(rules_vbox),rules_hbox2,FALSE,FALSE,0);

//----------------------------------------------------------

  gtk_container_add(GTK_CONTAINER(rules_window), rules_vbox);

  gtk_widget_show_all(rules_window);

  
}

   

  //========================== dialog =======================================


void rules_look_not_exist_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_look_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "不存在该规则");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_look_id_null_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_look_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "规则号不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_delete_not_exist_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_delete_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "不存在该规则");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_delete_id_null_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_delete_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "规则号不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_delete_ok_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_delete_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "删除成功", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_delete_fail_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_delete_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "删除失败", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_no_type_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_add_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "类型不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}


void rules_no_content_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_add_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "内容不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}


void rules_no_msg_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_add_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "报警信息不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_add_ok_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_add_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "添加成功", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_add_fail_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_add_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "添加失败", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_modify_id_length_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_modify_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "规则号应小于10位");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_modify_id_null_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_modify_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "规则号不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_modify_not_exist_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_modify_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "不存在该规则");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_modify_entry_null_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_modify_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "规则信息不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_modify_ok_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_modify_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "修改成功", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void rules_modify_fail_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(rules_modify_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "修改失败", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

string rules_string_to_hex(char * str, int len)
{
   string hex_content;

   int i;
   int leftNum= 0;
   for(i=0;i<len;i++)
   {
  //  printf("  -   %c\n", str[i]);
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
        char val[8] = {0};
        sprintf(val, "%2X", str[i]); //小写则为%x
     //   printf("%s\n", val);
        hex_content.append(string(val));
      }

  }

  return hex_content;
}