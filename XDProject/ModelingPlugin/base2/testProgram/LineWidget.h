#pragma once

#include <QWidget>
#include "ui_LineWidget.h"
#include "ElemWidgetFactory.h"

class LineWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    LineWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~LineWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::LineWidget ui;
};
