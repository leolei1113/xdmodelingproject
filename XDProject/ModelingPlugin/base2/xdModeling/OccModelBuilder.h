/*! \file OccModelBuilder.h
*   \brief OccModelBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/
#ifndef OCCMODELBUILDER_H
#define OCCMODELBUILDER_H

#include "xdModelingHeader.h"
#include "xdModelingDataType.h"
#include "GeomModelBuilder.h"
#include "EnumAndStruct.h"

#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <algorithm>

struct SplitFaceCreationInfos
{
    bool threepointtype = false;
    bool pointdirtype = false;
    bool isxoy = false;
    bool isxoz = false;
    bool isyoz = false;
    void* pt1;
    void* pt2;
    void* pt3;
    void* pt4;
    double dir[3];
};

struct SplitLineCreationInfos
{
    bool twopointtype = false;
    bool pointedgetype = false;
    void* pt1;
    void* pt2;
    void* pt3;
    void* edge;
};

class TDocStd_Document;
class OccModel;
/*! \class OccModelBuilder
*   \brief ���ڸ������ʵ��Ĵ����ͱ任����������㡢�ߡ��桢ʵ�塢�������㡢���뵼���ȡ�
 */
class XDMODELING_API OccModelBuilder : public GeomModelBuilder
{
public:
    /*! \enum OccModelBuilder::EnFileType
    * ģ���ļ�����
    */
	enum EnFileType
	{
		ENIGS,
		ENSTP,
		ENBREP
	};

    /*! \fn OccModelBuilder()
    *  \brief OccModelBuilder�Ĺ��캯��.
    */
	OccModelBuilder();

    /*! \fn virtual ~OccModelBuilder()
    *  \brief OccModelBuilder����������.
    */
	virtual ~OccModelBuilder();

    /*! \fn virtual void builderBegin()
    *  \brief ģ�������ڵ��ø�������ʱ�������ʼ�ķ��������ڳ�����������.
    */
	virtual void builderBegin();

    /*! \fn virtual void builderEnd()
    *  \brief ģ�������ڵ��ø�������ʱ������������ķ��������ڳ�����������.
    */
	virtual void builderEnd();

    /*! \fn virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const int& value)
    *  \brief Ϊģ�͸�int��������.
    *  \param modelEntity ģ��TopoDS_HShapeָ��.
    *  \param propertyName ��������.
    *  \param value ����ֵ.
    *  \return �����Ƿ�ɹ���ֵ.
    */
	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const int& value);

    /*! \fn virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const std::string& value)
    *  \brief Ϊģ�͸�std::string��������.
    *  \param modelEntity ģ��TopoDS_HShapeָ��.
    *  \param propertyName ��������.
    *  \param value ����ֵ.
    *  \return �����Ƿ�ɹ���ֵ.
    */
	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const std::string& value);

    /*! \fn virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const double& value)
    *  \brief Ϊģ�͸�double��������.
    *  \param modelEntity ģ��TopoDS_HShapeָ��.
    *  \param propertyName ��������.
    *  \param value ����ֵ.
    *  \return �����Ƿ�ɹ���ֵ.
    */
	virtual bool addModelAttribute(void* modelEntity, const std::string& propertyName, const double& value);

    /*! \fn virtual bool addModelAttribute(void* modelEntity, double dRed, double dGreen, double dBlue)
    *  \brief �ӿ�������.
    */
	virtual bool addModelAttribute(void* modelEntity, double dRed, double dGreen, double dBlue);

    /*! \fn double tolerance()
    *  \brief ��ȡ����.
    *  \return ��������.
    */
    virtual double tolerance();

    /*! \fn virtual void HandlePrimitiveAnnotations()
    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief �ӿ�δʵ��.
    */
    virtual void HandlePrimitiveAnnotations();

    /*! \fn virtual void HandleCopyAnnotations()
    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief �ӿ�δʵ��.
    */
    virtual void HandleCopyAnnotations();

    /*! \fn virtual void HandleSplitAnnotations()
    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief �ӿ�δʵ��.
    */
    virtual void HandleSplitAnnotations();

    /*! \fn virtual std::string CreateEdgeNameFromEntity(void* pSelEdge)
    *  \brief �ӿ�δʵ��.
    */
    virtual std::string CreateEdgeNameFromEntity(void* pSelEdge);

public:
    /*! \fn virtual bool createVertex(const double& xPos, const double& yPos, const double& zPos, const char* name, std::string& strPID, void*& resEntity)
    *  \brief �ӿ�������.
    */
    virtual bool createVertex(const double& xPos, const double& yPos, const double& zPos,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual  virtual bool createLine(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ӿ�������.
	*/
    virtual bool createLine(const double& xPos1, const double& yPos1, const double& zPos1,
        const double& xPos2, const double& yPos2, const double& zPos2,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual  virtual bool createPlane(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ӿ�������.
	*/
    virtual bool createPlane(const double& xPos1, const double& yPos1, const double& zPos1,
        const double& xPos2, const double& yPos2, const double& zPos2,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn bool importModel(const std::string& path, const char* name, const EnFileType& type, std::string& strPID)y)
    *  \brief �ӿ�������.
    */
    bool importModel(const std::string& path, const char* name, const EnFileType& type, std::string& strPID);

    /*! \fn static bool exportModel(const std::string& path, const EnFileType& type, const std::string& strPID)
    *  \brief �ӿ�������.
    */
    static bool exportModel(const std::string& path, const EnFileType& type, const std::string& strPID);

    /*! \fn bool exportEntityList(std::vector<void *>entList, std::string& path)
    *  \brief �ӿ�������.
    */
    bool exportEntityList(std::vector<void *>entList, std::string& path);

    /*! \fn bool deleteLabel(const std::string& strPID)
    *  \brief �ӿ�������.
    */
    bool deleteLabel(const std::string& strPID);

    /************************************************************************/
    /*                             real interface                           */
    /************************************************************************/
    /*! \fn virtual bool createUnusedEntity(const char* name, std::string& strPID, void*& resEntity)
    *  \brief ������0,0,0������1,0,0�����߶�.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createUnusedEntity(const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createPolyline(double* wcsMatrix, std::vector<std::vector<double>>& points, std::vector<PolylineSegment>& segments, const LOGICAL& isClosed, const LOGICAL& isCovered, const char * name, std::string & strPID, void *& resEntity);
    *  \brief �������������߶λ������ߵķ����������߶Ρ�B����������Բ���ߡ����ĵ�Բ����.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param points �����߶λ��������õ��ĵ��������У�Ϊn*3.
    *  \param segments �����߶����Ͷ����Լ����ĵ�Բ���ߵĲ���.
    *  \param isClosed �����߶λ��������Ƿ�պ�.
    *  \param isCovered ���������߶λ��������Ƿ����ɱպ����߲�������.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createPolyline(double* wcsMatrix, std::vector<std::vector<double>>& points, std::vector<PolylineSegment>& segments,
        const LOGICAL& isClosed, const LOGICAL& isCovered, const CrossSection& crossSection,
        const char * name, std::string & strPID, void *& resEntity);//1st stage

    /*! \fn virtual bool createHyperbola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief ˫���ߵĴ����ӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param points ����˫����ʱ�õ����������꣬����Ϊ��㡢���ĵ㡢�յ�.
    *  \param majorR ���뾶.
    *  \param minorR �ΰ뾶.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createHyperbola(double* wcsMatrix, std::vector<std::vector<double>>& points,
        const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createParabola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief ���������ߵĽӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param points ����������ʱ�õ����������꣬����Ϊ��㡢���ĵ㡢�յ�.
    *  \param focalDistance ����.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createParabola(double* wcsMatrix, std::vector<std::vector<double>>& points,
        const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createHyperbola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief ˫���ߵĴ����ӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param points ����˫����ʱ�õ������ĵ�����.
    *  \param tStart ��ʼtֵ.
    *  \param tEnd ��ֹtֵ.
    *  \param majorR ���뾶.
    *  \param minorR �ΰ뾶.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createHyperbola(double* wcsMatrix, std::vector<double>& points, const double& tStart, const double& tEnd,
        const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createParabola(double* wcsMatrix, std::vector<std::vector<double>>& points, const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
    *  \brief ���������ߵĽӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param points ����������ʱ�õ������ĵ�����.
    *  \param tStart ��ʼtֵ.
    *  \param tEnd ��ֹtֵ.
    *  \param focalDistance ����.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createParabola(double* wcsMatrix, std::vector<double>& points, const double& tStart, const double& tEnd,
        const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity);

    /*! \fn virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief �ռ������ߵĴ����ӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX ��ʼ��Բ��x����.
    *  \param centerY ��ʼ��Բ��y����.
    *  \param centerZ ��ʼ��Բ��z����.
    *  \param lowerRadius ���Բ�뾶.
    *  \param upperRadius �յ�Բ�뾶.
    *  \param height �߶�.
    *  \param turns ����.
    *  \param rightHanded �Ƿ�����.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& lowerRadius, const double& upperRadius, const double& height,
        const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& newEntry, void*& resEntity)
	*  \brief �ռ������ߵĴ����ӿ�.
	*  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
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
	*  \param axis �����ᳯ��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ,
		const double& upperRadius, const double& height,
		const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const double& maxRadius, const double& turns, const double& rightHanded,const DATUMAXISTYPE& axis,const char* name, std::string& newEntry, void*& resEntity) void*& resEntity)
	*  \brief ƽ�������ߵĴ����ӿ�.
	*  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
	*  \param centerX ���ĵ�x����.
	*  \param centerY ���ĵ�y����.
	*  \param centerZ ���ĵ�z����.
	*  \param startX ��ʼ��x����.
	*  \param startY ��ʼ��y����.
	*  \param startZ ��ʼ��z����.
	*  \param maxRadius �յ�Բ�뾶.
	*  \param turns ����.
	*  \param rightHanded �Ƿ�����.
	*  \param axis �����ᳯ��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& startX, const double& startY, const double& startZ,
		const double& maxRadius, const double& turns, const double& rightHanded,
		const DATUMAXISTYPE& axis,
		const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ƽ�������ߵĴ����ӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param lowerRadius ���Բ�뾶.
    *  \param upperRadius �յ�Բ�뾶.
    *  \param turns ����.
    *  \param rightHanded �Ƿ�����.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded,
        const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEquationCurve(double* wcsMatrix, const char* xtFunction, const char* ytFunction, const char* ztFunction, const double& startT, const double& strEndT, const char* name, std::string& strPID, void*& resEntity)
    *  \brief �����������Ĵ����ӿ�.
	*  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
	*  \param xtFunction x����T�ķ���.
	*  \param ytFunction y����T�ķ���.
	*  \param ztFunction z����T�ķ���.
	*  \param startT Tֵ����ʼֵ.
	*  \param strEndT Tֵ����ֵֹ.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
    */
    virtual bool createEquationCurve(double* wcsMatrix, const char* xtFunction, const char* ytFunction,
        const char* ztFunction, const double& startT, const double& strEndT,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createRectangle(double* wcsMatrix, const double& startX, const double& startY, const double& startZ, const double& width, const double& height, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ��������������߿�Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param startX ���x����.
    *  \param startY ���y����.
    *  \param startZ ���z����.
    *  \param width ���.
    *  \param height �߶�.
    *  \param axis �����ᳯ��.
    *  \param isCovered �Ƿ�������.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createRectangle(double* wcsMatrix, const double& startX, const double& startY, const double& startZ,
        const double& width, const double& height, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCircle(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& radius, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����Բ����Բ���߿�Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param radius �뾶.
    *  \param axis �����ᳯ��.
    *  \param isCovered �Ƿ�������.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createCircle(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& radius, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        int segment, const char* name, std::string& strPID, void*& entity);

    /*! \fn virtual bool createSector(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& radius, const double& startAng, const double& endAng, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief �ӿ�������.
    */
	virtual bool createSector(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
		const double& radius, const double& startAng, const double& endAng, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
		const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createRegularPolygon(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, const int& numSides, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief �����������������߿�Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param startX ���x����.
    *  \param startY ���y����.
    *  \param startZ ���z����.
    *  \param numSides ������αߵ���Ŀ.
    *  \param axis �����ᳯ��.
    *  \param isCovered �Ƿ�������.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createRegularPolygon(double* wcsMatrix, const double& centerX, const double& centerY,
        const double& centerZ, const double& startX, const double& startY, const double& startZ,
        const int& numSides, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEllipse(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ������Բ������߿�Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX ���ĵ�x����.
    *  \param centerY ���ĵ�y����.
    *  \param centerZ ���ĵ�z����.
    *  \param majRadius ����뾶.
    *  \param ratio ���������.
    *  \param axis �����ᳯ��.
    *  \param isCovered �Ƿ�������.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createEllipse(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ӿ�������.
	*/
    virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter,
        const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const DATUMAXISTYPE& axis, const double& raidus, const double& height, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����Բ����Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param xBottomCenter ����Բ��x����.
    *  \param yBottomCenter ����Բ��y����.
    *  \param zBottomCenter ����Բ��z����.
    *  \param xAxis Բ����������xֵ.
    *  \param yAxis Բ����������yֵ.
    *  \param zAxis Բ����������zֵ.
    *  \param radius �뾶.
    *  \param height Բ���߶�.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter,
        const DATUMAXISTYPE& axis, const double& raidus, const double& height,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createConeFace(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����Բ׶�����Բ̨��Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param botCenterX ����Բ��x����.
    *  \param botCenterY ����Բ��y����.
    *  \param botCenterZ ����Բ��z����.
    *  \param lowerRadius ����뾶.
    *  \param upperRadius ����뾶��Ϊ0��Բ׶��.
    *  \param axis �����ᳯ��.
    *  \param height Բ���߶�.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createConeFace(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEquationSurface(double* wcsMatrix, const char* xuvFunction, const char* yuvFunction, const char* zuvFunction, const double& startU, const double& strEndU, const double& startV, const double& strEndV, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ������������Ľӿ�.
	*  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
	*  \param xuvFunction x����U��V�ķ���.
	*  \param yuvFunction y����U��V�ķ���.
	*  \param zuvFunction z����U��V�ķ���.
	*  \param startU Uֵ����ʼֵ.
	*  \param strEndU Uֵ����ֵֹ.
	*  \param startV Vֵ����ʼֵ.
	*  \param strEndV Vֵ����ֵֹ.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
    */
    virtual bool createEquationSurface(double* wcsMatrix, const char* xuvFunction, const char* yuvFunction, const char* zuvFunction,
        const double& startU, const double& strEndU, const double& startV, const double& strEndV,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCuboid(double* wcsMatrix, const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����������Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param xStart ���������x����.
    *  \param yStart ���������y����.
    *  \param zStart ���������z����.
    *  \param xSize ������x���򳤶�.
    *  \param ySize ������y���򳤶�.
    *  \param zSize ������z���򳤶�.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createCuboid(double* wcsMatrix, const double& xStart, const double& yStart, const double& zStart,
        const double& xSize, const double& ySize, const double& zSize, const char* name,
        std::string& strPID, void*& resEntity);

    /*! \fn  virtual bool createCylinder(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& raidus, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����Բ��ʵ��Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param xBottomCenter ����Բ��x����.
    *  \param yBottomCenter ����Բ��y����.
    *  \param zBottomCenter ����Բ��z����.
    *  \param radius �뾶.
    *  \param height �߶�.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createCylinder(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter,
        const double& raidus, const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createSphere(double* wcsMatrix, const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ��������ʵ��Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param xCenter ����x����.
    *  \param yCenter ����y����.
    *  \param zCenter ����z����.
    *  \param radius �뾶.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createSphere(double* wcsMatrix, const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createCone(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����Բ׶����Բ̨ʵ��Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param botCenterX ����Բ��x����.
    *  \param botCenterY ����Բ��y����.
    *  \param botCenterZ ����Բ��z����.
    *  \param lowerRadius ����뾶.
    *  \param upperRadius ����뾶��Ϊ0��ΪԲ׶.
    *  \param height �߶�.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createCone(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn
    *  \brief ����������������Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param botCenterX ��������x����.
    *  \param botCenterY ��������y����.
    *  \param botCenterZ ��������z����.
    *  \param startX ���x����.
    *  \param startY ���y����.
    *  \param startZ ���z����.
    *  \param height �߶�.
    *  \param numSides ������αߵ���Ŀ.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createRegularPolyhedron(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& startX, const double& startY, const double& startZ,
        const double& height, const int& numSides, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createTorus(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& minorRadius, const double& majorRadius, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����Բ��ʵ��Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX Բ��x����.
    *  \param centerY Բ��y����.
    *  \param centerZ Բ��z����.
    *  \param minorRadius СԲ�뾶.
    *  \param majorRadius ��Բ�뾶.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createTorus(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& minorRadius, const double& majorRadius, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createFlare(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ������ʵ�崴���ӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param botCenterX ��������x����.
    *  \param botCenterY ��������y����.
    *  \param botCenterZ ��������z����.
    *  \param lowerWidth ������ο��.
    *  \param lowerDepth ������γ���.
    *  \param upperWidth ������ο��.
    *  \param upperDepth ������γ���.
    *  \param height �߶�.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createFlare(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ,
        const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth,
        const double& height, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEllipsoid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ��������ʵ��.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX ��������x����.
    *  \param centerY ��������y����.
    *  \param centerZ ��������z����.
    *  \param majRadius ���᳤��.
    *  \param ratio ���������.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createEllipsoid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
        const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createHelixSolid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& profileRadius, const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ����������ʵ��Ľӿ�.
    *  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
    *  \param centerX ���Բ��Բ��x����.
    *  \param centerY ���Բ��Բ��y����.
    *  \param centerZ ���Բ��Բ��z����.
    *  \param profileRadius ����뾶.
    *  \param lowerRadius ���Բ�뾶.
    *  \param upperRadius �յ�Բ�뾶.
    *  \param height �߶�.
    *  \param turns ����.
    *  \param rightHanded �Ƿ�����.
    *  \param axis �����ᳯ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
	virtual bool createHelixSolid(double* wcsMatrix,
		const double& centerX, const double& centerY, const double& centerZ,
		const double& profileRadius, const double& lowerRadius, const double& upperRadius,
		const double& height, const double& turns, const double& rightHanded,
		const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool makeBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ��������Ľӿڣ����������������ӡ.
    *  \param boolType ������������ͣ�Ϊ�����������ӡ.
    *  \param isKeep �Ƿ���ԭ��ģ��.
    *  \param entryList ���н��в������㴦���ģ��TopoDS_HShape��ָ������.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ����в�������.
    */
    virtual bool makeBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool translate(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ, const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ��ƽ�ơ��������еĽӿ�.
    *  \param entry ���б任ģ�͵�TopoDS_HShape��ָ��.
    *  \param wcsMatrix ģ�ͽ��б任�ľֲ�����ϵ����.
    *  \param vectorX ƽ��������x����.
    *  \param vectorY ƽ��������y����.
    *  \param vectorZ ƽ��������z����.
    *  \param totalNumber ����ƽ�ƵĴ���.
    *  \param isAttach �Ƿ�����.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool translate(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ,
        const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity);

    //��ת
    /*! \fn virtual bool rotate(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ�ͽ���������ת����Բ�����еĽӿ�.
    *  \param entry ���б任ģ�͵�TopoDS_HShape��ָ��.
    *  \param wcsMatrix ģ�ͽ��б任�ľֲ�����ϵ����.
    *  \param axisPosX ��ת������x����.
    *  \param axisPosY ��ת������y����.
    *  \param axisPosZ ��ת������z����.
    *  \param axisVecX ��ת�᷽��������x����.
    *  \param axisVecY ��ת�᷽��������y����.
    *  \param axisVecZ ��ת�᷽��������z����.
    *  \param angle ��ת�Ƕ�.
    *  \param totalNumber ����ƽ�ƵĴ���.
    *  \param isAttach �Ƿ�����.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool rotate(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ,
        const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle,
        const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity);

    //����
    /*! \fn virtual bool mirror(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const LOGICAL& isCopy, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ�ͽ��о���Ľӿ�.
    *  \param entry ���б任ģ�͵�TopoDS_HShape��ָ��.
    *  \param wcsMatrix ģ�ͽ��б任�ľֲ�����ϵ����.
    *  \param basePosX ����ƽ��������һ���x����.
    *  \param basePosY ����ƽ��������һ���y����.
    *  \param basePosZ ����ƽ��������һ���z����.
    *  \param normalVecX ����ƽ�淨��������x����.
    *  \param normalVecY ����ƽ�淨��������y����.
    *  \param normalVecZ ����ƽ�淨��������z����.
    *  \param isCopy �Ƿ���ԭģ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool mirror(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& normalVecX, const double& normalVecY, const double& normalVecZ, const LOGICAL& isCopy,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool scale(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const LOGICAL& isCopy, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ�����ŵĽӿ�.
    *  \param entry ���б任ģ�͵�TopoDS_HShape��ָ��.
    *  \param wcsMatrix ģ�ͽ��б任�ľֲ�����ϵ����.
    *  \param basePosX ���Ųο����x����.
    *  \param basePosY ���Ųο����y����.
    *  \param basePosZ ���Ųο����z����.
    *  \param scaleX x�������ű���.
    *  \param scaleY y�������ű���.
    *  \param scaleZ z�������ű���.
    *  \param isCopy �Ƿ���ԭģ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool scale(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& scaleX, const double& scaleY, const double& scaleZ, const LOGICAL& isCopy,
        const char* name, std::string& strPID, void*& resEntity);

    //extend
    //����
    /*! \fn virtual bool sweepAlongVector(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ������Ľӿ�.
    *  \param entry ��������ģ�͵�TopoDS_HShape��ָ��.
    *  \param wcsMatrix ģ�ͽ��б任�ľֲ�����ϵ����.
    *  \param vectorX ����������x����.
    *  \param vectorY ����������y����.
    *  \param vectorZ ����������z����.
    *  \param draftAngle ��ģ�Ƕ�.
    *  \param draftType ��ģ����.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool sweepAlongVector(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY,
        const double& vectorZ, const double& draftAngle, const SWEEPDRAFTTYPE& draftType,
        const char* name, std::string& strPID, void*& resEntity);

    //��ת
    /*! \fn virtual bool sweepAroundAxis(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ����ת���͵Ľӿ�.
    *  \param entry ������תģ�͵�TopoDS_HShape��ָ��.
    *  \param wcsMatrix ģ�ͽ��б任�ľֲ�����ϵ����.
    *  \param axisPosX ��ת�����x����.
    *  \param axisPosY ��ת�����y����.
    *  \param axisPosZ ��ת�����z����.
    *  \param axisVecX ��ת�᷽��������x����.
    *  \param axisVecY ��ת�᷽��������y����.
    *  \param axisVecZ ��ת�᷽��������z����.
    *  \param angle ��ת�Ƕ�.
    *  \param draftAngle ��ģ�Ƕ�.
    *  \param draftType ��ģ����.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool sweepAroundAxis(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY,
        const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ,
        const double& angle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType,
        const char* name, std::string& strPID, void*& resEntity);

    //ɨ��
    /*! \fn virtual bool sweepAlongPath(void* profileEntry, void* pathEntry, const double& twistAngle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ��ɨ�ԵĽӿ�.
    *  \param profileEntry ����ɨ�ӵĽ����TopoDS_HShape��ָ��.
    *  \param entry ����ɨ�ӵ�·��ģ�͵�TopoDS_HShape��ָ��.
    *  \param twistAngle Ťת�Ƕ�.
    *  \param draftAngle ��ģ�Ƕ�.
    *  \param draftType ��ģ����.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool sweepAlongPath(void* profileEntry, void* pathEntry, const double& twistAngle,
        const double& draftAngle,
        const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool sweepAlongPathPipeshell(void* profileEntry, void* pathEntry, const double& twistAngle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ��ɨ�ԵĽӿڣ���sweepAlongPath��ͬ���ǲ���OCC��PipeShell�Ľӿ�ȥʵ��ɨ��.
    *  \param profileEntry ����ɨ�ӵĽ����TopoDS_HShape��ָ��.
    *  \param entry ����ɨ�ӵ�·��ģ�͵�TopoDS_HShape��ָ��.
    *  \param twistAngle Ťת�Ƕ�.
    *  \param draftAngle ��ģ�Ƕ�.
    *  \param draftType ��ģ����.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool sweepAlongPathPipeshell(void* profileEntry, void* pathEntry, const double& twistAngle,
        const double& draftAngle,
        const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity);

    //�Ӻ�
    /*! \fn virtual bool thicken(void* entry, const double& thickness, const LOGICAL& isDoubleSides, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ʵ��Ӻ�Ľӿ�.
    *  \param entry ���мӺ�ģ�͵�TopoDS_HShape��ָ��.
    *  \param thickness ���.
    *  \param isDoubleSides �Ƿ����˫��Ӻ�.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ��Ӻ�.
    */
    virtual bool thicken(void* entry, const double& thickness, const LOGICAL& isDoubleSides,
        const char* name, std::string& strPID, void*& resEntity);

    //blend
    //��Բ��
    /*! \fn virtual bool fillet(void* entry, const std::list<std::string>& edgeorvertexlist, const double& radius, const double& setback, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ģ�͵�Բ�ǽӿ�.
    *  \param entry ���е���ģ�͵�TopoDS_HShape��ָ��.
    *  \param edgeorvertexlist ��ά���ǵ����������л��߶�ά�����еĵ���������.
    *  \param radius ���ǰ뾶.
    *  \param setback �ظ����Ǵ��˺󵹽ǵİ뾶.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool fillet(void* entry, const std::list<std::string>& edgeorvertexlist,
        const double& radius, const double& setback,
        const char* name, std::string& strPID, void*& resEntity);

	//��б��
	/*! \fn virtual bool chamfer(void* entry, const std::list<std::string>& edgeorvertexlist, const double& setback, const double& lDistance, const double& rDistance, const CHAMFERTYPE& chamferType, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ģ�͵�б�ǵĽӿ�.
	*  \param entry ���е���ģ�͵�TopoDS_HShape��ָ��.
	*  \param edgeorvertexlist ��ά���ǵ����������л��߶�ά�����еĵ���������.
	*  \param setback �ظ����Ǵ��˺󵹽ǵİ뾶.
	*  \param lDistance ����������.
	*  \param rDistance �����Ҳ����.
	*  \param chamferType �����Ƿ�ԳƵ���.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool chamfer(void* entry, const std::list<std::string>& edgeorvertexlist,
		const double& setback, const double& lDistance, const double& rDistance, const CHAMFERTYPE& chamferType,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool fillet(const std::list<void*>& edgesOrVertices, const double& radius, const double& setback, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ģ�͵�Բ�ǽӿ�.
	*  \param edgeorvertexlist ��ά���ǵ����������л��߶�ά�����еĵ���������.
	*  \param radius ���ǰ뾶.
	*  \param setback �ظ����Ǵ��˺󵹽ǵİ뾶.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool fillet(const std::list<void*>& edgesOrVertices,
		const double& radius, const double& setback,
		const char* name, std::string& strPID, void*& resEntity);

	//��б��
	/*! \fn virtual bool chamfer(const std::list<std::string>& edgeorvertexlist, const double& setback, const double& lDistance, const double& rDistance, const CHAMFERTYPE& chamferType, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ģ�͵�б�ǵĽӿ�.
	*  \param edgeorvertexlist ��ά���ǵ����������л��߶�ά�����еĵ���������.
	*  \param setback �ظ����Ǵ��˺󵹽ǵİ뾶.
	*  \param lDistance ����������.
	*  \param rDistance �����Ҳ����.
	*  \param chamferType �����Ƿ�ԳƵ���.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool chamfer(const std::list<void*>& edgesOrVertices,
		const double& setback, const double& lDistance, const double& rDistance,
		const CHAMFERTYPE& chamferType,
		const char* name, std::string& strPID, void*& resEntity);

	//surface
	/*! \fn virtual bool moveFaces(double* wcsMatrix, const std::list<void*>& entityList, const double& vectorX, const double& vectorY, const double& vectorZ, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ��ʵ���еĲ���������ƶ�ʵ���޸�ʵ���Ч��.
	*  \param wcsMatrix ģ�ͽ��в����ľֲ�����ϵ����.
	*  \param entityList ��Ҫ�����ƶ���������ָ��.
	*  \param vectorX ƽ��������x����.
	*  \param vectorY ƽ��������y����.
	*  \param vectorZ ƽ��������z����.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool moveFaces(double* wcsMatrix, const std::list<void*>& entityList,
		const double& vectorX, const double& vectorY, const double& vectorZ,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool offsetFaces(const std::list<void*>& entityList, const double& distance, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ��ʵ���еĲ��������ƫ��ʵ���޸�ʵ���Ч��.
	*  \param wcsMatrix ģ�ͽ��в����ľֲ�����ϵ����.
	*  \param entityList ��Ҫ����ƫ�Ƶ�������ָ��.
	*  \param distance ƫ�ƾ���.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool offsetFaces(const std::list<void*>& entityList,
		const double& distance, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn  virtual bool copyFaces(void* entity, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ���Ƽ�����Ľӿ�.
	*  \param entity ��Ҫ���Ƶ�ģ�͵�TopoDS_HShape��ָ��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool copyFaces(void* entity,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool sliceByFaces(void* entry, const std::list<void*>& toolFaceList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ߡ��桢ʵ���и�Ľӿڣ�����Ϊ���������ݣ���������ģ��.
	*  \param entry �����и�ģ�͵�TopoDS_HShape��ָ��.
	*  \param toolFaceList �����и�Ĺ�����.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ��任ģ��.
	*/
	virtual bool sliceByFaces(void* entry, const std::list<void*>& toolFaceList, const char* name, std::string& strPID,
		void*& resEntity);

	/*! \fn virtual bool detachEdges(void* entry, const std::list<void*>& edgeList, LOGICAL isCreated, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �����߿��е��߷ָ��߿�Ľӿ�.
	*  \param entry ���зָ���߿�ģ�͵�TopoDS_HShape��ָ��.
	*  \param edgeList �ָ�Ĺ���������.
	*  \param isCreated ���ݹ�������������ģ�ͻ��Ƿָ��߿�����ģ��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool detachEdges(void* entry, const std::list<void*>& edgeList, LOGICAL isCreated, const char* name,
		std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createPoint(double* wcsMatrix, const double& dCoordX, const double& dCoordY, const double& dCoordZ, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �������괴����.
	*  \param wcsMatrix ģ�ͽ��д����ľֲ�����ϵ����.
	*  \param dCoordX ��x����.
	*  \param dCoordY ��y����.
	*  \param dCoordZ ��z����.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool createPoint(double* wcsMatrix, const double& dCoordX, const double& dCoordY, const double& dCoordZ,
		const char* name, std::string& strPID, void*& resEntity);

    //���ƶ�
    /*! \fn virtual bool moveFaces(void* entry, double* wcsMatrix, const std::list<std::string>& edgeNames, const double& vectorX, const double& vectorY, const double& vectorZ, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ���ƶ�ʵ��ʵ����εĽӿڣ�δʵ��.
    */
    virtual bool moveFaces(void* entry, double* wcsMatrix, const std::list<std::string>& edgeNames,
        const double& vectorX, const double& vectorY, const double& vectorZ,
        const char* name, std::string& strPID, void*& resEntity);

    //��ƽ��
    /*! \fn virtual bool offsetFaces(void* entry, const std::list<std::string>& edgeNames, const double& distance, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ��ƽ��ʵ��ʵ����εĽӿڣ�δʵ��.
    */
    virtual bool offsetFaces(void* entry, const std::list<std::string>& edgeNames,
        const double& distance, const char* name, std::string& strPID, void*& resEntity);

    virtual bool offsetWire(void* entry, const double& distance,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool copyFaces(void* entry, const std::string& faceName, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ������Ľӿ�
	*  \param entity ��Ҫ���Ƶ�ģ�͵�TopoDS_HShape��ָ��.
	*  \param faceName ��Ҫ���и��Ƶ��������.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
    */
    virtual bool copyFaces(void* entry, const std::string& faceName,
        const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createEntityPIDForEntity(void* curBdy, std::string& newTag)
    *  \brief ��ȡģ��PID�Ľӿ�.
    *  \param curBdy ��Ҫ��ȡPID��ģ�͵�TopoDS_HShape��ָ��.
    *  \param newTag ���ػ�ȡ����PID.
    *  \return �Ƿ���ҵ���ģ��.
    */
    virtual bool createEntityPIDForEntity(void* curBdy, std::string& newTag);

    //���Ƽ�����
    /***********************************�ӿ���ʵ��****************************************/
    /*! \fn  virtual bool createCloneEntity(void* curBdy, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ���Ƽ�����Ľӿ�.
    *  \param curBdy ��Ҫ���Ƶ�ģ�͵�TopoDS_HShape��ָ��.
    *  \param name ����.
    *  \param strPID ���ش���ģ�͵�PID.
    *  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
    *  \return �Ƿ�ɹ�����.
    */
    virtual bool createCloneEntity(void* curBdy, const char* name,
        std::string& strPID, void*& resEntity);

    //�ߡ��桢ʵ���и�
    /*! \fn virtual bool split(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const KEEPSIDE& keepSide, const char* name, std::string& strPID, void*& resEntity
    *  \brief �ߡ��桢ʵ���и�Ľӿ�.
	*  \param entry �����и�ģ�͵�TopoDS_HShape��ָ��.
	*  \param wcsMatrix ģ�ͽ����и�ľֲ�����ϵ����.
	*  \param basePosX �и�ƽ�淨������x����.
	*  \param basePosY �и�ƽ�淨������y����.
	*  \param basePosZ �и�ƽ�淨������z����.
	*  \param normalVecX �и�ƽ�淨��������x����.
	*  \param normalVecY �и�ƽ�淨��������y����.
	*  \param normalVecZ �и�ƽ�淨��������z����.
	*  \param keepSide �Ƿ���ģ���ڲ����и�ƽ��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ��任ģ��.
    */
    virtual bool split(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ,
        const double& normalVecX, const double& normalVecY, const double& normalVecZ, const KEEPSIDE& keepSide,
        const char* name, std::string& strPID, void*& resEntity);

    //����
    /*! \fn virtual bool stitch(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
    *  \brief ���ϵĽӿڣ�����ܹ�����ʵ��������ʵ��.
	*  \param entryList ���н��з�ϵ���ģ��TopoDS_HShape��ָ������.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����з��.
    */
    virtual bool stitch(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool connect(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
    *  \brief �����Ľӿ�.
	*  \param entryList ���н��з������߿���ߵ�ģ��TopoDS_HShape��ָ������.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����з���.
    */
	virtual bool connect(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool cover(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �߿�cover�Ľӿ�.
	*  \param entryList ���н���cover���߿�TopoDS_HShape��ָ������.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����cover.
	*/
    virtual bool cover(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);


	/************************************************************************/
	/*                            added by YAO                              */
	/************************************************************************/

	/*! \fn virtual bool trimCurve(void* edgeEntry, const double& startT, const double& endT, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ���߲ü��Ľӿ�.
	*  \param edgeEntry ��Ҫ���вü����ߵ�TopoDS_HShape��ָ��.
	*  \param startT �ü�����ʼUֵ.
	*  \param endT �ü�����ֹUֵ.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����вü�.
	*/
	virtual bool trimCurve(void* edgeEntry, const double& startT, const double& endT, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool edgeExtendLength(void* edgeEntry, double startParam, double length, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �����ӳ��Ľӿ�.
	*  \param edgeEntry ��Ҫ�����ӳ����ߵ�TopoDS_HShape��ָ��.
	*  \param startParam ��ʼ�ӳ�λ�õ�Uֵ.
	*  \param length �ӳ��ĳ���.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ������ӳ�.
	*/
	virtual bool edgeExtendLength(void* edgeEntry, double startParam, double length, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool closedEdgeFace(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ������߷�ճ��߿�Ľӿ�.
	*  \param entryList ��Ҫ���з�յ��ߵ�TopoDS_HShape��ָ������.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����ģ��.
	*/
	virtual bool closedEdgeFace(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool wireOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �߿�ƫ�ƵĽӿ�.
	*  \param entry ��Ҫ����ƫ�Ƶ��߿��TopoDS_HShape��ָ��.
	*  \param distance ƫ�ƾ���.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����ģ��.
	*/
	virtual bool wireOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool faceOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ��ƫ�ƵĽӿ�.
	*  \param entry ��Ҫ����ƫ�Ƶ����TopoDS_HShape��ָ��.
	*  \param distance ƫ�ƾ���.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����ģ��.
	*/
	virtual bool faceOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool faceExtendLength(void* edgeEntry, double length, const bool& inU, const bool& forward, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �����ӳ��Ľӿ�.
	*  \param edgeEntry ��Ҫ�����ӳ������TopoDS_HShape��ָ��.
	*  \param length �ӳ��ĳ���.
	*  \param inU �Ƿ���U�������ΪV��������ӳ�.
	*  \param forward �Ƿ�Ϊ��ǰ����.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ������ӳ�.
	*/
	virtual bool faceExtendLength(void* edgeEntry, double length, const bool& inU, const bool& forward, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool faceIntersection(void* entry, void* entry2, const int& method, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �����ཻ�õ����ߵĽӿ�.
	*  \param entry ��Ҫ�����ཻ����1��TopoDS_HShape��ָ��.
	*  \param entry2 ��Ҫ�����ཻ����2��TopoDS_HShape��ָ��.
	*  \param method �ཻ�㷨.1ΪBRepAlgoAPI_Section, 2ΪBRepAlgoAPI_IntCS, 3ΪBRepAlgoAPI_IntSS.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����д���.
	*/
	virtual bool faceIntersection(void* entry, void* entry2, const int& method, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool edgeCombination(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ���������Ľӿ�.
	*  \param entry ��Ҫ������������1��TopoDS_HShape��ָ��.
	*  \param entry2 ��Ҫ������������2��TopoDS_HShape��ָ��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����д���.
	*/
	virtual bool edgeCombination(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool edgeIntersection(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �����ཻ�õ�����Ľӿ�.
	*  \param entry ��Ҫ�����ཻ����1��TopoDS_HShape��ָ��.
	*  \param entry2 ��Ҫ�����ཻ����2��TopoDS_HShape��ָ��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����д���.
	*/
	virtual bool edgeIntersection(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool makeFuzzyBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const double& fuzzyValue, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ģ����������Ľӿڣ���������������.
	*  \param boolType ������������ͣ�Ϊ����������.
	*  \param isKeep �Ƿ���ԭ��ģ��.
	*  \param entryList ���н��в������㴦���ģ��TopoDS_HShape��ָ������.
	*  \param fuzzyValue ģ���������㾫��.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����в�������.
	*/
	virtual bool makeFuzzyBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const double& fuzzyValue,
		const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool makeNonRegBoolean(const EnAdapatType& boolType, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �����򲼶�����Ľӿڣ���������������.
	*  \param boolType ������������ͣ�Ϊ����������.
	*  \param isKeep �Ƿ���ԭ��ģ��.
	*  \param entryList ���н��в������㴦���ģ��TopoDS_HShape��ָ������.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ����в�������.
	*/
	virtual bool makeNonRegBoolean(const EnAdapatType& boolType, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool createNurbsFace(const std::vector<std::vector<std::vector<double>>>& poles, const std::vector<std::vector<double>>& weight, const std::vector<double> &uValue, const std::vector<double>& vValue, const std::vector<int> &uMul, const std::vector<int>& vMul, const int& uDeg, const int& vDeg, const char* name, std::string& strPID, void*& resEntity)
	*  \brief ����Nurbs��Ľӿ�.
	*  \param poles ���Ƶ��ά����.
	*  \param weights Ȩ������, ���ȫΪ1�򹹽�������������.
	*  \param uValue U����ڵ�.
	*  \param vValue V����ڵ�.
	*  \param uMul U�����ظ��ڵ�.
	*  \param vMul V�����ظ��ڵ�.
	*  \param uDeg U�������ɶ�.
	*  \param vDeg V�������ɶ�.
	*  \param name ����.
	*  \param strPID ���ش���ģ�͵�PID.
	*  \param resEntity ���ش���ģ�͵�TopoDS_HShape��ָ��.
	*  \return �Ƿ�ɹ�����.
	*/
	virtual bool createNurbsFace(const std::vector<std::vector<std::vector<double>>>& poles, const std::vector<std::vector<double>>& weight,
		const std::vector<double> &uValue, const std::vector<double>& vValue,
		const std::vector<int> &uMul, const std::vector<int>& vMul,
		const int& uDeg, const int& vDeg, const char* name, std::string& strPID, void*& resEntity);

	/*! \fn virtual bool shapeFix(void* entry, const double& precision, const double& minTol, const double& maxTol, const EnFixType& fixType,const char* name, std::string& strPID, void*& resEntity, std::string& message)
	*  \brief ģ���޸��Ľӿ�.
	*  \param entry ��Ҫ�����޸���ģ�͵�TopoDS_HShape��ָ��.
	*  \param precision ��Ҫ�����޸��ľ���.
	*  \param minTol ��Ҫ�����޸�����С���.
	*  \param maxTol ��Ҫ�����޸���������.
	*  \param fixType �����޸�������.
	*  \param name ����.
	*  \param strPID �����޸�ģ�͵�PID.
	*  \param resEntity �����޸�ģ�͵�TopoDS_HShape��ָ��.
	*  \param message �����޸�ǰ��ļ����Ϣ.
	*  \return �Ƿ�ɹ������޸�.
	*/
	virtual bool shapeFix(void* entry, const double& precision, const double& minTol, const double& maxTol, const EnFixType& fixType,const char* name, std::string& strPID, void*& resEntity, std::string& message);

protected:
    /*! \fn virtual bool createBooleanCut(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ӿ�������.
	*/
    virtual bool createBooleanCut(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createBooleanCommon(const std::string& mainEntry, const std::string& toolEntry, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ӿ�������.
	*/
    virtual bool createBooleanCommon(const std::string& mainEntry, const std::string& toolEntry, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createBooleanFuse(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ӿ�������.
	*/
    virtual bool createBooleanFuse(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity);

    /*! \fn virtual bool createBooleanNonReg(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity)
	*  \brief �ӿ�������.
	*/
    virtual bool createBooleanNonReg(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& resEntity);

    virtual bool createIsoTrapezoid(double* wcsMatrix, double topwidth, double width, double height,
        LOGICAL iscovered, void*& ent);
    //std::vector<TDF_Label> m_labels;
    virtual bool splitWireBypoint(void* entry, std::vector<double> insertTs, const char* name, std::string& strPID, void*& entity);

    virtual bool splitFaceByEdge(void* entry, SplitLineCreationInfos infos, const char* name,
        std::string& strPID, void*& entity);

    virtual bool splitBodyByFace(void* entry, SplitFaceCreationInfos infos, const char* name,
        std::string& strPID, void*& entity);

    virtual bool insertHardPoints(void* entry, void* edge, std::vector<cmtPoint> points, const char* name,
        std::string& strPID, void*& entity);

   public:
    virtual bool newstitch(string facestostitchpids);
};


#endif
