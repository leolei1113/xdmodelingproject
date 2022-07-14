#pragma once

#include <QWidget>
#include "ui_NurbsFaceWidget.h"
#include "ElemWidgetFactory.h"

class NurbsFaceWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    NurbsFaceWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~NurbsFaceWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

protected slots:
	void on_toolButton_hplus1_clicked();
	void on_toolButton_vplus1_clicked();
	void on_toolButton_hminus1_clicked();
	void on_toolButton_vminus1_clicked();
private:
    Ui::NurbsFaceWidget ui;
};
