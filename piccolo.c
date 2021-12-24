/*
* piccolo.c
* Very Small Text Editor
* Linux GTK3
* Michael Leidel
* July 2021
* install: piccolo_1.5_amd64.deb
  ____  _               _
 |  _ \(_) ___ ___ ___ | | ___
 | |_) | |/ __/ __/ _ \| |/ _ \
 |  __/| | (_| (_| (_) | | (_) |
 |_|   |_|\___\___\___/|_|\___/

*/
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "myclib.h"
#include "gtkcss.h"

#define GDK_KEY_F2 0xffbf
#define GDK_KEY_F3 0xffc0

char* glade_xml = "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!-- Generated with glade 3.22.2 -->\n\
<interface>\n\
<requires lib=\"gtk+\" version=\"3.20\"/>\n\
<object class=\"GtkAboutDialog\" id=\"dlg_about\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"type_hint\">dialog</property>\n\
<property name=\"program_name\">Piccolo Small Text Editor</property>\n\
<property name=\"version\">1.5</property>\n\
<property name=\"comments\" translatable=\"yes\">Piccolo is a very light weight\n\
and limited text editor</property>\n\
<property name=\"website\">https://github.com/MLeidel/Piccolo</property>\n\
<property name=\"website_label\" translatable=\"yes\">Website and Hot Keys</property>\n\
<property name=\"authors\">Michael Leidel</property>\n\
<property name=\"logo_icon_name\">accessories-text-editor</property>\n\
<property name=\"license_type\">gpl-2-0</property>\n\
<signal name=\"delete-event\" handler=\"on_dlg_about_delete_event\" swapped=\"no\"/>\n\
<signal name=\"response\" handler=\"on_dlg_about_response\" swapped=\"no\"/>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child internal-child=\"vbox\">\n\
<object class=\"GtkBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"orientation\">vertical</property>\n\
<property name=\"spacing\">2</property>\n\
<child internal-child=\"action_area\">\n\
<object class=\"GtkButtonBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"layout_style\">end</property>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">False</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<placeholder/>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<object class=\"GtkDialog\" id=\"dlg_search\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"title\" translatable=\"yes\">Piccolo Search</property>\n\
<property name=\"resizable\">False</property>\n\
<property name=\"default_width\">300</property>\n\
<property name=\"default_height\">100</property>\n\
<property name=\"type_hint\">dialog</property>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child internal-child=\"vbox\">\n\
<object class=\"GtkBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"orientation\">vertical</property>\n\
<property name=\"spacing\">2</property>\n\
<child internal-child=\"action_area\">\n\
<object class=\"GtkButtonBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"layout_style\">end</property>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_search_cancel\">\n\
<property name=\"label\">gtk-cancel</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_search_cancel_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_search_ok\">\n\
<property name=\"label\">gtk-ok</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_search_ok_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">False</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<object class=\"GtkEntry\" id=\"entry_search\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"primary_icon_tooltip_text\" translatable=\"yes\">Begin a search</property>\n\
<property name=\"placeholder_text\" translatable=\"yes\">enter search item</property>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"padding\">5</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<object class=\"GtkFileChooserDialog\" id=\"fcdlg\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"type_hint\">dialog</property>\n\
<property name=\"skip_taskbar_hint\">True</property>\n\
<property name=\"deletable\">False</property>\n\
<property name=\"has_resize_grip\">True</property>\n\
<signal name=\"delete-event\" handler=\"on_fcdlg_delete_event\" swapped=\"no\"/>\n\
<signal name=\"file-activated\" handler=\"on_fcdlg_file_activated\" swapped=\"no\"/>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child internal-child=\"vbox\">\n\
<object class=\"GtkBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"orientation\">vertical</property>\n\
<property name=\"spacing\">2</property>\n\
<child internal-child=\"action_area\">\n\
<object class=\"GtkButtonBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"layout_style\">end</property>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_fc_cancel\">\n\
<property name=\"label\">gtk-cancel</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_fc_cancel_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_fc_ok\">\n\
<property name=\"label\">gtk-ok</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_fc_ok_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">False</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<placeholder/>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<object class=\"GtkMessageDialog\" id=\"msgdlg\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"resizable\">False</property>\n\
<property name=\"type_hint\">dialog</property>\n\
<property name=\"message_type\">warning</property>\n\
<property name=\"text\" translatable=\"yes\">Save before exiting ?</property>\n\
<property name=\"secondary_text\" translatable=\"yes\">Text has been modified</property>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child internal-child=\"vbox\">\n\
<object class=\"GtkBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"orientation\">vertical</property>\n\
<property name=\"spacing\">2</property>\n\
<child internal-child=\"action_area\">\n\
<object class=\"GtkButtonBox\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"homogeneous\">True</property>\n\
<property name=\"layout_style\">end</property>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_msgdlg_ok\">\n\
<property name=\"label\">gtk-ok</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_msgdlg_ok_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<object class=\"GtkButton\" id=\"btn_msgdlg_cancel\">\n\
<property name=\"label\">gtk-cancel</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"receives_default\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<property name=\"always_show_image\">True</property>\n\
<signal name=\"clicked\" handler=\"on_btn_msgdlg_cancel_clicked\" swapped=\"no\"/>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">False</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<object class=\"GtkTextBuffer\" id=\"textbuff\">\n\
<signal name=\"modified-changed\" handler=\"on_textbuff_modified_changed\" swapped=\"no\"/>\n\
</object>\n\
<object class=\"GtkWindow\" id=\"window1\">\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"title\" translatable=\"yes\">Piccolo</property>\n\
<signal name=\"delete-event\" handler=\"on_window1_delete_event\" swapped=\"no\"/>\n\
<signal name=\"key-press-event\" handler=\"on_window1_key_press_event\" swapped=\"no\"/>\n\
<child type=\"titlebar\">\n\
<placeholder/>\n\
</child>\n\
<child>\n\
<object class=\"GtkBox\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"orientation\">vertical</property>\n\
<child>\n\
<object class=\"GtkMenuBar\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<child>\n\
<object class=\"GtkMenuItem\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"label\" translatable=\"yes\">_File</property>\n\
<property name=\"use_underline\">True</property>\n\
<child type=\"submenu\">\n\
<object class=\"GtkMenu\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_new\">\n\
<property name=\"label\">gtk-new</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<signal name=\"activate\" handler=\"on_mu_new_activate\" swapped=\"no\"/>\n\
<accelerator key=\"n\" signal=\"activate\" modifiers=\"GDK_CONTROL_MASK\"/>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_open\">\n\
<property name=\"label\">gtk-open</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<signal name=\"activate\" handler=\"on_mu_open_activate\" swapped=\"no\"/>\n\
<accelerator key=\"o\" signal=\"activate\" modifiers=\"GDK_CONTROL_MASK\"/>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_save\">\n\
<property name=\"label\">gtk-save</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<signal name=\"activate\" handler=\"on_mu_save_activate\" swapped=\"no\"/>\n\
<accelerator key=\"s\" signal=\"activate\" modifiers=\"GDK_CONTROL_MASK\"/>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_saveas\">\n\
<property name=\"label\">gtk-save-as</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<signal name=\"activate\" handler=\"on_mu_saveas_activate\" swapped=\"no\"/>\n\
<accelerator key=\"s\" signal=\"activate\" modifiers=\"GDK_SHIFT_MASK | GDK_CONTROL_MASK\"/>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_find\">\n\
<property name=\"label\">gtk-find</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<signal name=\"activate\" handler=\"on_mu_find_activate\" swapped=\"no\"/>\n\
<accelerator key=\"F2\" signal=\"activate\"/>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_next\">\n\
<property name=\"label\">Find Next</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">False</property>\n\
<signal name=\"activate\" handler=\"on_mu_next_activate\" swapped=\"no\"/>\n\
<accelerator key=\"F3\" signal=\"activate\"/>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkSeparatorMenuItem\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_quit\">\n\
<property name=\"label\">gtk-quit</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<signal name=\"activate\" handler=\"on_mu_quit_activate\" swapped=\"no\"/>\n\
<accelerator key=\"q\" signal=\"activate\" modifiers=\"GDK_CONTROL_MASK\"/>\n\
</object>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
</child>\n\
<child>\n\
<object class=\"GtkMenuItem\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"label\" translatable=\"yes\">_Help</property>\n\
<property name=\"use_underline\">True</property>\n\
<child type=\"submenu\">\n\
<object class=\"GtkMenu\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<child>\n\
<object class=\"GtkImageMenuItem\" id=\"mu_about\">\n\
<property name=\"label\">gtk-about</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">False</property>\n\
<property name=\"use_underline\">True</property>\n\
<property name=\"use_stock\">True</property>\n\
<signal name=\"activate\" handler=\"on_mu_about_activate\" swapped=\"no\"/>\n\
</object>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">False</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"position\">0</property>\n\
</packing>\n\
</child>\n\
<child>\n\
<object class=\"GtkScrolledWindow\">\n\
<property name=\"width_request\">400</property>\n\
<property name=\"height_request\">275</property>\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"border_width\">2</property>\n\
<property name=\"shadow_type\">in</property>\n\
<property name=\"propagate_natural_width\">True</property>\n\
<property name=\"propagate_natural_height\">True</property>\n\
<child>\n\
<object class=\"GtkTextView\" id=\"textview\">\n\
<property name=\"visible\">True</property>\n\
<property name=\"can_focus\">True</property>\n\
<property name=\"pixels_above_lines\">1</property>\n\
<property name=\"left_margin\">8</property>\n\
<property name=\"right_margin\">4</property>\n\
<property name=\"top_margin\">4</property>\n\
<property name=\"bottom_margin\">4</property>\n\
<property name=\"buffer\">textbuff</property>\n\
<property name=\"monospace\">True</property>\n\
<style><class name=\"txtv\"/></style>\n\
</object>\n\
</child>\n\
</object>\n\
<packing>\n\
<property name=\"expand\">True</property>\n\
<property name=\"fill\">True</property>\n\
<property name=\"padding\">1</property>\n\
<property name=\"position\">1</property>\n\
</packing>\n\
</child>\n\
</object>\n\
</child>\n\
</object>\n\
</interface>\n\
\0";


// GtkWidget Pointers go here
// GtkWidget *g_;
GtkWidget *g_fc_dlg;
GtkWidget *g_txbuf;
GtkWidget *g_tview;
GtkWidget *g_window;
GtkWidget *g_about;
GtkWidget *g_msgdlg;
GtkWidget *g_dlg_search;
GtkWidget *g_entry_search;

FILE *fh;
char fbuf[512] = "\0";  // file name
char tbuf[512] = "\0";  // window title
char line[2048] = {0};
int fc_action = 2;  // 0=open, 1=Save As, 2=New(Save As), 3=command line
char winmet_path[128] = "\0";
char last_file[128] = "\0";
int modified = 0;
// for search
GtkTextIter start, end;
gboolean first = TRUE;
gchar sea_target[128] = {0};


/*
 ___  ___       ___       __   __   __
|   \/   |     /   \     |  | |  \ |  |
|  \  /  |    /  ^  \    |  | |   \|  |
|  |\/|  |   /  /_\  \   |  | |  . `  |
|  |  |  |  /  _____  \  |  | |  |\   |
|__|  |__| /__/     \__\ |__| |__| \__|

*/

int main(int argc, char *argv[]) {
    GtkBuilder      *builder;
    GtkWidget       *window1;
    gint             w_top;
    gint             w_left;
    gint             w_width;
    gint             w_height;

    gtk_init(&argc, &argv);

    sprintf(winmet_path, "/usr/local/piccolo/winmet.txt");
    sprintf(last_file, "/usr/local/piccolo/lastfile.txt");

    builder = gtk_builder_new_from_string (glade_xml, -1);

    window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    gtk_builder_connect_signals(builder, NULL);

    // set global GtkWidget Pointers to their builder objects
    // g_ = GTK_WIDGET(gtk_builder_get_object(builder, "WIDGET_ID"));
    g_fc_dlg = GTK_WIDGET(gtk_builder_get_object(builder, "fcdlg"));
    g_msgdlg = GTK_WIDGET(gtk_builder_get_object(builder, "msgdlg"));
    g_txbuf = GTK_WIDGET(gtk_builder_get_object(builder, "textbuff"));
    g_tview = GTK_WIDGET(gtk_builder_get_object(builder, "textview"));
    g_window = window1;
    g_about = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_about"));
    g_dlg_search = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_search"));
    g_entry_search = GTK_WIDGET(gtk_builder_get_object(builder, "entry_search"));

    g_object_unref(builder);

    // load the window metrics
    char line[64];
    fh = open_for_read(winmet_path);
    fgets(line, 64, fh);
    fclose(fh);
    removen(line);  // remove new line character
    fields(line, ",");
    w_left      = atol(_fields[0]);
    w_top       = atol(_fields[1]);
    w_width     = atol(_fields[2]);
    w_height    = atol(_fields[3]);

    gtk_widget_show(window1);
    gtk_window_move(GTK_WINDOW(window1), w_left, w_top);  // set metrics ...
    gtk_window_resize(GTK_WINDOW(window1), w_width, w_height);

    // either read file from command line or open the last_file

    if (argc > 1) {  // load file from command line
        //strcpy(fbuf, argv[1]);
        realpath(argv[1], fbuf);
    } else {
        if (file_exists(last_file)) {
            fh = open_for_read(last_file);
            fgets(fbuf, 512, fh);
            fclose(fh);
        }
    }
    if (fbuf != NULL) {
      // read the file and insert it into the textview
      if (file_exists(fbuf)) {
        fh = open_for_read(fbuf);
        fseek(fh, 0, SEEK_END);
        long fsize = ftell(fh);
        fseek(fh, 0, SEEK_SET);
        char *string = malloc(fsize + 1);
        fread(string, 1, fsize, fh);
        fclose(fh);
        string[fsize] = 0;
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_txbuf), string, -1);
        free(string);
      }
      gtk_window_set_title(GTK_WINDOW(g_window), fbuf);
      strcpy(tbuf, fbuf);
      fc_action = 3;
      gtk_text_buffer_set_modified (GTK_TEXT_BUFFER(g_txbuf) , FALSE);
      modified = 0;
    }

    apply_css_text(".txtv {font: bold 11pt 'Andale Mono';}");  // from gtkcss.h

    gtk_main();
    return 0;
}





void save_last_file() {
    fh = open_for_write(last_file);
    fprintf(fh, "%s", fbuf);
    fclose(fh);
}

// called when window is closed
void on_window1_delete_event() {
    FILE *fh;
    gint w_top;
    gint w_left;
    gint w_width;
    gint w_height;

    gtk_window_get_position (GTK_WINDOW(g_window), &w_left, &w_top);
    gtk_window_get_size (GTK_WINDOW(g_window), &w_width, &w_height);
    fh = open_for_write(winmet_path);
    fprintf(fh, "%d,%d,%d,%d\n", w_left, w_top, w_width, w_height);
    fclose(fh);

    if (modified == 1) { // incase called by Main Window Close Button
        gtk_dialog_run(GTK_DIALOG(g_msgdlg));
    }

    gtk_main_quit();
}

// User selected "Quit" from menu
void on_mu_quit_activate() {
    if (modified == 1) {
        gtk_dialog_run(GTK_DIALOG(g_msgdlg));
    }
    on_window1_delete_event();
}


// Clear Editor contents and set to New(Save As)
void on_mu_new_activate() {
    if (modified == 1) {
        gtk_dialog_run(GTK_DIALOG(g_msgdlg));
    }
    fc_action = 2;
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_txbuf), "", -1);
    gtk_window_set_title(GTK_WINDOW(g_window), "Piccolo");
    strcpy(tbuf, fbuf);
}


// Select a file to open
void on_mu_open_activate() {
    if (modified == 1) {
        // offer to save altered text first
        gtk_dialog_run(GTK_DIALOG(g_msgdlg));
    }
    fc_action = 0;
    gtk_dialog_run(GTK_DIALOG(g_fc_dlg));
}


// Just close the FileChooserDialog
void on_btn_fc_cancel_clicked() {
    gtk_widget_hide(g_fc_dlg);
}


// Signal the dialog to Hide not Destroy
void on_fcdlg_delete_event() {
    gtk_widget_hide_on_delete(g_fc_dlg);
}

void on_dlg_about_delete_event() {
    gtk_widget_hide_on_delete(g_about);
}

// Show FileChooserDialog for Save As
void on_mu_saveas_activate() {
    fc_action = 1;  // signal activate handler to process "saveas"
    gtk_file_chooser_set_action (GTK_FILE_CHOOSER(g_fc_dlg),
                                 GTK_FILE_CHOOSER_ACTION_SAVE);
    gtk_dialog_run(GTK_DIALOG(g_fc_dlg));
}


// Save the current editor content
void on_mu_save_activate() {
    if (fc_action == 2) {
        on_mu_saveas_activate();  // save after "New"
        return;
    }

    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(g_tview));
    gchar *text;

    gtk_text_buffer_get_bounds (buffer, &start, &end);
    text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
    //printf("%s\n", text);
    fh = open_for_write(fbuf);
    fprintf(fh, "%s", text);
    fclose(fh);
    g_free(text);
    gtk_text_buffer_set_modified (buffer , FALSE);
    modified = 0;
    save_last_file();
}


// User done with filechooser, now process user's request
void on_btn_fc_ok_clicked() {

    strcpy(fbuf, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_fc_dlg)));

    if (fbuf == NULL) {
        gtk_widget_hide(g_fc_dlg);  // user cancelled filechooser
        return;
    }

    gtk_window_set_title(GTK_WINDOW(g_window), fbuf);
    strcpy(tbuf, fbuf);

    if (fc_action == 0) {
        // read the file and insert it into the textview
        fh = open_for_read(fbuf);
        fseek(fh, 0, SEEK_END);
        long fsize = ftell(fh);
        fseek(fh, 0, SEEK_SET);
        char *string = malloc(fsize + 1);
        fread(string, 1, fsize, fh);
        fclose(fh);
        string[fsize] = 0;
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(g_txbuf), string, -1);
        free(string);
        gtk_text_buffer_set_modified (GTK_TEXT_BUFFER(g_txbuf) , FALSE);
        modified = 0;
    } else {
        // get text from textview and save it to a file
        on_mu_save_activate();
    }
    save_last_file();
    gtk_widget_hide(g_fc_dlg);
}

// User double clicked to the selected file
void on_fcdlg_file_activated() {
    on_btn_fc_ok_clicked();
}

void on_mu_about_activate() {
    gtk_dialog_run(GTK_DIALOG(g_about));
}

void on_dlg_about_response() {
    gtk_widget_hide(g_about);
}

void on_textbuff_modified_changed() {
    // process the asterisk in front of the
    // window title (filename) indicating "unsaved"
    char sfile[128] = {"\0"};

    if (modified == 0) {  // was clean now dirty
      if (tbuf[0] != '*') {
        strcpy(sfile, tbuf);
        strcpy(tbuf, "*");
        strcat(tbuf, sfile);
        modified = 1;
      }
    } else {              // was dirty now clean
      if (tbuf[0] == '*') {
        strcpy(tbuf, tbuf+1);
        modified = 0;
      }
    }
    gtk_window_set_title(GTK_WINDOW(g_window), tbuf);
}

void on_btn_msgdlg_cancel_clicked() {
    gtk_widget_hide(g_msgdlg);
}

void on_btn_msgdlg_ok_clicked() {
    on_mu_save_activate();
    gtk_widget_hide(g_msgdlg);
}

void search() {
    // GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_tview));
    if (first) {
        gtk_text_buffer_get_start_iter (buffer, &start);
        first = FALSE;
    }
    //gtk_text_buffer_get_end_iter (buffer, &end);
    if (gtk_text_iter_forward_search(&start, sea_target,
                                     GTK_TEXT_SEARCH_CASE_INSENSITIVE,
                                     &start, &end, NULL)) {
        gtk_text_buffer_select_range(buffer, &start, &end);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(g_tview),
                                                   &start,
                                                   0.3,
                                                   true,
                                                   0.5,
                                                   0.5);
        start = end;
    } else {
        // start over at begining
        gtk_text_buffer_get_start_iter (buffer, &start);
    }
}

void on_btn_search_ok_clicked() {
    sprintf(sea_target, "%s", gtk_entry_get_text(GTK_ENTRY(g_entry_search)));
    //printf("%s\n", sea_target);
    gtk_widget_hide (g_dlg_search);
    search(sea_target);
}

void on_btn_search_cancel_clicked() {
    gtk_widget_hide (g_dlg_search);
}

void on_mu_find_activate() {
  gtk_widget_show_all(g_dlg_search);
  gtk_dialog_run(GTK_DIALOG(g_dlg_search));
}

void on_mu_next_activate() {
  search();
}

_Bool on_window1_key_press_event(GtkWidget *w, GdkEvent *e) {
    guint keyval;
    guint state;

    gdk_event_get_keyval(e, &keyval);
    gdk_event_get_state (e, &state);

    //printf("%d\n", keyval);

    if (keyval == 117) {  // "u"
        GtkTextIter start, end;
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_tview));
        gchar *text;

        gtk_text_buffer_get_selection_bounds(buffer, &start, &end);

        if (&start != &end) {
            text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
            if (startswith(text, "http")) {  // if action is a website
                strcpy(line, "xdg-open ");
                strcat(line, text);
                system(line);
                return TRUE;
            }
        }
    }

    //if ((state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK && keyval == GDK_KEY_f) {
    if (keyval == GDK_KEY_F2) {
        on_mu_find_activate();
        return TRUE;
    }

    if (keyval == GDK_KEY_F3) {
        search();
        return TRUE;
    }

    return FALSE;
}

