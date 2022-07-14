#pragma once

#include <QWidget>
#include <QItemDelegate>

#include "ui_MultiPointsWidget.h"
#include "ElemWidgetFactory.h"

class ComboboxTypeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ComboboxTypeDelegate(QObject* parent = nullptr);
    ~ComboboxTypeDelegate();
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class ComboboxAxisDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ComboboxAxisDelegate(QObject* parent = nullptr);
    ~ComboboxAxisDelegate();
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class MultiLinesWidget : public ElemWidgetObject
{
    Q_OBJECT

public:
    MultiLinesWidget(DrawModelDlg *parent = Q_NULLPTR);
    ~MultiLinesWidget();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name);

public slots:

    void on_toolButton_clicked();

private:
    Ui::MultiPointsWidget ui;
};
