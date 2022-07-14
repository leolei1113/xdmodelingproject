#include "OccAlgo.h"
#include "OccApplication.h"
#include "OccModel.h"
#include "LabelUtilities.h"
#include "OccModelBuilder.h"
#include "ShapeUtilities.h"
#include "OCCBasicTools.h"
#include "VertexShapeBuilder.h"
#include "TransformShapeBuilder.h"

#include <list>

#include <TopoDS_HShape.hxx>
#include <TDF_Data.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <math_GaussSingleIntegration.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <TopExp_Explorer.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Surface.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>


OccAlgo* OccAlgo::Instance()
{
	static OccAlgo algo;
	return &algo;
}

bool OccAlgo::GetOwner(void* ent, void*& entOwner)
{
	entOwner = TheOCCApp.getCurrentModel()->GetOwnerEntity(ent);
	return entOwner ? true : false;
}

bool OccAlgo::GetType(void* ent, TOPOTYPE& type)
{
	type = InvalidTopo;
	if (!ent)
	{
		return false;
	}

	void* owner;
	if (!GetOwner(ent, owner))
	{
		return false;
	}

	if (ent == owner)
	{
		type = TopoBody;
		return true;
	}

	TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(ent);
	if (!hshape)
	{
		return false;
	}

	TopAbs_ShapeEnum shapeType = hshape->Shape().ShapeType();
	if (shapeType == TopAbs_VERTEX)
	{
		type = TopoVertex;
	}
	else if (shapeType == TopAbs_EDGE)
	{
		type = TopoEdge;
	}
	else if (shapeType == TopAbs_FACE)
	{
		type = TopoFace;
	}
	else
	{
		return false;
	}

	return true;
}

bool OccAlgo::GetEntities(void* ent, TOPOTYPE type, std::vector<void*>& entitylist)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(ent);
		if (!hshape)
		{
			return false;
		}

		Handle(TDF_Data) data = TheOCCApp.getCurrentModel()->getDoc()->GetData();
		TDF_Label fatherLabel;
		if (!LabelUtilities::findShapeLabelFromRoot(data->Root(), hshape->Shape(), fatherLabel))
		{
			return false;
		}

		TopAbs_ShapeEnum shapetype;
		switch (type)
		{
		case InvalidTopo:
			break;
		case TopoVertex:
		{
			shapetype = TopAbs_VERTEX;
			break;
		}
		case TopoEdge:
		{
			shapetype = TopAbs_EDGE;
			break;
		}
		case TopoFace:
		{
			shapetype = TopAbs_FACE;
			break;
		}
// 		case TopoBody:
// 			break;
// 		case TopoLoop:
// 			break;
// 		case TopoCoedge:
// 			break;
		case TopoWire:
		{
			shapetype = TopAbs_WIRE;
			break;
		}
		default:
			return false;
// 			break;
		}
		std::vector<std::string> strPIDs;
		if (!LabelUtilities::findSubShapePIDsFromRoot(fatherLabel, shapetype, strPIDs))
		{
			return false;
		}

		for (int i = 0; i < strPIDs.size(); ++i)
		{
			void* foundEntry = TheOCCApp.getCurrentModel()->findEntityFromCacheMap(strPIDs[i]);
			entitylist.push_back(foundEntry);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::GetFaces(void* ent, std::vector<void*>& facelist)
{
	return GetEntities(ent, TopoFace, facelist);
}

bool OccAlgo::GetEdges(void* ent, std::vector<void*>& edgelist)
{
	return GetEntities(ent, TopoEdge, edgelist);
}

bool OccAlgo::GetWires(void* ent, std::vector<void*>& wirelist)
{
	return GetEntities(ent, TopoWire, wirelist);
}

bool OccAlgo::GetCoedges(void* ent, std::vector<void*>& coEdgelist)
{
	return false;
}

bool OccAlgo::GetVertices(void* ent, std::vector<void*>& vtxlist)
{
	return GetEntities(ent, TopoVertex, vtxlist);
}

bool OccAlgo::GetBox(void* ent, const cmtCS* wcs, cmtBox& box)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(ent);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		double theXmin, theYmin, theZmin, theXmax, theYmax, theZmax;
		if (!OCCBasicTools::GetBndBoxParams(shape, theXmin, theYmin, theZmin, theXmax, theYmax, theZmax))
		{
			return false;
		}

		if (wcs)
		{
			double wcsMatrix[16] = {
				wcs->XAxis().x(),wcs->XAxis().y(),wcs->XAxis().z(),0,
				wcs->YAxis().x(),wcs->YAxis().y(),wcs->YAxis().z(),0,
				wcs->ZAxis().x(),wcs->ZAxis().y(),wcs->ZAxis().z(),0,
				wcs->Origin().x(),wcs->Origin().y(),wcs->Origin().z(),1
			};

			double globalXmin, globalYmin, globalZmin, globalXmax, globalYmax, globalZmax;
			ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, theXmin, theYmin, theZmin, globalXmin, globalYmin, globalZmin);
			ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, theXmax, theYmax, theZmax, globalXmax, globalYmax, globalZmax);

			box.Set(cmtPoint(globalXmin, globalYmin, globalZmin), cmtPoint(globalXmax, globalYmax, globalZmax));
		}
		else
		{
			box.Set(cmtPoint(theXmin, theYmin, theZmin), cmtPoint(theXmax, theYmax, theZmax));
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::GetBox(const std::vector<void*>& entlist, const cmtCS* wcs, cmtBox& box)
{
	try
	{
		TopTools_ListOfShape shapes;
		for (int i = 0; i < entlist.size(); ++i)
		{
			TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entlist[i]);
			if (!hshape)
			{
				return false;
			}
			TopoDS_Shape shape = hshape->Shape();
			shapes.Append(shape);
		}

		double theXmin, theYmin, theZmin, theXmax, theYmax, theZmax;
		if (!OCCBasicTools::GetBndBoxParams(shapes, theXmin, theYmin, theZmin, theXmax, theYmax, theZmax))
		{
			return false;
		}

		if (wcs)
		{
			double wcsMatrix[16] = {
				wcs->XAxis().x(),wcs->XAxis().y(),wcs->XAxis().z(),0,
				wcs->YAxis().x(),wcs->YAxis().y(),wcs->YAxis().z(),0,
				wcs->ZAxis().x(),wcs->ZAxis().y(),wcs->ZAxis().z(),0,
				wcs->Origin().x(),wcs->Origin().y(),wcs->Origin().z(),1
			};

			double globalXmin, globalYmin, globalZmin, globalXmax, globalYmax, globalZmax;
			ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, theXmin, theYmin, theZmin, globalXmin, globalYmin, globalZmin);
			ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, theXmax, theYmax, theZmax, globalXmax, globalYmax, globalZmax);

			box.Set(cmtPoint(globalXmin, globalYmin, globalZmin), cmtPoint(globalXmax, globalYmax, globalZmax));
		}
		else
		{
			box.Set(cmtPoint(theXmin, theYmin, theZmin), cmtPoint(theXmax, theYmax, theZmax));
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue)
{
	return TheOCCApp.getCurrentModel()->GetEntityAttribute(entity, attrName, attrValue);
}

bool OccAlgo::GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue)
{
	return TheOCCApp.getCurrentModel()->GetEntityAttribute(entity, attrName, attrValue);
}

bool OccAlgo::AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue, bool bNeedCopy /*= false*/)
{
	return TheOCCApp.getCurrentModel()->AddEntityAttribute(entity, attrName, attrValue, bNeedCopy);
}

bool OccAlgo::AddEntityAttribute(void* entity, const std::string &attrName, const int attrValue, bool bNeedCopy /*= false*/)
{
	return TheOCCApp.getCurrentModel()->AddEntityAttribute(entity, attrName, attrValue, bNeedCopy);
}

bool OccAlgo::HasEntityAttributeValue(void* entity, const std::string &attrName, const int attrValue)
{
	return TheOCCApp.getCurrentModel()->HasEntityAttributeValue(entity, attrName, attrValue);
}

bool OccAlgo::RemoveEntityAttribute(void* entity, const std::string &attrName)
{
	return TheOCCApp.getCurrentModel()->RemoveEntityAttribute(entity, attrName);
}

bool OccAlgo::EdgeIsDegenerated(void* entEdge)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}
		return BRep_Tool::IsClosed(TopoDS::Edge(shape));

// 		double length;
// 		if (!EdgeLength(entEdge, length))
// 		{
// 			return true;
// 		}
// 		else
// 		{
// 			if (length <= Precision::Confusion())
// 			{
// 				return true;
// 			}
// 			else
// 			{
// 				return false;
// 			}
// 		}
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeIsClosed(void* entEdge)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		ShapeAnalysis_Edge analysis;
		return analysis.IsClosed3d(TopoDS::Edge(shape));
// 		double start, end;
// 		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);
// 
// 		return curve->IsClosed();
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeLength(void* entEdge, double &edgSegLen)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);
		GeomAdaptor_Curve result(curve, start, end);

		return GCPnts_AbscissaPoint::Length(result);
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeLength(void* entEdge, double start, double end, double &edgSegLen)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double realStart, realEnd;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), realStart, realEnd);
		GeomAdaptor_Curve result(curve, start, end);

		return GCPnts_AbscissaPoint::Length(result);
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeValue(void* entEdge, double t, cmtPoint &pt)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		gp_Pnt pnt;
		curve->D0(t, pnt);
		pt.Set(pnt.X(), pnt.Y(), pnt.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeValue(void* entEdge, void* entFace, int edgeSense, double t, double& u, double& v)
{
	try
	{
		cmtPoint pt;
		if (!EdgeValue(entEdge, t, pt))
		{
			return false;
		}

		if (!CheckPntOnFace(entFace, pt, u, v))
		{
			return false;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeProject(void* entEdge, const cmtPoint &pt, double tol, cmtPoint &cloestPt, double&t)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);
		gp_Pnt origPnt(pt.x(), pt.y(), pt.z());
		GeomAPI_ProjectPointOnCurve proj(origPnt, curve);
		gp_Pnt nearestPnt = proj.NearestPoint();
		cloestPt.Set(nearestPnt.X(), nearestPnt.Y(), nearestPnt.Z());

		for (int i = 1; i = proj.NbPoints(); ++i)
		{
			gp_Pnt curPnt = proj.Point(i);
			if (curPnt.IsEqual(nearestPnt, tol))
			{
				proj.Parameter(i, t);
				return true;
			}
		}

		return false;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeStartVertex(void* entEdge, void *& entVtx)
{
	try
	{
		void* ent = nullptr;
		std::vector<void*> vtxlist;

		if (!GetVertices(ent, vtxlist))
		{
			return false;
		}

		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		TopoDS_Shape startVertex;
		TopExp_Explorer exp(shape, TopAbs_VERTEX);
		for (; exp.More(); exp.Next())
		{
			startVertex = exp.Value();
			break;
		}

		for (int i = 0; i < vtxlist.size(); ++i)
		{
			TopoDS_HShape* hvertex = static_cast<TopoDS_HShape*>(vtxlist[i]);
			if (!hvertex)
			{
				return false;
			}
			TopoDS_Shape vertex = hvertex->Shape();
			if (vertex.IsEqual(startVertex))
			{
				entVtx = vtxlist[i];
				return true;
			}
		}

		return false;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeEndVertex(void* entEdge, void * &entVtx)
{
	try
	{
		void* ent = nullptr;
		std::vector<void*> vtxlist;

		if (!GetVertices(ent, vtxlist))
		{
			return false;
		}

		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		TopoDS_Shape endVertex;
		TopExp_Explorer exp(shape, TopAbs_VERTEX);
		for (; exp.More(); exp.Next())
		{
			endVertex = exp.Value();
		}

		for (int i = 0; i < vtxlist.size(); ++i)
		{
			TopoDS_HShape* hvertex = static_cast<TopoDS_HShape*>(vtxlist[i]);
			if (!hvertex)
			{
				return false;
			}
			TopoDS_Shape vertex = hvertex->Shape();
			if (vertex.IsEqual(endVertex))
			{
				entVtx = vtxlist[i];
				return true;
			}
		}

		return false;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeParamRange(void* entEdge, double &fstT, double &lstT)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), fstT, lstT);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeUniformlyArcLengthParam(void* entEdge, double startParam, double arcLength, double &endParam)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);
		Handle(Geom_Circle) circleCurve = Handle(Geom_Circle)::DownCast(curve);
		if (!circleCurve)
		{
			return false;
		}

		double theta = arcLength/circleCurve->Radius();
		endParam = startParam + theta / 2 / M_PI;
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeMiddlePoint(void* entEdge, cmtPoint &pt)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		gp_Pnt pnt;
		curve->D0((start+end)*0.5, pnt);
		pt.Set(pnt.X(), pnt.Y(), pnt.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeStartDir(void* entEdge, cmtVector &dir)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		gp_Pnt pnt;
		gp_Vec vec;
		curve->D1(start, pnt, vec);
		dir.Set(vec.X(), vec.Y(), vec.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeEndDir(void* entEdge, cmtVector &dir)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		gp_Pnt pnt;
		gp_Vec vec;
		curve->D1(end, pnt, vec);
		dir.Set(vec.X(), vec.Y(), vec.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeMiddleDir(void* entEdge, cmtVector &dir)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		gp_Pnt pnt;
		gp_Vec vec;
		curve->D1((start+end)*0.5, pnt, vec);
		dir.Set(vec.X(), vec.Y(), vec.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeTangent(void* entEdge, double t, cmtPoint &pt, cmtVector &dir)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		gp_Pnt pnt;
		gp_Vec vec;
		curve->D1(t, pnt, vec);
		pt.Set(pnt.X(), pnt.Y(), pnt.Z());
		dir.Set(vec.X(), vec.Y(), vec.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeCurvature(void* entEdge, double t, cmtPoint &pt, cmtVector &dir)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double curvature;
		gp_Pnt centerOfCurvature;
		OCCBasicTools::calcCurveCurvature(shape, t, curvature, centerOfCurvature);

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		gp_Pnt pnt;
		curve->D0(t, pnt);
		pt.Set(pnt.X(), pnt.Y(), pnt.Z());
		gp_Vec vec1(pnt, centerOfCurvature);

		dir.Set(vec1.X(), vec1.Y(), vec1.Z());
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeDiscretizeUniformly(void* entEdge, const UINT segNum, std::vector<double>& paraList)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		double start, end;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), start, end);

		double diff = (end - start) / (double)segNum;
		paraList.push_back(start);
		for (int i = 1; i <= segNum; ++i)
		{
			paraList.push_back(start + i * diff);
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeSense(void* entEdge, int &coedgSense)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		if (!shape.Orientable())
		{
			return false;
		}

		if (shape.Orientation() == TopAbs_FORWARD)
		{
			coedgSense = 0;
		}
		else if (shape.Orientation() == TopAbs_REVERSED)
		{
			coedgSense = 1;
		}
		else
		{
			return false;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::EdgeExtendLength(void* entEdge, double startParam, double len, void* &extEdg)
{
	try
	{
		/*OccModelBuilder builder;
		std::string pid;
		return builder.edgeExtendLength(entEdge, startParam, len, nullptr, pid, extEdg);*/
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool OccAlgo::EdgeCreateFromPoints(const cmtPoint& fstPt, const cmtPoint& lstPt, void* &resEdg)
{
	try
	{
		double wcsMatrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
		std::vector<std::vector<double>> points;
		std::vector<double> p1 = { fstPt.x(), fstPt.y(),fstPt.z() };
		std::vector<double> p2 = { lstPt.x(), lstPt.y(),lstPt.z() };
		points.push_back(p1);
		points.push_back(p2);
		std::vector<PolylineSegment> segs;
		PolylineSegment seg;
		seg.Type = Line;
		seg.StartIndex = 0;
		seg.NoOfPoints = 2;
		segs.push_back(seg);

		/*OccModelBuilder builder;
		std::string pid;
		return builder.createPolyline(wcsMatrix, points, segs, LOGICAL::False, LOGICAL::False, nullptr, pid, resEdg);*/
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool OccAlgo::EdgeCreateFromParameters(void* entEdge, double startParam, double endParam, void* &extEdg)
{
	try
	{
		/*OccModelBuilder builder;
		std::string pid;
		return builder.trimCurve(entEdge, startParam, endParam, nullptr, pid, extEdg);*/
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::CircleParamFrom3Pnt(const cmtPoint& cmtPnt1, const cmtPoint& cmtPnt2, const cmtPoint& cmtPnt3, cmtPoint& centerPnt, double& radius)
{
	try
	{
		std::vector<std::vector<double>> points;
		std::vector<double> p1 = { cmtPnt1.x(), cmtPnt1.y(),cmtPnt1.z() };
		std::vector<double> p2 = { cmtPnt2.x(), cmtPnt2.y(),cmtPnt2.z() };
		std::vector<double> p3 = { cmtPnt3.x(), cmtPnt3.y(),cmtPnt3.z() };

		points.push_back(p1);
		points.push_back(p2);
		points.push_back(p3);

		double xcenter, ycenter, zcenter;
		ShapeUtilities::getRadius(points, xcenter, ycenter, zcenter, radius);
		centerPnt.Set(xcenter, ycenter, zcenter);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::ArcSamplePoints(const cmtPoint& center, const cmtVector& normal, const cmtPoint& start, double radian, int pointnum, std::vector<cmtPoint>& outputpnts)
{
	try
	{
		VertexShapeBuilder vertexBuilder;
		vertexBuilder.createVertex(start.x(), start.y(), start.z());
		TopoDS_Shape vertex = vertexBuilder.getShape();

		TransformShapeBuilder transBuilder;
		gp_Pnt rotatePnt;
		for (int i = 0; i < pointnum; ++i)
		{
			transBuilder.shapeRotate(vertex, center.x(), center.y(), center.z(), normal.x(), normal.y(), normal.z(), radian*180.0 / M_PI, false);
			rotatePnt = BRep_Tool::Pnt(TopoDS::Vertex(transBuilder.getShape()));
			cmtPoint pt(rotatePnt.X(), rotatePnt.Y(), rotatePnt.Z());
			outputpnts.push_back(pt);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceIsLeftHanded(void* entFace)
{
	return false;//face in occ follows right handed laws
}

bool OccAlgo::FaceIsOnFace(void* entFace, const cmtPoint& point)
{
	try
	{
		TopoDS_HShape* hshape1 = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape1)
		{
			return false;
		}
		TopoDS_Shape shape = hshape1->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		VertexShapeBuilder builder;
		builder.createVertex(point.x(), point.y(), point.z());
		TopoDS_Shape shape2 = builder.getShape();
		gp_Pnt pnt1, pnt2;
		double dist;
		bool success = OCCBasicTools::DistanceMeasurement(shape, shape2, dist, pnt1, pnt2);
		return dist < Precision::Confusion() ? true : false;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceIsOnFace(void* entFace, const cmtPoint& point, double u, double v)
{
	return FaceIsOnFace(entFace, point);
}

bool OccAlgo::CheckPntOnFace(void* entFace, const cmtPoint& point, double& u, double& v)
{
	try
	{
		TopoDS_HShape* hshape1 = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape1)
		{
			return false;
		}
		TopoDS_Shape shape = hshape1->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		VertexShapeBuilder builder;
		builder.createVertex(point.x(), point.y(), point.z());
		TopoDS_Shape shape2 = builder.getShape();
		gp_Pnt pnt1, pnt2;
		double dist;

		BRepExtrema_DistShapeShape measureTool;
		measureTool.LoadS1(shape);
		measureTool.LoadS2(shape2);
		measureTool.Perform();

		if (!measureTool.IsDone())
		{
			return false;
		}

		double minDis = measureTool.Value();

		pnt1 = measureTool.PointOnShape1(1);
		pnt2 = measureTool.PointOnShape2(1);
		bool onFace = minDis < Precision::Confusion() ? true : false;

		measureTool.ParOnFaceS1(1, u, v);
		return onFace;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceValue(void* entFace, double u, double v, cmtPoint &pt)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));
		gp_Pnt pnt;
		surface->D0(u, v, pnt);
		pt.Set(pnt.X(), pnt.Y(), pnt.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceProject(void* entFace, const cmtPoint& pt, cmtPoint& cloestpt, double tol, double& u, double& v)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));
		gp_Pnt origPnt(pt.x(),pt.y(), pt.z());
		GeomAPI_ProjectPointOnSurf proj(origPnt, surface, tol);
		gp_Pnt nearestPnt = proj.NearestPoint();
		cloestpt.Set(nearestPnt.X(), nearestPnt.Y(), nearestPnt.Z());

		for (int i = 1; i = proj.NbPoints(); ++i)
		{
			gp_Pnt curPnt = proj.Point(i);
			if (curPnt.IsEqual(nearestPnt, tol))
			{
				proj.Parameters(i, u, v);
				return true;
			}
		}

		return false;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceNormal(void* entFace, double u, double v, cmtVector& norm, bool isConsiderSense /*= true*/)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));

		gp_Pnt pnt;
		gp_Vec vec1u, vec1v;
		surface->D1(u, v, pnt, vec1u, vec1v);
		gp_Vec normal = vec1u.Crossed(vec1v);

		norm.Set(normal.X(), normal.Y(), normal.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceMiddleNormal(void* entFace, cmtVector& norm, bool isConsiderSense /*= true*/)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));

		double ustart, vstart, uend, vend;
		surface->Bounds(ustart, uend, vstart, vend);
		gp_Pnt pnt;
		gp_Vec vec1u, vec1v;
		surface->D1(0.5*(ustart+uend), 0.5*(vstart + vend), pnt, vec1u, vec1v);
		gp_Vec normal = vec1u.Crossed(vec1v);

		norm.Set(normal.X(), normal.Y(), normal.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceTangents(void* entFace, double u, double v, cmtPoint& pt, cmtVector& du, cmtVector& dv)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));

		gp_Pnt pnt;
		gp_Vec vec1u, vec1v;
		surface->D1(u, v, pnt, vec1u, vec1v);
		gp_Vec normal = vec1u.Crossed(vec1v);

		pt.Set(pnt.X(), pnt.Y(), pnt.Z());
		du.Set(vec1u.X(), vec1u.Y(), vec1u.Z());
		dv.Set(vec1v.X(), vec1v.Y(), vec1v.Z());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceIsUClosed(void* entFace)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));
		return surface->IsUClosed();
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceIsVClosed(void* entFace)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));
		return surface->IsVClosed();
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceURange(void* entFace, double &start, double &end)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));
		double vstart, vend;
		surface->Bounds(start, end, vstart, vend);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceVRange(void* entFace, double &start, double &end)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));
		
		double ustart, uend;
		surface->Bounds(ustart, uend, start, end);
		
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceUVRange(void* entFace, double &uStart, double &uEnd, double &vStart, double &vEnd)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));
		
		surface->Bounds(uStart, uEnd, vStart, vEnd);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceSense(void* entFace, int &facSense)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		if (!shape.Orientable())
		{
			return false;
		}

		if (shape.Orientation() == TopAbs_FORWARD)
		{
			facSense = 0;
		}
		else if (shape.Orientation() == TopAbs_REVERSED)
		{
			facSense = 1;
		}
		else
		{
			return false;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceArea(void* entFace, double &area)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entFace);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		Handle(Geom_Surface) surface = BRep_Tool::Surface(TopoDS::Face(shape));

		GProp_GProps gprops;
		BRepGProp::SurfaceProperties(shape, gprops);
		area = gprops.Mass();

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceCurvature(void* entEdge, const cmtPoint &pt, const cmtVector &nrm, double &ptCurRad)
{
	try
	{
		TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entEdge);
		if (!hshape)
		{
			return false;
		}
		TopoDS_Shape shape = hshape->Shape();

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		double u, v;
		if (!CheckPntOnFace(entEdge, pt, u, v))
		{
			return false;
		}

		gp_Pnt centerOfCurvature;
		OCCBasicTools::calcSurfCurvature(shape, u,v, ptCurRad);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceCreateFromTwoLines(void *edg1, void *edg2, void* &resFac)
{
	try
	{
		std::list<void*> edgeList;
		edgeList.push_back(edg1);
		edgeList.push_back(edg2);

		/*OccModelBuilder builder;
		std::string pid;
		return builder.closedEdgeFace(edgeList, nullptr, pid, resFac);*/
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::FaceCreateFromThreeLines(void *edg1, void *edg2, void *edg3, void* &resFac)
{return false;

}

bool OccAlgo::FaceCreateFromFourLines(void *edg1, void *edg2, void *edg3, void *edg4, void* &resFac)
{return false;

}

bool OccAlgo::CoedgeGetEdge(void* entCoedge, void* &entEdge)
{return false;

}

bool OccAlgo::CoedgeGetNextCoedge(void* entCoedge, void* &nextEntCoedge)
{return false;

}

bool OccAlgo::CoedgeSense(void* entCoedge, int &coedgSense)
{return false;

}

bool OccAlgo::LoopGetFirstCoedge(void* entLoop, void* &entCoedge)
{return false;

}

bool OccAlgo::LoopGetType(void* entLoop, int &type)
{return false;

}

bool OccAlgo::VertexCoordinates(void *entVtx, cmtPoint &pt)
{
	try
	{
		TopoDS_HShape* hshape1 = static_cast<TopoDS_HShape*>(entVtx);
		if (!hshape1)
		{
			return false;
		}
		gp_Pnt pnt = BRep_Tool::Pnt(TopoDS::Vertex(hshape1->Shape()));
		pt.Set(pnt.X(), pnt.Y(), pnt.Z());
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::PointToShapeDistance(const cmtPoint &pt, void *ent, double &dist, cmtPoint &closePt)
{
	try
	{
		TopoDS_HShape* hshape1 = static_cast<TopoDS_HShape*>(ent);
		if (!hshape1)
		{
			return false;
		}
		VertexShapeBuilder builder;
		builder.createVertex(pt.x(), pt.y(), pt.z());
		TopoDS_Shape shape2 = builder.getShape();
		gp_Pnt pnt1, pnt2;
		bool success = OCCBasicTools::DistanceMeasurement(hshape1->Shape(), shape2, dist, pnt1, pnt2);
		closePt.Set(pnt2.X(), pnt2.Y(), pnt2.Z());
		return success;
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::DeepCopyShape(void *ent, void *&cpEnt)
{
	try
	{
		/*OccModelBuilder builder;
		std::string pid;
		return builder.createCloneEntity(ent, nullptr, pid, cpEnt);*/
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::DeleteShapes(const std::vector<void*> &ownFacOrBdyLst)
{
	try
	{
		std::vector<void*> finalDeleteLst;
		for (int i = 0; i < ownFacOrBdyLst.size(); ++i)
		{
			if (!ownFacOrBdyLst[i])
			{
				return false;
			}

			void* owner;
			if (!GetOwner(ownFacOrBdyLst[i], owner))
			{
				return false;
			}
		}

		return TheOCCApp.getCurrentModel()->DeleteBodies(finalDeleteLst);
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::ShapeToShapeDistance(void *ent1, void *ent2, double &dist, cmtPoint &closePt1, cmtPoint &closePt2)
{
	try
	{
		TopoDS_HShape* hshape1 = static_cast<TopoDS_HShape*>(ent1);
		TopoDS_HShape* hshape2 = static_cast<TopoDS_HShape*>(ent2);
		if (!hshape1 && !hshape2)
		{
			return false;
		}

		gp_Pnt pnt1, pnt2;
		return OCCBasicTools::DistanceMeasurement(hshape1->Shape(), hshape2->Shape(), dist, pnt1, pnt2);
	}
	catch (...)
	{
		return false;
	}
}

bool OccAlgo::MultiThreadTurnOffHistory()
{return false;

}

bool OccAlgo::MultiThreadTurnOnHistory()
{return false;

}

bool OccAlgo::MultiThreadSafeRegionBegin()
{return false;

}

bool OccAlgo::MultiThreadSafeRegionEnd()
{return false;

}

bool OccAlgo::MultiThreadStartChildThread()
{return false;

}

bool OccAlgo::MultiThreadStopChildThread()
{return false;

}
