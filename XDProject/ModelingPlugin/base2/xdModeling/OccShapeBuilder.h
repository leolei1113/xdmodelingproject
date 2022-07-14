/*! \file OccShapeBuilder.h
*   \brief OccShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef OCCSHAPEBUILDER_H
#define OCCSHAPEBUILDER_H

#include <TopoDS_Shape.hxx>
#include <Precision.hxx>


/*! \class OccShapeBuilder
*   \brief 用于创建和变换TopoDS_Shape的基类.
 */
class OccShapeBuilder
{
public:
    /*! \fn OCCBasicTools()
    *  \brief OCCBasicTools的构造函数.
    */
    OccShapeBuilder();

    /*! \fn ~OCCBasicTools()
    *  \brief OCCBasicTools的析构函数.
    */
    ~OccShapeBuilder();

    /*! \fn virtual TopoDS_Shape getShape() const
    *  \brief 获取成员TopoDS_Shape对象.
    *  \return 返回成员TopoDS_Shape对象..
    */
    virtual TopoDS_Shape getShape() const { return m_shape; }

    /*! \fn virtual void setShape(TopoDS_Shape val)
    *  \brief 赋值成员TopoDS_Shape对象.
    *  \param val 进行赋值的TopoDS_Shape对象.
    */
    virtual void setShape(TopoDS_Shape val) { m_shape = val; }

protected:
    /*! \fn virtual bool checkInput()
    *  \brief 检查输入参数的接口.
    *  \return 返回参数是否合理.
    */
    virtual bool checkInput()
    {
        return true;
    }

protected:
    TopoDS_Shape m_shape;
};

#endif