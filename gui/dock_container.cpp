#include "dock_container.h"
#include "ui_dock_container.h"
#include <QIcon>
#include <QComboBox>
#include <QLayout>
#include "spectrum_optical_dock.h"
#include "spectrum_graph_dock.h"
#include "spectrum_combination_dock.h"
#include "empty_dock.h"

DockContainer::DockContainer(QDockWidget *parent) :
    DockContainer(static_cast<QWidget*>(parent))
{
    mDockParent = parent;
}

DockContainer::DockContainer(QWidget *parent) :
    Dock(parent),
    ui(new Ui::DockContainer)
{
    ui->setupUi(this);

    ui->comboBox->addItem(QIcon(":camera_icon.png"),"","Camera Dock");
    ui->comboBox->addItem(QIcon(":graph_icon"),"","Graph Dock");
    ui->comboBox->addItem(QIcon(":combination_icon"),"","Combination Dock");
    ui->comboBox->setIconSize(QSize(32,32));

    mContainedDock = new EmptyDock(this);
    ui->verticalLayout->replaceWidget(ui->containedDock,mContainedDock);

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDock(int)));
}

DockContainer::~DockContainer()
{
    delete ui;
    delete mContainedDock;
}

void DockContainer::changeDock(int comboIdx)
{
    QString id = ui->comboBox->currentData().toString();
    if (id == mContainedDock->getIdentifier())
        return;
    if (id == "Camera Dock")
    {
        setDock(new SpectrumOpticalDock);
    }
    if (id == "Graph Dock")
    {
        setDock(new SpectrumGraphDock);
    }
    if (id == "Combination Dock")
    {
        setDock(new SpectrumCombinationDock);
    }
}

void DockContainer::setDock(Dock *d)
{
    d->setParent(this);
    ui->verticalLayout->replaceWidget(mContainedDock,d);
    delete mContainedDock;
    mContainedDock = d;
    if (mDockParent!=nullptr)
        mDockParent->setWindowTitle(mContainedDock->getIdentifier());
    int cmb = ui->comboBox->findData(mContainedDock->getIdentifier());
    if (cmb != -1)
        ui->comboBox->setCurrentIndex(cmb);
}
