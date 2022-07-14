#include "ShapeFixWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"

ShapeFixWidget::ShapeFixWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

ShapeFixWidget::~ShapeFixWidget()
{
}

bool ShapeFixWidget::doModelOperation(float* wcsMatrix, const QString& name) 
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    std::list<void*> entityList;
	if (PIDs.size() < 1)
	{
		return false;
	}

	void* origEntity = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[0].toStdString());
	double precsion = ui.lineEdit_precision->text().toDouble();
	double minTol = ui.lineEdit_mintol->text().toDouble();
	double maxTol = ui.lineEdit_maxtol->text().toDouble();
	EnFixType fixType = EnFixType(ui.comboBox->currentIndex());

	m_message = "";
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.shapeFix(origEntity, precsion, minTol, maxTol, fixType, name.toStdString().c_str(), strPID, entity, m_message))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
