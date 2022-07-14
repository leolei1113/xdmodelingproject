#include "RegPolyhedronWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

RegPolyhedronWidget::RegPolyhedronWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

RegPolyhedronWidget::~RegPolyhedronWidget()
{
}

bool RegPolyhedronWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double x2 = ui.lineEdit_x2->text().toDouble();
    double y2 = ui.lineEdit_y2->text().toDouble();
    double z2 = ui.lineEdit_z2->text().toDouble();

    int numSides = ui.spinBox->value();
    double height = ui.lineEdit_height->text().toDouble();

    bool bottomFace = ui.checkBox_bottomface->isChecked();

    DATUMAXISTYPE axis = m_dlg->getAxis();
    LOGICAL covered = m_dlg->getCovered();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (bottomFace)
    {
        if (builder.createRegularPolygon(wcsMatrix, x1, y1, z1, x2, y2, z2, numSides, axis, covered, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }
    else
    {
        if (builder.createRegularPolyhedron(wcsMatrix, x1, y1, z1, x2, y2, z2, height, numSides, axis, name.toStdString().c_str(), strPID, entity))
        {
            m_newNames << name;
            m_newPIDs << QString::fromStdString(strPID);
            m_newEntities << entity;
            return true;
        }
    }
    return false;

}
