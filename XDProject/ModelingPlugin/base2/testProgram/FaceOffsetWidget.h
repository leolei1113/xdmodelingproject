#pragma once

#include <QWidget>
#include "ui_FaceOffsetWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class FaceOffsetWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    FaceOffsetWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~FaceOffsetWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::FaceOffsetWidget ui;
};
