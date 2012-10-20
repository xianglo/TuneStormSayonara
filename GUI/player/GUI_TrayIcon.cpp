/* GUI_TrayIcon.cpp */

/* Copyright (C) 2012  gleugner
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

#include "HelperStructs/MetaData.h"
#include "HelperStructs/Helper.h"
#include "GUI/player/GUI_TrayIcon.h"
#include <QAction>
#include <QMenu>
#include <QEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QIcon>
#include <QPixmap>



GUI_TrayIcon::GUI_TrayIcon (const QIcon & playIcon, const QIcon & pauseIcon, QObject *parent) : QSystemTrayIcon (parent), MESSAGE_TIMEOUT_MS (5000) {

    QPixmap play_pixmap = playIcon.pixmap(24, 24);
    m_playIcon = QIcon(play_pixmap);

    QPixmap pause_pixmap = pauseIcon.pixmap(24, 24);
    m_pauseIcon = QIcon(pause_pixmap);

    this -> setToolTip("Sayonara - Music - Player");
    this -> setIcon(playIcon);
}


void GUI_TrayIcon::setupMenu (    QAction* closeAction,
                                  QAction* playAction,
                                  QAction* stopAction,
                                  QAction* muteAction,
                                  QAction* fwdAction,
                                  QAction* bwdAction,
                                  QAction* showAction) {
    QMenu* trayContextMenu = new QMenu();
    trayContextMenu->addAction(playAction);
    trayContextMenu->addAction(stopAction);
    trayContextMenu->addSeparator();
    trayContextMenu->addAction(fwdAction);
    trayContextMenu->addAction(bwdAction);
    trayContextMenu->addSeparator();
    trayContextMenu->addAction(muteAction);
    trayContextMenu->addSeparator();
    trayContextMenu->addAction(showAction);
    trayContextMenu->addAction(closeAction);
    this ->  setContextMenu(trayContextMenu);
}

bool GUI_TrayIcon::event ( QEvent * e ) {
    if (e->type ()== QEvent::Wheel) {
        QWheelEvent * wheelEvent = dynamic_cast <QWheelEvent *> (e);
        emit onVolumeChangedByWheel (wheelEvent->delta());
    }
    return true;
}

void GUI_TrayIcon::songChangedMessage (const QString & message) {
    if (this -> isSystemTrayAvailable()) {
        this -> showMessage("Sayonara",message,QSystemTrayIcon::Information,MESSAGE_TIMEOUT_MS);
    }
    this -> setToolTip(message);
}

void GUI_TrayIcon::playStateChanged (bool playing) {
    if(playing) {
        this -> setIcon(this->m_playIcon);
    }
    else {
        this -> setIcon(this -> m_pauseIcon);
    }
}

void GUI_TrayIcon::trackChanged(const MetaData& md){
	this->setToolTip(md.title + " by " + md.artist);
}

GUI_TrayIcon::~GUI_TrayIcon() {

}
