#pragma once

#include <QWidget>
#include "ui_RevolWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class RevolWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    RevolWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~RevolWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::RevolWidget ui;
};
