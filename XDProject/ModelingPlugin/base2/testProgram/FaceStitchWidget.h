#pragma once

#include <QWidget>
#include "ui_FaceStitchWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class FaceStitchWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    FaceStitchWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~FaceStitchWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::FaceStitchWidget ui;
};
