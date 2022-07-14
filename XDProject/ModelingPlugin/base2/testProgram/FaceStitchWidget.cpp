#include "FaceStitchWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

FaceStitchWidget::FaceStitchWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

FaceStitchWidget::~FaceStitchWidget()   
{
}

bool FaceStitchWidget::doModelOperation(float* wcsMatrix, const QString& name)
{ 
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");      
    if (PIDs.size() < 1)  
    {
        return false;
    }  
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
	std::list<void*> shapes;
	shapes.push_back(origEntity);
          
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder; 
    if (builder.stitch(shapes, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
