/*! \file BRepOffsetAPI_MakeThickSolid_Extension.hxx
*   \brief BRepOffsetAPI_MakeThickSolid_Extension���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef _BRepOffsetAPI_MakeThickSolid_Extension_HeaderFile
#define _BRepOffsetAPI_MakeThickSolid_Extension_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <BRepOffsetAPI_MakeOffsetShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <Standard_Real.hxx>
#include <BRepOffset_Mode.hxx>
#include <Standard_Boolean.hxx>
#include <GeomAbs_JoinType.hxx>
class TopoDS_Shape;

/*! \class BRepOffsetAPI_MakeThickSolid_Extension
*   \brief BRepOffsetAPI_MakeThickSolid_Extension���װ��ʵ��Ӻ���㷨��
 */
class BRepOffsetAPI_MakeThickSolid_Extension : public BRepOffsetAPI_MakeOffsetShape
{
public:

    DEFINE_STANDARD_ALLOC

    /*! \fn BRepOffsetAPI_MakeThickSolid_Extension()
    *  \brief BRepOffsetAPI_MakeThickSolid_Extension�Ĺ��캯��.
    */
    Standard_EXPORT BRepOffsetAPI_MakeThickSolid_Extension();

    /*! \fn void Thickening(const TopoDS_Shape& S, const Standard_Real Offset, const Standard_Real Tol, const BRepOffset_Mode Mode = BRepOffset_Skin, const Standard_Boolean Intersection = Standard_False, const Standard_Boolean SelfInter = Standard_False, const GeomAbs_JoinType Join = GeomAbs_Arc, const Standard_Boolean Thickening = Standard_True, const Standard_Boolean RemoveIntEdges = Standard_False)
    *  \brief ���мӺ�Ľӿ�.
    *  \param S ��Ҫ���мӺ��������.
    *  \param Offset �Ӻ�ľ���.
    *  \param Tol �Ӻ�ľ���.
    *  \param Mode �Ӻ��ģʽ.
    *  \param Intersection �Ƿ����粿��.
    *  \param SelfInter �Ƿ����ڲ��ཻ���� .
    *  \param Join ���Ӵ��Ĵ���ʽ.
    *  \param Thickening �Ƿ�����ʵ��.
    *  \param RemoveIntEdges �Ƿ�ȥ���ڲ���.
    */
    Standard_EXPORT void Thickening(const TopoDS_Shape& S,
        const Standard_Real Offset,
        const Standard_Real Tol,
        const BRepOffset_Mode Mode = BRepOffset_Skin,
        const Standard_Boolean Intersection = Standard_False,
        const Standard_Boolean SelfInter = Standard_False,
        const GeomAbs_JoinType Join = GeomAbs_Arc,
        const Standard_Boolean Thickening = Standard_True,
        const Standard_Boolean RemoveIntEdges = Standard_False);

    /*! \fn virtual void Build()
    *  \brief ʵ�ָ���ӿڣ�ʲô������.
    */
    Standard_EXPORT virtual void Build() Standard_OVERRIDE;

    /*! \fn virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S)
    *  \brief ��д����ӿڣ��������˻�ȡ��ر༭��������.
    *  \param S ��Ҫ���в��ҵ�����.
    *  \return ��ر༭������������.
    */
    Standard_EXPORT virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S) Standard_OVERRIDE;
};

#endif // _BRepOffsetAPI_MakeThickSolid_Extension_HeaderFile
