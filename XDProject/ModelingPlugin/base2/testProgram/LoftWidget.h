#pragma once

#include <QWidget>
#include "ui_LoftWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class LoftWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    LoftWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~LoftWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::LoftWidget ui;
};
