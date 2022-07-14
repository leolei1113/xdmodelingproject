#include "TorusWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

TorusWidget::TorusWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

TorusWidget::~TorusWidget()
{
}

bool TorusWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double R1 = ui.lineEdit_x2->text().toDouble();
    double R2 = ui.lineEdit_y2->text().toDouble();
    DATUMAXISTYPE axis = m_dlg->getAxis();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createTorus(wcsMatrix, x1, y1, z1, R1, R2, axis, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
