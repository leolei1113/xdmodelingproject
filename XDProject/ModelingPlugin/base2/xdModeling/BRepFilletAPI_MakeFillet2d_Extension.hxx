/*! \file BRepFilletAPI_MakeFillet2d_Extension.hxx
*   \brief BRepFilletAPI_MakeFillet2d_Extension类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef _BRepFilletAPI_MakeFillet2d_Extension_HeaderFile
#define _BRepFilletAPI_MakeFillet2d_Extension_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <ChFi2d_Builder.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <TopTools_SequenceOfShape.hxx>
#include <Standard_Integer.hxx>
#include <TopTools_ListOfShape.hxx>
#include <ChFi2d_ConstructionError.hxx>
#include <ChFi2d_FilletAPI.hxx>
#include <ChFi2d_ChamferAPI.hxx>

class TopoDS_Face;
class TopoDS_Edge;
class TopoDS_Vertex;
class TopoDS_Shape;

// resolve name collisions with X11 headers
#ifdef Status
#undef Status
#endif

/*! \class BRepFilletAPI_MakeFillet2d_Extension
*   \brief BRepFilletAPI_MakeFillet2d_Extension类封装了二维平面对点做倒角的算法。
 */
class BRepFilletAPI_MakeFillet2d_Extension : public BRepBuilderAPI_MakeShape
{
public:

    DEFINE_STANDARD_ALLOC

    /*! \fn BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Face& F)
    *  \brief BRepFilletAPI_MakeFillet2d_Extension的构造函数.
    *  \param Shape 需要输入的面或者线框拓扑.
    */
//     Standard_EXPORT BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Face& F);
	Standard_EXPORT BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Shape& Shape);

    /*! \fn TopoDS_Edge AddFillet(const TopoDS_Vertex& V, const Standard_Real Radius)
    *  \brief 点倒圆角的接口.
    *  \param V 进行倒角的点.
    *  \param Radius 倒角半径.
    *  \return 返回完成倒角的边.
    */
    Standard_EXPORT TopoDS_Edge AddFillet(const TopoDS_Vertex& V, const Standard_Real Radius);

    /*! \fn TopoDS_Edge AddChamfer(const TopoDS_Vertex& V, const Standard_Real D1, const Standard_Real D2)
    *  \brief 点倒斜角的接口.
    *  \param V 进行倒角的点.
    *  \param D1 倒角左侧.
    *  \param D2 倒角右侧.
    *  \return 返回完成倒角的边.
    */
    Standard_EXPORT TopoDS_Edge AddChamfer(const TopoDS_Vertex& V, const Standard_Real D1, const Standard_Real D2);

    /*! \fn void BuildNewWire(const TopoDS_Edge& OldE1, const TopoDS_Edge& OldE2, const TopoDS_Edge& E1, const TopoDS_Edge& Fillet, const TopoDS_Edge& E2)
    *  \brief 根据原有的线框和新的倒角结果重构线框.
    *  \param OldE1 倒角前的线1.
    *  \param OldE2 倒角前的线2.
    *  \param E1 倒角后的线1.
    *  \param Fillet 倒角线.
    *  \param E2 倒角前的线2.
    */
    void BuildNewWire(const TopoDS_Edge& OldE1, const TopoDS_Edge& OldE2, const TopoDS_Edge& E1, const TopoDS_Edge& Fillet, const TopoDS_Edge& E2);

    /*! \fn const TopoDS_Edge& BasisEdge(const TopoDS_Edge& E) const
    *  \brief 根据倒角完的线获取倒角前的线.
    *  \param E 倒角后的线.
    *  \return 倒角前的线.
    */
    const TopoDS_Edge& BasisEdge(const TopoDS_Edge& E) const;

    /*! \fn void UpDateHistory(const TopoDS_Edge& E1, const TopoDS_Edge& E2, const TopoDS_Edge& TrimE1, const TopoDS_Edge& TrimE2, const TopoDS_Edge& NewEdge, const Standard_Integer Id)
    *  \brief 根据原有的线框和新的倒角结果记录在历史中，方便追踪.
    *  \param E1 倒角前的线1.
    *  \param E2 倒角前的线2.
    *  \param TrimE1 倒角后的线1.
    *  \param TrimE2 倒角前的线2.
    *  \param NewEdge 倒角线.
    *  \param Id 每次倒角的index.
    */
    void UpDateHistory(const TopoDS_Edge& E1, const TopoDS_Edge& E2, const TopoDS_Edge& TrimE1, const TopoDS_Edge& TrimE2, const TopoDS_Edge& NewEdge, const Standard_Integer Id);

    /*! \fn virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S)
    *  \brief 重写父类接口，根据拓扑获取相关编辑过的拓扑.
    *  \param S 需要进行查找的拓扑.
    *  \return 相关编辑过的拓扑序列.
    */
    Standard_EXPORT virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S) Standard_OVERRIDE;

    /*! \fn ChFi2d_ConstructionError Status() const
    *  \brief 获取倒角处理的当前状态.
    *  \return 倒角处理的当前状态.
    */
    ChFi2d_ConstructionError Status() const;

    /*! \fn virtual void Build()
    *  \brief 开始处理倒角状态，此时已经完成倒角.
    */
    Standard_EXPORT virtual void Build() Standard_OVERRIDE;

private:
    gp_Pln plane;

	ChFi2d_FilletAlgo myFilletBuilder;
	ChFi2d_ChamferAPI myChamferBuilder;



    ChFi2d_ConstructionError status;
//     TopoDS_Face refFace;
//     TopoDS_Face newFace;
	TopoDS_Shape refShape;
	TopoDS_Shape newShape;
	Standard_Boolean faceMode = true;

    TopTools_SequenceOfShape fillets;
    TopTools_SequenceOfShape chamfers;
    TopTools_DataMapOfShapeShape history;
};



#endif // _BRepFilletAPI_MakeFillet2d_Extension_HeaderFile
