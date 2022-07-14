#pragma once

#include <QDialog>
#include "ui_GetAttrModelDlg.h"


class GetAttrModelDlg : public QDialog
{
    Q_OBJECT

public:
    GetAttrModelDlg(QWidget *parent = Q_NULLPTR);
    ~GetAttrModelDlg();

protected slots:
    void on_toolButton_ok_clicked();

protected:
    Ui::GetAttrModelDlg ui;
};