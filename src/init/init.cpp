#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <mysql.h>
#include <string.h>
//my .h
#include "sniffer_sqlToCache.h"
#include "decoder_dataCache.h"
#include "preprocessor_process.h"
#include "detect_acvl.h"
#include "init.h"
#include "manager.h"
#include "rulebase.h"
#include "attack_store.h"
#include "record.h"
//gtk h
#include <gtk/gtk.h> 

using namespace std;
//login 
 void make_main_window();
 void make_login_window();
 void login_check();
 void show_login_error_warning();
//======================== main =====================================================
GtkWidget *login_window;
GtkWidget *login_name_entry;
GtkWidget *login_pwd_entry;

// end login

/***
g++ init.o sqlToCache.o dataCache.o process.o detect_acvl.o -fPIC -shared -o libids.so
g++ -I ../../include/ -o init.obj -lids -L/usr/lib/mysql -lmysqlclient -lz
ln libids.so /usr/lib/
//g++ -I ../../include/ -o init.o  init.cpp $(mysql_config --cflags) ../Sniffer/sqlToCache.cpp $(mysql_config --libs) ../Decoder/dataCache.cpp ../Preprocessor/process.cpp ../DetectionEngine/detect_acvl.cpp -lpthread                              
***/

//g++ init.o sqlToCache.o dataCache.o process.o detect_acvl.o -fPIC -shared -o libids.so -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient -lz
ACNODE *root;

pthread_t _thread_get_sql_data;

pthread_t _thread_get_pcap_data;

pthread_t _thread_get_tcp_cache, _thread_get_udp_cache;

pthread_t _thread_get_tcp_detect;

//------------------
queue <store_cache> store_queue ;
//-----------------
queue <tcp_cache> tcp_queue;//TCP INIT
queue <udp_cache> udp_queue;//UDP INIT

pthread_mutex_t tcpMut;
pthread_mutex_t udpMut;
pthread_cond_t hasNode;
pthread_cond_t hasUdpNode;

queue <detect_info> detect_info_queue;

//-------- control value ----------
bool isDetecting = false;
bool isBuilt = false;

//--------------------
void show_rules_not_exist_warning();
void show_rules_exist_warning();
void show_is_detecting_warning();
void show_is_not_detecting_warning();
void show_stop_ok_info();
void show_build_time_info(double time);
void show_about();
void show_store_ok_info();

void make_main_window();
void make_window();
void make_menubar();//void make_box1();
void make_box2();
void make_box3();

void ui_save_detect();
void ui_about();
//---------- extern ------------
    string my_username;

    GtkWidget *window;
    GtkWidget *vbox;            //盒装容器

    GtkWidget *tcpText;
    GtkWidget *udpText;
    GtkWidget *tcpScrolled;
    GtkWidget *udpScrolled;


    GtkWidget *tcpLabel;
    GtkWidget *udpLabel;

    GtkWidget *tcpCountLabel;
    GtkWidget *udpCountLabel;

    GtkTextBuffer *tcpBuffer;
    GtkTextBuffer *udpBuffer;

    GtkTextIter tcpIter;
    GtkTextIter udpIter;


void init_start_all()//
{
    root = (ACNODE *)malloc(sizeof(ACNODE));// init ac root
    ac_build_rules(root);//init ac automation rules and fails

    pthread_mutex_init(&tcpMut,NULL);
    pthread_mutex_init(&udpMut,NULL);

    start_tcp_sql_conn();
    start_udp_sql_conn();

   //if(pthread_create(&_thread_get_pcap_data,NULL,get_hex_data_from_sql,NULL) != 0)//get_data_from_sql
     if(pthread_create(&_thread_get_pcap_data,NULL,get_data_from_pcap,NULL) != 0)       //comment2
    {
        printf("线程get_data_from_pcap创建失败!\n");
    }

   if(pthread_create(&_thread_get_tcp_cache,NULL,get_tcp_cache_thread,(void *)root) != 0)       //comment2
    {
      printf("线程 get_tcp_cache_thread 创建失败!\n");
    }

    if(pthread_create(&_thread_get_udp_cache,NULL,get_udp_cache_thread,(void *)root) != 0)       //comment2
    {
       printf("线程 get_udp_cache_thread 创建失败!\n");
    }

   pthread_join(_thread_get_pcap_data,NULL);
   pthread_join(_thread_get_tcp_cache,NULL);
   pthread_join(_thread_get_udp_cache,NULL);
}

void init_sqltest_start_all()//sql test
{
    root = (ACNODE *)malloc(sizeof(ACNODE));// init ac root
    ac_build_rules(root);//init ac automation rules and fails

    pthread_mutex_init(&tcpMut,NULL);
    pthread_mutex_init(&udpMut,NULL);

    start_tcp_sql_conn();
    start_udp_sql_conn();

   if(pthread_create(&_thread_get_pcap_data,NULL,get_hex_data_from_sql,NULL) != 0)//get_data_from_sql
     //if(pthread_create(&_thread_get_pcap_data,NULL,get_data_from_pcap,NULL) != 0)       //comment2
    {
        printf("线程get_data_from_pcap创建失败!\n");
    }

   if(pthread_create(&_thread_get_tcp_cache,NULL,get_tcp_cache_thread,(void *)root) != 0)       //comment2
    {
      printf("线程 get_tcp_cache_thread 创建失败!\n");
    }

    if(pthread_create(&_thread_get_udp_cache,NULL,get_udp_cache_thread,(void *)root) != 0)       //comment2
    {
       printf("线程 get_udp_cache_thread 创建失败!\n");
    }

   pthread_join(_thread_get_pcap_data,NULL);
   pthread_join(_thread_get_tcp_cache,NULL);
   pthread_join(_thread_get_udp_cache,NULL);
}

//========= ui control callback ====================
void ui_start_detect()
{
  if(isBuilt && !isDetecting)
  {
    printf("%s\n", "detect start!");
    g_thread_create((GThreadFunc)init_start_all, NULL, FALSE, NULL);
    isDetecting = true;
  }

  else if(!isBuilt)
  {
    show_rules_not_exist_warning();
    printf("%s\n", "rules have not been built!");
  }

  else if(isDetecting)
  {
    show_is_detecting_warning();
    printf("%s\n", "already detecting!");
  }

}

void ui_auto_detect()
{
  if(!isBuilt && !isDetecting)
  {
    printf("%s\n", "auto_detect start!");
    printf("%s\n", "building...");

    root = (ACNODE *)malloc(sizeof(ACNODE));// init ac root

    ac_build_rules(root);//init ac automation rules and fails

    pthread_mutex_init(&tcpMut,NULL);
    pthread_mutex_init(&udpMut,NULL);

    start_tcp_sql_conn();
    start_udp_sql_conn();
    printf("%s\n", "building over");

    g_thread_create((GThreadFunc)init_start_all, NULL, FALSE, NULL);

    isDetecting = true;
    isBuilt = true;
  }

  else if(isBuilt)
  {
    show_rules_exist_warning();
    printf("%s\n", "rules have already been built!");
  }

  else if(isDetecting)
  {
    show_is_detecting_warning();
    printf("%s\n", "already detecting!");
  }

}

void ui_sqltest_detect()//sql test
{
  if(!isBuilt && !isDetecting)
  {
    printf("%s\n", "auto_detect start!");
    printf("%s\n", "building...");

    root = (ACNODE *)malloc(sizeof(ACNODE));// init ac root

    ac_build_rules(root);//init ac automation rules and fails

    pthread_mutex_init(&tcpMut,NULL);
    pthread_mutex_init(&udpMut,NULL);

    start_tcp_sql_conn();
    start_udp_sql_conn();
    printf("%s\n", "building over");

    g_thread_create((GThreadFunc)init_sqltest_start_all, NULL, FALSE, NULL);

    isDetecting = true;
    isBuilt = true;
  }

  else if(isBuilt)
  {
    //show_rules_exist_warning();
    //printf("%s\n", "rules have already been built!");
     printf("%s\n", "test_detect start!");

    pthread_mutex_init(&tcpMut,NULL);
    pthread_mutex_init(&udpMut,NULL);

    start_tcp_sql_conn();
    start_udp_sql_conn();
    printf("%s\n", "building over");

    g_thread_create((GThreadFunc)init_sqltest_start_all, NULL, FALSE, NULL);

    isDetecting = true;
  }

  else if(isDetecting)
  {
    show_is_detecting_warning();
    printf("%s\n", "already detecting!");
  }

}

void ui_about()
{
    printf("%s\n", "about");
    show_about();
}

void ui_save_detect()
{
    //store_show_all_queue();// in attack_store.cpp
    store_save_queue();// put queue into sql
    show_store_ok_info();
}

void ui_build_rules()
{
    if(!isBuilt && !isDetecting)
    {
    printf("%s\n", "building...");

    root = (ACNODE *)malloc(sizeof(ACNODE));// init ac root

    clock_t start,middle;
    double preTime;
    start = clock();

    ac_build_rules(root);//init ac automation rules and fails

    middle = clock();
    preTime= (double)(middle - start) / CLOCKS_PER_SEC;

    show_build_time_info(preTime);

    isBuilt = true;

    pthread_mutex_init(&tcpMut,NULL);
    pthread_mutex_init(&udpMut,NULL);

    start_tcp_sql_conn();
    start_udp_sql_conn();

    printf("%s\n", "building over");
    }

    else if(isBuilt)
    {
      show_rules_exist_warning();
      printf("%s\n", "rules have already been built!");
    }
    
    else if(isDetecting)
    {
      show_is_detecting_warning();
      printf("%s\n", "already detecting!");
    }

}

void ui_open_record_window()
{
   record_show();//in record.cpp
}

/*void ui_clean_detect()
{
    //need to clean the store queue and text
    gtk_label_set_text(GTK_LABEL(tcpCountLabel),"0");
    gtk_label_set_text(GTK_LABEL(udpCountLabel),"0");
    gtk_text_buffer_set_text(tcpBuffer,"",0); //设置文本内容为空
    gtk_text_buffer_set_text(udpBuffer,"",0); //设置文本内容为空
}*/

void ui_stop_detect()
{
    if(isDetecting)
    {
       //close connection
       close_detect_sql_connection();

       pthread_cancel(_thread_get_sql_data) ;
       pthread_cancel(_thread_get_pcap_data) ;
       pthread_cancel(_thread_get_tcp_cache) ;
       pthread_cancel(_thread_get_udp_cache) ;

       printf("%s\n", "successfuly stop!");
       show_stop_ok_info();
       //QMessageBox::about(NULL, "info", "停止成功");
       isDetecting=false;
      
    }

    else
    {
       show_is_not_detecting_warning();
       printf("%s\n", "is not detecting!");
    }

}

//========= main ====================

int main(int argc, char** argv)
{

    hasNode = PTHREAD_COND_INITIALIZER;
    hasUdpNode = PTHREAD_COND_INITIALIZER;

    gtk_init(&argc, &argv);

    //make_main_window();
     make_login_window();

    gdk_threads_enter();
    gtk_main ();
    gdk_threads_leave();
    
}

//======================================== ui_init ==========================================


void make_menubar()
{
    GtkWidget *MenuBar;

    GtkWidget *hbox1;

    GtkWidget *buildRulesItem;
    GtkWidget *startDetectItem;
    GtkWidget *autoDetectItem;
    GtkWidget *aboutItem;
    GtkWidget *stopItem;
    GtkWidget *exitItem;
    GtkWidget *saveDetectItem;

    GtkWidget *sqltestDetectItem;
    //GtkWidget *cleanItem;

    hbox1=gtk_hbox_new(FALSE,0);
       /*创建菜单条*/
    MenuBar=gtk_menu_bar_new();
  
//------------------------------detect menu-------------------------
    GtkWidget *detectMenu;
    GtkWidget *detectMenuTitle;
    detectMenu=gtk_menu_new();//open,save,quit都要放在file_menu中;
    detectMenuTitle= gtk_menu_item_new_with_label("检测");

  /* 建立规则 item*/
    buildRulesItem=gtk_menu_item_new_with_label("建立规则");
  gtk_menu_shell_append(GTK_MENU_SHELL(detectMenu),buildRulesItem);
 g_signal_connect(buildRulesItem, "activate", G_CALLBACK(ui_build_rules), NULL);

/* 开始检测 item*/
    startDetectItem=gtk_menu_item_new_with_label("开始检测");
  gtk_menu_shell_append(GTK_MENU_SHELL(detectMenu),startDetectItem);
  g_signal_connect(startDetectItem, "activate", G_CALLBACK(ui_start_detect), NULL);

/* 自动检测 item*/
    autoDetectItem=gtk_menu_item_new_with_label("自动检测");
  gtk_menu_shell_append(GTK_MENU_SHELL(detectMenu),autoDetectItem);
    g_signal_connect(autoDetectItem, "activate", G_CALLBACK(ui_auto_detect), NULL);

/* 检测测试 item*/
    sqltestDetectItem=gtk_menu_item_new_with_label("检测测试");
  gtk_menu_shell_append(GTK_MENU_SHELL(detectMenu),sqltestDetectItem);
    g_signal_connect(sqltestDetectItem, "activate", G_CALLBACK(ui_sqltest_detect), NULL);

/* 保存检测结果 item*/
    saveDetectItem=gtk_menu_item_new_with_label("保存检测结果");
  gtk_menu_shell_append(GTK_MENU_SHELL(detectMenu),saveDetectItem);
    g_signal_connect(saveDetectItem, "activate", G_CALLBACK(ui_save_detect), NULL);

 /* stop item*/
    stopItem=gtk_menu_item_new_with_label("结束");
  gtk_menu_shell_append(GTK_MENU_SHELL(detectMenu),stopItem);
   g_signal_connect(stopItem, "activate", G_CALLBACK(ui_stop_detect), NULL);

    /* clean item*/
    /*cleanItem=gtk_menu_item_new_with_label("清空本次检测");
  gtk_menu_shell_append(GTK_MENU_SHELL(detectMenu),cleanItem);
   g_signal_connect(cleanItem, "activate", G_CALLBACK(ui_clean_detect), NULL);
*/
//设置子菜单,把file_menu设为file_item的子菜单
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(detectMenuTitle),detectMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar), detectMenuTitle);
    gtk_menu_bar_append(GTK_MENU_BAR(MenuBar),detectMenu);

//------------------------------manager menu-------------------------

   GtkWidget *managerItem;
   managerItem=gtk_menu_item_new_with_label("管理员");
  gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),managerItem);
  g_signal_connect(managerItem, "activate", G_CALLBACK (ui_open_manager_window), NULL);

//------------------------------record menu-------------------------

   GtkWidget *recordItem;
   recordItem=gtk_menu_item_new_with_label("检测记录");
  gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),recordItem);
  g_signal_connect(recordItem, "activate", G_CALLBACK (ui_open_record_window), NULL);

//------------------------------rules menu-------------------------

   GtkWidget *rulesItem;
   rulesItem=gtk_menu_item_new_with_label("规则管理");
  gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),rulesItem);
  g_signal_connect(rulesItem, "activate", G_CALLBACK(ui_open_rules_window), NULL);

//------------------------------about menu-------------------------
/* about item*/
   aboutItem=gtk_menu_item_new_with_label("关于");
  gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),aboutItem);
   g_signal_connect(aboutItem, "activate", G_CALLBACK(ui_about), NULL);


   /* 退出 item*/
    exitItem=gtk_menu_item_new_with_label("退出");
  gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),exitItem);
  g_signal_connect(exitItem, "activate", G_CALLBACK (gtk_main_quit), NULL);


gtk_box_pack_start(GTK_BOX(hbox1),MenuBar,TRUE,TRUE,0);/*把菜单条加入组合框*/
gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,FALSE,0);


}


void make_box2()
{
    GtkWidget *hbox2;
    hbox2=gtk_hbox_new(FALSE,0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox2),2);/*边框宽*/

    tcpLabel=gtk_label_new("  TCP DETECT   |   count: ");
    gtk_box_pack_start(GTK_BOX(hbox2),tcpLabel,FALSE,FALSE,0);

    tcpCountLabel=gtk_label_new("0");
    gtk_box_pack_start(GTK_BOX(hbox2),tcpCountLabel,FALSE,FALSE,0);

    gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,FALSE,0);

//========================TCP TEXT===================================

    tcpScrolled=gtk_scrolled_window_new(NULL,NULL); /*创建滚动窗口构件*/

    tcpText=gtk_text_view_new();/*创建文本视图构件*/
    gtk_container_add(GTK_CONTAINER(tcpScrolled), tcpText);/*将文本视图构件加入滚动窗口*/

    tcpBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tcpText));
    gtk_text_buffer_get_iter_at_offset (tcpBuffer, &tcpIter, 0);

    gtk_box_pack_start(GTK_BOX(vbox), tcpScrolled, TRUE, TRUE, 5);
    
}

void make_box3()
{
    GtkWidget *hbox3;
    hbox3=gtk_hbox_new(FALSE,0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox3),2);/*边框宽*/

    udpLabel=gtk_label_new("  UDP DETECT   |   count: ");
    gtk_box_pack_start(GTK_BOX(hbox3),udpLabel,FALSE,FALSE,0);

    udpCountLabel=gtk_label_new("0");
    gtk_box_pack_start(GTK_BOX(hbox3),udpCountLabel,FALSE,FALSE,0);

    gtk_box_pack_start(GTK_BOX(vbox),hbox3,FALSE,FALSE,0);

//=========================UDP TEXT==================================

    udpScrolled=gtk_scrolled_window_new(NULL,NULL); /*创建滚动窗口构件*/

    udpText=gtk_text_view_new();/*创建文本视图构件*/
    gtk_container_add(GTK_CONTAINER(udpScrolled), udpText);/*将文本视图构件加入滚动窗口*/

    udpBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (udpText));
    gtk_text_buffer_get_iter_at_offset (udpBuffer, &udpIter, 0);

    gtk_box_pack_start(GTK_BOX(vbox), udpScrolled, TRUE, TRUE, 5);
}

void make_window()
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(window), "IDS");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(window), 1000, 700);
   /*设置窗口broder*/
}

void make_main_window()
{
    /*window*/
    make_window();

    /*创建一个盒装容器并添加到窗口中*/
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    /*H BOX1 and menubar*/
    make_menubar(); 

    /*H BOX2*/
    make_box2();

    /*H BOX3*/
    make_box3(); 

    /* show_all*/
    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    /*线程的初始化*/  
    if(!g_thread_supported()) g_thread_init(NULL);  
    gdk_threads_init();  
    /*创建线程*/  
}


//============= dialog ==============================


void show_rules_not_exist_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "还没建立规则");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}


void show_rules_exist_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "规则已经建立");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_is_detecting_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "正在检测中");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_is_not_detecting_warning()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "还没开始检测");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_stop_ok_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "停止成功", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_build_time_info(double pretime)
{
  printf("%f\n", pretime);
    char temp[10];
    string st, t;
    sprintf(temp, "%.2f", pretime);
    t=temp;
    st = "用时: " +t+"s";

  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            st.c_str(), "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void show_about()
{

  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "IDS");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0"); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), 
      "(c) Brooks Chen");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
     "IDS is a tool for detecting.");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), 
      "http://www.AC_IDS.net");

  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);

}

void show_store_ok_info()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "存储成功", "title");
  gtk_window_set_title(GTK_WINDOW(dialog), "Information");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

//============  login =================================

void login_check()
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

   char ch_login_username[20];
   char ch_login_pwd[20];

   strcpy(ch_login_username, gtk_entry_get_text(GTK_ENTRY(login_name_entry)) );
   strcpy(ch_login_pwd, gtk_entry_get_text(GTK_ENTRY(login_pwd_entry)) );

   my_username = ch_login_username;
   string login_pwd = ch_login_pwd;

   string query_sql="select * from manager where username ='"+ my_username+"' and pwd = '"+ login_pwd+"';";

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
    //pwd is wrong
     show_login_error_warning();
   }

   else //ok
   {
     //pwd is right
     gtk_widget_destroy(login_window);
     make_main_window();
   }

}

//================== login window =========================
void make_login_window()
{
  
  GtkWidget *login_frame;

  login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    /*设置窗口标题*/
  gtk_window_set_title(GTK_WINDOW(login_window), "Login");
    /*设置窗口默认大小*/
  gtk_window_set_default_size(GTK_WINDOW(login_window), 300, 190);

  gtk_window_set_position(GTK_WINDOW(login_window), GTK_WIN_POS_CENTER);

  
  login_frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(login_window), login_frame);

    //--------------   input ----------------------------------------------
  GtkWidget *name_label;
  name_label = gtk_label_new("用户名:");
  gtk_fixed_put(GTK_FIXED(login_frame), name_label, 35, 40);

  GtkWidget *pwd_label;
  pwd_label = gtk_label_new("密 码:");
  gtk_fixed_put(GTK_FIXED(login_frame), pwd_label, 35, 90);

      //GtkWidget *name_label;
  login_name_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(login_frame), login_name_entry, 100, 40);

     //GtkWidget *pwd_label;
  login_pwd_entry = gtk_entry_new();
  gtk_fixed_put(GTK_FIXED(login_frame), login_pwd_entry, 100, 90);
  gtk_entry_set_visibility(GTK_ENTRY(login_pwd_entry),FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY(login_pwd_entry), '*');

    //--------------  btn  ----------------------------------------------
  GtkWidget *confirm_change_btn;
  confirm_change_btn = gtk_button_new_with_label("确认");
  gtk_widget_set_size_request(confirm_change_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(login_frame), confirm_change_btn, 50, 150);
  g_signal_connect(confirm_change_btn, "clicked", G_CALLBACK (login_check), NULL);

  GtkWidget *cancel_change_btn;
  cancel_change_btn = gtk_button_new_with_label("取消");
  gtk_widget_set_size_request(cancel_change_btn, 70, 30);
  gtk_fixed_put(GTK_FIXED(login_frame), cancel_change_btn, 180, 150);
  g_signal_connect(cancel_change_btn, "clicked", G_CALLBACK (gtk_main_quit), NULL);

    //------------------------------------------------------------

  gtk_widget_show_all(login_window);

}

void show_login_error_warning()
{
   GtkWidget *dialog;
   dialog = gtk_message_dialog_new(GTK_WINDOW(login_window),
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_WARNING,
    GTK_BUTTONS_OK,
    "密码不正确");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}