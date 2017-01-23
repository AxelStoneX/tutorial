#ifndef INTERFACES_H
#define INTERFACES_H

class QString;
class QStringList;

// ===========================================================================
class StringInterface
{
public:
    virtual ~StringInterface() {}
    virtual QStringList operations() const = 0;
    virtual QString operation(const QString& strText,
                              const QString& strOperation) = 0;
};

Q_DECLARE_INTERFACE(StringInterface,
                    "com.mysoft.Application.StringInterface")

#endif // INTERFACES_H
