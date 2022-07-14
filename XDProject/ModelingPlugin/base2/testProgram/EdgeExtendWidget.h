#pragma once

#include <QWidget>
#include "ui_EdgeExtendWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class EdgeExtendWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    EdgeExtendWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~EdgeExtendWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

public slots:
	void on_toolButton_update_clicked();

private:
    Ui::EdgeExtendWidget ui;
};
