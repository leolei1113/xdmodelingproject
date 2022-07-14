/*! \file EdgeShapeBuilder.h
*   \brief EdgeShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef EDGESHAPEBUILDER_H
#define EDGESHAPEBUILDER_H

#include "OccShapeBuilder.h"
#include "xdModelingHeader.h"

#include <TopoDS_Edge.hxx>

#include <vector>

/*! \class EdgeShapeBuilder
*   \brief 用于创建各类edge的类
 */
class XDMODELING_API EdgeShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder的构造函数.
    */
    EdgeShapeBuilder();
    /*! \fn ~EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder的析构函数.
    */
    ~EdgeShapeBuilder();

public:
    /*! \fn bool createLine(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2)
    *  \brief 创建线段.
    *  \param xPos1 起点x坐标.
    *  \param yPos1 起点y坐标.
    *  \param zPos1 起点z坐标.
    *  \param xPos2 终点x坐标.
    *  \param yPos2 终点y坐标.
    *  \param zPos2 终点z坐标.
    *  \return 是否成功创建.
    */
    bool createLine(const double& xPos1, const double& yPos1, const double& zPos1,
        const double& xPos2, const double& yPos2, const double& zPos2);

    /*! \fn bool createThreePointArc(std::vector<std::vector<double>>& points)
    *  \brief 创建三点圆弧.
    *  \param points 三点坐标序列.
    *  \return 是否成功创建.
    */
    bool createThreePointArc(std::vector<std::vector<double>>& points);

    /*! \fn bool createAngularArc(std::vector<std::vector<double>>& points, const double& angleInDeg, const double& xAxis, const double& yAxis, const double& zAxis, std::vector<double>& newEndPnt
    *  \brief 创建中心点圆弧.
    *  \param points 三点坐标序列，依次为起点、圆心、终点坐标.
    *  \param angleInDeg 圆弧角度.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \return 是否成功创建.
    */
    bool createAngularArc(std::vector<std::vector<double>>& points, const double& angleInDeg, const double& xAxis, const double& yAxis, const double& zAxis, std::vector<double>& newEndPnt);

    /*! \fn bool createPolyLine(std::vector<std::vector<double>>& points)
    *  \brief 创建多段线.
    *  \param points 多段线各节点坐标.
    *  \return 是否成功创建.
    */
    bool createPolyLine(std::vector<std::vector<double>>& points);

    /*! \fn bool createBSpline(std::vector<std::vector<double>>& points)
    *  \brief 创建B样条曲线.
    *  \param points 各控制点坐标.
    *  \return 是否成功创建.
    */
    bool createBSpline(std::vector<std::vector<double>>& points);

    /*! \fn bool createInterpolateBSpline(std::vector<std::vector<double>>& points)
    *  \brief 创建插值样条.
    *  \param points 各控制点坐标.
    *  \return 是否成功创建.
    */
    bool createInterpolateBSpline(std::vector<std::vector<double>>& points);

    /*! \fn bool createMultiTypeSpline(std::vector<std::vector<double>>& ctrlPoints, int type)
    *  \brief 创建多种样条曲线，包括均匀B样条、准均匀B样条、有理B样条
    *  \param points 各控制点坐标.
    *  \param type 样条曲线类型.
    *  \return 是否成功创建.
    */
    bool createMultiTypeSpline(std::vector<std::vector<double>>& ctrlPoints, int type);

    /*! \fn bool createHyperbola(std::vector<double>& centerP, const double& majorR, const double& minorR, const double& startP, const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建双曲线.
    *  \param points 中心点坐标.
    *  \param majorR 主半径.
    *  \param minorR 次半径.
    *  \param startP 双曲线起始t值.
    *  \param endP 双曲线终止t值.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \return 是否成功创建.
    */
    bool createHyperbola(std::vector<double>& centerP, const double& majorR, const double& minorR, const double& startP,
        const double& endP, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createParabola(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建抛物线.
    *  \param points 中心点坐标.
    *  \param focalDistance 焦距.
    *  \param startP 双曲线起始t值.
    *  \param endP 双曲线终止t值.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \return 是否成功创建.
    */
    bool createParabola(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP,
        const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createHelix(const double& globalXcenter, const double& globalYcenter, const double& globalZcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建空间螺旋线.
    *  \param centerX 起始点圆心x坐标.
    *  \param centerY 起始点圆心y坐标.
    *  \param centerZ 起始点圆心z坐标.
    *  \param lowerRadius 起点圆半径.
    *  \param upperRadius 终点圆半径.
    *  \param height 高度.
    *  \param turns 程数.
    *  \param rightHanded 是否右旋.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \return 是否成功创建.
    */
    bool createHelix(gp_Pnt startpnt, const double& centerX, const double& centerY, const double& centerZ,
        const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded,
        const double& xAxis, const double& yAxis, const double& zAxis);

	/*! \fn bool createHelix(const double& globalXcenter, const double& globalYcenter, const double& globalZcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
	*  \brief 创建空间螺旋线.
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
	*  \param xAxis 所在平面法向向量x值.
	*  \param yAxis 所在平面法向向量y值.
	*  \param zAxis 所在平面法向向量z值.
	*  \return 是否成功创建.
	*/
	bool createHelix(const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ, const double& upperRadius, const double& height, const double& turns, const double& rightHanded,
		const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createSpiral(const double& centerX, const double& centerY, const double& centerZ, const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建平面螺旋线.
    *  \param centerX 中心点x坐标.
    *  \param centerY 中心点y坐标.
    *  \param centerZ 中心点z坐标.
    *  \param lowerRadius 起点圆半径.
    *  \param upperRadius 终点圆半径.
    *  \param turns 程数.
    *  \param rightHanded 是否右旋.
    *  \param xAxis 所在平面法向向量x值.
    *  \param yAxis 所在平面法向向量y值.
    *  \param zAxis 所在平面法向向量z值.
    *  \return 是否成功创建.
    */
    bool createSpiral(const double& centerX, const double& centerY, const double& centerZ, const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis);

	/*! \fn bool createHelix(const double& globalXcenter, const double& globalYcenter, const double& globalZcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
	*  \brief 创建平面螺旋线.
	*  \param centerX 起始点圆心x坐标.
	*  \param centerY 起始点圆心y坐标.
	*  \param centerZ 起始点圆心z坐标.
	*  \param startX 起始点x坐标.
	*  \param startY 起始点y坐标.
	*  \param startZ 起始点z坐标.
	*  \param upperRadius 终点圆半径.
	*  \param turns 程数.
	*  \param rightHanded 是否右旋.
	*  \param xAxis 所在平面法向向量x值.
	*  \param yAxis 所在平面法向向量y值.
	*  \param zAxis 所在平面法向向量z值.
	*  \return 是否成功创建.
	*/
	bool createSpiral(const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ, const double& upperRadius, const double& turns, const double& rightHanded,
		const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createFuncCurve(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, const std::string varName, const double startU, const double endU)
    *  \brief 创建函数曲线.
    *  \param xFuncExpress 变量关于x坐标的变化方程.
    *  \param yFuncExpress 变量关于y坐标的变化方程.
    *  \param zFuncExpress 变量关于z坐标的变化方程.
    *  \param varName 变量名称.
    *  \param startU 曲线起始t值.
    *  \param endU 曲线起始t值.
    *  \return 是否成功创建.
    */
    bool createFuncCurve(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress,
        const std::string varName, const double startU, const double endU);

    bool createEllipseArc(std::vector<double>& centerP, std::vector<double>& axis, const double& majorR,
        const double& minorR, double startAng, double endAng);

};

#endif