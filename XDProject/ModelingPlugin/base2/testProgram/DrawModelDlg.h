#ifndef DRAWMODELDLG_H
#define DRAWMODELDLG_H

#include <QDialog>
#include "ui_DrawModelDlg.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"


class DrawModelDlg : public QDialog
{
    Q_OBJECT

public:
    DrawModelDlg(EnWidgetType type, QWidget *parent = Q_NULLPTR);
    ~DrawModelDlg();

    void setPanel(QVariant set1 = 0);

    bool buildModel();

    QStringList getNewNames() const { return m_elemWidget->getNewNames(); }
    QStringList getNewPIDs() const { return m_elemWidget->getNewPIDs(); }
    QVector<void *> getNewEntities() const { return m_elemWidget->getNewEntities(); }

    LOGICAL getCovered();
    bool getSolid();
    DATUMAXISTYPE getAxis();

	EnWidgetType getType() const { return m_type; }
	void setType(EnWidgetType val) { m_type = val; }
	ElemWidgetObject* getElemWidget() const { return m_elemWidget; }
	void setElemWidget(ElemWidgetObject* val) { m_elemWidget = val; }
signals:
    void startCreate(DrawModelDlg*);

protected slots:
    void on_toolButton_ok_clicked();

protected:
    void updateWCSMatrix();

protected:
    Ui::DrawModelDlg ui;

    EnWidgetType                    m_type;
    ElemWidgetObject*               m_elemWidget;

    float*                          m_wcsMatrix;
};

#endif
