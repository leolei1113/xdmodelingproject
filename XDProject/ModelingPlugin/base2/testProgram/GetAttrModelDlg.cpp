#include "GetAttrModelDlg.h"
#include "OccApplication.h"
#include "OccModel.h"

#include <string>

GetAttrModelDlg::GetAttrModelDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

GetAttrModelDlg::~GetAttrModelDlg()
{
}


void GetAttrModelDlg::on_toolButton_ok_clicked()
{
    QString strPID = ui.lineEdit_PID->text();
    QString attrName = ui.lineEdit_attrName->text();

    if (strPID.isEmpty()|| attrName.isEmpty())
    {
        return;
    }

    void* entity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(strPID.toStdString());
    if (!entity)
    {
        ui.listWidget->addItem("The PID doesn't exist!");
        return;
    }

    std::string strV;
    int intV;
    if (TheOCCApp.getCurrentModel()->GetEntityAttribute(entity, attrName.toStdString(), strV))
    {
        QString text = "The '" + attrName + "' values is " + QString::fromStdString(strV) + " in STRING format";
        ui.listWidget->addItem(text);
    }
    else if (TheOCCApp.getCurrentModel()->GetEntityAttribute(entity, attrName.toStdString(), intV))
    {
        QString text = "The '" + attrName + "' values is " + QString::number(intV) + " in INT format";
        ui.listWidget->addItem(text);
    }
    else
    {
        ui.listWidget->addItem("No corresponding attribute!");
    }

    return;
}