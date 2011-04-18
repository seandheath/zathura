/* See LICENSE file for license and copyright information */

#include <callbacks.h>
#include <girara.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "zathura.h"
#include "render.h"
#include "document.h"

gboolean
cb_destroy(GtkWidget* widget, gpointer data)
{
  zathura_free(data);

  return TRUE;
}

void
buffer_changed(girara_session_t* session)
{
  g_return_if_fail(session != NULL);
  g_return_if_fail(session->global.data != NULL);

  zathura_t* zathura = session->global.data;

  char* buffer = girara_buffer_get(session);

  if (buffer) {
    girara_statusbar_item_set_text(session, zathura->ui.statusbar.buffer, buffer);
    free(buffer);
  } else {
    girara_statusbar_item_set_text(session, zathura->ui.statusbar.buffer, "");
  }
}

void
cb_view_vadjustment_value_changed(GtkAdjustment *adjustment, gpointer data)
{
  g_return_if_fail(data != NULL);

  zathura_t* zathura = data;
  if (!zathura->document || !zathura->document->pages || !zathura->ui.page_view) {
    return;
  }


  // FIXME
  /* get current adjustment values */
  /*gdouble lower = gtk_adjustment_get_value(adjustment);*/
  /*gdouble upper = lower + gtk_adjustment_get_page_size(adjustment);*/

  /* find page that fits */
  for (unsigned int page_id = 0; page_id < zathura->document->number_of_pages; page_id++)
  {
    zathura_page_t* page = zathura->document->pages[page_id];

    /* check for rendered attribute */
    if (page->rendered) {
      continue;
    }

    /*double begin = page->offset;*/
    /*double end   = page->offset + page->height;*/

    /*if (    ( (begin >= lower) && (end <= upper) ) [> page is in viewport <]*/
        /*|| ( (begin <= lower) && (end >= lower) && (end <= upper) ) [> end of the page is in viewport <]*/
        /*|| ( (begin >= lower) && (end >= upper) && (begin <= upper) ) [> begin of the page is in viewport <]*/
      /*) {*/
    if (page_id < 1) {
        render_page(zathura->sync.render_thread, zathura->document->pages[page_id]);
    }
    /*}*/
  }
}
