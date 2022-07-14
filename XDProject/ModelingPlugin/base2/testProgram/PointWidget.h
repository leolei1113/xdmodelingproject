#pragma once

#include <QWidget>
#include "ui_PointWidget.h"
#include "ElemWidgetFactory.h"

class PointWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    PointWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~PointWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::PointWidget ui;
};
