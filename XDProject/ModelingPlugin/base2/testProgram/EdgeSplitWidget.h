#pragma once

#include <QWidget>
#include "ui_EdgeSplitWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class EdgeSplitWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    EdgeSplitWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~EdgeSplitWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

public slots:
	void on_toolButton_update_clicked();

private:
    Ui::EdgeSplitWidget ui;
};
