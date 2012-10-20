/* GUI_StreamRecorder.h

 * Copyright (C) 2012  
 *
 * This file is part of sayonara-player
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * created by Lucio Carreras, 
 * May 13, 2012 
 *
 */

#ifndef GUI_STREAMRECORDER_H_
#define GUI_STREAMRECORDER_H_

#include <QWidget>
#include <QDialog>
#include "ui_GUI_StreamRecorder.h"
#include "HelperStructs/CSettingsStorage.h"

class GUI_StreamRecorder : public QDialog, private Ui_GUI_StreamRecorder{

	Q_OBJECT

public:

	signals:
		void sig_stream_recorder_active(bool);
		void sig_path_changed(const QString&);
		void sig_complete_tracks(bool);
		void sig_create_playlist(bool);

	private slots:
		void sl_cb_activate_toggled(bool);
		void sl_cb_complete_tracks_toggled(bool);
		void sl_cb_create_playlist_toggled(bool);
		void sl_btn_path_clicked();
		void sl_ok();

	public slots:
		void psl_show(bool);

	public:
		GUI_StreamRecorder(QWidget* parent=0);
		virtual ~GUI_StreamRecorder();

	private:
		Ui_GUI_StreamRecorder* ui;
		CSettingsStorage* _settings;

		QString _path;
		bool	_is_active;
		bool	_is_complete_tracks;
		bool	_is_create_playlist;

};

#endif /* GUI_STREAMRECORDER_H_ */
