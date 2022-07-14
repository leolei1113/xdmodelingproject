#pragma once

#include <QWidget>
#include "ui_MirrorWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class MirrorWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    MirrorWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~MirrorWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

private:
    Ui::MirrorWidget ui;
};
