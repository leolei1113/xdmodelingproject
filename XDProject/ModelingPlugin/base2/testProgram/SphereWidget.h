#pragma once

#include <QWidget>
#include "ui_SphereWidget.h"
#include "ElemWidgetFactory.h"

class SphereWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    SphereWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~SphereWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::SphereWidget ui;
};
