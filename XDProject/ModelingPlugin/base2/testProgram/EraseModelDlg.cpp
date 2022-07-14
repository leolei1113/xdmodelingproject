#include "EraseModelDlg.h"
#include "OccApplication.h"
#include "OccModel.h"

#include <string>
#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>
#include <TPrsStd_AISPresentation.hxx>

EraseModelDlg::EraseModelDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

EraseModelDlg::~EraseModelDlg()
{

}

void EraseModelDlg::on_toolButton_ok_clicked()
{
    bool display = ui.checkBox->isChecked();
    TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
    Handle(TDF_Data) data = curDoc->GetData();
    TDF_Label rootLabel = data->Root();

    QString sourceStr = ui.lineEdit_PID->text();
    QStringList PIDs = sourceStr.split(",");
    std::list<void*> entityList;
    for (int i = 0; i < PIDs.size(); ++i)
    {
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(PIDs[i].toStdString().c_str()), label, false);

        if (!label.IsNull())
        {
            Handle(TPrsStd_AISPresentation) prs;
            if (label.FindAttribute(TPrsStd_AISPresentation::GetID(), prs))
            {
                if (display)
                {
                    prs->Display(1);
                }
                else
                {
                    prs->Erase(0);
                }
                prs->Update();
            }
        }
    }
}