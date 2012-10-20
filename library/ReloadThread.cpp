/* ReloadThread.cpp */

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
 * ReloadThread.cpp
 *
 *  Created on: Jun 19, 2011
 *      Author: luke
 */

#define N_FILES_TO_STORE 500

#include "library/ReloadThread.h"
#include "HelperStructs/CDirectoryReader.h"
#include "HelperStructs/CSettingsStorage.h"
#include "DatabaseAccess/CDatabaseConnector.h"
#include "HelperStructs/MetaData.h"
#include "HelperStructs/id3.h"
#include "HelperStructs/Helper.h"

#include <qmetatype.h>
#include <QStringList>
#include <QDebug>
#include <vector>
#include <omp.h>



using namespace std;


int ReloadThread::get_and_save_all_files(){

	if(_library_path.size() == 0 || !QFile::exists(_library_path)) return 0;

	QDir dir(_library_path);
	vector<MetaData> v_md;
	int n_files = 0;

	get_files_recursive (dir, v_md, &n_files);

	n_files += v_md.size();

	if(v_md.size() >  0){
		CDatabaseConnector::getInstance()->storeMetadata(v_md);
		v_md.clear();
	}

	return n_files;

}


void ReloadThread::get_files_recursive (QDir baseDir, vector<MetaData>& v_md, int* n_files){
	QDir baseDirDirs(baseDir);
	QDir baseDirFiles(baseDir);
    QStringList dirs;

	baseDirDirs.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dirs = baseDirDirs.entryList();

    foreach (QString dir, dirs) {
    	baseDirDirs.cd(dir);
        this -> get_files_recursive(baseDirDirs, v_md, n_files);
        baseDirDirs.cd("..");
    }

    int num_files = *n_files;

    dirs.clear();
    baseDirFiles.setFilter(QDir::Files);
    baseDirFiles.setNameFilters( Helper::get_soundfile_extensions() );
    dirs = baseDirFiles.entryList();

    QString reload_status_str;
    foreach (QString f, dirs) {

    	MetaData md;
    	QString filename = baseDirFiles.absoluteFilePath(f);

		if(ID3::getMetaDataOfFile(filename, md)){
			v_md.push_back(md);
			num_files ++;


			if(num_files % 20 == 0){
				reload_status_str = QString("Reloading library: ") + QString::number(num_files) + " tracks";
				emit reloading_library( reload_status_str );
			}
		}

		if(v_md.size() >= N_FILES_TO_STORE ){
			CDatabaseConnector::getInstance()->storeMetadata(v_md);
			v_md.clear();
		}
	}

    *n_files = num_files;

}



ReloadThread::ReloadThread() {
	 qRegisterMetaType<QString*>("QString*");
	_state = -1;
}

ReloadThread::~ReloadThread() {

}

int ReloadThread::getState(){
	return _state;
}


void ReloadThread::run(){

	CDatabaseConnector* db = CDatabaseConnector::getInstance();

	vector<MetaData> v_metadata;
	vector<MetaData> v_to_delete;

	QString reload_status_str = QString("Reloading library: Delete orphaned tracks");
	emit reloading_library(reload_status_str);
	db->getTracksFromDatabase(v_metadata);

	// find orphaned tracks in library && delete them
	for(uint i=0; i<v_metadata.size(); i++){
		MetaData md = v_metadata[i];
		if(!Helper::checkTrack(md)){
			v_to_delete.push_back(md);
		}
	}

	db->deleteTracks(v_to_delete);
	v_to_delete.clear();
	v_metadata.clear();

	int n_files = get_and_save_all_files();
	(void) n_files;
}

void ReloadThread::set_lib_path(QString library_path){
	_library_path = library_path;
}

void ReloadThread::get_metadata(vector<MetaData>& md){
	md = _v_metadata;
}


