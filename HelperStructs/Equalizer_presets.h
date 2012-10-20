/* Equalizer_presets.h */

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


#ifndef _EQUALIZER_PRESETS_
#define _EQUALIZER_PRESETS_

#include <QString>
#include <QStringList>

struct EQ_Setting{
	
	
	QList<double> settings;
	QString name;

	EQ_Setting(){
		name = "";

		for(int i=0; i<10; i++){
			settings.push_back(0);
		}
	}
	
	void parseFromString(QString str){

		QStringList list = str.split(',');
		if(list.size() == 0) {
			return;
		}

		name = list.at(0);
		list.pop_front();

		for(int i=0; i<list.size(); i++){

			if( i == settings.size() ) break;
			settings[i] = list.at(i).toDouble();
		}
	}


	QString toString(){

		QString str = name;
		for(int i=0; i<settings.size(); i++){
			str += QString(",") + QString::number(settings[i]);
		}

		return str;
	}
};

#endif
