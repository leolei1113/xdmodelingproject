#pragma once

#include <QWidget>
#include "ui_BooleanWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class BooleanWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    BooleanWidget(BOOLTYPE boolType, DrawModelDlg *parent = Q_NULLPTR);
    ~BooleanWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::BooleanWidget ui;

    BOOLTYPE            m_boolType;
};
