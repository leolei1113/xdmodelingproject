#pragma once

#include <QWidget>
#include "ui_ShapeFixWidget.h"
#include "ElemWidgetFactory.h"
#include "xdModelingDataType.h"

class ShapeFixWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    ShapeFixWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~ShapeFixWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

	std::string getMessage() const { return m_message; }
	void setMessage(std::string val) { m_message = val; }
private:
    Ui::ShapeFixWidget ui;

    std::string            m_message;
};
