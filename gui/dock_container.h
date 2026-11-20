#ifndef DOCKCONTAINER_H
#define DOCKCONTAINER_H

#include <QWidget>
#include "dock.h"

namespace Ui {
class DockContainer;
}

class DockContainer : public Dock
{
    Q_OBJECT

public:
    explicit DockContainer(QDockWidget *parent);
    explicit DockContainer(QWidget *parent = nullptr);
    ~DockContainer();

    QString getIdentifier() {
        return "Dock Container";
    }
    Dock* getContainedDock() {return mContainedDock;}

public slots:
    void changeDock(int comboIdx);
    void setDock(Dock *d);

private:
    Ui::DockContainer *ui;
    QDockWidget *mDockParent = nullptr;
    Dock *mContainedDock = nullptr;
};

#endif // DOCKCONTAINER_H
