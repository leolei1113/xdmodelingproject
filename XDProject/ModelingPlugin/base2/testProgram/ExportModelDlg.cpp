#include "ExportModelDlg.h"
#include "OccApplication.h"
#include "OccModel.h"
#include "OccModelBuilder.h"

#include <string>
#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>
#include <TPrsStd_AISPresentation.hxx>

#include <QFileDialog>

ExportModelDlg::ExportModelDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

ExportModelDlg::~ExportModelDlg()
{

}

void ExportModelDlg::on_toolButton_ok_clicked()
{
    TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
    Handle(TDF_Data) data = curDoc->GetData();
    TDF_Label rootLabel = data->Root();

    QString sourceStr = ui.lineEdit_PID->text();
    QStringList PIDs = sourceStr.split(",");
    std::list<void*> entityList;

    if (PIDs.size() < 1)
    {
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "", "", "STEP model (*.step);; IGES model (*.iges);; BREP model (*.brep)");
    if (path.isEmpty())
        return;

    QFileInfo fileInfo(path);
    OccModelBuilder::EnFileType type;
    if (fileInfo.suffix() == "step")
    {
        type = OccModelBuilder::ENSTP;
    }
    else if (fileInfo.suffix() == "iges")
    {
        type = OccModelBuilder::ENIGS;
    } 
    else if (fileInfo.suffix() == "brep")
    {
        type = OccModelBuilder::ENBREP;
    }
    else    
    { 
        return;
    }    

    if (!OccModelBuilder::exportModel(path.toStdString(), type, PIDs[0].toStdString()))
    {
        return; 
    }
}