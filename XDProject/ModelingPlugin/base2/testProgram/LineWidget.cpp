#include "LineWidget.h"
#include "OccModelBuilder.h"

LineWidget::LineWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

LineWidget::~LineWidget()
{
}

bool LineWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createUnusedEntity(name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}
