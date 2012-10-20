/* PlaylistParser.h */

/* Copyright (C) 2012  Lucio Carreras
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


#ifndef PLAYLISTPARSER_H_
#define PLAYLISTPARSER_H_

#include "HelperStructs/Helper.h"
#include <HelperStructs/MetaData.h>
#include <QStringList>
#include <QString>

#include <vector>

using namespace std;


namespace PlaylistParser {


	int parse_playlist(QString playlist_file, vector<MetaData>& md);
	bool is_supported_playlist(QString filename);


};

#endif /* PLAYLISTPARSER_H_ */
