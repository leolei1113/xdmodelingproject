#pragma once

#include <QWidget>
#include "ui_CuboidWidget.h"
#include "ElemWidgetFactory.h"

class CuboidWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    CuboidWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~CuboidWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::CuboidWidget ui;
};
