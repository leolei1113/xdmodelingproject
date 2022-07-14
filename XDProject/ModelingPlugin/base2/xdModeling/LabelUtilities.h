/*! \file LabelUtilities.h
*   \brief LabelUtilities类的头文件
*   \author HST
*   \date 2020.10.8
*/

#ifndef LABELUTILITIES_H
#define LABELUTILITIES_H

#include "EnumAndStruct.h"
#include "xdModelingHeader.h"

#include <map>
#include <vector>

#include <TDF_Label.hxx>
#include <TopoDS_Shape.hxx>
#include <NCollection_DataMap.hxx>
#include <TDF_LabelList.hxx>

class TDocStd_Document;
/*! \class LabelUtilities
*   \brief 用于存放一些关于OCAF中Label处理的静态方法
 */
class XDMODELING_API LabelUtilities
{
public:
    /*! \fn LabelUtilities()
    *  \brief LabelUtilities的构造函数.
    */
    LabelUtilities();

    /*! \fn ~LabelUtilities()
    *  \brief LabelUtilities的析构函数.
    */
    ~LabelUtilities();

    /*! \fn static bool copyLabelWithoutFather(const TDF_Label& source, const TDF_Label& target)
    *  \brief 拷贝原Label节点下所有子节点数据到新的Label节点上.
    *  \param source 需要进行拷贝的节点.
    *  \param target 目标节点.
    *  \return 是否成功进行操作.
    */
    static bool copyLabelWithoutFather(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelWithFather(const TDF_Label& source, const TDF_Label& target)
    *  \brief 拷贝原Label节点及子节点的数据到新的Label节点上.
    *  \param source 需要进行拷贝的节点.
    *  \param target 目标节点.
    *  \return 是否成功进行操作.
    */
    static bool copyLabelWithFather(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelWithFather(const TDocStd_Document* doc, const std::string& sourceEntry, const std::string& targetEntry)
    *  \brief 拷贝原Label节点及子节点的数据到新的Label节点上，节点通过PID进行查找.
    *  \param doc 数据集指针.
    *  \param sourceEntry 需要进行拷贝的节点的PID.
    *  \param targetEntry 目标节点的PID.
    *  \return 是否成功进行操作.
    */
    static bool copyLabelWithFather(const TDocStd_Document* doc, const std::string& sourceEntry, const std::string& targetEntry);

    /*! \fn static bool copyLabelWithOnlyPropertyChild(const TDF_Label& source, const TDF_Label& target)
    *  \brief 拷贝原Label节点下名称为“property”的子节点数据到新的Label节点上，接口已弃用.
    *  \param source 需要进行拷贝的节点.
    *  \param target 目标节点.
    *  \return 是否成功进行操作.
    */
    static bool copyLabelWithOnlyPropertyChild(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelProperty(const TDF_Label& source, const TDF_Label& target)
    *  \brief 拷贝原Label节点下名称为“property”的子节点数据到新的Label节点上.
    *  \param source 需要进行拷贝的节点.
    *  \param target 目标节点.
    *  \return 是否成功进行操作.
    */
    static bool copyLabelProperty(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelName(const TDF_Label& source, const TDF_Label& target)
    *  \brief 拷贝原Label节点名称到新的Label节点上.
    *  \param source 需要进行拷贝的节点.
    *  \param target 目标节点.
    *  \return 是否成功进行操作.
    */
    static bool copyLabelName(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool findShapeLabel(const TDF_Label& label, const TopoDS_Shape& compareShape, TDF_Label& returnLabel)
    *  \brief 从根节点递归遍历数据节点，查找是否有相同的模型数据，有则返回找到的节点.
    *  \param label 需要进行查找的节点.
    *  \param compareShape 需要进行查找的模型数据对象.
    *  \param returnLabel 返回找到的节点.
    *  \return 是否找到节点.
    */
    static bool findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, TDF_Label& returnLabel);

    /*! \fn static bool findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, TDF_Label& returnLabel)
    *  \brief 从根节点递归遍历数据节点，查找是否有相同的模型数据，不返回找到的节点.
    *  \param label 需要进行查找的节点.
    *  \param compareShape 需要进行查找的模型数据对象.
    *  \return 是否找到节点.
    */
    static bool findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape);

    /*! \fn static bool findShapePIDFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, std::string& strPID)
    *  \brief 从根节点递归遍历数据节点，查找是否有相同的模型数据，有则返回找到的节点的PID.
    *  \param label 需要进行查找的节点.
    *  \param compareShape 需要进行查找的模型数据对象.
    *  \param returnLabel 返回找到的节点的PID.
    *  \return 是否找到节点.
    */
    static bool findShapePIDFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, std::string& strPID);

	/*! \fn static bool findSubShapePIDsFromRoot(const TDF_Label& label, const TopAbs_ShapeEnum& shapeType, std::vector<std::string>& strPIDs)
	*  \brief 从根节点递归遍历数据节点，查找所有与shapeType相同类型拓扑的PIDD.
	*  \param label 需要进行查找的节点.
	*  \param shapeType 标准拓扑类型.
	*  \param strPIDs 返回找到的节点的PID.
	*  \return 是否找到节点.
	*/
	static bool findSubShapePIDsFromRoot(const TDF_Label& label, const TopAbs_ShapeEnum& shapeType,
		std::vector<std::string>& strPIDs);

    /*! \fn static bool findNameLabel(const TDF_Label& sourceRootLabel, const std::string& name, TDF_Label& retuanLabel)
    *  \brief 递归遍历数据节点，查找是否有相同的名称，有则返回找到的节点.
    *  \param label 需要进行查找的节点.
    *  \param name 需要进行查找的模型数据对象.
    *  \param returnLabel 返回找到的节点.
    *  \return 是否找到节点.
    */
	static bool findNameLabel(const TDF_Label& label, const std::string& name, TDF_Label& retuanLabel);

    /*! \fn static TDF_Label getOwnerLabel(const TDF_Label& childLabel)
    *  \brief 获取childLabeld的次顶级节点，深度为root的下一级.
    *  \param childLabel 需要进行向上查找的节点.
    *  \return 是否找到节点.
    */
    static TDF_Label getOwnerLabel(const TDF_Label& childLabel);

    /*! \fn static bool getLastEntry(const TDocStd_Document* doc, std::string& entry)
    *  \brief 获取次顶级节点中的最新节点的PID.
    *  \param doc 模型数据集指针.
    *  \param entry 找到节点的PID.
    *  \return 是否找到节点.
    */
    static bool getLastEntry(const TDocStd_Document* doc, std::string& entry);

    /*! \fn static std::string getLabelPID(const TDF_Label& label)
    *  \brief 获取节点的PID.
    *  \param label 目标节点.
    *  \return 是否成功进行操作.
    */
    static std::string getLabelPID(const TDF_Label& label);

    /*! \fn static TDF_LabelList findEdgeOwnerFaces(const TDF_Label& ownerLabel, const TopoDS_Shape& edge)
    *  \brief 递归遍历节点，查找到所有包含参考edge的face.
    *  \param ownerLabel 需要进行查找的节点.
    *  \param edge 参考的edge拓扑.
    *  \return 匹配的face链表.
    */
    static TDF_LabelList findEdgeOwnerFaces(const TDF_Label& ownerLabel, const TopoDS_Shape& edge);

    /*! \fn static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape, LabelInfo& info)
    *  \brief 在shapeMap中查找到与参考模型数据一致的节点，并返回节点信息. 不用map原生的find是因为在几何操作过程中记录下来的shape变化对于朝向的记录是不准确的.
    *  \param shapeMap 需要进行查找的map.
    *  \param shape 参考的shape拓扑.
    *  \param info 返回查找到的Label信息.
    *  \return 是否查找到节点.
    */
    static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape, LabelInfo& info);

    /*! \fn static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape)
    *  \brief 在shapeMap中查找到与参考模型数据一致的节点，不返回节点信息.
    *  \param shapeMap 需要进行查找的map.
    *  \param shape 参考的shape拓扑.
    *  \return 是否查找到节点.
    */
    static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape);

    /*! \fn static bool removeShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape)
    *  \brief 在shapeMap中查找到与参考模型数据一致的节点，在map中删除模型.
    *  \param shapeMap 需要进行查找的map.
    *  \param shape 参考的shape拓扑.
    *  \return 是否查找到节点.
    */
    static void removeShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape);

    /*! \fn static TDF_Label createNewChild(const TDF_Label& parent, const std::string& name)
    *  \brief 在parent下创建子节点，进行名称赋值，自带孙节点“property”.
    *  \param parent 需要进行子节点创建的节点.
    *  \param name 新节点名称.
    *  \return 是否成功创建.
    */
    static TDF_Label createNewChild(const TDF_Label& parent, const std::string& name);

    /*! \fn static TDF_Label createNewChild(const TDF_Label& parent, const std::string& name)
    *  \brief 在parent下创建子节点，自带孙节点“property”.
    *  \param parent 需要进行子节点创建的节点.
    *  \return 是否成功创建.
    */
    static TDF_Label createNewChild(const TDF_Label& parent);

    /*! \fn static void createLabelNameWithShapeType(TopoDS_Shape shape, const int& id, std::string& rootName)
    *  \brief 根据模型拓扑类型创建名称.
    *  \param shape 模型数据.
    *  \param id 模型序列.
    *  \param rootName 返回模型名称.
    */
	static void createLabelNameWithShapeType(TopoDS_Shape shape, const int& id, std::string& rootName);

    /*! \fn static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, int& value)
    *  \brief 根据属性名称获取固定节点下的int型属性.
    *  \param label 目标节点.
    *  \param propertyName 属性名称.
    *  \param value 返回的int型属性值.
    *  \return 是否成功操作.
    */
    static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, int& value);

    /*! \fn static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, std::string& value)
    *  \brief 根据属性名称获取固定节点下的string型属性.
    *  \param label 目标节点.
    *  \param propertyName 属性名称.
    *  \param value 返回的string型属性值.
    *  \return 是否成功操作.
    */
    static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, std::string& value);

    /*! \fn static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, double& value)
    *  \brief 根据属性名称获取固定节点下的double型属性.
    *  \param label 目标节点.
    *  \param propertyName 属性名称.
    *  \param value 返回的double型属性值.
    *  \return 是否成功操作.
    */
    static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, double& value);

    /*! \fn static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const int& value)
    *  \brief 在固定节点下添加int型属性.
    *  \param label 目标节点.
    *  \param propertyName 属性名称.
    *  \param value 需要设定的int型属性值.
    *  \return 是否成功操作.
    */
    static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const int& value);

    /*! \fn static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const std::string& value)
    *  \brief 在固定节点下添加string型属性.
    *  \param label 目标节点.
    *  \param propertyName 属性名称.
    *  \param value 需要设定的string型属性值.
    *  \return 是否成功操作.
    */
    static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const std::string& value);

    /*! \fn static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const double& value)
    *  \brief 在固定节点下添加double型属性.
    *  \param label 目标节点.
    *  \param propertyName 属性名称.
    *  \param value 需要设定的double型属性值.
    *  \return 是否成功操作.
    */
    static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const double& value);

    /*! \fn static bool removeLabelProperty(const TDF_Label& label, const std::string& propertyName)
    *  \brief 在节点下根据属性名称删除属性.
    *  \param label 目标节点.
    *  \param propertyName 属性名称.
    *  \return 是否成功操作.
    */
    static bool removeLabelProperty(const TDF_Label& label, const std::string& propertyName);

    /*! \fn static bool getLabelName(const TDF_Label& label, std::string& name)
    *  \brief 获取节点名称属性.
    *  \param label 目标节点.
    *  \param name 返回的名称属性值.
    *  \return 是否成功操作.
    */
    static bool getLabelName(const TDF_Label& label, std::string& name);

    /*! \fn static bool setLabelName(const TDF_Label& label, const std::string& name)
    *  \brief 设定节点名称属性.
    *  \param label 目标节点.
    *  \param name 名称属性值.
    *  \return 是否成功操作.
    */
    static bool setLabelName(const TDF_Label& label, const std::string& name);

    /*! \fn static bool removeLabelName(const TDF_Label& label)
    *  \brief 删除节点名称属性.
    *  \param label 目标节点.
    *  \return 是否成功操作.
    */
    static bool removeLabelName(const TDF_Label& label);

    /*! \fn static TopoDS_Shape getLabelShape(const TDF_Label& label)
    *  \brief 获取节点模型数据.
    *  \param label 目标节点.
    *  \return 返回查找到的模型数据.
    */
    static TopoDS_Shape getLabelShape(const TDF_Label& label);

    /*! \fn static void recursiveRenameChildLabelWithSameName(const TDF_Label& fatherLabel)
    *  \brief 接口已弃用.
    */
    static void recursiveRenameChildLabelWithSameName(const TDF_Label& fatherLabel);

    /*! \fn static void renameChildLabelWithSameName(const TDF_Label& fatherLabel)
    *  \brief 接口已弃用.
    */
    static void renameChildLabelWithSameName(const TDF_Label& fatherLabel);

    static bool findFatherLabelAccordingToType(TDF_Label currentlabel,
        TopAbs_ShapeEnum type, TDF_Label& findlabel);
};

#endif