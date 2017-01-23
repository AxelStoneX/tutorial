#ifndef FILEFINDER_H
#define FILEFINDER_H

#include <QtWidgets>

class FileFinder : public QWidget {
    Q_OBJECT
public:
    QLineEdit* m_ptxtDir;
    QLineEdit* m_ptxtMask;
    QTextEdit* m_ptxtResult;
public:
    FileFinder(QWidget* pwgt);
    void start(const QDir& dir);
public slots:
    void slotBrowse();
    void slotFind();
};

#endif // FILEFINDER_H
