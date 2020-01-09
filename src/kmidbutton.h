#include <QPushButton>
#include <qpainter.h>
//Added by qt3to4:
#include <QPixmap>

class KMidButton : public QPushButton
{
protected:
    
    QPixmap pixmap1,pixmap2;

    virtual void drawButton(QPainter *paint)
    {
        if ((isChecked())&&(!pixmap1.isNull())) paint->drawPixmap(0,0,pixmap1);
        else if ((!isChecked())&&(!pixmap2.isNull())) paint->drawPixmap(0,0,pixmap2);
    }
    
public:
    
    KMidButton (QWidget *parent,const char *name) : QPushButton(parent)
    {
        setObjectName(name);
    }
    
    ~KMidButton()
    {
    }
    
    void setPixmaps(const QPixmap& p1, const QPixmap& p2)
    {
        pixmap1=p1;
        pixmap2=p2;
    }
    
};
