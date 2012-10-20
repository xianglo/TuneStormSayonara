/* MyTableView.h */

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
 * MyListView.h
 *
 *  Created on: Jun 26, 2011
 *      Author: luke
 */

#ifndef MYTABLEVIEW_H_
#define MYTABLEVIEW_H_

#include <QTableView>
#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QPoint>

#include "HelperStructs/CustomMimeData.h"


class MyTableView : public QTableView{

	Q_OBJECT

	signals:
		void context_menu_emitted(const QPoint&);
		void sig_middle_button_clicked(const QPoint&);


	public:
		MyTableView(QWidget* parent=0);
		virtual ~MyTableView();

		void set_mime_data(CustomMimeData* data);


	protected:
		void mousePressEvent(QMouseEvent* event);
		void mouseReleaseEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent* event);


	private:
		QWidget* 	_parent;
		bool		_drag;
		QPoint		_drag_pos;
		QDrag*		_qDrag;
};

#endif /* MYLISTVIEW_H_ */
