/*! \file FaceOperationBuilder.h
*   \brief FaceOperationBuilder类的头文件
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
*   \brief 用于对面进行的操作等
 */
class FaceOperationBuilder :
	public ShapeOperationBuilder
{
public:
	/*! \fn FaceOperationBuilder()
	*  \brief FaceOperationBuilder的构造函数.
	*/
	FaceOperationBuilder();
	/*! \fn ~FaceOperationBuilder()
	*  \brief FaceOperationBuilder的析构函数.
	*/
	~FaceOperationBuilder();

public:
	/*! \fn bool faceCover(TopTools_ListOfShape wires)
	*  \brief 线框cover的接口.
	*  \param wires 所有进行cover的线框.
	*  \return 是否成功进行cover.
	*/
	bool faceCover(TopTools_ListOfShape wires);

	/*! \fn bool faceUncover(TopTools_ListOfShape wires)
	*  \brief 线框uncover的接口.
	*  \param face 进行uncover的面.
	*  \return 是否成功进行uncover.
	*/
	bool faceUncover(TopoDS_Shape face);

	/*! \fn bool faceTrim(TopoDS_Shape face, TopTools_ListOfShape wires)
	*  \brief 曲面裁剪的接口.
	*  \param face 进行裁剪的面.
	*  \param wires 进行裁剪的工具线框.
	*  \return 是否成功进行裁剪.
	*/
	bool faceTrim(TopoDS_Shape face, TopTools_ListOfShape wires);

	/*! \fn bool faceExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont,const bool& inU, const bool& forward)
	*  \brief 曲面延长的接口.
	*  \param shape 需要进行延长的面.
	*  \param length 延长的长度.
	*  \param Cont 延长的连续级别.
	*  \param inU 是否在U方向否则为V方向进行延长.
	*  \param forward 是否为向前延伸.
	*  \return 是否成功进行延长.
	*/
	bool faceExtendOperation(TopoDS_Shape shape, const double& length, const int& Cont,const bool& inU, const bool& forward);

	/*! \fn bool faceSectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const int& method)
	*  \brief 进行面面求交的运算.
	*  \param shape1 主模型的对象.
	*  \param shape2 工具模型的对象.
	*  \param method 采用算法的id，只接受1或者2. 1采用BOPAlgo_Section，2采用BRepAlgoAPI_IntCS, 3采用BRepAlgoAPI_IntSS.
	*  \return 操作是否成功.
	*/
	bool faceIntersectionOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, const int& method);
};

#endif