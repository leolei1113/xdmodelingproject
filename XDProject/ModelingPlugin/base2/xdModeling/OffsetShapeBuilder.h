/*! \file OffsetShapeBuilder.h
*   \brief OffsetShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef OFFSETSHAPEBUILDER_H
#define OFFSETSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"


/*! \class OffsetShapeBuilder
*   \brief �����ߺ����ƫ�ƣ���������ߡ���
 */
class OffsetShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn OffsetShapeBuilder()
    *  \brief OffsetShapeBuilder�Ĺ��캯��.
    */
    OffsetShapeBuilder();
    /*! \fn ~OffsetShapeBuilder()
    *  \brief OffsetShapeBuilder����������.
    */
    ~OffsetShapeBuilder();

public:

    /*! \fn bool edgeOffsetOperation(TopoDS_Shape shape, const double& distance)
	*  \brief �߻����߿�ƫ�ƵĽӿ�.
	*  \param entry ��Ҫ����ƫ�Ƶ��߿��TopoDS_HShape��ָ��.
	*  \param distance ƫ�ƾ���.
	*  \return �Ƿ�ɹ�����ģ��.
	*/
	bool edgeOffsetOperation(TopoDS_Shape shape, const double& distance);

    /*! \fn bool faceOffsetOperation(TopoDS_Shape shape, const double& distance, const bool& thickening)
    *  \brief ʵ����ƫ�ƻ���sheet�Ӻ�����ʵ��Ľӿ�.
    *  \param shape ��Ҫ���в�����ģ��.
    *  \param distance ��Ȼ��߾���.
    *  \param thickening �Ƿ�Ӻ񣬷�����ƫ��.
    *  \return �Ƿ�Ӻ�ɹ�.
    */
    bool faceOffsetOperation(TopoDS_Shape shape, const double& distance, const bool& thickening);

	/*! \fn bool solidFaceOffsetOperation(TopoDS_Shape shape, TopTools_ListOfShape faceArray, const double& distance)
	*  \brief ��ʵ���е������sheet�Ӻ�Ĳ���.
	*  \param shape ��Ҫ���в�����ʵ��ģ��.
	*  \param faceArray ����ʵ������Ҫ����ƫ�Ƶ���.
	*  \param distance ��Ȼ��߾���.
	*  \return �Ƿ�Ӻ�ɹ�.
	*/
	bool solidFaceOffsetOperation(TopoDS_Shape shape, TopTools_ListOfShape faceArray, const double& distance);
};

#endif