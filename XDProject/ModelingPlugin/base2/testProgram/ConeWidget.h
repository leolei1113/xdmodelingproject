#pragma once

#include <QWidget>
#include "ui_ConeWidget.h"
#include "ElemWidgetFactory.h"

class ConeWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    ConeWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~ConeWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::ConeWidget ui;
};
