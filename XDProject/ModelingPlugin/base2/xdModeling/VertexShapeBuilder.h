/*! \file VertexShapeBuilder.h
*   \brief VertexShapeBuilder类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef VERTEXSHAPEBUILDER_H
#define VERTEXSHAPEBUILDER_H

#include "OccShapeBuilder.h"
#include "xdModelingHeader.h"

/*! \class VertexShapeBuilder
*   \brief 用于创建点的类，包括坐标、线线交点、线面交点
 */
class XDMODELING_API VertexShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder的构造函数.
    */
    VertexShapeBuilder();
    /*! \fn ~EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder的析构函数.
    */
    ~VertexShapeBuilder();

public:
    /*! \fn bool createVertex(const double& xPos, const double& yPos, const double& zPos)
    *  \brief 根据坐标创建点.
    *  \param xPos 点x坐标.
    *  \param yPos 点y坐标.
    *  \param zPos 点z坐标.
    *  \return 是否成功创建.
    */
    bool createVertex(const double& xPos, const double& yPos, const double& zPos);
};

#endif
