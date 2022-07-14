/*! \file IOShapeBuilder.h
*   \brief IOShapeBuilder类的头文件
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
*   \brief 用于模型数据导入导出成常用模型文件的类
 */
class IOShapeBuilder :
    public OccShapeBuilder
{
    /*! \enum IOShapeBuilder::FileType
    * 模型文件类型
    */
    enum FileType
    {
        IGES,
        STEP
    };
public:
    /*! \fn IOShapeBuilder()
    *  \brief IOShapeBuilder的构造函数.
    */
    IOShapeBuilder();
    /*! \fn ~IOShapeBuilder()
    *  \brief IOShapeBuilder的析构函数.
    */
    ~IOShapeBuilder();

    /*! \fn TopTools_HArray1OfShape getShapeArray() const
    *  \brief 获取成员m_shapeArray.
    *  \return 获取到的成员值.
    */
    TopTools_HArray1OfShape getShapeArray() const { return m_shapeArray; }

    /*! \fn void setShapeArray(TopTools_HArray1OfShape val)
    *  \brief 设定成员m_shapeArray的值.
    *  \param val 需要设定的TopTools_HArray1OfShape对象.
    */
    void setShapeArray(TopTools_HArray1OfShape val) { m_shapeArray = val; }

    /*! \fn std::vector<void *> getEntityList() const
    *  \brief 获取成员m_entityList.
    *  \return 获取到的成员值.
    */
    std::vector<void *> getEntityList() const { return m_entityList; }

    /*! \fn void setEntityList(std::vector<void *> val)
    *  \brief 设定成员m_entityList的值.
    *  \param val 需要设定的模型数据指针序列.
    */
    void setEntityList(std::vector<void *> val) { m_entityList = val; }

    /*! \fn TDF_LabelList getExpLabelList() const
    *  \brief 获取成员m_expLabelList.
    *  \return 获取到的成员值.
    */
    TDF_LabelList getExpLabelList() const { return m_expLabelList; }

    /*! \fn void setExpLabelList(TDF_LabelList val)
    *  \brief 设定成员m_expLabelList的值.
    *  \param val 需要设定的label链表.
    */
    void setExpLabelList(TDF_LabelList val) { m_expLabelList = val; }

public:
    /*! \fn bool importIGESModelViaXDE(const std::string& path)
    *  \brief 通过XDE模式导入IGES模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导入.
    */
    bool importIGESModelViaXDE(const std::string& path);

    /*! \fn bool importIGESModel(const std::string& path)
    *  \brief 导入IGES模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导入.
    */
    bool importIGESModel(const std::string& path);


    // 导出IGES模型文件
    /*! \fn bool exportIGESModel(const std::string& path)
    *  \brief 导出IGES模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导出.
    */
    bool exportIGESModel(const std::string& path);

    /*! \fn bool importSTEPModelViaXDE(const std::string& path)
    *  \brief 通过XDE模式导入STEP模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导入.
    */
    bool importSTEPModelViaXDE(const std::string& path);

    /*! \fn bool importSTEPModel(const std::string& path)
    *  \brief 导入STEP模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导入.
    */
    bool importSTEPModel(const std::string& path);

    /*! \fn bool exportSTEPModelViaXDE(const std::string& path)
    *  \brief 通过XDE模式导出STEP模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导出.
    */
    bool exportSTEPModelViaXDE(const std::string& path);

    /*! \fn bool exportSTEPModel(const std::string& path)
    *  \brief 导出STEP模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导出.
    */
    bool exportSTEPModel(const std::string& path);

    /*! \fn bool importBREPModel(const std::string& path)
    *  \brief 导入BREP模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导入.
    */
    bool importBREPModel(const std::string& path);

    /*! \fn bool exportBREPModel(const std::string& path)
    *  \brief 导出BREP模型文件.
    *  \param path 文件路径.
    *  \return 是否成功导出.
    */
    bool exportBREPModel(const std::string& path);

protected:
    /*! \fn void transferSolidLevelShapeToLabel(TopoDS_Shape shape, TDF_Label rootLabel, TopLoc_Location locationBefore, Handle(XCAFDoc_ShapeTool) shapeTool)
    *  \brief 将通过OCC的XDE方式导入step和iges文件得到的数据转化成本动态库的模型数据结构，层级解析拓扑，递归调用.
    *  \param shape 目标模型数据对象.
    *  \param rootLabel 需要挂载的本动态库数据节点.
    *  \param locationBefore 模型之前的位置，目前本参数无效.
    *  \param shapeTool 在XDE的数据中的模型查找工具.
    *  \return 是否成功导出.
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
