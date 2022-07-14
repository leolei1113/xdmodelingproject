#pragma once

#include <QWidget>
#include "ui_EdgeOffsetWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class EdgeOffsetWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    EdgeOffsetWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~EdgeOffsetWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::EdgeOffsetWidget ui;
};
