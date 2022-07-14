/*! \file TFOCCFixTool.h
*   \brief TFOCCFixTool类的头文件
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
*   \brief 提供各类修复工具，单例模式。
 */
class XDMODELING_API OCCFixTool
{
public:

    /*! \fn TFOCCFixTool()
    *  \brief TFOCCFixTool的构造函数.
    */
    OCCFixTool();

    /*! \fn ~TFOCCFixTool()
    *  \brief TFOCCFixTool的析构函数.
    */
    ~OCCFixTool();

    /*! \fn static TFOCCFixTool* Instance()
    *  \brief 提供单例的TFOCCFixTool指针.
    */
	static OCCFixTool* Instance();

	void shapeValidityAnalysis(const TopoDS_Shape& aShape, const TopAbs_ShapeEnum& curType, std::string& message);
    /*! \fn bool fixTopoValidityOp(TopoDS_Shape& aShape, const Standard_Boolean& wireframeRepair = Standard_False)
    *  \brief 几何拓扑TopoDS_Shape的整体修复.
    *  \param aShape 需要进行修复的拓扑.
    *  \param wireframeRepair 是否进行wireframe框架修复.false时默认修复solid, shell, face, wire等, true时修复曲线间缝隙和合并过小曲线.
    *  \return 是否成功进行修复.
    */
	bool fixTopoValidityOp(TopoDS_Shape& aShape, const Standard_Boolean& wireframeRepair = Standard_False);

	/*! \fn void FixShellOrientationOp(TopoDS_Shape& aShape)
	*  \brief 修复模型中所有shell的方向.
	*  \param aShape 需要进行修复的拓扑.
	*/
	void FixShellOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixFaceOrientationOp(TopoDS_Shape& aShape)
	*  \brief 修复模型中所有面的方向.
	*  \param aShape 需要进行修复的拓扑.
	*/
	void FixFaceOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixEdgeOrientationOp(TopoDS_Shape& aShape)
	*  \brief 修复模型中所有线的方向.
	*  \param aShape 需要进行修复的拓扑.
	*/
	void FixEdgeOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixWireOrientationOp(TopoDS_Shape& aShape)
	*  \brief 修复模型中所有wire的方向.
	*  \param aShape 需要进行修复的拓扑.
	*/
	void FixWireOrientationOp(TopoDS_Shape& aShape);

	/*! \fn void FixEdgeConnection(TopoDS_Shape& aShape)
	*  \brief 修复模型线连接的问题.
	*  \param aShape 需要进行修复的拓扑.
	*/
	void FixEdgeConnection(TopoDS_Shape& aShape);

	/*! \fn void FixFaceConnection(TopoDS_Shape& aShape)
	*  \brief 修复模型面连接的问题.
	*  \param aShape 需要进行修复的拓扑.
	*/
	void FixFaceConnection(TopoDS_Shape& aShape);

	/*! \fn void FixWire(TopoDS_Shape& aShape)
	*  \brief 修复线框的自交情况.
	*  \param aShape 需要进行修复的拓扑.
	*/
	void FixWire(TopoDS_Shape& aShape);

	/*! \fn double getPrecision() const
	*  \brief 获取几何修复的精度.
	*  \return 返回成员几何修复的精度.
	*/
	double getPrecision() const { return m_precision; }

	/*! \fn void setPrecision(double val)
	*  \brief 赋值几何修复的精度.
	*  \param val 进行几何修复的精度.
	*/
    void setPrecision(double val) { m_precision = val; }

	/*! \fn double getMinTol() const
	*  \brief 获取几何修复允许的最小误差.
	*  \return 返回几何修复允许的最小误差.
	*/
	double getMinTol() const { return m_minTol; }

	/*! \fn void setMinTol(double val)
	*  \brief 赋值几何修复允许的最小误差.
	*  \param val 进行几何修复允许的最小误差.
	*/
	void setMinTol(double val) { m_minTol = val; }

	/*! \fn double getMaxTol() const
	*  \brief 获取几何修复允许的最大误差.
	*  \return 返回几何修复允许的最大误差.
	*/
    double getMaxTol() const { return m_maxTol; }

	/*! \fn void setMaxTol(double val)
	*  \brief 赋值几何修复允许的最大误差.
	*  \param val 进行几何修复允许的最大误差.
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
	*  \brief 在进行修复之前对修复工具设定精度、误差等.
	*  \param theFix 修复工具的抽象类.
	*/
	void PreCommit(const Handle(ShapeFix_Root)& theFix);

	/*! \fn void PostCommit(TopoDS_Shape& myShape, const Handle(ShapeFix_Root)& theFix)
	*  \brief 完成修复后，记录模型修复和重建过程.
	*  \param myShape 进行修复的模型.
	*  \param theFix 修复工具的抽象类.
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
