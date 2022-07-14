#pragma once

#include <QWidget>
#include "ui_EdgeCombinationWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class EdgeCombinationWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    EdgeCombinationWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~EdgeCombinationWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::EdgeCombinationWidget ui;

    BOOLTYPE            m_boolType;
};
