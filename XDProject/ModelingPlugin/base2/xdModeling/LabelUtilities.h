/*! \file LabelUtilities.h
*   \brief LabelUtilities���ͷ�ļ�
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
*   \brief ���ڴ��һЩ����OCAF��Label����ľ�̬����
 */
class XDMODELING_API LabelUtilities
{
public:
    /*! \fn LabelUtilities()
    *  \brief LabelUtilities�Ĺ��캯��.
    */
    LabelUtilities();

    /*! \fn ~LabelUtilities()
    *  \brief LabelUtilities����������.
    */
    ~LabelUtilities();

    /*! \fn static bool copyLabelWithoutFather(const TDF_Label& source, const TDF_Label& target)
    *  \brief ����ԭLabel�ڵ��������ӽڵ����ݵ��µ�Label�ڵ���.
    *  \param source ��Ҫ���п����Ľڵ�.
    *  \param target Ŀ��ڵ�.
    *  \return �Ƿ�ɹ����в���.
    */
    static bool copyLabelWithoutFather(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelWithFather(const TDF_Label& source, const TDF_Label& target)
    *  \brief ����ԭLabel�ڵ㼰�ӽڵ�����ݵ��µ�Label�ڵ���.
    *  \param source ��Ҫ���п����Ľڵ�.
    *  \param target Ŀ��ڵ�.
    *  \return �Ƿ�ɹ����в���.
    */
    static bool copyLabelWithFather(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelWithFather(const TDocStd_Document* doc, const std::string& sourceEntry, const std::string& targetEntry)
    *  \brief ����ԭLabel�ڵ㼰�ӽڵ�����ݵ��µ�Label�ڵ��ϣ��ڵ�ͨ��PID���в���.
    *  \param doc ���ݼ�ָ��.
    *  \param sourceEntry ��Ҫ���п����Ľڵ��PID.
    *  \param targetEntry Ŀ��ڵ��PID.
    *  \return �Ƿ�ɹ����в���.
    */
    static bool copyLabelWithFather(const TDocStd_Document* doc, const std::string& sourceEntry, const std::string& targetEntry);

    /*! \fn static bool copyLabelWithOnlyPropertyChild(const TDF_Label& source, const TDF_Label& target)
    *  \brief ����ԭLabel�ڵ�������Ϊ��property�����ӽڵ����ݵ��µ�Label�ڵ��ϣ��ӿ�������.
    *  \param source ��Ҫ���п����Ľڵ�.
    *  \param target Ŀ��ڵ�.
    *  \return �Ƿ�ɹ����в���.
    */
    static bool copyLabelWithOnlyPropertyChild(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelProperty(const TDF_Label& source, const TDF_Label& target)
    *  \brief ����ԭLabel�ڵ�������Ϊ��property�����ӽڵ����ݵ��µ�Label�ڵ���.
    *  \param source ��Ҫ���п����Ľڵ�.
    *  \param target Ŀ��ڵ�.
    *  \return �Ƿ�ɹ����в���.
    */
    static bool copyLabelProperty(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool copyLabelName(const TDF_Label& source, const TDF_Label& target)
    *  \brief ����ԭLabel�ڵ����Ƶ��µ�Label�ڵ���.
    *  \param source ��Ҫ���п����Ľڵ�.
    *  \param target Ŀ��ڵ�.
    *  \return �Ƿ�ɹ����в���.
    */
    static bool copyLabelName(const TDF_Label& source, const TDF_Label& target);

    /*! \fn static bool findShapeLabel(const TDF_Label& label, const TopoDS_Shape& compareShape, TDF_Label& returnLabel)
    *  \brief �Ӹ��ڵ�ݹ�������ݽڵ㣬�����Ƿ�����ͬ��ģ�����ݣ����򷵻��ҵ��Ľڵ�.
    *  \param label ��Ҫ���в��ҵĽڵ�.
    *  \param compareShape ��Ҫ���в��ҵ�ģ�����ݶ���.
    *  \param returnLabel �����ҵ��Ľڵ�.
    *  \return �Ƿ��ҵ��ڵ�.
    */
    static bool findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, TDF_Label& returnLabel);

    /*! \fn static bool findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, TDF_Label& returnLabel)
    *  \brief �Ӹ��ڵ�ݹ�������ݽڵ㣬�����Ƿ�����ͬ��ģ�����ݣ��������ҵ��Ľڵ�.
    *  \param label ��Ҫ���в��ҵĽڵ�.
    *  \param compareShape ��Ҫ���в��ҵ�ģ�����ݶ���.
    *  \return �Ƿ��ҵ��ڵ�.
    */
    static bool findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape);

    /*! \fn static bool findShapePIDFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, std::string& strPID)
    *  \brief �Ӹ��ڵ�ݹ�������ݽڵ㣬�����Ƿ�����ͬ��ģ�����ݣ����򷵻��ҵ��Ľڵ��PID.
    *  \param label ��Ҫ���в��ҵĽڵ�.
    *  \param compareShape ��Ҫ���в��ҵ�ģ�����ݶ���.
    *  \param returnLabel �����ҵ��Ľڵ��PID.
    *  \return �Ƿ��ҵ��ڵ�.
    */
    static bool findShapePIDFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, std::string& strPID);

	/*! \fn static bool findSubShapePIDsFromRoot(const TDF_Label& label, const TopAbs_ShapeEnum& shapeType, std::vector<std::string>& strPIDs)
	*  \brief �Ӹ��ڵ�ݹ�������ݽڵ㣬����������shapeType��ͬ�������˵�PIDD.
	*  \param label ��Ҫ���в��ҵĽڵ�.
	*  \param shapeType ��׼��������.
	*  \param strPIDs �����ҵ��Ľڵ��PID.
	*  \return �Ƿ��ҵ��ڵ�.
	*/
	static bool findSubShapePIDsFromRoot(const TDF_Label& label, const TopAbs_ShapeEnum& shapeType,
		std::vector<std::string>& strPIDs);

    /*! \fn static bool findNameLabel(const TDF_Label& sourceRootLabel, const std::string& name, TDF_Label& retuanLabel)
    *  \brief �ݹ�������ݽڵ㣬�����Ƿ�����ͬ�����ƣ����򷵻��ҵ��Ľڵ�.
    *  \param label ��Ҫ���в��ҵĽڵ�.
    *  \param name ��Ҫ���в��ҵ�ģ�����ݶ���.
    *  \param returnLabel �����ҵ��Ľڵ�.
    *  \return �Ƿ��ҵ��ڵ�.
    */
	static bool findNameLabel(const TDF_Label& label, const std::string& name, TDF_Label& retuanLabel);

    /*! \fn static TDF_Label getOwnerLabel(const TDF_Label& childLabel)
    *  \brief ��ȡchildLabeld�Ĵζ����ڵ㣬���Ϊroot����һ��.
    *  \param childLabel ��Ҫ�������ϲ��ҵĽڵ�.
    *  \return �Ƿ��ҵ��ڵ�.
    */
    static TDF_Label getOwnerLabel(const TDF_Label& childLabel);

    /*! \fn static bool getLastEntry(const TDocStd_Document* doc, std::string& entry)
    *  \brief ��ȡ�ζ����ڵ��е����½ڵ��PID.
    *  \param doc ģ�����ݼ�ָ��.
    *  \param entry �ҵ��ڵ��PID.
    *  \return �Ƿ��ҵ��ڵ�.
    */
    static bool getLastEntry(const TDocStd_Document* doc, std::string& entry);

    /*! \fn static std::string getLabelPID(const TDF_Label& label)
    *  \brief ��ȡ�ڵ��PID.
    *  \param label Ŀ��ڵ�.
    *  \return �Ƿ�ɹ����в���.
    */
    static std::string getLabelPID(const TDF_Label& label);

    /*! \fn static TDF_LabelList findEdgeOwnerFaces(const TDF_Label& ownerLabel, const TopoDS_Shape& edge)
    *  \brief �ݹ�����ڵ㣬���ҵ����а����ο�edge��face.
    *  \param ownerLabel ��Ҫ���в��ҵĽڵ�.
    *  \param edge �ο���edge����.
    *  \return ƥ���face����.
    */
    static TDF_LabelList findEdgeOwnerFaces(const TDF_Label& ownerLabel, const TopoDS_Shape& edge);

    /*! \fn static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape, LabelInfo& info)
    *  \brief ��shapeMap�в��ҵ���ο�ģ������һ�µĽڵ㣬�����ؽڵ���Ϣ. ����mapԭ����find����Ϊ�ڼ��β��������м�¼������shape�仯���ڳ���ļ�¼�ǲ�׼ȷ��.
    *  \param shapeMap ��Ҫ���в��ҵ�map.
    *  \param shape �ο���shape����.
    *  \param info ���ز��ҵ���Label��Ϣ.
    *  \return �Ƿ���ҵ��ڵ�.
    */
    static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape, LabelInfo& info);

    /*! \fn static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape)
    *  \brief ��shapeMap�в��ҵ���ο�ģ������һ�µĽڵ㣬�����ؽڵ���Ϣ.
    *  \param shapeMap ��Ҫ���в��ҵ�map.
    *  \param shape �ο���shape����.
    *  \return �Ƿ���ҵ��ڵ�.
    */
    static bool findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape);

    /*! \fn static bool removeShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape)
    *  \brief ��shapeMap�в��ҵ���ο�ģ������һ�µĽڵ㣬��map��ɾ��ģ��.
    *  \param shapeMap ��Ҫ���в��ҵ�map.
    *  \param shape �ο���shape����.
    *  \return �Ƿ���ҵ��ڵ�.
    */
    static void removeShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape);

    /*! \fn static TDF_Label createNewChild(const TDF_Label& parent, const std::string& name)
    *  \brief ��parent�´����ӽڵ㣬�������Ƹ�ֵ���Դ���ڵ㡰property��.
    *  \param parent ��Ҫ�����ӽڵ㴴���Ľڵ�.
    *  \param name �½ڵ�����.
    *  \return �Ƿ�ɹ�����.
    */
    static TDF_Label createNewChild(const TDF_Label& parent, const std::string& name);

    /*! \fn static TDF_Label createNewChild(const TDF_Label& parent, const std::string& name)
    *  \brief ��parent�´����ӽڵ㣬�Դ���ڵ㡰property��.
    *  \param parent ��Ҫ�����ӽڵ㴴���Ľڵ�.
    *  \return �Ƿ�ɹ�����.
    */
    static TDF_Label createNewChild(const TDF_Label& parent);

    /*! \fn static void createLabelNameWithShapeType(TopoDS_Shape shape, const int& id, std::string& rootName)
    *  \brief ����ģ���������ʹ�������.
    *  \param shape ģ������.
    *  \param id ģ������.
    *  \param rootName ����ģ������.
    */
	static void createLabelNameWithShapeType(TopoDS_Shape shape, const int& id, std::string& rootName);

    /*! \fn static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, int& value)
    *  \brief �����������ƻ�ȡ�̶��ڵ��µ�int������.
    *  \param label Ŀ��ڵ�.
    *  \param propertyName ��������.
    *  \param value ���ص�int������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, int& value);

    /*! \fn static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, std::string& value)
    *  \brief �����������ƻ�ȡ�̶��ڵ��µ�string������.
    *  \param label Ŀ��ڵ�.
    *  \param propertyName ��������.
    *  \param value ���ص�string������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, std::string& value);

    /*! \fn static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, double& value)
    *  \brief �����������ƻ�ȡ�̶��ڵ��µ�double������.
    *  \param label Ŀ��ڵ�.
    *  \param propertyName ��������.
    *  \param value ���ص�double������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool getLabelProperty(const TDF_Label& label, const std::string& propertyName, double& value);

    /*! \fn static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const int& value)
    *  \brief �ڹ̶��ڵ������int������.
    *  \param label Ŀ��ڵ�.
    *  \param propertyName ��������.
    *  \param value ��Ҫ�趨��int������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const int& value);

    /*! \fn static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const std::string& value)
    *  \brief �ڹ̶��ڵ������string������.
    *  \param label Ŀ��ڵ�.
    *  \param propertyName ��������.
    *  \param value ��Ҫ�趨��string������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const std::string& value);

    /*! \fn static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const double& value)
    *  \brief �ڹ̶��ڵ������double������.
    *  \param label Ŀ��ڵ�.
    *  \param propertyName ��������.
    *  \param value ��Ҫ�趨��double������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool addLabelProperty(const TDF_Label& label, const std::string& propertyName, const double& value);

    /*! \fn static bool removeLabelProperty(const TDF_Label& label, const std::string& propertyName)
    *  \brief �ڽڵ��¸�����������ɾ������.
    *  \param label Ŀ��ڵ�.
    *  \param propertyName ��������.
    *  \return �Ƿ�ɹ�����.
    */
    static bool removeLabelProperty(const TDF_Label& label, const std::string& propertyName);

    /*! \fn static bool getLabelName(const TDF_Label& label, std::string& name)
    *  \brief ��ȡ�ڵ���������.
    *  \param label Ŀ��ڵ�.
    *  \param name ���ص���������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool getLabelName(const TDF_Label& label, std::string& name);

    /*! \fn static bool setLabelName(const TDF_Label& label, const std::string& name)
    *  \brief �趨�ڵ���������.
    *  \param label Ŀ��ڵ�.
    *  \param name ��������ֵ.
    *  \return �Ƿ�ɹ�����.
    */
    static bool setLabelName(const TDF_Label& label, const std::string& name);

    /*! \fn static bool removeLabelName(const TDF_Label& label)
    *  \brief ɾ���ڵ���������.
    *  \param label Ŀ��ڵ�.
    *  \return �Ƿ�ɹ�����.
    */
    static bool removeLabelName(const TDF_Label& label);

    /*! \fn static TopoDS_Shape getLabelShape(const TDF_Label& label)
    *  \brief ��ȡ�ڵ�ģ������.
    *  \param label Ŀ��ڵ�.
    *  \return ���ز��ҵ���ģ������.
    */
    static TopoDS_Shape getLabelShape(const TDF_Label& label);

    /*! \fn static void recursiveRenameChildLabelWithSameName(const TDF_Label& fatherLabel)
    *  \brief �ӿ�������.
    */
    static void recursiveRenameChildLabelWithSameName(const TDF_Label& fatherLabel);

    /*! \fn static void renameChildLabelWithSameName(const TDF_Label& fatherLabel)
    *  \brief �ӿ�������.
    */
    static void renameChildLabelWithSameName(const TDF_Label& fatherLabel);

    static bool findFatherLabelAccordingToType(TDF_Label currentlabel,
        TopAbs_ShapeEnum type, TDF_Label& findlabel);

    static bool FindTypeLevelFatherLabel(TDF_Label& inputlabel, TopAbs_ShapeEnum type, TopoDS_Shape& currentshape);

};

#endif