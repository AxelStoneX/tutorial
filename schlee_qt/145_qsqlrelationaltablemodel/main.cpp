#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QtWidgets>

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

bool createTable()
{
    QSqlQuery query;

    QString str = "CREATE TABLE addressbook ( "
                     "number INTEGER PRIMARY KEY NOT NULL, "
                     "name   VARCHAR(15),"
                     "phone  VARCHAR(12),"
                     "email  VARCHAR(15)"
                  ");";

    if (!query.exec(str))
    {
        qDebug() << "Unable to create a table";
        return false;
    }
    return true;
}

bool insertValues()
{
    QSqlQuery query;
    QString str = "CREATE TABLE status ( "
                     "number   INTEGER PRIMARY KEY NOT NULL, "
                     "married  VARCHAR(5) );";

    if (!query.exec(str))
        qDebug() << "Unable to create a table";

    str = "INSERT INTO status (number, married) VALUES(1, 'YES')";

    if (!query.exec(str))
    {
        qDebug() << "Unable to insert";
        return false;
    }

    str = "INSERT INTO status (number, married) VALUES(2, 'NO')";

    if (!query.exec(str))
    {
        qDebug() << "Unable to insert";
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    if (!createConnection())
    {
        return -1;
    }

    insertValues();

    QTableView view;
    QSqlRelationalTableModel model;

    model.setTable("addressbook");
    model.setRelation(0, QSqlRelation("status", "number", "married"));

    view.setModel(&model);
    view.show();

    return app.exec();
}
