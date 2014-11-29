/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "videowidget.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStringList>
#include <QTimer>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QSlider;
class QVideoProbe;
class QVideoWidget;
class QStatusBar;
QT_END_NAMESPACE

class PlaylistModel;
class HistogramWidget;
class HttpFileDownloader;
class Player : public QWidget
{
    Q_OBJECT

public:
    Player(QWidget *parent = 0);
    ~Player();

    bool loadMusicListFileNames();
    bool saveMusicListFileNames();

    QTimer danmuConnectionTimer;

    QStringList danmuHistory;
signals:
    void fullScreenChanged(bool fullScreen);
    void informUser(QString s);
public slots:
    void getDanmu(QString s);
    void playNextSong();
protected:
    void	closeEvent(QCloseEvent * event);
private slots:
    void resumeMute();
    void setPlayInfo();
    void refreshOrderList();
    void addHintInfo(QString s);
    void remoteInfo(QString s);

    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();



    QString getMp3UrlFromBaidu(QString query);
    QString getMp3UrlFromXiami(QString query);

    void previousClicked();

    void seek(int seconds);
    void jump1(const QModelIndex &index);
    void jump2(const QModelIndex &index);
    void playlistPositionChanged1(int);
    void playlistPositionChanged2(int);
    void stateChanged(QMediaPlayer::State state);
    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);

    void displayErrorMessage();

#ifndef PLAYER_NO_COLOROPTIONS
    void showColorDialog();
#endif
    void addToPlaylist(const QStringList &fileNames);

private:
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);


    HttpFileDownloader * downloader;

    QStringList musicFileNames;

    QMediaPlayer *player;
    QMediaPlaylist *playlist1, * playlist2;
    VideoWidget *videoWidget;
    QLabel *coverLabel;
    QSlider *slider;
    QLabel *labelDuration;
    QPushButton *acceptOrderButton;
    QPushButton *userSwitchButton;
#ifndef PLAYER_NO_COLOROPTIONS
    QPushButton *colorButton;
    QDialog *colorDialog;
#endif

    QStatusBar * statusBar;

    QLabel *labelHistogram;
    HistogramWidget *histogram;
    QVideoProbe *probe;

    PlaylistModel *playlistModel1,*playlistModel2;
    QAbstractItemView *playlistView1,*playlistView2;
    QString trackInfo;
    QString statusInfo;
    QString orderInfo;
    QString netAuthor, netTitle;
    qint64 duration;

    QTimer loadingTimer ;

    bool continuePlayFlag;
    bool manualChangeSongFlag;
};

#endif // PLAYER_H
