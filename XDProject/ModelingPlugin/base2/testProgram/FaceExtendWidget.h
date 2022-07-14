#pragma once

#include <QWidget>
#include "ui_FaceExtendWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class FaceExtendWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    FaceExtendWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~FaceExtendWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::FaceExtendWidget ui;
};
