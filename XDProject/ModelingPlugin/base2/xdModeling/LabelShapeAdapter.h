/*! \file LabelShapeAdapter.h
*   \brief LabelShapeAdapter类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef LABELSHAPEADAPTER_H
#define LABELSHAPEADAPTER_H

// #include <map>
#include "EnumAndStruct.h"
#include "xdModelingHeader.h"

#include <TDF_Label.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include <NCollection_DataMap.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc.hxx>

/*! \class LabelShapeAdapter
*   \brief LabelShapeAdapter类处理节点和模型数据之间的匹配关系，包括模型导入、布尔运算、倒角等。例如，布尔运算前后新增、删除、修改的拓扑在原节点和新节点中的位置。
 */
class XDMODELING_API LabelShapeAdapter
{
public:
    /*! \fn LabelShapeAdapter()
    *  \brief LabelShapeAdapter的构造函数.
    */
    LabelShapeAdapter();

    /*! \fn ~LabelShapeAdapter()
    *  \brief LabelShapeAdapter的析构函数.
    */
    ~LabelShapeAdapter();

    /*! \fn void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const int& numBrothers, const bool& topLevel)
    *  \brief 将shape分解成各个级别的拓扑结构，并分层级挂载label节点下，不添加名称，用于布尔运算等，接口已弃用.
    *  \param origShape 需要进行过滤的TopoDS_Shape对象.
    *  \return 返回完成过滤的TopoDS_Shape对象.
    */
    void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const int& numBrothers, const bool& topLevel);

    /*! \fn static void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const std::string& name, const int& id)
    *  \brief 将shape分解成各个级别的拓扑结构，并分层级挂载label节点下，用于基本几何创建等。支持自动命名. 接口已弃用.
    *  \param origShape 需要进行过滤的TopoDS_Shape对象.
    *  \return 返回完成过滤的TopoDS_Shape对象.
    */
    static void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const std::string& name, const int& id);

    /*! \fn static void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const std::string& name, const int& id)
    *  \brief 将模型数据分解成各个级别的拓扑结构，并分层级挂载节点下，并去重。用于基本几何创建等.
    *  \param label 目标节点.
    *  \param shape 需要解析挂载的模型数据.
    */
    void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape);

    /*! \fn static void replaceLabelPropertyWithName(const TDF_Label& sourceLabel, const TDF_Label& targetRootLabel)
    *  \brief 用于已有模型修改参数后的属性传递，接口已弃用.
    */
	static void replaceLabelPropertyWithName(const TDF_Label& sourceLabel, const TDF_Label& targetRootLabel);

    /*! \fn static void replaceLabelWithChangedShape(const TDF_Label& label, const TopoDS_Shape& changedShape, const std::string& name, const int& id)
    *  \brief 将changedShape各个级别的拓扑结构替换label节点下的shape，用于已有模型修改参数再次创建，接口已弃用.
    */
    static void replaceLabelWithChangedShape(const TDF_Label& label, const TopoDS_Shape& changedShape, const std::string& name, const int& id);

    /*! \fn static void replaceChildLabelWithCopiedShape(const TDF_Label& label, const TopoDS_Shape& copiedShape)
    *  \brief 将目标模型数据进行拓扑解析，并递归比对目标节点下各子节点的模型数据，如果是一样的就进行替换.
    *  \param label 目标节点.
    *  \param shape 需要解析替换的模型数据.
    */
    static void replaceChildLabelWithCopiedShape(const TDF_Label& label, const TopoDS_Shape& copiedShape);

    /*! \fn static void replaceChildLabelWithCopiedShape1(const TDF_Label& label, const TopoDS_Shape& copiedShape)
    *  \brief 将copiedShape各个级别的拓扑结构替换label节点下的shape，以TShape为相等依据，接口已弃用.
    */
    static void replaceChildLabelWithCopiedShape1(const TDF_Label& label, const TopoDS_Shape& copiedShape);

    /*! \fn static void replaceChildLabelWithCopiedShape2(const TDF_Label& label, const TopoDS_Shape& copiedShape)
    *  \brief 将copiedShape各个级别的拓扑结构替换label节点下的shape，以TShape和location为相等依据，接口已弃用.
    */
    static void replaceChildLabelWithCopiedShape2(const TDF_Label& label, const TopoDS_Shape& copiedShape);

    /*! \fn static void buildEntityAttributeName(const TDF_Label& orgLabel, const TDF_Label& curLabel, bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID)
    *  \brief 为节点的模型赋值名称属性，如果是新模型就构建新名称，如果是原模型进行了编辑则继承名称.
    *  \param orgLabel 新模型的节点.
    *  \param curLabel 编辑后模型节点.
    *  \param bIsNewCreate 模型是否是新建的.
    *  \param bIsCloneCS 模型是否是拷贝的.
    *  \param bNeedCopyFaceName 模型面或者边的名称.
    *  \param iNextFaceID 模型面的ID.
    *  \param iNextEdgeID 模型边的ID.
    */
    static void buildEntityAttributeName(const TDF_Label& orgLabel, const TDF_Label& curLabel,
        bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID);

    /*! \fn void collectRelatedLabelAndShapes(const TDF_Label& label, const EnAdapatType& type, void* operation)
    *  \brief 收集各类操作中节点的模型拓扑新增、编辑、删除和保留的信息，并保存成类成员.
    *  \param label 目标节点.
    *  \param type 操作类型.
    *  \param operation 操作的算子，例如布尔运算的算子等.
    */
    void collectRelatedLabelAndShapes(const TDF_Label& label, const EnAdapatType& type, void* operation);
    
    /*! \fn void labelDiscreteToMap(const TDF_Label& label)
    *  \brief 取出label中挂载的拓扑，将其与本label进行绑定，并存储到本地. 本接口递归调用.
    *  \param label 需要处理的节点.
    */
    void labelDiscreteToMap(const TDF_Label& label);

    /*! \fn void collectRelatedLabelAndShapes(const TopoDS_Shape& targetShape, const EnAdapatType& type, void* operation)
    *  \brief 根据不同的模型操作的算子，获取操作过程中的历史追踪，递归调用拓扑并查找其在历史追踪中的作用，将根据其生成、编辑、删除的拓扑保存到本地.
    *  \param origShape 进行查找的拓扑数据.
    *  \param type 操作类型.
    *  \param operation 操作的算子，例如布尔运算的算子等.
    */
    void collectRelatedLabelAndShapes(const TopoDS_Shape& origShape, const EnAdapatType& type, void* operation);

	/*! \fn void directRelateShapes(const TopoDS_Shape& origShape, const TopoDS_Shape& newShape)
	*  \brief 在剥离最外层shell、compound或者裁剪的时候采用的对应label的方法.
	*  \param origShape 原模型数据.
	*  \param newShape 新模型数据.
	*/
	void directRelateShapes(const TopoDS_Shape& origShape, const TopoDS_Shape& newShape);

    /*! \fn void linkRelatedLabels(const TDF_Label& targetLabel)
    *  \brief 根据成员中已经保存的模型拓扑新增、编辑、删除和保留的信息，递归挂载新拓扑到目标节点.
    *  \param targetLabel 目标节点.
    */
    void linkRelatedLabels(const TDF_Label& targetLabel);

    /*! \fn void linkRelatedLabels(const TDF_Label& targetLabel)
    *  \brief linkRelatedLabels的补充内容. 用于fuse和common，模型数据找不到对应的的情况. 默认与主体一对一匹配.
    *  \param targetLabel 目标节点.
    */
    void labelLinkMakeup(const TDF_Label& targetLabel);

    /*! \fn void collectLabelShapes(const TDF_Label& label, const EnAdapatType& type, void* operation)
    *  \brief 根据操作类型，收集所有编辑前后的shape类型以及对应label，接口已弃用.
    */
    void collectLabelShapes(const TDF_Label& label, const EnAdapatType& type, void* operation);

    /*! \fn void collectLabelShapesGeneratedByEdgeAndVertex(TopoDS_Shape sourceShape, const EnAdapatType& type, void* operation)
    *  \brief 根据操作类型，收集所有编辑前后的shape类型以及对应label，接口已弃用.
    */
	void collectLabelShapesGeneratedByEdgeAndVertex(TopoDS_Shape sourceShape, const EnAdapatType& type, void* operation);

    /*! \fn void transferColorAndName4ImportingStep(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool)
    *  \brief 将通过OCC的XDE方式导入step文件得到的数据挂载到本动态库的新节点上. 层级解析拓扑，递归调用.
    *  \param targetLabel 目标节点.
    *  \param shapeTool 在XDE的数据中的模型查找工具.
    */
    void transferColorAndName4ImportingStep(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool);

    /*! \fn void transferColorAndName4ImportingIges(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool)
    *  \brief 将通过OCC的XDE方式导入iges文件得到的数据挂载到本动态库的新节点上. 层级解析拓扑，递归调用.
    *  \param targetLabel 目标节点.
    *  \param shapeTool 在XDE的数据中的模型查找工具.
    */
    void transferColorAndName4ImportingIges(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool);

    /*! \fn bool isSupportedType(TopoDS_Shape shape)
    *  \brief 判断是否为支持查找过往信息的拓扑类型.
    *  \param shape 目标模型数据.
    *  \return 返回是否支持
    */
    bool isSupportedType(TopoDS_Shape shape);

    /*! \fn std::string getName() const
    *  \brief 获取成员m_name.
    *  \return 获取到的成员值.
    */
    std::string getName() const { return m_name; }

    /*! \fn void setName(std::string val)
    *  \brief 设定成员m_name的值.
    *  \param val 需要设定的string对象.
    */
    void setName(std::string val) { m_name = val; }

    /*! \fn TDF_Label getTopLabel() const
    *  \brief 获取成员m_topLabel.
    *  \return 获取到的成员值.
    */
    TDF_Label getTopLabel() const { return m_topLabel; }

    /*! \fn void setTopLabel(TDF_Label val)
    *  \brief 设定成员m_topLabel的值.
    *  \param val 需要设定的TDF_Label对象.
    */
    void setTopLabel(TDF_Label val) { m_topLabel = val; }

    /*! \fn void clearRelatedShapes()
    *  \brief 清除成员中用于记录编辑、新增、保留的拓扑信息的内容.
    */
    void clearRelatedShapes() { m_modifiedShapes.Clear(); m_constantShapes.Clear(); m_generatedShapes.Clear(); m_shapeHistoryMap; }

    /*! \fn void addModifiedShapes(TopoDS_Shape shape, TDF_Label label)
    *  \brief 在记录拓扑编辑信息的成员中添加新的模型数据和对应节点.
    *  \param shape 新增的模型数据.
    *  \param label 新增的模型数据对应的节点.
    */
    void addModifiedShapes(TopoDS_Shape shape, TDF_Label label)
    { 
        LabelInfo info;
        info.label = label;
        info.calledTimes = 0;
        m_modifiedShapes.Bind(shape, info); 
    }

protected:
    TopAbs_ShapeEnum                                m_supportType;

    TDF_Label                                       m_topLabel;

    std::string                                     m_name;

    TopTools_ListOfShape                            m_addedNewShape;

    NCollection_DataMap<TopoDS_Shape, LabelInfo>    m_modifiedShapes;

    NCollection_DataMap<TopoDS_Shape, LabelInfo>    m_constantShapes;

    NCollection_DataMap<TopoDS_Shape, LabelInfo>    m_generatedShapes;


    NCollection_DataMap<TopoDS_Shape, LabelInfo>    m_shapeHistoryMap;

};

#endif