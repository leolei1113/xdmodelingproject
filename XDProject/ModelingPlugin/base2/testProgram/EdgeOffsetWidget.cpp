#include "EdgeOffsetWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

EdgeOffsetWidget::EdgeOffsetWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

EdgeOffsetWidget::~EdgeOffsetWidget()   
{
}

bool EdgeOffsetWidget::doModelOperation(float* wcsMatrix, const QString& name)
{ 
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");      
    if (PIDs.size() < 1)  
    {
        return false;
    }  
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
     
    double distance = ui.lineEdit_x2->text().toDouble();
     
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder; 
    if (builder.wireOffset(origEntity, distance, name.toStdString().c_str(), strPID, entity))  
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
