#include "VertexShapeBuilder.h"

#include <TopoDS_Vertex.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

VertexShapeBuilder::VertexShapeBuilder()
{
}


VertexShapeBuilder::~VertexShapeBuilder()
{
}

bool VertexShapeBuilder::createVertex(const double& xPos, const double& yPos, const double& zPos)
{
    try
    {
        //create shape
        gp_Pnt P(xPos, yPos, zPos);
        TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(P);

        setShape(vertex);

        return true;
    }
    catch (...)
    {
        return false;
    }
}