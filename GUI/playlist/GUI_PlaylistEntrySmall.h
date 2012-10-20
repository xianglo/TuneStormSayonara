/* GUI_PlaylistEntry.h */

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


#ifndef GUI_PLAYLISTENTRYSMALL_H_
#define GUI_PLAYLISTENTRYSMALL_H_

#include "ui_GUI_Playlist_EntrySmall.h"
#include "HelperStructs/MetaData.h"

#include <QObject>


class GUI_PlaylistEntrySmall : public QFrame, private Ui::PlaylistEntrySmall {

	Q_OBJECT
public:
	GUI_PlaylistEntrySmall(QWidget* parent=0);
	virtual ~GUI_PlaylistEntrySmall();

public:
	void setContent(const MetaData& md, int idx);

	void setBackgroundColorPlaying();
	void setBackgroundColorNotPlaying();

	void setWidth(int w);



private:
	Ui::PlaylistEntrySmall* ui;

};

#endif /* GUI_PLAYLISTENTRY_H_ */
