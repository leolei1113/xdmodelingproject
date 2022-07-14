/*! \file EdgeShapeBuilder.h
*   \brief EdgeShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef EDGESHAPEBUILDER_H
#define EDGESHAPEBUILDER_H

#include "OccShapeBuilder.h"
#include "xdModelingHeader.h"

#include <TopoDS_Edge.hxx>

#include <vector>

/*! \class EdgeShapeBuilder
*   \brief ���ڴ�������edge����
 */
class XDMODELING_API EdgeShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder�Ĺ��캯��.
    */
    EdgeShapeBuilder();
    /*! \fn ~EdgeShapeBuilder()
    *  \brief EdgeShapeBuilder����������.
    */
    ~EdgeShapeBuilder();

public:
    /*! \fn bool createLine(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2)
    *  \brief �����߶�.
    *  \param xPos1 ���x����.
    *  \param yPos1 ���y����.
    *  \param zPos1 ���z����.
    *  \param xPos2 �յ�x����.
    *  \param yPos2 �յ�y����.
    *  \param zPos2 �յ�z����.
    *  \return �Ƿ�ɹ�����.
    */
    bool createLine(const double& xPos1, const double& yPos1, const double& zPos1,
        const double& xPos2, const double& yPos2, const double& zPos2);

    /*! \fn bool createThreePointArc(std::vector<std::vector<double>>& points)
    *  \brief ��������Բ��.
    *  \param points ������������.
    *  \return �Ƿ�ɹ�����.
    */
    bool createThreePointArc(std::vector<std::vector<double>>& points);

    /*! \fn bool createAngularArc(std::vector<std::vector<double>>& points, const double& angleInDeg, const double& xAxis, const double& yAxis, const double& zAxis, std::vector<double>& newEndPnt
    *  \brief �������ĵ�Բ��.
    *  \param points �����������У�����Ϊ��㡢Բ�ġ��յ�����.
    *  \param angleInDeg Բ���Ƕ�.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createAngularArc(std::vector<std::vector<double>>& points, const double& angleInDeg, const double& xAxis, const double& yAxis, const double& zAxis, std::vector<double>& newEndPnt);

    /*! \fn bool createPolyLine(std::vector<std::vector<double>>& points)
    *  \brief ���������.
    *  \param points ����߸��ڵ�����.
    *  \return �Ƿ�ɹ�����.
    */
    bool createPolyLine(std::vector<std::vector<double>>& points);

    /*! \fn bool createBSpline(std::vector<std::vector<double>>& points)
    *  \brief ����B��������.
    *  \param points �����Ƶ�����.
    *  \return �Ƿ�ɹ�����.
    */
    bool createBSpline(std::vector<std::vector<double>>& points);

    /*! \fn bool createInterpolateBSpline(std::vector<std::vector<double>>& points)
    *  \brief ������ֵ����.
    *  \param points �����Ƶ�����.
    *  \return �Ƿ�ɹ�����.
    */
    bool createInterpolateBSpline(std::vector<std::vector<double>>& points);

    /*! \fn bool createMultiTypeSpline(std::vector<std::vector<double>>& ctrlPoints, int type)
    *  \brief ���������������ߣ���������B������׼����B����������B����
    *  \param points �����Ƶ�����.
    *  \param type ������������.
    *  \return �Ƿ�ɹ�����.
    */
    bool createMultiTypeSpline(std::vector<std::vector<double>>& ctrlPoints, int type);

    /*! \fn bool createHyperbola(std::vector<double>& centerP, const double& majorR, const double& minorR, const double& startP, const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����˫����.
    *  \param points ���ĵ�����.
    *  \param majorR ���뾶.
    *  \param minorR �ΰ뾶.
    *  \param startP ˫������ʼtֵ.
    *  \param endP ˫������ֹtֵ.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createHyperbola(std::vector<double>& centerP, const double& majorR, const double& minorR, const double& startP,
        const double& endP, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createParabola(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����������.
    *  \param points ���ĵ�����.
    *  \param focalDistance ����.
    *  \param startP ˫������ʼtֵ.
    *  \param endP ˫������ֹtֵ.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createParabola(std::vector<double>& centerP, const double& focalDistance, const double& startP, const double& endP,
        const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createHelix(const double& globalXcenter, const double& globalYcenter, const double& globalZcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief �����ռ�������.
    *  \param centerX ��ʼ��Բ��x����.
    *  \param centerY ��ʼ��Բ��y����.
    *  \param centerZ ��ʼ��Բ��z����.
    *  \param lowerRadius ���Բ�뾶.
    *  \param upperRadius �յ�Բ�뾶.
    *  \param height �߶�.
    *  \param turns ����.
    *  \param rightHanded �Ƿ�����.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createHelix(gp_Pnt startpnt, const double& centerX, const double& centerY, const double& centerZ,
        const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded,
        const double& xAxis, const double& yAxis, const double& zAxis);

	/*! \fn bool createHelix(const double& globalXcenter, const double& globalYcenter, const double& globalZcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
	*  \brief �����ռ�������.
	*  \param centerX ��ʼ��Բ��x����.
	*  \param centerY ��ʼ��Բ��y����.
	*  \param centerZ ��ʼ��Բ��z����.
	*  \param startX ��ʼ��x����.
	*  \param startY ��ʼ��y����.
	*  \param startZ ��ʼ��z����.
	*  \param upperRadius �յ�Բ�뾶.
	*  \param height �߶�.
	*  \param turns ����.
	*  \param rightHanded �Ƿ�����.
	*  \param xAxis ����ƽ�淨������xֵ.
	*  \param yAxis ����ƽ�淨������yֵ.
	*  \param zAxis ����ƽ�淨������zֵ.
	*  \return �Ƿ�ɹ�����.
	*/
	bool createHelix(const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ, const double& upperRadius, const double& height, const double& turns, const double& rightHanded,
		const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createSpiral(const double& centerX, const double& centerY, const double& centerZ, const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����ƽ��������.
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param lowerRadius ���Բ�뾶.
    *  \param upperRadius �յ�Բ�뾶.
    *  \param turns ����.
    *  \param rightHanded �Ƿ�����.
    *  \param xAxis ����ƽ�淨������xֵ.
    *  \param yAxis ����ƽ�淨������yֵ.
    *  \param zAxis ����ƽ�淨������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createSpiral(const double& centerX, const double& centerY, const double& centerZ, const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis);

	/*! \fn bool createHelix(const double& globalXcenter, const double& globalYcenter, const double& globalZcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis)
	*  \brief ����ƽ��������.
	*  \param centerX ��ʼ��Բ��x����.
	*  \param centerY ��ʼ��Բ��y����.
	*  \param centerZ ��ʼ��Բ��z����.
	*  \param startX ��ʼ��x����.
	*  \param startY ��ʼ��y����.
	*  \param startZ ��ʼ��z����.
	*  \param upperRadius �յ�Բ�뾶.
	*  \param turns ����.
	*  \param rightHanded �Ƿ�����.
	*  \param xAxis ����ƽ�淨������xֵ.
	*  \param yAxis ����ƽ�淨������yֵ.
	*  \param zAxis ����ƽ�淨������zֵ.
	*  \return �Ƿ�ɹ�����.
	*/
	bool createSpiral(const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ, const double& upperRadius, const double& turns, const double& rightHanded,
		const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createFuncCurve(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, const std::string varName, const double startU, const double endU)
    *  \brief ������������.
    *  \param xFuncExpress ��������x����ı仯����.
    *  \param yFuncExpress ��������y����ı仯����.
    *  \param zFuncExpress ��������z����ı仯����.
    *  \param varName ��������.
    *  \param startU ������ʼtֵ.
    *  \param endU ������ʼtֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createFuncCurve(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress,
        const std::string varName, const double startU, const double endU);

    bool createEllipseArc(std::vector<double>& centerP, std::vector<double>& axis, const double& majorR,
        const double& minorR, double startAng, double endAng);

};

#endif