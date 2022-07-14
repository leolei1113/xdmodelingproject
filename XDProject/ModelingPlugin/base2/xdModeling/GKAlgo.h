//------------------------------------------------------------------------------
//
//   Copyright (c) 2020 Xidian University
//
//   Author : YafeiLiu
//   Date   : 2020.11.19
//
//------------------------------------------------------------------------------
#pragma once
#include <vector>
#include <string>
#include "cmx.h"
#include "xdModelingHeader.h"
#include "xdModelingDataType.h"

typedef unsigned int    UINT;

/** \Geometry kernel Algorithm
 *
 * This class contains some pure virtual functions to
 * implement the functions of geometry kernel algorithm.
 */
class XDMODELING_API GKAlgo
{
public:
	virtual ~GKAlgo() {};
	GKAlgo() {};

	// query topo
	virtual bool GetOwner(void* ent, void*& entOwner) = 0;
	virtual bool GetType(void* ent, TOPOTYPE& type) = 0;
	virtual bool GetEntities(void* ent, TOPOTYPE type, std::vector<void*>& entitylist) = 0;
	virtual bool GetFaces(void* ent, std::vector<void*>& facelist) = 0;
	virtual bool GetEdges(void* ent, std::vector<void*>& edgelist) = 0;
	virtual bool GetWires(void* ent, std::vector<void*>& wirelist) = 0;
	virtual bool GetCoedges(void* ent, std::vector<void*>& coEdgelist) = 0;
	virtual bool GetVertices(void* ent, std::vector<void*>& vtxlist) = 0;
	virtual bool GetBox(void* ent, const cmtCS* wcs, cmtBox& box) = 0;
	virtual bool GetBox(const std::vector<void*>& entlist, const cmtCS* wcs, cmtBox& box) = 0;

	//attribute
	virtual bool GetEntityAttribute(void* entity, const std::string &attrName,
		std::string &attrValue) = 0;
	virtual bool GetEntityAttribute(void* entity, const std::string &attrName,
		int &attrValue) = 0;
	virtual bool AddEntityAttribute(void* entity, const std::string &attrName,
		const std::string &attrValue, bool bNeedCopy = false) = 0;
	virtual bool AddEntityAttribute(void* entity, const std::string &attrName,
		const int attrValue, bool bNeedCopy = false) = 0;
	virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName,
		const int attrValue) = 0;
	virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName) = 0;

	//edge
	virtual bool EdgeIsDegenerated(void* entEdge) = 0;
	virtual bool EdgeIsClosed(void* entEdge) = 0;
    virtual bool EdgeLength(void* entEdge, double &edgSegLen) = 0;
	virtual bool EdgeLength(void* entEdge, double start, double end, double &edgSegLen) = 0;
	virtual bool EdgeValue(void* entEdge, double t, cmtPoint &pt) = 0;
	virtual bool EdgeValue(void* entEdge, void* entFace, int edgeSense, double t, double& u, double& v) = 0;
	virtual bool EdgeProject(void* entEdge, const cmtPoint &pt, double tol, cmtPoint &cloestPt, double&t) = 0;
	virtual bool EdgeStartVertex(void* entEdge, void *& entVtx) = 0;
	virtual bool EdgeEndVertex(void* entEdge, void * &entVtx) = 0;
	virtual bool EdgeParamRange(void* entEdge, double &fstT, double &lstT) = 0;
	virtual bool EdgeUniformlyArcLengthParam(void* entEdge, double startParam, double arcLength, double &endParam) = 0;
	virtual bool EdgeMiddlePoint(void* entEdge, cmtPoint &pt) = 0;
	virtual bool EdgeStartDir(void* entEdge, cmtVector &dir) = 0;
	virtual bool EdgeEndDir(void* entEdge, cmtVector &dir) = 0;
	virtual bool EdgeMiddleDir(void* entEdge, cmtVector &dir) = 0;
	virtual bool EdgeTangent(void* entEdge, double t, cmtPoint &pt, cmtVector &dir) = 0;
	virtual bool EdgeCurvature(void* entEdge, double t, cmtPoint &pt, cmtVector &dir) = 0;
	virtual bool EdgeDiscretizeUniformly(void* entEdge, const UINT segNum, std::vector<double>& paraList) = 0;
	virtual bool EdgeSense(void* entEdge, int &coedgSense) = 0;

    virtual bool EdgeExtendLength(void* entEdge, double startParam, double len, void* &extEdg) = 0;
    virtual bool EdgeCreateFromPoints(const cmtPoint& fstPt, const cmtPoint& lstPt, void* &resEdg) = 0;
    virtual bool EdgeCreateFromParameters(void* entEdge, double startParam, double endParam, void* &extEdg) = 0;

    virtual bool CircleParamFrom3Pnt(const cmtPoint& cmtPnt1, const cmtPoint& cmtPnt2, const cmtPoint& cmtPnt3,
        cmtPoint& centerPnt, double& radius) = 0;

    virtual bool ArcSamplePoints(const cmtPoint& center, const cmtVector& normal, 
        const cmtPoint& start, double radian, int pointnum, std::vector<cmtPoint>& outputpnts) = 0;


	//face
	virtual bool FaceIsLeftHanded(void* entFace) = 0;
	virtual bool FaceIsOnFace(void* entFace, const cmtPoint& point) = 0;
	virtual bool FaceIsOnFace(void* entFace, const cmtPoint& point, double u, double v) = 0; // given guessed UV for speed
	virtual bool FaceValue(void* entFace, double u, double v, cmtPoint &pt) = 0;
	virtual bool FaceProject(void* entFace, const cmtPoint& pt, cmtPoint& cloestpt, double tol, double& u, double& v) = 0;
	virtual bool FaceNormal(void* entFace, double u, double v, cmtVector& norm, bool isConsiderSense = true) = 0;
	virtual bool FaceMiddleNormal(void* entFace, cmtVector& norm, bool isConsiderSense = true) = 0;
	virtual bool FaceTangents(void* entFace, double u, double v, cmtPoint& pt, cmtVector& du, cmtVector& dv) = 0;
	virtual bool FaceIsUClosed(void* entFace) = 0;
	virtual bool FaceIsVClosed(void* entFace) = 0;
	virtual bool FaceURange(void* entFace, double &start, double &end) = 0;
	virtual bool FaceVRange(void* entFace, double &start, double &end) = 0;
	virtual bool FaceUVRange(void* entFace, double &uStart, double &uEnd, double &vStart, double &vEnd) = 0;
	virtual bool FaceSense(void* entFace, int &facSense) = 0;
	virtual bool FaceArea(void* entFace, double &area) = 0;
    virtual bool FaceCurvature(void* entEdge, const cmtPoint &pt, const cmtVector &nrm, double &ptCurRad) = 0;

    virtual bool FaceCreateFromTwoLines(void *edg1, void *edg2, void* &resFac) = 0;
    virtual bool FaceCreateFromThreeLines(void *edg1, void *edg2, void *edg3, void* &resFac) = 0;
    virtual bool FaceCreateFromFourLines(void *edg1, void *edg2, void *edg3, void *edg4, void* &resFac) = 0;

	//coedge
	virtual bool CoedgeGetEdge(void* entCoedge, void* &entEdge) = 0;
	virtual bool CoedgeGetNextCoedge(void* entCoedge, void* &nextEntCoedge) = 0;
	virtual bool CoedgeSense(void* entCoedge, int &coedgSense) = 0;

	//loop
	virtual bool LoopGetFirstCoedge(void* entLoop, void* &entCoedge) = 0;
	virtual bool LoopGetType(void* entLoop, int &type) = 0;

	//vertex
	virtual bool VertexCoordinates(void *entVtx, cmtPoint &pt) = 0;
    virtual bool PointToShapeDistance(const cmtPoint &pt, void *ent, double &dist, cmtPoint &closePt) = 0;

    // common creation/modification functions
    virtual bool DeepCopyShape(void *ent, void *&cpEnt) = 0;
    virtual bool DeleteShapes(const std::vector<void*> &ownFacOrBdyLst) = 0;

    virtual bool ShapeToShapeDistance(void *ent1, void *ent2, double &dist, cmtPoint &closePt1, cmtPoint &closePt2) = 0;


	// calculate params


    // ****************************************************************************
    // Geometry Kernel Multi-Thread API Functions
    // ****************************************************************************
    virtual bool MultiThreadTurnOffHistory() = 0;
    virtual bool MultiThreadTurnOnHistory() = 0;

    virtual bool MultiThreadSafeRegionBegin() = 0;
    virtual bool MultiThreadSafeRegionEnd() = 0;

    virtual bool MultiThreadStartChildThread() = 0;
    virtual bool MultiThreadStopChildThread() = 0;


};

