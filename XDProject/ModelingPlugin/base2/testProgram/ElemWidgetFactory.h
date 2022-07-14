#ifndef ELEMWIDGETFACTORY_H
#define ELEMWIDGETFACTORY_H


#include <map>
#include <QString>
#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QDockWidget>
#include <QToolButton>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>

#include <TopoDS_HShape.hxx>

class DrawModelDlg;
enum EnWidgetType
{
    EnNone,
    //Draw
    //vertex edge wire face
    EnPointWidget,
    EnLineWidget,
    EnMultiLinesWidget,
    EnInterPSplineWidget,
    EnHyperbolaWidget,
    EnParabolaWidget,
    EnEllipseWidget,
    EnSpiralWidget,
    EnHelixWidget,
    EnRectWidget,
    EnRegPolyhedronFaceWidget,
	EnNurbsFaceWidget,

    //solid
    EnCuboidWidget,
    EnSphereWidget,
    EnCylinderWidget,
    EnConeWidget,
    EnTorusWidget,
    EnPrismWidget,
    EnFlareWidget,
    EnHelixSolidWidget,


    //Model
    EnExtrudeWidget,
    EnRevolWidget,
    EnSweepWidget,
    EnLoftWidget,
    EnThickenWidget,
    EnFilletWidget,
    EnChamferWidget,

    EnTranslateWidget,
    EnRotateWidget,
    EnMirrorWidget,
    EnScaleWidget,

    EnFuseWidget,
    EnCommonWidget,
    EnCutWidget,

	EnEdgeOffsetWidget,
	EnEdgeExtendWidget,
	EnEdgeCombinationWidget,
	EnEdgeSplitWidget,
	EnEdgeIntersectionWidget,

	EnFaceSplitWidget,
	EnFaceIntersectionWidget,
	EnFaceOffsetWidget,
	EnFaceExtendWidget,
	EnFaceStitchWidget,

	EnFixWidget,

	EnMeasurementWidget,
};

class ElemWidgetObject : public QWidget
{
    Q_OBJECT

public:
    ElemWidgetObject(DrawModelDlg *parent = Q_NULLPTR);
    ~ElemWidgetObject();

    virtual bool doModelOperation(float* wcsMatrix, const QString& name) = 0;

    QStringList getNewNames() const { return m_newNames; }
    void setNewNames(QStringList val) { m_newNames = val; }
    QStringList getNewPIDs() const { return m_newPIDs; }
    void setNewPIDs(QStringList val) { m_newPIDs = val; }
    QVector<void *> getNewEntities() const { return m_newEntities; }
    void setNewEntities(QVector<void *> val) { m_newEntities = val; }
protected:
    QStringList                     m_newNames;
    QStringList                     m_newPIDs;
    QVector<void*>                  m_newEntities;

    DrawModelDlg*                   m_dlg;
    
};

class ElemWidgetFactory
{
public:
    static ElemWidgetObject* createWidgetObject(DrawModelDlg* dlg, EnWidgetType type, QVariant set1);
};

#endif
