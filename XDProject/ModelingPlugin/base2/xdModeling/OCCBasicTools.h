/*! \file OCCBasicTools.h
*   \brief OCCBasicTools���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#pragma once
#include "xdModelingHeader.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_ListOfShape.hxx>
#include <gp_Pnt.hxx>
#include <vector>

/*! \class OCCBasicTools
*   \brief ����OCC�������ߵľ�̬������
 */
class XDMODELING_API OCCBasicTools
{
public:
    /*! \fn OCCBasicTools()
    *  \brief OCCBasicTools�Ĺ��캯��.
    */
	OCCBasicTools();

    /*! \fn ~OCCBasicTools()
    *  \brief OCCBasicTools����������.
    */
	~OCCBasicTools();

    /*! \fn static TopoDS_Face GetFaceFromPntList(std::vector<gp_Pnt> PntsList)
    *  \brief ����ɢ�㴴������߿�Ӷ�����ƽ��Ľӿ�.
    *  \param PntsList ƽ�涥���gp_Pnt��������.
    *  \return ���ش���ƽ��TopoDS_Face����.
    */
	static TopoDS_Face GetFaceFromPntList(std::vector<gp_Pnt> PntsList);

    /*! \fn static Standard_Boolean GetOrderWireFromEdges(std::vector<TopoDS_Edge> anEdges, TopoDS_Wire& OrderWire)
    *  \brief ���ݱߴ����߿�Ľӿ�.
    *  \param anEdges ���д����ıߵ�����.
    *  \param OrderWire ���ش������߿�.
    *  \return �����Ƿ�ɹ�����.
    */
	static Standard_Boolean GetOrderWireFromEdges(std::vector<TopoDS_Edge> anEdges, TopoDS_Wire& OrderWire);

    /*! \fn static bool DistanceMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& minDis)
	*  \brief ����ģ�ͼ����Ľӿ�.
	*  \param shape1 ģ��1.
	*  \param shape2 ģ��2.
	*  \param minDis �������ģ�ͼ�ľ���.
	*  \param pnt1 �����������ģ��1�ϵĵ�����.
	*  \param pnt2 �����������ģ��2�ϵĵ�����.
	*  \return �����Ƿ�ɹ�����.
    */
    static bool DistanceMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& minDis, gp_Pnt& pnt1, gp_Pnt& pnt2);

    /*! \fn static bool AngleMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& angle)
	*  \brief ����ֱ��ģ�ͼ����Ľӿ�.
	*  \param shape1 ģ��1.
	*  \param shape2 ģ��2.
	*  \param angle �������ģ�ͼ�ĽǶ�.
	*  \return �����Ƿ�ɹ�����.
	*/
	static bool AngleMeasurement(TopoDS_Shape shape1, TopoDS_Shape shape2, double& angle);

    /*! \fn static bool GetBndBoxParams(TopoDS_Shape shape, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax)
    *  \brief ��ȡĳһģ�͵İ�Χ����Ϣ.
	*  \param shape Ŀ��ģ��.
	*  \param theXmin ��Χ����СX����.
	*  \param theYmin ��Χ����СY����.
	*  \param theZmin ��Χ����СZ����.
	*  \param theXmax ��Χ�����X����.
	*  \param theYmax ��Χ�����Y����.
	*  \param theZmax ��Χ�����Z����.
	*  \return �����Ƿ�ɹ�����.
	*/
	static bool GetBndBoxParams(TopoDS_Shape shape, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax);

	/*! \fn static bool GetBndBoxParams(TopTools_ListOfShape shapes, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax)
	*  \brief ��ȡ���ģ�͵İ�Χ����Ϣ.
	*  \param shapes Ŀ��ģ����������.
	*  \param theXmin ��Χ����СX����.
	*  \param theYmin ��Χ����СY����.
	*  \param theZmin ��Χ����СZ����.
	*  \param theXmax ��Χ�����X����.
	*  \param theYmax ��Χ�����Y����.
	*  \param theZmax ��Χ�����Z����.
	*  \return �����Ƿ�ɹ�����.
	*/
	static bool GetBndBoxParams(TopTools_ListOfShape shapes, double& theXmin, double& theYmin, double& theZmin, double& theXmax, double& theYmax, double& theZmax);

    /*! \fn static bool GetEdgeOrFaceGradients(TopoDS_Shape shape, const std::vector<double>& inParams, const int& level, std::vector<double>& pos, std::vector<double>& gradients)
	*  \brief ����U��V���������߻�������ݶ�.
	*  \param shapes �߻����������.
	*  \param inParams U��V����, ���������ֻ��һ����, ����������.
	*  \param level �ݶȵĽ���.
	*  \param pos U��V�����궨�ĵ����ά����.
	*  \param gradients U��V���������ݶ�.
	*  \return �����Ƿ�ɹ�����.
	*/
	static bool GetEdgeOrFaceGradients(TopoDS_Shape shape, const std::vector<double>& inParams, const int& level, std::vector<double>& pos, std::vector<double>& gradients);

    /*! \fn static bool GetShapeVolume(TopoDS_Shape shape, double& volume)
	*  \brief ����ģ�����.
	*  \param shape ��Ҫ���м����ģ��.
	*  \param volume ����õ������.
	*  \return �����Ƿ�ɹ�����.
	*/
    static bool GetShapeVolume(TopoDS_Shape shape, double& volume);

	/*! \fn static bool createCompound(TopTools_ListOfShape shapes, TopoDS_Shape& target)
	*  \brief ������ģ�ʹ�����һ��װ����.
	*  \param shapes ����ģ�����.
	*  \param target װ����ģ��.
	*  \return �����Ƿ�ɹ�װ��.
	*/
	static bool createCompound(TopTools_ListOfShape shapes, TopoDS_Shape& target);

	/*! \fn static bool calcCurveCurvature(TopoDS_Shape shape, const double& T, double& curvature, gp_Pnt& centerOfCurvature)
	*  \brief ����������Uֵ�������ʺ�Բ������Ľӿ�.
	*  \param shape ��Ҫ���м����ģ��.
	*  \param T Ŀ��Uֵ.
	*  \param curvature ����õ�������.
	*  \param centerOfCurvature ����Բ������.
	*  \return �����Ƿ�ɹ�����.
	*/
	static bool calcCurveCurvature(TopoDS_Shape shape, const double& T, double& curvature, gp_Pnt& centerOfCurvature);

	/*! \fn static bool calcSurfCurvature(TopoDS_Shape shape, const double& U, const double& V, double& maxCurvature)
	*  \brief ����������U��Vֵ����������ʵĽӿ�.
	*  \param shape ��Ҫ���м����ģ��.
	*  \param U Ŀ��Uֵ.
	*  \param V Ŀ��Vֵ.
	*  \param maxCurvature ����õ����������.
	*  \return �����Ƿ�ɹ�����.
	*/
	static bool calcSurfCurvature(TopoDS_Shape shape, const double& U, const double& V, double& maxCurvature);

	//ѡ���ģ
	static bool DraftFacesOnModel(TopoDS_Shape& muttershape, TopTools_ListOfShape facestodraft, double draftangle, gp_Pln Neutral, gp_Dir pullingdiraction);

	static bool AutoBuildSolidByFaces(TopTools_ListOfShape facelist, TopoDS_Shape& buildsolid);

	static bool RemoveHolesWithinAreaAndDetectFaces(TopoDS_Shape muttershape, TopoDS_Shape& output, TopTools_ListOfShape& facelist, double inputarea);

	static bool GetArcTopologyCenter(TopoDS_Edge edge, gp_Pnt center);

	static bool ComputeNURBSKnotsMults(std::vector<double> src, TColStd_Array1OfReal knots, TColStd_Array1OfInteger mults);

	static bool DetectSliverFaces(TopoDS_Shape ent, double tolerance, std::vector<TopoDS_Shape>& sliverfaces);

	static bool RemoveFace(TopoDS_Shape mutter_3d, TopTools_ListOfShape facestoremove, TopoDS_Shape& result);
};


