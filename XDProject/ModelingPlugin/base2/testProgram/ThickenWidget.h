#pragma once

#include <QWidget>
#include "ui_ThickenWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class ThickenWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    ThickenWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~ThickenWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::ThickenWidget ui;
};
