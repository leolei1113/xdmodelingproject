#pragma once

#include <QWidget>
#include "ui_HyperbolaWidget.h"
#include "ElemWidgetFactory.h"

class HyperbolaWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    HyperbolaWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~HyperbolaWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::HyperbolaWidget ui;
};
