#ifndef DROP_H
#define DROP_H

#include <QtWidgets>

// ===========================================================================
class Drop : public QLabel
{
    Q_OBJECT

protected:
    virtual void dragEnterEvent(QDragEnterEvent* pe)
    {
        qDebug() << "Catched";
        if (pe->mimeData()->hasFormat("text/uri-list"))
        {

            pe->acceptProposedAction();
        }
    }

    virtual void dropEvent(QDropEvent* pe)
    {
        QList<QUrl> urlList = pe->mimeData()->urls();
        QString str;
        foreach(QUrl url, urlList)
            str += url.toString() + "\n";
        setText("Dropped:\n" + str);
    }
public:
    Drop(QWidget* pwgt = 0) : QLabel("Drop Area", pwgt)
    {
    }
};

#endif // DROP_H
