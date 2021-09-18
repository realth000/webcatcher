#include "mainui.h"
#include "ui_mainui.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QScrollBar>
#include "proxystyle.h"
#include <QUrl>
#include <QThread>
#include "webthread.h"
#include <QUrl>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMetaEnum>
#include <QDateTime>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MainUi::MainUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainUi)
{
    ui->setupUi(this);
    initUi();

}

MainUi::~MainUi()
{
    delete ui;
}

void MainUi::initUi()
{
    this->setFixedSize(this->width(), this->height());

    // 读取QSS
    QFile qss(":/stylesheet.qss");
    qss.open(QIODevice::ReadOnly);
    QTextStream qssStream;
    qssStream.setDevice(&qss);
    this->setStyleSheet(QString(qss.readAll()).arg(this->objectName()).arg("rgb(55,85,100)")
                            .arg("qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 rgb(45,45,45), stop: 1 rgb(51,51,51));"
                                 "alternate-background-color:rgb(55,55,55)"));
    qss.close();

    ui->resultTE->verticalScrollBar()->setStyle(new VerticalScrollBarStyle);
    ui->resultTE->horizontalScrollBar()->setStyle(new HorizontalScrollBarStyle);
    ui->filterResultTE->verticalScrollBar()->setStyle(new VerticalScrollBarStyle);
    ui->filterResultTE->horizontalScrollBar()->setStyle(new HorizontalScrollBarStyle);

    ui->logLE->setEnabled(false);
    ui->logLE->setContextMenuPolicy(Qt::NoContextMenu);
    ui->useRegExpFilterRB->setChecked(true);
    ui->resultTE->setReadOnly(true);
    ui->filterResultTE->setReadOnly(true);
    ui->importFilterLE->setReadOnly(true);

    addFreeze(ui->useRegExpFilterRB);
    addFreeze(ui->usedFindFilterRB);
    addFreeze(ui->useImportFilterRB);
    addFreeze(ui->startWebCatchPB);
    addFreeze(ui->refiltPB);
    addFreeze(ui->saveWebPB);
    addFreeze(ui->saveFiltResultPB);
    addFreeze(ui->importWebPB);

    log("就绪");
}

void MainUi::log(QString l)
{
    ui->logLE->setText(l.prepend(" "));
}

void MainUi::filt_regexp(QString content)
{
    if(ui->filterLE->text().isEmpty()){
        log("正则为空");
        return;
    }
    QStringList filterResult;
    int endLength = content.length();
    int index = 0;
    QRegularExpression re_pre(ui->prefixFilterLE->text());
    QRegularExpression re(ui->prefixFilterLE->text() + ui->filterLE->text() + ui->suffixFilterLE->text());
    qDebug() << re;
    QRegularExpression re_filter(ui->filterLE->text());
    qDebug() << re_filter;
    QRegularExpressionMatch match;
    QRegularExpressionMatch match_filter;

    // 筛抓到的文本，先筛带前后缀的，一个一个筛，从文本头到文本尾
    while(index < endLength){
        match = re.match(content, index);
        if(match.hasMatch()){
            index = match.capturedEnd();
            // 再筛去前后缀，只留内容
            match_filter = re_filter.match(match.captured().replace(re_pre, ""));
            if(match.hasMatch()){
                filterResult.append(match_filter.captured());
                ui->filterResultTE->insertPlainText("《" + match_filter.captured() + "》");
                ui->filterResultTE->insertPlainText("\n");
            }
            else{
                // 这里是筛到了前缀 + 内容 + 后缀，但从中筛不出内容的情况，不应该
            }
        }
        else{
            // 已经筛到最后一个结果，直接结束
            break;
        }
    }
}

void MainUi::filt_find(QString content)
{
    if(ui->filterLE->text().isEmpty()){
        log("规则为空");
        return;
    }

    QString re(ui->prefixFilterLE->text() + ui->filterLE->text() + ui->suffixFilterLE->text());

    // 筛抓到的文本，先筛带前后缀的，需要筛的内容只有一条，只
    int match;
    match = content.indexOf(re);
    if(match != -1){
        // 再筛去前后缀，只留内容
        ui->filterResultTE->setText("含有" + re);
    }
    else{
        ui->filterResultTE->setText("不含" + re);
    }
}

void MainUi::filt_file(QString content)
{
    ui->filterResultTE->clear();

    QStringList filterResult;

    if(inFilter.isEmpty()){
        log("规则为空");
        return;
    }
    if(global_result.isEmpty()){
        log("尚未抓取");
        return;
    }

    int match = -1;
    QStringList filters = inFilter.replace("\r", "").split("\n");
    foreach(QString filter, filters){
        if(filter.isEmpty()){
            // 跳过空文本
            continue;
        }
        match = content.indexOf(filter);
        if(match != -1){
            filterResult.append(filter);
        }
    }
//    ui->filterResultTE->append(filterResult.join("\n"));
    ui->filterResultTE->insertPlainText(filterResult.join("\n"));
    ui->filterResultTE->insertPlainText("\n");
    qDebug() << filterResult.join("\n");
}


void MainUi::on_startWebCatchPB_clicked()
{
    freeze();
    QString url = ui->webUrlLE->text();
    if(url.isEmpty()){
        log("empty");
        unfreeze();
        return;
    }
    QUrl r(url);
    if(!r.isValid()){
        log("不是有效的Url");
        unfreeze();
        return;
    }
    global_url = url;

    WebThread *wth = new WebThread(url);
    connect(wth, &WebThread::log, this, &MainUi::log);
    connect(wth, &WebThread::webThreadFinish, wth, &WebThread::deleteLater);
    connect(wth, &WebThread::result, this, [=](QString result, QString error){checkResult(result, error);unfreeze();});
    QThread *workThread = new QThread;
    connect(workThread, &QThread::started, wth, &WebThread::getWeb);
    connect(wth, &WebThread::destroyed, workThread, &QThread::quit);
    connect(workThread, &QThread::finished, workThread, &QThread::deleteLater);

    wth->moveToThread(workThread);
    workThread->start();
    ui->currentUrlL->setText("原网页: " + url);
    log("爬取中");
}

void MainUi::checkResult(QString result, QString error)
{
    // filterResult存放最终的内容
    global_result = result;
    if(global_result.isEmpty()){
        log(error);
        return;
    }
    QStringList filterResult;

    QRegularExpression re_title("<title>.*?</title>");
    QRegularExpressionMatch match_title;
    match_title = re_title.match(result);
    if(match_title.hasMatch()){
        global_urlName = match_title.captured().replace("<title>","").replace("</title>","");
    }
    else{
        QRegularExpression re_title_main("//.*?/");
        match_title = re_title_main.match(result);
        if(match_title.hasMatch()){
            global_urlName = match_title.captured();
        }
        else{
            global_urlName = "Web";
        }
    }
    ui->resultTE->setPlainText(result);

    checkWork(result);
    log("完成");
}

void MainUi::checkWork(QString content)
{
    if(ui->useRegExpFilterRB->isChecked()){
        // 用正则筛
        filt_regexp(content);
    }
    else if(ui->usedFindFilterRB->isChecked()){
        // 直接筛内容
        filt_find(content);
    }
    else{
        // 筛文件内容，一行一条。
        filt_file(content);
    }
}

void MainUi::on_refiltPB_clicked()
{
    ui->filterResultTE->clear();
    freeze();
    if(global_result.isEmpty()){
        log("未抓取");
        unfreeze();
        return;
    }

    checkWork(global_result);
    unfreeze();
}

void MainUi::addFreeze(QWidget *w)
{
    freezeWidgets.append(w);
}

void MainUi::freeze()
{
    foreach(QWidget *p, freezeWidgets){
        p->setEnabled(false);
    }
}

void MainUi::unfreeze()
{
    foreach(QWidget *p, freezeWidgets){
        p->setEnabled(true);
    }
}

void MainUi::on_saveWebPB_clicked()
{
    freeze();
    if(global_result.isEmpty()){
        log("尚未抓取");
        unfreeze();
        return;
    }

    QString st("txt文档(*.txt)");
    QString savePath = QFileDialog::getSaveFileName(this, "保存原网页", "./saves/"+global_urlName+"_"+
                                                                            QDateTime::currentDateTime().toString("yyyyMMddHHmmss")+".txt", st, &st);
    if(savePath.isEmpty()){
        unfreeze();
        return;
    }
    QFile saveFile(savePath);
    if(!saveFile.open(QIODevice::WriteOnly)){
        log("无法打开保存的文件，保存失败");
        unfreeze();
        return;
    }
    QTextStream saveStream;
    saveStream.setDevice(&saveFile);
    saveStream.setCodec("UTF-8");
    saveStream << global_result;
    saveFile.close();
    log("保存"+QString::number(global_result.length())+"字节到:"+savePath);
    unfreeze();
}

void MainUi::on_saveFiltResultPB_clicked()
{
    freeze();
    if(global_result.isEmpty()){
        log("尚未抓取");
        unfreeze();
        return;
    }

    QString st("txt文档(*.txt)");
    QString savePath = QFileDialog::getSaveFileName(this, "保存筛选内容", "./saves/"+global_urlName+"_filted_"+
                                                                            QDateTime::currentDateTime().toString("yyyyMMddHHmmss")+".txt", st, &st);
    if(savePath.isEmpty()){
        unfreeze();
        return;
    }
    QFile saveFile(savePath);
    if(!saveFile.open(QIODevice::WriteOnly)){
        log("无法打开保存的文件，保存失败");
        unfreeze();
        return;
    }
    QString saveData = ui->filterResultTE->toPlainText();
    QTextStream saveStream;
    saveStream.setDevice(&saveFile);
    saveStream.setCodec("UTF-8");
    saveStream << saveData;
    saveFile.close();
    log("保存"+QString::number(saveData.length())+"字节到:"+savePath);
    unfreeze();
}

void MainUi::on_importFilterPB_clicked()
{
    freeze();
    QString fileFilter = QFileDialog::getOpenFileName(this, "导入文件内容", "", "所有文件(*.*);;网页文件(*.html);;文本文件(*.txt)");
    if(fileFilter.isEmpty()){
        unfreeze();
        return;
    }

    QFile fin;
    QTextStream inStream;
    fin.setFileName(fileFilter);
    if(!fin.open(QIODevice::ReadOnly)){
        log("文件无法打开");
        unfreeze();
        return;
    }
    inStream.setDevice(&fin);
    inFilter = inStream.readAll();
    fin.close();
    if(inFilter.isEmpty()){
        log("规则为空");
        unfreeze();
        return;
    }
    ui->importFilterLE->setText(fileFilter);
//    global_url = fileFilter;
//    global_urlName = QFileInfo(fileFilter).baseName();
    unfreeze();
}

void MainUi::on_importWebPB_clicked()
{
    freeze();
    QString importFile = QFileDialog::getOpenFileName(this, "导入文件内容", "", "所有文件(*.*);;网页文件(*.html);;文本文件(*.txt)");
    if(importFile.isEmpty()){
        unfreeze();
        return;
    }

    QFile fin;
    QTextStream inStream;
    fin.setFileName(importFile);
    if(!fin.open(QIODevice::ReadOnly)){
        log("文件无法打开");
        unfreeze();
        return;
    }
    inStream.setDevice(&fin);
    inStream.setCodec("UTF-8");
    QString inString = inStream.readAll();
    fin.close();
    if(inString.isEmpty()){
        log("导入文件为空");
        unfreeze();
        return;
    }
    global_result = inString;
    global_url = importFile;
    global_urlName = QFileInfo(importFile).baseName();
    ui->resultTE->setPlainText(inString);
    ui->webUrlLE->setText(importFile);
    unfreeze();

}
