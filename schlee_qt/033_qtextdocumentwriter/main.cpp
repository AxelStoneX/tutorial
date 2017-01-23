#include "mainwindow.h"
#include <QApplication>
#include <QtPrintSupport/QPrinter>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextEdit* ptxt = new QTextEdit("This is a <b>TEST</b>");
    QTextDocumentWriter writer;
    writer.setFormat("odf");
    writer.setFileName("output.odf");
    writer.write(ptxt->document());

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("output.pdf");
    ptxt->document()->print(&printer);

    return app.exec();
}
