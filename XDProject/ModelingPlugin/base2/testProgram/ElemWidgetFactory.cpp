#include "ElemWidgetFactory.h"
#include "DrawModelDlg.h"
#include "LineWidget.h"
#include "CuboidWidget.h"
#include "CylinderWidget.h"
#include "SphereWidget.h"
#include "ConeWidget.h"
#include "BooleanWidget.h"
#include "xdModelingDataType.h"
#include "PointWidget.h"
#include "MultiLinesWidget.h"
#include "InterPSplineWidget.h"
#include "HyperbolaWidget.h"
#include "ParabolaWidget.h"
#include "EllipseWidget.h"
#include "SpiralWidget.h"
#include "HelixWidget.h"
#include "RectangularWidget.h"
#include "RegPolyhedronWidget.h"
#include "TorusWidget.h"
#include "PrismWidget.h"
#include "FlareWidget.h"
#include "HelixSolidWidget.h"
#include "ExtrudeWidget.h"
#include "RevolWidget.h"
#include "SweepWidget.h"
#include "ThickenWidget.h"
#include "FilletWidget.h"
#include "ChamferWidget.h"
#include "TranslateWidget.h"
#include "RotateWidget.h"
#include "MirrorWidget.h"
#include "ScaleWidget.h"
#include "LoftWidget.h"
#include "EdgeOffsetWidget.h"
#include "EdgeExtendWidget.h"
#include "EdgeCombinationWidget.h"
#include "EdgeSplitWidget.h"
#include "EdgeIntersectionWidget.h"
#include "FaceSplitWidget.h"
#include "FaceIntersectionWidget.h"
#include "FaceOffsetWidget.h"
#include "FaceExtendWidget.h"
#include "MeasureWidget.h"
#include "NurbsFaceWidget.h"
#include "FaceStitchWidget.h"
#include "ShapeFixWidget.h"

ElemWidgetObject::ElemWidgetObject(DrawModelDlg *parent /*= Q_NULLPTR*/)
    :m_dlg(parent)
{
}

ElemWidgetObject::~ElemWidgetObject()
{
}

ElemWidgetObject* ElemWidgetFactory::createWidgetObject(DrawModelDlg* dlg, EnWidgetType type, QVariant set1)
{
    ElemWidgetObject* rst = nullptr;


    switch (type)
    {
    case EnPointWidget:
    {
        rst = new PointWidget(dlg);
        break;
    }
    case EnLineWidget:
    {
        rst = new LineWidget(dlg);
        break;
    }
    case EnMultiLinesWidget:
    {
        rst = new MultiLinesWidget(dlg);
        break;
    }
    case EnInterPSplineWidget:
    {
        rst = new InterPSplineWidget(dlg);
        break;
    }
    case EnHyperbolaWidget:
    {
        rst = new HyperbolaWidget(dlg);
        break;
    }
    case EnParabolaWidget:
    {
        rst = new ParabolaWidget(dlg);
        break;
    }
    case EnEllipseWidget:
    {
        rst = new EllipseWidget(dlg);
        break;
    }
    case EnSpiralWidget:
    {
        rst = new SpiralWidget(dlg);
        break;
    }
    case EnHelixWidget:
    {
        rst = new HelixWidget(dlg);
        break;
    }
    case EnRectWidget:
    {
        rst = new RectangularWidget(dlg);
        break;
    }
    case EnRegPolyhedronFaceWidget:
    {
        rst = new RegPolyhedronWidget(dlg);
        break;
    }
	case EnNurbsFaceWidget:
	{
		rst = new NurbsFaceWidget(dlg);
		break;
	}
    case EnCuboidWidget:
    {
        rst = new CuboidWidget(dlg);
        break;
    }
    case EnCylinderWidget:
    {
        rst = new CylinderWidget(dlg);
        break;
    }
    case EnConeWidget:
    {
        rst = new ConeWidget(dlg);
        break;
    }
    case EnSphereWidget:
    {
        rst = new SphereWidget(dlg);
        break;
    }
    case EnTorusWidget:
    {
        rst = new TorusWidget(dlg);
        break;
    }
    case EnPrismWidget:
    {
        rst = new PrismWidget(dlg);
        break;
    }
    case EnFlareWidget:
    {
        rst = new FlareWidget(dlg);
        break;
    }
    case EnHelixSolidWidget:
    {
        rst = new HelixSolidWidget(dlg);
        break;
    }
    case EnExtrudeWidget:
    {
        rst = new ExtrudeWidget(dlg);
        break;
    }
    case EnRevolWidget:
    {
        rst = new RevolWidget(dlg);
        break;
    }
    case EnSweepWidget:
    {
        rst = new SweepWidget(dlg);
        break;
    }
    case EnLoftWidget:
    {
        rst = new LoftWidget(dlg);
        break;
    }
    case EnThickenWidget:
    {
        rst = new ThickenWidget(dlg);
        break;
    }
    case EnFilletWidget:
    {
        rst = new FilletWidget(dlg);
        break;
    }
    case EnChamferWidget:
    {
        rst = new ChamferWidget(dlg);
        break;
    }
    case EnTranslateWidget:
    {
        rst = new TranslateWidget(dlg);
        break;
    }
    case EnRotateWidget:
    {
        rst = new RotateWidget(dlg);
        break;
    }
    case EnMirrorWidget:
    {
        rst = new MirrorWidget(dlg);
        break;
    }
    case EnScaleWidget:
    {
        rst = new ScaleWidget(dlg);
        break;
    }
    case EnFuseWidget:
	{
		rst = new BooleanWidget(Unite, dlg);
		break;
	}
	case EnCommonWidget:
	{
		rst = new BooleanWidget(Intersect, dlg);
		break;
	}
    case EnCutWidget:
    {
        rst = new BooleanWidget(Substract, dlg);
        break;
    }
	case EnEdgeSplitWidget:
	{
		rst = new EdgeSplitWidget(dlg);
		break;
	}
	case EnEdgeOffsetWidget:
	{
		rst = new EdgeOffsetWidget(dlg);
		break;
	}
	case EnEdgeIntersectionWidget:
	{
		rst = new EdgeIntersectionWidget(dlg);
		break;
	}
	case EnEdgeCombinationWidget:
	{
		rst = new EdgeCombinationWidget(dlg);
		break;
	}
	case EnEdgeExtendWidget:
	{
		rst = new EdgeExtendWidget(dlg);
		break;
	}
	case EnFaceOffsetWidget:
	{
		rst = new FaceOffsetWidget(dlg);
		break;
	}
	case EnFaceSplitWidget:
	{
		rst = new FaceSplitWidget(dlg);
		break;
	}
	case EnFaceIntersectionWidget:
	{
		rst = new FaceIntersectionWidget(dlg);
		break;
	}
	case EnFaceExtendWidget:
	{
		rst = new FaceExtendWidget(dlg);
		break;
	}
	case EnFaceStitchWidget:
	{
		rst = new FaceStitchWidget(dlg);
		break;
	}
	case EnMeasurementWidget:
	{
		rst = new MeasureWidget(dlg);
		break;
	}
	case EnFixWidget:
	{
		rst = new ShapeFixWidget(dlg);
		break;
	}
    default:
    {
//         rst = new DefaultWidget();
        break;
    }
    }

    return rst;
}