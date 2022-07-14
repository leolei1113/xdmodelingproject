#include "AddAttrModelDlg.h"
#include "OccApplication.h"
#include "OccModel.h"

#include <string>
#include <TDocStd_Document.hxx>

AddAttrModelDlg::AddAttrModelDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

AddAttrModelDlg::~AddAttrModelDlg()
{
     
}

void AddAttrModelDlg::on_toolButton_ok_clicked()
{
    QString strPID = ui.lineEdit_PID->text();
    QString attrName = ui.lineEdit_attrName->text();

    if (strPID.isEmpty() || attrName.isEmpty())
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
    int type = ui.comboBox->currentIndex();
    if (type == 0)
    {
        COMMAND_BEGIN

        strV = ui.lineEdit_strV->text().toStdString();
        if (TheOCCApp.getCurrentModel()->AddEntityAttribute(entity, attrName.toStdString(), strV))
        {
            QString text = "The '" + attrName + "' values has been successfully added.";
            ui.listWidget->addItem(text);

            COMMAND_END
        }
        else
        {
            ui.listWidget->addItem("Adding attribute failed!");
        }
    }
    else
    {
        COMMAND_BEGIN

        intV = ui.spinBox_intV->value();
        if (TheOCCApp.getCurrentModel()->AddEntityAttribute(entity, attrName.toStdString(), intV))
        {
            QString text = "The '" + attrName + "' values has been successfully added.";
            ui.listWidget->addItem(text);

            COMMAND_END
        }
        else
        {
            ui.listWidget->addItem("Adding attribute failed!");
        }
    }

    return;
}