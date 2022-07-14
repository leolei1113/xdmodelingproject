#include "FilletWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

FilletWidget::FilletWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

FilletWidget::~FilletWidget() 
{
}

bool FilletWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    if (PIDs.size() < 1)
    {
        return false; 
    }
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());

    sourceStr = ui.lineEdit_PIDs2->text();
    PIDs = sourceStr.split(",");
    if (PIDs.size() < 0)
    {
        return false;
    }
    std::list<std::string> entityList; 
    for (int i = 0; i < PIDs.size(); ++i)
    {
        entityList.push_back(PIDs[i].toStdString()); 
    }
     
    double radius = ui.lineEdit_x1->text().toDouble(); 
    double setback = ui.lineEdit_x2->text().toDouble();

    std::string strPID;  
    void* entity = nullptr;  
    OccModelBuilder builder;     
    if (builder.fillet(origEntity, entityList, radius, setback, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name; 
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
