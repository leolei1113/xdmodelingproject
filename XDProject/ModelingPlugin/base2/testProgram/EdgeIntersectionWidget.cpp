#include "EdgeIntersectionWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

EdgeIntersectionWidget::EdgeIntersectionWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

EdgeIntersectionWidget::~EdgeIntersectionWidget()
{
}

bool EdgeIntersectionWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
	QString sourceStr = ui.lineEdit_PIDs->text();
	QStringList PIDs = sourceStr.split(",");
	if (PIDs.size() != 2)
	{
		return false;
	} 

	void* entry1 = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
	void* entry2 = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[1].toStdString());

	std::string strPID;   
	void* entity = nullptr;  
	OccModelBuilder builder;  
	if (builder.edgeIntersection(entry1, entry2, name.toStdString().c_str(), strPID, entity))
	{ 
		m_newNames << name;
		m_newPIDs << QString::fromStdString(strPID);
		m_newEntities << entity;
		return true;
	}
	return false;
}
