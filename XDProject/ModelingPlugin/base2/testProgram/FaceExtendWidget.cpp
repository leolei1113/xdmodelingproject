#include "FaceExtendWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

FaceExtendWidget::FaceExtendWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

FaceExtendWidget::~FaceExtendWidget()   
{
}

bool FaceExtendWidget::doModelOperation(float* wcsMatrix, const QString& name)    
{ 
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");    
    if (PIDs.size() < 1)  
    {
        return false;
    }  
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
     
    double thickness = ui.lineEdit_x2->text().toDouble();
	bool inU = ui.checkBox_2->isChecked();
	bool forward = ui.checkBox->isChecked();
     
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.faceExtendLength(origEntity, thickness, inU, forward, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
     