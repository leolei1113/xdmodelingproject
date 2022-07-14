#include "CylinderWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

CylinderWidget::CylinderWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

CylinderWidget::~CylinderWidget()
{
}

bool CylinderWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double height = ui.lineEdit_height->text().toDouble();
    double radius = ui.lineEdit_radius->text().toDouble();
    bool sideFace = ui.checkBox_side->isChecked();

    DATUMAXISTYPE axis = m_dlg->getAxis();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (sideFace)
    {
        if (builder.createCylinderFace(wcsMatrix, x1, y1, z1, axis, radius, height, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }
    else
    {
        if (builder.createCylinder(wcsMatrix, x1, y1, z1, radius, height, axis, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }
    return false;
}
