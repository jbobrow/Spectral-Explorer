#ifndef DOCK_H
#define DOCK_H

#include <QWidget>
#include <QDockWidget>

class Dock : public QWidget
{
    Q_OBJECT
public:
    explicit Dock(QWidget *parent = nullptr);

    virtual QString getIdentifier() = 0;

public slots:
    void onVisibiltyChanged(bool v);

protected:
    virtual void resizeEvent(QResizeEvent *);
};

#endif // DOCK_H
