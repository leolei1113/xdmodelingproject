#pragma once

#include <QWidget>
#include <QItemDelegate>

#include "ui_PrismWidget.h"
#include "ElemWidgetFactory.h"

class PrismWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    PrismWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~PrismWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

public slots:

    void on_toolButton_clicked();

private:
    Ui::PrismWidget ui;
};
