#include "MyPlugin.h"
#include <QStringList>
#include <QString>
#include <QDebug>

QStringList MyPlugin::operations() const
{
    return QStringList() << "oddUpper" << "lower";
}

QString MyPlugin::oddUpper(const QString &str)
{
    QString strTemp;

    for ( int i = 0; i < str.length(); ++i)
    {
        strTemp += (i % 2) ? str.at(i) : str.at(i).toUpper();
    }

    return strTemp;
}

QString MyPlugin::operation(const QString& strText, const QString& strOperation)
{
    QString strTemp;
    if (strOperation == "oddUpper")
    {
        strTemp = oddUpper(strText);
    }
    else if (strOperation == "lower")
    {
        strTemp = strText.toLower();
    }
    else
    {
        qDebug() << "Unsupported operation";
    }
    return strTemp;
}
