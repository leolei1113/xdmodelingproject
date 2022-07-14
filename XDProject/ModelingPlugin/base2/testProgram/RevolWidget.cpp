#include "RevolWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

RevolWidget::RevolWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

RevolWidget::~RevolWidget()
{
}

bool RevolWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    if (PIDs.size() < 1)
    {
        return false;
    }
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());

    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double xAxis = ui.lineEdit_axisx->text().toDouble();
    double yAxis = ui.lineEdit_axisy->text().toDouble();
    double zAxis = ui.lineEdit_axisz->text().toDouble();
    double angle = ui.lineEdit_angle->text().toDouble();

    double draftAngle = ui.lineEdit_x2->text().toDouble();
    SWEEPDRAFTTYPE draftType = SWEEPDRAFTTYPE(ui.comboBox->currentIndex());
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.sweepAroundAxis(origEntity, wcsMatrix, x1, y1, z1, xAxis, yAxis, zAxis, angle, draftAngle, draftType, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
