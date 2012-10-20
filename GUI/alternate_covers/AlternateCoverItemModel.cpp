/* AlternateCoverItemModel.cpp */

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
 * AlternateCoverItemModel.cpp
 *
 *  Created on: Jul 1, 2011
 *      Author: luke
 */

#include "GUI/alternate_covers/AlternateCoverItemModel.h"
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QDebug>


AlternateCoverItemModel::AlternateCoverItemModel() {
	// TODO Auto-generated constructor stub

}

AlternateCoverItemModel::~AlternateCoverItemModel() {

}





int AlternateCoverItemModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	if(_pathlist.size() == 0) return 0;
	return _pathlist.size() / 5;

}
int AlternateCoverItemModel::columnCount(const QModelIndex &parent) const
{
	return 5;
}

QVariant AlternateCoverItemModel::data(const QModelIndex &index, int role) const
{

	 if (!index.isValid() || _pathlist.size() == 0)
		 return QVariant();

	 if(role == Qt::WhatsThisRole){
		 return _pathlist[index.row() * columnCount() + index.column()];
	 }

	 else
		 return QVariant();
}

Qt::ItemFlags AlternateCoverItemModel::flags(const QModelIndex &index) const{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index);

}

bool AlternateCoverItemModel::setData(const QModelIndex &index, const QVariant &value, int role){
	 if (!index.isValid())
		 return false;

	 if(role == Qt::EditRole){
		 _pathlist[index.row() * columnCount() + index.column()] = value.toString();
		 return true;
	 }

	 else
		 return false;

}

bool AlternateCoverItemModel::insertRows(int position, int rows, const QModelIndex &index){
	Q_UNUSED(index);

	beginInsertRows(QModelIndex(), position, position+rows-1);

	_pathlist.clear();
	int z=0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<columnCount(); j++, z++){
			_pathlist << "";
		}
	}

	endInsertRows();
	return true;

}
bool AlternateCoverItemModel::removeRows(int position, int rows, const QModelIndex &index){
	Q_UNUSED(index);

	 beginRemoveRows(QModelIndex(), position, position+rows-1);

	 _pathlist.clear();

	 endRemoveRows();
	 return true;

}
