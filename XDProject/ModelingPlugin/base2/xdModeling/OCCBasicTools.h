/*! \file OCCBasicTools.h
*   \brief OCCBasicTools类的头文件
*   \author HST
*   \date 2020.10.8
*/

#pragma once
#include "xdModelingHeader.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_ListOfShape.hxx>
#include <gp_Pnt.hxx>
#include <vector>

/*! \class OCCBasicTools
*   \brief 用于OCC基础工具的静态方法。
 */
class XDMODELING_API OCCBasicTools
{
public:
    /*! \fn OCCBasicTools()
    *  \brief OCCBasicTools的构造函数.
    */
	OCCBasicTools();

    /*! \fn ~OCCBasicTools()
    *  \brief OCCBasicTools的析构函数.
    */
	~OCCBasicTools();

    /*! \fn static TopoDS_Face GetFaceFromPntList(std::vector<gp_Pnt> PntsList)
    *  \brief 根据散点创建封闭线框从而生成平面的接口.
    *  \param PntsList 平面顶点的gp_Pnt对象链表.
    *  \return 返回创建平面TopoDS_Face对象.
    */
	static TopoDS_Face GetFaceFromPntList(std::vector<gp_Pnt> PntsList);

    /*! \fn static Standard_Boolean GetOrderWireFromEdges(std::vector<TopoDS_Edge> anEdges, TopoDS_Wire& OrderWire)
    *  \brief 根据边创建线框的接口.
    *  \param anEdges 进行创建的边的链表.
    *  \param OrderWire 返回创建点线框.
    *  \return 返回是否成功创建.
    */
	static Standard_Boolean GetOrderWireFromEdges(std::vector<TopoDS_Edge> anEdges, TopoDS_Wire& OrderWire);

    /*! \fn static bool DistanceMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& minDis)
	*  \brief 计算模型间距离的接口.
	*  \param shape1 模型1.
	*  \param shape2 模型2.
	*  \param minDis 计算出的模型间的距离.
	*  \param pnt1 最近距离下在模型1上的点坐标.
	*  \param pnt2 最近距离下在模型2上的点坐标.
	*  \return 返回是否成功计算.
    */
    static bool DistanceMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& minDis, gp_Pnt& pnt1, gp_Pnt& pnt2);

    /*! \fn static bool AngleMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& angle)
	*  \brief 计算直线模型间距离的接口.
	*  \param shape1 模型1.
	*  \param shape2 模型2.
	*  \param angle 计算出的模型间的角度.
	*  \return 返回是否成功计算.
	*/
	static bool AngleMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& angle);

    /*! \fn static bool GetBndBoxParams(TopoDS_Shape shape, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax)
    *  \brief 获取某一模型的包围盒信息.
	*  \param shape 目标模型.
	*  \param theXmin 包围盒最小X坐标.
	*  \param theYmin 包围盒最小Y坐标.
	*  \param theZmin 包围盒最小Z坐标.
	*  \param theXmax 包围盒最大X坐标.
	*  \param theYmax 包围盒最大Y坐标.
	*  \param theZmax 包围盒最大Z坐标.
	*  \return 返回是否成功计算.
	*/
	static bool GetBndBoxParams(TopoDS_Shape shape, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax);

	/*! \fn static bool GetBndBoxParams(TopTools_ListOfShape shapes, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax)
	*  \brief 获取多个模型的包围盒信息.
	*  \param shapes 目标模型数据链表.
	*  \param theXmin 包围盒最小X坐标.
	*  \param theYmin 包围盒最小Y坐标.
	*  \param theZmin 包围盒最小Z坐标.
	*  \param theXmax 包围盒最大X坐标.
	*  \param theYmax 包围盒最大Y坐标.
	*  \param theZmax 包围盒最大Z坐标.
	*  \return 返回是否成功计算.
	*/
	static bool GetBndBoxParams(TopTools_ListOfShape shapes, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax);

    /*! \fn static bool GetEdgeOrFaceGradients(TopoDS_Shape shape, const std::vector<double>& inParams, const int& level, std::vector<double>& pos, std::vector<double>& gradients)
	*  \brief 根据U、V参数计算线或者面的梯度.
	*  \param shapes 线或者面的数据.
	*  \param inParams U、V参数, 如果是线则只有一个数, 面有两个数.
	*  \param level 梯度的阶数.
	*  \param pos U、V参数标定的点的三维坐标.
	*  \param gradients U、V参数处的梯度.
	*  \return 返回是否成功计算.
	*/
	static bool GetEdgeOrFaceGradients(TopoDS_Shape shape, const std::vector<double>& inParams, const int& level, std::vector<double>& pos, std::vector<double>& gradients);

    /*! \fn static bool GetShapeVolume(TopoDS_Shape shape, double& volume)
	*  \brief 计算模型体积.
	*  \param shape 需要进行计算的模型.
	*  \param volume 计算得到的体积.
	*  \return 返回是否成功计算.
	*/
    static bool GetShapeVolume(TopoDS_Shape shape, double& volume);

	/*! \fn static bool createCompound(TopTools_ListOfShape shapes, TopoDS_Shape& target)
	*  \brief 将所有模型创建成一个装配体.
	*  \param shapes 所有模型零件.
	*  \param target 装配体模型.
	*  \return 返回是否成功装配.
	*/
	static bool createCompound(TopTools_ListOfShape shapes, TopoDS_Shape& target);

	/*! \fn static bool calcCurveCurvature(TopoDS_Shape shape, const double& T, double& curvature, gp_Pnt& centerOfCurvature)
	*  \brief 计算曲线在U值处的曲率和圆心坐标的接口.
	*  \param shape 需要进行计算的模型.
	*  \param T 目标U值.
	*  \param curvature 计算得到的曲率.
	*  \param centerOfCurvature 曲率圆心坐标.
	*  \return 返回是否成功计算.
	*/
	static bool calcCurveCurvature(TopoDS_Shape shape, const double& T, double& curvature, gp_Pnt& centerOfCurvature);

	/*! \fn static bool calcSurfCurvature(TopoDS_Shape shape, const double& U, const double& V, double& maxCurvature)
	*  \brief 计算曲面在U、V值处的最大曲率的接口.
	*  \param shape 需要进行计算的模型.
	*  \param U 目标U值.
	*  \param V 目标V值.
	*  \param maxCurvature 计算得到的最大曲率.
	*  \return 返回是否成功计算.
	*/
	static bool calcSurfCurvature(TopoDS_Shape shape, const double& U, const double& V, double& maxCurvature);

	//选面拔模
	static bool DraftFacesOnModel(TopoDS_Shape& muttershape, TopTools_ListOfShape facestodraft, double draftangle, gp_Pln Neutral, gp_Dir pullingdiraction);

	static bool AutoBuildSolidByFaces(TopTools_ListOfShape facelist, TopoDS_Shape& buildsolid);

	static bool RemoveHolesWithinAreaAndDetectFaces(TopoDS_Shape muttershape, TopoDS_Shape& output, TopTools_ListOfShape& facelist, double inputarea);

	static bool GetArcTopologyCenter(TopoDS_Edge edge, gp_Pnt center);

	static bool ComputeNURBSKnotsMults(std::vector<double> src, TColStd_Array1OfReal knots, TColStd_Array1OfInteger mults);

	static bool DetectSliverFaces(TopoDS_Shape ent, double tolerance, std::vector<TopoDS_Shape>& sliverfaces);

	static bool RemoveFace(TopoDS_Shape mutter_3d, TopTools_ListOfShape facestoremove, TopoDS_Shape& result);
};


