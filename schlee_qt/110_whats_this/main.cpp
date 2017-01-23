#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel lbl("Please press the ? Button",
               0,
               Qt::WindowTitleHint | Qt::WindowSystemMenuHint
               | Qt::WindowContextHelpButtonHint
               );

    lbl.setWhatsThis("<I>This is the Label</I><BR><B>Class QLabel</B>");

    lbl.show();

    return app.exec();
}
