// SPDX-License-Identifier: GPL-3.0

#include "manager.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QUrl>
#include <QDebug>

#include <LayerShellQt/Window>
#include <LayerShellQt/Shell>

int main(int argc, char *argv[])
{
    QApplication::setApplicationName(PROJECT_NAME);
    QApplication::setApplicationVersion(PROJECT_VERSION);

    QApplication app(argc, argv);

    // Qt6: removed Qt::AA_ImmediateWidgetCreation
    QApplication::setAttribute(Qt::AA_ImmediateWidgetCreation);
    // mpv must
    setlocale(LC_NUMERIC, "C");

    QCommandLineParser parser;
    parser.setApplicationDescription(app.applicationName() + app.applicationVersion());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("FILE", "Files to play.", "FILE...");

    // QCommandLineOption noLoopOption({"n", "no-loop"},
    //                                 "Do not loop. Defaults to loop.");
    // parser.addOptions({noLoopOption});

    parser.process(app);

    // qDebug() << parser.isSet(noLoopOption);

    Manager *manager = new Manager();
    QString filepath = parser.positionalArguments()[0];
    qDebug() << "filepath: " << filepath;
    manager->mFiles.insert("*", QFileInfo(filepath).absoluteFilePath());

    // TODO

    QObject::connect(&app, &QApplication::screenAdded,
                     [manager](QScreen *screen) { manager->screenAdd(screen); });

    // startup
    manager->startup();

    return app.exec();
}
