#ifndef MAINUI_H
#define MAINUI_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainUi; }
QT_END_NAMESPACE

class MainUi : public QWidget
{
    Q_OBJECT

public:
    MainUi(QWidget *parent = nullptr);
    ~MainUi();
    void initUi();
    void log(QString l);
    void filt_regexp(QString content);

private slots:
    void on_startWebCatchPB_clicked();
    void on_refiltPB_clicked();

    void on_saveWebPB_clicked();

    void on_saveFiltResultPB_clicked();

private:
    Ui::MainUi *ui;
    QString global_urlName;
    QString global_result;
    QString global_url;
    QVector<QWidget *> freezeWidgets;

    void addFreeze(QWidget *w);
    void freeze();
    void unfreeze();
    void checkResult(QString result, QString error);

};
#endif // MAINUI_H
