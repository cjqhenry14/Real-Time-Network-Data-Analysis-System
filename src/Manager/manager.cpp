#include <gtk/gtk.h>
#include <mysql.h>  
#include <stdlib.h>
#include <string.h>
#include <string>

#include "manager.h"

using namespace std;

GtkWidget *manager_clist;
GtkWidget *manager_window;

GtkWidget *create_manager_window;

GtkWidget *username_entry;
GtkWidget *pwd_entry;
GtkWidget *qr_pwd_entry;

GtkWidget *change_manager_window;
GtkWidget *old_pwd_entry;
GtkWidget *new_pwd_entry;
GtkWidget *qr_new_pwd_entry;

//string my_username="admin";// login user's name, just temp, should be get from login window
//=============================change pwd window=================================================

/* query old pwd is right or not*/
bool manager_query_old_pwd(string old_pwd)
{
    MYSQL *mmanager_query_pwd_conn;
    MYSQL_RES *manager_query_pwd_res;
    MYSQL_ROW manager_query_pwd_row;

    mmanager_query_pwd_conn=mysql_init(NULL);

   //连接test数据库
    if(!mysql_real_connect(mmanager_query_pwd_conn,"localhost","root","","myIDS",0,NULL,0)) 
    {
      printf("Connect Error!n");
      exit(1);
    }

   string query_sql="select * from manager where username ='"+ my_username+"' and pwd = '"+ old_pwd+"';";

   if( mysql_real_query( mmanager_query_pwd_conn , query_sql.c_str() , query_sql.length() ) )
   {
     printf("Query Error!n");
     exit(1);
   }

   manager_query_pwd_res=mysql_use_result(mmanager_query_pwd_conn); 
   
   int user_count=0;

   while((manager_query_pwd_row=mysql_fetch_row(manager_query_pwd_res))!=NULL) //打印结果
   {
         user_count++;
   }

   mysql_free_result(manager_query_pwd_res); //关闭结果集
   mysql_close(mmanager_query_pwd_conn); //关闭与数据库的连接

   if(user_count==0)
   {
     return false;//old pwd is wrong
   }
   else
   {
     return true;//old pwd is right
   }

}

/*manager_change_insert_sql*/
void manager_change_insert_sql()
{
   int old_pwd_len=strlen(gtk_entry_get_text(GTK_ENTRY(old_pwd_entry) )  );
   int new_pwd_len=strlen(gtk_entry_get_text(GTK_ENTRY(new_pwd_entry) )  );

   if( old_pwd_len > 10 || new_pwd_len>10 )
   {
    //show_new_pwd_length_warning();
    show_change_new_pwd_length_warning();
   }
   else
   {

     string str_old_pwd;
     string str_new_pwd;

     char ch_old_pwd[10];
     char ch_new_pwd[10];

     strcpy(ch_old_pwd, gtk_entry_get_text(GTK_ENTRY(old_pwd_entry)) );
     strcpy(ch_new_pwd, gtk_entry_get_text(GTK_ENTRY(new_pwd_entry)) );

     str_old_pwd = ch_old_pwd;
     str_new_pwd = ch_new_pwd;

     if(! manager_query_old_pwd(str_old_pwd) )// wrong pwd
     {
      // show_wrong_pwd_warning();
      show_change_wrong_pwd_warning();
     }

     else if(new_pwd_len>10)
     { 
      show_change_new_pwd_length_warning();
     }

     else if(strcmp(gtk_entry_get_text(GTK_ENTRY(new_pwd_entry)), gtk_entry_get_text(GTK_ENTRY(qr_new_pwd_entry))) != 0)
     {
       //show_qr_pwd_warning();
      show_change_qr_pwd_warning();
     }

     else // start insert sql
     {
       //show_change_pwd_ok_info();
         MYSQL *manager_insert_pwd_conn;

         manager_insert_pwd_conn=mysql_init(NULL);

         int res=0;
          //连接test数据库
         if(!mysql_real_connect(manager_insert_pwd_conn,"localhost","root","","myIDS",0,NULL,0)) 
         {
             printf("Connect Error!n");
             exit(1);
         } 

         string insert_sql ="UPDATE manager set pwd='"+ str_new_pwd +"' where username ='"+my_username+"';";
  
         res =mysql_real_query( manager_insert_pwd_conn , insert_sql.c_str() , insert_sql.length() ); 

        if (!res) 
        {
          printf("UPDATE %lu rows\n", (unsigned long)mysql_affected_rows(manager_insert_pwd_conn));
          /*change ok*/
          show_change_pwd_ok_info();
        }

        else 
        {
          fprintf(stderr, "UPDATE error %d: %s\n", mysql_errno(manager_insert_pwd_conn),mysql_error(manager_insert_pwd_conn));
        }

        mysql_close(manager_insert_pwd_conn); //关闭与数据库的连接


     }// end  insert sql

  }//end else length <10


}

void manager_close_change_window()
{
  gtk_widget_destroy(change_manager_window);
}

/*manager_open_change  window*/
void manager_open_change_window()
{
  
  change_manager_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(change_manager_window), "Change Password");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(change_manager_window), 300, 190);

  gtk_window_set_position(GTK_WINDOW(change_manager_window), GTK_WIN_POS_CENTER);

  GtkWidget *change_frame;
  change_frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(change_manager_window), change_frame);

    //--------------   input ----------------------------------------------
  GtkWidget *old_pwd_label;
  old_pwd_label = gtk_label_new("原密码:");
  gtk_fixed_put(GTK_FIXED(change_frame), old_pwd_label, 40, 20);

  GtkWidget *new_pwd_label;
  new_pwd_label = gtk_label_new("新密码:");
  gtk_fixed_put(GTK_FIXED(change_frame), new_pwd_label, 40, 60);

  GtkWidget *qr_new_pwd_label;
  qr_new_pwd_label = gtk_label_new("确认密码:");
  gtk_fixed_put(GTK_FIXED(change_frame), qr_new_pwd_label, 40, 100);

      //GtkWidget *old_pwd_entry;
  old_pwd_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(change_frame), old_pwd_entry, 110, 20);
  gtk_entry_set_visibility(GTK_ENTRY(old_pwd_entry),FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY(old_pwd_entry), '*');

     //GtkWidget *new_pwd_entry;
  new_pwd_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(change_frame), new_pwd_entry, 110, 60);
  gtk_entry_set_visibility(GTK_ENTRY(new_pwd_entry),FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY(new_pwd_entry), '*');

     //GtkWidget *qr_new_pwd_entry;
  qr_new_pwd_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(change_frame), qr_new_pwd_entry, 110, 100);
  gtk_entry_set_visibility(GTK_ENTRY(qr_new_pwd_entry),FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY(qr_new_pwd_entry), '*');


    //--------------  btn  ----------------------------------------------
  GtkWidget *confirm_change_btn;
  confirm_change_btn = gtk_button_new_with_label("确认");
  gtk_widget_set_size_request(confirm_change_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(change_frame), confirm_change_btn, 50, 150);
  g_signal_connect(confirm_change_btn, "clicked", G_CALLBACK (manager_change_insert_sql), NULL);

  GtkWidget *cancel_change_btn;
  cancel_change_btn = gtk_button_new_with_label("取消");
  gtk_widget_set_size_request(cancel_change_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(change_frame), cancel_change_btn, 180, 150);
  g_signal_connect(cancel_change_btn, "clicked", 
   G_CALLBACK (manager_close_change_window), NULL);

    //------------------------------------------------------------
  gtk_widget_show_all(change_manager_window);

}

//================================= end change pwd window  ========================================

//*************************************** dialog **************************************************

void show_no_pwd_warning()
{
   GtkWidget *dialog;
   dialog = gtk_message_dialog_new(GTK_WINDOW(create_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "密码不能为空");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_namepwd_length_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(create_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "用户名和密码长度应小于10");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_change_new_pwd_length_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(change_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "密码长度应小于10");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_new_pwd_length_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(create_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "密码长度应小于10");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_change_qr_pwd_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(change_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "确认密码错误");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_qr_pwd_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(create_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "确认密码错误");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_change_wrong_pwd_warning()
{
   GtkWidget *dialog;
   dialog = gtk_message_dialog_new(GTK_WINDOW(change_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "密码不正确");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_wrong_pwd_warning()
{
   GtkWidget *dialog;
   dialog = gtk_message_dialog_new(GTK_WINDOW(create_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "密码不正确");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_user_exist_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(create_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "用户名已经存在");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_change_pwd_ok_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(change_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "修改成功", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_create_manager_ok_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(create_manager_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "新建成功", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
//****************** end dialog *****************************************************

/* query user is exist or not*/
bool manager_query_user(string manager_username)
{
    MYSQL *mmanager_query_user_conn;
    MYSQL_RES *manager_query_user_res;
    MYSQL_ROW manager_query_user_row;

    mmanager_query_user_conn=mysql_init(NULL);

   //连接test数据库
    if(!mysql_real_connect(mmanager_query_user_conn,"localhost","root","","myIDS",0,NULL,0)) 
    {
      printf("Connect Error!n");
      exit(1);
    }

   string query_sql="select * from manager where username ='"+ manager_username+"';";

   if( mysql_real_query( mmanager_query_user_conn , query_sql.c_str() , query_sql.length() ) )
   {
     printf("Query Error!n");
     exit(1);
   }

   manager_query_user_res=mysql_use_result(mmanager_query_user_conn); //取得表中的数据并存储到manager_query_user_res中
   
   int user_count=0;

   while((manager_query_user_row=mysql_fetch_row(manager_query_user_res))!=NULL) //打印结果
   {
         user_count++;

   }

   mysql_free_result(manager_query_user_res); //关闭结果集
   mysql_close(mmanager_query_user_conn); //关闭与数据库的连接

   if(user_count==0)
   {
     return false;
   }
   else
   {
     return true;
   }

}


void manager_close_create_window()
{
  gtk_widget_destroy(create_manager_window);
}

/*manager_create_insert_sql*/
void manager_create_insert_sql()
{

   int name_len=strlen(gtk_entry_get_text(GTK_ENTRY(username_entry) )  );
   int pwd_len=strlen(gtk_entry_get_text(GTK_ENTRY(pwd_entry) )  );
   int qr_pwd_len=strlen(gtk_entry_get_text(GTK_ENTRY(qr_pwd_entry) )  );

  if(pwd_len==0)
  {
    show_no_pwd_warning();
  }

  else if(name_len>10 || pwd_len>10 ||qr_pwd_len>10 )
  {
    show_namepwd_length_warning();
  }

  else if(strcmp(gtk_entry_get_text(GTK_ENTRY(pwd_entry)), gtk_entry_get_text(GTK_ENTRY(qr_pwd_entry))) != 0)
  {
    show_qr_pwd_warning();
  }

  else
  {
  string manager_username;
  string manager_pwd;
  string manager_qr_pwd;
  string insert_sql;

  char ch_manager_username[10];
  char ch_manager_pwd[10];
  char ch_manager_qr_pwd[10];

  strcpy(ch_manager_username, gtk_entry_get_text(GTK_ENTRY(username_entry)) );
  strcpy(ch_manager_pwd, gtk_entry_get_text(GTK_ENTRY(pwd_entry)) );
  strcpy(ch_manager_qr_pwd, gtk_entry_get_text(GTK_ENTRY(qr_pwd_entry)) );


  manager_username = ch_manager_username;
  manager_pwd = ch_manager_pwd;
  manager_qr_pwd = ch_manager_qr_pwd;


  MYSQL *manager_insert_conn;

  manager_insert_conn=mysql_init(NULL);

  int res=0;

   //连接test数据库
  if(!mysql_real_connect(manager_insert_conn,"localhost","root","","myIDS",0,NULL,0)) 
  {
    printf("Connect Error!n");
    exit(1);
  }
  //-------- query username is exist --------------------------------
  
  bool userIsExist= manager_query_user(manager_username);
  //----------------- insert --------------------------------
  if(! userIsExist)
  {
  
    insert_sql ="INSERT INTO manager VALUES( NULL, '"+ manager_username +"', '"+  manager_pwd +"');";
  
    res =mysql_real_query( manager_insert_conn , insert_sql.c_str() , insert_sql.length() ); 

    if (!res) 
    {
      printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(manager_insert_conn));
      /*create ok*/
      show_create_manager_ok_info();
      int new_id = mysql_insert_id(manager_insert_conn);
      char ch_new_id[5];
      sprintf(ch_new_id,"%d",new_id);

      char * str[2];
      str[0]=ch_new_id;
      str[1]=ch_manager_username;
      gtk_clist_append(GTK_CLIST(manager_clist),str);
    } 

    else 
    {
      fprintf(stderr, "Insert error %d: %s\n", mysql_errno(manager_insert_conn),mysql_error(manager_insert_conn));
    }

      mysql_close(manager_insert_conn); //关闭与数据库的连接
  }// end if(! userIsExist)

    else  //if(userIsExist)
    {
        show_user_exist_warning();
    }

 }


}

/*manager_return_main window*/
void manager_return_main()
{
    gtk_widget_destroy(manager_window);
}

/*manager_create window*/
void manager_open_create_window()
{
  
  create_manager_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(create_manager_window), "New Manager");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(create_manager_window), 300, 190);

  gtk_window_set_position(GTK_WINDOW(create_manager_window), GTK_WIN_POS_CENTER);

  GtkWidget *create_frame;
  create_frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(create_manager_window), create_frame);

    //--------------   input ----------------------------------------------
  GtkWidget *username_label;
  username_label = gtk_label_new("用户名:");
  gtk_fixed_put(GTK_FIXED(create_frame), username_label, 40, 20);

  GtkWidget *pwd_label;
  pwd_label = gtk_label_new("密码:");
  gtk_fixed_put(GTK_FIXED(create_frame), pwd_label, 40, 60);

  GtkWidget *qr_pwd_label;
  qr_pwd_label = gtk_label_new("确认密码:");
  gtk_fixed_put(GTK_FIXED(create_frame), qr_pwd_label, 40, 100);

      //GtkWidget *username_entry;
  username_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), username_entry, 110, 20);

     //GtkWidget *pwd_entry;
  pwd_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), pwd_entry, 110, 60);
  gtk_entry_set_visibility(GTK_ENTRY(pwd_entry),FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY(pwd_entry), '*');

     //GtkWidget *qr_pwd_entry;
  qr_pwd_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(create_frame), qr_pwd_entry, 110, 100);
  gtk_entry_set_visibility(GTK_ENTRY(qr_pwd_entry),FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY(qr_pwd_entry), '*');


    //--------------  btn  ----------------------------------------------
  GtkWidget *create_yes_btn;
  create_yes_btn = gtk_button_new_with_label("新建");
  gtk_widget_set_size_request(create_yes_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(create_frame), create_yes_btn, 50, 150);
  g_signal_connect(create_yes_btn, "clicked", G_CALLBACK (manager_create_insert_sql), NULL);

  GtkWidget *create_no_btn;
  create_no_btn = gtk_button_new_with_label("取消");
  gtk_widget_set_size_request(create_no_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(create_frame), create_no_btn, 180, 150);
  g_signal_connect(create_no_btn, "clicked", 
   G_CALLBACK (manager_close_create_window), NULL);

    //------------------------------------------------------------
  gtk_widget_show_all(create_manager_window);

}

/*manager_show_all*/
void manager_show_all()
{
 MYSQL *manager_show_conn;
 MYSQL_RES *manager_show_res;
 MYSQL_ROW manager_show_row;
 manager_show_conn=mysql_init(NULL);

   //连接test数据库
 if(!mysql_real_connect(manager_show_conn,"localhost","root","","myIDS",0,NULL,0)) 
 {
  printf("Connect Error!n");
  exit(1);
 }

   if(mysql_query(manager_show_conn,"select * from manager")) //连接WordC表
   {
    printf("Query Error!n");
     exit(1);
   }

   manager_show_res=mysql_use_result(manager_show_conn); //取得表中的数据并存储到manager_show_res中

   while((manager_show_row=mysql_fetch_row(manager_show_res))!=NULL) //打印结果
   {

    char * str[2];
    str[0]=manager_show_row[0];
    str[1]=manager_show_row[1];
    gtk_clist_append(GTK_CLIST(manager_clist),str);

  }

   mysql_free_result(manager_show_res); //关闭结果集
   mysql_close(manager_show_conn); //关闭与数据库的连接
 }

/*manager open function; open manager_window*/
 void ui_open_manager_window()
 {

  manager_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(manager_window), "Manager");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(manager_window), 500, 400);

  gtk_window_set_position(GTK_WINDOW(manager_window), GTK_WIN_POS_CENTER);

  GtkWidget *manager_vbox;  
        /*创建一个盒装容器并添加到窗口中*/
  manager_vbox = gtk_vbox_new(FALSE, 0);


    //---------------manager_menu-------------------------------------------
  GtkWidget *manager_hbox1;
  manager_hbox1=gtk_hbox_new(FALSE,0);

  GtkWidget *manager_menu;
    manager_menu=gtk_menu_bar_new();/*创建菜单条*/

  GtkWidget *manager_create_item;
  manager_create_item=gtk_menu_item_new_with_label("新建管理员");
  gtk_menu_shell_append(GTK_MENU_SHELL(manager_menu),manager_create_item);
  g_signal_connect(manager_create_item, "activate", G_CALLBACK (manager_open_create_window), NULL);

  GtkWidget *manager_change_item;
  manager_change_item=gtk_menu_item_new_with_label("修改密码");
  gtk_menu_shell_append(GTK_MENU_SHELL(manager_menu),manager_change_item);
   g_signal_connect(manager_change_item, "activate", G_CALLBACK (manager_open_change_window), NULL);

  GtkWidget *manager_return_item;
  manager_return_item=gtk_menu_item_new_with_label("返回");
  gtk_menu_shell_append(GTK_MENU_SHELL(manager_menu),manager_return_item);
  g_signal_connect(manager_return_item, "activate", G_CALLBACK (manager_return_main), NULL);

    /*把菜单条加入组合框*/
  gtk_box_pack_start(GTK_BOX(manager_hbox1),manager_menu,TRUE,TRUE,0);

  gtk_box_pack_start(GTK_BOX(manager_vbox),manager_hbox1,FALSE,FALSE,0);

    //---------------manager hbox2-------------------------------------------
  GtkWidget *manager_hbox2;
  manager_hbox2=gtk_hbox_new(FALSE,0);


  char *manager_clist_title[] = {
    "id","username"
  };

  manager_clist = gtk_clist_new_with_titles(2,manager_clist_title);
  gtk_clist_set_column_width(GTK_CLIST(manager_clist),0,50);
  gtk_clist_set_column_width(GTK_CLIST(manager_clist),1,50);

    /*show admin list*/
  manager_show_all();

  gtk_box_pack_start(GTK_BOX(manager_hbox2),manager_clist,TRUE,TRUE,0);

  gtk_box_pack_start(GTK_BOX(manager_vbox),manager_hbox2,FALSE,FALSE,0);

//----------------------------------------------------------

  gtk_container_add(GTK_CONTAINER(manager_window), manager_vbox);

  gtk_widget_show_all(manager_window);

  
}