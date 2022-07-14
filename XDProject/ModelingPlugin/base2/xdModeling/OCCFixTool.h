/*! \file TFOCCFixTool.h
*   \brief TFOCCFixTool���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef _OCCFixTool_H_
#define _OCCFixTool_H_


#include "xdModelingHeader.h"

#include <string>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shell.hxx>
#include <ShapeProcess_ShapeContext.hxx>
#include <ShapeFix_Root.hxx>


/*! \class TFOCCFixTool
*   \brief �ṩ�����޸����ߣ�����ģʽ��
 */
class XDMODELING_API OCCFixTool
{
public:

    /*! \fn TFOCCFixTool()
    *  \brief TFOCCFixTool�Ĺ��캯��.
    */
    OCCFixTool();

    /*! \fn ~TFOCCFixTool()
    *  \brief TFOCCFixTool����������.
    */
    ~OCCFixTool();

    /*! \fn static TFOCCFixTool* Instance()
    *  \brief �ṩ������TFOCCFixToolָ��.
    */
	static OCCFixTool* Instance();

	void shapeValidityAnalysis(const TopoDS_Shape& aShape, const TopAbs_ShapeEnum& curType, std::string& message);
    /*! \fn bool fixTopoValidityOp(TopoDS_Shape& aShape, const Standard_Boolean& wireframeRepair = Standard_False)
    *  \brief ��������TopoDS_Shape�������޸�.
    *  \param aShape ��Ҫ�����޸�������.
    *  \param wireframeRepair �Ƿ����wireframe����޸�.falseʱĬ���޸�solid, shell, face, wire��, trueʱ�޸����߼��϶�ͺϲ���С����.
    *  \return �Ƿ�ɹ������޸�.
    */
	bool fixTopoValidityOp(TopoDS_Shape& aShape, const Standard_Boolean& wireframeRepair = Standard_False);

	/*! \fn void FixShellOrientationOp(TopoDS_Shape& aShape)
	*  \brief �޸�ģ��������shell�ķ���.
	*  \param aShape ��Ҫ�����޸�������.
	*/
	void FixShellOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixFaceOrientationOp(TopoDS_Shape& aShape)
	*  \brief �޸�ģ����������ķ���.
	*  \param aShape ��Ҫ�����޸�������.
	*/
	void FixFaceOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixEdgeOrientationOp(TopoDS_Shape& aShape)
	*  \brief �޸�ģ���������ߵķ���.
	*  \param aShape ��Ҫ�����޸�������.
	*/
	void FixEdgeOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixWireOrientationOp(TopoDS_Shape& aShape)
	*  \brief �޸�ģ��������wire�ķ���.
	*  \param aShape ��Ҫ�����޸�������.
	*/
	void FixWireOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixEdgeConnection(TopoDS_Shape& aShape)
	*  \brief �޸�ģ�������ӵ�����.
	*  \param aShape ��Ҫ�����޸�������.
	*/
	void FixEdgeConnection(TopoDS_Shape& aShape);

	/*! \fn void FixFaceConnection(TopoDS_Shape& aShape)
	*  \brief �޸�ģ�������ӵ�����.
	*  \param aShape ��Ҫ�����޸�������.
	*/
	void FixFaceConnection(TopoDS_Shape& aShape);

	/*! \fn void FixWire(TopoDS_Shape& aShape)
	*  \brief �޸��߿���Խ����.
	*  \param aShape ��Ҫ�����޸�������.
	*/
	void FixWire(TopoDS_Shape& aShape);

	/*! \fn double getPrecision() const
	*  \brief ��ȡ�����޸��ľ���.
	*  \return ���س�Ա�����޸��ľ���.
	*/
	double getPrecision() const { return m_precision; }

	/*! \fn void setPrecision(double val)
	*  \brief ��ֵ�����޸��ľ���.
	*  \param val ���м����޸��ľ���.
	*/
    void setPrecision(double val) { m_precision = val; }

	/*! \fn double getMinTol() const
	*  \brief ��ȡ�����޸��������С���.
	*  \return ���ؼ����޸��������С���.
	*/
	double getMinTol() const { return m_minTol; }

	/*! \fn void setMinTol(double val)
	*  \brief ��ֵ�����޸��������С���.
	*  \param val ���м����޸��������С���.
	*/
	void setMinTol(double val) { m_minTol = val; }

	/*! \fn double getMaxTol() const
	*  \brief ��ȡ�����޸������������.
	*  \return ���ؼ����޸������������.
	*/
    double getMaxTol() const { return m_maxTol; }

	/*! \fn void setMaxTol(double val)
	*  \brief ��ֵ�����޸������������.
	*  \param val ���м����޸������������.
	*/
    void setMaxTol(double val) { m_maxTol = val; }
private:
	static OCCFixTool*				m_OCCFixTool;

	TopoDS_Wire							aWire;

	Handle(ShapeProcess_ShapeContext)	m_aShCtx;
	Handle(ShapeBuild_ReShape)			m_theReShape ;

    double                          m_precision;
    double                          m_minTol;
    double                          m_maxTol;

protected:
	/*! \fn void PreCommit(const Handle(ShapeFix_Root)& theFix)
	*  \brief �ڽ����޸�֮ǰ���޸������趨���ȡ�����.
	*  \param theFix �޸����ߵĳ�����.
	*/
	void PreCommit(const Handle(ShapeFix_Root)& theFix);

	/*! \fn void PostCommit(TopoDS_Shape& myShape, const Handle(ShapeFix_Root)& theFix)
	*  \brief ����޸��󣬼�¼ģ���޸����ؽ�����.
	*  \param myShape �����޸���ģ��.
	*  \param theFix �޸����ߵĳ�����.
	*/
	void PostCommit(TopoDS_Shape& myShape, const Handle(ShapeFix_Root)& theFix);

	void shellValidityAnalysis(const TopoDS_Shape& aShape, int& orientationError, bool& freeEdges, bool& connectedEdges);

	void faceValidityAnalysis(const TopoDS_Shape& aShape, int& spotFace, int& stripFace, int& twistedFace);

	void wireValidityAnalysis(const TopoDS_Shape& aShape, int& orientationError, int& disconnectedEdges, int& smallEdges, int& selfIntersection);

// 	void edgeValidityAnalysis(const TopoDS_Shape& aShape, bool& orientationError, bool& disconnectedEdges, bool& smallEdges, bool& selfIntersection);
};

#undef  OCCFixToolInstance
#define OCCFixToolInstance OCCFixTool::Instance()

#endif //_TFOCCFixTool_H_
