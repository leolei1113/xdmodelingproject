/*! \file VertexShapeBuilder.h
*   \brief VertexShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef VERTEXSHAPEBUILDER_H
#define VERTEXSHAPEBUILDER_H

#include "OccShapeBuilder.h"
#include "xdModelingHeader.h"

/*! \class VertexShapeBuilder
*   \brief ���ڴ�������࣬�������ꡢ���߽��㡢���潻��
 */
class XDMODELING_API VertexShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder�Ĺ��캯��.
    */
    VertexShapeBuilder();
    /*! \fn ~EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder����������.
    */
    ~VertexShapeBuilder();

public:
    /*! \fn bool createVertex(const double& xPos, const double& yPos, const double& zPos)
    *  \brief �������괴����.
    *  \param xPos ��x����.
    *  \param yPos ��y����.
    *  \param zPos ��z����.
    *  \return �Ƿ�ɹ�����.
    */
    bool createVertex(const double& xPos, const double& yPos, const double& zPos);
};

#endif
