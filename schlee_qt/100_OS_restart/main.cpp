#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

// ===========================================================================
class MyApplication : public QApplication
{
public:
    MyApplication(int argc, char** argv)
        :QApplication(argc, argv)
    {
    }

    virtual ~MyApplication()
    {
    }

    virtual void commitData(QSessionManager&)
    {
        QMessageBox::information(0, "Dialog",
                                 "You are exiting operating system");
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel lbl("Exit the operating system to see the dialog box", 0);
    lbl.show();

    return app.exec();
}
