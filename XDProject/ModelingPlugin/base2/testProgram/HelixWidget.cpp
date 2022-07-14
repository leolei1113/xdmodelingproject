#include "HelixWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

HelixWidget::HelixWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

HelixWidget::~HelixWidget()
{
}

bool HelixWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double firstRadius = ui.lineEdit_x2->text().toDouble();
    double secondRadius = ui.lineEdit_y2->text().toDouble();
    double turns = ui.lineEdit_z2->text().toDouble();
    double rightHanded = ui.lineEdit_righthanded->text().toDouble();
    double height = ui.lineEdit_height->text().toDouble();

    DATUMAXISTYPE axis = m_dlg->getAxis();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createHelix(wcsMatrix, x1, y1, z1, firstRadius,secondRadius, height, turns, rightHanded, axis, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
