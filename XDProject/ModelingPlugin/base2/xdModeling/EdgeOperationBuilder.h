#ifndef EDGEOPERATIONBUILDER_H
#define EDGEOPERATIONBUILDER_H

#include "ShapeOperationBuilder.h"

#include <NCollection_DataMap.hxx>
#include <NCollection_List.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Wire.hxx>

/*! \class EdgeOperationBuilder
*   \brief ���ڶ�����еĲ�����
 */
class EdgeOperationBuilder :
	public ShapeOperationBuilder
{
public:
	/*! \fn EdgeOperationBuilder()
	*  \brief EdgeOperationBuilder�Ĺ��캯��.
	*/
	EdgeOperationBuilder();
	/*! \fn ~EdgeOperationBuilder()
	*  \brief EdgeOperationBuilder����������.
	*/
	~EdgeOperationBuilder();

public:
	/*! \fn bool trimCurve(const TopoDS_Shape& origEdge, const double startU, const double endU)
	*  \brief ���߲ü��Ľӿ�.
	*  \param origEdge ��Ҫ���вü�����.
	*  \param startU �ü�����ʼUֵ.
	*  \param endU �ü�����ֹUֵ.
	*  \return �Ƿ�ɹ����вü�.
	*/
	bool trimCurve(const TopoDS_Shape& origEdge, const double startU, const double endU);

	/*! \fn bool trimCurveAdaptEnd(const TopoDS_Shape& origEdge, const double endU)
	*  \brief ���߲ü��Ľӿ�. ��ʼ�㲻������޸��յ�.
	*  \param origEdge ��Ҫ���вü�����.
	*  \param endU �ü�����ֹUֵ.
	*  \return �Ƿ�ɹ����вü�.
	*/
	bool trimCurveAdaptEnd(const TopoDS_Shape& origEdge, const double endU);

	/*! \fn bool edgeExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont, const bool& forward)
	*  \brief �����ӳ��Ľӿ�.
	*  \param shape ��Ҫ�����ӳ�����.
	*  \param length �ӳ��ĳ���.
	*  \param Cont �ӳ�����������.
	*  \param forward �Ƿ���ǰ�ӳ�.
	*  \return �Ƿ�ɹ������ӳ�.
	*/
	bool edgeExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont, const bool& forward);

	/*! \fn bool edgeClosedWire(TopTools_ListOfShape edges)
	*  \brief ������߷�ճ��߿�Ľӿ�.
	*  \param edges ��Ҫ���з�յ�������.
	*  \return �Ƿ�ɹ�����ģ��.
	*/
	bool edgeClosedWire(TopTools_ListOfShape edges);

	/*! \fn bool wireRebuild(TopoDS_Wire wire)
	*  \brief �ع�wire.
	*  \param wire ��Ҫ�����ع���wire.
	*  \return �Ƿ�ɹ��ع�.
	*/
	bool wireRebuild(TopoDS_Wire wire);

	/*! \fn bool edgesIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol)
	*  \brief �����ཻ�õ�����Ľӿ�.
	*  \param shape1 ��Ҫ�����ཻ����1.
	*  \param shape2 ��Ҫ�����ཻ����2.
	*  \param tol �ж��ཻ����.
	*  \return �Ƿ�ɹ����д���.
	*/
	bool edgesIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol);

	/*! \fn bool edgesCombinationOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol)
	*  \brief ���������Ľӿ�.
	*  \param shape1 ��Ҫ������������1.
	*  \param shape2 ��Ҫ������������2.
	*  \param tol �ж��ཻ����.
	*  \return �Ƿ�ɹ����д���.
	*/
	bool edgesCombinationOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const double& tol);


};

#endif