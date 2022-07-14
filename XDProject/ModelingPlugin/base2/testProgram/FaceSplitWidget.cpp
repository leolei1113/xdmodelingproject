#include "FaceSplitWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

FaceSplitWidget::FaceSplitWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

FaceSplitWidget::~FaceSplitWidget()
{
}

bool FaceSplitWidget::doModelOperation(float* wcsMatrix, const QString& name) 
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    std::list<void*> entityList;
	if (PIDs.size() < 2)
	{
		return false;
	}

	void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
    for (int i = 1; i < PIDs.size(); ++i)
    {
        entityList.push_back(TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[i].toStdString()));
    }

    std::string strPID;
    void* entity = nullptr;
    bool keep = ui.checkBox->isChecked(); 
    OccModelBuilder builder;
    if (builder.sliceByFaces(origEntity, entityList, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
