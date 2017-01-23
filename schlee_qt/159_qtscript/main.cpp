#include <QtWidgets>
#include <QtScript>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel lbl;

    QScriptEngine scriptEngine;
    QScriptValue scriptLbl = scriptEngine.newQObject(&lbl);
    scriptEngine.globalObject().setProperty("lbl", scriptLbl);
    scriptEngine.evaluate("lbl.text = 'Hello, World!'");
    scriptEngine.evaluate("lbl.show()");

    return app.exec();
}
