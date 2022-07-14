#include "RectangularWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

RectangularWidget::RectangularWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

RectangularWidget::~RectangularWidget()
{
}

bool RectangularWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    double width = ui.lineEdit_x2->text().toDouble();
    double length = ui.lineEdit_y2->text().toDouble();
    DATUMAXISTYPE axis = m_dlg->getAxis();
    LOGICAL covered = m_dlg->getCovered();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createRectangle(wcsMatrix, x1, y1, z1, width, length, axis,covered,  name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
