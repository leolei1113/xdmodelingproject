#include "MirrorWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

MirrorWidget::MirrorWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

MirrorWidget::~MirrorWidget()
{
}

bool MirrorWidget::doModelOperation(float* wcsMatrix, const QString& name)
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
    double xNormal = ui.lineEdit_axisx->text().toDouble();
    double yNormal = ui.lineEdit_axisy->text().toDouble();
    double zNormal = ui.lineEdit_axisz->text().toDouble();

    LOGICAL copy = ui.checkBox->isChecked() ? True : False;

    std::string strPID; 
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.mirror(origEntity, wcsMatrix, x1, y1, z1, xNormal, yNormal, zNormal, copy, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
