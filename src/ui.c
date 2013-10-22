/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ui.c
 * Copyright (C) 2013 Jente Hidskes <jthidskes@outlook.com>
 *
 * Gcolor3 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gcolor3 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "gcolor3.h"
#include "callbacks.h"
#include "ui.h"

void show_file_error (gchar *file, gchar *type) {
	GtkDialog *error_dialog;

	error_dialog = GTK_DIALOG (gtk_message_dialog_new (GTK_WINDOW (window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, _("An error occurred trying to open file \"%s\" for %s access!\n\nPlease check the file permissions and try again."), file, type));
	gtk_dialog_run (error_dialog);
	gtk_widget_destroy (GTK_WIDGET (error_dialog));
}

void about_dialog_close (GtkWidget *about_dialog) {
	gtk_widget_destroy (GTK_WIDGET (about_dialog));
}

void about_dialog_open (void) {
	GtkWidget *about_dialog;
	gchar *license_trans;

	const gchar *authors[] = { "Jente Hidskes", NULL };
	const gchar *artists[] = { "HHH", NULL };
	const gchar *contributors[] = { "Sujoy Kumar Das", NULL };
	const gchar *license[] = {
		N_("Gcolor3 is free software: you can redistribute it and/or modify "
		   "it under the terms of the GNU General Public License as published by "
		   "the Free Software Foundation, either version 3 of the License, or "
		   "(at your option) any later version."),
		N_("Gcolor3 is distributed in the hope that it will be useful "
		   "but WITHOUT ANY WARRANTY; without even the implied warranty of "
		   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
		   "GNU General Public License for more details."),
		N_("You should have received a copy of the GNU General Public License "
		   "along with this program. If not, see http://www.gnu.org/licenses/.")
	};
	license_trans = g_strjoin ("\n\n", _(license[0]), _(license[1]), _(license[2]), NULL);

	about_dialog = gtk_about_dialog_new ();
	gtk_window_set_transient_for (GTK_WINDOW (about_dialog), GTK_WINDOW (window));
	gtk_window_set_modal (GTK_WINDOW (about_dialog), TRUE);

	gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about_dialog), "Gcolor3");
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (about_dialog), "1.2");
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (about_dialog), _("A simple color selection dialog in GTK3")),
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (about_dialog), "Copyright \xc2\xa9 2013 Jente Hidskes");
	gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (about_dialog), license_trans);
	gtk_about_dialog_set_wrap_license (GTK_ABOUT_DIALOG (about_dialog), TRUE);
	gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about_dialog), authors);
	gtk_about_dialog_set_artists (GTK_ABOUT_DIALOG (about_dialog), artists);
	gtk_about_dialog_set_translator_credits (GTK_ABOUT_DIALOG (about_dialog), _("translator-credits"));
	gtk_about_dialog_add_credit_section (GTK_ABOUT_DIALOG (about_dialog), _("Contributors"), contributors);
	gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (about_dialog), _("Website"));
	gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (about_dialog), "http://unia.github.io/gcolor3");
	gtk_about_dialog_set_logo_icon_name (GTK_ABOUT_DIALOG (about_dialog), "gcolor3");

	g_signal_connect (GTK_DIALOG (about_dialog), "response", G_CALLBACK (about_dialog_close), NULL);

	gtk_widget_show (about_dialog);

	g_free(license_trans);
}

GtkWidget *save_dialog_open (void) {
	GtkWidget *content_area, *save_dialog_cancel_button;

	save_dialog = gtk_dialog_new ();
	gtk_container_set_border_width (GTK_CONTAINER (save_dialog), 5);
	gtk_window_set_resizable (GTK_WINDOW (save_dialog), FALSE);
	gtk_window_set_title (GTK_WINDOW (save_dialog), _("Save a color")); /*ToDo: still needed?*/
	gtk_window_set_modal (GTK_WINDOW (save_dialog), TRUE);

	content_area = gtk_dialog_get_content_area (GTK_DIALOG (save_dialog));

	save_label = gtk_label_new ("");
	gtk_widget_show (save_label);
	gtk_label_set_justify (GTK_LABEL (save_label), GTK_JUSTIFY_CENTER);
	gtk_label_set_line_wrap (GTK_LABEL (save_label), TRUE);

	save_entry = gtk_entry_new ();
	gtk_widget_show (save_entry);
	gtk_entry_set_activates_default (GTK_ENTRY (save_entry), TRUE);

	save_dialog_ok_button = gtk_button_new_with_label (_("Save"));
	gtk_widget_show (save_dialog_ok_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (save_dialog), save_dialog_ok_button, GTK_RESPONSE_OK);
	gtk_widget_set_sensitive (save_dialog_ok_button, FALSE);
	gtk_widget_set_can_default (save_dialog_ok_button, TRUE);

	save_dialog_cancel_button = gtk_button_new_with_label (_("Cancel"));
	gtk_widget_show (save_dialog_cancel_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (save_dialog), save_dialog_cancel_button, GTK_RESPONSE_CANCEL);

	g_signal_connect ((gpointer) save_entry, "changed", G_CALLBACK (on_save_entry_changed), NULL);

	gtk_container_add (GTK_CONTAINER (content_area), save_label);
	gtk_container_add (GTK_CONTAINER (content_area), save_entry);

	gtk_widget_grab_focus (save_entry);
	gtk_widget_grab_default (save_dialog_ok_button);

	return save_dialog;
}

GtkWidget *create_window (void) {
	GtkWidget *box_all, *header_bar, *stackswitcher, *box_color_chooser, *scroll, *button_about, *image_about;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Gcolor3"); /*ToDo: still needed?*/
	gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	box_all = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_set_border_width (GTK_CONTAINER (box_all), 5);

	header_bar = gtk_header_bar_new ();
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);

	stack = gtk_stack_new ();
	gtk_stack_set_transition_type (GTK_STACK (stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	stackswitcher = gtk_stack_switcher_new ();
	gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (stackswitcher), GTK_STACK (stack));

	box_color_chooser = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	color_chooser = gtk_color_selection_new ();
	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_vexpand (scroll, TRUE);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scroll), GTK_SHADOW_IN);

	button_save = gtk_button_new ();
	button_about = gtk_button_new ();
	image_about = gtk_image_new_from_icon_name ("help-about", GTK_ICON_SIZE_MENU);
	gtk_button_set_image (GTK_BUTTON (button_about), image_about);

	liststore = gtk_list_store_new (N_COLUMNS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL (liststore));
	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree), COLOR_NAME);
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);

	column = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (column, _("Color"));
	renderer = gtk_cell_renderer_pixbuf_new ();
	gtk_tree_view_column_pack_start (column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column, renderer, "pixbuf", COLOR, NULL);
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_set_attributes (column, renderer, "text", COLOR_VALUE, NULL);
	gtk_tree_view_column_set_sort_column_id (column, COLOR_VALUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (_("Name"), renderer, "text", COLOR_NAME, NULL);
	gtk_tree_view_column_set_sort_column_id (column, COLOR_NAME);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

	gtk_window_set_titlebar (GTK_WINDOW (window), header_bar);
	gtk_box_pack_start (GTK_BOX (box_all), header_bar, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_color_chooser), color_chooser, FALSE, FALSE, 0);
	gtk_stack_add_titled (GTK_STACK (stack), box_color_chooser, "color chooser", _("Color chooser"));
	gtk_container_add (GTK_CONTAINER (scroll), tree);
	gtk_stack_add_titled (GTK_STACK (stack), scroll, "scroll", _("Saved colors"));
	gtk_container_add (GTK_CONTAINER (box_all), stack);
	gtk_header_bar_pack_start (GTK_HEADER_BAR (header_bar), stackswitcher);
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), button_save);
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), button_about);

	g_signal_connect (selection, "changed", G_CALLBACK (on_list_selection_changed), NULL);
	g_signal_connect (stack, "notify::visible-child", G_CALLBACK (on_stack_page_change), NULL);
	g_signal_connect (color_chooser, "color-changed", G_CALLBACK (on_colorselection_color_changed), NULL);
	g_signal_connect (button_save, "clicked", G_CALLBACK (on_sd_button_clicked), NULL);
	g_signal_connect (button_about, "clicked", G_CALLBACK (about_dialog_open), NULL);

	gtk_container_add (GTK_CONTAINER (window), box_all);

	return window;
}
