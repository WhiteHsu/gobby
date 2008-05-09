/* gobby - A GTKmm driven libobby client
 * Copyright (C) 2005 - 2008 0x539 dev group
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "dialogs/documentlocationdialog.hpp"
#include "util/i18n.hpp"

#include <gtkmm/stock.h>

Gobby::DocumentLocationDialog::DocumentLocationDialog(Gtk::Window& parent,
                                                      InfGtkBrowserModel* m):
	m_table(3, 2), m_name_label(_("Document Name:"), Gtk::ALIGN_RIGHT),
	m_location_label(
		_("Choose a directory to create the document into:"),
		Gtk::ALIGN_LEFT),
	m_filter_model(inf_gtk_browser_model_filter_new(m)),
	m_view(INF_GTK_BROWSER_VIEW(inf_gtk_browser_view_new_with_model(
		INF_GTK_BROWSER_MODEL(m_filter_model))))
{
	m_name_label.show();
	m_name_entry.show();
	m_location_label.show();
	gtk_widget_show(GTK_WIDGET(m_view));

	gtk_container_add(GTK_CONTAINER(m_scroll.gobj()), GTK_WIDGET(m_view));
	m_scroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_scroll.set_shadow_type(Gtk::SHADOW_IN);
	m_scroll.show();

	m_table.attach(m_name_label, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL);
	m_table.attach(m_name_entry, 1, 2, 0, 1,
	               Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK);
	m_table.attach(m_location_label, 0, 2, 1, 2,
	               Gtk::FILL | Gtk::EXPAND, Gtk::FILL);
	m_table.attach(m_scroll, 0, 2, 2, 3,
	               Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND);
	m_table.set_spacings(6);
	m_table.set_border_width(12);
	m_table.show();

	get_vbox()->pack_start(m_table, Gtk::PACK_EXPAND_WIDGET);

	// TODO: Make ACCEPT insensitive if nothing is selected
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_ACCEPT);

	set_default_size(-1, 480);
	set_title(_("Select document's target location"));
}

Gobby::DocumentLocationDialog::~DocumentLocationDialog()
{
	g_object_unref(m_filter_model);
	m_filter_model = NULL;
}

Glib::ustring Gobby::DocumentLocationDialog::get_document_name() const
{
	return m_name_entry.get_text();
}

void Gobby::DocumentLocationDialog::set_document_name(
	const Glib::ustring& document_name)
{
	m_name_entry.set_text(document_name);
}

InfcBrowser* Gobby::DocumentLocationDialog::get_selected_directory(
	InfcBrowserIter* iter) const
{
	GtkTreeIter tree_iter;
	InfcBrowserIter* browser_iter;
	InfcBrowser* browser;

	if(inf_gtk_browser_view_get_selected(m_view, &tree_iter))
	{
		gtk_tree_model_get(
			GTK_TREE_MODEL(m_filter_model),
			&tree_iter,
			INF_GTK_BROWSER_MODEL_COL_BROWSER, &browser,
			INF_GTK_BROWSER_MODEL_COL_NODE, &browser_iter,
			-1);

		*iter = *browser_iter;

		infc_browser_iter_free(browser_iter);
		g_object_unref(browser);

		return browser;
	}
	else
	{
		return NULL;
	}
}

void Gobby::DocumentLocationDialog::on_show()
{
	Gtk::Dialog::on_show();

	m_name_entry.select_region(0, m_name_entry.get_text_length());
	m_name_entry.grab_focus();
}
