#include "spectrometer_window.h"
#include "ui_spectrometer_window.h"
#include <QDockWidget>
#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>
#include <QCloseEvent>
#include "settings.h"
#include "spectrum_optical_dock.h"
#include "spectrum_graph_dock.h"
#include "spectrum_combination_dock.h"
#include "about_box.h"
#include "camera/camera_disposer.h"
#include "spectrum/spectrum_disposer.h"

SpectrometerWindow::SpectrometerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpectrometerWindow)
{
    ui->setupUi(this);
    ui->centralWidget->hide();

    setWindowTitle(tr("Spectral-Explorer Software ") + APP_VERSION);

    QSettings settings;
    QByteArray savedGeometry = settings.value("GuiBehaviour/MainWindowGeometry").toByteArray();
    if (!savedGeometry.isEmpty()) {
        restoreGeometry(savedGeometry);
    } else if (QScreen *screen = QGuiApplication::primaryScreen()) {
        QRect avail = screen->availableGeometry();
        int w = qMax(800, static_cast<int>(avail.width() * 0.75));
        int h = qMax(600, static_cast<int>(avail.height() * 0.75));
        resize(w, h);
        move(avail.center() - rect().center());
    }

    //absorption and emission view mode exclusive
    mViewModeGroup = new QActionGroup(ui->menuView);
    mViewModeGroup->setExclusive(true);
    mViewModeGroup->addAction(ui->actionEmissionMode);
    mViewModeGroup->addAction(ui->actionAbsorptionMode);
    mViewModeGroup->addAction(ui->actionAbsorptionModeSmall);
    ui->actionEmissionMode->setChecked(true);
#ifndef DEVELOP
    ui->menuView->menuAction()->setVisible(false);
    ui->actionAbsorptionMode->setVisible(false);
    ui->actionAbsorptionModeSmall->setVisible(false);
#endif

    connect(SpectrumDisposer::getInstance(), SIGNAL(initializeCamera(int)), this, SLOT(on_cameraInit(int)));
    SpectrumDisposer::getInstance()->startHardwareUpdater();
    connect(&CameraDisposer::getInstance(), SIGNAL(cameraTimout()), this, SLOT(on_cameraTimeout()));
}

SpectrometerWindow::~SpectrometerWindow()
{
    delete ui;
    qDebug() << "Quit Spectral-Explorer";
}

void SpectrometerWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("GuiBehaviour/MainWindowGeometry", saveGeometry());
    QMainWindow::closeEvent(event);
}

void SpectrometerWindow::setupEmissionView()
{
    clearDocks();
    QDockWidget *dock;
    //disable tabify docks
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

    //first row
    dock = new QDockWidget(this);
    mDocks.append(dock);
    SpectrumOpticalDock *od = new SpectrumOpticalDock(this);
    dock->setWidget(od);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::TopDockWidgetArea, dock, Qt::Vertical);
    connect(dock, SIGNAL(visibilityChanged(bool)), od, SLOT(onVisibiltyChanged(bool)));

    //second row
    dock = new QDockWidget(this);
    mDocks.append(dock);
    SpectrumGraphDock *gd = new SpectrumGraphDock(this);
    dock->setWidget(gd);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::BottomDockWidgetArea, dock, Qt::Vertical);
    connect(dock, SIGNAL(visibilityChanged(bool)), gd, SLOT(onVisibiltyChanged(bool)));
}

void SpectrometerWindow::setupAbsorptionView()
{
    clearDocks();
    QDockWidget *dock;
    //disable tabify docks
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

    //first row
    dock = new QDockWidget(this);
    mDocks.append(dock);
    SpectrumOpticalDock *od = new SpectrumOpticalDock(this);
    dock->setWidget(od);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::TopDockWidgetArea, dock, Qt::Vertical);
    connect(dock, SIGNAL(visibilityChanged(bool)), od, SLOT(onVisibiltyChanged(bool)));

    dock = new QDockWidget(this);
    mDocks.append(dock);
    dock->setMinimumWidth(400);
    od = new SpectrumOpticalDock(this);
    dock->setWidget(od);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::TopDockWidgetArea, dock,Qt::Horizontal);
    connect(dock, SIGNAL(visibilityChanged(bool)), od, SLOT(onVisibiltyChanged(bool)));

    //second row
    dock = new QDockWidget(this);
    mDocks.append(dock);
    SpectrumGraphDock* gd = new SpectrumGraphDock(this);
    gd->mWavelengthSteps = 100;
    dock->setWidget(gd);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::BottomDockWidgetArea, dock, Qt::Vertical);
    connect(dock, SIGNAL(visibilityChanged(bool)), gd, SLOT(onVisibiltyChanged(bool)));

    dock = new QDockWidget(this);
    mDocks.append(dock);
    dock->setMinimumWidth(400);
    gd = new SpectrumGraphDock(this);
    gd->mWavelengthSteps = 100;
    dock->setWidget(gd);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::BottomDockWidgetArea, dock, Qt::Horizontal);
    connect(dock, SIGNAL(visibilityChanged(bool)), gd, SLOT(onVisibiltyChanged(bool)));

    //third row
    dock = new QDockWidget(this);
    mDocks.append(dock);
    SpectrumCombinationDock *cd = new SpectrumCombinationDock;
    dock->setWidget(cd);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::BottomDockWidgetArea, dock, Qt::Vertical);
    connect(dock, SIGNAL(visibilityChanged(bool)), cd, SLOT(onVisibiltyChanged(bool)));
}

void SpectrometerWindow::setupAbsorptionViewSmall()
{
    clearDocks();
    QDockWidget *dock;
    //disable tabify docks
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

    //first row
    dock = new QDockWidget(this);
    mDocks.append(dock);
    SpectrumOpticalDock *od = new SpectrumOpticalDock(this);
    dock->setWidget(od);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::TopDockWidgetArea, dock, Qt::Vertical);
    connect(dock, SIGNAL(visibilityChanged(bool)), od, SLOT(onVisibiltyChanged(bool)));

    dock = new QDockWidget(this);
    mDocks.append(dock);
    dock->setMinimumWidth(400);
    od = new SpectrumOpticalDock(this);
    dock->setWidget(od);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::TopDockWidgetArea, dock,Qt::Horizontal);
    connect(dock, SIGNAL(visibilityChanged(bool)), od, SLOT(onVisibiltyChanged(bool)));

    //second row
    dock = new QDockWidget(this);
    mDocks.append(dock);
    SpectrumCombinationDock *cd = new SpectrumCombinationDock;
    cd->setSpectrum2(QString(tr("Device 1")));
    dock->setWidget(cd);
    dock->setFeatures(nullptr);
    addDockWidget(Qt::BottomDockWidgetArea, dock, Qt::Vertical);
    connect(dock, SIGNAL(visibilityChanged(bool)), cd, SLOT(onVisibiltyChanged(bool)));
}

void SpectrometerWindow::clearDocks()
{
    for (QDockWidget* d : mDocks) {
        removeDockWidget(d);
        delete d;
    }
    mDocks.clear();
}

void SpectrometerWindow::debug()
{
    qDebug() << "v";
}

void SpectrometerWindow::on_cameraTimeout()
{
    ui->statusBar->showMessage(tr("   Camera timed out. Please quit the application and connect the spectrometer to another USB-port."),3000);
}

void SpectrometerWindow::on_cameraInit(int devID)
{
    ui->statusBar->showMessage(QString(tr("   Initialize Camera ...")).arg(devID),3000);
}

void SpectrometerWindow::on_actionEmissionMode_toggled(bool arg1)
{
    if (arg1 == true)
    {
//        CameraDisposer::getInstance().stopAllInterfaces();
        mTimer = new QTimer(this);
        mTimer->singleShot(50,this, SLOT(setupEmissionView()));
    }
}

void SpectrometerWindow::on_actionAbsorptionMode_toggled(bool arg1)
{
    if (arg1 == true)
    {
//        CameraDisposer::getInstance().stopAllInterfaces();
        mTimer = new QTimer(this);
        mTimer->singleShot(50,this, SLOT(setupAbsorptionView()));
    }
}

void SpectrometerWindow::on_actionAbsorptionModeSmall_toggled(bool arg1)
{
    if (arg1 == true)
    {
//        CameraDisposer::getInstance().stopAllInterfaces();
        mTimer = new QTimer(this);
        mTimer->singleShot(50,this, SLOT(setupAbsorptionViewSmall()));
    }
}

void SpectrometerWindow::on_actionSettings_triggered()
{
    Settings::getInstance().setWindowModality(Qt::ApplicationModal);
    Settings::getInstance().show();
}

void SpectrometerWindow::on_actionAbout_Spectral_Explorer_triggered()
{
//    QMessageBox msgBox;
//    msgBox.setText(QString(tr("Spectral-Explorer Software Version ")).append(APP_VERSION));
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    int ret = msgBox.exec();
    AboutBox aboutBox;
    aboutBox.exec();
}
