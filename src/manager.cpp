// SPDX-License-Identifier: GPL-3.0

#include "manager.h"
#include "mpvwidget.h"
#include "paperwidget.h"
#include "qwidget.h"
#include "qwindowdefs.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QScreen>
#include <QDebug>
#include <QProcess>

#include <LayerShellQt/Window>
#include <LayerShellQt/Shell>

void Manager::setupEnv()
{
    if (QApplication::platformName() == "wayland") {
        LayerShellQt::Shell::useLayerShell();
    }

    // mpv must
    setlocale(LC_NUMERIC, "C");

    // The Nvidia drivers don't work well with EGL
    if (!isNvidiaGPU())
        qputenv("QT_XCB_GL_INTEGRATION", "xcb_egl");
}

void Manager::setupLayerShell(QWindow *window, QScreen *screen)
{
    LayerShellQt::Window *layerShellWindow = LayerShellQt::Window::get(window);
    // setLayer
    layerShellWindow->setLayer(LayerShellQt::Window::Layer::LayerBackground);
    // setAnchors
    // layerShellWindow->setAnchors({});
    // setMargins
    layerShellWindow->setMargins({ 0, 0, 0, 0 });
    // setExclusiveZone
    layerShellWindow->setExclusiveZone(-1);
    // setKeyboardInteractivity
    layerShellWindow->setKeyboardInteractivity(
            LayerShellQt::Window::KeyboardInteractivity::KeyboardInteractivityNone);
    // setScreenConfiguration
    layerShellWindow->setScreenConfiguration(LayerShellQt::Window::ScreenFromQWindow);
}

Manager::Manager()
{
    setupEnv();
}

Manager::~Manager() { }

void Manager::screenAdd(QScreen *screen)
{
    qDebug() << "screenAdd: " << screen->name() << " " << "scale: " << screen->devicePixelRatio()
             << " " << "size: " << screen->size() << " " << "count: " << mScreens.count() + 1;

    PaperWidget *paper = new PaperWidget();

    // init root-window
    paper->show();
    paper->hide();
    // or paper->winId();

    // parent is necessary, ensure add-in paper
    MpvWidget *m_mpv = new MpvWidget(paper);

    // fullscreen
    m_mpv->resize(screen->size());

    setupLayerShell(paper->windowHandle(), screen);
    // show, ensure order
    paper->show();

    m_mpv->setProperty("loop", true);
    m_mpv->setProperty("loop-playlist", true);
    m_mpv->setProperty("panscan", 1);
    m_mpv->setProperty("volume", 0);
    auto str = QString("/tmp/mpv-socket-") + screen->name();

    m_mpv->setProperty("input-ipc-server", str.toStdString().c_str());

    QString filepath;
    if (mFiles.contains(screen->name())) {
        filepath = mFiles.value(screen->name());
    } else {
        filepath = mFiles.value("*");
    }
    m_mpv->command(QStringList() << "loadfile" << filepath);

    mScreens.insert(screen->name(), screen);
    mMpvs.insert(screen->name(), m_mpv);
    mPaperWidgets.insert(screen->name(), paper);
    QObject::connect(screen, &QObject::destroyed, this, [screen, this] { screenRemove(screen); });
    QObject::connect(screen, &QScreen::geometryChanged, paper,
                     [paper](const QRect &rect) { paper->setGeometry(rect); });
}

void Manager::screenRemove(QScreen *screen)
{
    qDebug() << "screenRemove: " << screen->name() << " " << mScreens.count() - 1;
    //
    QString name = screen->name();

    if (mScreens.contains(name)) {
        auto screen = mScreens.take(name);

        screen->deleteLater();
    }

    if (mMpvs.contains(name)) {
        MpvWidget *widget = mMpvs.take(name);
        widget->close();

        widget->deleteLater();
    }

    if (mPaperWidgets.contains(name)) {
        PaperWidget *widget = mPaperWidgets.take(name);
        widget->close();

        widget->deleteLater();
    }
}

void Manager::startup()
{
    for (QScreen *screen : qApp->screens()) {
        screenAdd(screen);
    }
}

bool Manager::isNvidiaGPU()
{
    QProcess process;
    qDebug() << "manager:" << "Display devices:";
    process.start("lspci", QStringList() << "-v" << "-d" << "::0300");
    process.waitForFinished(1000);
    QString result = process.readAllStandardOutput();
    qDebug() << result;

    if (result.contains("nvidia", Qt::CaseSensitivity::CaseInsensitive) > 0)
        return true;
    else
        return false;
}
