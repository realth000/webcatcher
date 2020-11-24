#include "webthread.h"
#include <QEventLoop>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

WebThread::WebThread(QString url) : _url(url)
{

}

void WebThread::getWeb()
{
    if(_url.isEmpty()){
        emit log("Url为空");
        return;
    }
    QEventLoop loop;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QNetworkRequest request;
    request.setUrl(QUrl(_url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = manager->get(request);
    loop.exec();

    emit result(reply->readAll());
    emit webThreadFinish();
}
