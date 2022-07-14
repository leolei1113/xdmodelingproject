#include "MeasureWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"
#include "OccAlgo.h"
#include "LabelUtilities.h"
#include "OCCBasicTools.h"
#include "ShapeUtilities.h"

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TDocStd_Document.hxx>
#include <TNaming_Builder.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_AsciiString.hxx>


#include <QMessageBox>
MeasureWidget::MeasureWidget(DrawModelDlg *parent) 
    : ElemWidgetObject(parent) 
{
    ui.setupUi(this);
}

MeasureWidget::~MeasureWidget()  
{
}

bool MeasureWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
	TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
	Handle(TDF_Data) data = curDoc->GetData();
	TDF_Label rootLabel = data->Root();

    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    std::list<void*> entityList;
	if (PIDs.size() < 2)
	{
		return false;
	}

	void* ent1 = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
	void* ent2 = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[1].toStdString());

    std::string strPID;
    void* entity = nullptr;
    bool distancem = ui.checkBox->isChecked();
	if (distancem)
	{
		double distance;
		TopoDS_HShape* hshape1 = static_cast<TopoDS_HShape*>(ent1);
		TopoDS_HShape* hshape2 = static_cast<TopoDS_HShape*>(ent2);
		if (!hshape1 && !hshape2)
		{
			return false;
		}

		gp_Pnt pnt1, pnt2;    
		if (!OCCBasicTools::DistanceMeasurement(hshape1->Shape(), hshape2->Shape(), distance, pnt1, pnt2))
		{
			return false;   
		}

		BRepBuilderAPI_MakeVertex m1(pnt1);
		m1.Build();
		BRepBuilderAPI_MakeVertex m2(pnt2);
		m2.Build();

		{
			// 根据布尔运算后的模型自动分配子节点到各个label
			TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
			TNaming_Builder topNamingBuilder(targetLabel);
			topNamingBuilder.Generated(m1.Shape());

			// some infos for return;
			TCollection_AsciiString anEntry;
			TColStd_ListOfInteger aTagList;
			TDF_Tool::Entry(targetLabel, anEntry);
			strPID = anEntry.ToCString();
			TDataStd_AsciiString::Set(targetLabel, "top_level");

			m_newPIDs << QString::fromStdString(strPID);
// 			m_newEntities << entity;
		}

		{
			// 根据布尔运算后的模型自动分配子节点到各个label
			TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
			TNaming_Builder topNamingBuilder(targetLabel);
			topNamingBuilder.Generated(m2.Shape());

			// some infos for return;
			TCollection_AsciiString anEntry;
			TColStd_ListOfInteger aTagList;
			TDF_Tool::Entry(targetLabel, anEntry);
			strPID = anEntry.ToCString();
			TDataStd_AsciiString::Set(targetLabel, "top_level");

			m_newPIDs << QString::fromStdString(strPID);
// 			m_newEntities << entity;
		}
		m_newNames << QString::number(distance, 'f', 2);


		return true;
	}
	else
	{
		double angle;
		TopoDS_HShape* hshape1 = static_cast<TopoDS_HShape*>(ent1);
		TopoDS_HShape* hshape2 = static_cast<TopoDS_HShape*>(ent2);
		if (!hshape1 && !hshape2)
		{
			return false;
		}

		TopoDS_Shape shape1 = hshape1->Shape();
		TopoDS_Shape shape2 = hshape2->Shape();
		if (shape1.ShapeType() == TopAbs_WIRE)
		{
			ShapeUtilities::shapeFilter(shape1, TopAbs_EDGE, shape1);
		}
		else if (shape1.ShapeType() == TopAbs_SHELL)
		{
			ShapeUtilities::shapeFilter(shape1, TopAbs_FACE, shape1);
		}

		if (shape2.ShapeType() == TopAbs_WIRE)
		{
			ShapeUtilities::shapeFilter(shape2, TopAbs_EDGE, shape2);
		}
		else if (shape2.ShapeType() == TopAbs_SHELL)
		{
			ShapeUtilities::shapeFilter(shape2, TopAbs_FACE, shape2);
		}

		if (!OCCBasicTools::AngleMeasurement(shape1, shape2, angle))
		{
			return false;
		}

		m_newNames << QString::number(angle*180.0/M_PI, 'f', 2);


		return true;
	}
    return false;
}
