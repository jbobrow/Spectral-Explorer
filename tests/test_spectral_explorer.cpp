#include <QtTest/QtTest>
#include <QPainter>
#include <QTemporaryDir>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "camera/camera_disposer.h"
#include "camera/capture_worker.h"
#include "gui/config_dialog.h"
#include "gui/graph_viewer.h"
#include "gui/opencv_viewer.h"
#include "gui/spectrum_selector.h"
#include "settings/setting.h"
#include "spectrum/spectrum.h"
#include "spectrum/spectrum_config.h"
#include "spectrum/spectrum_data.h"
#include "spectrum/spectrum_disposer.h"
#include "spectrum/spectrum_intensity_colored_drawing.h"
#include "spectrum/spectrum_intensity_drawing.h"
#include "spectrum/spectrum_optical_drawing.h"
#include "spectrum/spectrum_rgb_drawing.h"

Q_DECLARE_METATYPE(cv::Mat)

class TestSpectralExplorer : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        qRegisterMetaType<cv::Mat>("cv::Mat");
        QCoreApplication::setOrganizationName("Spectral-Explorer-Test");
        QCoreApplication::setApplicationName("Spectral-Explorer-UnitTests");
    }

    void testSpectrumDataCalibrationAndCsv()
    {
        cv::Mat line(1, 200, CV_8UC3, cv::Scalar(100, 150, 200));
        line.at<cv::Vec3b>(0, 100) = cv::Vec3b(255, 255, 255);

        SpectrumData data(line, 1, 50, 150, nullptr);
        QCOMPARE(data.mSpecWidth, 200);
        QCOMPARE(data.mData.length(), 200);
        QVERIFY(qAbs(data.mData[50].wavelength - 436.6) < 0.01);
        QVERIFY(qAbs(data.mData[150].wavelength - 611.6) < 0.01);

        // x2w / w2x round-trip
        double w = data.x2w(75.0);
        QVERIFY(qAbs(data.w2x(w) - 75.0) < 0.01);

        // Linear interpolation on populated data
        QVERIFY(data.w2I(436.6) > 0.0);

        double maxScale = data.normalizeIntensity();
        QVERIFY(maxScale > 0.0);

        QStringList csv = data.exportCSV();
        QCOMPARE(csv.length(), 200);
        QVERIFY(csv[0].contains(';'));

        // Edge-case guards: equal calibration x, zero intensity, empty data
        SpectrumData emptyData;
        QCOMPARE(emptyData.w2I(500.0), 0.0);
        QCOMPARE(emptyData.normalizeIntensity(), 1.0);
        emptyData.configure(10, 436.6f, 10, 611.6f);
        emptyData.scaleSpecLine(0);
    }

    void testSpectrumDataOperators()
    {
        cv::Mat lineA(1, 50, CV_8UC3, cv::Scalar(100, 100, 100));
        cv::Mat lineB(1, 50, CV_8UC3, cv::Scalar(40, 40, 40));

        SpectrumData a(lineA, 1, 10, 40, nullptr);
        SpectrumData b(lineB, 2, 10, 40, nullptr);

        SpectrumData sum = a + b;
        SpectrumData diff = a - b;
        SpectrumData scaled = a * 2.0;

        QCOMPARE(sum.mSpecWidth, 50);
        QCOMPARE(diff.mSpecWidth, 50);
        QCOMPARE(scaled.mSpecWidth, 50);
        QVERIFY(qAbs(sum.mData[0].r - (a.mData[0].r + b.mData[0].r)) < 1e-6);
        QVERIFY(qAbs(diff.mData[0].r - (a.mData[0].r - b.mData[0].r)) < 1e-6);
        QVERIFY(qAbs(scaled.mData[0].r - (a.mData[0].r * 2.0)) < 1e-6);
    }

    void testSpectrumConfigSaveLoadAndOffsets()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        QString cfgPath = dir.filePath("test.config");

        Camera *cam = CameraDisposer::getInstance().createFileCamera("dummy.png");
        SpectrumConfig cfg(cam);
        cfg.setExposure(12);
        cfg.setGain(180);
        cfg.setAverageFrames(5);
        cfg.setLeftOffset(100);
        cfg.setRightOffset(50);
        cfg.setCFLX(200, 500);
        cfg.save(cfgPath);

        SpectrumConfig loaded(cam);
        loaded.load(cfgPath);
        QCOMPARE(loaded.getExposure(), 12);
        QCOMPARE(loaded.getGain(), 180);
        QCOMPARE(loaded.getAverageFrames(), 5);
        QCOMPARE(loaded.getLeftOffset(), 100);
        QCOMPARE(loaded.getRightOffset(), 50);
        QCOMPARE(loaded.getCFLX1(), 200);
        QCOMPARE(loaded.getCFLX2(), 500);

        // Changing left offset shifts CFLX accordingly
        loaded.setLeftOffset(120);
        QCOMPARE(loaded.getCFLX1(), 180);
        QCOMPARE(loaded.getCFLX2(), 480);

        CameraDisposer::getInstance().deleteCamera(cam);
    }

    void testCameraFileLoadsImage()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        QString imgPath = dir.filePath("sample_spectrum.png");

        cv::Mat img(120, 320, CV_8UC3, cv::Scalar(30, 80, 180));
        QVERIFY(cv::imwrite(imgPath.toLocal8Bit().constData(), img));

        Spectrum *s = new Spectrum(imgPath);
        QVERIFY(s->getCamera()->isVirtual());
        s->getCamera()->setActive(true);
        QCOMPARE(s->getCamera()->getWidth(), 320);
        QCOMPARE(s->getCamera()->getHeight(), 120);
        QCOMPARE(s->getSpectrumData().mSpecWidth, 320);

        delete s;
    }

    void testSpectrumDisposerRenameNoDuplicate()
    {
        SpectrumDisposer *disposer = SpectrumDisposer::getInstance();
        int initialCount = disposer->getSpectrums().length();

        Spectrum *s1 = new Spectrum(QString("nonexistent_test_1.png"));
        Spectrum *s2 = new Spectrum(QString("nonexistent_test_2.png"));

        QCOMPARE(disposer->getSpectrums().length(), initialCount + 2);

        disposer->renameSpectrum(s1, "RenamedSpectrum");
        QCOMPARE(s1->mName, QString("RenamedSpectrum"));
        QCOMPARE(disposer->getSpectrums().length(), initialCount + 2);
        QCOMPARE(disposer->getSpectrums().count(s1), 1);

        disposer->renameSpectrum(s2, "RenamedSpectrum");
        QCOMPARE(s2->mName, QString("RenamedSpectrum#1"));
        QCOMPARE(disposer->getSpectrums().length(), initialCount + 2);

        delete s1;
        delete s2;
        QCOMPARE(disposer->getSpectrums().length(), initialCount);
    }

    void testSpectrumOnNewFrameGuards()
    {
        Spectrum *s = new Spectrum(QString("nonexistent_frame_test.png"));

        QVERIFY(QMetaObject::invokeMethod(s, "onNewFrame", Qt::DirectConnection,
                                          Q_ARG(cv::Mat, cv::Mat())));

        s->getConfig()->setLeftOffset(500);
        s->getConfig()->setRightOffset(500);
        cv::Mat smallFrame(10, 100, CV_8UC3, cv::Scalar(10, 20, 30));
        QVERIFY(QMetaObject::invokeMethod(s, "onNewFrame", Qt::DirectConnection,
                                          Q_ARG(cv::Mat, smallFrame)));

        QCOMPARE(s->getSpectrumData().mSpecWidth, 100);

        delete s;
    }

    void testDrawingStrategiesOnImage()
    {
        cv::Mat line(1, 200, CV_8UC3, cv::Scalar(50, 120, 220));
        SpectrumData data(line, 1, 40, 160, nullptr);

        QImage canvas(400, 200, QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&canvas);
        QRect rect(0, 0, 400, 200);

        SpectrumIntensityDrawing intensityDr(data);
        intensityDr.addSpectrumData(data);
        intensityDr.draw(&painter, rect);

        SpectrumRGBDrawing rgbDr(data);
        rgbDr.draw(&painter, rect);

        SpectrumIntensityColoredDrawing coloredDr(data);
        coloredDr.draw(&painter, rect);

        QList<SpectrumData> opticalRows;
        opticalRows.append(data);
        opticalRows.append(data);
        SpectrumOpticalDrawing opticalDr(&opticalRows);
        opticalDr.draw(&painter, rect);

        // Empty-data edge cases should not crash
        SpectrumData empty;
        SpectrumIntensityDrawing emptyInt(empty);
        emptyInt.addSpectrumData(empty);
        emptyInt.draw(&painter, rect);

        SpectrumRGBDrawing emptyRgb(empty);
        emptyRgb.draw(&painter, rect);

        QList<SpectrumData> emptyRows;
        SpectrumOpticalDrawing emptyOpt(&emptyRows);
        emptyOpt.draw(&painter, QRect(0, 0, 0, 0));

        painter.end();
    }

    void testSpectrumSelectorSwitchAndDelete()
    {
        Spectrum *s1 = new Spectrum(QString("sel_test_1.png"));
        Spectrum *s2 = new Spectrum(QString("sel_test_2.png"));

        SpectrumSelector selector(true, true);
        selector.setSpectrum(s1->mName);
        QCOMPARE(selector.getSpectrum(), s1);

        selector.setSpectrum(s2->mName);
        QCOMPARE(selector.getSpectrum(), s2);

        selector.setDeletable(false);
        selector.setDeletable(true);

        delete s1;
        delete s2;
    }

    void testOpenCVViewerRejectsEmptyFrame()
    {
        OpenCVViewer viewer;
        viewer.resize(320, 240);
        QVERIFY(!viewer.showImage(cv::Mat(), 0));
    }

    void testSettingRoundTrip()
    {
        Setting setting("Test/SampleInt", 42);
        QCOMPARE(setting.value().toInt(), 42);
        setting.setValue(99);
        QCOMPARE(setting.value().toInt(), 42);
        setting.reset();
        QCOMPARE(setting.value().toInt(), 42);

        setting.setValue(99);
        setting.apply();
        QCOMPARE(setting.value().toInt(), 99);
    }

    void testSpectrumTargetsAndCombine()
    {
        Spectrum *s = new Spectrum(QString("target_combine_test.png"));

        cv::Mat frame(10, 100, CV_8UC3, cv::Scalar(20, 40, 60));
        QVERIFY(QMetaObject::invokeMethod(s, "onNewFrame", Qt::DirectConnection,
                                          Q_ARG(cv::Mat, frame)));

        MeasurementTarget t(500.0f, 0.5f);
        QVERIFY(QMetaObject::invokeMethod(s, "onTargetSelected", Qt::DirectConnection,
                                          Q_ARG(MeasurementTarget, t), Q_ARG(int, 0)));
        QVERIFY(QMetaObject::invokeMethod(s, "onNewFrame", Qt::DirectConnection,
                                          Q_ARG(cv::Mat, frame)));
        QCOMPARE(s->getSpectrumData().targets().length(), 1);

        // Remove within tolerance
        MeasurementTarget tNear(500.5f, 0.502f);
        QVERIFY(QMetaObject::invokeMethod(s, "onTargetSelected", Qt::DirectConnection,
                                          Q_ARG(MeasurementTarget, tNear), Q_ARG(int, 1)));
        QVERIFY(QMetaObject::invokeMethod(s, "onNewFrame", Qt::DirectConnection,
                                          Q_ARG(cv::Mat, frame)));
        QCOMPARE(s->getSpectrumData().targets().length(), 0);

        delete s;
    }

    void testCameraDisposerAndCaptureWorkerGuards()
    {
        Camera *c1 = CameraDisposer::getInstance().createFileCamera("dup_test.png");
        Camera *c2 = CameraDisposer::getInstance().createFileCamera("dup_test.png");
        QVERIFY(c1->getCamID() != c2->getCamID());

        CameraDisposer::getInstance().deleteCamera(c1);
        CameraDisposer::getInstance().deleteCamera(c2);

        CaptureWorker worker(nullptr, 0);
        worker.setAverageFrames(0);
        worker.setAverageFrames(-5);
        QVERIFY(worker.getFrame().empty());
        QCOMPARE(worker.getTimoutCnt(), 0);
    }

    void testGraphViewerMouseEvents()
    {
        GraphViewer viewer;
        viewer.resize(400, 200);

        // Empty drawing should safely ignore clicks
        QTest::mouseClick(&viewer, Qt::LeftButton, Qt::NoModifier, QPoint(100, 100));

        cv::Mat line(1, 200, CV_8UC3, cv::Scalar(50, 120, 220));
        SpectrumData data(line, 1, 40, 160, nullptr);
        SpectrumIntensityDrawing *dr = new SpectrumIntensityDrawing(data);
        dr->addSpectrumData(data);
        viewer.showSpectrumGraph(dr);

        QSignalSpy cflSpy(&viewer, SIGNAL(newCFLX(int)));
        QTest::mouseClick(&viewer, Qt::LeftButton, Qt::NoModifier, QPoint(200, 100));
        QCOMPARE(cflSpy.count(), 1);
    }

    void testConfigDialogCancelRevertsConfig()
    {
        Spectrum *s = new Spectrum(QString("cfg_dialog_test.png"));
        s->getConfig()->setExposure(10);

        ConfigDialog dlg(s);
        dlg.show();
        s->getConfig()->setExposure(25);
        QCOMPARE(s->getConfig()->getExposure(), 25);

        // Closing without confirm should revert to original exposure (10)
        dlg.close();
        QCOMPARE(s->getConfig()->getExposure(), 10);

        delete s;
    }
};

QTEST_MAIN(TestSpectralExplorer)
#include "test_spectral_explorer.moc"
