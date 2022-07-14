/*! \file FaceOperationBuilder.h
*   \brief FaceOperationBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef FACEOPERATIONBUILDER_H
#define FACEOPERATIONBUILDER_H

#include "ShapeOperationBuilder.h"

#include <NCollection_DataMap.hxx>
#include <NCollection_List.hxx>
#include <TopTools_ListOfShape.hxx>

/*! \class FaceOperationBuilder
*   \brief ���ڶ�����еĲ�����
 */
class FaceOperationBuilder :
	public ShapeOperationBuilder
{
public:
	/*! \fn FaceOperationBuilder()
	*  \brief FaceOperationBuilder�Ĺ��캯��.
	*/
	FaceOperationBuilder();
	/*! \fn ~FaceOperationBuilder()
	*  \brief FaceOperationBuilder����������.
	*/
	~FaceOperationBuilder();

public:
	/*! \fn bool faceCover(TopTools_ListOfShape wires)
	*  \brief �߿�cover�Ľӿ�.
	*  \param wires ���н���cover���߿�.
	*  \return �Ƿ�ɹ�����cover.
	*/
	bool faceCover(TopTools_ListOfShape wires);

	/*! \fn bool faceUncover(TopTools_ListOfShape wires)
	*  \brief �߿�uncover�Ľӿ�.
	*  \param face ����uncover����.
	*  \return �Ƿ�ɹ�����uncover.
	*/
	bool faceUncover(TopoDS_Shape face);

	/*! \fn bool faceTrim(TopoDS_Shape face, TopTools_ListOfShape wires)
	*  \brief ����ü��Ľӿ�.
	*  \param face ���вü�����.
	*  \param wires ���вü��Ĺ����߿�.
	*  \return �Ƿ�ɹ����вü�.
	*/
	bool faceTrim(TopoDS_Shape face, TopTools_ListOfShape wires);

	/*! \fn bool faceExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont,const bool& inU, const bool& forward)
	*  \brief �����ӳ��Ľӿ�.
	*  \param shape ��Ҫ�����ӳ�����.
	*  \param length �ӳ��ĳ���.
	*  \param Cont �ӳ�����������.
	*  \param inU �Ƿ���U�������ΪV��������ӳ�.
	*  \param forward �Ƿ�Ϊ��ǰ����.
	*  \return �Ƿ�ɹ������ӳ�.
	*/
	bool faceExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont,const bool& inU, const bool& forward);

	/*! \fn bool faceSectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const int& method)
	*  \brief ���������󽻵�����.
	*  \param shape1 ��ģ�͵Ķ���.
	*  \param shape2 ����ģ�͵Ķ���.
	*  \param method �����㷨��id��ֻ����1����2. 1����BOPAlgo_Section��2����BRepAlgoAPI_IntCS, 3����BRepAlgoAPI_IntSS.
	*  \return �����Ƿ�ɹ�.
	*/
	bool faceIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const int& method);
};

#endif