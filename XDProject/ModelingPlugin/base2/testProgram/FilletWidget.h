#pragma once

#include <QWidget>
#include "ui_FilletWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class FilletWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    FilletWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~FilletWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::FilletWidget ui;
};
