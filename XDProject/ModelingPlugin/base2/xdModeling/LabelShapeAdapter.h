/*! \file LabelShapeAdapter.h
*   \brief LabelShapeAdapter���ͷ�ļ�
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
*   \brief LabelShapeAdapter�ദ��ڵ��ģ������֮���ƥ���ϵ������ģ�͵��롢�������㡢���ǵȡ����磬��������ǰ��������ɾ�����޸ĵ�������ԭ�ڵ���½ڵ��е�λ�á�
 */
class XDMODELING_API LabelShapeAdapter
{
public:
    /*! \fn LabelShapeAdapter()
    *  \brief LabelShapeAdapter�Ĺ��캯��.
    */
    LabelShapeAdapter();

    /*! \fn ~LabelShapeAdapter()
    *  \brief LabelShapeAdapter����������.
    */
    ~LabelShapeAdapter();

    /*! \fn void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const int& numBrothers, const bool& topLevel)
    *  \brief ��shape�ֽ�ɸ�����������˽ṹ�����ֲ㼶����label�ڵ��£���������ƣ����ڲ�������ȣ��ӿ�������.
    *  \param origShape ��Ҫ���й��˵�TopoDS_Shape����.
    *  \return ������ɹ��˵�TopoDS_Shape����.
    */
    void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const int& numBrothers, const bool& topLevel);

    /*! \fn static void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const std::string& name, const int& id)
    *  \brief ��shape�ֽ�ɸ�����������˽ṹ�����ֲ㼶����label�ڵ��£����ڻ������δ����ȡ�֧���Զ�����. �ӿ�������.
    *  \param origShape ��Ҫ���й��˵�TopoDS_Shape����.
    *  \return ������ɹ��˵�TopoDS_Shape����.
    */
    static void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const std::string& name, const int& id);

    /*! \fn static void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const std::string& name, const int& id)
    *  \brief ��ģ�����ݷֽ�ɸ�����������˽ṹ�����ֲ㼶���ؽڵ��£���ȥ�ء����ڻ������δ�����.
    *  \param label Ŀ��ڵ�.
    *  \param shape ��Ҫ�������ص�ģ������.
    */
    void allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape);

    /*! \fn static void replaceLabelPropertyWithName(const TDF_Label& sourceLabel, const TDF_Label& targetRootLabel)
    *  \brief ��������ģ���޸Ĳ���������Դ��ݣ��ӿ�������.
    */
	static void replaceLabelPropertyWithName(const TDF_Label& sourceLabel, const TDF_Label& targetRootLabel);

    /*! \fn static void replaceLabelWithChangedShape(const TDF_Label& label, const TopoDS_Shape& changedShape, const std::string& name, const int& id)
    *  \brief ��changedShape������������˽ṹ�滻label�ڵ��µ�shape����������ģ���޸Ĳ����ٴδ������ӿ�������.
    */
    static void replaceLabelWithChangedShape(const TDF_Label& label, const TopoDS_Shape& changedShape, const std::string& name, const int& id);

    /*! \fn static void replaceChildLabelWithCopiedShape(const TDF_Label& label, const TopoDS_Shape& copiedShape)
    *  \brief ��Ŀ��ģ�����ݽ������˽��������ݹ�ȶ�Ŀ��ڵ��¸��ӽڵ��ģ�����ݣ������һ���ľͽ����滻.
    *  \param label Ŀ��ڵ�.
    *  \param shape ��Ҫ�����滻��ģ������.
    */
    static void replaceChildLabelWithCopiedShape(const TDF_Label& label, const TopoDS_Shape& copiedShape);

    /*! \fn static void replaceChildLabelWithCopiedShape1(const TDF_Label& label, const TopoDS_Shape& copiedShape)
    *  \brief ��copiedShape������������˽ṹ�滻label�ڵ��µ�shape����TShapeΪ������ݣ��ӿ�������.
    */
    static void replaceChildLabelWithCopiedShape1(const TDF_Label& label, const TopoDS_Shape& copiedShape);

    /*! \fn static void replaceChildLabelWithCopiedShape2(const TDF_Label& label, const TopoDS_Shape& copiedShape)
    *  \brief ��copiedShape������������˽ṹ�滻label�ڵ��µ�shape����TShape��locationΪ������ݣ��ӿ�������.
    */
    static void replaceChildLabelWithCopiedShape2(const TDF_Label& label, const TopoDS_Shape& copiedShape);

    /*! \fn static void buildEntityAttributeName(const TDF_Label& orgLabel, const TDF_Label& curLabel, bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID)
    *  \brief Ϊ�ڵ��ģ�͸�ֵ�������ԣ��������ģ�;͹��������ƣ������ԭģ�ͽ����˱༭��̳�����.
    *  \param orgLabel ��ģ�͵Ľڵ�.
    *  \param curLabel �༭��ģ�ͽڵ�.
    *  \param bIsNewCreate ģ���Ƿ����½���.
    *  \param bIsCloneCS ģ���Ƿ��ǿ�����.
    *  \param bNeedCopyFaceName ģ������߱ߵ�����.
    *  \param iNextFaceID ģ�����ID.
    *  \param iNextEdgeID ģ�ͱߵ�ID.
    */
    static void buildEntityAttributeName(const TDF_Label& orgLabel, const TDF_Label& curLabel,
        bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID);

    /*! \fn void collectRelatedLabelAndShapes(const TDF_Label& label, const EnAdapatType& type, void* operation)
    *  \brief �ռ���������нڵ��ģ�������������༭��ɾ���ͱ�������Ϣ������������Ա.
    *  \param label Ŀ��ڵ�.
    *  \param type ��������.
    *  \param operation ���������ӣ����粼����������ӵ�.
    */
    void collectRelatedLabelAndShapes(const TDF_Label& label, const EnAdapatType& type, void* operation);
    
    /*! \fn void labelDiscreteToMap(const TDF_Label& label)
    *  \brief ȡ��label�й��ص����ˣ������뱾label���а󶨣����洢������. ���ӿڵݹ����.
    *  \param label ��Ҫ����Ľڵ�.
    */
    void labelDiscreteToMap(const TDF_Label& label);

    /*! \fn void collectRelatedLabelAndShapes(const TopoDS_Shape& targetShape, const EnAdapatType& type, void* operation)
    *  \brief ���ݲ�ͬ��ģ�Ͳ��������ӣ���ȡ���������е���ʷ׷�٣��ݹ�������˲�����������ʷ׷���е����ã������������ɡ��༭��ɾ�������˱��浽����.
    *  \param origShape ���в��ҵ���������.
    *  \param type ��������.
    *  \param operation ���������ӣ����粼����������ӵ�.
    */
    void collectRelatedLabelAndShapes(const TopoDS_Shape& origShape, const EnAdapatType& type, void* operation);

	/*! \fn void directRelateShapes(const TopoDS_Shape& origShape, const TopoDS_Shape& newShape)
	*  \brief �ڰ��������shell��compound���߲ü���ʱ����õĶ�Ӧlabel�ķ���.
	*  \param origShape ԭģ������.
	*  \param newShape ��ģ������.
	*/
	void directRelateShapes(const TopoDS_Shape& origShape, const TopoDS_Shape& newShape);

    /*! \fn void linkRelatedLabels(const TDF_Label& targetLabel)
    *  \brief ���ݳ�Ա���Ѿ������ģ�������������༭��ɾ���ͱ�������Ϣ���ݹ���������˵�Ŀ��ڵ�.
    *  \param targetLabel Ŀ��ڵ�.
    */
    void linkRelatedLabels(const TDF_Label& targetLabel);

    /*! \fn void linkRelatedLabels(const TDF_Label& targetLabel)
    *  \brief linkRelatedLabels�Ĳ�������. ����fuse��common��ģ�������Ҳ�����Ӧ�ĵ����. Ĭ��������һ��һƥ��.
    *  \param targetLabel Ŀ��ڵ�.
    */
    void labelLinkMakeup(const TDF_Label& targetLabel);

    /*! \fn void collectLabelShapes(const TDF_Label& label, const EnAdapatType& type, void* operation)
    *  \brief ���ݲ������ͣ��ռ����б༭ǰ���shape�����Լ���Ӧlabel���ӿ�������.
    */
    void collectLabelShapes(const TDF_Label& label, const EnAdapatType& type, void* operation);

    /*! \fn void collectLabelShapesGeneratedByEdgeAndVertex(TopoDS_Shape sourceShape, const EnAdapatType& type, void* operation)
    *  \brief ���ݲ������ͣ��ռ����б༭ǰ���shape�����Լ���Ӧlabel���ӿ�������.
    */
	void collectLabelShapesGeneratedByEdgeAndVertex(TopoDS_Shape sourceShape, const EnAdapatType& type, void* operation);

    /*! \fn void transferColorAndName4ImportingStep(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool)
    *  \brief ��ͨ��OCC��XDE��ʽ����step�ļ��õ������ݹ��ص�����̬����½ڵ���. �㼶�������ˣ��ݹ����.
    *  \param targetLabel Ŀ��ڵ�.
    *  \param shapeTool ��XDE�������е�ģ�Ͳ��ҹ���.
    */
    void transferColorAndName4ImportingStep(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool);

    /*! \fn void transferColorAndName4ImportingIges(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool)
    *  \brief ��ͨ��OCC��XDE��ʽ����iges�ļ��õ������ݹ��ص�����̬����½ڵ���. �㼶�������ˣ��ݹ����.
    *  \param targetLabel Ŀ��ڵ�.
    *  \param shapeTool ��XDE�������е�ģ�Ͳ��ҹ���.
    */
    void transferColorAndName4ImportingIges(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool);

    /*! \fn bool isSupportedType(TopoDS_Shape shape)
    *  \brief �ж��Ƿ�Ϊ֧�ֲ��ҹ�����Ϣ����������.
    *  \param shape Ŀ��ģ������.
    *  \return �����Ƿ�֧��
    */
    bool isSupportedType(TopoDS_Shape shape);

    /*! \fn std::string getName() const
    *  \brief ��ȡ��Աm_name.
    *  \return ��ȡ���ĳ�Աֵ.
    */
    std::string getName() const { return m_name; }

    /*! \fn void setName(std::string val)
    *  \brief �趨��Աm_name��ֵ.
    *  \param val ��Ҫ�趨��string����.
    */
    void setName(std::string val) { m_name = val; }

    /*! \fn TDF_Label getTopLabel() const
    *  \brief ��ȡ��Աm_topLabel.
    *  \return ��ȡ���ĳ�Աֵ.
    */
    TDF_Label getTopLabel() const { return m_topLabel; }

    /*! \fn void setTopLabel(TDF_Label val)
    *  \brief �趨��Աm_topLabel��ֵ.
    *  \param val ��Ҫ�趨��TDF_Label����.
    */
    void setTopLabel(TDF_Label val) { m_topLabel = val; }

    /*! \fn void clearRelatedShapes()
    *  \brief �����Ա�����ڼ�¼�༭��������������������Ϣ������.
    */
    void clearRelatedShapes() { m_modifiedShapes.Clear(); m_constantShapes.Clear(); m_generatedShapes.Clear(); m_shapeHistoryMap; }

    /*! \fn void addModifiedShapes(TopoDS_Shape shape, TDF_Label label)
    *  \brief �ڼ�¼���˱༭��Ϣ�ĳ�Ա������µ�ģ�����ݺͶ�Ӧ�ڵ�.
    *  \param shape ������ģ������.
    *  \param label ������ģ�����ݶ�Ӧ�Ľڵ�.
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