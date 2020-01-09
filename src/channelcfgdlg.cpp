#include <QPushButton>
#include <q3buttongroup.h>
#include <qradiobutton.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <kdialog.h>

#include <kapplication.h>
#include <klocale.h>

#include "channelview.h"
#include "channelcfgdlg.h"
#include "version.h"
#include <QLayout>

ChannelViewConfigDialog::ChannelViewConfigDialog(QWidget *parent,const char *name) : KDialog(parent)
{
    setObjectName(name);
    setCaption( i18n("Configure Channel View") );
    setButtons( KDialog::Ok|KDialog::Cancel );
    setDefaultButton( KDialog::Ok );
    setModal( true );
    QWidget *page = new QWidget( this );
    setMainWidget(page);
    QVBoxLayout *topLayout = new QVBoxLayout( page );
    topLayout->setSpacing( spacingHint() );
    topLayout->setMargin( 0 );
    qbg = new Q3VButtonGroup(i18n("Choose Look Mode"),page,"qbg");

    rb0 = new QRadioButton( i18n("3D look"),qbg );
    rb0->setObjectName( "3d" );
    rb1 = new QRadioButton( i18n("3D - filled"),qbg );
    rb1->setObjectName( "4d" );

    qbg->setExclusive(TRUE);

    topLayout->addWidget( qbg );
    ((ChannelView::lookMode()==0)?rb0:rb1)->setChecked(TRUE);

    connect (qbg, SIGNAL(pressed(int)),this,SLOT(modeselected(int)));
}

void ChannelViewConfigDialog::modeselected(int idx)
{
selectedmode=idx;
}

int ChannelViewConfigDialog::selectedmode;
#include "channelcfgdlg.moc"
