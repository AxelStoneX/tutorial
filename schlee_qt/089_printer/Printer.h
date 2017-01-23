#ifndef PRINTER_H
#define PRINTER_H

#include <QWidget>

class QPrinter;       // Class forward declaration
class QPaintDevice;   // Class forward declaration

// ===========================================================================
class Printer : public QWidget
{
    Q_OBJECT
private:
    QPrinter* m_pprinter;

protected:
    virtual void paintEvent(QPaintEvent* pe);
    void draw(QPaintDevice* ppd);

public:
    Printer(QWidget* pwgt = 0);
    virtual ~Printer();

public slots:
    void slotPrint();
};

#endif // PRINTER_H
