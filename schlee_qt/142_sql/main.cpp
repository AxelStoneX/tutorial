#include "mainwindow.h"
#include <QApplication>
#include <QtSql>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("addressbook");
    db.setUserName("elton");
    db.setHostName("alex-pc");
    db.setPassword("password");
    if (!db.open())
    {
        qDebug() << "Cannot open database:" << db.lastError();
        return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if(!createConnection())
    {
        return -1;
    }

    QSqlQuery query;

    QString str = "CREATE TABLE addressbook ( "
                     "number INTEGER PRIMARY KEY NOT NULL, "
                     "name   VARCHAR(15),"
                     "phone  VARCHAR(12),"
                     "email  VARCHAR(15)"
                  ");";

    if (!query.exec(str))
        qDebug() << "Unable to create a table";

    QString strF =
            "INSERT INTO addressbook (number, name, phone, email) "
            "VALUES(%1, '%2', '%3', '%4');";

    str = strF.arg("1")
              .arg("Piggy")
              .arg("+49631322187")
              .arg("piggy@mega.de");
    if (!query.exec(str))
        qDebug() << "Unable to make insert operation";

    str = strF.arg("2")
              .arg("Kermit")
              .arg("+49631322181")
              .arg("kermit@mega.de");
    if(!query.exec(str))
        qDebug() << "Unable to make insert operation";

    if (!query.exec("SELECT * FROM addressbook;"))
    {
        qDebug() << "Unable to execute query - exiting";
        return 1;
    }

    QSqlRecord rec = query.record();
    int nNumber = 0;
    QString strName;
    QString strPhone;
    QString strEmail;

    while(query.next())
    {
        nNumber = query.value(rec.indexOf("number")).toInt();
        strName = query.value(rec.indexOf("name")).toString();
        strPhone = query.value(rec.indexOf("phone")).toString();
        strEmail = query.value(rec.indexOf("email")).toString();
        qDebug() << nNumber << " " << strName << ";\t"
                 << strPhone << ";\t" << strEmail;
    }


    return 0;
}
