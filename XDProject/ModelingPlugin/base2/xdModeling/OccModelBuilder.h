/*! \file OccModelBuilder.h
*   \brief OccModelBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/
#ifndef OCCMODELBUILDER_H
#define OCCMODELBUILDER_H

#include "xdModelingHeader.h"
#include "xdModelingDataType.h"
#include "GeomModelBuilder.h"
#include "EnumAndStruct.h"

#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <algorithm>

struct SplitFaceCreationInfos
{
    bool threepointtype = false;
    bool pointdirtype = false;
    bool isxoy = false;
    bool isxoz = false;
    bool isyoz = false;
    void* pt1;
    void* pt2;
    void* pt3;
    void* pt4;
    double dir[3];
};

struct SplitLineCreationInfos
{
    bool twopointtype = false;
    bool pointedgetype = false;
    void* pt1;
    void* pt2;
    void* pt3;
    void* edge;
};

class TDocStd_Document;
class OccModel;
/*! \class OccModelBuilder
*   \brief 用于各类基本实体的创建和变换：包括各类点、线、面、实体、布尔运算、导入导出等。
 */
class XDMODELING_API OccModelBuilder : public GeomModelBuilder
{
public:
    /*! \enum OccModelBuilder::EnFileType
    * 模型文件类型
    */
	enum EnFileType
	{
		ENIGS,
		ENSTP,
		ENBREP
	};

    /*! \fn OccModelBuilder()
    *  \brief OccModelBuilder的构造函数.
    */
	OccModelBuilder();

    /*! \fn virtual ~OccModelBuilder()
    *  \brief OccModelBuilder的析构函数.
    */
	virtual ~OccModelBuilder();

    /*! \fn virtual void builderBegin()
    *  \brief 模型数据在调用各个方法时标明命令开始的方法，用于撤销、反撤销.
    */
	virtual void builderBegin();

    /*! \fn virtual void builderEnd()
    *  \brief 模型数据在调用各个方法时标明命令结束的方法，用于撤销、反撤销.
    */
	virtual void builderEnd();

    /*! \fn virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const int& value)
    *  \brief 为模型附int类型属性.
    *  \param modelEntity 模型TopoDS_HShape指针.
    *  \param propertyName 属性名称.
    *  \param value 属性值.
    *  \return 返回是否成功赋值.
    */
	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const int& value);

    /*! \fn virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const std::string& value)
    *  \brief 为模型附std::string类型属性.
    *  \param modelEntity 模型TopoDS_HShape指针.
    *  \param propertyName 属性名称.
    *  \param value 属性值.
    *  \return 返回是否成功赋值.
    */
	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const std::string& value);

    /*! \fn virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const double& value)
    *  \brief 为模型附double类型属性.
    *  \param modelEntity 模型TopoDS_HShape指针.
    *  \param propertyName 属性名称.
    *  \param value 属性值.
    *  \return 返回是否成功赋值.
    */
	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const double& value);

    /*! \fn virtual bool addModelAttribute(void* modelEntity, double dRed, double dGreen, double dBlue)
    *  \brief 接口已弃用.
    */
	virtual bool addModelAttribute(void* modelEntity, double dRed, double dGreen, double dBlue);

    /*! \fn double tolerance()
    *  \brief 获取误差精度.
    *  \return 返回误差精度.
    */
    virtual double tolerance();

    /*! \fn virtual void HandlePrimitiveAnnotations()
    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief 接口未实现.
    */
    virtual void HandlePrimitiveAnnotations();

    /*! \fn virtual void HandleCopyAnnotations()
    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief 接口未实现.
    */
    virtual void HandleCopyAnnotations();

    /*! \fn virtual void HandleSplitAnnotations()
    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief 接口未实现.
    */
    virtual void HandleSplitAnnotations();

    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief 接口未实现.
    */
    virtual std::string CreateEdgeNameFromEntity(void* pSelEdge);

public:
    /*! \fn virtual bool createVertex(const double& xPos, const double& yPos, const double& zPos, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 接口已弃用.
    */
    virtual bool createVertex(const double& xPos, const double& yPos, const double& zPos,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual  virtual bool createLine(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 接口已弃用.
	*/
    virtual bool createLine(const double& xPos1, const double& yPos1, const double& zPos1,
        const double& xPos2, const double& yPos2, const double& zPos2,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual  virtual bool createPlane(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 接口已弃用.
	*/
    virtual bool createPlane(const double& xPos1, const double& yPos1, const double& zPos1,
        const double& xPos2, const double& yPos2, const double& zPos2,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn bool importModel(const std::string& path, const char* name, const EnFileType& type, std::string& strPID)y)
    *  \brief 接口已弃用.
    */
    bool importModel(const std::string& path, const char* name, const EnFileType& type, std::string& strPID);

    /*! \fn static bool exportModel(const std::string& path, const EnFileType& type, const std::string& strPID)
    *  \brief 接口已弃用.
    */
    static bool exportModel(const std::string& path, const EnFileType& type, const std::string& strPID);

    /*! \fn bool exportEntityList(std::vector<void *>entList, std::string& path)
    *  \brief 接口已弃用.
    */
    bool exportEntityList(std::vector<void *>entList, std::string& path);

    /*! \fn bool deleteLabel(const std::string& strPID)
    *  \brief 接口已弃用.
    */
    bool deleteLabel(const std::string& strPID);

    /************************************************************************/
    /*                             real interface                           */
    /************************************************************************/
    /*! \fn virtual bool createUnusedEntity(const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建（0,0,0）到（1,0,0）的线段.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createUnusedEntity(const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createPolyline(double* wcsMatrix, std::vector<std::vector<double>>& points, std::vector<PolylineSegment>& segments, const LOGICAL& isClosed, const LOGICAL& isCovered, const char * name, std::string & strPID, void *& resEntity);
    *  \brief 创建多种连续线段或者曲线的方法，包括线段、B样条、三点圆弧线、中心点圆弧线.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param points 多种线段或者曲线用到的点坐标序列，为n*3.
    *  \param segments 多种线段类型定义以及中心点圆弧线的参数.
    *  \param isClosed 连续线段或者曲线是否闭合.
    *  \param isCovered 连续多种线段或者曲线是否生成闭合曲线并生成面.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createPolyline(double* wcsMatrix, std::vector<std::vector<double>>& points, std::vector<PolylineSegment>& segments,
        const LOGICAL& isClosed, const LOGICAL& isCovered, const CrossSection& crossSection,
        const char * name, std::string & strPID, void *& resEntity);//1st stage

    /*! \fn virtual bool createHyperbola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief 双曲线的创建接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param points 创建双曲线时用到的三点坐标，依次为起点、中心点、终点.
    *  \param majorR 主半径.
    *  \param minorR 次半径.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createHyperbola(double* wcsMatrix, std::vector<std::vector<double>>& points,
        const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createParabola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief 创建抛物线的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param points 创建抛物线时用到的三点坐标，依次为起点、中心点、终点.
    *  \param focalDistance 焦距.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createParabola(double* wcsMatrix, std::vector<std::vector<double>>& points,
        const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createHyperbola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief 双曲线的创建接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param points 创建双曲线时用到的中心点坐标.
    *  \param tStart 起始t值.
    *  \param tEnd 终止t值.
    *  \param majorR 主半径.
    *  \param minorR 次半径.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createHyperbola(double* wcsMatrix, std::vector<double>& points, const double& tStart, const double& tEnd,
        const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createParabola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief 创建抛物线的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param points 创建抛物线时用到的中心点坐标.
    *  \param tStart 起始t值.
    *  \param tEnd 终止t值.
    *  \param focalDistance 焦距.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createParabola(double* wcsMatrix, std::vector<double>& points, const double& tStart, const double& tEnd,
        const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 空间螺旋线的创建接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 起始点圆心x坐标.
    *  \param centerY 起始点圆心y坐标.
    *  \param centerZ 起始点圆心z坐标.
    *  \param lowerRadius 起点圆半径.
    *  \param upperRadius 终点圆半径.
    *  \param height 高度.
    *  \param turns 程数.
    *  \param rightHanded 是否右旋.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& lowerRadius, const double& upperRadius, const double& height,
        const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& newEntry, void*& resEntity)
	*  \brief 空间螺旋线的创建接口.
	*  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
	*  \param centerX 起始点圆心x坐标.
	*  \param centerY 起始点圆心y坐标.
	*  \param centerZ 起始点圆心z坐标.
	*  \param startX 起始点x坐标.
	*  \param startY 起始点y坐标.
	*  \param startZ 起始点z坐标.
	*  \param upperRadius 终点圆半径.
	*  \param height 高度.
	*  \param turns 程数.
	*  \param rightHanded 是否右旋.
	*  \param axis 中心轴朝向.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
	*/
	virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ,
		const double& upperRadius, const double& height,
		const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const double& maxRadius, const double& turns, const double& rightHanded,const DATUMAXISTYPE& axis,const char* name, std::string& newEntry, void*& resEntity) void*& resEntity)
	*  \brief 平面螺旋线的创建接口.
	*  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
	*  \param centerX 中心点x坐标.
	*  \param centerY 中心点y坐标.
	*  \param centerZ 中心点z坐标.
	*  \param startX 起始点x坐标.
	*  \param startY 起始点y坐标.
	*  \param startZ 起始点z坐标.
	*  \param maxRadius 终点圆半径.
	*  \param turns 程数.
	*  \param rightHanded 是否右旋.
	*  \param axis 中心轴朝向.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
	*/
	virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ,
		const double& maxRadius, const double& turns, const double& rightHanded,
		const DATUMAXISTYPE& axis,
		const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 平面螺旋线的创建接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param lowerRadius 起点圆半径.
    *  \param upperRadius 终点圆半径.
    *  \param turns 程数.
    *  \param rightHanded 是否右旋.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded,
        const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEquationCurve(double* wcsMatrix, const char* xtFunction, const char* ytFunction, const char* ztFunction, const double& startT, const double& strEndT, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建函数曲的创建接口.
	*  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
	*  \param xtFunction x关于T的方程.
	*  \param ytFunction y关于T的方程.
	*  \param ztFunction z关于T的方程.
	*  \param startT T值的起始值.
	*  \param strEndT T值的终止值.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
    */
    virtual bool createEquationCurve(double* wcsMatrix, const char* xtFunction, const char* ytFunction,
        const char* ztFunction, const double& startT, const double& strEndT,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createRectangle(double* wcsMatrix, const double& startX, const double& startY, const double& startZ, const double& width, const double& height, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建矩形面或者线框的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param startX 起点x坐标.
    *  \param startY 起点y坐标.
    *  \param startZ 起点z坐标.
    *  \param width 宽度.
    *  \param height 高度.
    *  \param axis 中心轴朝向.
    *  \param isCovered 是否生成面.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createRectangle(double* wcsMatrix, const double& startX, const double& startY, const double& startZ,
        const double& width, const double& height, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCircle(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& radius, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建圆或者圆形线框的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param radius 半径.
    *  \param axis 中心轴朝向.
    *  \param isCovered 是否生成面.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createCircle(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& radius, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        int segment, const char* name, std::string& strPID, void*& entity);

    /*! \fn virtual bool createSector(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& radius, const double& startAng, const double& endAng, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 接口已弃用.
    */
	virtual bool createSector(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& radius, const double& startAng, const double& endAng, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
		const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createRegularPolygon(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const int& numSides, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建正多边形面或者线框的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param startX 起点x坐标.
    *  \param startY 起点y坐标.
    *  \param startZ 起点z坐标.
    *  \param numSides 正多边形边的数目.
    *  \param axis 中心轴朝向.
    *  \param isCovered 是否生成面.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createRegularPolygon(double* wcsMatrix, const double& centerX, const double& centerY,
        const double& centerZ, const double& startX, const double& startY, const double& startZ,
        const int& numSides, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEllipse(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建椭圆面或者线框的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param majRadius 主轴半径.
    *  \param ratio 长短轴比例.
    *  \param axis 中心轴朝向.
    *  \param isCovered 是否生成面.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createEllipse(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 接口已弃用.
	*/
    virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter,
        const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const DATUMAXISTYPE& axis, const double& raidus, const double& height, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建圆柱面的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param xBottomCenter 底面圆心x坐标.
    *  \param yBottomCenter 底面圆心y坐标.
    *  \param zBottomCenter 底面圆心z坐标.
    *  \param xAxis 圆柱朝向向量x值.
    *  \param yAxis 圆柱朝向向量y值.
    *  \param zAxis 圆柱朝向向量z值.
    *  \param radius 半径.
    *  \param height 圆柱高度.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter,
        const DATUMAXISTYPE& axis, const double& raidus, const double& height,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createConeFace(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建圆锥面或者圆台面的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param botCenterX 底面圆心x坐标.
    *  \param botCenterY 底面圆心y坐标.
    *  \param botCenterZ 底面圆心z坐标.
    *  \param lowerRadius 底面半径.
    *  \param upperRadius 顶面半径，为0即圆锥面.
    *  \param axis 中心轴朝向.
    *  \param height 圆柱高度.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createConeFace(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEquationSurface(double* wcsMatrix, const char* xuvFunction, const char* yuvFunction, const char* zuvFunction, const double& startU, const double& strEndU, const double& startV, const double& strEndV, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建函数曲面的接口.
	*  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
	*  \param xuvFunction x关于U和V的方程.
	*  \param yuvFunction y关于U和V的方程.
	*  \param zuvFunction z关于U和V的方程.
	*  \param startU U值的起始值.
	*  \param strEndU U值的终止值.
	*  \param startV V值的起始值.
	*  \param strEndV V值的终止值.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
    */
    virtual bool createEquationSurface(double* wcsMatrix, const char* xuvFunction, const char* yuvFunction, const char* zuvFunction,
        const double& startU, const double& strEndU, const double& startV, const double& strEndV,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCuboid(double* wcsMatrix, const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建立方体的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param xStart 立方体起点x坐标.
    *  \param yStart 立方体起点y坐标.
    *  \param zStart 立方体起点z坐标.
    *  \param xSize 立方体x方向长度.
    *  \param ySize 立方体y方向长度.
    *  \param zSize 立方体z方向长度.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createCuboid(double* wcsMatrix, const double& xStart, const double& yStart, const double& zStart,
        const double& xSize, const double& ySize, const double& zSize, const char* name,
        std::string& strPID, void*& resEntity);

    /*! \fn  virtual bool createCylinder(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& raidus, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建圆柱实体的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param xBottomCenter 底面圆心x坐标.
    *  \param yBottomCenter 底面圆心y坐标.
    *  \param zBottomCenter 底面圆心z坐标.
    *  \param radius 半径.
    *  \param height 高度.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createCylinder(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter,
        const double& raidus, const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createSphere(double* wcsMatrix, const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建球体实体的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param xCenter 球心x坐标.
    *  \param yCenter 球心y坐标.
    *  \param zCenter 球心z坐标.
    *  \param radius 半径.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createSphere(double* wcsMatrix, const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCone(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建圆锥或者圆台实体的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param botCenterX 底面圆心x坐标.
    *  \param botCenterY 底面圆心y坐标.
    *  \param botCenterZ 底面圆心z坐标.
    *  \param lowerRadius 底面半径.
    *  \param upperRadius 顶面半径，为0即为圆锥.
    *  \param height 高度.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createCone(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn
    *  \brief 创建正多边形棱柱的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param botCenterX 底面中心x坐标.
    *  \param botCenterY 底面中心y坐标.
    *  \param botCenterZ 底面中心z坐标.
    *  \param startX 起点x坐标.
    *  \param startY 起点y坐标.
    *  \param startZ 起点z坐标.
    *  \param height 高度.
    *  \param numSides 正多边形边的数目.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createRegularPolyhedron(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& startX, const double& startY, const double& startZ,
        const double& height, const int& numSides, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createTorus(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& minorRadius, const double& majorRadius, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建圆环实体的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 圆心x坐标.
    *  \param centerY 圆心y坐标.
    *  \param centerZ 圆心z坐标.
    *  \param minorRadius 小圆半径.
    *  \param majorRadius 大圆半径.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createTorus(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& minorRadius, const double& majorRadius, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createFlare(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 多棱排实体创建接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param botCenterX 底面中心x坐标.
    *  \param botCenterY 底面中心y坐标.
    *  \param botCenterZ 底面中心z坐标.
    *  \param lowerWidth 底面矩形宽度.
    *  \param lowerDepth 底面矩形长度.
    *  \param upperWidth 顶面矩形宽度.
    *  \param upperDepth 顶面矩形长度.
    *  \param height 高度.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createFlare(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth,
        const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEllipsoid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建椭球实体.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 椭球中心x坐标.
    *  \param centerY 椭球中心y坐标.
    *  \param centerZ 椭球中心z坐标.
    *  \param majRadius 主轴长度.
    *  \param ratio 长短轴比例.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
    virtual bool createEllipsoid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createHelixSolid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& profileRadius, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 创建螺旋线实体的接口.
    *  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
    *  \param centerX 起点圆面圆心x坐标.
    *  \param centerY 起点圆面圆心y坐标.
    *  \param centerZ 起点圆面圆心z坐标.
    *  \param profileRadius 截面半径.
    *  \param lowerRadius 起点圆半径.
    *  \param upperRadius 终点圆半径.
    *  \param height 高度.
    *  \param turns 程数.
    *  \param rightHanded 是否右旋.
    *  \param axis 中心轴朝向.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功创建.
    */
	virtual bool createHelixSolid(double* wcsMatrix,
		const double& centerX, const double& centerY, const double& centerZ,
		const double& profileRadius, const double& lowerRadius, const double& upperRadius,
		const double& height, const double& turns, const double& rightHanded,
		const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool makeBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 布尔运算的接口，包括交、并、差、刻印.
    *  \param boolType 布尔运算的类型，为交、并、差、刻印.
    *  \param isKeep 是否保留原有模型.
    *  \param entryList 所有进行布尔运算处理的模型TopoDS_HShape的指针链表.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功进行布尔运算.
    */
    virtual bool makeBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool translate(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ, const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型平移、线性阵列的接口.
    *  \param entry 进行变换模型的TopoDS_HShape的指针.
    *  \param wcsMatrix 模型进行变换的局部坐标系矩阵.
    *  \param vectorX 平移向量的x分量.
    *  \param vectorY 平移向量的y分量.
    *  \param vectorZ 平移向量的z分量.
    *  \param totalNumber 进行平移的次数.
    *  \param isAttach 是否阵列.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool translate(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ,
        const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity);

    //旋转
    /*! \fn virtual bool rotate(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型进行绕轴旋转或者圆周阵列的接口.
    *  \param entry 进行变换模型的TopoDS_HShape的指针.
    *  \param wcsMatrix 模型进行变换的局部坐标系矩阵.
    *  \param axisPosX 旋转轴起点的x坐标.
    *  \param axisPosY 旋转轴起点的y坐标.
    *  \param axisPosZ 旋转轴起点的z坐标.
    *  \param axisVecX 旋转轴方向向量的x分量.
    *  \param axisVecY 旋转轴方向向量的y分量.
    *  \param axisVecZ 旋转轴方向向量的z分量.
    *  \param angle 旋转角度.
    *  \param totalNumber 进行平移的次数.
    *  \param isAttach 是否阵列.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool rotate(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ,
        const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle,
        const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity);

    //镜像
    /*! \fn virtual bool mirror(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const LOGICAL& isCopy, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型进行镜像的接口.
    *  \param entry 进行变换模型的TopoDS_HShape的指针.
    *  \param wcsMatrix 模型进行变换的局部坐标系矩阵.
    *  \param basePosX 镜像平面上任意一点的x坐标.
    *  \param basePosY 镜像平面上任意一点的y坐标.
    *  \param basePosZ 镜像平面上任意一点的z坐标.
    *  \param normalVecX 镜像平面法向向量的x分量.
    *  \param normalVecY 镜像平面法向向量的y分量.
    *  \param normalVecZ 镜像平面法向向量的z分量.
    *  \param isCopy 是否复制原模型.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool mirror(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& normalVecX, const double& normalVecY, const double& normalVecZ, const LOGICAL& isCopy,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool scale(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const LOGICAL& isCopy, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型缩放的接口.
    *  \param entry 进行变换模型的TopoDS_HShape的指针.
    *  \param wcsMatrix 模型进行变换的局部坐标系矩阵.
    *  \param basePosX 缩放参考点的x坐标.
    *  \param basePosY 缩放参考点的y坐标.
    *  \param basePosZ 缩放参考点的z坐标.
    *  \param scaleX x方向缩放比例.
    *  \param scaleY y方向缩放比例.
    *  \param scaleZ z方向缩放比例.
    *  \param isCopy 是否复制原模型.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool scale(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& scaleX, const double& scaleY, const double& scaleZ, const LOGICAL& isCopy,
        const char* name, std::string& strPID, void*& resEntity);

    //extend
    //拉伸
    /*! \fn virtual bool sweepAlongVector(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型拉伸的接口.
    *  \param entry 进行拉伸模型的TopoDS_HShape的指针.
    *  \param wcsMatrix 模型进行变换的局部坐标系矩阵.
    *  \param vectorX 拉伸向量的x分量.
    *  \param vectorY 拉伸向量的y分量.
    *  \param vectorZ 拉伸向量的z分量.
    *  \param draftAngle 拔模角度.
    *  \param draftType 拔模类型.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool sweepAlongVector(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY,
        const double& vectorZ, const double& draftAngle, const SWEEPDRAFTTYPE& draftType,
        const char* name, std::string& strPID, void*& resEntity);

    //旋转
    /*! \fn virtual bool sweepAroundAxis(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型旋转造型的接口.
    *  \param entry 进行旋转模型的TopoDS_HShape的指针.
    *  \param wcsMatrix 模型进行变换的局部坐标系矩阵.
    *  \param axisPosX 旋转轴起点x坐标.
    *  \param axisPosY 旋转轴起点y坐标.
    *  \param axisPosZ 旋转轴起点z坐标.
    *  \param axisVecX 旋转轴方向向量的x分量.
    *  \param axisVecY 旋转轴方向向量的y分量.
    *  \param axisVecZ 旋转轴方向向量的z分量.
    *  \param angle 旋转角度.
    *  \param draftAngle 拔模角度.
    *  \param draftType 拔模类型.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool sweepAroundAxis(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY,
        const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ,
        const double& angle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType,
        const char* name, std::string& strPID, void*& resEntity);

    //扫掠
    /*! \fn virtual bool sweepAlongPath(void* profileEntry, void* pathEntry, const double& twistAngle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型扫略的接口.
    *  \param profileEntry 进行扫掠的截面的TopoDS_HShape的指针.
    *  \param entry 进行扫掠的路径模型的TopoDS_HShape的指针.
    *  \param twistAngle 扭转角度.
    *  \param draftAngle 拔模角度.
    *  \param draftType 拔模类型.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool sweepAlongPath(void* profileEntry, void* pathEntry, const double& twistAngle,
        const double& draftAngle,
        const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool sweepAlongPathPipeshell(void* profileEntry, void* pathEntry, const double& twistAngle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型扫略的接口，与sweepAlongPath不同的是采用OCC中PipeShell的接口去实现扫掠.
    *  \param profileEntry 进行扫掠的截面的TopoDS_HShape的指针.
    *  \param entry 进行扫掠的路径模型的TopoDS_HShape的指针.
    *  \param twistAngle 扭转角度.
    *  \param draftAngle 拔模角度.
    *  \param draftType 拔模类型.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功变换模型.
    */
    virtual bool sweepAlongPathPipeshell(void* profileEntry, void* pathEntry, const double& twistAngle,
        const double& draftAngle,
        const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity);

    //加厚
    /*! \fn virtual bool thicken(void* entry, const double& thickness, const LOGICAL& isDoubleSides, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 实体加厚的接口.
    *  \param entry 进行加厚模型的TopoDS_HShape的指针.
    *  \param thickness 厚度.
    *  \param isDoubleSides 是否进行双向加厚.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功加厚.
    */
    virtual bool thicken(void* entry, const double& thickness, const LOGICAL& isDoubleSides,
        const char* name, std::string& strPID, void*& resEntity);

    //blend
    //倒圆角
    /*! \fn virtual bool fillet(void* entry, const std::list<std::string>& edgeorvertexlist, const double& radius, const double& setback, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 模型倒圆角接口.
    *  \param entry 进行倒角模型的TopoDS_HShape的指针.
    *  \param edgeorvertexlist 三维倒角的线名称序列或者二维倒角中的点名称序列.
    *  \param radius 倒角半径.
    *  \param setback 重复倒角处退后倒角的半径.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功倒角.
    */
    virtual bool fillet(void* entry, const std::list<std::string>& edgeorvertexlist,
        const double& radius, const double& setback,
        const char* name, std::string& strPID, void*& resEntity);

	//倒斜角
	/*! \fn virtual bool chamfer(void* entry, const std::list<std::string>& edgeorvertexlist, const double& setback, const double& lDistance, const double& rDistance, const CHAMFERTYPE& chamferType, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 模型倒斜角的接口.
	*  \param entry 进行倒角模型的TopoDS_HShape的指针.
	*  \param edgeorvertexlist 三维倒角的线名称序列或者二维倒角中的点名称序列.
	*  \param setback 重复倒角处退后倒角的半径.
	*  \param lDistance 倒角左侧距离.
	*  \param rDistance 倒角右侧距离.
	*  \param chamferType 倒角是否对称倒角.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功倒角.
	*/
	virtual bool chamfer(void* entry, const std::list<std::string>& edgeorvertexlist,
		const double& setback, const double& lDistance, const double& rDistance, const CHAMFERTYPE& chamferType,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool fillet(const std::list<void*>& edgesOrVertices, const double& radius, const double& setback, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 模型倒圆角接口.
	*  \param edgeorvertexlist 三维倒角的线名称序列或者二维倒角中的点名称序列.
	*  \param radius 倒角半径.
	*  \param setback 重复倒角处退后倒角的半径.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功倒角.
	*/
	virtual bool fillet(const std::list<void*>& edgesOrVertices,
		const double& radius, const double& setback,
		const char* name, std::string& strPID, void*& resEntity);

	//倒斜角
	/*! \fn virtual bool chamfer(const std::list<std::string>& edgeorvertexlist, const double& setback, const double& lDistance, const double& rDistance, const CHAMFERTYPE& chamferType, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 模型倒斜角的接口.
	*  \param edgeorvertexlist 三维倒角的线名称序列或者二维倒角中的点名称序列.
	*  \param setback 重复倒角处退后倒角的半径.
	*  \param lDistance 倒角左侧距离.
	*  \param rDistance 倒角右侧距离.
	*  \param chamferType 倒角是否对称倒角.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功倒角.
	*/
	virtual bool chamfer(const std::list<void*>& edgesOrVertices,
		const double& setback, const double& lDistance, const double& rDistance,
		const CHAMFERTYPE& chamferType,
		const char* name, std::string& strPID, void*& resEntity);

	//surface
	/*! \fn virtual bool moveFaces(double* wcsMatrix, const std::list<void*>& entityList, const double& vectorX, const double& vectorY, const double& vectorZ, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 将实体中的部分面进行移动实现修改实体的效果.
	*  \param wcsMatrix 模型进行操作的局部坐标系矩阵.
	*  \param entityList 需要进行移动的面数据指针.
	*  \param vectorX 平移向量的x分量.
	*  \param vectorY 平移向量的y分量.
	*  \param vectorZ 平移向量的z分量.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
	*/
	virtual bool moveFaces(double* wcsMatrix, const std::list<void*>& entityList,
		const double& vectorX, const double& vectorY, const double& vectorZ,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool offsetFaces(const std::list<void*>& entityList, const double& distance, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 将实体中的部分面进行偏移实现修改实体的效果.
	*  \param wcsMatrix 模型进行操作的局部坐标系矩阵.
	*  \param entityList 需要进行偏移的面数据指针.
	*  \param distance 偏移距离.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
	*/
	virtual bool offsetFaces(const std::list<void*>& entityList,
		const double& distance, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn  virtual bool copyFaces(void* entity, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 复制几何面的接口.
	*  \param entity 需要复制的模型的TopoDS_HShape的指针.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功复制.
	*/
	virtual bool copyFaces(void* entity,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool sliceByFaces(void* entry, const std::list<void*>& toolFaceList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 线、面、实体切割的接口，工具为面拓扑数据，返回整个模型.
	*  \param entry 进行切割模型的TopoDS_HShape的指针.
	*  \param toolFaceList 进行切割的工具面.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功变换模型.
	*/
	virtual bool sliceByFaces(void* entry, const std::list<void*>& toolFaceList, const char* name, std::string& strPID,
		void*& resEntity);

	/*! \fn virtual bool detachEdges(void* entry, const std::list<void*>& edgeList, LOGICAL isCreated, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 根据线框中的线分割线框的接口.
	*  \param entry 进行分割的线框模型的TopoDS_HShape的指针.
	*  \param edgeList 分割的工具线链表.
	*  \param isCreated 根据工具线链表创建新模型还是分割线框获得新模型.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
	*/
	virtual bool detachEdges(void* entry, const std::list<void*>& edgeList, LOGICAL isCreated, const char* name,
		std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createPoint(double* wcsMatrix, const double& dCoordX, const double& dCoordY, const double& dCoordZ, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 根据坐标创建点.
	*  \param wcsMatrix 模型进行创建的局部坐标系矩阵.
	*  \param dCoordX 点x坐标.
	*  \param dCoordY 点y坐标.
	*  \param dCoordZ 点z坐标.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
	*/
	virtual bool createPoint(double* wcsMatrix, const double& dCoordX, const double& dCoordY, const double& dCoordZ,
		const char* name, std::string& strPID, void*& resEntity);

    //面移动
    /*! \fn virtual bool moveFaces(void* entry, double* wcsMatrix, const std::list<std::string>& edgeNames, const double& vectorX, const double& vectorY, const double& vectorZ, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 面移动实现实体变形的接口，未实现.
    */
    virtual bool moveFaces(void* entry, double* wcsMatrix, const std::list<std::string>& edgeNames,
        const double& vectorX, const double& vectorY, const double& vectorZ,
        const char* name, std::string& strPID, void*& resEntity);

    //面平移
    /*! \fn virtual bool offsetFaces(void* entry, const std::list<std::string>& edgeNames, const double& distance, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 面平移实现实体变形的接口，未实现.
    */
    virtual bool offsetFaces(void* entry, const std::list<std::string>& edgeNames,
        const double& distance, const char* name, std::string& strPID, void*& resEntity);

    virtual bool offsetWire(void* entry, const double& distance,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool copyFaces(void* entry, const std::string& faceName, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 复制面的接口
	*  \param entity 需要复制的模型的TopoDS_HShape的指针.
	*  \param faceName 需要进行复制的面的名称.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功复制.
    */
    virtual bool copyFaces(void* entry, const std::string& faceName,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEntityPIDForEntity(void* curBdy, std::string& newTag)
    *  \brief 获取模型PID的接口.
    *  \param curBdy 需要获取PID的模型的TopoDS_HShape的指针.
    *  \param newTag 返回获取到的PID.
    *  \return 是否查找到该模型.
    */
    virtual bool createEntityPIDForEntity(void* curBdy, std::string& newTag);

    //复制几何体
    /***********************************接口已实现****************************************/
    /*! \fn  virtual bool createCloneEntity(void* curBdy, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 复制几何体的接口.
    *  \param curBdy 需要复制的模型的TopoDS_HShape的指针.
    *  \param name 名称.
    *  \param strPID 返回创建模型的PID.
    *  \param resEntity 返回创建模型的TopoDS_HShape的指针.
    *  \return 是否成功复制.
    */
    virtual bool createCloneEntity(void* curBdy, const char* name,
        std::string& strPID, void*& resEntity);

    //线、面、实体切割
    /*! \fn virtual bool split(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const KEEPSIDE& keepSide, const char* name, std::string& strPID, void*& resEntity
    *  \brief 线、面、实体切割的接口.
	*  \param entry 进行切割模型的TopoDS_HShape的指针.
	*  \param wcsMatrix 模型进行切割的局部坐标系矩阵.
	*  \param basePosX 切割平面法向起点的x坐标.
	*  \param basePosY 切割平面法向起点的y坐标.
	*  \param basePosZ 切割平面法向起点的z坐标.
	*  \param normalVecX 切割平面法向向量的x分量.
	*  \param normalVecY 切割平面法向向量的y分量.
	*  \param normalVecZ 切割平面法向向量的z分量.
	*  \param keepSide 是否保留模型内部的切割平面.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功变换模型.
    */
    virtual bool split(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& normalVecX, const double& normalVecY, const double& normalVecZ, const KEEPSIDE& keepSide,
        const char* name, std::string& strPID, void*& resEntity);

    //面缝合
    /*! \fn virtual bool stitch(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 面缝合的接口，如果能够生成实体则生成实体.
	*  \param entryList 所有进行缝合的线模型TopoDS_HShape的指针链表.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行缝合.
    */
    virtual bool stitch(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool connect(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
    *  \brief 放样的接口.
	*  \param entryList 所有进行放样的线框或者点模型TopoDS_HShape的指针链表.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行放样.
    */
	virtual bool connect(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool cover(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 线框cover的接口.
	*  \param entryList 所有进行cover的线框TopoDS_HShape的指针链表.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行cover.
	*/
    virtual bool cover(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);


	/************************************************************************/
	/*                            added by YAO                              */
	/************************************************************************/

	/*! \fn virtual bool trimCurve(void* edgeEntry, const double& startT, const double& endT, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 曲线裁剪的接口.
	*  \param edgeEntry 需要进行裁剪的线的TopoDS_HShape的指针.
	*  \param startT 裁剪的起始U值.
	*  \param endT 裁剪的终止U值.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行裁剪.
	*/
	virtual bool trimCurve(void* edgeEntry, const double& startT, const double& endT, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool edgeExtendLength(void* edgeEntry, double startParam, double length, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 曲线延长的接口.
	*  \param edgeEntry 需要进行延长的线的TopoDS_HShape的指针.
	*  \param startParam 开始延长位置的U值.
	*  \param length 延长的长度.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行延长.
	*/
	virtual bool edgeExtendLength(void* edgeEntry, double startParam, double length, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool closedEdgeFace(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 多个曲线封闭成线框的接口.
	*  \param entryList 需要进行封闭的线的TopoDS_HShape的指针链表.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建模型.
	*/
	virtual bool closedEdgeFace(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool wireOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 线框偏移的接口.
	*  \param entry 需要进行偏移的线框的TopoDS_HShape的指针.
	*  \param distance 偏移距离.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建模型.
	*/
	virtual bool wireOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool faceOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 面偏移的接口.
	*  \param entry 需要进行偏移的面的TopoDS_HShape的指针.
	*  \param distance 偏移距离.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建模型.
	*/
	virtual bool faceOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool faceExtendLength(void* edgeEntry, double length, const bool& inU, const bool& forward, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 曲面延长的接口.
	*  \param edgeEntry 需要进行延长的面的TopoDS_HShape的指针.
	*  \param length 延长的长度.
	*  \param inU 是否在U方向否则为V方向进行延长.
	*  \param forward 是否为向前延伸.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行延长.
	*/
	virtual bool faceExtendLength(void* edgeEntry, double length, const bool& inU, const bool& forward, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool faceIntersection(void* entry, void* entry2, const int& method, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 面面相交得到交线的接口.
	*  \param entry 需要进行相交的面1的TopoDS_HShape的指针.
	*  \param entry2 需要进行相交的面2的TopoDS_HShape的指针.
	*  \param method 相交算法.1为BRepAlgoAPI_Section, 2为BRepAlgoAPI_IntCS, 3为BRepAlgoAPI_IntSS.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行创建.
	*/
	virtual bool faceIntersection(void* entry, void* entry2, const int& method, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool edgeCombination(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 曲线相连的接口.
	*  \param entry 需要进行相连的线1的TopoDS_HShape的指针.
	*  \param entry2 需要进行相连的线2的TopoDS_HShape的指针.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行创建.
	*/
	virtual bool edgeCombination(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool edgeIntersection(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 线线相交得到交点的接口.
	*  \param entry 需要进行相交的线1的TopoDS_HShape的指针.
	*  \param entry2 需要进行相交的线2的TopoDS_HShape的指针.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行创建.
	*/
	virtual bool edgeIntersection(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool makeFuzzyBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const double& fuzzyValue, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 模糊布尔运算的接口，包括交、并、差.
	*  \param boolType 布尔运算的类型，为交、并、差.
	*  \param isKeep 是否保留原有模型.
	*  \param entryList 所有进行布尔运算处理的模型TopoDS_HShape的指针链表.
	*  \param fuzzyValue 模糊布尔运算精度.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行布尔运算.
	*/
	virtual bool makeFuzzyBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const double& fuzzyValue,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool makeNonRegBoolean(const EnAdapatType& boolType, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 不规则布尔运算的接口，包括交、并、差.
	*  \param boolType 布尔运算的类型，为交、并、差.
	*  \param isKeep 是否保留原有模型.
	*  \param entryList 所有进行布尔运算处理的模型TopoDS_HShape的指针链表.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功进行布尔运算.
	*/
	virtual bool makeNonRegBoolean(const EnAdapatType& boolType, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createNurbsFace(const std::vector<std::vector<std::vector<double>>>& poles, const std::vector<std::vector<double>>& weight, const std::vector<double> &uValue, const std::vector<double>& vValue, const std::vector<int> &uMul, const std::vector<int>& vMul, const int& uDeg, const int& vDeg, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 创建Nurbs面的接口.
	*  \param poles 控制点二维数组.
	*  \param weights 权重数组, 如果全为1则构建非有理样条线.
	*  \param uValue U方向节点.
	*  \param vValue V方向节点.
	*  \param uMul U方向重复节点.
	*  \param vMul V方向重复节点.
	*  \param uDeg U方向自由度.
	*  \param vDeg V方向自由度.
	*  \param name 名称.
	*  \param strPID 返回创建模型的PID.
	*  \param resEntity 返回创建模型的TopoDS_HShape的指针.
	*  \return 是否成功创建.
	*/
	virtual bool createNurbsFace(const std::vector<std::vector<std::vector<double>>>& poles, const std::vector<std::vector<double>>& weight,
		const std::vector<double> &uValue, const std::vector<double>& vValue,
		const std::vector<int> &uMul, const std::vector<int>& vMul,
		const int& uDeg, const int& vDeg, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool shapeFix(void* entry, const double& precision, const double& minTol, const double& maxTol, const EnFixType& fixType,const char* name, std::string& strPID, void*& resEntity, std::string& message)
	*  \brief 模型修复的接口.
	*  \param entry 需要进行修复的模型的TopoDS_HShape的指针.
	*  \param precision 需要进行修复的精度.
	*  \param minTol 需要进行修复的最小误差.
	*  \param maxTol 需要进行修复的最大误差.
	*  \param fixType 进行修复的类型.
	*  \param name 名称.
	*  \param strPID 返回修复模型的PID.
	*  \param resEntity 返回修复模型的TopoDS_HShape的指针.
	*  \param message 返回修复前后的检测信息.
	*  \return 是否成功进行修复.
	*/
	virtual bool shapeFix(void* entry, const double& precision, const double& minTol, const double& maxTol, const EnFixType& fixType,const char* name, std::string& strPID, void*& resEntity, std::string& message);

protected:
    /*! \fn virtual bool createBooleanCut(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 接口已弃用.
	*/
    virtual bool createBooleanCut(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createBooleanCommon(const std::string& mainEntry, const std::string& toolEntry, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 接口已弃用.
	*/
    virtual bool createBooleanCommon(const std::string& mainEntry, const std::string& toolEntry, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createBooleanFuse(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 接口已弃用.
	*/
    virtual bool createBooleanFuse(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createBooleanNonReg(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief 接口已弃用.
	*/
    virtual bool createBooleanNonReg(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity);

    virtual bool createIsoTrapezoid(double* wcsMatrix, double topwidth, double width, double height,
        LOGICAL iscovered, void*& ent);
    //std::vector<TDF_Label> m_labels;
    virtual bool splitWireBypoint(void* entry, std::vector<double> insertTs, const char* name, std::string& strPID, void*& entity);

    virtual bool splitFaceByEdge(void* entry, SplitLineCreationInfos infos, const char* name,
        std::string& strPID, void*& entity);

    virtual bool splitBodyByFace(void* entry, SplitFaceCreationInfos infos, const char* name,
        std::string& strPID, void*& entity);

    virtual bool insertHardPoints(void* entry, void* edge, std::vector<cmtPoint> points, const char* name,
        std::string& strPID, void*& entity);
};


#endif
