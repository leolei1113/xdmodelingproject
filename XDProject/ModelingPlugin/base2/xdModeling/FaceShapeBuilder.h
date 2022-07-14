/*! \file FaceShapeBuilder.h
*   \brief FaceShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef FACESHAPEBUILDER_H
#define FACESHAPEBUILDER_H

#include "OccShapeBuilder.h"

#include <TopoDS_Wire.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <Standard_Integer.hxx>

#include <vector>

/*! \class FaceShapeBuilder
*   \brief ���ڴ�������face����
 */
class FaceShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn FaceShapeBuilder()
    *  \brief FaceShapeBuilder�Ĺ��캯��.
    */
    FaceShapeBuilder();
    /*! \fn ~FaceShapeBuilder()
    *  \brief FaceShapeBuilder����������.
    */
    ~FaceShapeBuilder();

public:
    /*! \fn bool createRectFace(const double& xPos, const double& yPos, const double& zPos,  const double& xAxis, const double& yAxis, const double&zAxis, const double& width, const double& height)
    *  \brief ���ݶ���ͳ�����������.
    *  \param xPos ���x����.
    *  \param yPos ���y����.
    *  \param zPos ���z����.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \param width ���.
    *  \param height �߶�.
    *  \return �Ƿ�ɹ�����.
    */
    bool createRectFace(const double& xPos, const double& yPos, const double& zPos, 
        const double& xAxis, const double& yAxis, const double&zAxis, 
        const double& width, const double& height);

    /*! \fn bool createCircleFace(const double& centerX, const double& centerY, const double& centerZ, const double& xAxis, const double& yAxis, const double&zAxis, const double& radius)
    *  \brief ����Բ��.
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \param radius �뾶.
    *  \return �Ƿ�ɹ�����.
    */
    bool createCircleFace(const double& centerX, const double& centerY, const double& centerZ,
        const double& xAxis, const double& yAxis, const double&zAxis,
        const double& radius);

    /*! \fn  bool createSectorFace(const double& xCenter, const double& yCenter, const double& zCenter,
		const double& xAxis, const double& yAxis, const double&zAxis,
		const double& radius, const double& startAng, const double& endAng)
    *  \brief ����Բ��λ�á��뾶�ͽǶȴ��������棬������.
    */
	bool createSectorFace(const double& xCenter, const double& yCenter, const double& zCenter,
		const double& xAxis, const double& yAxis, const double&zAxis,
		const double& radius, const double& startAng, const double& endAng);

    /*! \fn bool createEllipseFace(const double& centerX, const double& centerY, const double& centerZ, const double& xAxis, const double& yAxis, const double&zAxis, const double& majorRadius, const double& minorRadius)
    *  \brief ����Բ��λ�úͳ�����뾶������Բ��.
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \param majorRadius ����뾶.
    *  \param minorRadius �������.
    *  \return �Ƿ�ɹ�����.
    */
    bool createEllipseFace(const double& centerX, const double& centerY, const double& centerZ,
        const double& xAxis, const double& yAxis, const double&zAxis,
        const double& majorRadius, const double& minorRadius);

    /*! \fn bool createCylinderFace(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height)
    *  \brief ����Բ������.
    *  \param xBottomCenter ����Բ��x����.
    *  \param yBottomCenter ����Բ��y����.
    *  \param zBottomCenter ����Բ��z����.
    *  \param xAxis Բ����������xֵ.
    *  \param yAxis Բ����������yֵ.
    *  \param zAxis Բ����������zֵ.
    *  \param radius �뾶.
    *  \param height Բ���߶�.
    *  \return �Ƿ�ɹ�����.
    */
    bool createCylinderFace(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height);

    /*! \fn bool createConeFace(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����Բ׶��Բ̨����.
    *  \param botCenterX ����Բ��x����.
    *  \param botCenterY ����Բ��y����.
    *  \param botCenterZ ����Բ��z����.
    *  \param lowerRadius ����뾶.
    *  \param upperRadius ����뾶��Ϊ0��Բ׶��.
    *  \param height Բ���߶�.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createConeFace(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createPolygon(const std::vector<std::vector<double>>& pPos)
    *  \brief ���������ƽ�棬����Ϊ��������.
    *  \param points ����Ķ���ζ�������.
    *  \return �Ƿ�ɹ�����.
    */
	bool createPolygon(const std::vector<std::vector<double>>& pPos);

    /*! \fn bool createRegularPolygon(const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const int& numSides, double xAxis, double yAxis, double zAxis
    *  \brief �����������ƽ��
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param startX ���x����.
    *  \param startY ���y����.
    *  \param startZ ���z����.
    *  \param numSides ������αߵ���Ŀ.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
	bool createRegularPolygon(const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const int& numSides, double xAxis, double yAxis, double zAxis);

	/*! \fn bool createParabolaFace(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
	*  \brief ���������棬ͨ����������ת�õ�.
	*  \param points ���ĵ�����.
	*  \param focalDistance ����.
	*  \param startP ˫������ʼtֵ.
	*  \param endP ˫������ֹtֵ.
	*  \param xAxis ����ƽ�淨������xֵ.
	*  \param yAxis ����ƽ�淨������yֵ.
	*  \param zAxis ����ƽ�淨������zֵ.
	*  \return �Ƿ�ɹ�����.
	*/
	bool createParabolaFace(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP,
		const double& xAxis, const double& yAxis, const double& zAxis);

	/*! \fn bool createFuncSurface(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, const std::string varUName, const std::string varVName, const double startU, const double endU, const double startV, const double endV)
	*  \brief ������������.
	*  \param xFuncExpress ��������x����ı仯����.
	*  \param yFuncExpress ��������y����ı仯����.
	*  \param zFuncExpress ��������z����ı仯����.
	*  \param varUName U��������.
	*  \param varVName V��������.
	*  \param startU ������ʼUֵ.
	*  \param endU ������ʼUֵ.
	*  \param startV ������ʼVֵ.
	*  \param endV ������ʼVֵ.
	*  \return �Ƿ�ɹ�����.
	*/
	bool createFuncSurface(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress,
		const std::string varUName, const std::string varVName, const double startU, const double endU, const double startV, const double endV);

    // ���ݷ���߿򴴽���
    /*! \fn bool createClosedFace(TopoDS_Wire closedWire)
    *  \brief ���ݷ���߿򴴽��棬�ӿ�������.
    */
    bool createClosedFace(TopoDS_Wire closedWire);

	/*! \fn bool createNurbsSurface(const TColgp_Array2OfPnt& Poles, const TColStd_Array2OfReal& Weights, const TColStd_Array1OfReal& UKnots, const TColStd_Array1OfReal& VKnots, const TColStd_Array1OfInteger& UMults, const TColStd_Array1OfInteger& VMults, const Standard_Integer UDegree, const Standard_Integer VDegree)
	*  \brief ����Nurbs��Ľӿ�.
	*  \param Poles ���Ƶ��ά����.
	*  \param Weights Ȩ������, ���ȫΪ1�򹹽�������������.
	*  \param UKnots U����ڵ�.
	*  \param VKnots V����ڵ�.
	*  \param UMults U�����ظ��ڵ�.
	*  \param VMults V�����ظ��ڵ�.
	*  \param UDegree U�������ɶ�.
	*  \param VDegree V�������ɶ�.
	*  \return �Ƿ�ɹ�����.
	*/
	bool createNurbsSurface(const TColgp_Array2OfPnt& Poles, const TColStd_Array2OfReal& Weights, 
		const TColStd_Array1OfReal& UKnots, const TColStd_Array1OfReal& VKnots, 
		const TColStd_Array1OfInteger& UMults, const TColStd_Array1OfInteger& VMults, 
		const Standard_Integer UDegree, const Standard_Integer VDegree);

    bool createInterpolateBSplineSurface(TColgp_Array2OfPnt poles);
};

#endif
