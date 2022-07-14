#pragma once

#include <QWidget>
#include "ui_TorusWidget.h"
#include "ElemWidgetFactory.h"

class TorusWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    TorusWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~TorusWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::TorusWidget ui;
};
