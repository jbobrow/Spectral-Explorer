#ifndef EMPTYDOCK_H
#define EMPTYDOCK_H

#include "dock.h"

class EmptyDock : public Dock
{
public:
    explicit EmptyDock(QWidget *parent = nullptr);
    ~EmptyDock();
    QString getIdentifier() {
        return "Empty Dock";
    }
};

#endif // EMPTYDOCK_H
