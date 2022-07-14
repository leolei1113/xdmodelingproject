/*! \file BRepFilletAPI_MakeFillet2d_Extension.hxx
*   \brief BRepFilletAPI_MakeFillet2d_Extension���ͷ�ļ�
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
*   \brief BRepFilletAPI_MakeFillet2d_Extension���װ�˶�άƽ��Ե������ǵ��㷨��
 */
class BRepFilletAPI_MakeFillet2d_Extension : public BRepBuilderAPI_MakeShape
{
public:

    DEFINE_STANDARD_ALLOC

    /*! \fn BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Face& F)
    *  \brief BRepFilletAPI_MakeFillet2d_Extension�Ĺ��캯��.
    *  \param Shape ��Ҫ�����������߿�����.
    */
//     Standard_EXPORT BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Face& F);
	Standard_EXPORT BRepFilletAPI_MakeFillet2d_Extension(const TopoDS_Shape& Shape);

    /*! \fn TopoDS_Edge AddFillet(const TopoDS_Vertex& V, const Standard_Real Radius)
    *  \brief �㵹Բ�ǵĽӿ�.
    *  \param V ���е��ǵĵ�.
    *  \param Radius ���ǰ뾶.
    *  \return ������ɵ��ǵı�.
    */
    Standard_EXPORT TopoDS_Edge AddFillet(const TopoDS_Vertex& V, const Standard_Real Radius);

    /*! \fn TopoDS_Edge AddChamfer(const TopoDS_Vertex& V, const Standard_Real D1, const Standard_Real D2)
    *  \brief �㵹б�ǵĽӿ�.
    *  \param V ���е��ǵĵ�.
    *  \param D1 �������.
    *  \param D2 �����Ҳ�.
    *  \return ������ɵ��ǵı�.
    */
    Standard_EXPORT TopoDS_Edge AddChamfer(const TopoDS_Vertex& V, const Standard_Real D1, const Standard_Real D2);

    /*! \fn void BuildNewWire(const TopoDS_Edge& OldE1, const TopoDS_Edge& OldE2, const TopoDS_Edge& E1, const TopoDS_Edge& Fillet, const TopoDS_Edge& E2)
    *  \brief ����ԭ�е��߿���µĵ��ǽ���ع��߿�.
    *  \param OldE1 ����ǰ����1.
    *  \param OldE2 ����ǰ����2.
    *  \param E1 ���Ǻ����1.
    *  \param Fillet ������.
    *  \param E2 ����ǰ����2.
    */
    void BuildNewWire(const TopoDS_Edge& OldE1, const TopoDS_Edge& OldE2, const TopoDS_Edge& E1, const TopoDS_Edge& Fillet, const TopoDS_Edge& E2);

    /*! \fn const TopoDS_Edge& BasisEdge(const TopoDS_Edge& E) const
    *  \brief ���ݵ�������߻�ȡ����ǰ����.
    *  \param E ���Ǻ����.
    *  \return ����ǰ����.
    */
    const TopoDS_Edge& BasisEdge(const TopoDS_Edge& E) const;

    /*! \fn void UpDateHistory(const TopoDS_Edge& E1, const TopoDS_Edge& E2, const TopoDS_Edge& TrimE1, const TopoDS_Edge& TrimE2, const TopoDS_Edge& NewEdge, const Standard_Integer Id)
    *  \brief ����ԭ�е��߿���µĵ��ǽ����¼����ʷ�У�����׷��.
    *  \param E1 ����ǰ����1.
    *  \param E2 ����ǰ����2.
    *  \param TrimE1 ���Ǻ����1.
    *  \param TrimE2 ����ǰ����2.
    *  \param NewEdge ������.
    *  \param Id ÿ�ε��ǵ�index.
    */
    void UpDateHistory(const TopoDS_Edge& E1, const TopoDS_Edge& E2, const TopoDS_Edge& TrimE1, const TopoDS_Edge& TrimE2, const TopoDS_Edge& NewEdge, const Standard_Integer Id);

    /*! \fn virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S)
    *  \brief ��д����ӿڣ��������˻�ȡ��ر༭��������.
    *  \param S ��Ҫ���в��ҵ�����.
    *  \return ��ر༭������������.
    */
    Standard_EXPORT virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S) Standard_OVERRIDE;

    /*! \fn ChFi2d_ConstructionError Status() const
    *  \brief ��ȡ���Ǵ���ĵ�ǰ״̬.
    *  \return ���Ǵ���ĵ�ǰ״̬.
    */
    ChFi2d_ConstructionError Status() const;

    /*! \fn virtual void Build()
    *  \brief ��ʼ������״̬����ʱ�Ѿ���ɵ���.
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
