/* Gcolor3 main
 *
 * Copyright (C) 2015 Jente Hidskes <hjdskes@gmail.com>
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * Modeled after eog's main.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <glib/gi18n.h>

#include "gcolor3-application.h"

static gboolean
_print_version_and_exit (const gchar *name,
			 const gchar *value,
			 gpointer     user_data,
			 GError     **error)
{
	g_print ("%s %s\n", _("Color picker"), VERSION);
	exit (EXIT_SUCCESS);
	return TRUE;
}

static const GOptionEntry options[] = {
	{ "version", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, _print_version_and_exit, N_("Show the application's version"), NULL },
	{ NULL }
};

int
main (int argc, char **argv)
{
	Gcolor3Application *application;
	GOptionContext *context;
	GError *error = NULL;
	int status;

	bindtextdomain (GETTEXT_PACKAGE, LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	context = g_option_context_new (_("Pick a color from the palette of the screen"));
	g_option_context_add_main_entries (context, options, GETTEXT_PACKAGE);
	/* This initialises GTK during parsing. */
	g_option_context_add_group (context, gtk_get_option_group (FALSE));

	if (!g_option_context_parse (context, &argc, &argv, &error)) {
		gchar *help;

		help = g_strdup_printf (_("Run '%s --help' to see a full "
					"list of available command line "
					"options."), argv[0]);
		g_printerr ("%s\n%s\n", error->message, help);

		g_error_free (error);
		g_free (help);
		g_option_context_free (context);

		return 1;
	}

	g_option_context_free (context);

	application = gcolor3_application_new ();
	status = g_application_run (G_APPLICATION (application), argc, argv);
	g_object_unref (application);

	return status;
}
