/*! \file OccModel.h
*   \brief OccModel类的头文件
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
*   \brief OccModel类通过OPENCASCADE的动态库提供模型数据管理的相关方法。包括数据流化，属性赋值、获取、查找，拓扑信息查找、删除，撤销、反撤销等。
 */
class XDMODELING_API OccModel : public GKModel
{
public:
    /*! \fn OccModel(TDocStd_Document* doc)
    *  \brief OccModel的构造函数.
    *  \param doc TDocStd_Document的指针.
    */
	OccModel(TDocStd_Document* doc);
    /*! \fn ~OccModel()
    *  \brief OccModel的析构函数.
    */
	~OccModel();

    /************************************************************************/
    /*                    interface compatible with OCC                     */
    /************************************************************************/

    /*! \fn void undo()
    *  \brief 撤销操作.
    */
    void undo();

    /*! \fn void redo()
    *  \brief 反撤销操作.
    */
    void redo();

    /*! \fn void updateShapeListOverUndo()
    *  \brief 用于查找上一个undo环节中变化的模型数据并在类成员中更新（目前暂时弃用）.
    */
    void updateShapeListOverUndo();

    /*! \fn void updateShapeListOverRedo()
    *  \brief 用于查找上一个redo环节中变化的模型数据并在类成员中更新（目前暂时弃用）.
    */
    void updateShapeListOverRedo();

    /*! \fn void addShapeToCacheByEntryOnSecondLevel(const std::string& strPID, void*& topEntity)
    *  \brief 将新创建的模型节点挂载到模型数据集中.
    *  \param strPID 模型节点的PID.
    *  \param topEntity 返回节点的模型指针（TopoDS_HShape）.
    */
    void addShapeToCacheByEntryOnSecondLevel(const std::string& strPID, void*& topEntity);

    /*! \fn void deleteShapeToCacheByEntryOnSecond(const std::string& strPID)
    *  \brief 在模型数据中删除PID对应的数据.
    *  \param strPID 模型节点的PID.
    */
    void deleteShapeToCacheByEntryOnSecond(const std::string& strPID);

    /*! \fn void* findEntityFromCacheMap(const std::string& strPID)
    *  \brief 从成员map中根据label的标识进行查找并获取模型.
    *  \param strPID 模型节点的PID.
    *  \return 节点的模型指针（TopoDS_HShape）.
    */
    void* findEntityFromCacheMap(const std::string& strPID);

    /*! \fn TDocStd_Document* getDoc() const
    *  \brief 获取成员TDocStd_Document指针，构造类时存储.
    *  \return 成员TDocStd_Document指针.
    */
    TDocStd_Document* getDoc() const;

    /*! \fn void setDoc(TDocStd_Document* curDoc)
    *  \brief 设置成员TDocStd_Document指针（目前暂时弃用）.
    *  \param curDoc TDocStd_Document指针.
    */
	void setDoc(TDocStd_Document* curDoc);

    /*! \fn std::list<void *> getReturnAddedShapeList() const
    *  \brief 返回成员中因为上一步命令新增的模型数据指针.
    *  \return TopoDS_HShape指针链表.
    */
    std::list<void *> getReturnAddedShapeList() const;

    /*! \fn void setReturnAddedShapeList(std::list<void *> val)
    *  \brief 设置成员中因为上一步命令新增的模型数据指针（目前暂时弃用）.
    *  \param val TopoDS_HShape指针链表.
    */
    void setReturnAddedShapeList(std::list<void *> val);

    /*! \fn std::list<void *> getReturnDeletedShapeList() const
    *  \brief 返回成员中因为上一步命令删除的模型数据指针.
    *  \return TopoDS_HShape指针链表.
    */
    std::list<void *> getReturnDeletedShapeList() const;

    /*! \fn void setReturnDeletedShapeList(std::list<void *> val)
    *  \brief 设置成员中因为上一步命令删除的模型数据指针（目前暂时弃用）.
    *  \param val TopoDS_HShape指针链表.
    */
    void setReturnDeletedShapeList(std::list<void *> val);

    /*! \fn void BeginCommand()
    *  \brief 模型数据在调用各个方法时标明命令开始的方法，用于撤销、反撤销.
    */
	void BeginCommand();

    /*! \fn void BeginCommand()
    *  \brief 模型数据在调用各个方法时标明命令结束的方法，用于撤销、反撤销.
    */
    bool EndCommand();


    /************************************************************************/
    /*                    new interface consistent with acis                */
    /************************************************************************/
    /*! \fn virtual bool findShapePID(void* curEntity, std::string& strPID)
    *  \brief 根据TopoDS_HShape指针查找数据PID.
    *  \param curEntity TopoDS_HShape指针.
    *  \param strPID 返回查找到的PID.
    */
    virtual bool findShapePID(void* curEntity, std::string& strPID);

    /*! \fn virtual void ImportGeomModel(FILE* fp, const string& fileName, int& iEntityNum, std::vector<void*>& entityList)
    *  \brief 导入模型文件.
    *  \param fp 文件句柄.
    *  \param iEntityNum 返回读取到的模型数目.
    *  \return entityList 返回读取到的模型TopoDS_HShape指针链表.
    */
    virtual void ImportGeomModel(FILE* fp, const string& fileName, int& iEntityNum, std::vector<void*>& entityList);

	/*! \fn virtual bool PrepareModel(const std::vector<void*>& inputEntList, std::vector<void*>& OutEntList)
	*  \brief 接口已弃用.
	*  \param inputEntList 所有进行非规则布尔运算处理的模型TopoDS_HShape的指针链表.
	*  \param OutEntList 返回创建模型的TopoDS_HShape的指针，实际上size为1.
	*  \return 是否成功进行操作.
    */
	virtual bool PrepareModel(const std::vector<void*>& inputEntList,
		std::vector<void*>& OutEntList);

	/*! \fn virtual bool PrepareModel(const std::vector<void*>& inputEntList, const std::vector<int>& idList, std::vector<void*>& OutEntList)
	*  \brief 接口已弃用.
	*/
    virtual bool PrepareModel(const std::vector<void*>& inputEntList,
        const std::vector<int>& idList, std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp);

	/*! \fn virtual bool PrepareModel(const std::vector<void*>& inputEntList, std::vector<void*>& OutEntList)
	*  \brief 非规则布尔并以及后续属性赋值的接口.
	*  \param inputEntList 所有进行非规则布尔运算处理的模型TopoDS_HShape的指针链表.
	*  \param OutEntList 返回创建模型的TopoDS_HShape的指针，实际上size为1.
	*  \return 是否成功进行操作.
	*/
	virtual bool PrepareModel(const std::vector<void*>& inputEntList,/* const std::vector<int>& idList,*/
		std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp);

    /*! \fn virtual void ExportModelFile(const std::vector<void*>& inputEntList, const std::vector<string>& entityNameList, FILE* fp)
    *  \brief 接口未实现.
    */
    virtual void ExportModelFile(const std::vector<void*>& inputEntList, const std::vector<string>& entityNameList,
        FILE* fp);

    /*! \fn virtual void RestoreModelArchive(xdArchive &ar, std::vector<void*>& loadedEntList, std::vector<string>& entPIDList)
    *  \brief 从外部文件中读取数据到本类.
    *  \param ar 文件句柄.
    *  \param loadedEntList 返回读取到的模型TopoDS_HShape指针链表.
    *  \param entPIDList 返回读取到的模型PID链表.
    */
    virtual void RestoreModelArchive(xdArchive &ar, std::vector<void*>& loadedEntList, std::vector<string>& entPIDList);

    /*! \fn virtual void SaveModelArchive(const std::vector<void*>& inputEntList, xdArchive &ar)
    *  \brief 将本类数据保存成外部文件.
    *  \param inputEntList 需要保存的模型TopoDS_HShape指针链表.
    *  \param ar 文件句柄.
    */
    virtual void SaveModelArchive(const std::vector<void*>& inputEntList, xdArchive &ar);

    /*! \fn double GetTolerance()
    *  \brief 获取误差精度.
    *  \return 返回误差精度.
    */
    virtual double GetTolerance();

    /*! \fn virtual std::string GetStringAttribute(void* pEntity, const std::string& attrName)
    *  \brief 获取模型string类型属性值.
    *  \param pEntity 模型TopoDS_HShape指针.
    *  \param attrName 属性名称.
    *  \return 属性值.
    */
    virtual std::string GetStringAttribute(void* pEntity, const std::string& attrName);

    /*! \fn void BuildEntityAttributeName(void* orgBdy, void* curBdy, const std::string& bodyName, bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID)
    *  \brief 为模型赋值名称属性，如果是新模型就构建新名称，如果是原模型进行了编辑则继承名称.
    *  \param orgBdy 新模型或者原模型TopoDS_HShape指针.
    *  \param curBdy 编辑后模型TopoDS_HShape指针.
    *  \param bodyName 模型名称.
    *  \param bIsNewCreate 模型是否是新建的.
    *  \param bIsCloneCS 模型是否是拷贝的.
    *  \param bNeedCopyFaceName 模型面或者边的名称.
    *  \param iNextFaceID 模型面的ID.
    *  \param iNextEdgeID 模型边的ID.
    */
    virtual void BuildEntityAttributeName(void* orgBdy, void* curBdy, const std::string& bodyName,
        bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID);

    /*! \fn virtual bool GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue)
    *  \brief 获取模型string类型属性值.
    *  \param entity 模型TopoDS_HShape指针.
    *  \param attrName 属性名称.
    *  \param attrValue 返回属性值.
    *  \return 返回是否存在该属性值.
    */
    virtual bool GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue);

    /*! \fn virtual bool GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue)
    *  \brief 获取模型int类型属性值.
    *  \param entity 模型TopoDS_HShape指针.
    *  \param attrName 属性名称.
    *  \param attrValue 返回属性值.
    *  \return 返回是否存在该属性值.
    */
    virtual bool GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue);

    /*! \fn virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue)
    *  \brief 为模型附string类型属性.
    *  \param entity 模型TopoDS_HShape指针.
    *  \param attrName 属性名称.
    *  \param attrValue 属性值.
    *  \param bNeedCopy 是否进行复制.
    *  \return 返回是否成功赋值.
    */
    virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue, bool bNeedCopy = false);

    /*! \fn virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue)
    *  \brief 为模型附int类型属性.
    *  \param entity 模型TopoDS_HShape指针.
    *  \param attrName 属性名称.
    *  \param attrValue 属性值.
    *  \param bNeedCopy 是否进行复制.
    *  \return 返回是否成功赋值.
    */
    virtual bool AddEntityAttribute(void* entity, const std::string &attrName, const int attrValue, bool bNeedCopy = false);

	/*! \fn virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName, const int attrValue)
	*  \brief 查找该属性是否存在.
	*  \param entity 模型TopoDS_HShape指针.
	*  \param attrName 属性名称.
	*  \param attrValue 属性值.
	*  \return 返回是否存在.
	*/
	virtual bool HasEntityAttributeValue(void* entity, const std::string &attrName,
		const int attrValue);

    /*! \fn virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName)
    *  \brief 删除模型属性.
    *  \param entity 模型TopoDS_HShape指针.
    *  \param attrName 属性名称.
    *  \return 返回是否成功删除.
    */
    virtual bool RemoveEntityAttribute(void* entity, const std::string &attrName);

    /*! \fn virtual bool RemoveCellTopology(std::vector<void*>& entity_list)
    *  \brief 接口未实现.
    */
    virtual bool RemoveCellTopology(std::vector<void*>& entity_list);

    /*! \fn virtual bool DeleteBodies(std::vector<void*>& entity_list)
    *  \brief 批量删除实体模型.
    *  \param entity_list 需要删除的模型TopoDS_HShape指针链表.
    *  \return 是否成功删除.
    */
    virtual bool DeleteBodies(std::vector<void*>& entity_list);

    /*! \fn virtual bool WriteSatFile(std::vector<void*>& entity_list, FILE* file_prt)
    *  \brief 接口未实现.
    */
    virtual bool WriteSatFile(std::vector<void*>& entity_list, FILE* file_prt);

    /*! \fn virtual std::string GetEntityUuid(void* entity, int id)
    *  \brief 根据模型数据创建名称.
    *  \param entity 模型TopoDS_HShape指针.
    *  \param id 模型id.
    *  \return 创建的模型名称.
    */
    virtual std::string GetEntityUuid(void* entity, int id = -1);

    /*! \fn virtual void* GetEntityByUuid(void* entity, std::vector<std::string>& pidlist)
    *  \brief 根据模型名称解析得到模型PID，从而得到模型数据.
    *  \param entity 模型TopoDS_HShape指针.
    *  \param pidlist 模型PID链表.
    *  \return 解析并查找到的模型TopoDS_HShape指针.
    */
    virtual void* GetEntityByUuid(void* entity, std::vector<std::string>& pidlist);

	/*! \fn virtual void* GetEntityByUuid(void* owner, std::string& strEntUuid)
	*  \brief 接口未实现.
	*/
	virtual void* GetEntityByUuid(void* owner, std::string& strEntUuid) { return nullptr; }

    /*! \fn virtual int GetBodyMaterial(void* entity)
    *  \brief 接口未实现.
    */
    virtual int GetBodyMaterial(void* entity);

    /*! \fn virtual int GetFaceBoundaryCondition(void* entity)
    *  \brief 接口未实现.
    */
    virtual int GetFaceBoundaryCondition(void* entity);

    /*! \fnvirtual bool IsPeriodicBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList, std::vector<void*>& faceList)
    *  \brief 接口未实现.
    */
    virtual bool IsPeriodicBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
        std::vector<void*>& faceList);

    /*! \fn virtual bool IsFEIEBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList, std::vector<void*>& faceList)
    *  \brief 接口未实现.
    */
    virtual bool IsFEIEBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList,
        std::vector<void*>& faceList);

    /*! \fn virtual std::string GetPointUuid(void* entity, const std::string& faceName, double u, double v)
    *  \brief 接口未实现.
    */
    virtual std::string GetPointUuid(void* entity, const std::string& faceName, double u, double v);

    /*! \fn virtual std::string GetPointUuid(void* face, double u, double v)
    *  \brief 接口未实现.
    */
    virtual std::string GetPointUuid(void* face, double u, double v);

    /*! \fn virtual std::string GetPointUuid(void* entity, const std::string& edgeName, double u)
    *  \brief 接口未实现.
    */
    virtual std::string GetPointUuid(void* entity, const std::string& edgeName, double u);

    /*! \fn virtual std::string GetPointUuid(void* edge, double u)
    *  \brief 接口未实现.
    */
    virtual std::string GetPointUuid(void* edge, double u);

    /*! \fn virtual bool GetEdgeLength(void* entity, double& minLength, double& maxLength)
    *  \brief 接口未实现.
    */
	virtual bool GetEdgeLength(void* entity, double& minLength, double& maxLength) { return false; };

    /*! \fn virtual bool GetRelatedBody(void* vFace, std::vector<void*>& entList, std::vector<void*>& bodyList)
    *  \brief 接口未实现.
    */
	virtual bool GetRelatedBody(void* vFace, std::vector<void*>& entList, std::vector<void*>& bodyList) { return false; }

    /*! \fn virtual bool GetFaces(void* entity, std::vector<void*>& faceList)
    *  \brief 接口未实现.
    */
	virtual bool GetFaces(void* entity, std::vector<void*>& faceList) { return false; }
	
    /*! \fn virtual bool GetFaces(void* entity, std::vector<std::string>& faceUuidList)
    *  \brief 接口未实现.
    */
	virtual bool GetFaces(void* entity, std::vector<std::string>& faceUuidList) { return false; }

	/*! \fn virtual bool GetEdgeList(void* entity, std::vector<void*>& edgeList)
	*  \brief 接口未实现.
	*/
	virtual bool GetEdgeList(void* entity, std::vector<void*>& edgeList) { return false; };

	/*! \fn virtual bool GetVertexList(void* entity, std::vector<void*>& vertexList)
	*  \brief 接口未实现.
	*/
	virtual bool GetVertexList(void* entity, std::vector<void*>& vertexList) { return false; };

    /*! \fn virtual bool IsApproximatePlane(void* face, double tolAngle)
    *  \brief 接口未实现.
    */
    virtual bool IsApproximatePlane(void* face, double tolAngle) { return false; }

    /*! \fn virtual bool CreatePML(void* box, int layerCount, bool joiningEdges, bool joiningVertices, std::vector<void*>& bodyList)
    *  \brief 接口未实现.
    */
	virtual bool CreatePML(void* box, int layerCount,
		bool joiningEdges, bool joiningVertices, std::vector<void*>& bodyList) {
		return false;
	}

    /*! \fn virtual bool GetPlaneType(void* face, int& planeType)
    *  \brief 接口未实现.
    */
	virtual bool GetPlaneType(void* face, int& planeType) { return false; }

    /*! \fn virtual bool GetFeaturePointCoordinates(void* face, std::vector<std::vector<double>>& pointCoords)
    *  \brief 接口未实现.
    */
	virtual bool GetFeaturePointCoordinates(void* face, std::vector<std::vector<double>>& pointCoords) { return false; };

    /*! \fn virtual std::string GetVertexNameFromCoordinate(void* owneEntity, double dX, double dY, double dZ)
    *  \brief 接口未实现.
    */
	virtual std::string GetVertexNameFromCoordinate(void* owneEntity, double dX, double dY, double dZ) { return "";  }

    /*! \fn virtual bool GetPointRelationshipOfEntity(void* pEntity, double dX, double dY, double dZ)
    *  \brief 接口未实现.
    */
    virtual bool GetPointRelationshipOfEntity(void* pEntity, double dX, double dY, double dZ) { return false; }

    /*! \fn virtual BODYTYPE GetBodyType(void* entity)
    *  \brief 接口未实现.
    */
	virtual BODYTYPE GetBodyType(void* entity) { return SheetBody; }

    /*! \fn virtual bool IsClosedWireBody(void* entity)
    *  \brief 接口未实现.
    */
	virtual bool IsClosedWireBody(void* entity) { return false; }

    /*! \fn virtual bool GetEntityListBndBox(const std::vector<void*>& inputEntList, double& dMinX, double& dMinY, double& dMinZ, double& dMaxX, double& dMaxY, double& dMaxZ)
    *  \brief 接口未实现.
    */
	virtual bool GetEntityListBndBox(const std::vector<void*>& inputEntList,
		double& dMinX, double& dMinY, double& dMinZ,
		double& dMaxX, double& dMaxY, double& dMaxZ) 
	{
		return false;
	}

    /*! \fn virtual bool GetSharedEdges(std::vector<void*>& faceList, std::vector<void*>& edgeList)
    *  \brief 接口未实现.
    */
	virtual bool GetSharedEdges(std::vector<void*>& faceList, std::vector<void*>& edgeList) { return false;  }

	/*! \fn virtual void* GetOwnerEntity(void* pEntity)
	*  \brief 获取包含该模型的顶层拓扑数据.
	*  \param pEntity 需要查找的模型TopoDS_HShape指针.
	*  \return 找到的底层拓扑数据(TopoDS_HShape指针).
	*/
	virtual void* GetOwnerEntity(void* pEntity);

	/*! \fn virtual void* CreateBoundaryBoxSolid(const std::vector<void*>& modelList, bool& bInsideBox)
	*  \brief 接口未实现.
	*/
	virtual void* CreateBoundaryBoxSolid(const std::vector<void*>& modelList, bool& bInsideBox) { return nullptr; }

	/*! \fn virtual bool GetEntityColorFromAttribute(void* pEntity, int& iR, int& iG, int& iB)
	*  \brief 接口未实现.
	*/
	virtual bool GetEntityColorFromAttribute(void* pEntity, int& iR, int& iG, int& iB) { return false; };

	/*! \fn virtual void SetRestoreFileCallbackFunc(void(*pCallbackFunc)(int, int))
	*  \brief 接口未实现.
	*/
	virtual void SetRestoreFileCallbackFunc(void(*pCallbackFunc)(int, int)) {};

	/*! \fn virtual void GetGeomMeshData(void* pEntity, std::map<void*, cmtPoint>& vertPosMap, std::vector<std::vector<void*>>& meshDataList, std::vector<void*>& meshFaceDataList)
	*  \brief 接口未实现.
	*/
	virtual void GetGeomMeshData(void* pEntity, std::map<void*, cmtPoint>& vertPosMap,
		std::vector<std::vector<void*>>& meshDataList, std::vector<void*>& meshFaceDataList) {};

	/*! \fn virtual void CheckAndFixModel(void* pEntity, bool bFix)
	*  \brief 接口未实现.
	*/
	virtual void CheckAndFixModel(void* pEntity, bool bFix) {};

	/*! \fn virtual bool GetPointParasiticEntityUuid(const std::string& strPtUuid, std::string& strEntUuid, std::string& strParams)
	*  \brief 接口未实现.
	*/
	virtual bool GetPointParasiticEntityUuid(const std::string& strPtUuid, std::string& strEntUuid,
		std::string& strParams) {
		return false;
	}

	/*! \fn virtual std::string GetPointUuid(const std::string& strEntUuid, const std::string& strParams)
	*  \brief 接口未实现.
	*/
	virtual std::string GetPointUuid(const std::string& strEntUuid, const std::string& strParams) { return ""; };
protected:
    /*! \fn bool getPIDProperty(const std::string& strPID, const std::string& propertyName, int& value)
    *  \brief 根据属性名称和PID进行查找并获取int型属性.
    *  \param strPID 模型数据PID.
    *  \param propertyName 属性名称.
    *  \param value 返回的int型属性值.
    *  \return 是否查找到属性.
    */
    bool getPIDProperty(const std::string& strPID, const std::string& propertyName, int& value);

    /*! \fn bool getPIDProperty(const std::string& strPID, const std::string& propertyName, std::string& value)
    *  \brief 根据属性名称和PID进行查找并获取std::string型属性.
    *  \param strPID 模型数据PID.
    *  \param propertyName 属性名称.
    *  \param value 返回的std::string型属性值.
    *  \return 是否查找到属性.
    */
    bool getPIDProperty(const std::string& strPID, const std::string& propertyName, std::string& value);

    /*! \fn bool getPIDProperty(const std::string& strPID, const std::string& propertyName, double& value)
    *  \brief 根据属性名称和PID进行查找并获取double型属性.
    *  \param strPID 模型数据PID.
    *  \param propertyName 属性名称.
    *  \param value 返回的double型属性值.
    *  \return 是否查找到属性.
    */
    bool getPIDProperty(const std::string& strPID, const std::string& propertyName, double& value);

    /*! \fn bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const int& value)
    *  \brief 根据属性名称和PID进行模型数据查找并添加int型属性.
    *  \param strPID 模型数据PID.
    *  \param propertyName 属性名称.
    *  \param value 返回的int型属性值.
    *  \return 是否查找到属性.
    */
    bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const int& value);

    /*! \fn bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const std::string& value)
    *  \brief 根据属性名称和PID进行模型数据查找并添加std::string型属性.
    *  \param strPID 模型数据PID.
    *  \param propertyName 属性名称.
    *  \param value 返回的std::string型属性值.
    *  \return 是否查找到属性.
    */
    bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const std::string& value);

    /*! \fn bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const double& value)
    *  \brief 根据属性名称和PID进行模型数据查找并添加double型属性.
    *  \param strPID 模型数据PID.
    *  \param propertyName 属性名称.
    *  \param value 返回的double型属性值.
    *  \return 是否查找到属性.
    */
    bool addPIDProperty(const std::string& strPID, const std::string& propertyName, const double& value);

    /*! \fn bool removeLabelProperty(const std::string& strPID, const std::string& propertyName)
    *  \brief 根据属性名称和PID的标识删除属性.
    *  \param strPID 模型数据PID.
    *  \param propertyName 属性名称.
    *  \return 是否查找到属性.
    */
    bool removeLabelProperty(const std::string& strPID, const std::string& propertyName);

    bool Heal(const std::map<void*, std::string> entnamemap, 
        std::vector<void*>& outentlist, HealOption params, std::string& m_err);

    bool RemoveHolesWithinTolerance(double maxradius);

protected:
    OccModelData*       m_data;
};

#endif