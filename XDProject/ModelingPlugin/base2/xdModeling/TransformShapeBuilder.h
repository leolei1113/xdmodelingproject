/*! \file TransformShapeBuilder.h
*   \brief TransformShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef TRANSFORMSHAPEBUILDER_H
#define TRANSFORMSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"

#include <TopoDS_Wire.hxx>

/*! \class TransformShapeBuilder
*   \brief 用于对几何体进行变换的类，包括平移、旋转、缩放、复制
*/
class TransformShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn TransformShapeBuilder()
    *  \brief TransformShapeBuilder的构造函数.
    */
    TransformShapeBuilder();
    /*! \fn ~TransformShapeBuilder()
    *  \brief TransformShapeBuilder的析构函数.
    */
    ~TransformShapeBuilder();

public:
    /*! \fn bool shapeTranslate(TopoDS_Shape origShape, const double& vectorX, const double& vectorY, const double& vectorZ, const bool& isCopy)
    *  \brief 对模型进行平移操作.
    *  \param origShape 需要进行操作的模型对象.
    *  \param vectorX 平移向量的x分量.
    *  \param vectorY 平移向量的y分量.
    *  \param vectorZ 平移向量的z分量.
    *  \param isCopy 是否复制原模型.
    *  \return 操作是否成功.
    */
    bool shapeTranslate(TopoDS_Shape origShape, const double& vectorX, const double& vectorY, const double& vectorZ, const bool& isCopy);

    /*! \fn bool shapeRotate(TopoDS_Shape origShape, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const bool& isCopy)
    *  \brief 对模型进行旋转操作.
    *  \param origShape 需要进行操作的模型对象.
    *  \param axisPosX 旋转轴起点的x坐标.
    *  \param axisPosY 旋转轴起点的y坐标.
    *  \param axisPosZ 旋转轴起点的z坐标.
    *  \param axisVecX 旋转轴方向向量的x分量.
    *  \param axisVecY 旋转轴方向向量的y分量.
    *  \param axisVecZ 旋转轴方向向量的z分量.
    *  \param angle 旋转角度.
    *  \param isCopy 是否复制原模型.
    *  \return 操作是否成功.
    */
    bool shapeRotate(TopoDS_Shape origShape, const double& axisPosX, const double& axisPosY, const double& axisPosZ, 
        const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const bool& isCopy);

    /*! \fn bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy)
    *  \brief 对模型进行缩放操作.
    *  \param origShape 需要进行操作的模型对象.
    *  \param basePosX 缩放参考点的x坐标.
    *  \param basePosY 缩放参考点的y坐标.
    *  \param basePosZ 缩放参考点的z坐标.
    *  \param scaleX x方向缩放比例.
    *  \param scaleY y方向缩放比例.
    *  \param scaleZ z方向缩放比例.
    *  \param isCopy 是否复制原模型.
    *  \return 操作是否成功.
    */
    bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy);

	/*! \fn bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scale, const bool& isCopy)
	*  \brief 对模型进行缩放操作.
	*  \param origShape 需要进行操作的模型对象.
	*  \param basePosX 缩放参考点的x坐标.
	*  \param basePosY 缩放参考点的y坐标.
	*  \param basePosZ 缩放参考点的z坐标.
	*  \param scale 各方向缩放比例.
	*  \return 操作是否成功.
	*/
	bool shapeScale(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& scale, const bool& isCopy);

    /*! \fn bool shapeScale(TopoDS_Shape origShape, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy)
    *  \brief 接口已弃用.
    */
    bool shapeScale(TopoDS_Shape origShape, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& scaleX, const double& scaleY, const double& scaleZ, const bool& isCopy);

    /*! \fn bool shapeMirror(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& isCopy)
    *  \brief 对模型进行镜像操作.
    *  \param origShape 需要进行操作的模型对象.
    *  \param basePosX 镜像平面上任意一点的x坐标.
    *  \param basePosY 镜像平面上任意一点的y坐标.
    *  \param basePosZ 镜像平面上任意一点的z坐标.
    *  \param normalVecX 镜像平面法向向量的x分量.
    *  \param normalVecY 镜像平面法向向量的y分量.
    *  \param normalVecZ 镜像平面法向向量的z分量.
    *  \param isCopy 是否复制原模型.
    *  \return 操作是否成功.
    */
    bool shapeMirror(TopoDS_Shape origShape, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& isCopy);

    /*! \fn bool shapeCopy(TopoDS_Shape origShape)
    *  \brief 对模型进行复制操作.
    *  \param origShape 需要进行操作的模型对象.
    *  \return 操作是否成功.
    */
    bool shapeCopy(TopoDS_Shape origShape);
};

#endif
