/*! \file FilletShapeBuilder.h
*   \brief FilletShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef FILLETSHAPEBUILDER_H
#define FILLETSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"
#include <NCollection_DataMap.hxx>
#include <NCollection_List.hxx>

/*! \class FilletShapeBuilder
*   \brief 用于模型倒角的类，可支持倒圆角、倒斜角、不对称倒角、平面线框倒角、实体倒角等
 */
class FilletShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn FilletShapeBuilder()
    *  \brief FilletShapeBuilder的构造函数.
    */
    FilletShapeBuilder();
    /*! \fn ~FilletShapeBuilder()
    *  \brief FilletShapeBuilder的析构函数.
    */
    ~FilletShapeBuilder();

public:
    // 获得单边的倒圆
    /*! \fn bool edgeFilletOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& radius)
    *  \brief 三维模型进行边倒角的接口.
    *  \param shape 需要倒角的模型.
    *  \param edgelist 三维倒角的线模型序列.
    *  \param radius 倒角半径.
    *  \return 是否倒角成功.
    */
    bool edgeFilletOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& radius);

    // 获得单边的倒角
    /*! \fn bool edgeChamferOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& lDistance, const double& rDistance, const bool& symmetryFlag)
    *  \brief 三维模型进行边倒角的接口.
    *  \param shape 需要倒角的模型.
    *  \param edgelist 三维倒角的线模型序列.
    *  \param lDistance 倒角左侧距离.
    *  \param rDistance 倒角右侧距离.
    *  \param symmetryFlag 倒角是否对称倒角.
    *  \return 是否倒角成功.
    */
    bool edgeChamferOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& lDistance, const double& rDistance, const bool& symmetryFlag);

    /*! \fn bool vertexFillet2DOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
    *  \brief 二维平面或者线框中点的倒圆角倒斜角，采用BRepFilletAPI_MakeFillet2d_Extension接口.
    *  \param shape 需要倒角的模型.
    *  \param edgelist 二维倒角中点的点模型序列.
    *  \param lDistanceOrRradius 倒圆角的半径或者倒斜角左侧距离.
    *  \param rDistance 倒角右侧距离.
    *  \param filletFlag 倒圆角或者倒斜角的标识.
    *  \param symmetryFlag 倒角是否对称倒角.
    *  \return 是否倒角成功.
    */
    bool vertexFillet2DOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);

	/*! \fn bool vertexFillet2DOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
	*  \brief 二维平面或者线框中点的倒圆角倒斜角，采用BRepFilletAPI_MakeFillet2d接口.
	*  \param shape 需要倒角的模型.
	*  \param edgelist 二维倒角中点的点模型序列.
	*  \param lDistanceOrRradius 倒圆角的半径或者倒斜角左侧距离.
	*  \param rDistance 倒角右侧距离.
	*  \param filletFlag 倒圆角或者倒斜角的标识.
	*  \param symmetryFlag 倒角是否对称倒角.
	*  \return 是否倒角成功.
	*/
	bool vertexFillet2DOperation2(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);

    /*! \fn bool vertexFillet2DOperation(TopoDS_Shape shape, const TopoDS_Shape& vertex, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
    *  \brief 二维平面或者线框中点的倒圆角倒斜角，采用BRepFilletAPI_MakeFillet2d_Extension接口.
    *  \param shape 需要倒角的模型.
    *  \param vertex 二维倒角中点的点模型.
    *  \param lDistanceOrRradius 倒圆角的半径或者倒斜角左侧距离.
    *  \param rDistance 倒角右侧距离.
    *  \param filletFlag 倒圆角或者倒斜角的标识.
    *  \param symmetryFlag 倒角是否对称倒角.
    *  \return 是否倒角成功.
    */
	bool vertexFillet2DOperation(TopoDS_Shape solidShape, const TopoDS_Shape& vertex, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
};

#endif