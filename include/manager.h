// SPDX-License-Identifier: GPL-3.0

#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QHash>

class QScreen;
class QWindow;
class PaperWidget;
class MpvWidget;

class Manager Q_DECL_FINAL : public QObject
{
    Q_OBJECT
public:
    Manager();
    ~Manager();
    void setupWayland();
    void setupLayerShell(QWindow *window, QScreen *screen);

Q_SIGNALS:

protected:
public slots:
    void screenAdd(QScreen *screen);
    void screenRemove(QScreen *screen);

    void startup();

protected:
    QHash<QString, QScreen *> mScreens;
    QHash<QString, MpvWidget *> mMpvs;
    QHash<QString, PaperWidget *> mPaperWidgets;

public:
    QHash<QString, QString> mFiles;
};

#endif // MANAGER_H
