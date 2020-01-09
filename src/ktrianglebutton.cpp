/**************************************************************************

    ktrianglebutton.cpp  - The KTriangleButton widget (button with an arrow)
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

    Note: This widget was based on KButton as found in the kdelibs/kdeui
    KButton was originally copyrighted by Torben Weis (weis@kde.org)
    and Matthias Ettrich (ettrich@kde.org) on 1997

***************************************************************************/
#include "ktrianglebutton.h"
#include <qpainter.h>
#include <qdrawutil.h>
#include <qstyle.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QTimerEvent>
#include <QEvent>
#include <QAbstractEventDispatcher>

KTriangleButton::KTriangleButton( Direction d,QWidget *_parent, const char *name )
    : Q3Button( _parent , name)
{
    dir=d;
    raised = FALSE;
    setFocusPolicy( Qt::NoFocus );
}

KTriangleButton::~KTriangleButton()
{
}

void KTriangleButton::enterEvent( QEvent* )
{
  if ( isEnabled() )
    {
      raised = TRUE;
      repaint();
    }
}

void KTriangleButton::leaveEvent( QEvent * )
{
  if( raised != FALSE )
    {
      raised = FALSE;
      repaint();
    }
}


void KTriangleButton::drawButton( QPainter *_painter )
{
  paint( _painter );
}

void KTriangleButton::drawButtonLabel( QPainter *_painter )
{
  paint( _painter );
}

void KTriangleButton::paint( QPainter *painter )
{
  if ( isDown() || isChecked() )
    {
#ifdef __GNUC__
#warning "kde4: FIXME use for the moment qDrawWinButton";
#endif
			qDrawWinButton( painter, 0, 0, width(),
				            height(), palette(), TRUE );
#if 0
			if ( style().styleHint(QStyle::SH_GUIStyle) == WindowsStyle )
	qDrawWinButton( painter, 0, 0, width(),
			height(), colorGroup(), TRUE );
      else
	qDrawShadePanel( painter, 0, 0, width(),
			 height(), colorGroup(), TRUE, 2, 0L );
#endif
    }
  else if ( raised )
    {
		qDrawWinButton( painter, 0, 0, width(), height(),
                                palette(), FALSE );
#if 0
      if ( style().styleHint(QStyle::SH_GUIStyle) == WindowsStyle )
	qDrawWinButton( painter, 0, 0, width(), height(),
			colorGroup(), FALSE );
      else
	qDrawShadePanel( painter, 0, 0, width(), height(),
			 colorGroup(), FALSE, 2, 0L );
#endif
    }

  if (dir==Right)
  {
      int x=width()/4;
      int y=height()/6;
      int l=height()-y*2;
      int i=0;
      int maxi=width()-2*x;
      double m=(double)(l/2)/maxi;
      while (i<=maxi)
      {
          painter->drawLine(x,y+(int)(i*m),x,y+l-(int)(i*m));
          x++;
          i++;
      };
  }
  else if (dir==Left)
  {
      int x=width()/4;
      int y=height()/6;
      int l=height()-y*2;
      int i=0;
      int maxi=width()-2*x;
      x=width()-x;
      double m=(double)(l/2)/maxi;
      while (i<=maxi)
      {
          painter->drawLine(x,y+(int)(i*m),x,y+l-(int)(i*m));
          x--;
          i++;
      };

  };

}

void KTriangleButton::mousePressEvent(QMouseEvent *e)
{
    Q3Button::mousePressEvent(e);
    usingTimer=true;
    startTimer(500);
    timeCount=0;

}

void KTriangleButton::mouseReleaseEvent(QMouseEvent *e)
{
    usingTimer=false;
    Q3Button::mouseReleaseEvent(e);
}

void KTriangleButton::timerEvent(QTimerEvent *)
{
    if (!usingTimer) {QAbstractEventDispatcher::instance()->unregisterTimers(this);return;};
    if (timeCount==0)
    {
        timeCount++;
        QAbstractEventDispatcher::instance()->unregisterTimers(this);
        startTimer(120);
    } else
        if (timeCount==30)
        {
            timeCount=-1;
            QAbstractEventDispatcher::instance()->unregisterTimers(this);
            startTimer(80);
        }
    else if (timeCount>0) timeCount++;
    emit clickedQuickly();

}
#include "ktrianglebutton.moc"
