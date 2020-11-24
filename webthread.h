#ifndef WEBTHREAD_H
#define WEBTHREAD_H
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WebThread : public QThread
{
    Q_OBJECT
public:
    WebThread(QString url);

signals:
    void log(QString);
    void webThreadFinish();
    void result(QString);

public slots:
    void getWeb();

private:
    QString _url;
};

#endif // WEBTHREAD_H
