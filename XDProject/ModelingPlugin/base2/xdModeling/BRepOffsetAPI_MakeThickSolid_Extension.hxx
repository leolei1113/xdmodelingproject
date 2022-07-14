/*! \file BRepOffsetAPI_MakeThickSolid_Extension.hxx
*   \brief BRepOffsetAPI_MakeThickSolid_Extension类的头文件
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
*   \brief BRepOffsetAPI_MakeThickSolid_Extension类封装了实体加厚的算法。
 */
class BRepOffsetAPI_MakeThickSolid_Extension : public BRepOffsetAPI_MakeOffsetShape
{
public:

    DEFINE_STANDARD_ALLOC

    /*! \fn BRepOffsetAPI_MakeThickSolid_Extension()
    *  \brief BRepOffsetAPI_MakeThickSolid_Extension的构造函数.
    */
    Standard_EXPORT BRepOffsetAPI_MakeThickSolid_Extension();

    /*! \fn void Thickening(const TopoDS_Shape& S, const Standard_Real Offset, const Standard_Real Tol, const BRepOffset_Mode Mode = BRepOffset_Skin, const Standard_Boolean Intersection = Standard_False, const Standard_Boolean SelfInter = Standard_False, const GeomAbs_JoinType Join = GeomAbs_Arc, const Standard_Boolean Thickening = Standard_True, const Standard_Boolean RemoveIntEdges = Standard_False)
    *  \brief 进行加厚的接口.
    *  \param S 需要进行加厚处理的拓扑.
    *  \param Offset 加厚的距离.
    *  \param Tol 加厚的精度.
    *  \param Mode 加厚的模式.
    *  \param Intersection 是否处理交界部分.
    *  \param SelfInter 是否处理内部相交部分 .
    *  \param Join 连接处的处理方式.
    *  \param Thickening 是否生成实体.
    *  \param RemoveIntEdges 是否去除内部线.
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
    *  \brief 实现父类接口，什么都不做.
    */
    Standard_EXPORT virtual void Build() Standard_OVERRIDE;

    /*! \fn virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S)
    *  \brief 重写父类接口，根据拓扑获取相关编辑过的拓扑.
    *  \param S 需要进行查找的拓扑.
    *  \return 相关编辑过的拓扑序列.
    */
    Standard_EXPORT virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S) Standard_OVERRIDE;
};

#endif // _BRepOffsetAPI_MakeThickSolid_Extension_HeaderFile
