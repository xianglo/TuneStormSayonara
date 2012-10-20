/* GUI_Playlist.cpp */

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
 * GUI_Playlist.cpp
 *
 *  Created on: Apr 6, 2011
 *      Author: luke
 */


#include "HelperStructs/MetaData.h"
#include "HelperStructs/Helper.h"
#include "HelperStructs/id3.h"
#include "HelperStructs/PlaylistMode.h"
#include "HelperStructs/CSettingsStorage.h"
#include "HelperStructs/Style.h"
#include "HelperStructs/globals.h"
#include "HelperStructs/CustomMimeData.h"

#include "GUI/playlist/GUI_Playlist.h"
#include "GUI/playlist/PlaylistItemModel.h"
#include "GUI/playlist/PlaylistItemDelegate.h"
#include "GUI/playlist/PlaylistItemDelegateSmall.h"
#include "GUI/playlist/PlaylistItemDelegateInterface.h"
#include "GUI/InfoDialog/GUI_InfoDialog.h"

#include "StreamPlugins/LastFM/LastFM.h"
#include <QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QFileDialog>
#include <QScrollBar>
#include <QMacStyle>
#include <QAbstractListModel>
#include <iostream>
#include <vector>
#include <QStyleFactory>
#include <QPaintDevice>
#include <QPainter>
#include <QMessageBox>
#include <QTextEdit>
#include <QAction>
#include <QMenu>
#include <QUrl>


class QPaintEngine;
class QPaintDevice;


using namespace std;
// CTOR
GUI_Playlist::GUI_Playlist(QWidget *parent, GUI_InfoDialog* dialog) :
	QWidget(parent)
	 {

	this->ui = new Ui::Playlist_Window();
	ui->setupUi(this);
	initGUI();


	CSettingsStorage* settings = CSettingsStorage::getInstance();
	bool small_playlist_items = settings->getShowSmallPlaylist();

	_info_dialog = dialog;

	_pli_model = new PlaylistItemModel();
	_pli_delegate = create_item_delegate(small_playlist_items);

	_inner_drag_drop = false;

	_playlist_mode = settings->getPlaylistMode();

	this->ui->btn_append->setChecked(_playlist_mode.append);
	this->ui->btn_repAll->setChecked(_playlist_mode.repAll);

	if(settings->getLastFMActive() && LastFM::getInstance()->lfm_is_logged_in())
		this->ui->btn_dynamic->setChecked(_playlist_mode.dynamic);
	else
		this->ui->btn_dynamic->setChecked(false);

	this->ui->btn_shuffle->setChecked(_playlist_mode.shuffle);
	this->ui->btn_numbers->setChecked(settings->getPlaylistNumbers());

	this->ui->listView->setDragEnabled(true);
	this->ui->listView->setModel(_pli_model);
	this->ui->listView->setItemDelegate(_pli_delegate);
	this->ui->listView->setSelectionRectVisible(true);
	this->ui->listView->setAlternatingRowColors(true);
	this->ui->listView->setMovement(QListView::Free);

	this->ui->btn_import->setVisible(false);

	connect(this->ui->btn_clear, SIGNAL(released()), this, SLOT(clear_playlist_slot()));
	//this->connect(this->ui->btn_dummy, SIGNAL(released()), this, SLOT(dummy_pressed()));

	connect(this->ui->btn_rep1, SIGNAL(released()), this, SLOT(playlist_mode_changed_slot()));
	connect(this->ui->btn_repAll, SIGNAL(released()), this, SLOT(playlist_mode_changed_slot()));
	connect(this->ui->btn_shuffle, SIGNAL(released()), this, SLOT(playlist_mode_changed_slot()));
	connect(this->ui->btn_dynamic, SIGNAL(released()), this, SLOT(playlist_mode_changed_slot()));
	connect(this->ui->btn_append, SIGNAL(released()), this, SLOT(playlist_mode_changed_slot()));

	connect(this->ui->listView, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressed(const QModelIndex&)));
	connect(this->ui->listView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(double_clicked(const QModelIndex &)));
	connect(this->ui->listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(released(const QModelIndex &)));
	connect(this->ui->listView, SIGNAL(context_menu_emitted(const QPoint&)), this, SLOT(psl_show_context_menu(const QPoint&)));

	connect(this->ui->btn_import, SIGNAL(clicked()), this, SLOT(import_button_clicked()));
	connect(this->ui->btn_numbers, SIGNAL(toggled(bool)), this, SLOT(btn_numbers_changed(bool)));

	// we need a reason for refreshing the list
	QStringList empty_list;
	emit sound_files_dropped(empty_list);
	this->setAcceptDrops(true);

	_parent = parent;
	_total_secs = 0;

	_cur_playing_row = -1;

	_radio_active = RADIO_OFF;

	check_dynamic_play_button();

	int style = settings->getPlayerStyle();
	bool dark = (style == 1);
	change_skin(dark);

	init_menues();

    hide();
}


// DTOR
GUI_Playlist::~GUI_Playlist() {
	delete _pli_delegate;
	delete _pli_model;

	delete ui;
}


// initialize gui
// maybe the button state (pressed/unpressed) should be loaded from db here
void GUI_Playlist::initGUI(){

	QString icon_path = Helper::getIconPath();

	this->ui->btn_append->setIcon(QIcon(icon_path + "append.png"));
	this->ui->btn_rep1->setIcon(QIcon(icon_path + "rep1.png"));
	this->ui->btn_rep1->setVisible(false);
	this->ui->btn_repAll->setIcon(QIcon(icon_path + "repAll.png"));
	this->ui->btn_dynamic->setIcon(QIcon(icon_path + "dynamic.png"));
	this->ui->btn_shuffle->setIcon(QIcon(icon_path + "shuffle.png"));
	this->ui->btn_clear->setIcon(QIcon(icon_path + "broom.png"));
	this->ui->btn_import->setIcon(QIcon(icon_path + "import.png"));
	this->ui->btn_numbers->setIcon(QIcon(icon_path + "numbers.png"));
}


void GUI_Playlist::init_menues(){

	_right_click_menu = new QMenu(this);
	_info_action = new QAction(QIcon(Helper::getIconPath() + "info.png"), "Info", this);
	_edit_action = new QAction(QIcon(Helper::getIconPath() + "lyrics.png"), "Edit", this);

	_right_click_menu->addAction(_info_action);
	_right_click_menu->addAction(_edit_action);
}



void GUI_Playlist::library_path_changed(QString path){
	Q_UNUSED(path);
	check_dynamic_play_button();
}

void GUI_Playlist::check_dynamic_play_button(){

	QString libraryPath = CSettingsStorage::getInstance()->getLibraryPath();
	bool lfm_active = CSettingsStorage::getInstance()->getLastFMActive() && LastFM::getInstance()->lfm_is_logged_in();


	if(libraryPath.size() == 0 || !QFile::exists(libraryPath)){
		this->ui->btn_dynamic->setEnabled(false);
		this->ui->btn_dynamic->setToolTip("Please set library path first");
	}

	if(!lfm_active){
		this->ui->btn_dynamic->setEnabled(false);
		this->ui->btn_dynamic->setToolTip("Please configure LastFM");
	}

	else{
		this->ui->btn_dynamic->setEnabled(true);
		this->ui->btn_dynamic->setToolTip("Dynamic playing");
	}
}

void GUI_Playlist::dummy_pressed(){

}


// SLOT: switch between dark & light skin
void GUI_Playlist::change_skin(bool dark){

	QString btn_style = Style::get_btn_style(dark ? 6 : 7);
	QString table_style = Style::get_tv_style(dark);
	QString scrollbar_style = Style::get_v_scrollbar_style();

	if(dark)
		this->ui->lab_totalTime->setStyleSheet("background-color: " + Style::get_player_back_color() + ";");
	else
		this->ui->lab_totalTime->setStyleSheet("");

	this->ui->listView->setStyleSheet(table_style);
	this->ui->listView->verticalScrollBar()->setStyleSheet(dark ? scrollbar_style : table_style);

	this->ui->btn_append->setStyleSheet(btn_style);
	this->ui->btn_clear->setStyleSheet(btn_style);
	this->ui->btn_dynamic->setStyleSheet(btn_style);
	this->ui->btn_repAll->setStyleSheet(btn_style);
	this->ui->btn_shuffle->setStyleSheet(btn_style);
	this->ui->btn_numbers->setStyleSheet(btn_style);
}



// SLOT: fill all tracks in v_metadata into playlist
// the current track should be highlighted
void GUI_Playlist::fillPlaylist(vector<MetaData>& v_metadata, int cur_play_idx){

	_pli_model->removeRows(0, _pli_model->rowCount());
	if(_radio_active != RADIO_LFM)
		_pli_model->insertRows(0, v_metadata.size());
	else
		_pli_model->insertRows(0, 1);

	_total_secs = 0;
	int idx = 0;

	_cur_playing_row = cur_play_idx;

	this->ui->btn_import->setVisible(false);
	for(vector<MetaData>::iterator it = v_metadata.begin(); it != v_metadata.end(); it++){

		if(it->is_extern && _radio_active == RADIO_OFF) {
			this->ui->btn_import->setVisible(true);
		}

		QModelIndex model_idx = _pli_model->index(idx, 0);

		int min, sek;
		Helper::cvtSecs2MinAndSecs(it->length_ms / 1000, &min, &sek);
		_total_secs += (min * 60 + sek);

		QStringList str4Playlist = it->toStringList();

		if(idx == _cur_playing_row) str4Playlist.push_back("1");
		else str4Playlist.push_back("0");

		str4Playlist.push_back("0");

		_pli_model->setData(model_idx, (const QVariant&) str4Playlist, Qt::EditRole);

		if(_radio_active == RADIO_LFM) break;
		idx++;
	}

	set_total_time_label();
}

// private SLOT: clear button pressed
void GUI_Playlist::clear_playlist_slot(){

	if(_radio_active != RADIO_OFF) return;

	this->ui->lab_totalTime->setText("Total Time: 0m 0s");
	this->ui->btn_import->setVisible(false);

	_pli_model->removeRows(0, _pli_model->rowCount());
	_cur_playing_row = -1;
	_cur_selected_rows.clear();

	emit clear_playlist();
}


// private SLOT: playlist item pressed (init drag & drop)
void GUI_Playlist::pressed(const QModelIndex& index){

	if(!index.isValid() || index.row() < 0 || index.row() >= _pli_model->rowCount()) return;

	QModelIndexList idx_list = this->ui->listView->selectionModel()->selectedRows();
	vector<MetaData> v_md;

	_cur_selected_rows.clear();

	foreach(QModelIndex idx, idx_list){

		MetaData md;
		QStringList metadata = this->_pli_model->data(idx, Qt::WhatsThisRole).toStringList();
		if(metadata.size() == 0) continue;

		md.fromStringList(metadata);
		v_md.push_back(md);

		_cur_selected_rows.push_back(idx.row());
	}

	if(_info_dialog)
		_info_dialog->setMetaData(v_md);

	CustomMimeData* mime = new CustomMimeData();
	mime->setMetaData(v_md);

	this->ui->listView->set_mime_data(mime);

	if(_cur_selected_rows.contains( index.row() ))
		_inner_drag_drop = true;

	else _inner_drag_drop = false;
}

void GUI_Playlist::released(const QModelIndex& index){

	_inner_drag_drop = false;

	if(!index.isValid() || index.row() < 0 || index.row() >= _pli_model->rowCount()) return;

		this->ui->listView->set_mime_data(NULL);
		_inner_drag_drop = false;
}


// private SLOT: track chosen (change track)
void GUI_Playlist::double_clicked(const QModelIndex & index){

	if(!index.isValid() || index.row() < 0 || index.row() >= _pli_model->rowCount()) return;

	clear_drag_lines(index.row());

	for(int i=0; i<index.model()->rowCount(); i++){

		QModelIndex tmp_idx = index.model()->index(i, 0);
		QStringList str4Playlist = index.model()->data(tmp_idx, Qt::WhatsThisRole).toStringList();

		if(i == index.row())
			str4Playlist[str4Playlist.length()-2] = "1";	// paint as marked

		else
			str4Playlist[str4Playlist.length()-2] = "0";

		_pli_model->setData(tmp_idx, (const QVariant&) str4Playlist, Qt::EditRole);
	}

	int new_row = index.row();

	if(new_row < 0 || new_row >= _pli_model->rowCount() ) return;

	emit selected_row_changed(new_row);
	_cur_playing_row = new_row;
}



// SLOT: if the current track has has changed
// by playlist
void GUI_Playlist::track_changed(int new_row){

	if(new_row < 0) return;

	QModelIndex index = _pli_model->index(new_row, 0);

	_cur_playing_row = new_row;

	for(int i=0; i<_pli_model->rowCount(); i++){

		QModelIndex tmp_idx = _pli_model->index(i, 0);
		QStringList str4Playlist = index.model()->data(tmp_idx, Qt::WhatsThisRole).toStringList();

		if(str4Playlist.size() >= 3){

			if(i == index.row())
				str4Playlist[str4Playlist.size()-2] = QString("1");

			else
				str4Playlist[str4Playlist.size()-2] = QString("0");

			_pli_model->setData(tmp_idx, (const QVariant&) str4Playlist, Qt::EditRole);
		}
	}

	this->ui->listView->scrollTo(index);
}

// private SLOT: rep1, repAll, shuffle or append has changed
void GUI_Playlist::playlist_mode_changed_slot(){

	this->parentWidget()->setFocus();

	_playlist_mode.rep1 = this->ui->btn_rep1->isChecked();
	_playlist_mode.repAll = this->ui->btn_repAll->isChecked();
	_playlist_mode.shuffle = this->ui->btn_shuffle->isChecked();
	_playlist_mode.append = this->ui->btn_append->isChecked();
	_playlist_mode.dynamic = this->ui->btn_dynamic->isChecked();

	emit playlist_mode_changed(_playlist_mode);
	emit save_playlist("bla");
}


void GUI_Playlist::psl_edit_tracks(){
	if(!_info_dialog) return;
	_info_dialog->setMode(INFO_MODE_TRACKS);
	_info_dialog->show(TAB_EDIT);
}

void GUI_Playlist::psl_info_tracks(){
	if(!_info_dialog) return;
	_info_dialog->setMode(INFO_MODE_TRACKS);
	_info_dialog->show(TAB_INFO);
}

void GUI_Playlist::psl_delete_tracks(){

}


void GUI_Playlist::psl_show_context_menu(const QPoint& p){

	connect(_edit_action, SIGNAL(triggered()), this, SLOT(psl_edit_tracks()));
	connect(_info_action, SIGNAL(triggered()), this, SLOT(psl_info_tracks()));

	this->_right_click_menu->exec(p);

	disconnect(_edit_action, SIGNAL(triggered()), this, SLOT(psl_edit_tracks()));
	disconnect(_info_action, SIGNAL(triggered()), this, SLOT(psl_info_tracks()));
}


// we start the drag action, all lines has to be cleared
void GUI_Playlist::dragLeaveEvent(QDragLeaveEvent* event){

	Q_UNUSED(event);
	int row = this->ui->listView->indexAt(_last_known_drag_pos).row();
	clear_drag_lines(row);
}


// remove the black line under the titles
void GUI_Playlist::clear_drag_lines(int row){

	for(int i=row-3; i<=row+3; i++){

		QModelIndex idx = this->_pli_model->index(i, 0);
		if(!idx.isValid() || idx.row() < 0 || idx.row() >= _pli_model->rowCount())
			continue;

		QStringList list = _pli_model->data(this->_pli_model->index(i, 0), Qt::WhatsThisRole).toStringList();

		// set the flag, that no black line should be painted
		if(list.size() > 1)
			list[list.length()-1] = QString("0");

		_pli_model->setData(this->_pli_model->index(i, 0), (const QVariant&) list, Qt::EditRole);

	}
}

// the drag comes, if there's data --> accept it
void GUI_Playlist::dragEnterEvent(QDragEnterEvent* event){

	if(event->mimeData() != NULL )
		event->acceptProposedAction();
}

// we move the title
// scroll, if neccessary
// paint line
void GUI_Playlist::dragMoveEvent(QDragMoveEvent* event){

	if( !event->mimeData() )  {
		qDebug() << "mime data not available";
		return;
	}

	QPoint pos = event->pos();
	_last_known_drag_pos = pos;

	int scrollbar_pos = this->ui->listView->verticalScrollBar()->sliderPosition();
	int y_event =  event->pos().y();
	int y_playlist = this->ui->listView->y();


	// scroll up
	if(y_event <= y_playlist+10){

		if(scrollbar_pos >= 1)
			this->ui->listView->scrollTo(this->_pli_model->index(scrollbar_pos-1, 0));
	}

	// scroll down
	else if(y_event >= y_playlist + this->ui->listView->height()-10){
		int num_steps = ui->listView->height() / _pli_delegate->rowHeight();
		QModelIndex idx = _pli_model->index(scrollbar_pos+num_steps, 0);

		if(idx.isValid()) this->ui->listView->scrollTo(idx);
		if(idx.row() == -1) this->ui->listView->scrollToBottom();
	}


	int row = this->ui->listView->indexAt(pos).row();

	if(row <= -1) row = _pli_model->rowCount()-1;
	else if(row > 0) row--;

	if( (_cur_selected_rows.contains(row) && _inner_drag_drop) ||
		row >= _pli_model->rowCount()){
			return;
	}

	clear_drag_lines(row);

	if(pos.y() <= y_playlist + 2){
		return;
	}

	QModelIndex cur_idx = _pli_model->index(row, 0);
	if( !cur_idx.isValid() ){

		return;
	}

	// paint line
	QStringList list = _pli_model->data(cur_idx, Qt::WhatsThisRole).toStringList();
	list[list.length()-1] = QString("1");

	_pli_model->setData(cur_idx, (const QVariant&) list, Qt::EditRole);
}

// finally drop it
void GUI_Playlist::dropEvent(QDropEvent* event){


	if(!event->mimeData()) return;

	// where did i drop?
	QPoint pos = event->pos();
	QModelIndex idx = this->ui->listView->indexAt(pos);
	int row = idx.row();
	if(row == -1){
		row = this->_pli_model->rowCount();
	}

	// remove line
	clear_drag_lines(row);

	if(_inner_drag_drop){
		_inner_drag_drop = false;
		if( _cur_selected_rows.contains(row-1) ){
			event->ignore();
			return;
		}

		if(_cur_selected_rows.first() < row ) {
			row -= _cur_selected_rows.size();
		}

		remove_cur_selected_rows();
	}

	const CustomMimeData* d = (const CustomMimeData*) event->mimeData();
	vector<MetaData> v_metadata;


	// extern
	if( d->hasUrls() ){

		foreach(QUrl url, d->urls()){

				QString path;
				QString url_str = url.toString();

				if(url_str.startsWith("file://")){
					path =  url_str.right(url_str.length() - 7).trimmed();
					path = path.replace("%20", " ");

					if(QFile::exists(path)){
						if(Helper::is_soundfile(path)){
							MetaData md;
							if( ID3::getMetaDataOfFile(path, md) ){
								v_metadata.push_back( md );
							}
						}

						else if(path.at(path.length()-4) != '.'){ // directory
							emit directory_dropped(path, row);
							return;
						}
					}
				}

		} // end foreach

		if(v_metadata.size() == 0) return;

		if(_radio_active == RADIO_OFF)
			emit dropped_tracks(v_metadata, row);
		else
			emit dropped_tracks(v_metadata, 0);

		return;
	}

	else if(d->getMetaData(v_metadata) > 0){

		if(row == -1) row = _pli_model->rowCount();
		if(row < _cur_playing_row) _cur_playing_row += v_metadata.size();

		if(_radio_active == RADIO_OFF)
			emit dropped_tracks(v_metadata, row);
		else
			emit dropped_tracks(v_metadata, 0);

	}
}


void GUI_Playlist::set_total_time_label(){

	if(_radio_active == RADIO_STATION){
		this->ui->lab_totalTime->setText("Radio");
		return;
	}

	else if(_radio_active == RADIO_LFM){
		this->ui->lab_totalTime->setText("Last.fm Radio");
		return;
	}

	this->ui->lab_totalTime->setContentsMargins(0, 2, 0, 2);

	QString playlist_string = QString::number(this->_pli_model->rowCount());
	if(this->_pli_model->rowCount() == 1) playlist_string += " Track - ";
	else playlist_string += " Tracks - ";

	playlist_string += Helper::cvtMsecs2TitleLengthString(_total_secs * 1000, false);

	this->ui->lab_totalTime->setText(playlist_string);
}

void GUI_Playlist::remove_cur_selected_rows(){

	if(_pli_model->rowCount() > 1){

		if(_cur_selected_rows.first() < _cur_playing_row)
			_cur_playing_row --;

		emit rows_removed(_cur_selected_rows);
	}

	else
		clear_playlist_slot();

	_cur_selected_rows.clear();
}

void GUI_Playlist::keyPressEvent(QKeyEvent* e){

	QWidget::keyPressEvent(e);
	int key = e->key();

	if(key == Qt::Key_Delete && _cur_selected_rows.size() > 0)
		remove_cur_selected_rows();
}


void GUI_Playlist::edit_id3_but_pressed(){
	emit edit_id3_signal();
}

void GUI_Playlist::last_fm_logged_in(bool success){

	this->ui->btn_dynamic->setEnabled(success);
}

void GUI_Playlist::psl_lfm_activated(bool b){
	this->ui->btn_dynamic->setEnabled(b);
}


void GUI_Playlist::import_button_clicked(){
	emit sig_import_to_library(false);
}

void GUI_Playlist::import_result(bool success){

	this->ui->btn_import->setVisible(!success);
}


void GUI_Playlist::set_radio_active(int radio){

	_radio_active = radio;
	this->ui->btn_append->setVisible(radio == RADIO_OFF);
	this->ui->btn_dynamic->setVisible(radio == RADIO_OFF);
	this->ui->btn_repAll->setVisible(radio == RADIO_OFF);
	this->ui->btn_shuffle->setVisible(radio == RADIO_OFF);
	this->ui->btn_import->setVisible(radio == RADIO_OFF);
}


PlaylistItemDelegateInterface* GUI_Playlist::create_item_delegate(bool small_playlist_items){

	PlaylistItemDelegateInterface* delegate = 0;
	if(small_playlist_items)
		delegate = new PlaylistItemDelegateSmall(this->ui->listView);
	else
		delegate = new PlaylistItemDelegate(this->ui->listView);

	return delegate;
}


void GUI_Playlist::psl_show_small_playlist_items(bool small_playlist_items){

	_pli_delegate = create_item_delegate(small_playlist_items);

	this->ui->listView->setItemDelegate(_pli_delegate);
	this->ui->listView->reset();
}


void GUI_Playlist::btn_numbers_changed(bool b){
	this->parentWidget()->setFocus();
	CSettingsStorage::getInstance()->setPlaylistNumbers(b);
	this->ui->listView->reset();

}
