#include "BRepFilletAPI_MakeFillet2d_Extension.hxx"
#include "ShapeUtilities.h"

// Created on: 1995-08-31
// Created by: Remi LEQUETTE
// Copyright (c) 1995-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.


#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Builder.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRepLib.hxx>
#include <BRepLib_MakeFace.hxx>
#include <TopTools_ListOfShape.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>


// =======================================================================
// function : BRepFilletAPI_MakeFillet2d_Extension
// purpose  : 
// =======================================================================
// BRepFilletAPI_MakeFillet2d_Extension::BRepFilletAPI_MakeFillet2d_Extension()
// {
// }

//=======================================================================
//function : BRepFilletAPI_MakeFillet2d_Extension
//purpose  : 
//=======================================================================

// BRepFilletAPI_MakeFillet2d_Extension::BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Face& F)
// {
//     if (F.IsNull()) {
//         status = ChFi2d_NoFace;
//         return;
//     }
// 
// 	newFace = refFace = F;
// 	if (!ShapeUtilities::isShapePlannar(refFace, plane))
// 	{
// 		status = ChFi2d_NotPlanar;
// 	}
// //     TopLoc_Location Loc;
// //     //  syntaxe invalide sur NT
// //     //  const Handle(Geom_Surface)&  surf = BRep_Tool::Surface( F, Loc);
// //     //  if (surf->IsKind(STANDARD_TYPE(Geom_Plane))) {
// //     if (BRep_Tool::Surface(F, Loc)
// //         ->IsKind(STANDARD_TYPE(Geom_Plane))) {
// //         newFace = refFace = F;
// //         newFace.Orientation(TopAbs_FORWARD);
// //         BRepLib::BuildCurves3d(newFace);
// //         status = ChFi2d_Ready;
// //     }
// //     else status = ChFi2d_NotPlanar;
// // 
// //     Handle(Geom_Plane) hPlane = ShapeUtilities::findPlane(refFace);
// //     if (hPlane.IsNull())
// //     {
// //         status = ChFi2d_NotPlanar;
// //     }
// //     else
// //     {
// //         // Algo.
// //         plane = hPlane->Pln();
// //     }
// }

Standard_EXPORT BRepFilletAPI_MakeFillet2d_Extension::BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Shape& Shape)
{
	if (Shape.IsNull()) {
		status = ChFi2d_NoFace;
		return;
	}

	if (Shape.ShapeType() == TopAbs_WIRE)
	{
		faceMode = false;
	}
	else if (Shape.ShapeType() == TopAbs_FACE)
	{
		faceMode = true;
	}
	else
	{
		return;
	}

	newShape = refShape = Shape;
	if (!ShapeUtilities::isShapePlannar(refShape, plane))
	{
		status = ChFi2d_NotPlanar;
	}
	else
	{
		status = ChFi2d_Ready;
	}
}

//=======================================================================
//function : AddFillet
//purpose  : 
//=======================================================================

TopoDS_Edge BRepFilletAPI_MakeFillet2d_Extension::AddFillet(const TopoDS_Vertex& V, const Standard_Real Radius)
{
	TopTools_ListOfShape adjEdges;
    TopoDS_Edge adjEdge1, adjEdge2, basisEdge1, basisEdge2;
    TopoDS_Edge adjEdge1Mod, adjEdge2Mod, fillet;
    bool found = ShapeUtilities::FindConnectedEdges(newShape, V, adjEdges);
	if (!found || adjEdges.Size() != 2)
	{
		status = ChFi2d_ConnexionError;
		return fillet;
	}
	adjEdge1 = TopoDS::Edge(adjEdges.First());
	adjEdge2 = TopoDS::Edge(adjEdges.Last());

    ChFi2d_FilletAlgo filletBuilder;
    filletBuilder.Init(adjEdge1, adjEdge2, plane);
    filletBuilder.Perform(Radius);
    fillet = filletBuilder.Result(BRep_Tool::Pnt(V), adjEdge1Mod, adjEdge2Mod);

    if (fillet.IsNull())
    {
        status = ChFi2d_ComputationError;
        return fillet;
    }
    else
    {
        status = ChFi2d_IsDone;
    }

    if (status == ChFi2d_IsDone) {
        BuildNewWire(adjEdge1, adjEdge2, adjEdge1Mod, fillet, adjEdge2Mod);
        basisEdge1 = BasisEdge(adjEdge1);
        basisEdge2 = BasisEdge(adjEdge2);
        UpDateHistory(basisEdge1, basisEdge2,
            adjEdge1Mod, adjEdge2Mod, fillet, 1);
        status = ChFi2d_IsDone;
        return TopoDS::Edge(fillets.Value(fillets.Length()));
    }
    return fillet;
}

//=======================================================================
//function : AddChamfer
//purpose  : 
//=======================================================================

TopoDS_Edge BRepFilletAPI_MakeFillet2d_Extension::AddChamfer(const TopoDS_Vertex& V, const Standard_Real D1, const Standard_Real D2)
{
//     TopoDS_Vertex commonVertex;
	TopTools_ListOfShape adjEdges;
	TopoDS_Edge adjEdge1, adjEdge2, basisEdge1, basisEdge2;
	TopoDS_Edge E1Mod, E2Mod, chamfer;
	bool found = ShapeUtilities::FindConnectedEdges(newShape, V, adjEdges);
	if (!found || adjEdges.Size() != 2)
	{
		status = ChFi2d_ConnexionError;
		return chamfer;
	}
	adjEdge1 = TopoDS::Edge(adjEdges.First());
	adjEdge2 = TopoDS::Edge(adjEdges.Last());

// 
//     if (EE1.IsSame(E2)) {
//         TopAbs_Orientation orient = EE1.Orientation();
//         EE1 = EE2;
//         EE2 = E2;
//         EE2.Orientation(orient);
//     }

//     // EE1 and EE2 are copies of E1 and E2 with the good orientation
//     // on <newFace>
//     TopoDS_Edge EE1, EE2;
//     status = ChFi2d::FindConnectedEdges(newFace, commonVertex, EE1, EE2);
//     if (EE1.IsSame(E2)) {
//         TopAbs_Orientation orient = EE1.Orientation();
//         EE1 = EE2;
//         EE2 = E2;
//         EE2.Orientation(orient);
//     }

    ChFi2d_ChamferAPI chamferBuilder;
    chamferBuilder.Init(adjEdge1, adjEdge2);
    chamferBuilder.Perform();
    chamfer = chamferBuilder.Result(E1Mod, E2Mod, D1, D2);

    if (chamfer.IsNull())
    {
        status = ChFi2d_ComputationError;
        return chamfer;
    }
    else
    {
        status = ChFi2d_IsDone;
    }

    if (status == ChFi2d_IsDone) {
        //  if (status == ChFi2d_IsDone) {
        BuildNewWire(adjEdge1, adjEdge2, E1Mod, chamfer, E2Mod);
        basisEdge1 = BasisEdge(adjEdge1);
        basisEdge2 = BasisEdge(adjEdge2);
        UpDateHistory(basisEdge1, basisEdge2,
            E1Mod, E2Mod, chamfer, 2);
        status = ChFi2d_IsDone;
        return TopoDS::Edge(chamfers.Value(chamfers.Length()));
    }
    return chamfer;
}

//=======================================================================
//function : Build
//purpose  : 
//=======================================================================

void BRepFilletAPI_MakeFillet2d_Extension::BuildNewWire(const TopoDS_Edge& OldE1,
    const TopoDS_Edge& OldE2,
    const TopoDS_Edge& E1,
    const TopoDS_Edge& Fillet,
    const TopoDS_Edge& E2)
{

    Standard_Boolean aClosedStatus = Standard_True;

    TopExp_Explorer Ex(refShape, TopAbs_WIRE);
    while (Ex.More()) {
        const TopoDS_Wire& aWire = TopoDS::Wire(Ex.Current());
        aClosedStatus = aWire.Closed();
        break;
    }


    Standard_Boolean filletIsAdded = Standard_False;

    Ex.Init(newShape, TopAbs_EDGE);
    TopoDS_Wire newWire;
    BRep_Builder B;
    B.MakeWire(newWire);

    while (Ex.More()) {
        const TopoDS_Edge& theEdge = TopoDS::Edge(Ex.Current());
        if (!theEdge.IsSame(OldE1) && !theEdge.IsSame(OldE2)) {
            B.Add(newWire, theEdge);
        }
        else {
            if (theEdge == OldE1) {
                if (status != ChFi2d_FirstEdgeDegenerated
                    && status != ChFi2d_BothEdgesDegenerated) {
                    B.Add(newWire, E1);
                }
                if (!filletIsAdded) {
                    B.Add(newWire, Fillet);
                    filletIsAdded = Standard_True;
                } // if ( !filletIsAdded ...
            } // if (theEdge == ...
            else {
                if (status != ChFi2d_LastEdgeDegenerated
                    && status != ChFi2d_BothEdgesDegenerated) {
                    B.Add(newWire, E2);
                }
                if (!filletIsAdded) {
                    B.Add(newWire, Fillet);
                    filletIsAdded = Standard_True;
                }// if ( !filletIsAdded ...
            } // else ...
        } // else ...
        Ex.Next();
    } // while ...

    newWire.Closed(aClosedStatus);
//     BRepAdaptor_Surface Adaptor3dSurface(refFace);
//     BRepLib_MakeFace mFace(Adaptor3dSurface.Plane(), newWire);
// 	BRepAdaptor_Surface Adaptor3dSurface(refFace);
	if (faceMode)
	{
		BRepBuilderAPI_MakeFace mFace(newWire,true);
		newShape = mFace;
	}
	else
	{
		newShape = newWire;
	}

} // BuildNewWire

//=======================================================================
//function : BasisEdge
//purpose  : 
//=======================================================================
const TopoDS_Edge& BRepFilletAPI_MakeFillet2d_Extension::BasisEdge(const TopoDS_Edge& E) const
{
    TopTools_DataMapIteratorOfDataMapOfShapeShape iterator(history);
    TopoDS_Edge anEdge;
    while (iterator.More()) {
        anEdge = TopoDS::Edge(iterator.Value());
        if (anEdge.IsSame(E)) {
            const TopoDS_Edge& anotherEdge = TopoDS::Edge(iterator.Key());
            return anotherEdge;
        } // if (anEdge.IsSame ...
        iterator.Next();
    } // while (Iterator.More ...
    return E;
} // BasisEdge

void BRepFilletAPI_MakeFillet2d_Extension::UpDateHistory(const TopoDS_Edge& E1, const TopoDS_Edge& E2, const TopoDS_Edge& TrimE1, const TopoDS_Edge& TrimE2, const TopoDS_Edge& NewEdge, const Standard_Integer Id)
{
    if (Id == 1) // the new edge is a fillet
    {
        fillets.Append(NewEdge);
    }
    else // the new edge is a chamfer
    {
        chamfers.Append(NewEdge);
    }

    history.UnBind(E1);
    if (status != ChFi2d_FirstEdgeDegenerated
        && status != ChFi2d_BothEdgesDegenerated)
    {
        if (!E1.IsSame(TrimE1))
        {
            history.Bind(E1, TrimE1);
        }
    }
    history.UnBind(E2);
    if (status != ChFi2d_LastEdgeDegenerated
        && status != ChFi2d_BothEdgesDegenerated)
    {
        if (!E2.IsSame(TrimE2))
        {
            history.Bind(E2, TrimE2);
        }
    }
} // UpDateHistory

void BRepFilletAPI_MakeFillet2d_Extension::Build()
{
    // test if the operation is done
    if (status == ChFi2d_IsDone) {
        Done();
        myShape = newShape;
    }
    else
        NotDone();
}

//=======================================================================
//function : Modified
//purpose  : 
//=======================================================================

const TopTools_ListOfShape& BRepFilletAPI_MakeFillet2d_Extension::Modified(const TopoDS_Shape& E)
{
    myGenerated.Clear();
    myGenerated.Append(TopoDS::Edge(history.Find(E)));
    return myGenerated;
}

ChFi2d_ConstructionError BRepFilletAPI_MakeFillet2d_Extension::Status() const
{
    return status;
}

