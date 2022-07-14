#include "MultiLinesWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

#include <QComboBox>
#include <QAbstractItemModel>

ComboboxTypeDelegate::ComboboxTypeDelegate(QObject* parent /*= nullptr*/) : QItemDelegate(parent)
{

}

ComboboxTypeDelegate::~ComboboxTypeDelegate()
{

}

QWidget* ComboboxTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    QComboBox* box = new QComboBox(parent);
    box->addItem("Polyline");
    box->addItem("BSpline");
    box->addItem("3PArc");
    box->addItem("CenterArc");

    return box;
}

void ComboboxTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const
{
    QComboBox* box = qobject_cast<QComboBox*>(editor);
    model->setData(index, box->currentText());
}

void ComboboxTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index)const
{
    QComboBox* box = qobject_cast<QComboBox*>(editor);
    box->setCurrentText(index.data(Qt::DisplayRole).toString());
}

void ComboboxTypeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    editor->setGeometry(option.rect);
}

#include <QComboBox>
#include <QAbstractItemModel>

ComboboxAxisDelegate::ComboboxAxisDelegate(QObject* parent /*= nullptr*/) : QItemDelegate(parent)
{

}

ComboboxAxisDelegate::~ComboboxAxisDelegate()
{

}

QWidget* ComboboxAxisDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    QComboBox* box = new QComboBox(parent);
    box->addItem("X");
    box->addItem("Y");
    box->addItem("Z");
    return box;
}

void ComboboxAxisDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const
{
    QComboBox* box = qobject_cast<QComboBox*>(editor);
    model->setData(index, box->currentText());
}

void ComboboxAxisDelegate::setEditorData(QWidget *editor, const QModelIndex &index)const
{
    QComboBox* box = qobject_cast<QComboBox*>(editor);
    box->setCurrentText(index.data(Qt::DisplayRole).toString());
}

void ComboboxAxisDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    editor->setGeometry(option.rect);
}

MultiLinesWidget::MultiLinesWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent)
{
    ui.setupUi(this);

    ui.tableWidget->setItemDelegateForColumn(3, new ComboboxTypeDelegate(this));
}

MultiLinesWidget::~MultiLinesWidget()
{
}

bool MultiLinesWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
    bool planar = ui.checkBox_planar->isChecked();

    std::vector<std::vector<double>> points;
    std::vector<PolylineSegment> segments;

    QString type = "";
    DATUMAXISTYPE axis = m_dlg->getAxis();
    int num = 0;
    int startIndex = 0;
    for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
    {
        double x1 = ui.tableWidget->item(i, 0)->text().toDouble();
        double y1 = ui.tableWidget->item(i, 1)->text().toDouble();
        double z1 = ui.tableWidget->item(i, 2)->text().toDouble();

        switch (axis)
        {
        case X:
        {
            x1 = 0;
            break;
        }
        case Y:
        {
            y1 = 0;
            break;
        }
        case Z:
        {
            z1 = 0;
            break;
        }
        default:
            break;
        }

        QString newtype = ui.tableWidget->item(i, 3)->text();
        double angle = ui.tableWidget->item(i, 4)->text().toDouble();

        std::vector<double> point;
        point.push_back(x1);
        point.push_back(y1);
        point.push_back(z1);
        points.push_back(point);

        if (i == 0)
        {
            type = newtype;
        }

        if (type != newtype|| i == ui.tableWidget->rowCount()-1)
        {
            PolylineSegment seg;
            seg.StartIndex = points.size();
            if (type == "Polyline")
            {
                seg.Type = MULTISEGMENTTYPE::Line;
            }
            else if (type == "BSpline")
            {
                seg.Type = MULTISEGMENTTYPE::Spline;
            }
            else if (type == "Interp Spline")
            {
                seg.Type = MULTISEGMENTTYPE::Spline;
            }
            else if (type == "3PArc")
            {
                seg.Type = MULTISEGMENTTYPE::ThreePointArc;
            }
            else if (type == "CenterArc")
            {
                seg.Type = MULTISEGMENTTYPE::AngularArc;
            } 

            seg.NoOfPoints = i - startIndex + 1;
            seg.StartIndex = startIndex;
            seg.ArcAngle = angle;
            seg.CenterAxis = axis;
            startIndex = i;
            segments.push_back(seg);

            type = newtype;
        }
    }

    bool closed = ui.checkBox_closed->isChecked();
    LOGICAL covered = m_dlg->getCovered();

    std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;
    if (builder.createPolyline(wcsMatrix, points, segments, closed? True:False, covered, name.toStdString().c_str(), strPID, entity))
    {
        m_newNames << name;
        m_newPIDs << QString::fromStdString(strPID);
        m_newEntities << entity;
        return true;
    }
    return false;
}

void MultiLinesWidget::on_toolButton_clicked()
{
    ui.tableWidget->setRowCount(ui.tableWidget->rowCount() + 1);
    int row = ui.tableWidget->rowCount() - 1;
    double d = 0;
    QString value = QString::number(d, 'f', 2);
    for (int i = 0; i < ui.tableWidget->columnCount(); ++i)
    {
        ui.tableWidget->setItem(row, i, new QTableWidgetItem(value));
    }
    ui.tableWidget->setItem(row, 3, new QTableWidgetItem("Polyline"));

}
