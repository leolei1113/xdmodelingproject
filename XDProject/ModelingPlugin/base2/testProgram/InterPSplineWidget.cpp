#include "InterPSplineWidget.h"
#include "OccModelBuilder.h"
#include "EdgeShapeBuilder.h"
#include "DrawModelDlg.h"
#include "OccApplication.h"
#include "OccModel.h"

#include <QComboBox>
#include <QAbstractItemModel>
#include <TopoDS_HShape.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TNaming_Builder.hxx>

InterPSplineWidget::InterPSplineWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);
}

InterPSplineWidget::~InterPSplineWidget()
{
}

bool InterPSplineWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    std::vector<std::vector<double>> points;
    for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
    {
        double x1 = ui.tableWidget->item(i, 0)->text().toDouble();
        double y1 = ui.tableWidget->item(i, 1)->text().toDouble();
        double z1 = ui.tableWidget->item(i, 2)->text().toDouble();
        {
            std::vector<double> point;
            point.push_back(x1);
            point.push_back(y1);
            point.push_back(z1);
            points.push_back(point);

        }
    }

    std::string strPID;
    void* entity = nullptr;
    EdgeShapeBuilder builder;
    if (builder.createInterpolateBSpline(points))
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

void InterPSplineWidget::on_toolButton_clicked()
{
    ui.tableWidget->setRowCount(ui.tableWidget->rowCount() + 1);
    int row = ui.tableWidget->rowCount() - 1;
    double d = 0;
    QString value = QString::number(d, 'f', 2);
    for (int i = 0; i < ui.tableWidget->columnCount(); ++i)
    {
        ui.tableWidget->setItem(row, i, new QTableWidgetItem(value));
    }
}
