#include "EllipseWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

EllipseWidget::EllipseWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

EllipseWidget::~EllipseWidget()
{
}

bool EllipseWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();


    double majorR = ui.lineEdit_x2->text().toDouble();
    double ratio = ui.lineEdit_y2->text().toDouble();

//     double fDistance = ui.lineEdit_z2->text().toDouble();
    DATUMAXISTYPE axis = m_dlg->getAxis();
    LOGICAL covered = m_dlg->getCovered();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (ratio == 1)
    {
        if (builder.createCircle(wcsMatrix, x1, y1, z1, majorR, axis, covered, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }
    else
    {
        if (builder.createEllipse(wcsMatrix, x1, y1, z1, majorR, ratio, axis, covered, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }
    return false;
}
