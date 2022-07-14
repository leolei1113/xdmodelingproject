/*! \file ShapeUtilities.h
*   \brief ShapeUtilities类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef SHAPEUTILITIES_H
#define SHAPEUTILITIES_H

#include "xdModelingDataType.h"
#include "xdModelingHeader.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_ListOfShape.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_Plane.hxx>
#include <TopTools_ListOfShape.hxx>

#include <vector>

enum CoinsideType
{
    Containing,
    Contained,
    PartCoinside,
    NotConside,
    Same,
    Neither
};

/*! \class ShapeUtilities
*   \brief 用于存放一些关于TopoDS_Shape的静态方法
 */
class XDMODELING_API ShapeUtilities
{
public:
    /*! \fn ShapeUtilities()
    *  \brief ShapeUtilities的构造函数.
    */
    ShapeUtilities();

    /*! \fn ~ShapeUtilities()
    *  \brief ShapeUtilities的析构函数.
    */
    ~ShapeUtilities();

    /*! \fn static bool shapeFilter(TopoDS_Shape& origShape, TopAbs_ShapeEnum targetType)
    *  \brief 过滤模型的接口，剥除拓扑结构中仅有一个子拓扑的层级，直到达到目标层级，如果在未达到之前就已经有多个子拓扑则返回false.
    *  \param origShape 需要进行过滤的TopoDS_Shape对象.
    *  \param targetType 目标拓扑级别.
    *  \param filterShape 过滤完的拓扑.
    *  \return 返回是否过滤成功.
    */
    static bool shapeFilter(const TopoDS_Shape& origShape, TopAbs_ShapeEnum targetType, TopoDS_Shape& filterShape);

    /*! \fn static bool shapeCover(const TopoDS_Shape& origShape, TopoDS_Shape& coveredShape)
    *  \brief 模型拓扑提升一级的接口.
    *  \param origShape 需要进行升级的TopoDS_Shape对象.
    *  \param coveredShape 升级完的拓扑.
    *  \return 返回是否升级成功.
    */
    static bool shapeCover(const TopoDS_Shape& origShape, TopoDS_Shape& coveredShape);

    /*! \fn static TopoDS_ListOfShape findEdgeOwnerFaces(const TopoDS_Shape& bdy, const TopoDS_Shape& edge)
    *  \brief 在body中查找edge所在所有平面.
    *  \param bdy 主体body的TopoDS_Shape对象.
    *  \param edge 需要进行查找的edge的TopoDS_Shape对象.
    *  \return 返回查找到的所有平面.
    */
    static TopoDS_ListOfShape findEdgeOwnerFaces(const TopoDS_Shape& bdy, const TopoDS_Shape& edge);

    /*! \fn static TopoDS_Shape shapeTransformFromLocalToGlobal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false)
    *  \brief 根据局部坐标系将模型转换到全局坐标系中的接口.
    *  \param localCoor 局部坐标系矩阵.
    *  \param shape 修改位置的模型.
    *  \param isCopy 是否拷贝，默认不拷贝
    *  \return 完成位置修改的模型.
    */
    static TopoDS_Shape shapeTransformFromLocalToGlobal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false);

    /*! \fn static TopoDS_Shape shapeTransformFromGlobalToLocal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false)
    *  \brief 根据局部坐标系将模型转换到局部坐标系中的接口.
    *  \param localCoor 局部坐标系矩阵.
    *  \param shape 修改位置的模型.
    *  \param isCopy 是否拷贝，默认不拷贝
    *  \return 完成位置修改的模型.
    */
    static TopoDS_Shape shapeTransformFromGlobalToLocal(double* localCoor, const TopoDS_Shape& shape, const bool& isCopy = false);

    /*! \fn static void vectorTransformFromLocalToGlobal(double* localCoor, const double& sourceVecX, const double& sourceVecY, const double& sourceVecZ, double& xVec, double& yVec, double& zVec)
    *  \brief 将向量从局部坐标系转到全局中的接口.
    *  \param sourceVecX 转换前向量x方向分量.
    *  \param sourceVecY 转换前向量y方向分量.
    *  \param sourceVecZ 转换前向量z方向分量.
    *  \param xVec 转换后向量x方向分量.
    *  \param yVec 转换后向量y方向分量.
    *  \param zVec 转换后向量z方向分量.
    */
    static void vectorTransformFromLocalToGlobal(double* localCoor, const double& sourceVecX, const double& sourceVecY, const double& sourceVecZ,
        double& xVec, double& yVec, double& zVec);

    /*! \fn static void pntTransformFromLocalToGlobal(double* localCoor, const double& sourceX, const double& sourceY, const double& sourceZ, double& x, double& y, double& z)
    *  \brief 将点坐标从局部坐标系转到全局中的接口.
    *  \param sourceX 转换前点坐标x方向分量.
    *  \param sourceY 转换前点坐标y方向分量.
    *  \param sourceZ 转换前点坐标z方向分量.
    *  \param x 转换后点坐标x方向分量.
    *  \param y 转换后点坐标y方向分量.
    *  \param z 转换后点坐标z方向分量.
    */
    static void pntTransformFromLocalToGlobal(double* localCoor,
        const double& sourceX, const double& sourceY, const double& sourceZ,
        double& x, double& y, double& z);

    /*! \fn static void getAxis(const DATUMAXISTYPE& axis, double& xAxis, double& yAxis, double& zAxis)
    *  \brief 根据枚举类型获取目标轴的x、y、z分量的接口.
    *  \param axis 坐标轴枚举.
    *  \param x 返回坐标轴x分量.
    *  \param y 返回坐标轴y分量.
    *  \param z 返回坐标轴z分量.
    */
    static void getAxis(const DATUMAXISTYPE& axis, double& xAxis, double& yAxis, double& zAxis, double isanti);

    /*! \fn static void getRadius(const std::vector<std::vector<double>>& points, double& xCenter, double& yCenter, double& zCenter, double& radius)
    *  \brief 根据三点计算圆心和半径的接口.
    *  \param points 三点坐标.
    *  \param xCenter 圆心x坐标.
    *  \param yCenter 圆心y坐标.
    *  \param zCenter 圆心z坐标.
    *  \param radius 半径.
    */
    static void getRadius(const std::vector<std::vector<double>>& points, double& xCenter, double& yCenter, double& zCenter, double& radius);


    /*! \fn static bool isShapePlannar(const TopoDS_Shape& shape, gp_Pln& plane)
    *  \brief 判断模型是否是平面的接口，并获取该平面.
    *  \param shape 进行判断的模型.
    *  \param plane 如果是平面时返回的几何平面.
    *  \return 是否是平面模型.
    */
    static bool isShapePlannar(const TopoDS_Shape& shape, gp_Pln& plane);

	/*! \fn static int getNumTopoType(const TopoDS_Shape& shape, const TopAbs_ShapeEnum& type)
	*  \brief 获取模型的某级拓扑的树木.
	*  \param shape 进行获取的模型.
	*  \param type 需要判断的拓扑类型.
	*  \return 拓扑数目.
	*/
	static int getNumTopoType(const TopoDS_Shape& shape, const TopAbs_ShapeEnum& type);

    /*! \fn static bool FindConnectedEdges(const TopoDS_Shape& F, const TopoDS_Vertex& V, TopoDS_Edge& E1, TopoDS_Edge& E2)
    *  \brief 获取模型中，共点的两条线.
    *  \param F 进行查找的模型.
    *  \param V 点的模型拓扑.
    *  \param edges 共点线集合.
    *  \return 是否能找到共点线.
    */
    static bool FindConnectedEdges(const TopoDS_Shape& F/*orig const TopoDS_Face& F*/, const TopoDS_Vertex& V, TopTools_ListOfShape& edges);

    /*! \fn static bool PlanarBaryCalculate(const TopoDS_Shape& planarShape, gp_XYZ& bary)
    *  \brief 简单获取模型中心的方法，通过凭据顶点坐标.
    *  \param planarShape 进行处理的模型.
    *  \param bary 计算出的模型中心.
    *  \return 是否成功计算.
    */
    static bool PlanarBaryCalculate(const TopoDS_Shape& planarShape, gp_XYZ& bary);

    static bool GetCircleCenter(Handle(Geom_Curve) curve, gp_Pnt& pnt);

    static void GetAllNonCompoundShapes(const TopoDS_Shape& orgShape, TopTools_ListOfShape& collections);

    static void GetAllNonCompoundShapes(const TopoDS_Shape& orgShape, TopTools_ListOfShape& solids, TopTools_ListOfShape& faces, TopTools_ListOfShape& edges);

    static void GetAllFaces(const TopoDS_Shape& orgShape, TopTools_ListOfShape& collections);

    static void GetShapeTypeRange(const TopTools_ListOfShape& shapes, TopAbs_ShapeEnum& lower, TopAbs_ShapeEnum& upper);

    static Handle(Geom_Surface) GetBasicSurface(Handle(Geom_Surface) surf, GeomAbs_SurfaceType& type);

    static Handle(Geom_Curve) GetBasicCurve(Handle(Geom_Curve) curve);

    static bool GetPntUVonFace(TopoDS_Shape& face, gp_Pnt& pnt, double& UValue, double& VValue);

    static bool GetPntUVonFace(TopoDS_Shape face, TopoDS_Shape edge, gp_Pnt& pnt, double& UValue, double& VValue);

    static bool GetPntUVonEdge(TopoDS_Shape edge, gp_Pnt& pnt, double& UValue);

    static bool IsShapeGeomSame(const TopoDS_Shape shape1, const TopoDS_Shape shape2, const TopAbs_ShapeEnum type);

    static void FindAllCells(const TopoDS_Shape shape, TopTools_ListOfShape& collections);

    static bool ConvertBoundedCurve(TopoDS_Shape edge, Handle(Geom_BoundedCurve)& BndCurve);

    static bool SameDimension(const TopTools_ListOfShape collections, int& minDim, int& maxDim);

    static bool FaceNormal(const TopoDS_Face face, const double& u, const double& v, gp_Dir& dir);

    static bool FaceMiddleNormal(const TopoDS_Face face, gp_Dir& dir);

    static bool EdgeIsLinear(const TopoDS_Edge edge);

    static bool ReverseFaceNormal4CellsBuild(const TopTools_ListOfShape cellsArgs, const TopTools_ListOfShape cellsTools,
        TopTools_ListOfShape& newcellsArgs, TopTools_ListOfShape& newcellsTools);

    static CoinsideType IsEdgeCoincide(const TopoDS_Shape blankedge, const TopoDS_Shape tooledge, const double tolerance);

    static void BoxEFSort(const TopoDS_Shape box, TopTools_IndexedMapOfShape& faces, TopTools_IndexedMapOfShape& edges);

    static void GetMultishellSolids(const TopTools_ListOfShape solids, TopTools_ListOfShape& singleShellSolid, TopTools_ListOfShape& multiShellSolid);

    static void UnifyAllFaceNormal(const gp_Dir refDir, TopTools_ListOfShape& cellsArgs);

    static void FindAllCellsWithoutCheck(const TopoDS_Shape shape, TopTools_ListOfShape& collections);

    static bool IsShapePlannarNumericalSolution(const TopoDS_Shape shape, gp_Pln& plane, double precision);

    static bool BuildSurfaceEdge(TopoDS_Shape owner, TopoDS_Shape preshape, TopoDS_Shape curshape, std::tuple<double, double> preValue,
        std::tuple<double, double> curValue, TopoDS_Edge& targetedge);

    static bool GetUVPoint(double normalizedT, TopoDS_Edge edge, TopoDS_Face face,
        bool& firstpcurve, bool& secondpcurve, gp_Pnt2d& p);

    static void GetUVPoint(const double& normalizedU, const double& normalizedV, TopoDS_Face face, gp_Pnt2d& p);

    static bool shapeFilter(TopoDS_Shape& origshape, TopAbs_ShapeEnum targettype,
        TopoDS_Shape& filtershape);

    static bool GetPntUonEdge(TopoDS_Shape edge, gp_Pnt pnt, double& uvalue);
};

#endif
