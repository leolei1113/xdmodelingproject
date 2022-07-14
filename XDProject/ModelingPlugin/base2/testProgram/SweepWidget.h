#pragma once

#include <QWidget>
#include "ui_SweepWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class SweepWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    SweepWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~SweepWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::SweepWidget ui;
};
