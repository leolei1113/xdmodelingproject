#pragma once

#include <QWidget>
#include "ui_ScaleWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class ScaleWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    ScaleWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~ScaleWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::ScaleWidget ui;
};
