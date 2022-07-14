/*! \file BRepAlgoAPI_IntSS.hxx
*   \brief BRepAlgoAPI_IntSS类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef _BRepAlgoAPI_IntSS_HeaderFile
#define _BRepAlgoAPI_IntSS_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>
#include <GeomAPI_IntSS.hxx>

#include <Standard_Boolean.hxx>
#include <BRepAlgoAPI_BooleanOperation.hxx>
#include <Geom_Plane.hxx>
class BOPAlgo_PaveFiller;
class TopoDS_Shape;
class gp_Pln;
class Geom_Surface;



//! The algorithm is to build a Secton operation between arguments and tools.
//! The result of Section operation consists of vertices and edges.
//! The result of Section operation contains:
//! 1. new vertices that are subjects of V/V, E/E, E/F, F/F interferences
//! 2. vertices that are subjects of V/E, V/F interferences
//! 3. new edges that are subjects of F/F interferences
//! 4. edges that are Common Blocks
/*! \class BRepAlgoAPI_IntSS
*   \brief BRepAlgoAPI_IntSS类封装了surface和surface之间求交的算法。
 */
class BRepAlgoAPI_IntSS : public BRepAlgoAPI_BooleanOperation
{
public:

    DEFINE_STANDARD_ALLOC


    //! Empty constructor
	/*! \fn BRepAlgoAPI_IntSS()
	*  \brief BRepAlgoAPI_IntSS的构造函数.
	*/
    Standard_EXPORT BRepAlgoAPI_IntSS();

	/*! \fn virtual ~BRepAlgoAPI_IntSS()
	*  \brief BRepAlgoAPI_IntSS的析构函数.
	*/
	Standard_EXPORT virtual ~BRepAlgoAPI_IntSS();

    //! initialize the argument
    //! <S1>  - argument
    //! Obsolete
	/*! \fn void Init(const TopoDS_Shape& S1, const TopoDS_Shape& S2)
	*  \brief 初始化函数, 通过两个拓扑数据初始化.
	*  \param S1 面1的拓扑数据.
	*  \param S2 面2的拓扑数据.
	*/
    Standard_EXPORT void Init(const TopoDS_Shape& S1, const TopoDS_Shape& S2);

	/*! \fn void Init(const TopoDS_Shape& S1, const TopoDS_Shape& S2)
	*  \brief 初始化函数, 通过拓扑数据和几何数据初始化.
	*  \param S1 面1的拓扑数据.
	*  \param S2 面2的几何数据.
	*/
    Standard_EXPORT void Init(const TopoDS_Shape& S1, Handle(Geom_Surface)  cutSurf);

    //! Performs the algorithm
    //! Filling interference Data Structure (if it is necessary)
    //! Building the result of the operation.
	/*! \fn virtual void Build()
	*  \brief 求交的接口.
	*/
    Standard_EXPORT virtual void Build() Standard_OVERRIDE;

    /*! \fn virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S)
    *  \brief 重写父类接口，根据拓扑获取相关编辑过的拓扑.
    *  \param S 需要进行查找的拓扑.
    *  \return 相关编辑过的拓扑序列.
    */
    Standard_EXPORT virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& shapeS) Standard_OVERRIDE;

private:

    GeomAPI_IntSS       myIntSS;
    TopoDS_Shape        myS1;
    TopoDS_Shape        myS2;
    Handle(Geom_Surface)  myCutSurf;

};

#endif // _BRepAlgoAPI_Section_Extension_HeaderFile
