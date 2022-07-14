#include "HyperbolaWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

HyperbolaWidget::HyperbolaWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

HyperbolaWidget::~HyperbolaWidget()
{
}

bool HyperbolaWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    std::vector<double> point;
    point.push_back(x1);
    point.push_back(y1);
    point.push_back(z1);


    double tstart = ui.lineEdit_x2->text().toDouble();
    double tend = ui.lineEdit_y2->text().toDouble();
    double majorR = ui.lineEdit_z2->text().toDouble();
    double minorR = ui.lineEdit_minorR->text().toDouble();

    DATUMAXISTYPE axis = m_dlg->getAxis();
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createHyperbola(wcsMatrix, point, tstart, tend, majorR, minorR, axis, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
