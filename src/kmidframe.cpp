/**************************************************************************

    kmidframe.cpp  - The main widget of KMid
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
    Foundation, Inc., 59 Temple

    Send comments and bug fixes to larrosa@kde.org
    or to Antonio Larrosa, Rio Arnoya, 10 5B, 29006 Malaga, Spain

***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <QtDBus>
#include <qnamespace.h>
#include <QString>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QDropEvent>

#include <kactioncollection.h>
#include <kselectaction.h>
#include <ktoggleaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kcmdlineargs.h>
#include <kfiledialog.h>
#include <kfontdialog.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktoolbar.h>
#include <kurl.h>
#include <kdebug.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kedittoolbar.h>
#include <kicon.h>

#include "kmidframe.h"
#include "kmidclient.h"
#include "midicfgdlg.h"
#include "collectdlg.h"
#include "channelcfgdlg.h"
#include "channelview.h"
#include "version.h"

kmidFrame::kmidFrame()
    :KXmlGuiWindow()
{
    kmidclient=new kmidClient(this,actionCollection());
    kmidclient->setSongType(1);
    kmidclient->show();
    setCentralWidget( kmidclient );
/*
    kKeysAccel=new KAccel(this);
    kKeysAccel->insertItem(i18n("Play/Pause"),"Play/Pause", Key_Space);
    kKeysAccel->connectItem("Play/Pause", this, SLOT(spacePressed()));
    kKeysAccel->insertItem(i18n("Stop"),"Stop", Key_Backspace);
    kKeysAccel->connectItem("Stop",kmidclient,SLOT(song_Stop()));
    kKeysAccel->insertItem(i18n("Previous Song"),"Previous Song", Key_Left);
    kKeysAccel->connectItem("Previous Song",kmidclient,SLOT(song_PlayPrevSong()));
    kKeysAccel->insertItem(i18n("Next Song"),"Next Song", Key_Right);
    kKeysAccel->connectItem("Next Song",kmidclient,SLOT(song_PlayNextSong()));
    kKeysAccel->insertItem(i18n("Scroll Down Karaoke"),"Scroll down karaoke",Key_Down);
    kKeysAccel->connectItem("Scroll Down karaoke",kmidclient->getKDisplayText(),SLOT(ScrollDown()));
    kKeysAccel->insertItem(i18n("Scroll Up Karaoke"),"Scroll up karaoke",Key_Up);
    kKeysAccel->connectItem("Scroll Up Karaoke",kmidclient->getKDisplayText(),SLOT(ScrollUp()));
    kKeysAccel->insertItem(i18n("Scroll Page Down Karaoke"),"Scroll page down karaoke",Key_PageDown);
    kKeysAccel->connectItem("Scroll Page Down Karaoke",kmidclient->getKDisplayText(),SLOT(ScrollPageDown()));
    kKeysAccel->insertItem(i18n("Scroll Page Up Karaoke"),"Scroll page up karaoke",Key_PageUp);
    kKeysAccel->connectItem("Scroll Page Up Karaoke",kmidclient->getKDisplayText(),SLOT(ScrollPageUp()));

    kKeysAccel->readSettings();
*/
    KStandardAction::open(this, SLOT(file_Open()), actionCollection());
    QAction *action = actionCollection()->addAction("file_save_lyrics");
    action->setText(i18n("&Save Lyrics..."));
    connect(action, SIGNAL(triggered(bool) ), SLOT(file_SaveLyrics()));
    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

    action = actionCollection()->addAction("song_play");
    action->setIcon(KIcon("media-playback-start"));
    action->setText(i18n("&Play"));
    connect(action, SIGNAL(triggered(bool)), kmidclient, SLOT(slotPlay()));
    action->setShortcut(Qt::Key_Space);
    action = actionCollection()->addAction("song_pause");
    action->setIcon(KIcon("media-playback-pause"));
    action->setText(i18n("P&ause"));
    connect(action, SIGNAL(triggered(bool)), kmidclient, SLOT(slotPause()));
    action->setShortcut(Qt::Key_P);
    action = actionCollection()->addAction("song_stop");
    action->setIcon(KIcon("media-playback-stop"));
    action->setText(i18n("&Stop"));
    connect(action, SIGNAL(triggered(bool)), kmidclient, SLOT(slotStop()));
    action->setShortcut(Qt::Key_Backspace);

    action = actionCollection()->addAction("song_previous");
    action->setIcon(KIcon("media-skip-backward"));
    action->setText(i18n("P&revious Song"));
    connect(action, SIGNAL(triggered(bool) ), kmidclient, SLOT(slotPrevSong()));
    action->setShortcut(Qt::Key_Left);
    action = actionCollection()->addAction("song_next");
    action->setIcon(KIcon("media-skip-forward"));
    action->setText(i18n("&Next Song"));
    connect(action, SIGNAL(triggered(bool) ), kmidclient, SLOT(slotNextSong()));
    action->setShortcut(Qt::Key_Right);
    action = actionCollection()->add<KToggleAction>("song_loop");
    action->setText(i18n("&Loop"));
    connect(action, SIGNAL(triggered(bool) ), SLOT(song_Loop()));

    action = actionCollection()->addAction("song_rewind");
    action->setIcon(KIcon("media-seek-backward"));
    action->setText(i18n("Rewind"));
    connect(action, SIGNAL(triggered(bool) ), kmidclient, SLOT(slotRewind()));

    action = actionCollection()->addAction("song_forward");
    action->setIcon(KIcon("media-seek-forward"));
    action->setText(i18n("Forward"));
    connect(action, SIGNAL(triggered(bool) ), kmidclient, SLOT(slotForward()));

    action = actionCollection()->addAction("collect_organize");
    action->setText(i18n("&Organize..."));
    connect(action, SIGNAL(triggered(bool) ), SLOT(collect_organize()));

    QStringList playmodes;
    playmodes.append(i18n("In Order"));
    playmodes.append(i18n("Shuffle"));

    KSelectAction *act=actionCollection()->add<KSelectAction>("play_order");
    act->setText(i18n("Play Order"));
    connect(act,SIGNAL(activated(int)),this, SLOT(collect_PlayOrder(int)));
    act->setItems(playmodes);

    KToggleAction* autoAddAct = actionCollection()->add<KToggleAction>("collect_autoadd");
    autoAddAct->setText(i18n("Auto-Add to Collection"));
    connect( autoAddAct, SIGNAL( triggered() ), this, SLOT( collect_AutoAdd() ) );

    playmodes.clear();
    playmodes.append(i18n("&General MIDI"));
    playmodes.append(i18n("&MT-32"));

    act=actionCollection()->add<KSelectAction>("file_type");
    act->setText(i18n("File Type"));
    connect(act,SIGNAL(activated(int)),this, SLOT(options_FileType(int)));
    act->setItems(playmodes);

    playmodes.clear();
    playmodes.append(i18n("&Text Events"));
    playmodes.append(i18n("&Lyric Events"));

    act=actionCollection()->add<KSelectAction>("display_events");
    act->setText(i18n("Display Events"));
    act->setShortcuts(KShortcut(Qt::Key_T));
    connect(act,SIGNAL(activated(int)),this, SLOT(options_DisplayEvents(int)));
    act->setItems(playmodes);

    action = actionCollection()->add<KToggleAction>("option_automatictext");
    action->setText(i18n("Automatic Text Chooser"));
    connect(action, SIGNAL(triggered(bool) ), SLOT(options_AutomaticText()));

    KToggleAction *togact = actionCollection()->add<KToggleAction>("toggle_volumebar");
    togact->setIcon(KIcon("volume"));
    togact->setText(i18n("Show &Volume Bar"));
    connect(togact, SIGNAL(triggered(bool) ), SLOT(options_ShowVolumeBar()));
    togact->setCheckedState(KGuiItem(i18n("Hide &Volume Bar")));

    togact = actionCollection()->add<KToggleAction>("toggle_channelview");
    togact->setIcon(KIcon("piano"));
    togact->setText(i18n("Show &Channel View"));
    connect(togact, SIGNAL(triggered(bool) ), SLOT(options_ShowChannelView()));
    togact->setCheckedState(KGuiItem(i18n("Hide &Channel View")));

    action = actionCollection()->addAction("channelview_options");
    action->setText(i18n("Channel View &Options..."));
    connect(action, SIGNAL(triggered(bool)), SLOT(options_ChannelViewOptions()));

    action = actionCollection()->addAction("change_font");
    action->setText(i18n("&Font Change..."));
    connect(action, SIGNAL(triggered(bool)), SLOT(options_FontChange()));

    action = actionCollection()->addAction("midi_setup");
    action->setText(i18n("MIDI &Setup..."));
    connect(action, SIGNAL(triggered(bool) ), SLOT(options_MidiSetup()));


    setupGUI((ToolBar | Keys | StatusBar | Save | Create ), "kmidui.rc" );

    KConfigGroup cfg( KGlobal::config(), "KMid");
    KSelectAction *tmplistaction=
		((KSelectAction*)actionCollection()->action("display_events"));

    if (cfg.readEntry("TypeOfTextEvents",5)==5)
        tmplistaction->setCurrentItem(1);
    else
        tmplistaction->setCurrentItem(0);

    tmplistaction=((KSelectAction*)actionCollection()->action("file_type"));
    if (cfg.readEntry("TypeOfMidiFile",0)==0)
        tmplistaction->setCurrentItem(0);
    else
        tmplistaction->setCurrentItem(1);

    if (cfg.readEntry("Loop",0)==1)
	((KToggleAction*)actionCollection()->action("song_loop"))->setChecked(true);

    if (cfg.readEntry("ShowVolumeBar",0)==1)
	((KToggleAction*)actionCollection()->action("toggle_volumebar"))->setChecked(true);


    tmplistaction=((KSelectAction*)actionCollection()->action("play_order"));
    if (cfg.readEntry("CollectionPlayMode",0)==0)
        tmplistaction->setCurrentItem(0);
    else
        tmplistaction->setCurrentItem(1);

    if ((cfg.readEntry("AutoAddToCollection",0))==1)
	((KToggleAction*)actionCollection()->action("collect_autoadd"))->setChecked(true);

    if ((cfg.readEntry("AutomaticTextEventChooser",1))==1)
	((KToggleAction*)actionCollection()->action("option_automatictext"))->setChecked(true);

    setAcceptDrops(true);

    connect( kmidclient, SIGNAL( mustRechooseTextEvent() ),
             this, SLOT( rechooseTextEvent() ) );

    connect( kmidclient, SIGNAL( stopPause() ),
             this, SLOT( song_stopPause() ) );

//    connect( kmidclient, SIGNAL( channelView_Destroyed() ),
//             this, SLOT( channelViewDestroyed() ) );

    KCmdLineArgs * args = KCmdLineArgs::parsedArgs();

    if ( args->count() > 0 )
    {
//        printf("Opening command line file...\n");
        int backautoadd=cfg.readEntry("AutoAddToCollection",0);
        cfg.writeEntry("AutoAddToCollection",0);

        char ttt[40];
        sprintf(ttt,"%d",kapp->argc());
        int i=0;
        int c=autoAddSongToCollection( args->url( 0 ).path() , 1 );
        i++;
        while (i<args->count())
        {
            autoAddSongToCollection( args->url( i ).path() , 0 );
            i++;
        }

        kmidclient->setActiveCollection(c);

        ///	    kmidclient->openUrl((kapp->argv())[1]);
        /*	    if ((cfg.readEntry("AutomaticTextEventChooser",1))==1)
         {
         if (kmidclient->ChooseTypeOfTextEvents()==1)
         options_Text();
         else
         options_Lyrics();
         }*/
        if (kmidclient->midiFileName()!=NULL) kmidclient->play();
        cfg.writeEntry("AutoAddToCollection",backautoadd);
    }

    args->clear();
        /*
         kKeys->addKey("Play/Pause",Key_Space);
         kKeys->registerWidget("KMidFrame",this);
         kKeys->connectFunction("KMidFrame","Play/Pause",this,SLOT(spacePressed()));
         */
//    kKeysAccel->writeSettings(cfg);

	QDBusConnection::sessionBus().registerObject(QLatin1String("/kmid"), this);
	QDBusConnection::sessionBus().connectToBus(QDBusConnection::SessionBus, "org.kde.kmid");
}

kmidFrame::~kmidFrame()
{
}

void kmidFrame::file_Open()
{
  KUrl url = KFileDialog::getOpenUrl(KUrl(), "*.kar *.mid *.kar.gz *.mid.gz\n*.kar *.kar.gz\n*.mid *.mid.gz\n*",this);

  if( url.isEmpty() )
    return;

  if( !url.isLocalFile() )
  {
    KMessageBox::sorry( 0L, i18n( "Only local files are currently supported." ) );
    return;
  }

  openUrl(url.path());
}



void kmidFrame::song_stopPause()
{
  if (kmidclient->isPaused())
  {
    //   song_Pause();
    ((KToggleAction*)actionCollection()->action("song_pause"))->setChecked(false);
    kmidclient->pause();
  }
}

void kmidFrame::options_FileType(int i)
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  cfg.writeEntry("TypeOfMidiFile",i);
  kmidclient->setSongType(1-i);
}

void kmidFrame::options_DisplayEvents(int i)
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  cfg.writeEntry("TypeOfTextEvents",(i==0)?1:5);
  cfg.sync();
  kmidclient->repaintText((i==0)?1:5);
}

void kmidFrame::options_AutomaticText()
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  cfg.writeEntry("AutomaticTextEventChooser",1-cfg.readEntry("AutomaticTextEventChooser",1));
}

void kmidFrame::options_FontChange()
{
  KFontDialog *kfd=new KFontDialog(this);
  QFont font;
  font=*kmidclient->getFont();
  kfd->getFont(font);
  delete kfd;
  KConfigGroup cfg(KGlobal::config(), "KMid");
  cfg.writeEntry("KaraokeFont",font);
  cfg.sync();
  kmidclient->fontChanged();
}

int kmidFrame::autoAddSongToCollection(const QString& filename,int setactive)
{
  int r;
  SLManager *slman;
  SongList *sl;
  KConfigGroup cfg(KGlobal::config(), "KMid");
  if (cfg.readEntry("AutoAddToCollection",0)==0)
  {
    r=0;
    slman=kmidclient->getSLManager();
    if (setactive) slman->createTemporaryCollection();
    sl=slman->getCollection(0);
    if (filename==NULL) sl->AddSong(kmidclient->midiFileName());
    else sl->AddSong(QFile::encodeName(filename));
  }
  else
  {
    slman=kmidclient->getSLManager();
    sl=slman->getCollection(kmidclient->getActiveCollection());
    r=kmidclient->getActiveCollection();
    if (sl==NULL) return 0;
    int id;
    if (filename==NULL) id=sl->AddSong(kmidclient->midiFileName());
    else id=sl->AddSong(QFile::encodeName(filename));
    if (setactive)
    {
      sl->setActiveSong(id);
    }
  }
  return r;
}

void kmidFrame::urlDrop_slot(QDropEvent* e)
{
  dropEvent(e);
}

void kmidFrame::dragEnterEvent(QDragEnterEvent* e)
{
  e->setAccepted(KUrl::List::canDecode(e->mimeData()));
}

void kmidFrame::dropEvent( QDropEvent * event )
{
  KUrl::List list = KUrl::List::fromMimeData( event->mimeData() );

  if (list.isEmpty()) return;

  bool first = true;
  int c = true;

  for (KUrl::List::ConstIterator it = list.begin();
       it != list.end(); ++it)
  {
     if (!(*it).isLocalFile())
        continue;

     if (first)
     {
         c = autoAddSongToCollection((*it).path(),1);
         first = false;
     }
     else
     {
         autoAddSongToCollection((*it).path(),0);
     }
  }

  if (!first)
  {
     kmidclient->setActiveCollection(c);

     if ((!kmidclient->isPlaying())&&(kmidclient->midiFileName()!=NULL))
		kmidclient->play();
  }
}

void kmidFrame::shuttingDown()
{
   if (kmidclient->isPlaying()==1)
   {
//      kmidclient->stop();
      kmidclient->shuttingDown();
   }
   kmidclient->saveCollections();
}

void kmidFrame::saveProperties(KConfigGroup &cfg)
{
  kmidclient->saveCollections();
  int play=kmidclient->isPlaying();
  cfg.writeEntry("File", QFile::decodeName(kmidclient->midiFileName()));
  cfg.writeEntry("ActiveCollection",int(kmidclient->getActiveCollection()));
  cfg.writeEntry("ActiveSong",kmidclient->getSelectedSong());
  cfg.writeEntry("Playing",play);
}

void kmidFrame::readProperties(const KConfigGroup &cfg)
{
  int activecol=cfg.readEntry("ActiveCollection",0);
  int activesong=cfg.readEntry("ActiveSong",0);
  int wasplaying=cfg.readEntry("Playing",0);
  SLManager *slman=kmidclient->getSLManager();
  SongList *sl=slman->getCollection(activecol);
  sl->setActiveSong(activesong);
  kmidclient->setActiveCollection(activecol);
  kmidclient->slotSelectSong(activesong-1);

  if ((activecol==0)&&(wasplaying)) // It was the temporary collection,
  {		// surely the user would like to continue
    // hearing the song
    QString s = cfg.readPathEntry("File", QString());
    int c=autoAddSongToCollection(s,1);
    kmidclient->setActiveCollection(c);
  }

  if ((wasplaying)&&(kmidclient->midiFileName()!=NULL)) kmidclient->play();
}

void kmidFrame::options_MidiSetup()
{
  if (kmidclient->devman()->checkInit()<0)
  {
    KMessageBox::error(this,
	i18n("Could not open /dev/sequencer to get some info.\nProbably there is another program using it."));
    return;
  }
  MidiConfigDialog *dlg;

  dlg=new MidiConfigDialog(kmidclient->devman(),NULL,"MidiDialog");
  if (dlg->exec() == QDialog::Accepted)
  {
    KConfigGroup cfg(KGlobal::config(), "KMid");
    cfg.writeEntry("MidiPortNumber",MidiConfigDialog::selecteddevice);
    kmidclient->setMidiDevice(MidiConfigDialog::selecteddevice);
    cfg.changeGroup("Midimapper");
    cfg.writePathEntry("LoadFile",
	(MidiConfigDialog::selectedmap==NULL)? QString():MidiConfigDialog::selectedmap);
    kmidclient->setMidiMapFilename(MidiConfigDialog::selectedmap);
  }
  delete dlg;
}

void kmidFrame::collect_organize()
{
  CollectionDialog *dlg;
  SLManager *slman=new SLManager(*kmidclient->getSLManager());
  int idx=kmidclient->getActiveCollection();

  dlg=new CollectionDialog(slman,idx,NULL,"MidiDialog");
  if (dlg->exec() == QDialog::Accepted)
    {
    kmidclient->setSLManager(slman);
    kmidclient->setActiveCollection(CollectionDialog::selectedC);
    kmidclient->slotSelectSong(
     (slman->getCollection(CollectionDialog::selectedC))->getActiveSongID()-1);
    }
   else
    {
    delete slman;
    }
delete dlg;
}

void kmidFrame::rechooseTextEvent()
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  if ((cfg.readEntry("AutomaticTextEventChooser",1))==1)
  {
    int t=kmidclient->ChooseTypeOfTextEvents();
    kmidclient->repaintText(t);
    if (t==1)
      ((KSelectAction*)actionCollection()->action("display_events"))->setCurrentItem(0);
    else
      ((KSelectAction*)actionCollection()->action("display_events"))->setCurrentItem(1);
  }
}

void kmidFrame::song_Loop()
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  int i=1-cfg.readEntry("Loop",0);
  cfg.writeEntry("Loop",i);
  cfg.sync();
  kmidclient->setSongLoop(i);
}

void kmidFrame::collect_PlayOrder(int i)
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  cfg.writeEntry("CollectionPlayMode", i);
  cfg.sync();
  kmidclient->setCollectionPlayMode(i);
}

void kmidFrame::collect_AutoAdd()
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  cfg.writeEntry("AutoAddToCollection",1-cfg.readEntry("AutoAddToCollection",0));
  cfg.sync();
}

void kmidFrame::file_SaveLyrics()
{
  KUrl url = KFileDialog::getSaveUrl(KUrl(),"*",this);

  if( url.isEmpty() )
    return;

  if( !url.isLocalFile() )
  {
    KMessageBox::sorry( 0L, i18n( "Only local files are currently supported." ) );
    return;
  }

  QString filename = url.path();
  struct stat statbuf;

  if (stat(QFile::encodeName(filename), &statbuf)!=-1)
  {
    QString s = i18n("File %1 already exists\nDo you want to overwrite it?", filename);
    if (KMessageBox::warningYesNo(this,s,QString::null,KGuiItem(i18n("Overwrite")),KStandardGuiItem::cancel())==1)
      return;
  }

  FILE *fh=fopen(QFile::encodeName(filename),"wt");
  kmidclient->saveLyrics(fh);
  fclose(fh);
}

void kmidFrame::spacePressed()
{
  if (!kmidclient->isPlaying()) kmidclient->play();
  else pause();
}
void kmidFrame::options_ShowVolumeBar()
{
  KConfigGroup cfg(KGlobal::config(), "KMid");
  int i=1-cfg.readEntry("ShowVolumeBar",0);
  cfg.writeEntry("ShowVolumeBar",i);
  kmidclient->visibleVolumeBar(i);
}

void kmidFrame::options_ShowChannelView()
{
  if (!((KToggleAction*)actionCollection()->action("toggle_channelview"))->isChecked())
  {
    kmidclient->visibleChannelView(0);
  } else
  {
    kmidclient->visibleChannelView(1);
    connect (kmidclient->getChannelView(),SIGNAL(destroyMe()),this,SLOT(channelViewDestroyed()));
  }

}

void kmidFrame::channelViewDestroyed()
{
  kmidclient->channelViewDestroyed();
  ((KToggleAction*)actionCollection()->action("toggle_channelview"))->setChecked(false);
}

void kmidFrame::options_ChannelViewOptions()
{
  ChannelViewConfigDialog *dlg;

  dlg=new ChannelViewConfigDialog(NULL,"ChannelViewConfigDialog");
  if (dlg->exec() == QDialog::Accepted)
  {
    if (kmidclient->getChannelView())
      kmidclient->getChannelView()->lookMode(
	  ChannelViewConfigDialog::selectedmode );
  }
  delete dlg;
}

void kmidFrame::openUrl( const QString url )
{
  int c=autoAddSongToCollection(url,1);
  kmidclient->setActiveCollection(c);
}

/*void kmidFrame::play()
{
  kmidclient->play();
}

void kmidFrame::pause()
{
//  kmidclient->pause();
  ((KAction*)actionCollection()->action("pause"))->activate();
}

void kmidFrame::stop()
{
  kmidclient->stop();
}

void kmidFrame::rewind()
{
  kmidclient->rewind();
}

void kmidFrame::forward()
{
  kmidclient->forward();
}

void kmidFrame::seek(int ms)
{
  kmidclient->timebarChange(ms);
}

void kmidFrame::prevSong()
{
  kmidclient->prevSong();
}

void kmidFrame::nextSong()
{
  kmidclient->nextSong();
}

void kmidFrame::setSongLoop(int i)
{
  ((KToggleAction*)actionCollection()->action("song_loop"))->setChecked(i!=0);
}

void kmidFrame::setVolume(int i)
{
  kmidclient->volumebarChange(200-i);
}

void kmidFrame::setTempo(int i)
{
  kmidclient->changeTempo(i);
}

void kmidFrame::setSongType( int i )
{
  KSelectAction *tmplistaction=
    ((KSelectAction*)actionCollection()->action("file_type"));

  tmplistaction->setCurrentItem(i);
}

void kmidFrame::setLyricEvents( int i )
{
  KSelectAction *tmplistaction=
    ((KSelectAction*)actionCollection()->action("display_events"));
  tmplistaction->setCurrentItem(i);
}

void kmidFrame::selectSong(int i)
{
  kmidclient->getComboSongs()->setCurrentItem(i-1);
  kmidclient->selectSong(i-1);
}

void kmidFrame::setActiveCollection( int i )
{
  kmidclient->setActiveCollection(i);
}

void kmidFrame::setCollectionPlayMode(int i)
{
  ((KSelectAction*)actionCollection()->action("play_order"))->setCurrentItem(i);
}

void kmidFrame::setMidiDevice(int i)
{


}
*/


#include "kmidframe.moc"
