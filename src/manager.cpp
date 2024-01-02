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

#include <LayerShellQt/Window>
#include <LayerShellQt/Shell>

void Manager::setupWayland()
{
    if (QApplication::platformName() == "wayland") {
        LayerShellQt::Shell::useLayerShell();
    }
}

void Manager::setupLayerShell(QWindow *window, QScreen *screen)
{
    LayerShellQt::Window *layerShellWindow = LayerShellQt::Window::get(window);
    // setLayer
    layerShellWindow->setLayer(LayerShellQt::Window::Layer::LayerBottom);
    // setAnchors
    // layerShellWindow->setAnchors({});
    // setMargins
    layerShellWindow->setMargins({ 0, 0, 0, 0 });
    // setExclusiveZone
    layerShellWindow->setExclusiveZone(-1);
    // setKeyboardInteractivity
    layerShellWindow->setKeyboardInteractivity(
            LayerShellQt::Window::KeyboardInteractivity::KeyboardInteractivityNone);
    // setDesiredOutput
    layerShellWindow->setDesiredOutput(screen);
}

Manager::Manager()
{
    setupWayland();
}

Manager::~Manager() { }

void Manager::screenAdd(QScreen *screen)
{
    qDebug() << "screenAdd: " << screen->name() << " "
             << "scale: " << screen->devicePixelRatio() << " "
             << "size: " << screen->size() << " "
             << "count: " << mScreens.count() + 1;

    // // QWindow *window = new QWindow(screen);
    // // window->create();
    // // QWidget *w = QWidget::createWindowContainer(window);
    // QWidget *w = new QWidget;
    // // w->setScreen(screen);
    // w->createWinId();
    // w->windowHandle()->setScreen(screen);

    // MpvWidget *ww = new MpvWidget();
    // ww->createWinId();
    // ww->windowHandle()->setScreen(screen);
    // ww->setScale(screen->devicePixelRatio());

    // QVBoxLayout *layout = new QVBoxLayout();
    // w->setLayout(layout);
    // layout->addWidget(ww);
    // setupLayerShell(w->windowHandle(), screen);

    // QString filepath;
    // if (mFiles.contains(screen->name())) {
    //     filepath = mFiles.value(screen->name());
    // } else {
    //     filepath = mFiles.value("*");
    // }

    // w->show();
    // ww->command(QStringList() << "loadfile" << filepath);
    // qDebug() << "screen: " << ww->window()->windowHandle()->screen()->name();

    PaperWidget *paper = new PaperWidget();

    setupLayerShell(paper->windowHandle(), screen);

    QVBoxLayout *layout = new QVBoxLayout();
    paper->setLayout(layout);
    paper->setContentsMargins({ 0, 0, 0, 0 });
    layout->setContentsMargins({ 0, 0, 0, 0 });

    MpvWidget *m_mpv = new MpvWidget();

    layout->addWidget(m_mpv);
    // fixed scale for hdpi screen
    m_mpv->setScale(screen->devicePixelRatio());
    m_mpv->setContentsMargins({ 0, 0, 0, 0 });

    m_mpv->setProperty("loop", true);
    m_mpv->setProperty("loop-playlist", true);
    m_mpv->setProperty("panscan", 1);
    m_mpv->setProperty("volume", 0);
    auto str= QString("/tmp/mpv-socket-")+screen->name();
    
    m_mpv->setProperty("input-ipc-server", str.toStdString().c_str());

    QString filepath;
    if (mFiles.contains(screen->name())) {
        filepath = mFiles.value(screen->name());
    } else {
        filepath = mFiles.value("*");
    }
    m_mpv->command(QStringList() << "loadfile" << filepath);

    paper->resize(screen->size());
    paper->show();

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