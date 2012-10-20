/* Style.h */

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
 * Style.h
 *
 *  Created on: Oct 27, 2011
 *      Author: luke
 */

#ifndef STYLE_H_
#define STYLE_H_


#include <QString>



namespace Style{


	QString get_tv_style(bool dark);
	QString get_player_back_color();
	QString get_player_fore_color();

	QString get_v_scrollbar_style();
	QString get_h_scrollbar_style();
	QString get_tv_header_style();
	QString get_btn_style(int intensity=7);
	QString get_menubar_style(bool dark);
	QString get_menu_style(bool dark);
};


#endif /* STYLE_H_ */
