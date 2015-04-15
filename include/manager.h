#ifndef MANAGER_H
#define MANAGER_H
#include <gtk/gtk.h> 
#include <string>

using namespace std;



//-----------------------------

void ui_open_manager_window();
void manager_show_all();
void manager_open_create_window();
void manager_return_main();
void manager_create_insert_sql();
void manager_close_create_window();
bool manager_query_user(string manager_username);
void manager_open_change_window();
void manager_close_change_window();
void manager_change_insert_sql();
bool manager_query_old_pwd(string old_pwd);

//--------------------------
void show_qr_pwd_warning();
void show_wrong_pwd_warning();
void show_change_pwd_ok_info();
void show_new_pwd_length_warning();
void show_change_wrong_pwd_warning();
void show_change_new_pwd_length_warning();
void show_change_qr_pwd_warning();

extern  string my_username;

#endif // MANAGER_H
