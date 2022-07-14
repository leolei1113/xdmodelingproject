#pragma once

#include <QWidget>
#include "ui_HelixWidget.h"
#include "ElemWidgetFactory.h"

class HelixWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    HelixWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~HelixWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::HelixWidget ui;
};
