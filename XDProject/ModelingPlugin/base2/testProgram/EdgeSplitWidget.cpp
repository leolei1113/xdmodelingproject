#include "EdgeSplitWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"
#include "OccAlgo.h"
#include "ShapeUtilities.h"
 
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>

EdgeSplitWidget::EdgeSplitWidget(DrawModelDlg *parent)    
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

EdgeSplitWidget::~EdgeSplitWidget()      
{
}

bool EdgeSplitWidget::doModelOperation(float* wcsMatrix, const QString& name)
{ 
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");    
    if (PIDs.size() < 1)  
    {
        return false;
    }  
    void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
     
	on_toolButton_update_clicked();

	double param = ui.lineEdit_param->text().toDouble();
	double start = ui.lineEdit_pstart->text().toDouble();
	double end = ui.lineEdit_pend->text().toDouble();

    std::string strPID;
    void* entity = nullptr;  
    OccModelBuilder builder;   
    if (builder.trimCurve(origEntity, start, param, name.toStdString().c_str(), strPID, entity))  
    {
		m_newNames << name;
		m_newPIDs << QString::fromStdString(strPID); 
		m_newEntities << entity; 

		if (builder.trimCurve(origEntity, param, end, name.toStdString().c_str(), strPID, entity))
		{
			m_newNames << name;
			m_newPIDs << QString::fromStdString(strPID);
			m_newEntities << entity;
			return true;
		}
    }
    return false;
}

void EdgeSplitWidget::on_toolButton_update_clicked()
{
	QString sourceStr = ui.lineEdit_PIDs->text();
	QStringList PIDs = sourceStr.split(",");
	if (PIDs.size() < 1)
	{
		return;
	}
	void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());

	TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(origEntity);
	if (!hshape)
	{
		return;
	}
	TopoDS_Shape shape = hshape->Shape();
	ShapeUtilities::shapeFilter(shape, TopAbs_EDGE, shape);

	double start, end;
	Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);
	ui.lineEdit_pstart->setText(QString::number(start, 'f', 5));
	ui.lineEdit_pend->setText(QString::number(end, 'f', 5));
}
