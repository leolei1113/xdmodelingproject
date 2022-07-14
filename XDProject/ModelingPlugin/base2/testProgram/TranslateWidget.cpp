#include "TranslateWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

TranslateWidget::TranslateWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

TranslateWidget::~TranslateWidget()
{
}

bool TranslateWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    if (PIDs.size() < 1)
    {
        return false;
    }
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());

    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();
    int num = ui.spinBox->value();
    LOGICAL attach = ui.checkBox->isChecked() ? True : False;

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.translate(origEntity, wcsMatrix, x1, y1, z1, num, attach, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
