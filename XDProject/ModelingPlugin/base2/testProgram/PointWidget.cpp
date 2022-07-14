#include "PointWidget.h"
#include "OccModelBuilder.h"
#include "VertexShapeBuilder.h"
#include "OccApplication.h"
#include "OccModel.h"

#include <TopoDS_HShape.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TNaming_Builder.hxx>

PointWidget::PointWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

PointWidget::~PointWidget()
{
}

bool PointWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    double x1 = ui.lineEdit_x1->text().toDouble();
    double y1 = ui.lineEdit_y1->text().toDouble();
    double z1 = ui.lineEdit_z1->text().toDouble();


    VertexShapeBuilder builder; 
    if (builder.createVertex(x1, y1, z1))
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //create label
        TDF_Label targetLabel = TDF_TagSource::NewChild(rootLabel);
        TDF_Label propLabel = TDF_TagSource::NewChild(targetLabel);
        TDataStd_Name::Set(propLabel, "property");

        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        m_newPIDs << QString::fromStdString(anEntry.ToCString());

        //create child label
        TNaming_Builder topNamingBuilder(targetLabel);
        topNamingBuilder.Generated(builder.getShape());
        TDataStd_AsciiString::Set(targetLabel, "top_level");


        TopoDS_HShape* entity = new TopoDS_HShape(builder.getShape());

        m_newEntities << entity;
        return true;
    }
    return false;
}
