#pragma once

#include <QWidget>
#include "ui_FaceSplitWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class FaceSplitWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    FaceSplitWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~FaceSplitWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::FaceSplitWidget ui;

    BOOLTYPE            m_boolType;
};
