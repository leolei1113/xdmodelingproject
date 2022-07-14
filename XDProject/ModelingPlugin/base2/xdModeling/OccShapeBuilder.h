/*! \file OccShapeBuilder.h
*   \brief OccShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef OCCSHAPEBUILDER_H
#define OCCSHAPEBUILDER_H

#include <TopoDS_Shape.hxx>
#include <Precision.hxx>


/*! \class OccShapeBuilder
*   \brief ���ڴ����ͱ任TopoDS_Shape�Ļ���.
 */
class OccShapeBuilder
{
public:
    /*! \fn OCCBasicTools()
    *  \brief OCCBasicTools�Ĺ��캯��.
    */
    OccShapeBuilder();

    /*! \fn ~OCCBasicTools()
    *  \brief OCCBasicTools����������.
    */
    ~OccShapeBuilder();

    /*! \fn virtual TopoDS_Shape getShape() const
    *  \brief ��ȡ��ԱTopoDS_Shape����.
    *  \return ���س�ԱTopoDS_Shape����..
    */
    virtual TopoDS_Shape getShape() const { return m_shape; }

    /*! \fn virtual void setShape(TopoDS_Shape val)
    *  \brief ��ֵ��ԱTopoDS_Shape����.
    *  \param val ���и�ֵ��TopoDS_Shape����.
    */
    virtual void setShape(TopoDS_Shape val) { m_shape = val; }

protected:
    /*! \fn virtual bool checkInput()
    *  \brief �����������Ľӿ�.
    *  \return ���ز����Ƿ����.
    */
    virtual bool checkInput()
    {
        return true;
    }

protected:
    TopoDS_Shape m_shape;
};

#endif