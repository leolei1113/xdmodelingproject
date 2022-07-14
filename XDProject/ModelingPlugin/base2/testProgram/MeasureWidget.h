#pragma once

#include <QWidget>
#include "ui_MeasureWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class MeasureWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    MeasureWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~MeasureWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::MeasureWidget ui;

    BOOLTYPE            m_boolType;
};
