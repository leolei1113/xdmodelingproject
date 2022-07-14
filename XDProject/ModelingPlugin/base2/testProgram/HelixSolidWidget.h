#pragma once

#include <QWidget>
#include "ui_HelixSolidWidget.h"
#include "ElemWidgetFactory.h"

class HelixSolidWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    HelixSolidWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~HelixSolidWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::HelixSolidWidget ui;
};
