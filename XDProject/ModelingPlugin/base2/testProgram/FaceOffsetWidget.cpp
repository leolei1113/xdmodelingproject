#include "FaceOffsetWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

FaceOffsetWidget::FaceOffsetWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

FaceOffsetWidget::~FaceOffsetWidget()   
{
}

bool FaceOffsetWidget::doModelOperation(float* wcsMatrix, const QString& name)
{ 
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");    
    if (PIDs.size() < 1)  
    {
        return false;
    }  
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
     
    double distance = ui.lineEdit_x2->text().toDouble();
    bool moveSolidFace = ui.checkBox->isChecked();
     
    std::string strPID;  
    void* entity = nullptr;
    OccModelBuilder builder;
	if (moveSolidFace)
	{
		std::list<void*> listEntry;
		for (int i = 0; i < PIDs.size(); ++i)  
		{
			listEntry.push_back(TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[i].toStdString()));
		}
		if (builder.offsetFaces(listEntry, distance, name.toStdString().c_str(), strPID, entity))
		{
			m_newNames << name;
			m_newPIDs << QString::fromStdString(strPID);
			m_newEntities << entity;
			return true;
		}
	}
	else
	{
		if (builder.faceOffset(origEntity, distance, name.toStdString().c_str(), strPID, entity))
		{
			m_newNames << name;
			m_newPIDs << QString::fromStdString(strPID);
			m_newEntities << entity;
			return true;
		}
	}
    return false;
}
