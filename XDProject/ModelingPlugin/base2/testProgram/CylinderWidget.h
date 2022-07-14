#pragma once

#include <QWidget>
#include "ui_CylinderWidget.h"
#include "ElemWidgetFactory.h"

class CylinderWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    CylinderWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~CylinderWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::CylinderWidget ui;
};
