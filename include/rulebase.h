#ifndef RULEBASE_H
#define RULEBASE_H

#include <gtk/gtk.h> 
#include <string>

using namespace std;

void rules_look_not_exist_warning();
void rules_look_id_null_warning();

void rules_delete_not_exist_warning();
void rules_delete_id_null_warning();
void rules_delete_ok_info();
void rules_delete_fail_info();

void rules_no_type_warning();
void rules_no_content_warning();
void rules_no_msg_warning();
void rules_add_ok_info();
bool rules_add_check();
void rules_add_fail_info();
string rules_string_to_hex(char * str, int len);

void rules_modify_id_length_warning();
void rules_modify_id_null_warning();
void rules_modify_not_exist_warning();
void rules_modify_entry_null_warning();
void rules_modify_ok_info();
void rules_modify_fail_info();

//-----------------------------
void rules_modify_query_sql();
void rules_modify_sql();
void rules_close_modify_window();
void rules_open_modify_window();
bool rules_add_check();
void rules_add_sql();
void rules_close_add_window();
void rules_open_add_window();
void rules_delete_sql();
void rules_close_delete_window();
void rules_open_delete_window();
void rules_look_sql();
void rules_close_look_window();
void rules_open_look_window();
void rules_return_main();
void ui_open_rules_window();



#endif // RULEBASE_H
