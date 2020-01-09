/**************************************************************************

    midicfgdlg.cpp  - The midi config dialog
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
#include <QPushButton>
#include <q3listbox.h>
#include <QLabel>
#include <QLayout>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <kapplication.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <libkmid/deviceman.h>

#include "midicfgdlg.h"
#include "version.h"
#include <kglobal.h>
#include <kstandarddirs.h>

MidiConfigDialog::MidiConfigDialog(DeviceManager *dm,
    QWidget *parent,const char *name)
    : KDialog(parent)
{
    setObjectName(name);
    setCaption( i18n("Configure MIDI Devices") );
    setButtons( KDialog::Ok|KDialog::Cancel);
    setModal( true );
  setMinimumSize(360,240);
  QWidget *page = new QWidget( this );
  setMainWidget(page);

  QVBoxLayout *topLayout=new QVBoxLayout(page);
  topLayout->setSpacing(spacingHint());
  topLayout->setMargin(0);
  QLabel *label=new QLabel(i18n("Select the MIDI device you want to use:"),page);
  topLayout->addWidget(label);
  mididevices=new Q3ListBox(page,"midideviceslist");
  topLayout->addWidget(mididevices,3);
  connect(mididevices,SIGNAL(highlighted(int)),SLOT(deviceselected(int)));
  devman=dm;
  QString temp;
  for (int i=0;i<devman->midiPorts()+devman->synthDevices();i++)
  {
    if (strcmp(devman->type(i),"")!=0)
      temp = QString("%1 - %2").arg(devman->name(i)).arg(devman->type(i));
    else
      temp = devman->name(i);

    mididevices->insertItem(temp,i);
  };
  selecteddevice=devman->defaultDevice();
  mididevices->setCurrentItem(selecteddevice);

  QLabel *label2=new QLabel(i18n("Use the MIDI map:"),page);
  topLayout->addWidget(label2);


  if (selectedmap!=NULL) delete selectedmap;
  if (strcmp(devman->midiMapFilename(),"")==0)
    selectedmap=NULL;
  else
  {
    selectedmap=new char[strlen(devman->midiMapFilename())+1];
    strcpy(selectedmap,devman->midiMapFilename());
  }

  if (selectedmap!=NULL) maplabel=new QLabel(selectedmap,page);
  else maplabel=new QLabel(i18n("None"),page);

  topLayout->addWidget(maplabel);

  QHBoxLayout *hbox=new QHBoxLayout();
  topLayout->addItem(hbox);
  hbox->addStretch(1);
  mapbrowse=new QPushButton(i18n("Browse..."),page);
  hbox->addWidget(mapbrowse);
  connect(mapbrowse,SIGNAL(clicked()),SLOT(browseMap()) );

  mapnone=new QPushButton(i18n("None"),page);
  hbox->addWidget(mapnone);
  connect(mapnone,SIGNAL(clicked()),SLOT(noMap()) );

  topLayout->addStretch(1);

}

void MidiConfigDialog::deviceselected(int idx)
{
  selecteddevice=idx;
}

void MidiConfigDialog::browseMap()
{
  QString path = KGlobal::dirs()->findAllResources("appdata", "maps/*.map").last();
  path.truncate(path.lastIndexOf('/'));

  KUrl url = KFileDialog::getOpenUrl(path,"*.map",this);

  if( url.isEmpty() )
    return;

  if( !url.isLocalFile() )
  {
    KMessageBox::sorry( 0L, i18n( "Only local files are currently supported." ) );
    return;
  }

  QString filename = url.path();

  delete selectedmap;
  selectedmap=new char[filename.length()+1];
  strcpy(selectedmap,QFile::encodeName(filename));
  maplabel->setText(selectedmap);
}

void MidiConfigDialog::noMap()
{
  if (selectedmap!=NULL) {delete selectedmap;selectedmap=NULL;};
  maplabel->setText(i18n("None"));
}

int MidiConfigDialog::selecteddevice=0;
char *MidiConfigDialog::selectedmap=NULL;

#include "midicfgdlg.moc"
