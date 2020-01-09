#ifndef __KMIDPART_H__
#define __KMIDPART_H__

#include <kparts/browserextension.h>
#include <kparts/factory.h>
#include <kdemacros.h>
class KAboutData;
class KComponentData;
class KMidBrowserExtension;
class kmidClient;

class KDE_EXPORT KMidFactory : public KParts::Factory
{
  Q_OBJECT
  public:
    KMidFactory();
    virtual ~KMidFactory();

    virtual KParts::Part* createPartObject(QWidget *parentWidget,
                QObject* parent = 0,
                const char* classname = "QObject",
                const QStringList &args = QStringList());

    static const KComponentData &componentData();

    static KAboutData *aboutData();
  private:
    static KComponentData *s_instance;
};

class KMidPart: public KParts::ReadOnlyPart
{
  Q_OBJECT
  public:
    KMidPart(QWidget *parentWidget, 
             QObject *parent);
    virtual ~KMidPart();


    virtual bool closeUrl();

  protected:
    virtual bool openFile();

  protected slots:
    void slotPlay();
    void slotStop();

  private:
    kmidClient *widget;
    KMidBrowserExtension *m_extension;
};

class KMidBrowserExtension : public KParts::BrowserExtension
{
  Q_OBJECT
    friend class KMidPart;
  public:
  KMidBrowserExtension(KMidPart *parent);
  virtual ~KMidBrowserExtension();
};

#endif
