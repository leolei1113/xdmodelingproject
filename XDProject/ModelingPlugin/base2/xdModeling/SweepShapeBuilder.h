/*! \file SweepShapeBuilder.h
*   \brief SweepShapeBuilder���ͷ�ļ�
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
*   \brief ���ڶ�shape�������졢��ת��ɨ�ԡ������������
 */
class XDMODELING_API SweepShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn SweepShapeBuilder()
    *  \brief SweepShapeBuilder�Ĺ��캯��.
    */
    SweepShapeBuilder();
    /*! \fn ~SweepShapeBuilder()
    *  \brief SweepShapeBuilder����������.
    */
    ~SweepShapeBuilder();

public:
    /*! \fn bool extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec)
    *  \brief ʵ��ģ������Ľӿڣ���������ߡ���������桢���������
    *  \param shape ��Ҫ�����ģ��.
    *  \param xVec ����ʸ����x����.
    *  \param yVec ����ʸ����y����.
    *  \param zVec ����ʸ����z����.
    *  \return �Ƿ�����ɹ�.
    */
    bool extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec);

    /*! \fn bool extrudeDraftOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec, const double& draftAngle, const BRepBuilderAPI_TransitionMode mode)
    *  \brief ʵ��ģ��׶������Ľӿڣ���������ߡ���������桢���������.
    *  \param shape ��Ҫ�����ģ��.
    *  \param xVec ����ʸ����x����.
    *  \param yVec ����ʸ����y����.
    *  \param zVec ����ʸ����z����.
    *  \param draftAngle ������̵�׶�Ƚ�.
    *  \param mode ���ڹ����еķ�϶���ú��ַ�ʽ��������������졢Բ�Ǻ���Ȼ�ཻ.
    *  \param topCover �����Ƿ���.
    *  \return �Ƿ�����ɹ�.
    */
    bool extrudeDraftOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec, const double& draftAngle, const BRepBuilderAPI_TransitionMode mode, const bool& topCover);

    /*! \fn bool revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle = 360.0)
    *  \brief ʵ��ģ����ת�Ľӿڣ�����ת���ߡ�����ת���桢����ת����.
    *  \param shape ��Ҫ��ת��ģ��.
    *  \param xStart ��ת������x����.
    *  \param yStart ��ת������y����.
    *  \param zStart ��ת������z����.
    *  \param xVec ��ת��������x����.
    *  \param yVec ��ת��������y����.
    *  \param zVec ��ת��������z����.
    *  \param angle ��ת�Ƕ�.
    *  \return �Ƿ���ת�ɹ�.
    */
    bool revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle = 360.0);

    /*! \fn bool sweepPipeOperation(TopoDS_Shape shape, TopoDS_Shape path)
    *  \brief ʵ��ģ����ĸ��ɨ�ԣ���ɨ�Գ��桢��ɨ�Գ���.
    *  \param shape ��Ҫɨ�Ե�ģ��.
    *  \param path ɨ��ĸ��.
    *  \return �Ƿ�ɨ�Գɹ�.
    */
    bool sweepPipeOperation(TopoDS_Shape shape, TopoDS_Shape path);

    /*! \fn bool sweepPipeShellOperation(TopoDS_Shape shape, TopoDS_Shape path, const BRepBuilderAPI_TransitionMode mode)
    *  \brief ʵ��ģ����ĸ��ɨ�ԣ�ģ�ͱ������߿򣬷�յ��߿������壬����յ���������.
    *  \param shape ��Ҫɨ�Ե�ģ��.
    *  \param path ɨ��ĸ��.
    *  \param twistAngle ɨ�Թ����н���Ťת.
    *  \param mode ���ڹ����еķ�϶���ú��ַ�ʽ��������������졢Բ�Ǻ���Ȼ�ཻ.
    *  \return �Ƿ�ɨ�Գɹ�.
    */
    bool sweepPipeShellOperation(TopoDS_Shape shape, TopoDS_Shape path, const double& twistAngle, const BRepBuilderAPI_TransitionMode mode);

    /*! \fn bool loftOperation(TopTools_HArray1OfShape shapeArray, const bool& genSolid = false)
    *  \brief ʵ��ģ�ͷ����Ľӿ�.
    *  \param shapeArray ��Ҫ������ģ������.
    *  \param genSolid �Ƿ�����ʵ��.
    *  \return �Ƿ�����ɹ�.
    */
    bool loftOperation(TopTools_ListOfShape shapeArray, const bool& genSolid = false);

protected:
    double m_sweepAlongPathResolution;
    double m_sweepAlongPathPrecision;
};

#endif