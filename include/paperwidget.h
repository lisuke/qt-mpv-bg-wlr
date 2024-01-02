// SPDX-License-Identifier: GPL-3.0

#ifndef PAPERWIDGET_H
#define PAPERWIDGET_H

#include <QWidget>

class MpvWidget;

class PaperWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaperWidget(QWidget *parent = nullptr);
    ~PaperWidget();

public Q_SLOTS:

private:
};

#endif // PAPERWIDGET_H
