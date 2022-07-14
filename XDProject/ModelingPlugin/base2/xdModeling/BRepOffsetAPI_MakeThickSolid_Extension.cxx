#include "BRepOffsetAPI_MakeThickSolid_Extension.hxx"

#include <BRepOffset_MakeOffset.hxx>
#include <Standard_ConstructionError.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>


//=======================================================================
//function : BRepOffsetAPI_MakeThickSolid_Extension
//purpose  : 
//=======================================================================
BRepOffsetAPI_MakeThickSolid_Extension::BRepOffsetAPI_MakeThickSolid_Extension()
{
    // Build only solids.
}

//=======================================================================
//function : MakeThickSolidByJoin
//purpose  : 
//=======================================================================
void BRepOffsetAPI_MakeThickSolid_Extension::Thickening(const TopoDS_Shape& S,
    const Standard_Real Offset,
    const Standard_Real Tol,
    const BRepOffset_Mode Mode,
    const Standard_Boolean Intersection,
    const Standard_Boolean SelfInter,
    const GeomAbs_JoinType Join,
    const Standard_Boolean Thickening,
    const Standard_Boolean RemoveIntEdges)
{
    NotDone();

    myLastUsedAlgo = OffsetAlgo_JOIN;

    myOffsetShape.Initialize(S, Offset, Tol, Mode, Intersection, SelfInter,
        Join, Thickening, RemoveIntEdges);
    myOffsetShape.MakeOffsetShape();
    if (!myOffsetShape.IsDone())
        return;

    myShape = myOffsetShape.Shape();

    Done();
}

//=======================================================================
//function : Build
//purpose  : 
//=======================================================================
void BRepOffsetAPI_MakeThickSolid_Extension::Build()
{
}

//=======================================================================
//function : Modified
//purpose  : 
//=======================================================================
const TopTools_ListOfShape& BRepOffsetAPI_MakeThickSolid_Extension::Modified(const TopoDS_Shape& F)
{
    myGenerated.Clear();

    if (myLastUsedAlgo == OffsetAlgo_JOIN && myOffsetShape.OffsetFacesFromShapes().HasImage(F))
    {
        if (myOffsetShape.ClosingFaces().Contains(F))
        {
            myOffsetShape.OffsetFacesFromShapes().LastImage(F, myGenerated);

            // Reverse generated shapes in case of small solids.
            // Useful only for faces without influence on others.
            TopTools_ListIteratorOfListOfShape it(myGenerated);
            for (; it.More(); it.Next())
                it.Value().Reverse();
        }
    }
    else if (myLastUsedAlgo == OffsetAlgo_SIMPLE)
    {
        TopoDS_Shape aModShape = mySimpleOffsetShape.Modified(F);
        if (!aModShape.IsNull())
            myGenerated.Append(aModShape);
    }

    return myGenerated;
}
