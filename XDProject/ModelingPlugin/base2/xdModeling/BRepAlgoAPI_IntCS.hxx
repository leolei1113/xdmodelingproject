/*! \file BRepAlgoAPI_IntCS.hxx
*   \brief BRepAlgoAPI_IntCS���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef _BRepAlgoAPI_IntCS_HeaderFile
#define _BRepAlgoAPI_IntCS_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>
#include <GeomAPI_IntCS.hxx>

#include <Standard_Boolean.hxx>
#include <BRepAlgoAPI_BooleanOperation.hxx>
class BOPAlgo_PaveFiller;
class TopoDS_Shape;
class gp_Pln;
class Geom_Surface;
class Geom_Curve;



//! The algorithm is to build a Secton operation between arguments and tools.
//! The result of Section operation consists of vertices and edges.
//! The result of Section operation contains:
//! 1. new vertices that are subjects of V/V, E/E, E/F, F/F interferences
//! 2. vertices that are subjects of V/E, V/F interferences
//! 3. new edges that are subjects of F/F interferences
//! 4. edges that are Common Blocks
/*! \class BRepAlgoAPI_IntCS
*   \brief BRepAlgoAPI_IntCS���װ��curve��surface֮���󽻵��㷨��
 */
class BRepAlgoAPI_IntCS : public BRepAlgoAPI_BooleanOperation
{
public:

    DEFINE_STANDARD_ALLOC


    /*! \fn BRepAlgoAPI_IntCS()
    *  \brief BRepAlgoAPI_IntCS�Ĺ��캯��.
    */
    Standard_EXPORT BRepAlgoAPI_IntCS();

	/*! \fn virtual ~BRepAlgoAPI_IntCS()
	*  \brief BRepAlgoAPI_IntCS����������.
	*/
	Standard_EXPORT virtual ~BRepAlgoAPI_IntCS();

    //! initialize the argument
    //! <S1>  - argument
    //! Obsolete
	/*! \fn void Init(const TopoDS_Shape& S1, const TopoDS_Shape& S2)
	*  \brief ��ʼ������.
	*  \param S1 �����������.
	*  \param S2 �ߵ���������.
	*/
    Standard_EXPORT void Init(const TopoDS_Shape& S1, const TopoDS_Shape& S2);

    //! Performs the algorithm
    //! Filling interference Data Structure (if it is necessary)
    //! Building the result of the operation.
	/*! \fn virtual void Build()
	*  \brief �󽻵Ľӿ�.
	*/
    Standard_EXPORT virtual void Build() Standard_OVERRIDE;

    /*! \fn virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& S)
    *  \brief ��д����ӿڣ��������˻�ȡ��ر༭��������.
    *  \param S ��Ҫ���в��ҵ�����.
    *  \return ��ر༭������������.
    */
    Standard_EXPORT virtual const TopTools_ListOfShape& Modified(const TopoDS_Shape& shapeS) Standard_OVERRIDE;

private:

    GeomAPI_IntCS       myIntCS;
    TopoDS_Shape        myS1;
    TopoDS_Shape        myS2;
};

#endif // _BRepAlgoAPI_Section_Extension_HeaderFile
