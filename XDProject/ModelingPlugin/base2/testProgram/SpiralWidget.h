#pragma once

#include <QWidget>
#include "ui_SpiralWidget.h"
#include "ElemWidgetFactory.h"

class SpiralWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    SpiralWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~SpiralWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::SpiralWidget ui;
};
