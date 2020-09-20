#include "screen.h"
#include <stdlib.h>

#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QPalette>

Screen::Screen(QWidget *parent)
    : QWidget(parent), bouncer(this)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Screen::update));
    timer->start(10);  // Update interval ms

    // TODO should be setTitle, and setTitle should call all what is
    // needed to change the title everywhere
    setWindowTitle(tr("ZX Screen"));

    resize(200, 200);
}

void Screen::paintEvent(QPaintEvent *e)
{
    bouncer.paintEvent(e);
    if (lines.size()<5)
    {
        lines.push_back(new LineBouncer(this));
    }
    for(auto line: lines)
    {
        line->paintEvent(e);
    }
}
