// SPDX-License-Identifier: GPL-3.0

#include "mpvwidget.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QWindow>
#include <QWidget>
#include <QScreen>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDebug>

#include <LayerShellQt/Window>
#include <LayerShellQt/Shell>

int main(int argc, char *argv[])
{
    LayerShellQt::Shell::useLayerShell();

    QApplication::setApplicationName("simple");
    QApplication::setApplicationVersion("dev");
    // Qt6 removed Qt::AA_ImmediateWidgetCreation
    QApplication::setAttribute(Qt::AA_ImmediateWidgetCreation);

    QApplication app(argc, argv);
    setlocale(LC_NUMERIC, "C");

    QCommandLineParser parser;
    parser.setApplicationDescription(app.applicationName() + app.applicationVersion());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("FILE", "Files to play.", "FILE...");

    QCommandLineOption noLoopOption({ "n", "no-loop" }, "Do not loop. Defaults to loop.");

    parser.addOptions({ noLoopOption });
    parser.process(app);

    QWidget root;

    auto layerShell = LayerShellQt::Window::get(root.windowHandle());
    // setLayer
    layerShell->setLayer(LayerShellQt::Window::Layer::LayerBackground);
    // setAnchors
    layerShell->setAnchors({});
    // setMargins
    layerShell->setMargins({ 0, 0, 0, 0 });
    // setExclusiveZone
    layerShell->setExclusiveZone(-1);
    // setKeyboardInteractivity
    layerShell->setKeyboardInteractivity(
            LayerShellQt::Window::KeyboardInteractivity::KeyboardInteractivityNone);
    // setDesiredOutput
    layerShell->setDesiredOutput(QGuiApplication::primaryScreen());

    QVBoxLayout layout;
    root.setLayout(&layout);
    layout.setContentsMargins({ 0, 0, 0, 0 });
    root.setContentsMargins({ 0, 0, 0, 0 });
    root.resize(QGuiApplication::primaryScreen()->size());
    MpvWidget m_mpv;

    m_mpv.setStyleSheet("* {margin: 0px 0px; padding:0px 0px; background: cyan;}");
    layout.addWidget(&m_mpv);
    // fixed scale for hdpi screen
    m_mpv.setScale(QGuiApplication::primaryScreen()->devicePixelRatio());
    m_mpv.setContentsMargins({ 0, 0, 0, 0 });
    m_mpv.setProperty("loop", true);
    m_mpv.setProperty("panscan", 1);
    m_mpv.setProperty("volume", 50);
    qDebug() << parser.isSet(noLoopOption);
    m_mpv.command(QStringList() << "loadfile"
                                << QFileInfo(parser.positionalArguments()[0]).absoluteFilePath());

    root.show();
    return app.exec();
}
