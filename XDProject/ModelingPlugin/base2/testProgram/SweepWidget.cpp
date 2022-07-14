#include "SweepWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

SweepWidget::SweepWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

SweepWidget::~SweepWidget()
{
}

bool SweepWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    if (PIDs.size() < 2)
    {
        return false;
    }
    void* profileEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
    void* pathEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[1].toStdString());
    bool pipeshell = ui.checkBox->isChecked();

    double angle = ui.lineEdit_angle->text().toDouble();

    double draftAngle = ui.lineEdit_x2->text().toDouble();
    SWEEPDRAFTTYPE draftType = SWEEPDRAFTTYPE(ui.comboBox->currentIndex());
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (pipeshell)
    {
        if (builder.sweepAlongPathPipeshell(profileEntity, pathEntity, angle, draftAngle, draftType, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }
    else
    {
        if (builder.sweepAlongPath(profileEntity, pathEntity, angle, draftAngle, draftType, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }

    return false; 
}
