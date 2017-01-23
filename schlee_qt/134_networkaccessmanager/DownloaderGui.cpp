#include <DownloaderGui.h>
#include "Downloader.h"
#include <QtWidgets>

DownloaderGui::DownloaderGui(QWidget* pwgt) : QWidget(pwgt)
{
    m_pdl = new Downloader(this);
    m_ppb = new QProgressBar;
    m_ptxt = new QLineEdit;
    m_pcmd = new QPushButton(tr("&Go"));

    QString strDownloadLink =
            "http://www.neonway.com/wallpaper/images/traderstar.jpg";
    m_ptxt->setText(strDownloadLink);

    connect(m_pcmd, SIGNAL(clicked()), SLOT(slotGo()));
    connect(m_pdl, SIGNAL(downloadProgress(qint64,qint64)),
            this,  SLOT(slotDownloadProgress(qint64,qint64)));

    connect(m_pdl, SIGNAL(done(const QUrl&,const QByteArray&)),
            this, SLOT(slotDone(const QUrl&,const QByteArray&)));

    // Layout setup
    QGridLayout* pLayout = new QGridLayout;
    pLayout->addWidget(m_ptxt, 0, 0);
    pLayout->addWidget(m_pcmd, 0, 1);
    pLayout->addWidget(m_ppb, 1, 0, 1, 1);
    setLayout(pLayout);
}

void DownloaderGui::slotGo()
{
    m_pdl->download(QUrl(m_ptxt->text()));
}

void DownloaderGui::slotDownloadProgress(qint64 nReceived, qint64 nTotal)
{
    if (nTotal <= 0)
    {
        slotError();
        return;
    }
    m_ppb->setValue(100 * nReceived/nTotal);
}

void DownloaderGui::slotDone(const QUrl& url, const QByteArray& ba)
{
    QFile file(url.path().section('/', -1));
    QString strFileName = file.fileName();
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(ba);
        file.close();

        if(strFileName.endsWith(".jpg")
                || strFileName.endsWith(".png"))
        {
            showPic(strFileName);
        }
    }
}

void DownloaderGui::showPic(const QString& strFilename)
{
    QPixmap pix(strFilename);
    pix = pix.scaled(pix.size() / 3,
                     Qt::IgnoreAspectRatio,
                     Qt::SmoothTransformation);
    QLabel* plbl = new QLabel;
    plbl->setPixmap(pix);
    plbl->setFixedSize(pix.size());
    plbl->show();
}

void DownloaderGui::slotError()
{
    QMessageBox::critical(0,
                          tr("Error"),
                          tr("An error while download is occured"));
}
