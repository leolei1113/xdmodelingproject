#pragma once

#include <QWidget>
#include "ui_ExtrudeWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class ExtrudeWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    ExtrudeWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~ExtrudeWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::ExtrudeWidget ui;
};
