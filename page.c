/* See LICENSE file for license and copyright information */

#include <girara/session.h>
#include <girara/utils.h>
#include <glib/gi18n.h>

#include "document.h"
#include "page.h"
#include "plugin.h"
#include "utils.h"
#include "internal.h"
#include "types.h"

struct zathura_page_s {
  double height; /**< Page height */
  double width; /**< Page width */
  unsigned int index; /**< Page number */
  void* data; /**< Custom data */
  bool visible; /**< Page is visible */
  zathura_document_t* document; /**< Document */
};

zathura_page_t*
zathura_page_new(zathura_document_t* document, unsigned int index, zathura_error_t* error)
{
  if (document == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    goto error_ret;
  }

  /* init page */
  zathura_page_t* page = g_malloc0(sizeof(zathura_page_t));

  page->index    = index;
  page->visible  = false;
  page->document = document;

  /* init plugin */
  zathura_plugin_t* plugin = zathura_document_get_plugin(document);
  if (plugin->functions.page_init == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    goto error_ret;
  }

  zathura_error_t ret = plugin->functions.page_init(page);
  if (ret != ZATHURA_ERROR_OK) {
    if (error != NULL) {
      *error = ret;
    }
    goto error_free;
  }

  return page;

error_free:

  if (page != NULL) {
    zathura_page_free(page);
  }

error_ret:

  return NULL;
}

zathura_error_t
zathura_page_free(zathura_page_t* page)
{
  if (page == NULL) {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  if (page->document == NULL) {
    g_free(page);
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_clear == NULL) {
    return ZATHURA_ERROR_NOT_IMPLEMENTED;
  }

  zathura_error_t error = plugin->functions.page_clear(page, page->data);

  g_free(page);

  return error;
}

zathura_document_t*
zathura_page_get_document(zathura_page_t* page)
{
  if (page == NULL) {
    return NULL;
  }

  return page->document;
}

unsigned int
zathura_page_get_index(zathura_page_t* page)
{
  if (page == NULL) {
    return 0;
  }

  return page->index;
}

double
zathura_page_get_width(zathura_page_t* page)
{
  if (page == NULL) {
    return -1;
  }

  return page->width;
}

void
zathura_page_set_width(zathura_page_t* page, double width)
{
  if (page == NULL) {
    return;
  }

  page->width = width;
}

double
zathura_page_get_height(zathura_page_t* page)
{
  if (page == NULL) {
    return -1;
  }

  return page->height;
}

void
zathura_page_set_height(zathura_page_t* page, double height)
{
  if (page == NULL) {
    return;
  }

  page->height = height;
}

bool
zathura_page_get_visibility(zathura_page_t* page)
{
  if (page == NULL) {
    return false;
  }

  return page->visible;
}

void
zathura_page_set_visibility(zathura_page_t* page, bool visibility)
{
  if (page == NULL) {
    return;
  }

  page->visible = visibility;
}

void*
zathura_page_get_data(zathura_page_t* page)
{
  if (page == NULL) {
    return NULL;
  }

  return page->data;
}

void
zathura_page_set_data(zathura_page_t* page, void* data)
{
  if (page == NULL) {
    return;
  }

  page->data = data;
}

girara_list_t*
zathura_page_search_text(zathura_page_t* page, const char* text, zathura_error_t* error)
{
  if (page == NULL || page->document == NULL || text == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    return NULL;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_search_text == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    return NULL;
  }

  return plugin->functions.page_search_text(page, page->data, text, error);
}

girara_list_t*
zathura_page_links_get(zathura_page_t* page, zathura_error_t* error)
{
  if (page == NULL || page->document == NULL ) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    return NULL;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_links_get == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    return NULL;
  }

  return plugin->functions.page_links_get(page, page->data, error);
}

zathura_error_t
zathura_page_links_free(girara_list_t* UNUSED(list))
{
  return false;
}

girara_list_t*
zathura_page_form_fields_get(zathura_page_t* page, zathura_error_t* error)
{
  if (page == NULL || page->document == NULL ) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    return NULL;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_form_fields_get == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    return NULL;
  }

  return plugin->functions.page_form_fields_get(page, page->data, error);
}

zathura_error_t
zathura_page_form_fields_free(girara_list_t* UNUSED(list))
{
  return ZATHURA_ERROR_NOT_IMPLEMENTED;
}

girara_list_t*
zathura_page_images_get(zathura_page_t* page, zathura_error_t* error)
{
  if (page == NULL || page->document == NULL ) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    return NULL;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_images_get == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    return NULL;
  }

  return plugin->functions.page_images_get(page, page->data, error);
}

cairo_surface_t*
zathura_page_image_get_cairo(zathura_page_t* page, zathura_image_t* image, zathura_error_t* error)
{
  if (page == NULL || page->document == NULL  || image == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    return NULL;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_image_get_cairo == NULL) {
    if (error != NULL) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    return NULL;
  }

  return plugin->functions.page_image_get_cairo(page, page->data, image, error);
}

char*
zathura_page_get_text(zathura_page_t* page, zathura_rectangle_t rectangle, zathura_error_t* error)
{
  if (page == NULL || page->document == NULL ) {
    if (error) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    return NULL;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_get_text == NULL) {
    if (error) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    return NULL;
  }

  return plugin->functions.page_get_text(page, page->data, rectangle, error);
}

girara_list_t*
zathura_page_get_annotations(zathura_page_t* page, zathura_error_t* error)
{
  if (page == NULL || page->document == NULL ) {
    if (error) {
      *error = ZATHURA_ERROR_INVALID_ARGUMENTS;
    }
    return NULL;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_get_annotations == NULL) {
    if (error) {
      *error = ZATHURA_ERROR_NOT_IMPLEMENTED;
    }
    return NULL;
  }

  girara_list_t* result = plugin->functions.page_get_annotations(page, page->data, error);
  if (result != NULL) {
    girara_list_set_free_function(result, (girara_free_function_t) zathura_annotation_free);
  }

  return result;
}

zathura_error_t
zathura_page_render(zathura_page_t* page, cairo_t* cairo, bool printing)
{
  if (page == NULL  || page->document == NULL || cairo == NULL) {
    return ZATHURA_ERROR_INVALID_ARGUMENTS;
  }

  zathura_plugin_t* plugin = zathura_document_get_plugin(page->document);
  if (plugin->functions.page_render_cairo == NULL) {
    return ZATHURA_ERROR_NOT_IMPLEMENTED;
  }

  return plugin->functions.page_render_cairo(page, page->data, cairo, printing);
}
