#pragma once

#include <QWidget>
#include "ui_FlareWidget.h"
#include "ElemWidgetFactory.h"

class FlareWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    FlareWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~FlareWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::FlareWidget ui;
};
