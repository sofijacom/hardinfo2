/*
 *    HardInfo - Displays System Information
 *    Copyright (C) 2003-2007 L. A. F. Pereira <l@tia.mat.br>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, version 2 or later.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */
#include <iconcache.h>
#include <config.h>
#include <hardinfo.h>

static GHashTable *cache = NULL;

void icon_cache_init(void)
{
    if (!cache) {
        DEBUG("initializing icon cache");
	cache = g_hash_table_new(g_str_hash, g_str_equal);
    }
}

GdkPixbuf *icon_cache_get_pixbuf(const gchar * file)
{
    GdkPixbuf *icon;
    gchar *ikey;
   
    if (!cache) icon_cache_init();
   
    ikey = g_strdup_printf("%s" ICON_SUFFIX(DEF_ICON_SIZE), file);
    icon = g_hash_table_lookup(cache, ikey);
   
    if (!icon)
    {
        gchar *path;
      
        path = g_build_filename(params.path_data, "pixmaps", file, NULL);
        icon = gdk_pixbuf_new_from_file_at_size(path, DEF_ICON_SIZE*params.scale, DEF_ICON_SIZE*params.scale, NULL);
        g_hash_table_insert(cache, g_strdup(ikey), icon);
      
        g_free(path);
    }
   
    g_free(ikey);
   
    if (icon) g_object_ref(icon);
   
    return icon;
}

GtkWidget *icon_cache_get_image(const gchar * file)
{
    GdkPixbuf *icon;

    icon = icon_cache_get_pixbuf(file);
    return gtk_image_new_from_pixbuf(icon);
}

GdkPixbuf *icon_cache_get_pixbuf_at_size(const gchar * file, gint wid, gint hei)
{
    GdkPixbuf *icon;
    gchar *ikey;
   
    if (!cache) icon_cache_init();
   
    ikey = g_strdup_printf("%s@%ix%i", file, wid, hei);
    icon = g_hash_table_lookup(cache, ikey);
   
    if (!icon)
    {
        gchar *path;
      
        path = g_build_filename(params.path_data, "pixmaps", file, NULL);
        icon = gdk_pixbuf_new_from_file_at_size(path, wid==-1?wid:wid*params.scale, hei==-1?hei:hei*params.scale, NULL);
        g_hash_table_insert(cache, g_strdup(ikey), icon);
      
        g_free(path);
    }
   
    g_free(ikey);
   
    if (icon) g_object_ref(icon);

    return icon;
}

GtkWidget *icon_cache_get_image_at_size(const gchar * file, gint wid,
					gint hei)
{
    GdkPixbuf *icon;

    icon = icon_cache_get_pixbuf_at_size(file, wid, hei);
    return gtk_image_new_from_pixbuf(icon);
}
