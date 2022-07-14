#include "SphereWidget.h"
#include "OccModelBuilder.h"

SphereWidget::SphereWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

SphereWidget::~SphereWidget()
{
}

bool SphereWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double radius = ui.lineEdit_radius->text().toDouble();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createSphere(wcsMatrix, x1, y1, z1, radius, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
