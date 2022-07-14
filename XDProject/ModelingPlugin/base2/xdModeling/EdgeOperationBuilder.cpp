#include "EdgeOperationBuilder.h"
#include "ShapeUtilities.h"
#include "VertexShapeBuilder.h"

#include <algorithm>

#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>
#include <ChFi2d_FilletAPI.hxx>
#include <BRepBuilderAPI_FindPlane.hxx>
#include <ChFi2d_Builder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <ChFi2d.hxx>
#include <TopExp.hxx>
#include <ChFi2d_ChamferAPI.hxx>
#include <BRep_Tool.hxx>
#include <Geom_BoundedCurve.hxx>
#include <GeomLib.hxx>
#include <GeomAPI_IntCS.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <BOPAlgo_Builder.hxx>
#include <GeomConvert_ApproxCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_Line.hxx>
#include <Geom_OffsetCurve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <BRep_Builder.hxx>
#include <BRepExtrema_DistanceSS.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepAlgoAPI_Section.hxx>


EdgeOperationBuilder::EdgeOperationBuilder()
{
}


EdgeOperationBuilder::~EdgeOperationBuilder()
{
}

bool EdgeOperationBuilder::trimCurve(const TopoDS_Shape& origEdge, const double startU, const double endU)
{
	try
	{
		double finalStart, finalEnd;
		double realStart, realEnd;
		// 		BRepAdaptor_Curve adaptor(TopoDS::Edge((origEdge)));
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(origEdge), realStart, realEnd);

		finalStart = startU;
		finalEnd = endU;
		if (abs(startU - realStart) < 0.001)
		{
			finalStart = realStart;
		}
		if (abs(endU - realEnd) < 0.001)
		{
			finalEnd = realEnd;
		}

		if (finalStart<realStart || finalEnd > realEnd)
		{
			return false;
		}

		BRepBuilderAPI_MakeEdge me(curve, finalStart, finalEnd);
		me.Build();
		if (!me.IsDone())
		{
			return false;
		}

		setShape(me.Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool EdgeOperationBuilder::trimCurveAdaptEnd(const TopoDS_Shape& origEdge, const double endU)
{
	try
	{
		double realStart, realEnd;
		// 		BRepAdaptor_Curve adaptor(TopoDS::Edge((origEdge)));
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(origEdge), realStart, realEnd);
		// 		curve->FirstParameter();
		// 
		// 		Handle(Adaptor3d_HCurve) trimmedCurve = adaptor.Trim(startU, endU, Precision::Confusion());

		double finalStart, finalEnd;
		finalStart = realStart;
		finalEnd = endU;
		if (abs(endU - realEnd) < 0.001)
		{
			finalEnd = realEnd;
		}

		if (finalEnd > realEnd)
		{
			return false;
		}


		BRepBuilderAPI_MakeEdge me(curve, finalStart, finalEnd);
		me.Build();
		if (!me.IsDone())
		{
			return false;
		}

		setShape(me.Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool EdgeOperationBuilder::edgeExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont, const bool& forward)
{
	try
	{
		m_builder = nullptr;

		TopoDS_Shape edge;
		if (shape.ShapeType() <= TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeFilter(shape, TopAbs_EDGE, edge))
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		double first, last;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(edge), first, last);
		Handle(Geom_BoundedCurve) boundedCurve = Handle(Geom_BoundedCurve)::DownCast(curve);
		Handle(Geom_TrimmedCurve) trimmedCurve = Handle(Geom_TrimmedCurve)::DownCast(curve);

		bool normalExtend = false;
		if (trimmedCurve)
		{
			if (trimmedCurve->BasisCurve()->IsKind(STANDARD_TYPE(Geom_Line))||
				trimmedCurve->BasisCurve()->IsKind(STANDARD_TYPE(Geom_Circle)) ||
				trimmedCurve->BasisCurve()->IsKind(STANDARD_TYPE(Geom_Ellipse)) || 
				trimmedCurve->BasisCurve()->IsKind(STANDARD_TYPE(Geom_Parabola)) || 
				trimmedCurve->BasisCurve()->IsKind(STANDARD_TYPE(Geom_Hyperbola)))
			{
				curve = trimmedCurve->BasisCurve();
				normalExtend = true;
			}
		}

		TopoDS_Shape newEdge;
		if (!boundedCurve|| normalExtend)
		{
			double diff=0, curL = 0;
			while (abs(length- curL)>0.01&&curL <length)
			{
				diff += 0.01;
				GeomAdaptor_Curve result(curve, last, last+diff);
				curL = GCPnts_AbscissaPoint::Length(result);
			}
			last += diff;
// 			Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(shape), realStart, realEnd);
// 
// 			return 
// 			if (curve->IsKind(STANDARD_TYPE(Geom_Line)))
// 			{
// 				last += length;
// 			}
// 			else if (curve->IsKind(STANDARD_TYPE(Geom_Circle)))
// 			{
// 				Handle(Geom_Circle) baseCurve = Handle(Geom_Circle)::DownCast(curve);
// 				double radius = baseCurve->Circ().Radius();
// 				last += length/radius;
// 			}
// 			else if (curve->IsKind(STANDARD_TYPE(Geom_Ellipse)))
// 			{
// 			}
// 			else if (curve->IsKind(STANDARD_TYPE(Geom_Hyperbola)))
// 			{
// 			}
// 			else if (curve->IsKind(STANDARD_TYPE(Geom_Parabola)))
// 			{
// 			}
// 			else if (curve->IsKind(STANDARD_TYPE(Geom_OffsetCurve)))
// 			{
// 			}
// 			else
// 			{
// 				return false;
// 			}
			newEdge = BRepBuilderAPI_MakeEdge(curve, first, last);
			if (newEdge.IsNull())
			{
				return false;
			}
		}
		else
		{
			gp_Pnt endPnt;
			gp_Vec endVec;
			boundedCurve->D1(last, endPnt, endVec);
			gp_Pnt targetPnt = endPnt.Translated(gp_Vec(gp_Dir(endVec).XYZ()*length));

			GeomLib::ExtendCurveToPoint(boundedCurve, targetPnt, Cont, forward);
			newEdge = BRepBuilderAPI_MakeEdge(boundedCurve);
			if (newEdge.IsNull())
			{
				return false;
			}
		}
		setShape(newEdge);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool EdgeOperationBuilder::edgeClosedWire(TopTools_ListOfShape edges)
{
	try
	{
		//check 
		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_MakeWire;
		BRepBuilderAPI_MakeWire* builder = dynamic_cast<BRepBuilderAPI_MakeWire*>(m_builder);
		TopTools_ListOfShape::Iterator iter(edges);
		for (; iter.More(); iter.Next())
		{
			if (iter.Value().ShapeType() != TopAbs_WIRE)
			{
				return false;
			}
			else
			{
				builder->Add(TopoDS::Edge(iter.Value()));
			}
		}
		builder->Build();
		if (!builder->IsDone())
		{
			return false;
		}

		setShape(builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool EdgeOperationBuilder::wireRebuild(TopoDS_Wire wire)
{
	try
	{
		//check 
		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_MakeWire;
		BRepBuilderAPI_MakeWire* builder = dynamic_cast<BRepBuilderAPI_MakeWire*>(m_builder);
		builder->Add(wire);

		builder->Build();
		if (!builder->IsDone())
		{
			return false;
		}

		setShape(builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

// bool EdgeOperationBuilder::edgeSplitOperation(TopoDS_Shape shape1, Handle(Geom_Plane) cutPlane)
// {
// 	try
// 	{
// 		TopoDS_Edge edge;
// 		if (shape1.ShapeType() <= TopAbs_EDGE)
// 		{
// 			if (!ShapeUtilities::shapeFilter(shape1, TopAbs_EDGE, edge))
// 			{
// 				return false;
// 			}
// 		}
// 		else
// 		{
// 			return false;
// 		}
// 
// 		//get surface
// 		double first, last;
// 		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
// 		if (curve.IsNull())
// 		{
// 			return false;
// 		}
// 
// 		GeomAPI_IntCS intCS;
// 		intCS.Perform(curve, cutPlane);
// 		if (!intCS.IsDone())
// 		{
// 			return false;
// 		}
// 
// 		int numPnt = intCS.NbPoints();
// 		if (numPnt < 1)
// 		{
// 			return false;
// 		}
// 
// 		std::vector<double> UVector;
// 		UVector.push_back(first);
// 		UVector.push_back(last);
// 
// 		double U, V, T;
// 		for (int i = 1; i <= numPnt; ++i)
// 		{
// 			intCS.Parameters(i, U, V, T);
// 			UVector.push_back(T);
// 		}
// 
// 		//ÅÅÐò¡¢È¥ÖØ
// 		std::sort(UVector.begin(), UVector.end());
// 		std::vector<double>::iterator iter_end;
// 		iter_end = std::unique(UVector.begin(), UVector.end());
// 		UVector.erase(iter_end, UVector.end());
// 
// // 		m_ressultShapes.Clear();
// // 		for (int i = 0; i < UVector.size() - 1; ++i)
// // 		{
// // 			TopoDS_Edge newEdge = BRepBuilderAPI_MakeEdge(curve, UVector.at(i), UVector.at(i + 1));
// // 			m_ressultShapes.Append(newEdge);
// // 		}
// 
// 		return true;
// 	}
// 	catch (...)
// 	{
// 		return false;
// 	}
// }

bool EdgeOperationBuilder::edgesIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol)
{
	try
	{
		TopoDS_Edge edge1, edge2;
		if (shape1.ShapeType() <= TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeFilter(shape1, TopAbs_EDGE, edge1))
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		if (shape2.ShapeType() <= TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeFilter(shape2, TopAbs_EDGE, edge2))
			{
				return false;
			}
		}
		else
		{
			return false;
		}

// 		BRepExtrema_DistShapeShape extCC;
// 		extCC.LoadS1(edge1);
// 		extCC.LoadS2(edge2);
// 		extCC.Perform();
// 		if (!extCC.IsDone())
// 		{
// 			return false;
// 		}
// 		double distance = extCC.Value();
// 
// 		//collection
// 		int numPnt = extCC.NbSolution();
// 		if (numPnt < 1)
// 		{
// 			return false;
// 		}
// 		BRep_Builder B;
// 		TopoDS_Compound cmp;
// 		B.MakeCompound(cmp);
// 
// 		VertexShapeBuilder vertexBuilder;
// 		for (int i = 1; i <= numPnt; ++i)
// 		{
// 			gp_Pnt pnt = extCC.PointOnShape1(i);
// 			if (!vertexBuilder.createVertex(pnt.X(), pnt.Y(), pnt.Z()))
// 			{
// 				return false;
// 			}
// 			B.Add(cmp, vertexBuilder.getShape());
// 		}
// 		setShape(cmp);

		m_builder = new BRepAlgoAPI_Section;
		BRepAlgoAPI_Section* builder = dynamic_cast<BRepAlgoAPI_Section*>(m_builder);
		builder->Init1(shape1);
		builder->Init2(shape2);
		builder->Approximation(true);
		builder->Build();

		if (!builder->IsDone())
		{
			return false;
		}
		setShape(m_builder->Shape());
		return true;

		BRepExtrema_ExtCC extCC;
		extCC.Initialize(edge2);
		extCC.Perform(edge1);
		if (!extCC.IsDone())
		{
			return false;
		}
		if (extCC.IsParallel())
		{
			return false;
		}

		//collection
		int numPnt = extCC.NbExt();
		if (numPnt < 1)
		{
			return false;
		}
		BRep_Builder B;
		TopoDS_Compound cmp;
		B.MakeCompound(cmp);

		VertexShapeBuilder vertexBuilder;
		for (int i = 1; i <= numPnt; ++i)
		{
			double distance = extCC.SquareDistance(i);
			gp_Pnt pnt = extCC.PointOnE1(i);
			if (!vertexBuilder.createVertex(pnt.X(), pnt.Y(), pnt.Z()))
			{
				return false;
			}
			if (distance <= tol)
			{
				B.Add(cmp, vertexBuilder.getShape());
			}
		}
		setShape(cmp);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool EdgeOperationBuilder::edgesCombinationOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol)
{
	try
	{
		if (shape1.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}

		if (shape2.ShapeType() != TopAbs_EDGE)
		{
			return false;
		}
		Handle(Geom_BoundedCurve) finalCurve1, finalCurve2;

		double first1, last1;
		Handle(Geom_Curve) curve1 = BRep_Tool::Curve(TopoDS::Edge(shape1), first1, last1);
		Handle(Geom_BoundedCurve) boundedCurve1 = Handle(Geom_BoundedCurve)::DownCast(curve1);
		if (!boundedCurve1)
		{
			finalCurve1 = new Geom_TrimmedCurve(curve1, first1, last1);
		}
		else
		{
			finalCurve1 = boundedCurve1;
		}

		double first2, last2;
		Handle(Geom_Curve) curve2 = BRep_Tool::Curve(TopoDS::Edge(shape2), first2, last2);
		Handle(Geom_BoundedCurve) boundedCurve2 = Handle(Geom_BoundedCurve)::DownCast(curve2);
		if (!boundedCurve2)
		{
			finalCurve2 = new Geom_TrimmedCurve(curve2, first2, last2);
		}
		else
		{
			finalCurve2 = boundedCurve2;
		}

		GeomConvert_CompCurveToBSplineCurve convert;
		bool success1 = convert.Add(finalCurve1, Precision::Confusion());
		bool success2 = convert.Add(finalCurve2, Precision::Confusion());
		if (!success1 || !success2)
		{
			return false;
		}
		Handle(Geom_Curve) resultCurve = convert.BSplineCurve();
		
		TopoDS_Shape newEdge = BRepBuilderAPI_MakeEdge(resultCurve);
		if (newEdge.IsNull())
		{
			return false;
		}

		BRep_Builder B;
		TopoDS_Wire finalWire;
		B.MakeWire(finalWire);
		B.Add(finalWire, newEdge);

		setShape(finalWire);

// 		BOPAlgo_Builder aBuilder;
// 		// Setting arguments
// 		TopTools_ListOfShape aLSObjects;
// 		aLSObjects.Append(shape1); // Objects
// 		aLSObjects.Append(shape2); // Objects
// 		aBuilder.SetArguments(aLSObjects);
// 		aBuilder.SetFuzzyValue(tol);
// 		Standard_Boolean bSafeMode = Standard_True;
// 		aBuilder.SetNonDestructive(bSafeMode);
// 		BOPAlgo_GlueEnum aGlue = BOPAlgo_GlueShift;
// 		aBuilder.SetGlue(aGlue);
// 		Standard_Boolean bCheckInverted = Standard_False;
// 		aBuilder.SetCheckInverted(bCheckInverted);
// 		Standard_Boolean bUseOBB = Standard_True;
// 		aBuilder.SetUseOBB(bUseOBB);
// 		aBuilder.Perform();
// 
// 		if (aBuilder.HasErrors())
// 		{
// 			return false;
// 		}
// 
// 		setShape(aBuilder.Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}
