#ifndef WEBTHREAD_H
#define WEBTHREAD_H
#include <QThread>


class WebThread : public QThread
{
    Q_OBJECT
public:
    WebThread(QString url);

signals:
    void log(QString);
    void webThreadFinish();
    void result(QString, QString);

public slots:
    void getWeb();

private:
    QString _url;
};

#endif // WEBTHREAD_H
