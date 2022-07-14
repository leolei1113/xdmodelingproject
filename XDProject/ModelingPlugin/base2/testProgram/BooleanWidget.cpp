#include "BooleanWidget.h"
#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"
#include "EnumAndStruct.h"

BooleanWidget::BooleanWidget(BOOLTYPE boolType, DrawModelDlg *parent)
    : ElemWidgetObject(parent), m_boolType(boolType)
{
    ui.setupUi(this);
	ui.comboBox->setCurrentIndex(m_boolType);
}

BooleanWidget::~BooleanWidget() 
{
}
 
bool BooleanWidget::doModelOperation(float* wcsMatrix, const QString& name)    
{
    QString sourceStr = ui.lineEdit_PIDs->text();
    QStringList PIDs = sourceStr.split(",");
    std::list<void*> entityList;
    for (int i = 0; i < PIDs.size(); ++i) 
    { 
        entityList.push_back(TheOCCApp.getCurrentModel()->findEntityFromCacheMap(PIDs[i].toStdString()));
    }

    m_boolType =  BOOLTYPE(ui.comboBox->currentIndex());
	QString fuzzyValue = ui.lineEdit_fuzzy->text();
	bool nonreg = ui.checkBox_non_regular->isChecked();

    std::string strPID;
    void* entity = nullptr;
    bool keep = ui.checkBox->isChecked();
    OccModelBuilder builder;

	if (nonreg)
	{
		EnAdapatType type;
		if (m_boolType == Unite)
		{
			type = EnCellsBuilderFuse; 
		}
		else if (m_boolType == Substract)
		{
			type = EnCellsBuilderCut;
		}
		else
		{
			type = EnCellsBuilderCommon; 
		}
		if (builder.makeNonRegBoolean(type, entityList, name.toStdString().c_str(), strPID, entity))
		{
			m_newNames << name;
			m_newPIDs << QString::fromStdString(strPID);
			m_newEntities << entity;
			return true;
		}
	} 
	else if (!fuzzyValue.isEmpty()) 
	{
		if (builder.makeFuzzyBoolean(m_boolType, keep ? LOGICAL::True : LOGICAL::False, entityList, fuzzyValue.toDouble(), name.toStdString().c_str(), strPID, entity))
		{
			m_newNames << name;
			m_newPIDs << QString::fromStdString(strPID);
			m_newEntities << entity;
			return true;
		}
	}
	else
	{
		if (builder.makeBoolean(m_boolType, keep ? LOGICAL::True : LOGICAL::False, entityList, name.toStdString().c_str(), strPID, entity))
		{
			m_newNames << name;
			m_newPIDs << QString::fromStdString(strPID);
			m_newEntities << entity;
			return true;
		}
	}
    return false;
}
