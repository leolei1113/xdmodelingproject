#pragma once

#include <QWidget>
#include "ui_RectangularWidget.h"
#include "ElemWidgetFactory.h"

class RectangularWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    RectangularWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~RectangularWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::RectangularWidget ui;
};
