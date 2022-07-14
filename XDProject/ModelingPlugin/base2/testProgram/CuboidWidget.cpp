#include "CuboidWidget.h"
#include "OccModelBuilder.h"

CuboidWidget::CuboidWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

CuboidWidget::~CuboidWidget()
{
}

bool CuboidWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double x2 = ui.lineEdit_x2->text().toDouble();
    double y2 = ui.lineEdit_y2->text().toDouble();
    double z2 = ui.lineEdit_z2->text().toDouble();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createCuboid(wcsMatrix, x1, y1, z1, x2, y2, z2, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
