﻿#include "mainui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainUi w;
    w.setWindowTitle("WebCatcher - 1.0");
    w.show();
    return a.exec();
}
