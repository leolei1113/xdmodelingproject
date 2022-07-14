/*! \file FilletShapeBuilder.h
*   \brief FilletShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef FILLETSHAPEBUILDER_H
#define FILLETSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"
#include <NCollection_DataMap.hxx>
#include <NCollection_List.hxx>

/*! \class FilletShapeBuilder
*   \brief ����ģ�͵��ǵ��࣬��֧�ֵ�Բ�ǡ���б�ǡ����ԳƵ��ǡ�ƽ���߿򵹽ǡ�ʵ�嵹�ǵ�
 */
class FilletShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn FilletShapeBuilder()
    *  \brief FilletShapeBuilder�Ĺ��캯��.
    */
    FilletShapeBuilder();
    /*! \fn ~FilletShapeBuilder()
    *  \brief FilletShapeBuilder����������.
    */
    ~FilletShapeBuilder();

public:
    // ��õ��ߵĵ�Բ
    /*! \fn bool edgeFilletOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& radius)
    *  \brief ��άģ�ͽ��бߵ��ǵĽӿ�.
    *  \param shape ��Ҫ���ǵ�ģ��.
    *  \param edgelist ��ά���ǵ���ģ������.
    *  \param radius ���ǰ뾶.
    *  \return �Ƿ񵹽ǳɹ�.
    */
    bool edgeFilletOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& radius);

    // ��õ��ߵĵ���
    /*! \fn bool edgeChamferOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& lDistance, const double& rDistance, const bool& symmetryFlag)
    *  \brief ��άģ�ͽ��бߵ��ǵĽӿ�.
    *  \param shape ��Ҫ���ǵ�ģ��.
    *  \param edgelist ��ά���ǵ���ģ������.
    *  \param lDistance ����������.
    *  \param rDistance �����Ҳ����.
    *  \param symmetryFlag �����Ƿ�ԳƵ���.
    *  \return �Ƿ񵹽ǳɹ�.
    */
    bool edgeChamferOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& edgelist, const double& lDistance, const double& rDistance, const bool& symmetryFlag);

    /*! \fn bool vertexFillet2DOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
    *  \brief ��άƽ������߿��е�ĵ�Բ�ǵ�б�ǣ�����BRepFilletAPI_MakeFillet2d_Extension�ӿ�.
    *  \param shape ��Ҫ���ǵ�ģ��.
    *  \param edgelist ��ά�����е�ĵ�ģ������.
    *  \param lDistanceOrRradius ��Բ�ǵİ뾶���ߵ�б��������.
    *  \param rDistance �����Ҳ����.
    *  \param filletFlag ��Բ�ǻ��ߵ�б�ǵı�ʶ.
    *  \param symmetryFlag �����Ƿ�ԳƵ���.
    *  \return �Ƿ񵹽ǳɹ�.
    */
    bool vertexFillet2DOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);

	/*! \fn bool vertexFillet2DOperation(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
	*  \brief ��άƽ������߿��е�ĵ�Բ�ǵ�б�ǣ�����BRepFilletAPI_MakeFillet2d�ӿ�.
	*  \param shape ��Ҫ���ǵ�ģ��.
	*  \param edgelist ��ά�����е�ĵ�ģ������.
	*  \param lDistanceOrRradius ��Բ�ǵİ뾶���ߵ�б��������.
	*  \param rDistance �����Ҳ����.
	*  \param filletFlag ��Բ�ǻ��ߵ�б�ǵı�ʶ.
	*  \param symmetryFlag �����Ƿ�ԳƵ���.
	*  \return �Ƿ񵹽ǳɹ�.
	*/
	bool vertexFillet2DOperation2(TopoDS_Shape shape, const NCollection_List<TopoDS_Shape>& vertexlist, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);

    /*! \fn bool vertexFillet2DOperation(TopoDS_Shape shape, const TopoDS_Shape& vertex, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
    *  \brief ��άƽ������߿��е�ĵ�Բ�ǵ�б�ǣ�����BRepFilletAPI_MakeFillet2d_Extension�ӿ�.
    *  \param shape ��Ҫ���ǵ�ģ��.
    *  \param vertex ��ά�����е�ĵ�ģ��.
    *  \param lDistanceOrRradius ��Բ�ǵİ뾶���ߵ�б��������.
    *  \param rDistance �����Ҳ����.
    *  \param filletFlag ��Բ�ǻ��ߵ�б�ǵı�ʶ.
    *  \param symmetryFlag �����Ƿ�ԳƵ���.
    *  \return �Ƿ񵹽ǳɹ�.
    */
	bool vertexFillet2DOperation(TopoDS_Shape solidShape, const TopoDS_Shape& vertex, const double& lDistanceOrRradius, const double& rDistance, const bool& filletFlag, const bool& symmetryFlag);
};

#endif