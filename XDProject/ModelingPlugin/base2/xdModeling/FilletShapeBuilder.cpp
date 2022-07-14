#include "FilletShapeBuilder.h"
#include "ShapeUtilities.h"
#include "BRepFilletAPI_MakeFillet2d_Extension.hxx"
#include "OCCFixTool.h"

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

FilletShapeBuilder::FilletShapeBuilder()
{
}


FilletShapeBuilder::~FilletShapeBuilder()
{
}

bool FilletShapeBuilder::edgeFilletOperation(TopoDS_Shape solidShape, const NCollection_List<TopoDS_Shape>& edgelist, const double& radius)
{
    try
	{
		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepFilletAPI_MakeFillet(solidShape);
        BRepFilletAPI_MakeFillet* builder = dynamic_cast<BRepFilletAPI_MakeFillet*>(m_builder);
        NCollection_List<TopoDS_Shape>::Iterator iter(edgelist);
        for (; iter.More(); iter.Next())
        {
            if (iter.Value().ShapeType() != TopAbs_EDGE)
            {
                return false;
            }

            builder->Add(radius, TopoDS::Edge(iter.Value()));
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

bool FilletShapeBuilder::edgeChamferOperation(TopoDS_Shape solidShape, const NCollection_List<TopoDS_Shape>& edgelist, const double& lDistance, const double& rDistance, const bool& symmetryFlag)
{
    try
	{
		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepFilletAPI_MakeChamfer(solidShape);
        BRepFilletAPI_MakeChamfer* builder = dynamic_cast<BRepFilletAPI_MakeChamfer*>(m_builder);
        NCollection_List<TopoDS_Shape>::Iterator iter(edgelist);
        if (symmetryFlag)
        {
            for (; iter.More(); iter.Next())
            {
                if (iter.Value().ShapeType() != TopAbs_EDGE)
                {
                    return false;
                }

                builder->Add(lDistance, TopoDS::Edge(iter.Value()));
            }
        }
        else
        {
            for (; iter.More(); iter.Next())
            {
                if (iter.Value().ShapeType() != TopAbs_EDGE)
                {
                    return false;
                }

                bool found = false;
                TopoDS_Face foundFace;
                TopExp_Explorer faceExp(solidShape, TopAbs_FACE);
                for (; faceExp.More(); faceExp.Next())
                {
                    TopExp_Explorer edgeExp(faceExp.Value(), TopAbs_EDGE);
                    for (; edgeExp.More(); edgeExp.Next())
                    {
                        if (edgeExp.Value().IsEqual(iter.Value())&&!faceExp.Value().IsEqual(foundFace))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                    {
                        break;
                    }
                }
                builder->Add(lDistance,rDistance, TopoDS::Edge(iter.Value()), TopoDS::Face(faceExp.Value()));
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

bool FilletShapeBuilder::vertexFillet2DOperation(TopoDS_Shape ownerShape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag)
{
	try
	{
		if (ownerShape.ShapeType() != TopAbs_WIRE && ownerShape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		//start
		if (m_builder)
		{
			delete m_builder;
		}

		if (filletFlag)
		{
			m_builder = new BRepFilletAPI_MakeFillet2d_Extension(ownerShape);
			BRepFilletAPI_MakeFillet2d_Extension* builder = dynamic_cast<BRepFilletAPI_MakeFillet2d_Extension*>(m_builder);
			ChFi2d_ConstructionError error = builder->Status();

			NCollection_List<TopoDS_Shape>::Iterator iter(vertexlist);
			for (; iter.More(); iter.Next())
			{
				TopoDS_Shape vertex = iter.Value();
				if (vertex.ShapeType() != TopAbs_VERTEX)
				{
					return false;
				}

				TopoDS_Edge edge = builder->AddFillet(TopoDS::Vertex(vertex), lDistanceOrRradius);
				error = builder->Status();
				if (error != ChFi2d_IsDone)
				{
					return false;
				}
			}
		}
		else
		{
			double D1 = lDistanceOrRradius, D2;
			if (symmetryFlag)
			{
				D2 = D1;
			}
			else
			{
				D2 = rDistance;
			}
			m_builder = new BRepFilletAPI_MakeFillet2d_Extension(ownerShape);
			BRepFilletAPI_MakeFillet2d_Extension* builder = dynamic_cast<BRepFilletAPI_MakeFillet2d_Extension*>(m_builder);
			ChFi2d_ConstructionError error = builder->Status();

			NCollection_List<TopoDS_Shape>::Iterator iter(vertexlist);
			for (; iter.More(); iter.Next())
			{
				if (iter.Value().ShapeType() != TopAbs_VERTEX)
				{
					return false;
				}

				TopoDS_Edge edge = builder->AddChamfer(TopoDS::Vertex(iter.Value()), D1, D2);
				error = builder->Status();
				if (error != ChFi2d_IsDone)
				{
					return false;
				}
			}
		}

		TopoDS_Shape finalShape = m_builder->Shape();

		setShape(finalShape);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FilletShapeBuilder::vertexFillet2DOperation2(TopoDS_Shape ownerShape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag)
{
	try
	{
		TopoDS_Face sourceFace;
		if (ownerShape.ShapeType() == TopAbs_WIRE)
		{
			if (!ShapeUtilities::shapeCover(ownerShape, sourceFace))
			{
				return false;
			}
		}

		else if (ownerShape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}
		else
		{
			sourceFace = TopoDS::Face(ownerShape);
		}

		//start
		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepFilletAPI_MakeFillet2d(sourceFace);
		BRepFilletAPI_MakeFillet2d* builder = dynamic_cast<BRepFilletAPI_MakeFillet2d*>(m_builder);
		ChFi2d_ConstructionError error = builder->Status();
		if (filletFlag)
		{
			NCollection_List<TopoDS_Shape>::Iterator iter(vertexlist);
			for (; iter.More(); iter.Next())
			{
				if (iter.Value().ShapeType() != TopAbs_VERTEX)
				{
					return false;
				}

				TopoDS_Edge edge = builder->AddFillet(TopoDS::Vertex(iter.Value()), lDistanceOrRradius);
				error = builder->Status();
			}
		}
		else
		{
			NCollection_List<TopoDS_Shape>::Iterator iter(vertexlist);
			NCollection_List<TopoDS_Edge> edgeShapelist;
			for (; iter.More(); iter.Next())
			{
				if (iter.Value().ShapeType() != TopAbs_VERTEX)
				{
					return false;
				}

				TopTools_ListOfShape adjEdges;
				TopoDS_Edge adjEdge1, adjEdge2;
				bool found = ShapeUtilities::FindConnectedEdges(TopoDS::Face(sourceFace), TopoDS::Vertex(iter.Value()), adjEdges);
				if (!found || adjEdges.Size() != 2) return false;
				adjEdge1 = TopoDS::Edge(adjEdges.First());
				adjEdge2 = TopoDS::Edge(adjEdges.Last());

				if (symmetryFlag)
				{
					builder->AddChamfer(adjEdge1, adjEdge2, lDistanceOrRradius, lDistanceOrRradius);
				}
				else
				{
					builder->AddChamfer(adjEdge1, adjEdge2, lDistanceOrRradius, rDistance);
				}
			}
		}

		builder->Build();
		if (!builder->IsDone()) {
			return false;
		}

		TopoDS_Shape finalShape = builder->Shape();
		if (ownerShape.ShapeType() == TopAbs_WIRE)
		{
			if (!ShapeUtilities::shapeFilter(finalShape, TopAbs_WIRE, finalShape))
			{
				return false;
			}
		}

		setShape(finalShape);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool FilletShapeBuilder::vertexFillet2DOperation(TopoDS_Shape ownerShape, const TopoDS_Shape& vertex, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag)
{
	try
	{
		if (ownerShape.ShapeType() != TopAbs_WIRE && ownerShape.ShapeType() != TopAbs_FACE)
		{
			return false;
		}

		//start
		if (m_builder)
		{
			delete m_builder;
		}

		if (filletFlag)
		{
			m_builder = new BRepFilletAPI_MakeFillet2d_Extension(ownerShape);
			BRepFilletAPI_MakeFillet2d_Extension* builder = dynamic_cast<BRepFilletAPI_MakeFillet2d_Extension*>(m_builder);
			ChFi2d_ConstructionError error = builder->Status();

			if (vertex.ShapeType() != TopAbs_VERTEX)
			{
				return false;
			}

			TopoDS_Edge edge = builder->AddFillet(TopoDS::Vertex(vertex), lDistanceOrRradius);
			error = builder->Status();
			if (error != ChFi2d_IsDone)
			{
				return false;
			}
		}
		else
		{
// 			double D1 = lDistanceOrRradius, D2;
// 			if (symmetryFlag)
// 			{
// 				D2 = D1;
// 			}
// 			else
// 			{
// 				D2 = rDistance;
// 			}
// 			NCollection_List<TopoDS_Shape>::Iterator iter(vertexlist);
// 			for (; iter.More(); iter.Next())
// 			{
// 				// 				m_builder = new BRepFilletAPI_MakeFillet2d_Extension(sourceFace);
// 				BRepFilletAPI_MakeFillet2d_Extension* builder = dynamic_cast<BRepFilletAPI_MakeFillet2d_Extension*>(m_builder);
// 				ChFi2d_ConstructionError error = builder->Status();
// 
// 				if (iter.Value().ShapeType() != TopAbs_VERTEX)
// 				{
// 					return false;
// 				}
// 
// 				TopoDS_Edge edge = builder->AddChamfer(TopoDS::Vertex(iter.Value()), D1, D2);
// 				error = builder->Status();
// 				if (error != ChFi2d_IsDone)
// 				{
// 					return false;
// 				}
// 				// 				sourceFace = TopoDS::Face(builder->Shape());
// 			}
		}

		//         if (!m_builder->IsDone()) {
		//             return false;
		//         }
		// 
		TopoDS_Shape finalShape = m_builder->Shape();
// 		if (ownerShape.ShapeType() == TopAbs_WIRE)
// 		{
// 			if (!ShapeUtilities::shapeFilter(finalShape, TopAbs_WIRE, finalShape))
// 			{
// 				return false;
// 			}
// 		}

		setShape(finalShape);

		return true;
	}
	catch (...)
	{
		return false;
	}
}
