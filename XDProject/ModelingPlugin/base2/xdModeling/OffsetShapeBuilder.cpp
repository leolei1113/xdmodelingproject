#include "OffsetShapeBuilder.h"
#include "BRepOffsetAPI_MakeThickSolid_Extension.hxx"
#include "ShapeUtilities.h"

#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx>
#include <BRep_Tool.hxx>
#include <Geom_BoundedSurface.hxx>
#include <GeomLib.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepOffsetAPI_MakeOffset.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Geom_BoundedCurve.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>


OffsetShapeBuilder::OffsetShapeBuilder()
{
}


OffsetShapeBuilder::~OffsetShapeBuilder()
{
}

bool OffsetShapeBuilder::edgeOffsetOperation(TopoDS_Shape shape, const double& distance)
{
    try
    {
        TopoDS_Shape wire;
        if (shape.ShapeType() <= TopAbs_WIRE)
        {
            if (!ShapeUtilities::shapeFilter(shape, TopAbs_WIRE, wire))
            {
                return false;
            }
        }
        else
        {
            return false;
        }

		if (m_builder)
		{
			delete m_builder;
		}
		//m_builder = new BRepOffsetAPI_MakeOffset();
// 		BRepOffsetAPI_MakeOffset* builder = dynamic_cast<BRepOffsetAPI_MakeOffset*>(m_builder);
// 		builder->AddWire(TopoDS::Wire(wire));
// 		builder->Perform(distance);

		m_builder = new BRepOffsetAPI_MakeOffset(TopoDS::Wire(wire), GeomAbs_Intersection, Standard_True);
        BRepOffsetAPI_MakeOffset* builder = dynamic_cast<BRepOffsetAPI_MakeOffset*>(m_builder);
        builder->Perform(distance);

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

bool OffsetShapeBuilder::faceOffsetOperation(TopoDS_Shape shape, const double& distance, const bool& thickening)
{
    try
    {
		if (m_builder)
		{
			delete m_builder;
		}
		if (thickening)
		{
			m_builder = new BRepOffsetAPI_MakeThickSolid_Extension();
			BRepOffsetAPI_MakeThickSolid_Extension* builder = dynamic_cast<BRepOffsetAPI_MakeThickSolid_Extension*>(m_builder);
			builder->Thickening(shape, distance, Precision::Confusion(), BRepOffset_Skin, true, false, GeomAbs_Intersection, thickening, true);
			if (!m_builder->IsDone()) {
				return false;
			}
		}
		else
		{
			m_builder = new BRepOffsetAPI_MakeOffsetShape();
			BRepOffsetAPI_MakeOffsetShape* builder = dynamic_cast<BRepOffsetAPI_MakeOffsetShape*>(m_builder);
			builder->PerformByJoin(shape, distance, Precision::Confusion(), BRepOffset_Skin, false, false, GeomAbs_Intersection);
			if (!m_builder->IsDone()) {
				if (m_builder)
				{
					delete m_builder;
				}
				m_builder = new BRepOffsetAPI_MakeOffsetShape();
				BRepOffsetAPI_MakeOffsetShape* builder2 = dynamic_cast<BRepOffsetAPI_MakeOffsetShape*>(m_builder);
				builder2->PerformByJoin(shape, distance, Precision::Confusion(), BRepOffset_Skin, false, false, GeomAbs_Arc);
				if (!m_builder->IsDone()) {
					return false;
				}
			}
		}

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
    // 
    //     try
    //     {
    //         m_builder = nullptr;
    // 
    //         BRepOffset_MakeOffset myOffsetShape;
    // 
    //         myOffsetShape.Initialize(shape, thickness, Precision::Confusion(), BRepOffset_Skin, true, false, GeomAbs_Intersection, true, true);
    //         myOffsetShape.MakeOffsetShape();
    //         if (!myOffsetShape.IsDone()) return false;
    //         setShape(myOffsetShape.Shape());
    //         return true;
    //     }
    //     catch (...)
    //     {
    //         return false;
    //     }
}

bool OffsetShapeBuilder::solidFaceOffsetOperation(TopoDS_Shape shape, TopTools_ListOfShape faceArray, const double& distance)
{
	try
	{
		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepOffsetAPI_MakeThickSolid();
		BRepOffsetAPI_MakeThickSolid* builder = dynamic_cast<BRepOffsetAPI_MakeThickSolid*>(m_builder);
		builder->MakeThickSolidByJoin(shape, faceArray,  distance, Precision::Confusion(), BRepOffset_Skin, false, false, GeomAbs_Intersection, true);
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