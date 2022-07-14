#include "NurbsFaceWidget.h"
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"

#include <vector>

using namespace std;

NurbsFaceWidget::NurbsFaceWidget(DrawModelDlg *parent)
    : ElemWidgetObject(parent) 
{
    ui.setupUi(this);
	ui.tableWidgetPoles->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidgetPoles->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//poles
	{
		ui.tableWidgetPoles->setRowCount(3);
		ui.tableWidgetPoles->setColumnCount(3);
		QTableWidgetItem * item1 = new QTableWidgetItem("(0,0,0,1)");
		ui.tableWidgetPoles->setItem(0, 0, item1);
		QTableWidgetItem * item2 = new QTableWidgetItem("(0,1,0,1)");
		ui.tableWidgetPoles->setItem(0, 1, item2);
		QTableWidgetItem * item3 = new QTableWidgetItem("(0,2,0,1)");
		ui.tableWidgetPoles->setItem(0, 2, item3);
		QTableWidgetItem * item4 = new QTableWidgetItem("(1,0,0,1)");
		ui.tableWidgetPoles->setItem(1, 0, item4);
		QTableWidgetItem * item5 = new QTableWidgetItem("(1,1,0,1)");
		ui.tableWidgetPoles->setItem(1, 1, item5);
		QTableWidgetItem * item6 = new QTableWidgetItem("(1,2,0,1)");
		ui.tableWidgetPoles->setItem(1, 2, item6);
		QTableWidgetItem * item7 = new QTableWidgetItem("(2,0,1,1)");
		ui.tableWidgetPoles->setItem(2, 0, item7);
		QTableWidgetItem * item8 = new QTableWidgetItem("(2,1,1,1)");
		ui.tableWidgetPoles->setItem(2, 1, item8);
		QTableWidgetItem * item9 = new QTableWidgetItem("(2,2,1,1)");
		ui.tableWidgetPoles->setItem(2, 2, item9);
	}
}

NurbsFaceWidget::~NurbsFaceWidget()
{
}

bool NurbsFaceWidget::doModelOperation(float* wcsMatrix, const QString& name)
{
	vector<vector<vector<double>>> poles;
	vector<vector<double>> weight;
	vector<double> uValue, vValue;
	vector<int>uMul, vMul;
	int uDeg, vDeg;

	int numRow = ui.tableWidgetPoles->rowCount();
	int numCol = ui.tableWidgetPoles->columnCount();
	for (int i = 0; i < numRow; ++i)
	{
		vector<vector<double>> hPoles;
		vector<double> hWeight;
		for (int j = 0; j < numCol; ++j)
		{
			QTableWidgetItem * item = ui.tableWidgetPoles->item(i, j);
			QString str = item->text();

			str = str.mid(str.indexOf('(')+1, str.indexOf(')')-1);
			QStringList values = str.split(',');
			vector<double> pnt;

			pnt.push_back(values[0].trimmed().toDouble());
			pnt.push_back(values[1].trimmed().toDouble());
			pnt.push_back(values[2].trimmed().toDouble());
			hWeight.push_back(values[3].trimmed().toDouble());

			hPoles.push_back(pnt);
		}
		poles.push_back(hPoles);
		weight.push_back(hWeight);
	}

	QString str = ui.lineEdit_uValue->text();
	QStringList values = str.split(',');
	for (int i = 0; i < values.size(); ++i)
	{
		uValue.push_back(values[i].trimmed().toDouble());
	}

	str = ui.lineEdit_vValue->text();
	values = str.split(',');
	for (int i = 0; i < values.size(); ++i)
	{
		vValue.push_back(values[i].trimmed().toDouble());
	}

	str = ui.lineEdit_uMul->text();
	values = str.split(',');
	for (int i = 0; i < values.size(); ++i)
	{
		uMul.push_back(values[i].trimmed().toDouble());
	}

	str = ui.lineEdit_vMul->text();
	values = str.split(',');
	for (int i = 0; i < values.size(); ++i)
	{
		vMul.push_back(values[i].trimmed().toDouble());
	}

	uDeg = ui.spinBox->value();
	vDeg = ui.spinBox_2->value();

	std::string strPID;
    void* entity = nullptr;
    OccModelBuilder builder;   
 	if (builder.createNurbsFace(poles, weight, uValue,vValue, uMul, vMul, uDeg, vDeg, name.toStdString().c_str(), strPID, entity))
	{
		m_newNames << name;
		m_newPIDs << QString::fromStdString(strPID);
		m_newEntities << entity;
		return true;
	}
	return false;

}

void NurbsFaceWidget::on_toolButton_hplus1_clicked()
{
	int numRow = ui.tableWidgetPoles->rowCount();
	int numCol = ui.tableWidgetPoles->columnCount();

	ui.tableWidgetPoles->insertColumn(numCol);
	for (int i = 0; i < numRow; ++i)
	{
		QTableWidgetItem * item = new QTableWidgetItem("(0,0,0,1)");
		ui.tableWidgetPoles->setItem(i, numCol, item);
	}
}

void NurbsFaceWidget::on_toolButton_vplus1_clicked()
{
	int numRow = ui.tableWidgetPoles->rowCount();
	int numCol = ui.tableWidgetPoles->columnCount();

	ui.tableWidgetPoles->insertRow(numRow);
	for (int i = 0; i < numCol; ++i)
	{
		QTableWidgetItem * item = new QTableWidgetItem("(0,0,0,1)");
		ui.tableWidgetPoles->setItem(numRow, i, item);
	}
}

void NurbsFaceWidget::on_toolButton_hminus1_clicked()
{
	int numRow = ui.tableWidgetPoles->rowCount();
	int numCol = ui.tableWidgetPoles->columnCount();
	ui.tableWidgetPoles->removeColumn(numCol-1);
}

void NurbsFaceWidget::on_toolButton_vminus1_clicked()
{
	int numRow = ui.tableWidgetPoles->rowCount();
	int numCol = ui.tableWidgetPoles->columnCount();
	ui.tableWidgetPoles->removeRow(numRow-1);
}