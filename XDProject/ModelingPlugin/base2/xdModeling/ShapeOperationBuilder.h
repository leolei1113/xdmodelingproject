/*! \file ShapeOperationBuilder.h
*   \brief ShapeOperationBuilder���ͷ�ļ�
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
*   \brief ���ڶ�shape���м��β����ĸ���
 */
class XDMODELING_API ShapeOperationBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn ShapeOperationBuilder()
    *  \brief ShapeOperationBuilder�Ĺ��캯��.
    */
    ShapeOperationBuilder();
    /*! \fn ~ShapeOperationBuilder()
    *  \brief ShapeOperationBuilder����������.
    */
    ~ShapeOperationBuilder();

    /*! \fn virtual void* getBuilder() const
    *  \brief ��ȡ��Աm_builder������ǰ�������������.
    *  \return ��ȡ��������ָ��.
    */
    virtual void* getBuilder() const { return m_builder; }

/*
    / *! \fn void setBuilder(void* val)
    *  \brief �趨��Աm_builder��ָ��.
    *  \param val ��Ҫ�趨��m_builder��ָ��.
    * /
    virtual void setBuilder(void* val) { m_builder = val; }
*/

protected:
    BRepBuilderAPI_MakeShape*                           m_builder;
};

#endif