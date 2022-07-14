// ****************************************************************************
//
// Copyright(C), 2020, XDU.
//
// Author: jHuang         Date: 2020/08/05
//
// ****************************************************************************

#pragma once

#include "xdModelingHeader.h"
#include "xdModelingDataType.h"

#include <string>
#include <vector>
#include <map>

// #include "xd.h"
#include "cmx.h"
using namespace std;

/** \Geom kernel Model
 *
 * This class contains some pure virtual functions to
 * implement the function of model.
 */

#ifdef OCCMODEL
#define TheGKApp TheOCCApp
#endif

#ifndef OCCMODEL
#define TheGKApp TheACISApp
#endif

class xdArchive;
typedef unsigned int    UINT;

class XDMODELING_API GKModel
{
public:
	GKModel() {};

	virtual ~GKModel() {};

	virtual bool findShapePID(void* shape, std::string& strPID) = 0;

	virtual void ImportGeomModel(FILE* fp, const string& fileName, int& iEntityNum, std::vector<void*>& entityList) = 0;

	virtual bool PrepareModel(const std::vector<void*>& inputEntList,/* const std::vector<int>& idList,*/
		std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp)
	{
		return false;
	}

	virtual void ExportModelFile(const std::vector<void*>& inputEntList, const std::vector<string>& entityNameList,
		FILE* fp) {};

	virtual void SaveModelArchive(const std::vector<void*>& inputEntList, xdArchive &ar) {};

	virtual void RestoreModelArchive(xdArchive &ar, std::vector<void*>& loadedEntList, std::vector<string>& entPIDList) {};

	virtual double GetTolerance()
	{
		return 0.0;
	}

	virtual std::string GetStringAttribute(void* pEntity, const std::string& attrName) 
	{
		return "";
	};

	virtual void BuildEntityAttributeName(void* orgBdy, void* curBdy, const string& bodyName,
		bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID) {}


	virtual bool GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue) { return false; }

	virtual bool GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue) { return false; }

	virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue, bool bNeedCopy = false) { return false; }

	virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const int attrValue, bool bNeedCopy = false) { return false; }

	virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName,
		const int attrValue) = 0;

	virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName) { return false; }

	virtual bool RemoveCellTopology(std::vector<void*> entity_list) { return false; }

	virtual bool DeleteBodies(std::vector<void*> entity_list) { return false; }

	virtual bool WriteSatFile(std::vector<void*> entity_list, FILE* file_prt) { return false; }

	virtual std::string GetEntityUuid(void* entity, int id = -1) { return std::string(); }

	virtual void* GetEntityByUuid(void* owner, std::string& strEntUuid) { return nullptr; }

	virtual void* GetEntityByUuid(void* entity, std::vector<std::string>& pidlist) { return nullptr; }

	virtual SELECTEDENTITYTYPE GetEntityType(const std::string& entUuid) { return SELECTEDENTITYTYPE::NotSelEntity; }

	virtual SELECTEDENTITYTYPE GetEntityType(void* entity) { return SELECTEDENTITYTYPE::NotSelEntity; }

	virtual int GetBodyMaterial(void* entity) { return 0; }

	virtual int GetFaceBoundaryCondition(void* entity) { return false; }

	virtual bool IsPeriodicBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
		std::vector<void*>& faceList) { return false; }

	virtual bool IsFEIEBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
		std::vector<void*>& faceList){ return false; }

	virtual std::string GetPointUuid(void* entity, const std::string& faceName, double u, double v) { return std::string(); }

	virtual std::string GetPointUuid(void* face, double u, double v) { return std::string(); }

	virtual std::string GetPointUuid(void* entity, const std::string& edgeName, double u) { return std::string(); }

	virtual std::string GetPointUuid(void* edge, double u) { return std::string(); }

	virtual bool GetEdgeLength(void* entity, double& minLength, double& maxLength) { return false; };

	virtual bool GetRelatedBody(void* vFace, std::vector<void*>& entList, std::vector<void*>& bodyList) { return false; }

	//virtual bool GetFaces(void* entity, std::vector<std::string>& faceUuidList) { return false; }

	virtual bool GetFaces(void* entity, std::vector<void*>& faceList) { return false; }

	//virtual bool GetEdges(void* entity, std::vector<std::string>& edgeUuidList) { return false; }

	virtual bool GetEdges(void* entity, std::vector<void*>& edgeList) { return false; }

	virtual bool GetEdgeList(void* entity, std::vector<void*>& edgeList) = 0;

	virtual bool GetVertexList(void* entity, std::vector<void*>& vertexList) = 0;

	virtual bool IsApproximatePlane(void* face, double tolAngle) { return false; }

	virtual bool CreatePML(void* box, int layerCount,
		bool joiningEdges, bool joiningVertices, std::vector<void*>& bodyList) { return false; }

	virtual bool GetPlaneType(void* face, int& planeType) { return false; }

	virtual bool GetFeaturePointCoordinates(void* face, std::vector<std::vector<double>>& pointCoords) { return false; };

	virtual std::string GetVertexNameFromCoordinate(void* owneEntity, double dX, double dY, double dZ) = 0;

	/** Get the relationship of the input point for the Air box on current model.
	*
	* @param dX: X coordinate value of input point
	* @param dY: Y coordinate value of input point
	* @param dZ: Z coordinate value of input point
	* @return true/false:Inside the air box/Outside the air box
	*/
	virtual bool GetPointRelationshipOfEntity(void* pEntity, double dX, double dY, double dZ) = 0;

	virtual BODYTYPE GetBodyType(void* entity) { return BODYTYPE::NotBody; };

	virtual bool GetEntityListBndBox(const std::vector<void*>& inputEntList,
		double& dMinX, double& dMinY, double& dMinZ,
		double& dMaxX, double& dMaxY, double& dMaxZ) = 0;

	virtual bool GetSharedEdges(std::vector<void*>& faceList, std::vector<void*>& edgeList) = 0;

	virtual void* GetOwnerEntity(void* pEntity) = 0;

	virtual void* CreateBoundaryBoxSolid(const std::vector<void*>& modelList, bool& bInsideBox) = 0;

	virtual bool GetEntityColorFromAttribute(void* pEntity, int& iR, int& iG, int& iB) = 0;

	virtual void SetRestoreFileCallbackFunc(void(*pCallbackFunc)(int, int)) = 0;

	virtual void GetGeomMeshData(void* pEntity, std::map<void*, cmtPoint>& vertPosMap,
		std::vector<std::vector<void*>>& meshDataList, std::vector<void*>& meshFaceDataList) = 0;

	virtual void CheckAndFixModel(void* pEntity, bool bFix) = 0;

	virtual bool GetPointParasiticEntityUuid(const std::string& strPtUuid, std::string& strEntUuid,
		std::string& strParams) = 0;

	virtual std::string GetPointUuid(const std::string& strEntUuid, const std::string& strParams) = 0;
};