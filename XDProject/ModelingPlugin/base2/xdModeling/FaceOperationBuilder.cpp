#include "FaceOperationBuilder.h"
#include "ShapeUtilities.h"

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
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <GeomLib.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_BoundedSurface.hxx>
#include <GeomConvert.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepAlgoAPI_IntCS.hxx>
#include <BRepAlgoAPI_IntSS.hxx>

FaceOperationBuilder::FaceOperationBuilder()
{
}


FaceOperationBuilder::~FaceOperationBuilder()
{
}

bool FaceOperationBuilder::faceCover(TopTools_ListOfShape wires)
{
	try
	{
		//check 
		TopTools_ListOfShape::Iterator iter(wires);
		for (; iter.More(); iter.Next())
		{
			if (iter.Value().ShapeType() != TopAbs_WIRE)
			{
				return false;
			}
		}

		//start
		TopoDS_Shape contour = wires.First();
		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_MakeFace(TopoDS::Wire(contour));
		if (wires.Size() > 1)
		{
			BRepBuilderAPI_MakeFace* builder = dynamic_cast<BRepBuilderAPI_MakeFace*>(m_builder);
			iter.Init(wires);
			iter.Next();
			for (; iter.More(); iter.Next())
			{
				TopoDS_Shape cutWire = iter.Value();
				if (cutWire.Orientation() == TopAbs_FORWARD)
				{
					cutWire.Reverse();
				}
				builder->Add(TopoDS::Wire(cutWire));
			}
		}

		m_builder->Build();
		if (!m_builder->IsDone()) {
			return false;
		}

		setShape(m_builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceOperationBuilder::faceUncover(TopoDS_Shape face)
{
	try
	{
		//check 
		if (face.ShapeType() != TopAbs_SHELL && face.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		//start
		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_MakeWire;
		BRepBuilderAPI_MakeWire* builder = dynamic_cast<BRepBuilderAPI_MakeWire*>(m_builder);
		TopExp_Explorer exp(face, TopAbs_WIRE);
		for (; exp.More(); exp.Next())
		{
			builder->Add(TopoDS::Wire(exp.Value()));
		}

		m_builder->Build();
		if (!m_builder->IsDone()) {
			return false;
		}

		setShape(m_builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceOperationBuilder::faceTrim(TopoDS_Shape face, TopTools_ListOfShape wires)
{
	try
	{
		//check 
		TopoDS_Shape filterFace;
		if (!ShapeUtilities::shapeFilter(face, TopAbs_FACE, filterFace))
		{
			return false;
		}

		TopTools_ListOfShape::Iterator iter(wires);
		for (; iter.More(); iter.Next())
		{
			if (iter.Value().ShapeType() != TopAbs_WIRE)
			{
				return false;
			}
		}

		//start
		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepBuilderAPI_MakeFace(TopoDS::Face(filterFace));
		if (wires.Size() > 1)
		{
			BRepBuilderAPI_MakeFace* builder = dynamic_cast<BRepBuilderAPI_MakeFace*>(m_builder);
			iter.Init(wires);
			for (; iter.More(); iter.Next())
			{
				TopoDS_Shape cutWire = iter.Value();
				if (cutWire.Orientation() == TopAbs_FORWARD)
				{
					cutWire.Reverse();
				}
				builder->Add(TopoDS::Wire(cutWire));
			}
		}

		m_builder->Build();
		if (!m_builder->IsDone()) {
			return false;
		}

		setShape(m_builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}


bool FaceOperationBuilder::faceExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont, const bool& inU, const bool& forward)
{
	try
	{
		m_builder = nullptr;

		if (shape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

// 		double first, last;
		Handle(Geom_Surface) surf = BRep_Tool::Surface(TopoDS::Face(shape));
		Handle(Geom_BoundedSurface) boundedSurf = Handle(Geom_BoundedSurface)::DownCast(surf);

		TopoDS_Shape newFace;
		if (!boundedSurf)
		{
			BRepAdaptor_Surface adaptor(TopoDS::Face(shape));
			double Ustart, Uend, Vstart, Vend;
			Ustart = adaptor.FirstUParameter();
			Uend = adaptor.LastUParameter();
			Vstart = adaptor.FirstVParameter();
			Vend = adaptor.LastVParameter();
			Handle(Geom_RectangularTrimmedSurface) trimmedSurf = new Geom_RectangularTrimmedSurface(surf, Ustart, Uend, Vstart, Vend);
			boundedSurf = trimmedSurf;
// 
// 			boundedSurf = adaptor.Surface().BSpline();
		}

		GeomLib::ExtendSurfByLength(boundedSurf, length, Cont,inU, forward);
		newFace = BRepBuilderAPI_MakeFace(boundedSurf,Precision::Confusion());
		if (newFace.IsNull())
		{
			return false;
		}

		setShape(newFace);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FaceOperationBuilder::faceIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const int& method)
{
	try
	{
		if (method == 1)
		{
			if (m_builder)
			{
				delete m_builder;
			}
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
		}
		else if (method == 2)
		{
			if (m_builder)
			{
				delete m_builder;
			}
			m_builder = new BRepAlgoAPI_IntCS;
			BRepAlgoAPI_IntCS* builder = dynamic_cast<BRepAlgoAPI_IntCS*>(m_builder);
			builder->Init(shape1, shape2);
			builder->Build();

			if (!builder->IsDone())
			{
				return false;
			}
		}
		else if (method == 3)
		{
			if (m_builder)
			{
				delete m_builder;
			}
			m_builder = new BRepAlgoAPI_IntSS;
			BRepAlgoAPI_IntSS* builder = dynamic_cast<BRepAlgoAPI_IntSS*>(m_builder);
			builder->Init(shape1, shape2);
			builder->Build();

			if (!builder->IsDone())
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		setShape(m_builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}