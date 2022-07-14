#pragma once

#include <QWidget>
#include "ui_FaceIntersectionWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class FaceIntersectionWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    FaceIntersectionWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~FaceIntersectionWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::FaceIntersectionWidget ui;

    BOOLTYPE            m_boolType;
};
