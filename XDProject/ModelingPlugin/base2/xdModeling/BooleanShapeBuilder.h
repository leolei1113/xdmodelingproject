/*! \file BooleanShapeBuilder.h
*   \brief BooleanShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef BOOLEANSHAPEBUILDER_H
#define BOOLEANSHAPEBUILDER_H

#include "ShapeOperationBuilder.h"
#include "EnumAndStruct.h"

#include <TopTools_ListOfShape.hxx>
#include <Geom_Plane.hxx>
#include <BOPAlgo_Operation.hxx>

class BOPAlgo_CellsBuilder;
class BRepBuilderAPI_Sewing;
class BOPAlgo_MakerVolume;
/*! \class BooleanShapeBuilder
*   \brief ���ڴ�����ֲ����������
 */
class BooleanShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn BooleanShapeBuilder()
    *  \brief BooleanShapeBuilder�Ĺ��캯��.
    */
    BooleanShapeBuilder();
    /*! \fn ~BooleanShapeBuilder()
    *  \brief BooleanShapeBuilder����������.
    */
    ~BooleanShapeBuilder();

    /*! \fn virtual TopoDS_Shape getRestShape() const
    *  \brief ��ȡ��ԱTopoDS_Shape�������ڻ�ȡ�ָ��ʣ����������.
    *  \return ���س�ԱTopoDS_Shape����.
    */
    virtual TopoDS_Shape getRestShape() const { return m_restShape; }

    /*! \fn virtual TopoDS_Shape getRestShape() const
    *  \brief ��ȡ��ԱTopoDS_Shape���������������ߡ����桢�����󽻵Ľ��.
    *  \return ���س�ԱTopoDS_Shape����.
    */
    virtual TopTools_ListOfShape getResultShape() const { return m_ressultShapes; }

    /*! \fn void* getBuilder() const
    *  \brief ��ȡ��Աm_builder������ǰ�������������.
    *  \return ��ȡ��������ָ��.
    */
    virtual void* getBuilder() const;

public:    /*! \fn bool booleanOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, EnAdapatType boolType, const double& fuzzyValue)
    *  \brief ���в�������Ľӿڣ������������������ģ����������.
    *  \param shape1 ��ģ�͵Ķ���.
    *  \param shape2 ����ģ�͵Ķ���.
    *  \param fuzzyValue ģ�������������ֵ����ģ�������������ֵΪ���ȵ�ȫ�ֱ���.
    *  \return �����Ƿ�ɹ�.
    */
    bool booleanOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, BOPAlgo_Operation boolType, const double& fuzzyValue);

    /*! \fn bool booleanOperation(TopTools_ListOfShape args, TopTools_ListOfShape tools, EnAdapatType boolType, const double& fuzzyValue)
    *  \brief �������в�������Ľӿڣ������������������ģ����������.
    *  \param args ��ģ�͵Ķ���.
    *  \param tools ����ģ�͵Ķ���.
    *  \param fuzzyValue ģ�������������ֵ����ģ�������������ֵΪ���ȵ�ȫ�ֱ���.
    *  \return �����Ƿ�ɹ�.
    */
    bool booleanOperation(TopTools_ListOfShape args, TopTools_ListOfShape tools, BOPAlgo_Operation boolType, const double& fuzzyValue);

    /*! \fn bool singleNonRegFuseOperation(TopoDS_Shape shape1, TopoDS_Shape shape2)
    *  \brief ���в����򲼶������в������Ĳ������õ�shape1��shape2�Ĳ���.
    *  \param shape1 ��ģ�͵Ķ���.
    *  \param shape2 ����ģ�͵Ķ���.
    *  \return �����Ƿ�ɹ�.
    */
    bool singleNonRegFuseOperation(TopoDS_Shape shape1, TopoDS_Shape shape2);

    /*! \fn bool cellsBuilderOperation(TopTools_ListOfShape args, TopTools_ListOfShape tools, EnAdapatType boolType)
    *  \brief �������в����򲼶�����Ľӿڣ����������򲼶�����������.
    *  \param args ��ģ�͵Ķ���.
    *  \param tools ����ģ�͵Ķ���.
    *  \return �����Ƿ�ɹ�.
    */
    bool cellsBuilderOperation(TopTools_ListOfShape args, EnAdapatType boolType);

	/*! \fn bool detachEdges(TopoDS_Shape origWire, TopTools_ListOfShape toolEdges, const bool& isCreated)
	*  \brief �����߿��е��߷ָ��߿�Ľӿ�.
	*  \param origWire ���зָ���߿�ģ��.
	*  \param toolEdges �ָ�Ĺ���������.
	*  \param isCreated ���ݹ�������������ģ�ͻ��Ƿָ��߿�����ģ��.
	*  \return �Ƿ�ɹ�����.
	*/
	bool detachEdges(TopoDS_Shape origWire, TopTools_ListOfShape toolEdges, const bool& isCreated);
		
    /*! \fn bool faceSplitOperation(TopoDS_Shape shape1, TopoDS_Shape shape2)
    *  \brief �ӿ�������.
    */
    bool faceSplitOperation(TopoDS_Shape shape1, TopoDS_Shape shape2);

    /*! \fn bool faceSplitOperation(TopoDS_Shape shape1, TopTools_ListOfShape toolEdges)
    *  \brief �ӿ�������.
    */
    bool faceSplitOperation(TopoDS_Shape shape1, TopTools_ListOfShape toolEdges);

	/*! \fn bool shapeSplitOperation(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
	*  \brief �����и�Ľӿڣ�ͨ���淨�򹹽��ܹ�����סһ�ߵ������壬Ȼ��ͨ��common��cut�ķ�ʽ��ʵ���и�.
	*  \param srcShape �����и������.
	*  \param basePosX �и�ƽ�淨������x����.
	*  \param basePosY �и�ƽ�淨������y����.
	*  \param basePosZ �и�ƽ�淨������z����.
	*  \param normalVecX �и�ƽ�淨��������x����.
	*  \param normalVecY �и�ƽ�淨��������y����.
	*  \param normalVecZ �и�ƽ�淨��������z����.
	*  \param normalVecSide ����ģ���Ƿ�Ϊ�����򲿷�.
	*  \return �����Ƿ�ɹ�.
	*/
	bool shapeSplitOperation(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide);

	/*! \fn bool shapeSplitOperation2(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
	*  \brief �����и�Ľӿڣ�ͨ��OCC��BRepAlgoAPI_Splitter�����и��������и������ģ��.
	*  \param srcShape �����и������.
	*  \param basePosX �и�ƽ�淨������x����.
	*  \param basePosY �и�ƽ�淨������y����.
	*  \param basePosZ �и�ƽ�淨������z����.
	*  \param normalVecX �и�ƽ�淨��������x����.
	*  \param normalVecY �и�ƽ�淨��������y����.
	*  \param normalVecZ �и�ƽ�淨��������z����.
	*  \param normalVecSide ����ģ���Ƿ�Ϊ�����򲿷�.
	*  \return �����Ƿ�ɹ�.
	*/
	bool shapeSplitOperation2(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide);

	/*! \fn bool shapeSplitOperation2(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
	*  \brief �����и�Ľӿڣ�ͨ��OCC��BRepAlgoAPI_Splitter�����и��������и������ģ��.
	*  \param srcShape �����и������.
	*  \param cutShapes �����и�Ĺ�������.
	*  \return �����Ƿ�ɹ�.
	*/
	bool shapeSplitOperation2(TopoDS_Shape srcShape, TopTools_ListOfShape cutShapes);

	/*! \fn bool shapeSewing(TopTools_ListOfShape shapes, bool constructCompound = false)
	*  \brief �����ϵĽӿ�.
	*  \param shapes ���з�ϵ�����.
	*  \param constructCompound �Ƿ�����compound.
	*  \return �����Ƿ�ɹ�.
	*/
	bool shapeSewing(TopTools_ListOfShape shapes, bool constructCompound = false);

	/*! \fn bool shapeSewing(TopTools_ListOfShape shapes, bool constructCompound = false)
	*  \brief �����ϵĽӿ�.����volumeMarker���д���
	*  \param faces ���з�ϵ�����.
	*  \return �����Ƿ�ɹ�.
	*/
	bool volumeMarker(TopTools_ListOfShape faces);

    bool shapeImprintOperation(TopoDS_Shape mainshape, TopTools_ListOfShape tools, const double& fuzzyvalue, int method);

    bool faceCommon(TopTools_ListOfShape args, TopTools_ListOfShape tools,
        LabelShapeAdapter* adapter, double fuzzyvalue);

    bool cellsBuilderOperation(TopTools_ListOfShape args,
        TopTools_ListOfShape tools, EnAdapatType boolType, bool isRemoveInternal,
        double fuzzyValue);

    bool splitReshape(TopoDS_Shape blank, TopTools_ListOfShape tools);

protected:
    TopoDS_Shape                    m_restShape;//used for splitting

    TopTools_ListOfShape            m_ressultShapes;
//     BRepAlgoAPI_Cut*            m_cut;
//     BRepAlgoAPI_Fuse*           m_fuse;
//     BRepAlgoAPI_Common*         m_common;
    BOPAlgo_CellsBuilder*			m_nonreg;
	BRepBuilderAPI_Sewing*			m_sewOperator;
	BOPAlgo_MakerVolume*			m_mkVolume;
    BOPAlgo_BuilderShape*           m_pBOPAlgo;

	EnAdapatType					m_operationType;
//     bool							m_doCellsBuilder;
};

#endif