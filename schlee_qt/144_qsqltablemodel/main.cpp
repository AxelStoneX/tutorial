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



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!createConnection())
    {
        return -1;
    }

    QTableView     view;
    QSqlTableModel model;

    model.setTable("addressbook");
    model.select();
    model.setEditStrategy(QSqlTableModel::OnFieldChange);

    view.setModel(&model);
    view.show();

    return app.exec();
}
