#include "dock.h"
#include <QDebug>

Dock::Dock(QWidget *parent) :
    QWidget(parent)
{
}

void Dock::onVisibiltyChanged(bool v)
{
    if (!v)
        this->deleteLater();
}

void Dock::resizeEvent(QResizeEvent *)
{

}
