#include "proxystyle.h"



const int padding_up = 4;
const int padding_left = 4;
const int padding_down = 4;

void HorizontalScrollBarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_ScrollBar){
        if(const QStyleOptionSlider *sli = qstyleoption_cast<const QStyleOptionSlider *>(option)){
            QRect sliRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
            QRect grooveRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget);
            QRect upRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSubLine, widget);
            QRect downRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarAddLine, widget);
            // 抗锯齿
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(QColor(201,204,204));
            painter->setBrush(QColor(201,204,204));
            QPoint addGround[4] = {grooveRect.topLeft(), grooveRect.topRight(), grooveRect.bottomRight(), grooveRect.bottomLeft()};
            painter->drawPolygon(addGround,4);
            painter->restore();
            // ScrollBar的slider(handle)
            if(sli->state & QStyle::State_Sunken){
                painter->save();
                painter->setPen(QColor(51,51,51));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(51,51,51));
                g.setColorAt(1,QColor(61,61,61));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else if(sli->state & QStyle::State_MouseOver){
                painter->save();
                painter->setPen(QColor(85,85,90));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(85,85,90));
                g.setColorAt(1,QColor(95,100,100));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else{
                painter->save();
                painter->setPen(QColor(120,125,130));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(120,125,130));
                g.setColorAt(1,QColor(132,135,140));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            // 两个arrow所在的方框
            painter->save();
            painter->setPen(QColor(51,51,51));
            painter->setBrush(QColor(240,255,255));
            QPoint up[4] = {upRect.topLeft(), upRect.topRight(), upRect.bottomRight(), upRect.bottomLeft()};
            QPoint down[4] = {downRect.topLeft(), downRect.topRight(), downRect.bottomRight(), downRect.bottomLeft()};
            painter->drawPolygon(up,4);
            painter->drawPolygon(down,4);
            painter->restore();

            // arrow的颜色和形状
            painter->save();
            painter->setPen(QColor(240,255,255));
            painter->setBrush(QColor(51,51,51));
            int topRightY = upRect.bottomLeft().y();
            int topwidth = upRect.width();
            int downRightX = downRect.x();
            QPoint leftArr[3] = {
                QPoint(padding_up, topRightY/2),
                QPoint(topwidth-padding_down, padding_left),
                QPoint(topwidth-padding_down, topRightY-padding_left),
                };
            QPoint rightArr[3] = {
                QPoint(padding_down+downRightX, padding_left),
                QPoint(padding_down+downRightX, topRightY-padding_left),
                QPoint(topwidth-padding_down+downRightX, topwidth/2),
                };
            painter->drawPolygon(leftArr,3);
            painter->drawPolygon(rightArr,3);
            painter->restore();
            return;
        }
    }
    else{
        drawComplexControl(control, option, painter, widget);
    }
}

void VerticalScrollBarStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == CC_ScrollBar){
        if(const QStyleOptionSlider *sli = qstyleoption_cast<const QStyleOptionSlider *>(option)){
            QRect sliRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
            QRect grooveRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget);
            QRect upRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSubLine, widget);
            QRect downRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarAddLine, widget);
            // 抗锯齿
            painter->setRenderHint(QPainter::Antialiasing);
            painter->save();
            painter->setPen(QColor(201,204,204));
            painter->setBrush(QColor(201,204,204));
            QPoint addGround[4] = {grooveRect.topLeft(), grooveRect.topRight(), grooveRect.bottomRight(), grooveRect.bottomLeft()};
            painter->drawPolygon(addGround,4);
            painter->restore();
            // ScrollBar的slider(handle)
            if(sli->state & QStyle::State_Sunken){
                painter->save();
                painter->setPen(QColor(51,51,51));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(51,51,51));
                g.setColorAt(1,QColor(61,61,61));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else if(sli->state & QStyle::State_MouseOver){
                painter->save();
                painter->setPen(QColor(85,85,90));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(85,85,90));
                g.setColorAt(1,QColor(95,100,100));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            else{
                painter->save();
                painter->setPen(QColor(120,125,130));
                QLinearGradient g(sliRect.topRight(), sliRect.bottomRight());
                g.setColorAt(0,QColor(120,125,130));
                g.setColorAt(1,QColor(132,135,140));
                painter->setBrush(g);
                QPoint backGround[4] = {sliRect.topLeft(), sliRect.topRight(), sliRect.bottomRight(), sliRect.bottomLeft()};
                painter->drawPolygon(backGround,4);
                painter->restore();
            }
            // 两个arrow所在的方框
            painter->save();
            painter->setPen(QColor(51,51,51));
            painter->setBrush(QColor(240,255,255));
            QPoint up[4] = {upRect.topLeft(), upRect.topRight(), upRect.bottomRight(), upRect.bottomLeft()};
            QPoint down[4] = {downRect.topLeft(), downRect.topRight(), downRect.bottomRight(), downRect.bottomLeft()};
            painter->drawPolygon(up,4);
            painter->drawPolygon(down,4);
            painter->restore();

            // arrow的颜色和形状
            painter->save();
            painter->setPen(QColor(240,255,255));
            painter->setBrush(QColor(51,51,51));
            int topRightX = upRect.topRight().x();
            int topHeight = upRect.height();
            int downRightY = downRect.y();
            QPoint upArr[3] = {
                QPoint(topRightX/2, padding_up),
                QPoint(padding_left, topHeight-padding_down),
                QPoint(topRightX-padding_left, topHeight-padding_down),
                };
            QPoint downArr[3] = {
                QPoint(padding_left, padding_down+downRightY),
                QPoint(topRightX-padding_left, padding_down+downRightY),
                QPoint(topHeight/2, topHeight-padding_down+downRightY),
                };
            painter->drawPolygon(upArr,3);
            painter->drawPolygon(downArr,3);
            painter->restore();
            return;
        }
    }
    else{
        drawComplexControl(control, option, painter, widget);
    }

}
