/*! \file SweepShapeBuilder.h
*   \brief SweepShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef SWEEPSHAPEBUILDER_H
#define SWEEPSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"
#include "xdModelingHeader.h"

#include <TopoDS_Shape.hxx>
#include <BRepBuilderAPI_TransitionMode.hxx>
#include <TopTools_HArray1OfShape.hxx>

/*! \class SweepShapeBuilder
*   \brief 用于对shape进行拉伸、旋转、扫略、放样处理的类
 */
class XDMODELING_API SweepShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn SweepShapeBuilder()
    *  \brief SweepShapeBuilder的构造函数.
    */
    SweepShapeBuilder();
    /*! \fn ~SweepShapeBuilder()
    *  \brief SweepShapeBuilder的析构函数.
    */
    ~SweepShapeBuilder();

public:
    /*! \fn bool extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec)
    *  \brief 实现模型拉伸的接口，点拉伸成线、线拉伸成面、面拉伸成体
    *  \param shape 需要拉伸的模型.
    *  \param xVec 拉伸矢量的x分量.
    *  \param yVec 拉伸矢量的y分量.
    *  \param zVec 拉伸矢量的z分量.
    *  \return 是否拉伸成功.
    */
    bool extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec);

    /*! \fn bool extrudeDraftOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec, const double& draftAngle, const BRepBuilderAPI_TransitionMode mode)
    *  \brief 实现模型锥度拉伸的接口，点拉伸成线、线拉伸成面、面拉伸成体.
    *  \param shape 需要拉伸的模型.
    *  \param xVec 拉伸矢量的x分量.
    *  \param yVec 拉伸矢量的y分量.
    *  \param zVec 拉伸矢量的z分量.
    *  \param draftAngle 拉伸过程的锥度角.
    *  \param mode 对于过程中的缝隙采用何种方式进行填补，包括延伸、圆角和自然相交.
    *  \param topCover 顶部是否补面.
    *  \return 是否拉伸成功.
    */
    bool extrudeDraftOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec, const double& draftAngle, const BRepBuilderAPI_TransitionMode mode, const bool& topCover);

    /*! \fn bool revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle = 360.0)
    *  \brief 实现模型旋转的接口，点旋转成线、线旋转成面、面旋转成体.
    *  \param shape 需要旋转的模型.
    *  \param xStart 旋转轴起点的x坐标.
    *  \param yStart 旋转轴起点的y坐标.
    *  \param zStart 旋转轴起点的z坐标.
    *  \param xVec 旋转轴向量的x分量.
    *  \param yVec 旋转轴向量的y分量.
    *  \param zVec 旋转轴向量的z分量.
    *  \param angle 旋转角度.
    *  \return 是否旋转成功.
    */
    bool revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle = 360.0);

    /*! \fn bool sweepPipeOperation(TopoDS_Shape shape, TopoDS_Shape path)
    *  \brief 实现模型延母线扫略，线扫略成面、面扫略成体.
    *  \param shape 需要扫略的模型.
    *  \param path 扫略母线.
    *  \return 是否扫略成功.
    */
    bool sweepPipeOperation(TopoDS_Shape shape, TopoDS_Shape path);

    /*! \fn bool sweepPipeShellOperation(TopoDS_Shape shape, TopoDS_Shape path, const BRepBuilderAPI_TransitionMode mode)
    *  \brief 实现模型延母线扫略，模型必须是线框，封闭的线框将生成体，不封闭的生成曲面.
    *  \param shape 需要扫略的模型.
    *  \param path 扫略母线.
    *  \param twistAngle 扫略过程中进行扭转.
    *  \param mode 对于过程中的缝隙采用何种方式进行填补，包括延伸、圆角和自然相交.
    *  \return 是否扫略成功.
    */
    bool sweepPipeShellOperation(TopoDS_Shape shape, TopoDS_Shape path, const double& twistAngle, const BRepBuilderAPI_TransitionMode mode);

    /*! \fn bool loftOperation(TopTools_HArray1OfShape shapeArray, const bool& genSolid = false)
    *  \brief 实现模型放样的接口.
    *  \param shapeArray 需要放样的模型数组.
    *  \param genSolid 是否生成实体.
    *  \return 是否放样成功.
    */
    bool loftOperation(TopTools_ListOfShape shapeArray, const bool& genSolid = false);

protected:
    double m_sweepAlongPathResolution;
    double m_sweepAlongPathPrecision;
};

#endif