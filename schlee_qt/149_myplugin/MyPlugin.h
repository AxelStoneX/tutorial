#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <QObject>
#include "../148_plugins/Interfaces.h"

// ===========================================================================
class MyPlugin : public QObject, public StringInterface
{
    Q_OBJECT
    Q_INTERFACES(StringInterface)
    Q_PLUGIN_METADATA(IID "com.mysoft.Application.StringInterface" FILE
                      "stringinterface.json")

private:
    QString oddUpper(const QString& str);

public:
    virtual ~MyPlugin();
    virtual QStringList operations() const;
    virtual QStrin<g operation(const QString& strText,
                              const QString& strOperation);
};

#endif // MYPLUGIN_H
