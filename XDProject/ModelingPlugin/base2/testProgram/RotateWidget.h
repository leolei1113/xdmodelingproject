#pragma once

#include <QWidget>
#include "ui_RotateWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class RotateWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    RotateWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~RotateWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::RotateWidget ui;
};
