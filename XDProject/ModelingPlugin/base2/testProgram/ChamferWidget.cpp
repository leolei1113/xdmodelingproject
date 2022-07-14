#include "ChamferWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

ChamferWidget::ChamferWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

ChamferWidget::~ChamferWidget()
{
}

bool ChamferWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    if (PIDs.size() < 1)
    {
        return false;
    }
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());

    sourceStr = ui.lineEdit_PIDs2->text();
    PIDs = sourceStr.split(",");
    if (PIDs.size() < 0)
    {
        return false;
    }
    std::list<std::string> entityList;
    for (int i = 0; i < PIDs.size(); ++i)
    {
        entityList.push_back(PIDs[i].toStdString());
    }

    double lDistance = ui.lineEdit_x1->text().toDouble();
    double rDistance = ui.lineEdit_y1->text().toDouble();
    double setback = ui.lineEdit_x2->text().toDouble();

    CHAMFERTYPE type = ui.checkBox->isChecked() ? Symmetric : Asymmetric;

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.chamfer(origEntity, entityList, setback, lDistance, rDistance, type, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
