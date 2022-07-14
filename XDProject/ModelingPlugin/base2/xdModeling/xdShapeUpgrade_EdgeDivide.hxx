// Created on: 1999-07-22
// Created by: data exchange team
// Copyright (c) 1999 Matra Datavision
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

#ifndef _xdShapeUpgrade_EdgeDivide_HeaderFile
#define _xdShapeUpgrade_EdgeDivide_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <ShapeUpgrade_EdgeDivide.hxx>
#include <Standard_Boolean.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <Standard_Real.hxx>
#include <TopTools_ShapeMapHasher.hxx>
#include <NCollection_DataMap.hxx>

#include <Vector>

enum EnSplitMode
{
	EnSplitPoint,
	EnRatio,
	EnAverage,
	EnDiscretePoints,
};

struct SplitEdgeInfo
{
	TopoDS_Edge edge;
	EnSplitMode mode;
	double normalizedT;
	double ratio;
	bool reverse;
	int segNum;
	std::vector<double> normTVec;
	SplitEdgeInfo()
	{
		normalizedT=0.5;
		ratio=1.0;
		segNum=2;
	}
};

typedef NCollection_DataMap<TopoDS_Shape,SplitEdgeInfo,TopTools_ShapeMapHasher> TopTools_DataMapOfShapeSplitEdgeInfo;

class xdShapeUpgrade_EdgeDivide
DEFINE_STANDARD_HANDLE(xdShapeUpgrade_EdgeDivide, ShapeUpgrade_EdgeDivide）
//! Divides all closed faces in the shape. Class
//! ShapeUpgrade_ClosedFaceDivide is used as divide tool.
class xdShapeUpgrade_EdgeDivide  : public ShapeUpgrade_EdgeDivide
{
public:

  Standard_EXPORT xdShapeUpgrade_EdgeDivide();
  
  //used for insert single pointer
  bool Add(const TopoDS_Edge& edge, const double& normalizedT);
  
  bool Add(const TopoDS_Edge& edge, const double& ratio, const int& segNum,
  const bool& reverse);

  bool Add(const TopoDS_Edge& edge, const int& segNum);

  bool Add(const TopoDS_Edge& edge, const std::vector<double>& normTVec);
  
  Standard_EXPORT virtual Standard_Boolean Compute (const TopoDS_Edge& anEdge) Standard_OVERRIDE;
  
  DEFINE_STANDARD_RTTIEXT(xdShapeUpgrade_EdgeDivide, ShapeUpgrade_EdgeDivide)
protected:
  void CalculateParameters(Handle(TColStd_HSequenceOfReal) values, const SplitEdgeInfo& info, const double& startT,
  const double& endT);

  TopTools_DataMapOfShapeSplitEdgeInfo m_edgeMaps;


private:





};







#endif // _ShapeUpgrade_ShapeDivideClosed_HeaderFile
