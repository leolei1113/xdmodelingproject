// ****************************************************************************
//
// Copyright(C), 2020, XDU.
//
// Author: jHuang         Date: 2020/08/05
//
// ****************************************************************************

#pragma once

#include "GKModel.h"

class SPA_progress_info;

class XDMODELING_API AcisModel :
	public GKModel
{
public:
	AcisModel();

	virtual ~AcisModel();

	virtual bool findShapePID(void* curEntity, std::string& strPID);

	/** Import geometry file
	 *
	 * @param fp:			(Input)Import file pointer
	 * @param iEntityNum:	(Output)The number of imported entities
	 * @param entityList:	(Output)All the imported entities
	 * @return void
	 */
	virtual void ImportGeomModel(FILE* fp, const string& fileName, int& iEntityNum, std::vector<void*>& entityList);

	/** Prepare mode for mesh, do the no-reg unite for all the model in database.
	 *
	 * @param inputEntList:		(Input)All the entity in database
	 * @param idxList:			(Input)The CS index of entity
	 * @param OutEntList:		(Output)the no-reg unite result entities
	 * @return bool:			Success or Failure.
	 */
	virtual bool PrepareModel(const std::vector<void*>& inputEntList,
		const std::vector<int>& idList, std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp);

	virtual void ExportModelFile(const std::vector<void*>& inputEntList, const std::vector<string>& entityNameList,
		FILE* fp);

	virtual void RestoreModelArchive(xdArchive &ar, std::vector<void*>& loadedEntList, std::vector<string>& entPIDList);

	virtual void SaveModelArchive(const std::vector<void*>& inputEntList, xdArchive &ar);

	virtual double GetTolerance();

	virtual std::string GetStringAttribute(void* pEntity, const std::string& attrName);

	virtual void BuildEntityAttributeName(void* orgBdy, void* curBdy, const string& bodyName,
		bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID);


	virtual bool GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue);

	virtual bool GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue);

	virtual bool AddEntityAttribute(void* entity, const std::string &attrName,
		const std::string &attrValue, bool bNeedCopy = false);

	virtual bool AddEntityAttribute(void* entity, const std::string &attrName,
		const int attrValue, bool bNeedCopy = false);

	virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName,
		const int attrValue);

	virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName);

	virtual bool RemoveCellTopology(std::vector<void*>& entity_list);

	virtual bool DeleteBodies(std::vector<void*>& entity_list);

	virtual bool WriteSatFile(std::vector<void*>& entity_list, FILE* file_prt);

	virtual std::string GetEntityUuid(void* entity, int id = -1);

	virtual void* GetEntityByUuid(void* owner, std::string& strEntUuid);

	// NOTE: 下个版本弃用，用GetEntityByUuid(void* owner, std::string& strEntUuid)代替
	virtual void* GetEntityByUuid(void* owner, std::vector<std::string>& pidlist);

	virtual SELECTEDENTITYTYPE GetEntityType(const std::string& entUuid);

	virtual SELECTEDENTITYTYPE GetEntityType(void* entity);


	virtual int GetBodyMaterial(void* entity);

	virtual int GetFaceBoundaryCondition(void* entity);

	virtual bool IsPeriodicBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
		std::vector<void*>& faceList);

	virtual bool IsFEIEBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
		std::vector<void*>& faceList);

	virtual std::string GetPointUuid(void* entity, const std::string& faceName, double u, double v);

	virtual std::string GetPointUuid(void* face, double u, double v);
	
	virtual std::string GetPointUuid(void* entity, const std::string& edgeName, double u);

	virtual std::string GetPointUuid(void* edge, double u);

	virtual bool GetEdgeLength(void* entity, double& minLength, double& maxLength);

	virtual bool GetRelatedBody(void* vFace, std::vector<void*>& entList, std::vector<void*>& bodyList);

	//virtual bool GetFaces(void* entity, std::vector<std::string>& faceUuidList);

	virtual bool GetFaces(void* entity, std::vector<void*>& faceList);

	//virtual bool GetEdges(void* entity, std::vector<std::string>& edgeUuidList);

	virtual bool GetEdges(void* entity, std::vector<void*>& edgeList);

	virtual bool GetEdgeList(void* entity, std::vector<void*>& edgeList);

	virtual bool GetVertexList(void* entity, std::vector<void*>& vertexList);

	virtual bool IsApproximatePlane(void* face, double tolAngle);

	virtual bool CreatePML(void* box, int layerCount,
		bool joiningEdges, bool joiningVertices, std::vector<void*>& bodyList);

	virtual bool GetPlaneType(void* face, int& planeType);

	virtual bool GetFeaturePointCoordinates(void* face, std::vector<std::vector<double>>& pointCoords);

	virtual std::string GetVertexNameFromCoordinate(void* owneEntity, double dX, double dY, double dZ);

	virtual bool GetPointRelationshipOfEntity(void* pEntity, double dX, double dY, double dZ);

	virtual BODYTYPE GetBodyType(void* entity);

	virtual bool IsClosedWireBody(void* entity);

	virtual bool GetEntityListBndBox(const std::vector<void*>& inputEntList,
		double& dMinX, double& dMinY, double& dMinZ,
		double& dMaxX, double& dMaxY, double& dMaxZ);

	virtual bool GetSharedEdges(std::vector<void*>& faceList, std::vector<void*>& edgeList);

	virtual void* GetOwnerEntity(void* pEntity);

	virtual void* CreateBoundaryBoxSolid(const std::vector<void*>& modelList, bool& bInsideBox);

	virtual bool GetEntityColorFromAttribute(void* pEntity, int& iR, int& iG, int& iB);

	virtual void SetRestoreFileCallbackFunc(void(*pCallbackFunc)(int, int));

	virtual void GetGeomMeshData(void* pEntity, std::map<void*, cmtPoint>& vertPosMap,
		std::vector<std::vector<void*>>& meshDataList, std::vector<void*>& meshFaceDataList);

	virtual void CheckAndFixModel(void* pEntity, bool bFix);

	virtual bool GetPointParasiticEntityUuid(const std::string& strPtUuid, std::string& strEntUuid,
		std::string& strParams);

	virtual std::string GetPointUuid(const std::string& strEntUuid, const std::string& strParams);

public:
	void(*m_pRestoreFileCallbackFunc)(int, int);
};

