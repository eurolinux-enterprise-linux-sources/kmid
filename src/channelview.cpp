/**************************************************************************

    channelview.cpp  - The ChannelView dialog
    Copyright (C) 1998  Antonio Larrosa Jimenez

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

#include <kapplication.h>
#include <klocale.h>
#include <kconfig.h>
#include <kconfiggroup.h>

#include "channelview.h"
#include "channel3d.h"
#include "channel4d.h"
//Added by qt3to4:
#include <QResizeEvent>
#include <QCloseEvent>


ChannelView::ChannelView(void) : KXmlGuiWindow(0)
{
    setObjectName("ChannelView");
    setCaption(i18n("Channel View"));
    for (int i=0;i<16;i++)
    {
        if (lookMode()==0)
            Channel[i]=new KMidChannel3D(i+1,this);
        else
            Channel[i]=new KMidChannel4D(i+1,this);
        connect(Channel[i],SIGNAL(signalToKMidClient(int *)),this,SLOT(slottokmidclient(int *)));
        Channel[i]->setGeometry(5,5+i*CHANNELHEIGHT,width()-20,CHANNELHEIGHT);
        Channel[i]->show();
    }
    scrollbar = new QScrollBar(Qt::Vertical, this);
    scrollbar->setMinimum(1);
    scrollbar->setMaximum(16);
    scrollbar->setSingleStep(1);
    scrollbar->setPageStep(1);
    scrollbar->setValue(1);
    scrollbar->setObjectName("Channelscrollbar");
    connect(scrollbar,SIGNAL(valueChanged(int)),this,SLOT(ScrollChn(int)));
    setScrollBarRange();
}

ChannelView::~ChannelView()
{

}

void ChannelView::closeEvent(QCloseEvent *e)
{
    emit destroyMe();
    e->accept();
}

void ChannelView::resizeEvent(QResizeEvent *)
{
    scrollbar->setGeometry(width()-16,0,16,height());
    for (int i=0;i<16;i++)
    {
        Channel[i]->setGeometry(5,5+(i-(scrollbar->value()-1))*CHANNELHEIGHT,width()-20,CHANNELHEIGHT);
    }
    setScrollBarRange();

}

void ChannelView::setScrollBarRange(void)
{
    nvisiblechannels=height()/CHANNELHEIGHT;
    if (nvisiblechannels<16)
    scrollbar->setRange(1,16-nvisiblechannels+1);
   else
    scrollbar->setRange(1,1);
}

void ChannelView::ScrollChn(int v)
{
    for (int i=0;i<16;i++)
    {
        Channel[i]->move(5,5+(i-(v-1))*CHANNELHEIGHT);
    }
}

void ChannelView::noteOn(int chn,int note)
{
    Channel[chn]->noteOn(note);
}

void ChannelView::noteOff(int chn,int note)
{
    Channel[chn]->noteOff(note);
}

void ChannelView::changeInstrument(int chn,int pgm)
{
    Channel[chn]->changeInstrument(pgm);
}

void ChannelView::changeForceState(int chn,bool i)
{
    Channel[chn]->changeForceState(i);
}


void ChannelView::reset(int level)
{
    for (int i=0;i<16;i++)
    {
        Channel[i]->reset(level);
    }
}

int ChannelView::lookmode=0;

int ChannelView::lookMode(void)
{
    KConfig *kcfg=(KApplication::kApplication())->sessionConfig();

    KConfigGroup kmidGroup = kcfg->group("KMid");
    lookmode = kmidGroup.readEntry("ChannelViewLookMode", 0);

    return lookmode;
}

void ChannelView::lookMode(int i)
{
    KConfig *kcfg=(KApplication::kApplication())->sessionConfig();

    lookmode=i;

    KConfigGroup kmidGroup = kcfg->group("KMid");
    kmidGroup.writeEntry("ChannelViewLookMode", lookmode);

    bool tmp[128];
    int pgm;
    for (int i=0;i<16;i++)
    {
        Channel[i]->saveState(tmp,&pgm);
        delete Channel[i];

        if (lookmode==0)
            Channel[i]=new KMidChannel3D(i+1,this);
        else
            Channel[i]=new KMidChannel4D(i+1,this);

        connect(Channel[i],SIGNAL(signalToKMidClient(int *)),this,SLOT(slottokmidclient(int *)));
        Channel[i]->setGeometry(5,5+(i-(scrollbar->value()-1))*CHANNELHEIGHT,width()-20,CHANNELHEIGHT);
        Channel[i]->loadState(tmp,&pgm);
        Channel[i]->show();
    }

}

void ChannelView::slottokmidclient(int *data)
{
    emit signalToKMidClient(data);
}
#include "channelview.moc"
