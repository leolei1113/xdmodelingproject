#include "BRepAlgoAPI_IntSS.hxx"
#include "ShapeUtilities.h"
#include "OCCFixTool.h"

#include <BOPAlgo_PaveFiller.hxx>
#include <BOPDS_DS.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <gp_Pln.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

//
//=======================================================================
//function : BRepAlgoAPI_Section_Extension
//purpose  : 
//=======================================================================
BRepAlgoAPI_IntSS::BRepAlgoAPI_IntSS()
    :
    BRepAlgoAPI_BooleanOperation()
{
    myGenerated.Clear();
}

//=======================================================================
//function : ~BRepAlgoAPI_Section_Extension
//purpose  : 
//=======================================================================
BRepAlgoAPI_IntSS::~BRepAlgoAPI_IntSS()
{
}
//=======================================================================
//function : Init
//purpose  : 
//=======================================================================

void BRepAlgoAPI_IntSS::Init(const TopoDS_Shape& S1, const TopoDS_Shape& S2)
{
    myS1 = S1;
    myS2 = S2;
}

void BRepAlgoAPI_IntSS::Init(const TopoDS_Shape& S1, Handle(Geom_Surface) cutSurf)
{
    myS1 = S1;
    myCutSurf = cutSurf;
}

//=======================================================================
//function : Build
//purpose  : 
//=======================================================================
void BRepAlgoAPI_IntSS::Build()
{
    NotDone();

    myHistory = new BRepTools_History;

    //check
    if (myS1.IsNull() || myS2.IsNull())
    {
        return;
    }
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

    if (myCutSurf.IsNull())
    {
        if (myS2.IsNull())
        {
            return;
        }

        TopoDS_Face face2;
        if (myS2.ShapeType() <= TopAbs_FACE)
        {
            if (!ShapeUtilities::shapeFilter(myS2, TopAbs_FACE, face2))
            {
                return;
            }
        }
        else
        {
            return;
        }

        myCutSurf = BRep_Tool::Surface(face2);
    }


    try
    {
        //get surface
        Handle(Geom_Surface) surf1;
        surf1 = BRep_Tool::Surface(face1);

        if (surf1.IsNull() || myCutSurf.IsNull())
        {
            return;
        }

        myIntSS = GeomAPI_IntSS();
        myIntSS.Perform(surf1, myCutSurf, Precision::Confusion());
        if (myIntSS.IsDone())
        {
            int numLines = myIntSS.NbLines();

            if (numLines > 0)
            {
                BRep_Builder B;
                TopoDS_Wire wire;
                B.MakeWire(wire);
                for (int i = 1; i <= numLines; ++i)
                {
                    Handle_Geom_Curve curve = myIntSS.Line(i);
                    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(curve);

                    if (!edge.IsNull())
                    {
                        //ÒÔÖ÷shapeÈ¥¼Ì³Ð
                        myHistory->AddGenerated(myS1, edge);
                        //                     myHistory->AddGenerated(myS2, edge);
                        B.Add(wire, edge);
                    }
                }
                OCCFixToolInstance->fixTopoValidityOp(wire);
                myShape = wire;
                myHistory->AddGenerated(myS1, wire);
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

const TopTools_ListOfShape& BRepAlgoAPI_IntSS::Modified(const TopoDS_Shape& shape)
{
    myGenerated.Clear();
    if (myHistory)
    {
        myGenerated.Assign(myHistory->Generated(shape));
    }

    return myGenerated;
}
