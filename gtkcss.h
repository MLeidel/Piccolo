#ifndef GTKCSS
#define GTKCSS
/* gtkcss.h */

void apply_css_text(const gchar * css) {
    /* activate css styles from text string
    example:
    apply_css_text(".btn1 {background-color: red;} .entrystyle {color: red;}");
        associate css class names (btn1, entrystyle) with Glade
        "style classes" field in "common tab"
    */
    GdkDisplay *display;
    GdkScreen *screen;
    GtkCssProvider *css_provider;

    css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data (css_provider,
                                     css,
                                    -1,
                                    NULL);
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen,
                                     GTK_STYLE_PROVIDER (css_provider),
                                     GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}


#endif
