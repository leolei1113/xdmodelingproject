#pragma once

#include <QWidget>
#include "ui_RegPolyhedronWidget.h"
#include "ElemWidgetFactory.h"

class RegPolyhedronWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    RegPolyhedronWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~RegPolyhedronWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::RegPolyhedronWidget ui;
};
