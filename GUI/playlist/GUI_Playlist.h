/* GUI_Playlist.h */

/* Copyright (C) 2011  Lucio Carreras
 *
 * This file is part of sayonara player
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
 * GUI_Playlist.h
 *
 *  Created on: Apr 6, 2011
 *      Author: luke
 */

#ifndef GUI_PLAYLIST_H_
#define GUI_PLAYLIST_H_

#include "ui_GUI_Playlist.h"

#include "playlist/Playlist.h"
#include "HelperStructs/MetaData.h"
#include "HelperStructs/PlaylistMode.h"
#include "GUI/playlist/PlaylistItemDelegateInterface.h"
#include "GUI/InfoDialog/GUI_InfoDialog.h"

#include <QObject>
#include <QMainWindow>
#include <QWidget>
#include <QKeyEvent>
#include <QTextEdit>
#include <QList>

#include <string>
#include <vector>


	class GUI_Playlist : public QWidget, private Ui::Playlist_Window
	{
		Q_OBJECT

		public:
			GUI_Playlist(QWidget *parent, GUI_InfoDialog* dialog);
			~GUI_Playlist();

			void dragEnterEvent(QDragEnterEvent* event);
			void dragLeaveEvent(QDragLeaveEvent* event);
			void dropEvent(QDropEvent* event);
			void dragMoveEvent(QDragMoveEvent* event);
			void clear_drag_lines(int row);

            void *operator new[](size_t size)
             {
                 return malloc(size);
             }

             void *operator new(size_t size)
             {
                 return malloc(size);
             }

             void operator delete[](void *ptr)
             {
                 free(ptr);
             }

             void operator delete[](void *ptr, size_t)
             {
                 free(ptr);
             }

             void operator delete(void *ptr)
             {
                 free(ptr);
             }

             void operator delete(void *ptr, size_t)
             {
                 free(ptr);
             }


		signals:
			void selected_row_changed(int);
			void clear_playlist();
			void save_playlist(const QString&);
			void playlist_mode_changed(const Playlist_Mode&);
			void dropped_tracks(const vector<MetaData>&, int);
			void dropped_albums(const vector<Album>&, int);
			void dropped_artists(const vector<Artist>&, int);
			void playlist_filled(vector<MetaData>&);
			void sound_files_dropped(QStringList&);
			void directory_dropped(const QString&, int);
			void rows_removed(const QList<int>&);
			void edit_id3_signal();
			void search_similar_artists(const QString&);
			void sig_import_to_library(bool);

		public slots:
			void fillPlaylist(vector<MetaData>&, int);
			void track_changed(int);
			void change_skin(bool);
			void library_path_changed(QString);
			void last_fm_logged_in(bool);
			void import_result(bool);
			void set_radio_active(int radio);
			void psl_show_small_playlist_items(bool small_items);
			void psl_lfm_activated(bool);

		private slots:
			void double_clicked(const QModelIndex &);
			void pressed(const QModelIndex&);
			void released(const QModelIndex&);
			void clear_playlist_slot();
			void playlist_mode_changed_slot();
			void edit_id3_but_pressed();
			void dummy_pressed();
			void import_button_clicked();

			void psl_show_context_menu(const QPoint &);
			void psl_info_tracks();
			void psl_edit_tracks();
			void psl_delete_tracks();

			void btn_numbers_changed(bool);

		protected:
			void keyPressEvent(QKeyEvent* e);

		private:

			Ui::Playlist_Window* 			ui;
			QWidget*						_parent;
			GUI_InfoDialog*					_info_dialog;

			Playlist_Mode					_playlist_mode;
			QAbstractItemModel* 			_pli_model;
			PlaylistItemDelegateInterface* 	_pli_delegate;


			qint64 		_total_secs;
			int			_cur_playing_row;
			QList<int>	_cur_selected_rows;
			bool 		_inner_drag_drop;
			QPoint		_last_known_drag_pos;

			int			_radio_active;

			QMenu* 		_right_click_menu;

			QAction* 	_info_action;
			QAction* 	_edit_action;
			QAction* 	_delete_action;


			void initGUI();

			void set_total_time_label();
			void remove_cur_selected_rows();
			void check_dynamic_play_button();
			void init_menues();

			PlaylistItemDelegateInterface* create_item_delegate(bool small_playlist_items);




};



#endif /* GUI_PLAYLIST_H_ */
