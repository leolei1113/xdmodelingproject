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
#include "GKAlgo.h"
#include "cmx.h"
#include "xdModelingHeader.h"
#include "xdModelingDataType.h"

typedef unsigned int    UINT;

/** \Geometry kernel Algorithm
 *
 * This class contains some pure virtual functions to
 * implement the functions of geometry kernel algorithm.
 */
class XDMODELING_API OccAlgo :
	public GKAlgo
{
public:
	virtual ~OccAlgo() {};

	/*! \fn static OccAlgo* Instance()
	*  \brief 提供单例的OccAlgo指针.
	*/
	static OccAlgo* Instance();

	// query topo
	virtual bool GetOwner(void* ent, void*& entOwner);
	virtual bool GetType(void* ent, TOPOTYPE& type);
	virtual bool GetEntities(void* ent, TOPOTYPE type, std::vector<void*>& entitylist);
	virtual bool GetFaces(void* ent, std::vector<void*>& facelist);
	virtual bool GetEdges(void* ent, std::vector<void*>& edgelist);
	virtual bool GetWires(void* ent, std::vector<void*>& wirelist);
	virtual bool GetCoedges(void* ent, std::vector<void*>& coEdgelist);
	virtual bool GetVertices(void* ent, std::vector<void*>& vtxlist);
	virtual bool GetBox(void* ent, const cmtCS* wcs, cmtBox& box);
	virtual bool GetBox(const std::vector<void*>& entlist, const cmtCS* wcs, cmtBox& box);

	//attribute
	virtual bool GetEntityAttribute(void* entity, const std::string &attrName,
		std::string &attrValue);
	virtual bool GetEntityAttribute(void* entity, const std::string &attrName,
		int &attrValue);
	virtual bool AddEntityAttribute(void* entity, const std::string &attrName,
		const std::string &attrValue, bool bNeedCopy = false);
	virtual bool AddEntityAttribute(void* entity, const std::string &attrName,
		const int attrValue, bool bNeedCopy = false);
	virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName,
		const int attrValue);
	virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName);

	//edge
	virtual bool EdgeIsDegenerated(void* entEdge);
	virtual bool EdgeIsClosed(void* entEdge);
	virtual bool EdgeLength(void* entEdge, double &edgSegLen);
	virtual bool EdgeLength(void* entEdge, double start, double end, double &edgSegLen);
	virtual bool EdgeValue(void* entEdge, double t, cmtPoint &pt);
	virtual bool EdgeValue(void* entEdge, void* entFace, int edgeSense, double t, double& u, double& v);
	virtual bool EdgeProject(void* entEdge, const cmtPoint &pt, double tol, cmtPoint &cloestPt, double&t);
	virtual bool EdgeStartVertex(void* entEdge, void *& entVtx);
	virtual bool EdgeEndVertex(void* entEdge, void * &entVtx);
	virtual bool EdgeParamRange(void* entEdge, double &fstT, double &lstT);
	virtual bool EdgeUniformlyArcLengthParam(void* entEdge, double startParam, double arcLength, double &endParam);
	virtual bool EdgeMiddlePoint(void* entEdge, cmtPoint &pt);
	virtual bool EdgeStartDir(void* entEdge, cmtVector &dir);
	virtual bool EdgeEndDir(void* entEdge, cmtVector &dir);
	virtual bool EdgeMiddleDir(void* entEdge, cmtVector &dir);
	virtual bool EdgeTangent(void* entEdge, double t, cmtPoint &pt, cmtVector &dir);
	virtual bool EdgeCurvature(void* entEdge, double t, cmtPoint &pt, cmtVector &dir);
	virtual bool EdgeDiscretizeUniformly(void* entEdge, const UINT segNum, std::vector<double>& paraList);
	virtual bool EdgeSense(void* entEdge, int &coedgSense);

	virtual bool EdgeExtendLength(void* entEdge, double startParam, double len, void* &extEdg);
	virtual bool EdgeCreateFromPoints(const cmtPoint& fstPt, const cmtPoint& lstPt, void* &resEdg);
	virtual bool EdgeCreateFromParameters(void* entEdge, double startParam, double endParam, void* &extEdg);

	virtual bool CircleParamFrom3Pnt(const cmtPoint& cmtPnt1, const cmtPoint& cmtPnt2, const cmtPoint& cmtPnt3,
		cmtPoint& centerPnt, double& radius);

	virtual bool ArcSamplePoints(const cmtPoint& center, const cmtVector& normal,
		const cmtPoint& start, double radian, int pointnum, std::vector<cmtPoint>& outputpnts);


	//face
	virtual bool FaceIsLeftHanded(void* entFace);
	virtual bool FaceIsOnFace(void* entFace, const cmtPoint& point);
	virtual bool FaceIsOnFace(void* entFace, const cmtPoint& point, double u, double v); // given guessed UV for speed
	virtual bool CheckPntOnFace(void* entFace, const cmtPoint& point, double& u, double& v); // added by YAO
	virtual bool FaceValue(void* entFace, double u, double v, cmtPoint &pt);
	virtual bool FaceProject(void* entFace, const cmtPoint& pt, cmtPoint& cloestpt, double tol, double& u, double& v);
	virtual bool FaceNormal(void* entFace, double u, double v, cmtVector& norm, bool isConsiderSense = true);
	virtual bool FaceMiddleNormal(void* entFace, cmtVector& norm, bool isConsiderSense = true);
	virtual bool FaceTangents(void* entFace, double u, double v, cmtPoint& pt, cmtVector& du, cmtVector& dv);
	virtual bool FaceIsUClosed(void* entFace);
	virtual bool FaceIsVClosed(void* entFace);
	virtual bool FaceURange(void* entFace, double &start, double &end);
	virtual bool FaceVRange(void* entFace, double &start, double &end);
	virtual bool FaceUVRange(void* entFace, double &uStart, double &uEnd, double &vStart, double &vEnd);
	virtual bool FaceSense(void* entFace, int &facSense);
	virtual bool FaceArea(void* entFace, double &area);
	virtual bool FaceCurvature(void* entEdge, const cmtPoint &pt, const cmtVector &nrm, double &ptCurRad);

	virtual bool FaceCreateFromTwoLines(void *edg1, void *edg2, void* &resFac);
	virtual bool FaceCreateFromThreeLines(void *edg1, void *edg2, void *edg3, void* &resFac);
	virtual bool FaceCreateFromFourLines(void *edg1, void *edg2, void *edg3, void *edg4, void* &resFac);

	/************************************************************************/
	/*              can not be realized in occ                              */
	/************************************************************************/
	//coedge
	virtual bool CoedgeGetEdge(void* entCoedge, void* &entEdge);
	virtual bool CoedgeGetNextCoedge(void* entCoedge, void* &nextEntCoedge);
	virtual bool CoedgeSense(void* entCoedge, int &coedgSense);

	//loop
	virtual bool LoopGetFirstCoedge(void* entLoop, void* &entCoedge);
	virtual bool LoopGetType(void* entLoop, int &type);

	//vertex
	virtual bool VertexCoordinates(void *entVtx, cmtPoint &pt);
	virtual bool PointToShapeDistance(const cmtPoint &pt, void *ent, double &dist, cmtPoint &closePt);

	// common creation/modification functions
	virtual bool DeepCopyShape(void *ent, void *&cpEnt);
	virtual bool DeleteShapes(const std::vector<void*> &ownFacOrBdyLst);

	virtual bool ShapeToShapeDistance(void *ent1, void *ent2, double &dist, cmtPoint &closePt1, cmtPoint &closePt2);


	// calculate params


	// ****************************************************************************
	// Geometry Kernel Multi-Thread API Functions
	// ****************************************************************************
	virtual bool MultiThreadTurnOffHistory();
	virtual bool MultiThreadTurnOnHistory();

	virtual bool MultiThreadSafeRegionBegin();
	virtual bool MultiThreadSafeRegionEnd();

	virtual bool MultiThreadStartChildThread();
	virtual bool MultiThreadStopChildThread();

protected:
	OccAlgo() {};
};

