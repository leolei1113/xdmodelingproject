#pragma once

#include <QWidget>
#include "ui_EdgeIntersectionWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class EdgeIntersectionWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    EdgeIntersectionWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~EdgeIntersectionWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::EdgeIntersectionWidget ui;

    BOOLTYPE            m_boolType;
};
