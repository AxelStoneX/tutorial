#include <QApplication>
#include <QtWebKitWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWebView     webView;
    webView.load(QUrl("http://www.bhv.ru"));
    webView.show();

    return app.exec();
}
