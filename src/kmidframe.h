/**************************************************************************

    kmidframe.h  - The main widget of KMid
    Copyright (C) 1997,98  Antonio Larrosa Jimenez

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    Send comments and bug fixes to larrosa@kde.org
    or to Antonio Larrosa, Rio Arnoya, 10 5B, 29006 Malaga, Spain

***************************************************************************/
#ifndef _KMIDFRAME_H
#define _KMIDFRAME_H

#include <kxmlguiwindow.h>
#include <kmenubar.h>
#include <libkmid/midiout.h>
#include <libkmid/player.h>
#include <libkmid/track.h>
#include <QTimer>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QDropEvent>
#include <kdemacros.h>

class KConfig;
class kmidClient;
class KAccel;

class KDE_EXPORT kmidFrame : public KXmlGuiWindow
{
    Q_OBJECT
private:
#ifdef KMidDEBUG
    long passcount;
#endif

    MidiOut *Midi;
    MidiPlayer *Player;
/*
    int playerProcessID;
    PlayerController *pctl;

    int donttoggle;

    QPopupMenu *m_file;
    QPopupMenu *m_song;
    QPopupMenu *m_collections;
    QPopupMenu *m_options;
    QPopupMenu *m_help;
*/
    KAccel *kKeysAccel;
protected:
    int autoAddSongToCollection(const QString& filename=QString::null,int setactive=1);


    virtual void saveProperties(KConfigGroup &kcfg);
    virtual void readProperties(const KConfigGroup &kcfg);

//    virtual void closeEvent(QCloseEvent *e);

    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent ( QDropEvent * event );


public:
    kmidFrame();
    virtual ~kmidFrame();

public slots:

    void file_Open();
    void file_SaveLyrics();
    void song_stopPause(); // release the pause button and quit the pause mode
    void song_Loop();
    void collect_organize();
    void collect_PlayOrder(int i);
    void collect_AutoAdd();
    void options_FileType(int i);
    void options_DisplayEvents(int i);
    void options_AutomaticText();
    void options_ShowVolumeBar();
    void options_ShowChannelView();
    void options_ChannelViewOptions();
    void options_FontChange();

    void options_MidiSetup();
    void spacePressed();


   virtual void openUrl( const QString s );
/*   virtual void play();
   virtual void pause();
   virtual void stop();
   virtual void rewind();
   virtual void forward();
   virtual void seek(int ms);
   virtual void prevSong();
   virtual void nextSong();
   virtual void setSongLoop(int i);
   virtual void setVolume(int i);
   virtual void setTempo(int i);
   virtual void setSongType( int i );
   virtual void setLyricEvents( int i );
   virtual void selectSong(int i);
   virtual void setActiveCollection( int i );
   virtual void setCollectionPlayMode(int i);
   virtual void setMidiDevice(int i);
*/
    void urlDrop_slot(class QDropEvent *);

    void rechooseTextEvent();

    void channelViewDestroyed();
    void shuttingDown();

private:
    kmidClient *kmidclient;
};

#endif
