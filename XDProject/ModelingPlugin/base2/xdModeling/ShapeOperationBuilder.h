/*! \file ShapeOperationBuilder.h
*   \brief ShapeOperationBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef SHAPEOPERATIONBUILDER_H
#define SHAPEOPERATIONBUILDER_H

#include "OccShapeBuilder.h"
#include "EnumAndStruct.h"
#include "xdModelingHeader.h"

#include <BRepBuilderAPI_MakeShape.hxx>

/*! \class ShapeOperationBuilder
*   \brief 用于对shape进行几何操作的父类
 */
class XDMODELING_API ShapeOperationBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn ShapeOperationBuilder()
    *  \brief ShapeOperationBuilder的构造函数.
    */
    ShapeOperationBuilder();
    /*! \fn ~ShapeOperationBuilder()
    *  \brief ShapeOperationBuilder的析构函数.
    */
    ~ShapeOperationBuilder();

    /*! \fn virtual void* getBuilder() const
    *  \brief 获取成员m_builder，即当前布尔运算的算子.
    *  \return 获取到的算子指针.
    */
    virtual void* getBuilder() const { return m_builder; }

/*
    / *! \fn void setBuilder(void* val)
    *  \brief 设定成员m_builder的指针.
    *  \param val 需要设定的m_builder的指针.
    * /
    virtual void setBuilder(void* val) { m_builder = val; }
*/

protected:
    BRepBuilderAPI_MakeShape*                           m_builder;
};

#endif