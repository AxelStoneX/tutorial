#include <QtWidgets>
#include <QtScript>
#include <QScriptEngineDebugger>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QScriptEngine  scriptEngine;
    QScriptEngineDebugger scriptDebugger;

    scriptDebugger.attachTo(&scriptEngine);

    QAction* pact =
            scriptDebugger.action(QScriptEngineDebugger::InterruptAction);
    pact->trigger();

    QString strCode = "var sum = 0;\n"
                      "for (var i = 1; i < 5; i++) {\n"
                      "    sum += i;\n"
                      "}";

    QScriptValue result = scriptEngine.evaluate(strCode);

    return app.exec();
}
