#include "kmid_part.h"

#include <kcomponentdata.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kaboutdata.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kicon.h>

#include <kparts/partmanager.h>

#include "kmidclient.h"
#include <QTimer>
#include <kdemacros.h>

extern "C"
{
  /**
   * This function is the 'main' function of this part.  It takes
   * the form 'void *init_lib<library name>()'.  It always returns a
   * new factory object
   */
  KDE_EXPORT void *init_libkmidpart()
  {
    return new KMidFactory;
  }
}

/**
 * We need one static instance of the factory for our C 'main'
 * function
 */
KComponentData *KMidFactory::s_instance = 0L;

KMidFactory::KMidFactory()
{
  s_instance=0L;
}

KMidFactory::~KMidFactory()
{
  if (s_instance)
  {
    delete s_instance->aboutData();
    delete s_instance;
  }

  s_instance = 0;
}


KParts::Part *KMidFactory::createPartObject(QWidget *parentWidget,
                                            QObject *parent, const char*,
    const QStringList& )
{
  KParts::Part *obj = new KMidPart(parentWidget, parent);
  return obj;
}

KAboutData *KMidFactory::aboutData()
{
    KAboutData *aboutdata = new KAboutData("kmid", 0, ki18n("KMid"), "2.0",
        ki18n("MIDI/Karaoke file player"), KAboutData::License_GPL,
        ki18n("(c) 1997,98,99,2000, Antonio Larrosa Jimenez"),KLocalizedString(),
        "http://perso.wanadoo.es/antlarr/kmid.html");
    aboutdata->addAuthor(ki18n("Antonio Larrosa Jimenez"),
        ki18n("Original Developer/Maintainer"),"larrosa@kde.org",
        "http://perso.wanadoo.es/antlarr/index.html");
    return aboutdata;
}

const KComponentData &KMidFactory::componentData()
{
  if ( !s_instance )
    s_instance = new KComponentData( aboutData() );

  return *s_instance;
}

  KMidPart::KMidPart(QWidget *parentWidget,
                     QObject *parent)
: KParts::ReadOnlyPart(parent)
{
  setComponentData(KMidFactory::componentData());

  widget = new kmidClient(parentWidget, actionCollection());
  widget->show();
  widget->setFocusPolicy(Qt::ClickFocus);
  setWidget(widget);

  // create and connect our actions
   QAction *action = actionCollection()->addAction("play");
   action->setIcon(KIcon("media-playback-start"));
   action->setText(i18n("Play"));
   connect(action, SIGNAL(triggered(bool) ), SLOT(slotPlay()));

   action = actionCollection()->addAction("stop");
   action->setIcon(KIcon("media-playback-stop"));
   action->setText(i18n("Stop"));
   connect(action, SIGNAL(triggered(bool) ), SLOT(slotStop()));

   action = actionCollection()->addAction("backward");
   action->setIcon(KIcon("media-seek-backward"));
   action->setText(i18n("Backward"));
   connect(action, SIGNAL(triggered(bool) ), SLOT(slotBackward()));

   action = actionCollection()->addAction("forward");
   action->setIcon(KIcon("media-seek-forward"));
   action->setText(i18n("Forward"));
   connect(action, SIGNAL(triggered(bool) ), SLOT(slotForward()));

  m_extension = new KMidBrowserExtension(this);

  setXMLFile("kmid_partui.rc");


}

KMidPart::~KMidPart()
{
}

bool KMidPart::openFile()
{
  widget->openUrl(localFilePath());
  widget->stop();
  widget->show();
  QTimer::singleShot(2000, this, SLOT(slotPlay()));

  return true;
}

bool KMidPart::closeUrl()
{
  slotStop();
  return true;
}

void KMidPart::slotPlay()
{
  widget->stop();
  widget->play();
}


void KMidPart::slotStop()
{
  widget->stop();
}
KMidBrowserExtension::KMidBrowserExtension(KMidPart *parent)
    : KParts::BrowserExtension(parent)
{
	setObjectName("KMidBrowserExtension");
}

KMidBrowserExtension::~KMidBrowserExtension()
{
}
#include "kmid_part.moc"
