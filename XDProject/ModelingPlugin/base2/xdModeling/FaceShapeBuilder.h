/*! \file FaceShapeBuilder.h
*   \brief FaceShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef FACESHAPEBUILDER_H
#define FACESHAPEBUILDER_H

#include "OccShapeBuilder.h"

#include <TopoDS_Wire.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <Standard_Integer.hxx>

#include <vector>

/*! \class FaceShapeBuilder
*   \brief 用于创建各类face的类
 */
class FaceShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn FaceShapeBuilder()
    *  \brief FaceShapeBuilder的构造函数.
    */
    FaceShapeBuilder();
    /*! \fn ~FaceShapeBuilder()
    *  \brief FaceShapeBuilder的析构函数.
    */
    ~FaceShapeBuilder();

public:
    /*! \fn bool createRectFace(const double& xPos, const double& yPos, const double& zPos,  const double& xAxis, const double& yAxis, const double&zAxis, const double& width, const double& height)
    *  \brief 根据顶点和长宽创建长方形.
    *  \param xPos 起点x坐标.
    *  \param yPos 起点y坐标.
    *  \param zPos 起点z坐标.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \param width 宽度.
    *  \param height 高度.
    *  \return 是否成功创建.
    */
    bool createRectFace(const double& xPos, const double& yPos, const double& zPos, 
        const double& xAxis, const double& yAxis, const double&zAxis, 
        const double& width, const double& height);

    /*! \fn bool createCircleFace(const double& centerX, const double& centerY, const double& centerZ, const double& xAxis, const double& yAxis, const double&zAxis, const double& radius)
    *  \brief 创建圆面.
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \param radius 半径.
    *  \return 是否成功创建.
    */
    bool createCircleFace(const double& centerX, const double& centerY, const double& centerZ,
        const double& xAxis, const double& yAxis, const double&zAxis,
        const double& radius);

    /*! \fn  bool createSectorFace(const double& xCenter, const double& yCenter, const double& zCenter,
		const double& xAxis, const double& yAxis, const double&zAxis,
		const double& radius, const double& startAng, const double& endAng)
    *  \brief 根据圆心位置、半径和角度创建扇形面，已弃用.
    */
	bool createSectorFace(const double& xCenter, const double& yCenter, const double& zCenter,
		const double& xAxis, const double& yAxis, const double&zAxis,
		const double& radius, const double& startAng, const double& endAng);

    /*! \fn bool createEllipseFace(const double& centerX, const double& centerY, const double& centerZ, const double& xAxis, const double& yAxis, const double&zAxis, const double& majorRadius, const double& minorRadius)
    *  \brief 根据圆心位置和长短轴半径创建椭圆面.
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \param majorRadius 长轴半径.
    *  \param minorRadius 短轴比例.
    *  \return 是否成功创建.
    */
    bool createEllipseFace(const double& centerX, const double& centerY, const double& centerZ,
        const double& xAxis, const double& yAxis, const double&zAxis,
        const double& majorRadius, const double& minorRadius);

    /*! \fn bool createCylinderFace(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height)
    *  \brief 创建圆柱侧面.
    *  \param xBottomCenter 底面圆心x坐标.
    *  \param yBottomCenter 底面圆心y坐标.
    *  \param zBottomCenter 底面圆心z坐标.
    *  \param xAxis 圆柱朝向向量x值.
    *  \param yAxis 圆柱朝向向量y值.
    *  \param zAxis 圆柱朝向向量z值.
    *  \param radius 半径.
    *  \param height 圆柱高度.
    *  \return 是否成功创建.
    */
    bool createCylinderFace(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height);

    /*! \fn bool createConeFace(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建圆锥和圆台侧面.
    *  \param botCenterX 底面圆心x坐标.
    *  \param botCenterY 底面圆心y坐标.
    *  \param botCenterZ 底面圆心z坐标.
    *  \param lowerRadius 底面半径.
    *  \param upperRadius 顶面半径，为0即圆锥面.
    *  \param height 圆柱高度.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \return 是否成功创建.
    */
    bool createConeFace(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createPolygon(const std::vector<std::vector<double>>& pPos)
    *  \brief 创建多边形平面，参数为顶点坐标.
    *  \param points 共面的多边形顶点坐标.
    *  \return 是否成功创建.
    */
	bool createPolygon(const std::vector<std::vector<double>>& pPos);

    /*! \fn bool createRegularPolygon(const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const int& numSides, double xAxis, double yAxis, double zAxis
    *  \brief 创建正多边形平面
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param startX 起点x坐标.
    *  \param startY 起点y坐标.
    *  \param startZ 起点z坐标.
    *  \param numSides 正多边形边的数目.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \return 是否成功创建.
    */
	bool createRegularPolygon(const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const int& numSides, double xAxis, double yAxis, double zAxis);

	/*! \fn bool createParabolaFace(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
	*  \brief 创建抛物面，通过抛物面旋转得到.
	*  \param points 中心点坐标.
	*  \param focalDistance 焦距.
	*  \param startP 双曲线起始t值.
	*  \param endP 双曲线终止t值.
	*  \param xAxis 所在平面法向向量x值.
	*  \param yAxis 所在平面法向向量y值.
	*  \param zAxis 所在平面法向向量z值.
	*  \return 是否成功创建.
	*/
	bool createParabolaFace(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP,
		const double& xAxis, const double& yAxis, const double& zAxis);

	/*! \fn bool createFuncSurface(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, const std::string varUName, const std::string varVName, const double startU, const double endU, const double startV, const double endV)
	*  \brief 创建函数曲面.
	*  \param xFuncExpress 变量关于x坐标的变化方程.
	*  \param yFuncExpress 变量关于y坐标的变化方程.
	*  \param zFuncExpress 变量关于z坐标的变化方程.
	*  \param varUName U变量名称.
	*  \param varVName V变量名称.
	*  \param startU 曲线起始U值.
	*  \param endU 曲线起始U值.
	*  \param startV 曲线起始V值.
	*  \param endV 曲线起始V值.
	*  \return 是否成功创建.
	*/
	bool createFuncSurface(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress,
		const std::string varUName, const std::string varVName, const double startU, const double endU, const double startV, const double endV);

    // 根据封闭线框创建面
    /*! \fn bool createClosedFace(TopoDS_Wire closedWire)
    *  \brief 根据封闭线框创建面，接口已弃用.
    */
    bool createClosedFace(TopoDS_Wire closedWire);

	/*! \fn bool createNurbsSurface(const TColgp_Array2OfPnt& Poles, const TColStd_Array2OfReal& Weights, const TColStd_Array1OfReal& UKnots, const TColStd_Array1OfReal& VKnots, const TColStd_Array1OfInteger& UMults, const TColStd_Array1OfInteger& VMults, const Standard_Integer UDegree, const Standard_Integer VDegree)
	*  \brief 创建Nurbs面的接口.
	*  \param Poles 控制点二维数组.
	*  \param Weights 权重数组, 如果全为1则构建非有理样条线.
	*  \param UKnots U方向节点.
	*  \param VKnots V方向节点.
	*  \param UMults U方向重复节点.
	*  \param VMults V方向重复节点.
	*  \param UDegree U方向自由度.
	*  \param VDegree V方向自由度.
	*  \return 是否成功创建.
	*/
	bool createNurbsSurface(const TColgp_Array2OfPnt& Poles, const TColStd_Array2OfReal& Weights, 
		const TColStd_Array1OfReal& UKnots, const TColStd_Array1OfReal& VKnots, 
		const TColStd_Array1OfInteger& UMults, const TColStd_Array1OfInteger& VMults, 
		const Standard_Integer UDegree, const Standard_Integer VDegree);

    bool createInterpolateBSplineSurface(TColgp_Array2OfPnt poles);
};

#endif
