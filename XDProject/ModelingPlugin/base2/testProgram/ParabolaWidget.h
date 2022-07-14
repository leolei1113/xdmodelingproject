#pragma once

#include <QWidget>
#include "ui_ParabolaWidget.h"
#include "ElemWidgetFactory.h"

class ParabolaWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    ParabolaWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~ParabolaWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::ParabolaWidget ui;
};
