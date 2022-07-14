/*! \file IOShapeBuilder.h
*   \brief IOShapeBuilder���ͷ�ļ�
*   \author HST
*   \date 2020.10.8
*/

#ifndef IOSHAPEBUILDER_H
#define IOSHAPEBUILDER_H

#include "OccShapeBuilder.h"

#include <TopTools_HArray1OfShape.hxx>
#include <TDF_Label.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_LabelList.hxx>

#include <vector>

/*! \class IOShapeBuilder
*   \brief ����ģ�����ݵ��뵼���ɳ���ģ���ļ�����
 */
class IOShapeBuilder :
    public OccShapeBuilder
{
    /*! \enum IOShapeBuilder::FileType
    * ģ���ļ�����
    */
    enum FileType
    {
        IGES,
        STEP
    };
public:
    /*! \fn IOShapeBuilder()
    *  \brief IOShapeBuilder�Ĺ��캯��.
    */
    IOShapeBuilder();
    /*! \fn ~IOShapeBuilder()
    *  \brief IOShapeBuilder����������.
    */
    ~IOShapeBuilder();

    /*! \fn TopTools_HArray1OfShape getShapeArray() const
    *  \brief ��ȡ��Աm_shapeArray.
    *  \return ��ȡ���ĳ�Աֵ.
    */
    TopTools_HArray1OfShape getShapeArray() const { return m_shapeArray; }

    /*! \fn void setShapeArray(TopTools_HArray1OfShape val)
    *  \brief �趨��Աm_shapeArray��ֵ.
    *  \param val ��Ҫ�趨��TopTools_HArray1OfShape����.
    */
    void setShapeArray(TopTools_HArray1OfShape val) { m_shapeArray = val; }

    /*! \fn std::vector<void *> getEntityList() const
    *  \brief ��ȡ��Աm_entityList.
    *  \return ��ȡ���ĳ�Աֵ.
    */
    std::vector<void *> getEntityList() const { return m_entityList; }

    /*! \fn void setEntityList(std::vector<void *> val)
    *  \brief �趨��Աm_entityList��ֵ.
    *  \param val ��Ҫ�趨��ģ������ָ������.
    */
    void setEntityList(std::vector<void *> val) { m_entityList = val; }

    /*! \fn TDF_LabelList getExpLabelList() const
    *  \brief ��ȡ��Աm_expLabelList.
    *  \return ��ȡ���ĳ�Աֵ.
    */
    TDF_LabelList getExpLabelList() const { return m_expLabelList; }

    /*! \fn void setExpLabelList(TDF_LabelList val)
    *  \brief �趨��Աm_expLabelList��ֵ.
    *  \param val ��Ҫ�趨��label����.
    */
    void setExpLabelList(TDF_LabelList val) { m_expLabelList = val; }

public:
    /*! \fn bool importIGESModelViaXDE(const std::string& path)
    *  \brief ͨ��XDEģʽ����IGESģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool importIGESModelViaXDE(const std::string& path);

    /*! \fn bool importIGESModel(const std::string& path)
    *  \brief ����IGESģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool importIGESModel(const std::string& path);


    // ����IGESģ���ļ�
    /*! \fn bool exportIGESModel(const std::string& path)
    *  \brief ����IGESģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool exportIGESModel(const std::string& path);

    /*! \fn bool importSTEPModelViaXDE(const std::string& path)
    *  \brief ͨ��XDEģʽ����STEPģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool importSTEPModelViaXDE(const std::string& path);

    /*! \fn bool importSTEPModel(const std::string& path)
    *  \brief ����STEPģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool importSTEPModel(const std::string& path);

    /*! \fn bool exportSTEPModelViaXDE(const std::string& path)
    *  \brief ͨ��XDEģʽ����STEPģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool exportSTEPModelViaXDE(const std::string& path);

    /*! \fn bool exportSTEPModel(const std::string& path)
    *  \brief ����STEPģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool exportSTEPModel(const std::string& path);

    /*! \fn bool importBREPModel(const std::string& path)
    *  \brief ����BREPģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool importBREPModel(const std::string& path);

    /*! \fn bool exportBREPModel(const std::string& path)
    *  \brief ����BREPģ���ļ�.
    *  \param path �ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool exportBREPModel(const std::string& path);

protected:
    /*! \fn void transferSolidLevelShapeToLabel(TopoDS_Shape shape, TDF_Label rootLabel, TopLoc_Location locationBefore, Handle(XCAFDoc_ShapeTool) shapeTool)
    *  \brief ��ͨ��OCC��XDE��ʽ����step��iges�ļ��õ�������ת���ɱ���̬���ģ�����ݽṹ���㼶�������ˣ��ݹ����.
    *  \param shape Ŀ��ģ�����ݶ���.
    *  \param rootLabel ��Ҫ���صı���̬�����ݽڵ�.
    *  \param locationBefore ģ��֮ǰ��λ�ã�Ŀǰ��������Ч.
    *  \param shapeTool ��XDE�������е�ģ�Ͳ��ҹ���.
    *  \return �Ƿ�ɹ�����.
    */
    void transferSolidLevelShapeToLabel(TopoDS_Shape shape, TDF_Label rootLabel, TopLoc_Location locationBefore, Handle(XCAFDoc_ShapeTool) shapeTool);

protected:
    //import
    TopTools_HArray1OfShape                         m_shapeArray;
    std::vector<void*>                              m_entityList;

    //export
    TDF_LabelList                                   m_expLabelList;

    FileType                                        m_fileType;
};

#endif
