#pragma once

#include <QWidget>
#include "ui_ChamferWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class ChamferWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    ChamferWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~ChamferWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::ChamferWidget ui;
};
