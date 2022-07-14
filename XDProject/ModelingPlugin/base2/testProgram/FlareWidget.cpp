#include "FlareWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

FlareWidget::FlareWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

FlareWidget::~FlareWidget()
{
}

bool FlareWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double w1 = ui.lineEdit_x2->text().toDouble();
    double l1 = ui.lineEdit_y2->text().toDouble();
    double w2 = ui.lineEdit_z2->text().toDouble();
    double l2 = ui.lineEdit_length2->text().toDouble();
    double height = ui.lineEdit_height->text().toDouble();

    DATUMAXISTYPE axis = m_dlg->getAxis();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createFlare(wcsMatrix, x1, y1, z1, w1, l1, w2, l2, height, axis, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
