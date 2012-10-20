/* LibraryItemDelegateArtists.cpp */

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
 * LibraryItemDelegateArtists.cpp
 *
 *  Created on: Apr 28, 2011
 *      Author: luke
 */

#include "GUI/library/LibraryItemDelegateArtists.h"
#include <HelperStructs/MetaData.h>
#include <QObject>
#include <QLabel>
#include <QDebug>
#include <QTableView>
#include <QItemDelegate>
#include <QPainter>
#include <HelperStructs/Helper.h>

LibraryItemDelegateArtists::LibraryItemDelegateArtists(QTableView* parent) {
	this->_parent = parent;


}

LibraryItemDelegateArtists::~LibraryItemDelegateArtists() {
	// TODO Auto-generated destructor stub
}




void LibraryItemDelegateArtists::paint(QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{

	if(!index.isValid()) return;

		QLabel label;

		QRect rect(option.rect);

		painter->save();
		painter->translate(0, 0);



		if(index.column() == 0){
			int num_albums = index.model()->data(index, Qt::WhatsThisRole).toInt();

			if(num_albums <= 1)
				label.setPixmap(QPixmap(Helper::getIconPath() + "play_small.png"));
			else
				label.setPixmap(QPixmap(Helper::getIconPath() + "fwd_orange.png"));

			label.setContentsMargins(2, 0, 2, 2);
		}

		else if(index.column() == 1){

			QStringList list = index.model()->data(index, Qt::WhatsThisRole).toStringList();
			Artist artist;
			artist.fromStringList(list);

			QString text = "<b>" + artist.name + "</b>";
			label.setText(text);
			label.setContentsMargins(2, 0, 2, 2);
		}

		else if(index.column() == 2){
			QString text = index.model()->data(index, Qt::WhatsThisRole).toString();
			label.setText(text + " tracks");
			label.setContentsMargins(2, 0, 4, 2);
			label.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		}

		QString style;
		QString fg_color;

		bool is_selected = ((option.state & QStyle::State_Selected) != 0);

		QPalette palette = _parent->palette();
		QColor col_background = palette.color(QPalette::Active, QPalette::Background);
		QColor col_highlight = palette.color(QPalette::Active, QPalette::Highlight);
		//QColor col_highlight_lighter = palette.color(QPalette::Active, QPalette::Highlight).light();

		int val_bg = col_background.lightness();
		int val_sel = col_highlight.lightness();

		if(!is_selected){

			if(val_bg > 96) fg_color = " color: #202020";
			else fg_color = " color: #D8D8D8; ";

			style = QString("background-color: transparent; ") + fg_color;
		}

		else {
			if(val_sel > 96) fg_color = " color: #202020";
			else fg_color = " color: #D8D8D8; ";

			style = QString("background-color: " + col_highlight.name() + ";") + fg_color;
		}


		label.setAlignment( Qt::AlignVCenter );
		label.setStyleSheet(style);
		label.setFixedHeight(_parent->rowHeight(index.row())-1);
		label.setFixedWidth(_parent->columnWidth(index.column()));
		label.setContentsMargins(2, 0, 2, 0);

		label.setStyleSheet(style);
		label.render(painter, rect.topLeft() );

		painter->restore();

}





QSize LibraryItemDelegateArtists::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{


	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize(1, _parent->rowHeight(index.row()));


}




void LibraryItemDelegateArtists::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex & index) const
{
	Q_UNUSED(editor);
	Q_UNUSED(model);
	Q_UNUSED(index);
}

QWidget *LibraryItemDelegateArtists::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(parent);
	Q_UNUSED(option);
	Q_UNUSED(index);
	return 0;
}




void LibraryItemDelegateArtists::setEditorData(QWidget *editor, const QModelIndex & index) const
{
	Q_UNUSED(editor);
	Q_UNUSED(index);
}


