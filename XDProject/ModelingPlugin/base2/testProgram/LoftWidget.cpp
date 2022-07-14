#include "LoftWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"
#include "SweepShapeBuilder.h"
#include "LabelUtilities.h"

#include <TDF_Label.hxx>
#include <TDF_Data.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>
#include <TopTools_HArray1OfShape.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TNaming_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_ListOfShape.hxx>

LoftWidget::LoftWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

LoftWidget::~LoftWidget()
{
}

bool LoftWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
    Handle(TDF_Data) data = curDoc->GetData();
    TDF_Label rootLabel = data->Root();

	QString sourceStr = ui.lineEdit_PIDs->text();
	QStringList PIDs = sourceStr.split(",");
	if (PIDs.size() < 2)
	{
		return false;
	}
	std::list<void*> entryList;
	for (int i = 0; i < PIDs.size(); ++i)
	{
		void* entry = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[i].toStdString());
		entryList.push_back(entry);
	}

	std::string strPID;
	void* entity = nullptr;
	OccModelBuilder builder;
	if (builder.connect(entryList, name.toStdString().c_str(), strPID, entity))
	{
		m_newNames << name;
		m_newPIDs << QString::fromStdString(strPID);
		m_newEntities << entity;
		return true;
	}

	return false;
}
