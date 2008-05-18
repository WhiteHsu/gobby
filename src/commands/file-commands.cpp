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

#include "commands/file-commands.hpp"
#include "util/i18n.hpp"

Gobby::FileCommands::FileCommands(Gtk::Window& parent, Header& header,
                                  const Browser& browser, Folder& folder,
                                  StatusBar& status_bar):
	m_parent(parent), m_browser(browser), m_folder(folder),
	m_status_bar(status_bar)
{
	header.action_file_new->signal_activate().connect(
		sigc::mem_fun(*this, &FileCommands::on_new));
}

void Gobby::FileCommands::on_new()
{
	if(m_location_dialog.get() == NULL)
	{
		m_location_dialog.reset(
			new DocumentLocationDialog(
				m_parent,
				INF_GTK_BROWSER_MODEL(
					m_browser.get_store())));

		m_location_dialog->signal_response().connect(
			sigc::mem_fun(
				*this,
				&FileCommands::on_location_dialog_response));
	}

	m_location_dialog->set_document_name(_("New Document"));
	m_location_dialog->present();
}

void Gobby::FileCommands::on_location_dialog_response(int id)
{
	if(id == Gtk::RESPONSE_ACCEPT)
	{
		InfcBrowserIter iter;
		InfcBrowser* browser =
			m_location_dialog->get_selected_directory(&iter);
		g_assert(browser != NULL);

		Glib::ustring str(m_location_dialog->get_document_name());
		
		// TODO: Use FileOperations here
		InfcNodeRequest* request = infc_browser_add_note(
			browser, &iter, str.c_str(),
			infc_browser_lookup_plugin(browser, "InfText"),
			FALSE);

		// TODO: Info in statusbar, map, etc.
	}

	m_location_dialog->hide();
}
