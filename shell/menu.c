/*
 * HardInfo
 * Copyright(C) 2003-2007 L. A. F. Pereira.
 *
 * menu.c is based on UI Manager tutorial by Ryan McDougall
 * Copyright(C) 2005 Ryan McDougall.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gtk/gtk.h>
#include <menu.h>
#include <config.h>

#include <stock.h>
#include <iconcache.h>

#include <callbacks.h>
#include <hardinfo.h>


#include "uidefs.h"

static GtkActionEntry entries[] = {
  //    {"MainMenuAction", NULL, ""},
    {"InformationMenuAction", NULL, N_("_Information")},	/* name, stock id, label */
    {"ViewMenuAction", NULL, N_("_View")},
#if GTK_CHECK_VERSION(3, 20, 0)
    {"ThemeMenuAction", NULL, N_("_Theme")},
#endif
    {"HelpMenuAction", NULL, N_("_Help")},
    {"MainMenuBarAction", NULL, ""},

    {"ReportAction", HI_STOCK_REPORT,	/* name, stock id */
     N_("Generate _Report"), "<control>R",	/* label, accelerator */
     N_("Generates a report with detailed system information"),			/* tooltip */
     G_CALLBACK(cb_generate_report)},

    {"SyncManagerAction", HI_STOCK_SYNC,
     N_("Synchronize"), "<control>S",
     N_("Send benchmark results and receive updated data from the network"),
     G_CALLBACK(cb_sync_manager)},

    //does not work correctly and value low
    /*{"CopyAction", HI_STOCK_CLIPBOARD,
     N_("_Copy to Clipboard"), "<control>C",
     N_("Copy to clipboard"),
     G_CALLBACK(cb_copy_to_clipboard)},*/

    {"RefreshAction", HI_STOCK_REFRESH,
     N_("_Refresh"), "F5",
     NULL,
     G_CALLBACK(cb_refresh)},

    {"HomePageAction", NULL,
     N_("_Open HardInfo2 Web Site"), NULL,
     NULL,
     G_CALLBACK(cb_open_web_page)},

    {"ReportBugAction", NULL,
     N_("_Report bug"), NULL,
     NULL,
     G_CALLBACK(cb_report_bug)},

    {"AboutAction", NULL,
     N_("_About HardInfo2"), "<control>A",
     N_("Displays program version information"),
     G_CALLBACK(cb_about)},

    {"QuitAction", NULL,
     N_("_Quit"), "<control>Q",
     NULL,
     G_CALLBACK(cb_quit)}
};

static GtkToggleActionEntry toggle_entries[] = {
    {"SidePaneAction", NULL,
     N_("_Side Pane"), NULL,
     N_("Toggles side pane visibility"),
     G_CALLBACK(cb_side_pane)},
    {"ToolbarAction", NULL,
     N_("_Toolbar"), NULL,
     NULL,
     G_CALLBACK(cb_toolbar)},
    {"SyncOnStartupAction", NULL,
     N_("Synchronize on startup"), NULL,
     NULL,
     G_CALLBACK(cb_sync_on_startup)},
#if GTK_CHECK_VERSION(3, 20, 0)
    {"DisableThemeAction", NULL,
     N_("Disable Theme"), NULL,
     NULL,
     G_CALLBACK(cb_disable_theme)},
    {"Theme1Action", NULL,
     N_("Theme Motherboard"), NULL,
     NULL,
     G_CALLBACK(cb_theme1)},
    {"Theme2Action", NULL,
     N_("Theme Graffiti"), NULL,
     NULL,
     G_CALLBACK(cb_theme2)},
    {"Theme3Action", NULL,
     N_("Theme Anime PC"), NULL,
     NULL,
     G_CALLBACK(cb_theme3)},
    {"Theme4Action", NULL,
     N_("Theme Tux Star"), NULL,
     NULL,
     G_CALLBACK(cb_theme4)},
    {"Theme5Action", NULL,
     N_("Theme PixArt"), NULL,
     NULL,
     G_CALLBACK(cb_theme5)},
    {"Theme6Action", NULL,
     N_("Theme Silicon"), NULL,
     NULL,
     G_CALLBACK(cb_theme6)},
#endif
};

/* Implement a handler for GtkUIManager's "add_widget" signal. The UI manager
 * will emit this signal whenever it needs you to place a new widget it has. */
static void
menu_add_widget(GtkUIManager * ui, GtkWidget * widget,
		GtkContainer * container)
{
    gtk_box_pack_start(GTK_BOX(container), widget, FALSE, FALSE, 0);
    gtk_widget_show(widget);
}

void menu_init(Shell * shell)
{
    GtkWidget *menu_box;	/* Packing box for the menu and toolbars */
    GtkActionGroup *action_group;	/* Packing group for our Actions */
    GtkUIManager *menu_manager;	/* The magic widget! */
    GError *error;		/* For reporting exceptions or errors */
    GtkAccelGroup *accel_group;

    /* Create our objects */
    menu_box = shell->vbox;
    action_group = gtk_action_group_new("HardInfo2");
    menu_manager = gtk_ui_manager_new();

    shell->action_group = action_group;
    shell->ui_manager = menu_manager;

    /* Pack up our objects:
     * menu_box -> window
     * actions -> action_group
     * action_group -> menu_manager */
    gtk_action_group_set_translation_domain( action_group, "hardinfo2" );//gettext
    gtk_action_group_add_actions(action_group, entries,
				 G_N_ELEMENTS(entries), NULL);
    gtk_action_group_add_toggle_actions(action_group, toggle_entries,
					G_N_ELEMENTS(toggle_entries),
					NULL);
    gtk_ui_manager_insert_action_group(menu_manager, action_group, 0);


    /* Read in the UI from our XML file */
    error = NULL;
    gtk_ui_manager_add_ui_from_string(menu_manager, uidefs_str, -1,
				      &error);

    if (error) {
	g_error("Building menus failed: %s", error->message);
	g_error_free(error);
	return;
    }

    /* Enable menu accelerators */
    accel_group = gtk_ui_manager_get_accel_group(menu_manager);
    gtk_window_add_accel_group(GTK_WINDOW(shell->window), accel_group);

    /* Connect up important signals */
    /* This signal is necessary in order to place widgets from the UI manager
     * into the menu_box */
    g_signal_connect(menu_manager, "add_widget",
		     G_CALLBACK(menu_add_widget), menu_box);

    /* Show the window and run the main loop, we're done! */
    gtk_widget_show(menu_box);

    //Scale menubar
    GtkIconSize icon_size=3;//24
    int size=16;
    if(params.scale>=1.5) {size=20;icon_size=5;}//32
    if(params.scale>=2) {size=24;icon_size=6;}//48
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenuBarAction")), icon_size);

    //Scale (menubar)+menu items
    /*GtkToolButton *b;
    b=GTK_TOOL_BUTTON(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenuBarAction/Refresh"));
    GtkWidget *i=icon_cache_get_image_at_size("hardinfo2.svg",2*size,2*size);
    gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(b),"hardinfo2.svg");
    b=GTK_TOOL_BUTTON(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenuBar/Report"));
    gtk_tool_button_set_icon_widget(b,icon_cache_get_image_at_size("report.svg",2*size,2*size));
    b=GTK_TOOL_BUTTON(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenuBar/SyncManager"));
    gtk_tool_button_set_icon_widget(b,icon_cache_get_image_at_size("sync.svg",2*size,2*size));
    */
    GtkImageMenuItem *t;
    t=GTK_IMAGE_MENU_ITEM(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenu/InformationMenu/Report"));
    gtk_image_menu_item_set_image(t,icon_cache_get_image_at_size("report.svg",size,size));
    t=GTK_IMAGE_MENU_ITEM(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenu/InformationMenu/SyncManager"));
    gtk_image_menu_item_set_image(t,icon_cache_get_image_at_size("sync.svg",size,size));
    t=GTK_IMAGE_MENU_ITEM(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenu/InformationMenu/Quit"));
    gtk_image_menu_item_set_image(t,icon_cache_get_image_at_size("close.svg",size,size));
    t=GTK_IMAGE_MENU_ITEM(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenu/ViewMenu/Refresh"));
    gtk_image_menu_item_set_image(t,icon_cache_get_image_at_size("refresh.svg",size,size));
    t=GTK_IMAGE_MENU_ITEM(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenu/HelpMenu/WebPage"));
    gtk_image_menu_item_set_image(t,icon_cache_get_image_at_size("home.svg",size,size));
    t=GTK_IMAGE_MENU_ITEM(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenu/HelpMenu/ReportBug"));
    gtk_image_menu_item_set_image(t,icon_cache_get_image_at_size("report-bug.svg",size,size));
    t=GTK_IMAGE_MENU_ITEM(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenu/HelpMenu/About"));
    gtk_image_menu_item_set_image(t,icon_cache_get_image_at_size("hardinfo2.svg",size,size));

    gtk_toolbar_set_style(GTK_TOOLBAR(gtk_ui_manager_get_widget(shell->ui_manager, "/MainMenuBarAction")),
			  GTK_TOOLBAR_BOTH_HORIZ);
}
