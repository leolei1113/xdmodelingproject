#include "BRepAlgoAPI_IntCS.hxx"
#include "ShapeUtilities.h"
#include "OCCFixTool.h"

#include <BOPAlgo_PaveFiller.hxx>
#include <BOPDS_DS.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <gp_Pln.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
//
//=======================================================================
//function : BRepAlgoAPI_Section_Extension
//purpose  : 
//=======================================================================
BRepAlgoAPI_IntCS::BRepAlgoAPI_IntCS()
    :
    BRepAlgoAPI_BooleanOperation()
{
    myGenerated.Clear();
}

//=======================================================================
//function : ~BRepAlgoAPI_Section_Extension
//purpose  : 
//=======================================================================
BRepAlgoAPI_IntCS::~BRepAlgoAPI_IntCS()
{
}
//=======================================================================
//function : Init
//purpose  : 
//=======================================================================

void BRepAlgoAPI_IntCS::Init(const TopoDS_Shape& S1, const TopoDS_Shape& S2)
{
    myS1 = S1;
    myS2 = S2;
}

//=======================================================================
//function : Build
//purpose  : 
//=======================================================================
void BRepAlgoAPI_IntCS::Build()
{
    NotDone();

    myHistory = new BRepTools_History;

    //check
    if (myS1.IsNull() || myS2.IsNull())
    {
        return;
    }

    //get face
    TopoDS_Face face1;
    if (myS1.ShapeType() <= TopAbs_FACE)
    {
        if (!ShapeUtilities::shapeFilter(myS1, TopAbs_FACE, face1))
        {
            return;
        }
    }
    else
    {
        return;
    }

    TopoDS_Edge edge2;
    if (myS2.ShapeType() <= TopAbs_EDGE)
    {
        if (!ShapeUtilities::shapeFilter(myS2, TopAbs_EDGE, edge2))
        {
            return;
        }
    }
    else
    {
        return;
    }

    try
    {
        //get surface
        Handle(Geom_Surface) surf1;
        surf1 = BRep_Tool::Surface(face1);
        Handle(Geom_Curve) curve2;
        double first, last;
        curve2 = BRep_Tool::Curve(edge2, first, last);
        if (surf1.IsNull() || curve2.IsNull())
        {
            return;
        }

        myIntCS = GeomAPI_IntCS();
        myIntCS.Perform(curve2, surf1);
        if (myIntCS.IsDone())
        {
            int numPnts = myIntCS.NbPoints();
            int numSegs = myIntCS.NbSegments();//Returns the number of computed intersection segments in case of tangential intersection.

            if (numSegs > 0)
            {
                BRep_Builder B;
                TopoDS_Wire wire;
                B.MakeWire(wire);
                for (int i = 1; i <= numSegs; ++i)
                {
                    Handle_Geom_Curve curve = myIntCS.Segment(i);
                    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(curve);

                    if (!edge.IsNull())
                    {
                        //以主shape去继承
                        myHistory->AddGenerated(myS1, edge);
                        //                     myHistory->AddGenerated(myS2, edge);
                        B.Add(wire, edge);
                    }
                }

                OCCFixToolInstance->fixTopoValidityOp(wire);
                myShape = wire;
                myHistory->AddGenerated(myS1, wire);
            }
            else if (numPnts > 0)
            {
                BRep_Builder B;
                TopoDS_Compound cmp;
                B.MakeCompound(cmp);
                double U, V, T;
                for (int i = 1; i <= numPnts; ++i)
                {
                    myIntCS.Parameters(i, U, V, T);
                    gp_Pnt P;
                    curve2->D0(T, P);
                    TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(P);
                    myHistory->AddGenerated(myS1, vertex); //以主shape去继承

                    if (numPnts == 1)
                    {
                        myShape = vertex;
                    }
                    else
                    {
                        B.Add(cmp, vertex);
                    }
                }

                if (numPnts>1)
                {
                    myShape = cmp;
                    myHistory->AddGenerated(myS1, cmp);
                }
            }
            else
            {
                NotDone();
                return;
            }
        }
    }
    catch (...)
    {
        NotDone();
        return;
    }

    Done();
}

const TopTools_ListOfShape& BRepAlgoAPI_IntCS::Modified(const TopoDS_Shape& shape)
{
    myGenerated.Clear();
    if (myHistory)
    {
        myGenerated.Assign(myHistory->Generated(shape));
    }

    return myGenerated;
}
