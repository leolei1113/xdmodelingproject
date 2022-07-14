#include "FaceIntersectionWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

FaceIntersectionWidget::FaceIntersectionWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

FaceIntersectionWidget::~FaceIntersectionWidget()
{
}

bool FaceIntersectionWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
	QString sourceStr = ui.lineEdit_PIDs->text();
	QStringList PIDs = sourceStr.split(",");
	if (PIDs.size() < 2)
	{
		return false;
	}

	void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
	void* origEntity2 = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[1].toStdString());

	int method = 1;
	if (ui.radioButton->isChecked())
	{
		method = 1;
	}
	else if (ui.radioButton_2->isChecked())
	{
		method = 2;
	}
	else if (ui.radioButton_3->isChecked()) 
	{
		method = 3;
	}

	std::string strPID;
	void* entity = nullptr;
	bool keep = ui.checkBox->isChecked();
	OccModelBuilder builder;
	if (builder.faceIntersection(origEntity, origEntity2, method, name.toStdString().c_str(), strPID, entity))
	{
		m_newNames << name;
		m_newPIDs << QString::fromStdString(strPID);
		m_newEntities << entity;
		return true;
	}
	return false;
}
