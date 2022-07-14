#pragma once

#include <QWidget>
#include "ui_TranslateWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class TranslateWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    TranslateWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~TranslateWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::TranslateWidget ui;
};
