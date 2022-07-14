#include "ThickenWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

ThickenWidget::ThickenWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

ThickenWidget::~ThickenWidget()   
{
}

bool ThickenWidget::doModelOperation(float* wcsMatrix, const QString& name)
{ 
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");    
    if (PIDs.size() < 1)  
    {
        return false;
    }  
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
     
    double thickness = ui.lineEdit_x2->text().toDouble();
    LOGICAL doubleSides = ui.checkBox->isChecked() ? True : False;
     
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.thicken(origEntity, thickness, doubleSides, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
