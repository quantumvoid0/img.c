#include <gtk/gtk.h>

GdkPixbuf *original_pixbuf = NULL;
GtkWidget *image = NULL;

void on_size_allocate(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data) {
    if (!original_pixbuf) return;

    int win_width = allocation->width;
    int win_height = allocation->height;

    int img_width = gdk_pixbuf_get_width(original_pixbuf);
    int img_height = gdk_pixbuf_get_height(original_pixbuf);

    double scale = MIN((double)win_width / img_width, (double)win_height / img_height);
    int new_width = img_width * scale;
    int new_height = img_height * scale;

    GdkPixbuf *scaled = gdk_pixbuf_scale_simple(original_pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), scaled);
    g_object_unref(scaled);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        g_print("Usage: %s path/to/image\n", argv[0]);
        return 1;
    }

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    image = gtk_image_new();
    gtk_container_add(GTK_CONTAINER(window), image);

    original_pixbuf = gdk_pixbuf_new_from_file(argv[1], NULL);
    if (!original_pixbuf) {
        g_print("Failed to load image.\n");
        return 1;
    }

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "size-allocate", G_CALLBACK(on_size_allocate), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(original_pixbuf);
    return 0;
}
