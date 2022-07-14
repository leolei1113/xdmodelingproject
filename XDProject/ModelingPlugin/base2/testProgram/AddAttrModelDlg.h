#pragma once

#include <QDialog>
#include "ui_AddAttrModelDlg.h"


class AddAttrModelDlg : public QDialog
{
    Q_OBJECT

public:
    AddAttrModelDlg(QWidget *parent = Q_NULLPTR);
    ~AddAttrModelDlg();

protected slots:
    void on_toolButton_ok_clicked();

protected:
    Ui::AddAttrModelDlg ui;
};