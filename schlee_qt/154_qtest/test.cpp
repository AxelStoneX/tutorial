#include <QtTest>
#include <QtWidgets>

// ===========================================================================
class test_QLineEdit : public QObject
{
    Q_OBJECT
private slots:
    void edit();
};

// ---------------------------------------------------------------------------
void test_QLineEdit::edit()
{
    QLineEdit txt;
    QTest::keyClicks(&txt, "ABCDEFGH");

    QCOMPARE(txt.text(), QString("ABCDEFGH"));
    QVERIFY(txt.isModified());
}

QTEST_MAIN(test_QLineEdit)
#include "test.moc"
