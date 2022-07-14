#ifndef EDGEOPERATIONBUILDER_H
#define EDGEOPERATIONBUILDER_H

#include "ShapeOperationBuilder.h"

#include <NCollection_DataMap.hxx>
#include <NCollection_List.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Wire.hxx>

/*! \class EdgeOperationBuilder
*   \brief 用于对面进行的操作等
 */
class EdgeOperationBuilder :
	public ShapeOperationBuilder
{
public:
	/*! \fn EdgeOperationBuilder()
	*  \brief EdgeOperationBuilder的构造函数.
	*/
	EdgeOperationBuilder();
	/*! \fn ~EdgeOperationBuilder()
	*  \brief EdgeOperationBuilder的析构函数.
	*/
	~EdgeOperationBuilder();

public:
	/*! \fn bool trimCurve(const TopoDS_Shape& origEdge, const double startU, const double endU)
	*  \brief 曲线裁剪的接口.
	*  \param origEdge 需要进行裁剪的线.
	*  \param startU 裁剪的起始U值.
	*  \param endU 裁剪的终止U值.
	*  \return 是否成功进行裁剪.
	*/
	bool trimCurve(const TopoDS_Shape& origEdge, const double startU, const double endU);

	/*! \fn bool trimCurveAdaptEnd(const TopoDS_Shape& origEdge, const double endU)
	*  \brief 曲线裁剪的接口. 起始点不变而仅修改终点.
	*  \param origEdge 需要进行裁剪的线.
	*  \param endU 裁剪的终止U值.
	*  \return 是否成功进行裁剪.
	*/
	bool trimCurveAdaptEnd(const TopoDS_Shape& origEdge, const double endU);

	/*! \fn bool edgeExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont, const bool& forward)
	*  \brief 曲线延长的接口.
	*  \param shape 需要进行延长的线.
	*  \param length 延长的长度.
	*  \param Cont 延长的连续级别.
	*  \param forward 是否向前延长.
	*  \return 是否成功进行延长.
	*/
	bool edgeExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont, const bool& forward);

	/*! \fn bool edgeClosedWire(TopTools_ListOfShape edges)
	*  \brief 多个曲线封闭成线框的接口.
	*  \param edges 需要进行封闭的线链表.
	*  \return 是否成功创建模型.
	*/
	bool edgeClosedWire(TopTools_ListOfShape edges);

	/*! \fn bool wireRebuild(TopoDS_Wire wire)
	*  \brief 重构wire.
	*  \param wire 需要进行重构的wire.
	*  \return 是否成功重构.
	*/
	bool wireRebuild(TopoDS_Wire wire);

	/*! \fn bool edgesIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol)
	*  \brief 线线相交得到交点的接口.
	*  \param shape1 需要进行相交的线1.
	*  \param shape2 需要进行相交的线2.
	*  \param tol 判断相交精度.
	*  \return 是否成功进行创建.
	*/
	bool edgesIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol);

	/*! \fn bool edgesCombinationOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol)
	*  \brief 曲线相连的接口.
	*  \param shape1 需要进行相连的线1.
	*  \param shape2 需要进行相连的线2.
	*  \param tol 判断相交精度.
	*  \return 是否成功进行创建.
	*/
	bool edgesCombinationOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol);


};

#endif