#ifndef INIT_H
#define INIT_H
#include <gtk/gtk.h> 

void init_start_all();
void init_sqltest_start_all();//sql test

extern   GtkWidget *tcpText;
extern   GtkWidget *udpText;
extern   GtkWidget *tcpScrolled;
extern   GtkWidget *udpScrolled;


extern  GtkWidget *tcpCountLabel;
extern  GtkWidget *udpCountLabel;

extern  GtkTextBuffer *tcpBuffer;
extern  GtkTextBuffer *udpBuffer;

extern  GtkTextIter tcpIter;
extern  GtkTextIter udpIter;

#endif // INIT_H
