/**************************************************************************

    midicfgdlg.h  - The midi config dialog
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
#ifndef _midicfgdlg_h_
#define _midicfgdlg_h_

#include <kdialog.h>
//Added by qt3to4:
#include <QLabel>

class DeviceManager;

class QLabel;
class QPushButton;
class Q3ListBox;
class MidiConfigDialog : public KDialog
{
  Q_OBJECT

public:

  MidiConfigDialog(DeviceManager *dm,QWidget *parent,const char *name);

public slots:
  void deviceselected(int idx);
  void browseMap();
  void noMap();

private:
  QLabel *maplabel;
  Q3ListBox *mididevices;
  QPushButton *mapbrowse;
  QPushButton *mapnone;

  DeviceManager *devman;

public:
  static int selecteddevice;
  static char *selectedmap;
};

#endif
