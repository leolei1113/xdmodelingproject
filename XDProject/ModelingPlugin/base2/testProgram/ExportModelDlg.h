#pragma once

#include <QDialog>
#include "ui_ExportModelDlg.h"


class ExportModelDlg : public QDialog
{
    Q_OBJECT

public:
    ExportModelDlg(QWidget *parent = Q_NULLPTR);
    ~ExportModelDlg();

protected slots:
    void on_toolButton_ok_clicked();

protected:
    Ui::ExportModelDlg ui;
};