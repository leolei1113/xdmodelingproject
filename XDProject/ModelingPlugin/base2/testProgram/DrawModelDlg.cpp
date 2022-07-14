#include "DrawModelDlg.h"
// #include "OccModelBuilder.h"

#include <string>

DrawModelDlg::DrawModelDlg(EnWidgetType type, QWidget *parent)
    : QDialog(parent), m_type(type), m_elemWidget(nullptr), m_wcsMatrix(nullptr)
{
    ui.setupUi(this);

    m_wcsMatrix = new float[16];
    m_wcsMatrix[0] = 1;
    m_wcsMatrix[1] = 0;
    m_wcsMatrix[2] = 0;
    m_wcsMatrix[3] = 0;

    m_wcsMatrix[4] = 0;
    m_wcsMatrix[5] = 0;
    m_wcsMatrix[6] = 1;
    m_wcsMatrix[7] = 0;

    m_wcsMatrix[8] = 0;
    m_wcsMatrix[9] = 0;
    m_wcsMatrix[10] = 1;
    m_wcsMatrix[11] = 0;

    m_wcsMatrix[12] = 1;
    m_wcsMatrix[13] = 1;
    m_wcsMatrix[14] = 1;
    m_wcsMatrix[15] = 1;

}

DrawModelDlg::~DrawModelDlg()
{
    if (m_wcsMatrix)
    {
        delete m_wcsMatrix;
    }
}

void DrawModelDlg::setPanel(QVariant set1)
{
    if (m_type == EnFuseWidget || m_type == EnCommonWidget || m_type == EnCutWidget)
    {
        ui.groupBox_matrix->setVisible(false);
    }

    m_elemWidget = ElemWidgetFactory::createWidgetObject(this, m_type, set1);
    if (m_elemWidget)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_elemWidget);
        ui.widget->setLayout(layout);

         
    }
}
 
bool DrawModelDlg::buildModel()
{
    if (m_elemWidget)
    {
        updateWCSMatrix();
        QString name = ui.lineEdit_name->text();
        return m_elemWidget->doModelOperation(m_wcsMatrix, name);   
    }
    else
    {
        return false;
    }
}

LOGICAL DrawModelDlg::getCovered()
{
    bool covered = ui.checkBox_covered->isChecked();
    return covered ? True : False;
}

bool DrawModelDlg::getSolid()
{
    bool solid = ui.checkBox_solid->isChecked();
    return solid;
}

DATUMAXISTYPE DrawModelDlg::getAxis()
{
    if (ui.radioButton_XAxis->isChecked())
    {
        return X;
    }
    else if (ui.radioButton_YAxis->isChecked())
    {
        return Y;
    }
    else if (ui.radioButton_ZAxis->isChecked())
    {
        return Z;
    }

    return Z;
}

void DrawModelDlg::on_toolButton_ok_clicked()
{
    emit startCreate(this);

    accept();
}

void DrawModelDlg::updateWCSMatrix()
{
    m_wcsMatrix[0] = ui.lineEdit_1->text().toDouble();
    m_wcsMatrix[1] = ui.lineEdit_2->text().toDouble();
    m_wcsMatrix[2] = ui.lineEdit_3->text().toDouble();
    m_wcsMatrix[3] = ui.lineEdit_4->text().toDouble();

    m_wcsMatrix[4] = ui.lineEdit_5->text().toDouble();
    m_wcsMatrix[5] = ui.lineEdit_6->text().toDouble();
    m_wcsMatrix[6] = ui.lineEdit_7->text().toDouble();
    m_wcsMatrix[7] = ui.lineEdit_8->text().toDouble();

    m_wcsMatrix[8] = ui.lineEdit_9->text().toDouble();
    m_wcsMatrix[9] = ui.lineEdit_10->text().toDouble();
    m_wcsMatrix[10] = ui.lineEdit_11->text().toDouble();
    m_wcsMatrix[11] = ui.lineEdit_12->text().toDouble();

    m_wcsMatrix[12] = ui.lineEdit_13->text().toDouble();
    m_wcsMatrix[13] = ui.lineEdit_14->text().toDouble();
    m_wcsMatrix[14] = ui.lineEdit_15->text().toDouble();
    m_wcsMatrix[15] = ui.lineEdit_16->text().toDouble();
}
