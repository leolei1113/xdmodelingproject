/*! \file SolidShapeBuilder.h
*   \brief SolidShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef SOLIDSHAPEBUILDER_H
#define SOLIDSHAPEBUILDER_H

#include "OccShapeBuilder.h"
#include "xdModelingHeader.h"

#include <vector>

/*! \class SolidShapeBuilder
*   \brief 用于创建各类基本实体的类
 */
class XDMODELING_API SolidShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn SolidShapeBuilder()
    *  \brief SolidShapeBuilder的构造函数.
    */
    SolidShapeBuilder();
    /*! \fn ~SolidShapeBuilder()
    *  \brief SolidShapeBuilder的析构函数.
    */
    ~SolidShapeBuilder();

public:
    /*! \fn bool createCuboid(const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize)
    *  \brief 创建长方体实体.
    *  \param xStart 立方体起点x坐标.
    *  \param yStart 立方体起点y坐标.
    *  \param zStart 立方体起点z坐标.
    *  \param xSize 立方体x方向长度.
    *  \param ySize 立方体y方向长度.
    *  \param zSize 立方体z方向长度.
    *  \return 是否成功创建.
    */
    bool createCuboid(const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize);

    /*! \fn bool createCylinder(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height)
    *  \brief 创建圆柱实体.
    *  \param xBottomCenter 底面圆心x坐标.
    *  \param yBottomCenter 底面圆心y坐标.
    *  \param zBottomCenter 底面圆心z坐标.
    *  \param xAxis 中心轴向量x值.
    *  \param yAxis 中心轴向量y值.
    *  \param zAxis 中心轴向量z值.
    *  \param radius 半径.
    *  \param height 高度.
    *  \return 是否成功创建.
    */
    bool createCylinder(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height);

    /*! \fn bool createCone(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建圆锥和圆台实体.
    *  \param botCenterX 底面圆心x坐标.
    *  \param botCenterY 底面圆心y坐标.
    *  \param botCenterZ 底面圆心z坐标.
    *  \param lowerRadius 底面半径.
    *  \param upperRadius 顶面半径，为0即为圆锥.
    *  \param height 高度.
    *  \param xAxis 中心轴向量x值.
    *  \param yAxis 中心轴向量y值.
    *  \param zAxis 中心轴向量z值.
    *  \return 是否成功创建.
    */
    bool createCone(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createSphere(const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus)
    *  \brief 创建球体实体.
    *  \param xCenter 球心x坐标.
    *  \param yCenter 球心y坐标.
    *  \param zCenter 球心z坐标.
    *  \param radius 半径.
    *  \return 是否成功创建.
    */
    bool createSphere(const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus);

    /*! \fn bool createTorus(const double& centerX, const double& centerY, const double& centerZ, const double& minorRadius, const double& majorRadius, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建圆环实体.
    *  \param centerX 圆心x坐标.
    *  \param centerY 圆心y坐标.
    *  \param centerZ 圆心z坐标.
    *  \param minorRadius 小圆半径.
    *  \param majorRadius 大圆半径.
    *  \param xAxis 中心轴向量x值.
    *  \param yAxis 中心轴向量y值.
    *  \param zAxis 中心轴向量z值.
    *  \return 是否成功创建.
    */
    bool createTorus(const double& centerX, const double& centerY, const double& centerZ, const double& minorRadius, const double& majorRadius, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createPrism(const std::vector<std::vector<double>>& pPos, const double& xVec, const double& yVec, const double& zVec)
    *  \brief 创建棱柱实体.
    *  \param pPos 棱柱底面各顶点坐标.
    *  \param xVec 中心轴向向量x值.
    *  \param yVec 中心轴向向量y值.
    *  \param zVec 中心轴向向量z值.
    *  \return 是否成功创建.
    */
    bool createPrism(const std::vector<std::vector<double>>& pPos, const double& xVec, const double& yVec, const double& zVec);

    /*! \fn bool createFrustrumPrism(const std::vector<std::vector<double>>& pPos, const double& scale, const double& xVec, const double& yVec, const double& zVec)
    *  \brief 创建棱台实体.
    *  \param pPos 棱柱底面各顶点坐标.
    *  \param scale 棱柱顶面较底面的缩放比例.
    *  \param xVec 中心轴向向量x值.
    *  \param yVec 中心轴向向量y值.
    *  \param zVec 中心轴向向量z值.
    *  \return 是否成功创建.
    */
    bool createFrustrumPrism(const std::vector<std::vector<double>>& pPos, const double& scale, const double& xVec, const double& yVec, const double& zVec);

    /*! \fnbool createRegularPolyhedron(double xBotCenter, double yBotCenter, double zBotCenter, double xBotStart, double yBotStart, double zBotStart, const double& height, const int& numSides, double xAxis, double yAxis, double zAxis)
    *  \brief 创建正多边形棱柱实体.
    *  \param xBotCenter 底面中心x坐标.
    *  \param yBotCenter 底面中心y坐标.
    *  \param zBotCenter 底面中心z坐标.
    *  \param xBotStart 起点x坐标.
    *  \param yBotStart 起点y坐标.
    *  \param zBotStart 起点z坐标.
    *  \param height 高度.
    *  \param numSides 正多边形边的数目.
    *  \param xAxis 中心轴向量x值.
    *  \param yAxis 中心轴向量y值.
    *  \param zAxis 中心轴向量z值.
    *  \return 是否成功创建.
    */
    bool createRegularPolyhedron(double xBotCenter, double yBotCenter, double zBotCenter, double xBotStart, double yBotStart, double zBotStart, const double& height, const int& numSides, double xAxis, double yAxis, double zAxis);

    /*! \fn bool createFlare(const double& xbotCenter, const double& ybotCenter, const double& zbotCenter, const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth, const double& Height, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建多棱排实体.
    *  \param xbotCenter 底面中心x坐标.
    *  \param ybotCenter 底面中心y坐标.
    *  \param zbotCenter 底面中心z坐标.
    *  \param lowerWidth 底面矩形宽度.
    *  \param lowerDepth 底面矩形长度.
    *  \param upperWidth 顶面矩形宽度.
    *  \param upperDepth 顶面矩形长度.
    *  \param height 高度.
    *  \param xAxis 中心轴向量x值.
    *  \param yAxis 中心轴向量y值.
    *  \param zAxis 中心轴向量z值.
    *  \return 是否成功创建.
    */
    bool createFlare(const double& xbotCenter, const double& ybotCenter, const double& zbotCenter, const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth, const double& height, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createEllipsoid(const double& XbotCenter, const double& YbotCenter, const double& ZbotCenter, const double& majRadius, const double& ratio, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief 创建椭球体
    *  \param XbotCenter 椭球中心x坐标.
    *  \param YbotCenter 椭球中心y坐标.
    *  \param ZbotCenter 椭球中心z坐标.
    *  \param majRadius 主轴长度.
    *  \param ratio 长短轴比例.
    *  \param xAxis 中心轴向量x值.
    *  \param yAxis 中心轴向量y值.
    *  \param zAxis 中心轴向量z值.
    *  \return 是否成功创建.
    */
    bool createEllipsoid(const double& XbotCenter, const double& YbotCenter, const double& ZbotCenter, const double& majRadius, const double& ratio, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createHelixSolid(const double& Xcenter, const double& Ycenter, const double& Zcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& profileRadius, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis);
    *  \brief 创建螺旋线实体.
    *  \param Xcenter 起点圆面圆心x坐标.
    *  \param Ycenter 起点圆面圆心y坐标.
    *  \param Zcenter 起点圆面圆心z坐标.
    *  \param lowerRadius 起点圆半径.
    *  \param upperRadius 终点圆半径.
    *  \param height 高度.
    *  \param turns 程数.
    *  \param profileRadius 截面半径.
    *  \param rightHanded 是否右旋.
    *  \param xAxis 中心轴向量x值.
    *  \param yAxis 中心轴向量y值.
    *  \param zAxis 中心轴向量z值.
    *  \return 是否成功创建.
    */
    bool createHelixSolid(const double& Xcenter, const double& Ycenter, const double& Zcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& profileRadius, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis);

    bool createSpindle(double xbotcenter, double ybotcenter, double zbotcenter,
        double majradius, double ratio, double xaxis, double yaxis, double zaxis);
};

#endif
