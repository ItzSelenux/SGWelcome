#include <gtk/gtk.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define ML 4096
GtkIconTheme *theme;
GtkIconInfo *info;
GtkWidget *window;

gchar distrotitle[ML], distrologo[ML], mainfolder[ML], config_file_path[ML], current_folder[ML] = "",
data_folder[ML] = "", entry_folder[ML] = "";

gint format, tabposition, showcomment=0, actiondefaultsize=32, windowsizex=640, windowsizey=400;

GtkWidget *wgrid, *notebook;



#include "events.h"

#include "fmanip.h"
#include "settings.h"

#include "mainwindow.h"
