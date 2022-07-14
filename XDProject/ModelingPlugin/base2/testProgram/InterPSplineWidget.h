#pragma once

#include <QWidget>
#include <QItemDelegate>

#include "ui_InterPSplineWidget.h"
#include "ElemWidgetFactory.h"

class InterPSplineWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    InterPSplineWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~InterPSplineWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

public slots:

    void on_toolButton_clicked();

private:
    Ui::InterPSplineWidget ui;
};
