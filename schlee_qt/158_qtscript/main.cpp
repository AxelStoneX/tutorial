#include <QtCore>
#include <QtScript>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QScriptEngine scriptEngine;
    QScriptValue value = scriptEngine.evaluate("2 * 2");
    qDebug() << value.toInt32();

    return 0;
}
