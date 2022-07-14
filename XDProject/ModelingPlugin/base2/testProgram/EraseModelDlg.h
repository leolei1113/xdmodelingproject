#pragma once

#include <QDialog>
#include "ui_EraseModelDlg.h"


class EraseModelDlg : public QDialog
{
    Q_OBJECT

public:
    EraseModelDlg(QWidget *parent = Q_NULLPTR);
    ~EraseModelDlg();

protected slots:
    void on_toolButton_ok_clicked();

protected:
    Ui::EraseModelDlg ui;
};