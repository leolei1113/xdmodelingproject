/*! \file OffsetShapeBuilder.h
*   \brief OffsetShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef OFFSETSHAPEBUILDER_H
#define OFFSETSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"


/*! \class OffsetShapeBuilder
*   \brief 用于线和面的偏移，包括组合线、面
 */
class OffsetShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn OffsetShapeBuilder()
    *  \brief OffsetShapeBuilder的构造函数.
    */
    OffsetShapeBuilder();
    /*! \fn ~OffsetShapeBuilder()
    *  \brief OffsetShapeBuilder的析构函数.
    */
    ~OffsetShapeBuilder();

public:

    /*! \fn bool edgeOffsetOperation(TopoDS_Shape shape, const double& distance)
	*  \brief 线或者线框偏移的接口.
	*  \param entry 需要进行偏移的线框的TopoDS_HShape的指针.
	*  \param distance 偏移距离.
	*  \return 是否成功创建模型.
	*/
	bool edgeOffsetOperation(TopoDS_Shape shape, const double& distance);

    /*! \fn bool faceOffsetOperation(TopoDS_Shape shape, const double& distance, const bool& thickening)
    *  \brief 实现面偏移或者sheet加厚生成实体的接口.
    *  \param shape 需要进行操作的模型.
    *  \param distance 厚度或者距离.
    *  \param thickening 是否加厚，否则是偏移.
    *  \return 是否加厚成功.
    */
    bool faceOffsetOperation(TopoDS_Shape shape, const double& distance, const bool& thickening);

	/*! \fn bool solidFaceOffsetOperation(TopoDS_Shape shape, TopTools_ListOfShape faceArray, const double& distance)
	*  \brief 对实体中的面进行sheet加厚的操作.
	*  \param shape 需要进行操作的实体模型.
	*  \param faceArray 隶属实体且需要进行偏移的面.
	*  \param distance 厚度或者距离.
	*  \return 是否加厚成功.
	*/
	bool solidFaceOffsetOperation(TopoDS_Shape shape, TopTools_ListOfShape faceArray, const double& distance);
};

#endif