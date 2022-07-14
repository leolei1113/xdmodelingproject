#pragma once

#include <QWidget>
#include "ui_EllipseWidget.h"
#include "ElemWidgetFactory.h"

class EllipseWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    EllipseWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~EllipseWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::EllipseWidget ui;
};
