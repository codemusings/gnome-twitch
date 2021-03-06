#include "utils.h"
#include <stdlib.h>
#include <string.h>

gpointer
g_value_ref_sink_object(const GValue* val)
{
    if (val == NULL || !G_VALUE_HOLDS_OBJECT(val) || g_value_get_object(val) == NULL)
        return NULL;
    else
        return g_object_ref_sink(g_value_get_object(val));
}

gchar*
g_value_dup_string_allow_null(const GValue* val)
{
    if (g_value_get_string(val))
        return g_value_dup_string(val);
}

void
gtk_container_clear(GtkContainer* cont)
{
    for(GList* l = gtk_container_get_children(cont);
        l != NULL; l = l->next)
    {
        gtk_container_remove(cont, GTK_WIDGET(l->data));
    }
}

gchar*
strrpl(gchar *str, gchar *orig, gchar *rep)
{
  gchar buffer[4096];
  gchar *p;

  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return g_strdup(buffer);
}

void
utils_pixbuf_scale_simple(GdkPixbuf** pixbuf, gint width, gint height, GdkInterpType interp)
{
    if (!*pixbuf)
        return;

    GdkPixbuf* tmp = gdk_pixbuf_scale_simple(*pixbuf, width, height, interp);
    g_clear_object(pixbuf);
    *pixbuf = tmp;
}

GdkPixbuf*
utils_download_picture(SoupSession* soup, const gchar* url)
{
    SoupMessage* msg;
    GdkPixbuf* ret;
    GInputStream* input;

    msg = soup_message_new("GET", url);
    input = soup_session_send(soup, msg, NULL, NULL);

    ret = gdk_pixbuf_new_from_stream(input, NULL, NULL);

    g_input_stream_close(input, NULL, NULL);
    g_object_unref(msg);

    return ret;
}
