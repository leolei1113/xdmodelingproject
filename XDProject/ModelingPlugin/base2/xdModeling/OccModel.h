/*! \file OccModel.h
*   \brief OccModel���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef OCCMODEL_H
#define OCCMODEL_H

#include "xdModelingHeader.h"
#include "OccBasicTools.h"
#include "GKModel.h"
//#include "cmx.h"

#include <list>
#include <map>
#include <vector>

#define COMMAND_BEGIN \
    TheOCCApp.getCurrentModel()->BeginCommand();

#define COMMAND_END \
    bool cmdSuccess = TheOCCApp.getCurrentModel()->EndCommand();

class TDocStd_Document;
class OccModelData;
/*! \class OccModel
*   \brief OccModel��ͨ��OPENCASCADE�Ķ�̬���ṩģ�����ݹ������ط����������������������Ը�ֵ����ȡ�����ң�������Ϣ���ҡ�ɾ�����������������ȡ�
 */
class XDMODELING_API OccModel : public GKModel
{
public:
    /*! \fn OccModel(TDocStd_Document* doc)
    *  \brief OccModel�Ĺ��캯��.
    *  \param doc TDocStd_Document��ָ��.
    */
	OccModel(TDocStd_Document* doc);
    /*! \fn ~OccModel()
    *  \brief OccModel����������.
    */
	~OccModel();

    /************************************************************************/
    /*                    interface compatible with OCC                     */
    /************************************************************************/

    /*! \fn void undo()
    *  \brief ��������.
    */
    void undo();

    /*! \fn void redo()
    *  \brief ����������.
    */
    void redo();

    /*! \fn void updateShapeListOverUndo()
    *  \brief ���ڲ�����һ��undo�����б仯��ģ�����ݲ������Ա�и��£�Ŀǰ��ʱ���ã�.
    */
    void updateShapeListOverUndo();

    /*! \fn void updateShapeListOverRedo()
    *  \brief ���ڲ�����һ��redo�����б仯��ģ�����ݲ������Ա�и��£�Ŀǰ��ʱ���ã�.
    */
    void updateShapeListOverRedo();

    /*! \fn void addShapeToCacheByEntryOnSecondLevel(const std::string& strPID, void*& topEntity)
    *  \brief ���´�����ģ�ͽڵ���ص�ģ�����ݼ���.
    *  \param strPID ģ�ͽڵ��PID.
    *  \param topEntity ���ؽڵ��ģ��ָ�루TopoDS_HShape��.
    */
    void addShapeToCacheByEntryOnSecondLevel(const std::string& strPID, void*& topEntity);

    /*! \fn void deleteShapeToCacheByEntryOnSecond(const std::string& strPID)
    *  \brief ��ģ��������ɾ��PID��Ӧ������.
    *  \param strPID ģ�ͽڵ��PID.
    */
    void deleteShapeToCacheByEntryOnSecond(const std::string& strPID);

    /*! \fn void* findEntityFromCacheMap(const std::string& strPID)
    *  \brief �ӳ�Աmap�и���label�ı�ʶ���в��Ҳ���ȡģ��.
    *  \param strPID ģ�ͽڵ��PID.
    *  \return �ڵ��ģ��ָ�루TopoDS_HShape��.
    */
    void* findEntityFromCacheMap(const std::string& strPID);

    /*! \fn TDocStd_Document* getDoc() const
    *  \brief ��ȡ��ԱTDocStd_Documentָ�룬������ʱ�洢.
    *  \return ��ԱTDocStd_Documentָ��.
    */
    TDocStd_Document* getDoc() const;

    /*! \fn void setDoc(TDocStd_Document* curDoc)
    *  \brief ���ó�ԱTDocStd_Documentָ�루Ŀǰ��ʱ���ã�.
    *  \param curDoc TDocStd_Documentָ��.
    */
	void setDoc(TDocStd_Document* curDoc);

    /*! \fn std::list<void *> getReturnAddedShapeList() const
    *  \brief ���س�Ա����Ϊ��һ������������ģ������ָ��.
    *  \return TopoDS_HShapeָ������.
    */
    std::list<void *> getReturnAddedShapeList() const;

    /*! \fn void setReturnAddedShapeList(std::list<void *> val)
    *  \brief ���ó�Ա����Ϊ��һ������������ģ������ָ�루Ŀǰ��ʱ���ã�.
    *  \param val TopoDS_HShapeָ������.
    */
    void setReturnAddedShapeList(std::list<void *> val);

    /*! \fn std::list<void *> getReturnDeletedShapeList() const
    *  \brief ���س�Ա����Ϊ��һ������ɾ����ģ������ָ��.
    *  \return TopoDS_HShapeָ������.
    */
    std::list<void *> getReturnDeletedShapeList() const;

    /*! \fn void setReturnDeletedShapeList(std::list<void *> val)
    *  \brief ���ó�Ա����Ϊ��һ������ɾ����ģ������ָ�루Ŀǰ��ʱ���ã�.
    *  \param val TopoDS_HShapeָ������.
    */
    void setReturnDeletedShapeList(std::list<void *> val);

    /*! \fn void BeginCommand()
    *  \brief ģ�������ڵ��ø�������ʱ�������ʼ�ķ��������ڳ�����������.
    */
	void BeginCommand();

    /*! \fn void BeginCommand()
    *  \brief ģ�������ڵ��ø�������ʱ������������ķ��������ڳ�����������.
    */
    bool EndCommand();


    /************************************************************************/
    /*                    new interface consistent with acis                */
    /************************************************************************/
    /*! \fn virtual bool findShapePID(void* curEntity, std::string& strPID)
    *  \brief ����TopoDS_HShapeָ���������PID.
    *  \param curEntity TopoDS_HShapeָ��.
    *  \param strPID ���ز��ҵ���PID.
    */
    virtual bool findShapePID(void* curEntity, std::string& strPID);

    /*! \fn virtual void ImportGeomModel(FILE* fp, const string& fileName, int& iEntityNum, std::vector<void*>& entityList)
    *  \brief ����ģ���ļ�.
    *  \param fp �ļ����.
    *  \param iEntityNum ���ض�ȡ����ģ����Ŀ.
    *  \return entityList ���ض�ȡ����ģ��TopoDS_HShapeָ������.
    */
    virtual void ImportGeomModel(FILE* fp, const string& fileName, int& iEntityNum, std::vector<void*>& entityList);

	/*! \fn virtual bool PrepareModel(const std::vector<void*>& inputEntList, std::vector<void*>& OutEntList)
	*  \brief �ӿ�������.
	*  \param inputEntList ���н��зǹ��򲼶����㴦���ģ��TopoDS_HShape��ָ������.
	*  \param OutEntList ���ش���ģ�͵�TopoDS_HShape��ָ�룬ʵ����sizeΪ1.
	*  \return �Ƿ�ɹ����в���.
    */
	virtual bool PrepareModel(const std::vector<void*>& inputEntList,
		std::vector<void*>& OutEntList);

	/*! \fn virtual bool PrepareModel(const std::vector<void*>& inputEntList, const std::vector<int>& idList, std::vector<void*>& OutEntList)
	*  \brief �ӿ�������.
	*/
    virtual bool PrepareModel(const std::vector<void*>& inputEntList,
        const std::vector<int>& idList, std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp);

	/*! \fn virtual bool PrepareModel(const std::vector<void*>& inputEntList, std::vector<void*>& OutEntList)
	*  \brief �ǹ��򲼶����Լ��������Ը�ֵ�Ľӿ�.
	*  \param inputEntList ���н��зǹ��򲼶����㴦���ģ��TopoDS_HShape��ָ������.
	*  \param OutEntList ���ش���ģ�͵�TopoDS_HShape��ָ�룬ʵ����sizeΪ1.
	*  \return �Ƿ�ɹ����в���.
	*/
	virtual bool PrepareModel(const std::vector<void*>& inputEntList,/* const std::vector<int>& idList,*/
		std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp);

    /*! \fn virtual void ExportModelFile(const std::vector<void*>& inputEntList, const std::vector<string>& entityNameList, FILE* fp)
    *  \brief �ӿ�δʵ��.
    */
    virtual void ExportModelFile(const std::vector<void*>& inputEntList, const std::vector<string>& entityNameList,
        FILE* fp);

    /*! \fn virtual void RestoreModelArchive(xdArchive &ar, std::vector<void*>& loadedEntList, std::vector<string>& entPIDList)
    *  \brief ���ⲿ�ļ��ж�ȡ���ݵ�����.
    *  \param ar �ļ����.
    *  \param loadedEntList ���ض�ȡ����ģ��TopoDS_HShapeָ������.
    *  \param entPIDList ���ض�ȡ����ģ��PID����.
    */
    virtual void RestoreModelArchive(xdArchive &ar, std::vector<void*>& loadedEntList, std::vector<string>& entPIDList);

    /*! \fn virtual void SaveModelArchive(const std::vector<void*>& inputEntList, xdArchive &ar)
    *  \brief ���������ݱ�����ⲿ�ļ�.
    *  \param inputEntList ��Ҫ�����ģ��TopoDS_HShapeָ������.
    *  \param ar �ļ����.
    */
    virtual void SaveModelArchive(const std::vector<void*>& inputEntList, xdArchive &ar);

    /*! \fn double GetTolerance()
    *  \brief ��ȡ����.
    *  \return ��������.
    */
    virtual double GetTolerance();

    /*! \fn virtual std::string GetStringAttribute(void* pEntity, const std::string& attrName)
    *  \brief ��ȡģ��string��������ֵ.
    *  \param pEntity ģ��TopoDS_HShapeָ��.
    *  \param attrName ��������.
    *  \return ����ֵ.
    */
    virtual std::string GetStringAttribute(void* pEntity, const std::string& attrName);

    /*! \fn void BuildEntityAttributeName(void* orgBdy, void* curBdy, const std::string& bodyName, bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID)
    *  \brief Ϊģ�͸�ֵ�������ԣ��������ģ�;͹��������ƣ������ԭģ�ͽ����˱༭��̳�����.
    *  \param orgBdy ��ģ�ͻ���ԭģ��TopoDS_HShapeָ��.
    *  \param curBdy �༭��ģ��TopoDS_HShapeָ��.
    *  \param bodyName ģ������.
    *  \param bIsNewCreate ģ���Ƿ����½���.
    *  \param bIsCloneCS ģ���Ƿ��ǿ�����.
    *  \param bNeedCopyFaceName ģ������߱ߵ�����.
    *  \param iNextFaceID ģ�����ID.
    *  \param iNextEdgeID ģ�ͱߵ�ID.
    */
    virtual void BuildEntityAttributeName(void* orgBdy, void* curBdy, const std::string& bodyName,
        bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID);

    /*! \fn virtual bool GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue)
    *  \brief ��ȡģ��string��������ֵ.
    *  \param entity ģ��TopoDS_HShapeָ��.
    *  \param attrName ��������.
    *  \param attrValue ��������ֵ.
    *  \return �����Ƿ���ڸ�����ֵ.
    */
    virtual bool GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue);

    /*! \fn virtual bool GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue)
    *  \brief ��ȡģ��int��������ֵ.
    *  \param entity ģ��TopoDS_HShapeָ��.
    *  \param attrName ��������.
    *  \param attrValue ��������ֵ.
    *  \return �����Ƿ���ڸ�����ֵ.
    */
    virtual bool GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue);

    /*! \fn virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue)
    *  \brief Ϊģ�͸�string��������.
    *  \param entity ģ��TopoDS_HShapeָ��.
    *  \param attrName ��������.
    *  \param attrValue ����ֵ.
    *  \param bNeedCopy �Ƿ���и���.
    *  \return �����Ƿ�ɹ���ֵ.
    */
    virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue, bool bNeedCopy = false);

    /*! \fn virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue)
    *  \brief Ϊģ�͸�int��������.
    *  \param entity ģ��TopoDS_HShapeָ��.
    *  \param attrName ��������.
    *  \param attrValue ����ֵ.
    *  \param bNeedCopy �Ƿ���и���.
    *  \return �����Ƿ�ɹ���ֵ.
    */
    virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const int attrValue, bool bNeedCopy = false);

	/*! \fn virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName, const int attrValue)
	*  \brief ���Ҹ������Ƿ����.
	*  \param entity ģ��TopoDS_HShapeָ��.
	*  \param attrName ��������.
	*  \param attrValue ����ֵ.
	*  \return �����Ƿ����.
	*/
	virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName,
		const int attrValue);

    /*! \fn virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName)
    *  \brief ɾ��ģ������.
    *  \param entity ģ��TopoDS_HShapeָ��.
    *  \param attrName ��������.
    *  \return �����Ƿ�ɹ�ɾ��.
    */
    virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName);

    /*! \fn virtual bool RemoveCellTopology(std::vector<void*>& entity_list)
    *  \brief �ӿ�δʵ��.
    */
    virtual bool RemoveCellTopology(std::vector<void*>& entity_list);

    /*! \fn virtual bool DeleteBodies(std::vector<void*>& entity_list)
    *  \brief ����ɾ��ʵ��ģ��.
    *  \param entity_list ��Ҫɾ����ģ��TopoDS_HShapeָ������.
    *  \return �Ƿ�ɹ�ɾ��.
    */
    virtual bool DeleteBodies(std::vector<void*>& entity_list);

    /*! \fn virtual bool WriteSatFile(std::vector<void*>& entity_list, FILE* file_prt)
    *  \brief �ӿ�δʵ��.
    */
    virtual bool WriteSatFile(std::vector<void*>& entity_list, FILE* file_prt);

    /*! \fn virtual std::string GetEntityUuid(void* entity, int id)
    *  \brief ����ģ�����ݴ�������.
    *  \param entity ģ��TopoDS_HShapeָ��.
    *  \param id ģ��id.
    *  \return ������ģ������.
    */
    virtual std::string GetEntityUuid(void* entity, int id = -1);

    /*! \fn virtual void* GetEntityByUuid(void* entity, std::vector<std::string>& pidlist)
    *  \brief ����ģ�����ƽ����õ�ģ��PID���Ӷ��õ�ģ������.
    *  \param entity ģ��TopoDS_HShapeָ��.
    *  \param pidlist ģ��PID����.
    *  \return ���������ҵ���ģ��TopoDS_HShapeָ��.
    */
    virtual void* GetEntityByUuid(void* entity, std::vector<std::string>& pidlist);

	/*! \fn virtual void* GetEntityByUuid(void* owner, std::string& strEntUuid)
	*  \brief �ӿ�δʵ��.
	*/
	virtual void* GetEntityByUuid(void* owner, std::string& strEntUuid) { return nullptr; }

    /*! \fn virtual int GetBodyMaterial(void* entity)
    *  \brief �ӿ�δʵ��.
    */
    virtual int GetBodyMaterial(void* entity);

    /*! \fn virtual int GetFaceBoundaryCondition(void* entity)
    *  \brief �ӿ�δʵ��.
    */
    virtual int GetFaceBoundaryCondition(void* entity);

    /*! \fnvirtual bool IsPeriodicBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList, std::vector<void*>& faceList)
    *  \brief �ӿ�δʵ��.
    */
    virtual bool IsPeriodicBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
        std::vector<void*>& faceList);

    /*! \fn virtual bool IsFEIEBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList, std::vector<void*>& faceList)
    *  \brief �ӿ�δʵ��.
    */
    virtual bool IsFEIEBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
        std::vector<void*>& faceList);

    /*! \fn virtual std::string GetPointUuid(void* entity, const std::string& faceName, double u, double v)
    *  \brief �ӿ�δʵ��.
    */
    virtual std::string GetPointUuid(void* entity, const std::string& faceName, double u, double v);

    /*! \fn virtual std::string GetPointUuid(void* face, double u, double v)
    *  \brief �ӿ�δʵ��.
    */
    virtual std::string GetPointUuid(void* face, double u, double v);

    /*! \fn virtual std::string GetPointUuid(void* entity, const std::string& edgeName, double u)
    *  \brief �ӿ�δʵ��.
    */
    virtual std::string GetPointUuid(void* entity, const std::string& edgeName, double u);

    /*! \fn virtual std::string GetPointUuid(void* edge, double u)
    *  \brief �ӿ�δʵ��.
    */
    virtual std::string GetPointUuid(void* edge, double u);

    /*! \fn virtual bool GetEdgeLength(void* entity, double& minLength, double& maxLength)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetEdgeLength(void* entity, double& minLength, double& maxLength) { return false; };

    /*! \fn virtual bool GetRelatedBody(void* vFace, std::vector<void*>& entList, std::vector<void*>& bodyList)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetRelatedBody(void* vFace, std::vector<void*>& entList, std::vector<void*>& bodyList) { return false; }

    /*! \fn virtual bool GetFaces(void* entity, std::vector<void*>& faceList)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetFaces(void* entity, std::vector<void*>& faceList) { return false; }
	
    /*! \fn virtual bool GetFaces(void* entity, std::vector<std::string>& faceUuidList)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetFaces(void* entity, std::vector<std::string>& faceUuidList) { return false; }

	/*! \fn virtual bool GetEdgeList(void* entity, std::vector<void*>& edgeList)
	*  \brief �ӿ�δʵ��.
	*/
	virtual bool GetEdgeList(void* entity, std::vector<void*>& edgeList) { return false; };

	/*! \fn virtual bool GetVertexList(void* entity, std::vector<void*>& vertexList)
	*  \brief �ӿ�δʵ��.
	*/
	virtual bool GetVertexList(void* entity, std::vector<void*>& vertexList) { return false; };

    /*! \fn virtual bool IsApproximatePlane(void* face, double tolAngle)
    *  \brief �ӿ�δʵ��.
    */
    virtual bool IsApproximatePlane(void* face, double tolAngle) { return false; }

    /*! \fn virtual bool CreatePML(void* box, int layerCount, bool joiningEdges, bool joiningVertices, std::vector<void*>& bodyList)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool CreatePML(void* box, int layerCount,
		bool joiningEdges, bool joiningVertices, std::vector<void*>& bodyList) {
		return false;
	}

    /*! \fn virtual bool GetPlaneType(void* face, int& planeType)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetPlaneType(void* face, int& planeType) { return false; }

    /*! \fn virtual bool GetFeaturePointCoordinates(void* face, std::vector<std::vector<double>>& pointCoords)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetFeaturePointCoordinates(void* face, std::vector<std::vector<double>>& pointCoords) { return false; };

    /*! \fn virtual std::string GetVertexNameFromCoordinate(void* owneEntity, double dX, double dY, double dZ)
    *  \brief �ӿ�δʵ��.
    */
	virtual std::string GetVertexNameFromCoordinate(void* owneEntity, double dX, double dY, double dZ) { return "";  }

    /*! \fn virtual bool GetPointRelationshipOfEntity(void* pEntity, double dX, double dY, double dZ)
    *  \brief �ӿ�δʵ��.
    */
    virtual bool GetPointRelationshipOfEntity(void* pEntity, double dX, double dY, double dZ) { return false; }

    /*! \fn virtual BODYTYPE GetBodyType(void* entity)
    *  \brief �ӿ�δʵ��.
    */
	virtual BODYTYPE GetBodyType(void* entity) { return SheetBody; }

    /*! \fn virtual bool IsClosedWireBody(void* entity)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool IsClosedWireBody(void* entity) { return false; }

    /*! \fn virtual bool GetEntityListBndBox(const std::vector<void*>& inputEntList, double& dMinX, double& dMinY, double& dMinZ, double& dMaxX, double& dMaxY, double& dMaxZ)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetEntityListBndBox(const std::vector<void*>& inputEntList,
		double& dMinX, double& dMinY, double& dMinZ,
		double& dMaxX, double& dMaxY, double& dMaxZ) 
	{
		return false;
	}

    /*! \fn virtual bool GetSharedEdges(std::vector<void*>& faceList, std::vector<void*>& edgeList)
    *  \brief �ӿ�δʵ��.
    */
	virtual bool GetSharedEdges(std::vector<void*>& faceList, std::vector<void*>& edgeList) { return false;  }

	/*! \fn virtual void* GetOwnerEntity(void* pEntity)
	*  \brief ��ȡ������ģ�͵Ķ�����������.
	*  \param pEntity ��Ҫ���ҵ�ģ��TopoDS_HShapeָ��.
	*  \return �ҵ��ĵײ���������(TopoDS_HShapeָ��).
	*/
	virtual void* GetOwnerEntity(void* pEntity);

	/*! \fn virtual void* CreateBoundaryBoxSolid(const std::vector<void*>& modelList, bool& bInsideBox)
	*  \brief �ӿ�δʵ��.
	*/
	virtual void* CreateBoundaryBoxSolid(const std::vector<void*>& modelList, bool& bInsideBox) { return nullptr; }

	/*! \fn virtual bool GetEntityColorFromAttribute(void* pEntity, int& iR, int& iG, int& iB)
	*  \brief �ӿ�δʵ��.
	*/
	virtual bool GetEntityColorFromAttribute(void* pEntity, int& iR, int& iG, int& iB) { return false; };

	/*! \fn virtual void SetRestoreFileCallbackFunc(void(*pCallbackFunc)(int, int))
	*  \brief �ӿ�δʵ��.
	*/
	virtual void SetRestoreFileCallbackFunc(void(*pCallbackFunc)(int, int)) {};

	/*! \fn virtual void GetGeomMeshData(void* pEntity, std::map<void*, cmtPoint>& vertPosMap, std::vector<std::vector<void*>>& meshDataList, std::vector<void*>& meshFaceDataList)
	*  \brief �ӿ�δʵ��.
	*/
	virtual void GetGeomMeshData(void* pEntity, std::map<void*, cmtPoint>& vertPosMap,
		std::vector<std::vector<void*>>& meshDataList, std::vector<void*>& meshFaceDataList) {};

	/*! \fn virtual void CheckAndFixModel(void* pEntity, bool bFix)
	*  \brief �ӿ�δʵ��.
	*/
	virtual void CheckAndFixModel(void* pEntity, bool bFix) {};

	/*! \fn virtual bool GetPointParasiticEntityUuid(const std::string& strPtUuid, std::string& strEntUuid, std::string& strParams)
	*  \brief �ӿ�δʵ��.
	*/
	virtual bool GetPointParasiticEntityUuid(const std::string& strPtUuid, std::string& strEntUuid,
		std::string& strParams) {
		return false;
	}

	/*! \fn virtual std::string GetPointUuid(const std::string& strEntUuid, const std::string& strParams)
	*  \brief �ӿ�δʵ��.
	*/
	virtual std::string GetPointUuid(const std::string& strEntUuid, const std::string& strParams) { return ""; };
protected:
    /*! \fn bool getPIDProperty(const std::string& strPID, const std::string& propertyName, int& value)
    *  \brief �����������ƺ�PID���в��Ҳ���ȡint������.
    *  \param strPID ģ������PID.
    *  \param propertyName ��������.
    *  \param value ���ص�int������ֵ.
    *  \return �Ƿ���ҵ�����.
    */
    bool getPIDProperty(const std::string& strPID, const std::string& propertyName, int& value);

    /*! \fn bool getPIDProperty(const std::string& strPID, const std::string& propertyName, std::string& value)
    *  \brief �����������ƺ�PID���в��Ҳ���ȡstd::string������.
    *  \param strPID ģ������PID.
    *  \param propertyName ��������.
    *  \param value ���ص�std::string������ֵ.
    *  \return �Ƿ���ҵ�����.
    */
    bool getPIDProperty(const std::string& strPID, const std::string& propertyName, std::string& value);

    /*! \fn bool getPIDProperty(const std::string& strPID, const std::string& propertyName, double& value)
    *  \brief �����������ƺ�PID���в��Ҳ���ȡdouble������.
    *  \param strPID ģ������PID.
    *  \param propertyName ��������.
    *  \param value ���ص�double������ֵ.
    *  \return �Ƿ���ҵ�����.
    */
    bool getPIDProperty(const std::string& strPID, const std::string& propertyName, double& value);

    /*! \fn bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const int& value)
    *  \brief �����������ƺ�PID����ģ�����ݲ��Ҳ����int������.
    *  \param strPID ģ������PID.
    *  \param propertyName ��������.
    *  \param value ���ص�int������ֵ.
    *  \return �Ƿ���ҵ�����.
    */
    bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const int& value);

    /*! \fn bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const std::string& value)
    *  \brief �����������ƺ�PID����ģ�����ݲ��Ҳ����std::string������.
    *  \param strPID ģ������PID.
    *  \param propertyName ��������.
    *  \param value ���ص�std::string������ֵ.
    *  \return �Ƿ���ҵ�����.
    */
    bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const std::string& value);

    /*! \fn bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const double& value)
    *  \brief �����������ƺ�PID����ģ�����ݲ��Ҳ����double������.
    *  \param strPID ģ������PID.
    *  \param propertyName ��������.
    *  \param value ���ص�double������ֵ.
    *  \return �Ƿ���ҵ�����.
    */
    bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const double& value);

    /*! \fn bool removeLabelProperty(const std::string& strPID, const std::string& propertyName)
    *  \brief �����������ƺ�PID�ı�ʶɾ������.
    *  \param strPID ģ������PID.
    *  \param propertyName ��������.
    *  \return �Ƿ���ҵ�����.
    */
    bool removeLabelProperty(const std::string& strPID, const std::string& propertyName);

    bool Heal(const std::map<void*, std::string> entnamemap, 
        std::vector<void*>& outentlist, HealOption params, std::string& m_err);

    bool RemoveHolesWithinTolerance(double maxradius);

protected:
    OccModelData*       m_data;
};

#endif