/* CSettingsStorage.cpp */

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


#include "HelperStructs/Helper.h"
#include "HelperStructs/PlaylistMode.h"
#include "HelperStructs/CSettingsStorage.h"
#include "DatabaseAccess/CDatabaseConnector.h"


#include <QFile>
#include <QDir>
#include <QDebug>



CSettingsStorage * CSettingsStorage::getInstance() {
    static CSettingsStorage inst;
    return &inst;
}

bool CSettingsStorage::isRunFirstTime () {
    bool ret = false;
    QDir dir = QDir::homePath();
    if (dir.cd(this -> m_sayonaraPath)) {
        qDebug() << dir.path() + "/" + m_dbFile;
        QFile f (dir.path() + "/" + m_dbFile);
        ret = f.exists();
    }
    return ret;
}

bool CSettingsStorage::runFirstTime (bool deleteOld) {
    bool ret = false;
    QDir dir = QDir::homePath();

    ret = dir.cd(this -> m_sayonaraPath);
    if (!ret) {
        //dir does not exist, so we are creating it...
        ret = dir.mkdir(this -> m_sayonaraPath);
        if (ret) {
            ret = dir.cd(this -> m_sayonaraPath);
        }
    }
    //if ret is still not true we are not able to create the directory
    if (ret) {
        QFile dest (dir.absolutePath() + QDir::separator() + m_dbFile);
        if (deleteOld) {
            if (dest.exists()) {
                qDebug() << "Deleting: " << dir.absolutePath() + QDir::separator() + m_dbFile;
                dir.remove(m_dbFile);
            }
        }
        if (!dest.exists()) {

            qDebug() << "cp " << Helper::getIconPath() + m_dbSource << " " << dir.absolutePath() + QDir::separator() + m_dbFile;
            if (QFile::copy(Helper::getIconPath() + m_dbSource, dir.absolutePath() + QDir::separator()+ m_dbFile)) {
               qDebug() << "DB File has been copied to " <<   dir.absolutePath() + QDir::separator()+ m_dbFile;
            }
            else {
               // qFatal(QString ("Were not able to copy file" + dir.absolutePath() + QDir::separator()+ m_dbFile).toStdString().c_str());
                qFatal("Were not able to copy file %s", QString(dir.absolutePath() + QDir::separator()+ m_dbFile).toStdString().c_str());


            }
        }
    }
    else {
        qFatal("We are not able to create the directory %s", QString(QDir::homePath() + m_sayonaraPath).toStdString().c_str() );
    }
    if (!dir.exists()) {

    }
    return ret;
}


CSettingsStorage::~CSettingsStorage () {
    //is called
}


void CSettingsStorage::init() {
    m_dbFile = "player.db";
    m_dbSource = "empty.db";
    m_sayonaraPath = ".Sayonara";
}

QString CSettingsStorage::getDBFileName () {
		
#ifdef Q_OS_WIN   
	QString dir = QDir::homePath() + QString("\\.Sayonara\\player.db");
    return dir;
#else

    //dir.cd(Helper::getIconPath());
    return QDir::homePath() + QDir::separator() + m_sayonaraPath + QDir::separator() + m_dbFile;
#endif


}

bool CSettingsStorage::getLastFMActive(){
	return m_lfm_active;
}

void CSettingsStorage::setLastFMActive(bool b){
	m_lfm_active = b;
}

QPair<QString, QString> CSettingsStorage::getLastFMNameAndPW () { return this -> m_lastFm; }
void CSettingsStorage::getLastFMNameAndPW (QString & name, QString & pw) {
    name= this -> m_lastFm.first;
    pw= this -> m_lastFm.second;
}
void CSettingsStorage::setLastFMNameAndPW (const QString & name,const QString & pw) {
    this -> m_lastFm.first = name;
    this -> m_lastFm.second = pw;
}


void CSettingsStorage::setLastFMCorrections(bool b){
	m_lfm_corrections = b;
}

bool CSettingsStorage::getLastFMCorrections(){
	return m_lfm_corrections;
}


EQ_Setting CSettingsStorage::getCustomEqualizer(){ 
	if(m_vec_eqSettings.size() == 0) {
		EQ_Setting setting;
		return setting;
	}
	return this->m_vec_eqSettings[m_vec_eqSettings.size()-1]; }
void CSettingsStorage::getEqualizerSettings(vector<EQ_Setting>& vec){vec = this->m_vec_eqSettings;}
void CSettingsStorage::setEqualizerSettings(const vector<EQ_Setting>& vec){

	m_vec_eqSettings.clear();
	for(uint i=0; i<vec.size(); i++){
		m_vec_eqSettings.push_back(vec[i]);
	}
}


int CSettingsStorage::getVolume(){ return m_volume; }
void CSettingsStorage::setVolume(int vol){ if(vol >= 0 && vol <= 100) m_volume = vol; }

void CSettingsStorage::setLastEqualizer(int eq_idx){ m_last_eq = eq_idx; }
int CSettingsStorage::getLastEqualizer(){ return m_last_eq;}

QString CSettingsStorage::getLibraryPath(){return m_library_path;}
void CSettingsStorage::setLibraryPath(QString path){m_library_path = path;}

QSize CSettingsStorage::getPlayerSize(){ return m_player_size; }
void CSettingsStorage::setPlayerSize(QSize size){ m_player_size = size; }

QString CSettingsStorage::getPlaylist(){
	return m_playlist;
}
void CSettingsStorage::setPlaylist(QString playlist){
	m_playlist = playlist;
}

bool CSettingsStorage::getLoadPlaylist(){
	return m_loadPlaylist;
}

void CSettingsStorage::setLoadPlaylist(bool b){
	m_loadPlaylist = b;
}

void CSettingsStorage::setPlaylistMode(const Playlist_Mode& plmode){
	m_playlistmode = plmode;
}

Playlist_Mode CSettingsStorage::getPlaylistMode(){
	return m_playlistmode;
}

void CSettingsStorage::setPlayerStyle(int style){
	m_style = style;
}

int CSettingsStorage::getPlayerStyle(){
	return m_style;
}

void CSettingsStorage::setShowNotifications(bool active){
	m_show_notifications = active;
}

bool CSettingsStorage::getShowNotification(){
	return m_show_notifications;
}

void CSettingsStorage::setLastFMSessionKey(QString key){
	m_lfm_sessionKey = key;
}
QString CSettingsStorage::getLastFMSessionKey(){
	return m_lfm_sessionKey;
}

void CSettingsStorage::setShowLibrary(bool b){
	m_show_library = b;
}

bool CSettingsStorage::getShowLibrary(){
	return m_show_library;
}

void CSettingsStorage::setShownPlugin(int plugin){
    m_shown_plugin = plugin;
}

int CSettingsStorage::getShownPlugin(){
	return m_shown_plugin;
}

void CSettingsStorage::setMinimizeToTray(bool b){
	m_minimize_to_tray = b;
}
bool CSettingsStorage::getMinimizeToTray(){
	return m_minimize_to_tray;
}

bool CSettingsStorage::getShowSmallPlaylist(){
	return m_show_small_playlist;
}

void CSettingsStorage::setShowSmallPlaylist(bool b){
	m_show_small_playlist = b;
}

QString CSettingsStorage::getSoundEngine(){
	return m_sound_engine;
}

void CSettingsStorage::setSoundEngine(QString engine){
	m_sound_engine = engine;
}

bool CSettingsStorage::getStreamRipper(){
	return m_streamripper;
}

void CSettingsStorage::setStreamRipper(bool b){
	m_streamripper = b;
}

bool CSettingsStorage::getStreamRipperWarning(){
	return	m_streamripper_warning;
}

void CSettingsStorage::setStreamRipperWarning(bool b){
	m_streamripper_warning = b;
}

QString CSettingsStorage::getStreamRipperPath(){
	return m_streamripper_path;
}

void CSettingsStorage::setStreamRipperPath(QString path){
	m_streamripper_path = path;
}

bool CSettingsStorage::getStreamRipperCompleteTracks(){
	return m_streamripper_complete_tracks;
}

void CSettingsStorage::setStreamRipperCompleteTracks(bool b){
	m_streamripper_complete_tracks = b;
}

bool CSettingsStorage::getStreamRipperPlaylist(){
	return m_streamripper_playlist;
}

void CSettingsStorage::setStreamRipperPlaylist(bool b){
	m_streamripper_playlist = b;
}

bool CSettingsStorage::getSocketActivated(){
	return m_socket_activated;
}
void CSettingsStorage::setSocketActivated(bool b){
	m_socket_activated = b;
}

int CSettingsStorage::getSocketFrom(){
	return m_socket_from;
}
void CSettingsStorage::setSocketFrom(int val){
	m_socket_from = val;
}

int CSettingsStorage::getSocketTo(){
	return m_socket_to;
}

void CSettingsStorage::setSocketTo(int val){
	m_socket_to = val;
}

bool CSettingsStorage::getPlaylistNumbers(){
	return m_show_playlist_numbers;
}

void CSettingsStorage::setPlaylistNumbers(bool b){
	m_show_playlist_numbers = b;
}





