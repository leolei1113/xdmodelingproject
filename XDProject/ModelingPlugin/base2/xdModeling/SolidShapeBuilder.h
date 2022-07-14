/*! \file SolidShapeBuilder.h
*   \brief SolidShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef SOLIDSHAPEBUILDER_H
#define SOLIDSHAPEBUILDER_H

#include "OccShapeBuilder.h"
#include "xdModelingHeader.h"

#include <vector>

/*! \class SolidShapeBuilder
*   \brief ���ڴ����������ʵ�����
 */
class XDMODELING_API SolidShapeBuilder :
    public OccShapeBuilder
{
public:
    /*! \fn SolidShapeBuilder()
    *  \brief SolidShapeBuilder�Ĺ��캯��.
    */
    SolidShapeBuilder();
    /*! \fn ~SolidShapeBuilder()
    *  \brief SolidShapeBuilder����������.
    */
    ~SolidShapeBuilder();

public:
    /*! \fn bool createCuboid(const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize)
    *  \brief ����������ʵ��.
    *  \param xStart ���������x����.
    *  \param yStart ���������y����.
    *  \param zStart ���������z����.
    *  \param xSize ������x���򳤶�.
    *  \param ySize ������y���򳤶�.
    *  \param zSize ������z���򳤶�.
    *  \return �Ƿ�ɹ�����.
    */
    bool createCuboid(const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize);

    /*! \fn bool createCylinder(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height)
    *  \brief ����Բ��ʵ��.
    *  \param xBottomCenter ����Բ��x����.
    *  \param yBottomCenter ����Բ��y����.
    *  \param zBottomCenter ����Բ��z����.
    *  \param xAxis ����������xֵ.
    *  \param yAxis ����������yֵ.
    *  \param zAxis ����������zֵ.
    *  \param radius �뾶.
    *  \param height �߶�.
    *  \return �Ƿ�ɹ�����.
    */
    bool createCylinder(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height);

    /*! \fn bool createCone(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����Բ׶��Բ̨ʵ��.
    *  \param botCenterX ����Բ��x����.
    *  \param botCenterY ����Բ��y����.
    *  \param botCenterZ ����Բ��z����.
    *  \param lowerRadius ����뾶.
    *  \param upperRadius ����뾶��Ϊ0��ΪԲ׶.
    *  \param height �߶�.
    *  \param xAxis ����������xֵ.
    *  \param yAxis ����������yֵ.
    *  \param zAxis ����������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createCone(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createSphere(const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus)
    *  \brief ��������ʵ��.
    *  \param xCenter ����x����.
    *  \param yCenter ����y����.
    *  \param zCenter ����z����.
    *  \param radius �뾶.
    *  \return �Ƿ�ɹ�����.
    */
    bool createSphere(const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus);

    /*! \fn bool createTorus(const double& centerX, const double& centerY, const double& centerZ, const double& minorRadius, const double& majorRadius, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����Բ��ʵ��.
    *  \param centerX Բ��x����.
    *  \param centerY Բ��y����.
    *  \param centerZ Բ��z����.
    *  \param minorRadius СԲ�뾶.
    *  \param majorRadius ��Բ�뾶.
    *  \param xAxis ����������xֵ.
    *  \param yAxis ����������yֵ.
    *  \param zAxis ����������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createTorus(const double& centerX, const double& centerY, const double& centerZ, const double& minorRadius, const double& majorRadius, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createPrism(const std::vector<std::vector<double>>& pPos, const double& xVec, const double& yVec, const double& zVec)
    *  \brief ��������ʵ��.
    *  \param pPos �����������������.
    *  \param xVec ������������xֵ.
    *  \param yVec ������������yֵ.
    *  \param zVec ������������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createPrism(const std::vector<std::vector<double>>& pPos, const double& xVec, const double& yVec, const double& zVec);

    /*! \fn bool createFrustrumPrism(const std::vector<std::vector<double>>& pPos, const double& scale, const double& xVec, const double& yVec, const double& zVec)
    *  \brief ������̨ʵ��.
    *  \param pPos �����������������.
    *  \param scale ��������ϵ�������ű���.
    *  \param xVec ������������xֵ.
    *  \param yVec ������������yֵ.
    *  \param zVec ������������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createFrustrumPrism(const std::vector<std::vector<double>>& pPos, const double& scale, const double& xVec, const double& yVec, const double& zVec);

    /*! \fnbool createRegularPolyhedron(double xBotCenter, double yBotCenter, double zBotCenter, double xBotStart, double yBotStart, double zBotStart, const double& height, const int& numSides, double xAxis, double yAxis, double zAxis)
    *  \brief ���������������ʵ��.
    *  \param xBotCenter ��������x����.
    *  \param yBotCenter ��������y����.
    *  \param zBotCenter ��������z����.
    *  \param xBotStart ���x����.
    *  \param yBotStart ���y����.
    *  \param zBotStart ���z����.
    *  \param height �߶�.
    *  \param numSides ������αߵ���Ŀ.
    *  \param xAxis ����������xֵ.
    *  \param yAxis ����������yֵ.
    *  \param zAxis ����������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createRegularPolyhedron(double xBotCenter, double yBotCenter, double zBotCenter, double xBotStart, double yBotStart, double zBotStart, const double& height, const int& numSides, double xAxis, double yAxis, double zAxis);

    /*! \fn bool createFlare(const double& xbotCenter, const double& ybotCenter, const double& zbotCenter, const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth, const double& Height, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����������ʵ��.
    *  \param xbotCenter ��������x����.
    *  \param ybotCenter ��������y����.
    *  \param zbotCenter ��������z����.
    *  \param lowerWidth ������ο��.
    *  \param lowerDepth ������γ���.
    *  \param upperWidth ������ο��.
    *  \param upperDepth ������γ���.
    *  \param height �߶�.
    *  \param xAxis ����������xֵ.
    *  \param yAxis ����������yֵ.
    *  \param zAxis ����������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createFlare(const double& xbotCenter, const double& ybotCenter, const double& zbotCenter, const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth, const double& height, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createEllipsoid(const double& XbotCenter, const double& YbotCenter, const double& ZbotCenter, const double& majRadius, const double& ratio, const double& xAxis, const double& yAxis, const double& zAxis)
    *  \brief ����������
    *  \param XbotCenter ��������x����.
    *  \param YbotCenter ��������y����.
    *  \param ZbotCenter ��������z����.
    *  \param majRadius ���᳤��.
    *  \param ratio ���������.
    *  \param xAxis ����������xֵ.
    *  \param yAxis ����������yֵ.
    *  \param zAxis ����������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createEllipsoid(const double& XbotCenter, const double& YbotCenter, const double& ZbotCenter, const double& majRadius, const double& ratio, const double& xAxis, const double& yAxis, const double& zAxis);

    /*! \fn bool createHelixSolid(const double& Xcenter, const double& Ycenter, const double& Zcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& profileRadius, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis);
    *  \brief ����������ʵ��.
    *  \param Xcenter ���Բ��Բ��x����.
    *  \param Ycenter ���Բ��Բ��y����.
    *  \param Zcenter ���Բ��Բ��z����.
    *  \param lowerRadius ���Բ�뾶.
    *  \param upperRadius �յ�Բ�뾶.
    *  \param height �߶�.
    *  \param turns ����.
    *  \param profileRadius ����뾶.
    *  \param rightHanded �Ƿ�����.
    *  \param xAxis ����������xֵ.
    *  \param yAxis ����������yֵ.
    *  \param zAxis ����������zֵ.
    *  \return �Ƿ�ɹ�����.
    */
    bool createHelixSolid(const double& Xcenter, const double& Ycenter, const double& Zcenter, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& profileRadius, const double& rightHanded, const double& xAxis, const double& yAxis, const double& zAxis);

    bool createSpindle(double xbotcenter, double ybotcenter, double zbotcenter,
        double majradius, double ratio, double xaxis, double yaxis, double zaxis);
};

#endif
