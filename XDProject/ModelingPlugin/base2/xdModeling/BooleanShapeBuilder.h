/*! \file BooleanShapeBuilder.h
*   \brief BooleanShapeBuilder类的头文件
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
*   \brief 用于处理各种布尔运算的类
 */
class BooleanShapeBuilder :
    public ShapeOperationBuilder
{
public:
    /*! \fn BooleanShapeBuilder()
    *  \brief BooleanShapeBuilder的构造函数.
    */
    BooleanShapeBuilder();
    /*! \fn ~BooleanShapeBuilder()
    *  \brief BooleanShapeBuilder的析构函数.
    */
    ~BooleanShapeBuilder();

    /*! \fn virtual TopoDS_Shape getRestShape() const
    *  \brief 获取成员TopoDS_Shape对象，用于获取分割的剩余拓扑数据.
    *  \return 返回成员TopoDS_Shape对象.
    */
    virtual TopoDS_Shape getRestShape() const { return m_restShape; }

    /*! \fn virtual TopoDS_Shape getRestShape() const
    *  \brief 获取成员TopoDS_Shape对象链表，用于线线、线面、面面求交的结果.
    *  \return 返回成员TopoDS_Shape对象.
    */
    virtual TopTools_ListOfShape getResultShape() const { return m_ressultShapes; }

    /*! \fn void* getBuilder() const
    *  \brief 获取成员m_builder，即当前布尔运算的算子.
    *  \return 获取到的算子指针.
    */
    virtual void* getBuilder() const;

public:    /*! \fn bool booleanOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, EnAdapatType boolType, const double& fuzzyValue)
    *  \brief 进行布尔运算的接口，包括布尔交、并、差、模糊布尔运算.
    *  \param shape1 主模型的对象.
    *  \param shape2 工具模型的对象.
    *  \param fuzzyValue 模糊布尔运算的数值，非模糊布尔运算该数值为精度的全局变量.
    *  \return 操作是否成功.
    */
    bool booleanOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, BOPAlgo_Operation boolType, const double& fuzzyValue);

    /*! \fn bool booleanOperation(TopTools_ListOfShape args, TopTools_ListOfShape tools, EnAdapatType boolType, const double& fuzzyValue)
    *  \brief 批量进行布尔运算的接口，包括布尔交、并、差、模糊布尔运算.
    *  \param args 主模型的对象集.
    *  \param tools 工具模型的对象集.
    *  \param fuzzyValue 模糊布尔运算的数值，非模糊布尔运算该数值为精度的全局变量.
    *  \return 操作是否成功.
    */
    bool booleanOperation(TopTools_ListOfShape args, TopTools_ListOfShape tools, BOPAlgo_Operation boolType, const double& fuzzyValue);

    /*! \fn bool singleNonRegFuseOperation(TopoDS_Shape shape1, TopoDS_Shape shape2)
    *  \brief 进行不规则布尔运算中布尔并的操作，得到shape1与shape2的并集.
    *  \param shape1 主模型的对象.
    *  \param shape2 工具模型的对象.
    *  \return 操作是否成功.
    */
    bool singleNonRegFuseOperation(TopoDS_Shape shape1, TopoDS_Shape shape2);

    /*! \fn bool cellsBuilderOperation(TopTools_ListOfShape args, TopTools_ListOfShape tools, EnAdapatType boolType)
    *  \brief 批量进行不规则布尔运算的接口，包括不规则布尔交、并、差.
    *  \param args 主模型的对象集.
    *  \param tools 工具模型的对象集.
    *  \return 操作是否成功.
    */
    bool cellsBuilderOperation(TopTools_ListOfShape args, EnAdapatType boolType);

	/*! \fn bool detachEdges(TopoDS_Shape origWire, TopTools_ListOfShape toolEdges, const bool& isCreated)
	*  \brief 根据线框中的线分割线框的接口.
	*  \param origWire 进行分割的线框模型.
	*  \param toolEdges 分割的工具线链表.
	*  \param isCreated 根据工具线链表创建新模型还是分割线框获得新模型.
	*  \return 是否成功创建.
	*/
	bool detachEdges(TopoDS_Shape origWire, TopTools_ListOfShape toolEdges, const bool& isCreated);
		
    /*! \fn bool faceSplitOperation(TopoDS_Shape shape1, TopoDS_Shape shape2)
    *  \brief 接口已弃用.
    */
    bool faceSplitOperation(TopoDS_Shape shape1, TopoDS_Shape shape2);

    /*! \fn bool faceSplitOperation(TopoDS_Shape shape1, TopTools_ListOfShape toolEdges)
    *  \brief 接口已弃用.
    */
    bool faceSplitOperation(TopoDS_Shape shape1, TopTools_ListOfShape toolEdges);

	/*! \fn bool shapeSplitOperation(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
	*  \brief 拓扑切割的接口，通过面法向构建能够包裹住一边的立方体，然后通过common和cut的方式来实现切割.
	*  \param srcShape 进行切割的拓扑.
	*  \param basePosX 切割平面法向起点的x坐标.
	*  \param basePosY 切割平面法向起点的y坐标.
	*  \param basePosZ 切割平面法向起点的z坐标.
	*  \param normalVecX 切割平面法向向量的x分量.
	*  \param normalVecY 切割平面法向向量的y分量.
	*  \param normalVecZ 切割平面法向向量的z分量.
	*  \param normalVecSide 返回模型是否为法向方向部分.
	*  \return 操作是否成功.
	*/
	bool shapeSplitOperation(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide);

	/*! \fn bool shapeSplitOperation2(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
	*  \brief 拓扑切割的接口，通过OCC的BRepAlgoAPI_Splitter进行切割，返回完成切割的整体模型.
	*  \param srcShape 进行切割的拓扑.
	*  \param basePosX 切割平面法向起点的x坐标.
	*  \param basePosY 切割平面法向起点的y坐标.
	*  \param basePosZ 切割平面法向起点的z坐标.
	*  \param normalVecX 切割平面法向向量的x分量.
	*  \param normalVecY 切割平面法向向量的y分量.
	*  \param normalVecZ 切割平面法向向量的z分量.
	*  \param normalVecSide 返回模型是否为法向方向部分.
	*  \return 操作是否成功.
	*/
	bool shapeSplitOperation2(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ,
		const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide);

	/*! \fn bool shapeSplitOperation2(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
	*  \brief 拓扑切割的接口，通过OCC的BRepAlgoAPI_Splitter进行切割，返回完成切割的整体模型.
	*  \param srcShape 进行切割的拓扑.
	*  \param cutShapes 用于切割的工具拓扑.
	*  \return 操作是否成功.
	*/
	bool shapeSplitOperation2(TopoDS_Shape srcShape, TopTools_ListOfShape cutShapes);

	/*! \fn bool shapeSewing(TopTools_ListOfShape shapes, bool constructCompound = false)
	*  \brief 曲面缝合的接口.
	*  \param shapes 进行缝合的拓扑.
	*  \param constructCompound 是否生成compound.
	*  \return 操作是否成功.
	*/
	bool shapeSewing(TopTools_ListOfShape shapes, bool constructCompound = false);

	/*! \fn bool shapeSewing(TopTools_ListOfShape shapes, bool constructCompound = false)
	*  \brief 曲面缝合的接口.采用volumeMarker进行处理
	*  \param faces 进行缝合的拓扑.
	*  \return 操作是否成功.
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