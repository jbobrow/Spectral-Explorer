#include "spectrometer_window.h"
#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QString>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <QLocale>
#include <stdio.h>
#include <stdlib.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

const QString logFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
                            "/.Spectral-Explorer";
const QString logFileName =  logFilePath +
                            "/log-" +
                            QDateTime::currentDateTime().toString("yyMMdd-HHmmss") +
                            ".txt";

void loadTranslations(QApplication *a)
{
    if (QLocale::system().language() != QLocale::German)
        return;

    QTranslator *translator = new QTranslator();
    if (!translator->load(":translations/app_de.qm"))
        qWarning("Can't load German translation");
    a->installTranslator(translator);
}

void loadStyleSheet(QApplication *a)
{
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a->setStyleSheet(ts.readAll());
    }
}

void customDebugHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
//    if (!msg.contains("[GraDock]")) return;

    QString debugOutput;
    debugOutput = QTime::currentTime().toString("hh:mm:ss:zzz");

    switch (type) {
    case QtInfoMsg:
        debugOutput += " [Info]";
        break;
    case QtDebugMsg:
        debugOutput += " [Debug]";
        break;
    case QtWarningMsg:
        debugOutput += " [Warning]";
        break;
    case QtCriticalMsg:
        debugOutput += " [Critical]";
        break;
    case QtFatalMsg:
        debugOutput += " [Fatal]";
    }
    debugOutput += ' ' + msg;

#ifdef Q_OS_WIN
    OutputDebugString(reinterpret_cast<const wchar_t *>((debugOutput+'\n').utf16()));
#endif

    QFile logFile(logFileName);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&logFile);
    ts << debugOutput << endl;

    if (type == QtFatalMsg)
        abort();
}

void initLogFiles()
{
    QDir dir(logFilePath);
    if (!dir.exists())
      dir.mkdir(".");
    QStringList oldLogFiles;
    oldLogFiles = dir.entryList(QStringList("log*"),
                                QDir::Files | QDir::NoSymLinks,
                                QDir::Name);
    for(int i = 0; i < oldLogFiles.size() - 20; i++)
    {
        QFile logFile(logFilePath + "/" + oldLogFiles[i]);
        logFile.remove();
    }
    qInstallMessageHandler(customDebugHandler);
//    qSetMessagePattern("\033[32m%{time h:mm:ss.zzz} [%{type}]\t\033[0m %{message}\ ");
}

int main(int argc, char *argv[])
{
    initLogFiles();
    qDebug() << "Start Spectral-Explorer v" << APP_VERSION;
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Spectral-Explorer");
    QCoreApplication::setApplicationName("Spectral-Explorer Software");
    a.setApplicationVersion(APP_VERSION);

    loadTranslations(&a);
    loadStyleSheet(&a);

    qDebug() << "Start GUI.";
    SpectrometerWindow w;
    w.show();

    return a.exec();
}
