#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H

#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>
class HorizontalScrollBarStyle : public QProxyStyle
{
public:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;

};

class VerticalScrollBarStyle : public QProxyStyle
{
public:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;

};

#endif // PROXYSTYLE_H
