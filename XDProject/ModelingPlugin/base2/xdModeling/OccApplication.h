/*! \file OccApplication.h
*   \brief OccApplication类的头文件
*   \author HST
*   \date 2020.10.8
*/
#ifndef OCCAPPLICATION_H
#define OCCAPPLICATION_H

#include "xdModelingHeader.h"
#include "GKModel.h"

#include <map>

class TDocStd_Application;
class OccModel;
class GKAlgo;

/*! \class OccApplication
*   \brief 用于创建唯一的OccApplication类（单例模式）。通过此类可以创建不同的OccModel，每个OccModel对应一个工程，可以单独进行模型管理操作。
 */
class XDMODELING_API OccApplication
{
public:
    /*! \fn static OccApplication* instance()
    *  \brief 获取OccApplication的静态指针.
    *  \return OccApplication的静态指针.
    */
	static OccApplication* instance();

    /*! \fn ~OccApplication()
    *  \brief OccApplication的析构函数.
    */
	~OccApplication();

    /*! \fn OccModel* createNewDoc(int& id)
    *  \brief 创建新的OccModel.
    *  \param id 返回新创建的OccModel的id，id是唯一的.
    *  \return 返回新创建的OccModel的指针.
    */
	OccModel* createNewDoc(int& id);

    /*! \fn bool saveAs(const int& id, const std::string& path)
    *  \brief 将OccModel的数据另存成外部文件.
    *  \param id OccModel对应的唯一id.
    *  \param path 需要保存的外部文件路径.
    *  \return 是否成功保存.
    */
    bool saveAs(const int& id, const std::string& path);

    /*! \fn bool saveAs(OccModel* model, const std::string& path)
    *  \brief 将OccModel的数据另存成外部文件.
    *  \param model OccModel的指针.
    *  \param path 需要保存的外部文件路径.
    *  \return 是否成功保存.
    */
    bool saveAs(OccModel* model, const std::string& path);

    /*! \fn bool open(const std::string& path, int& id, OccModel* newModel)
    *  \brief 打开外部文件到新的OccModel.
    *  \param path 需要打开的外部文件路径.
    *  \param id 返回新创建的OccModel的id，id是唯一的.
    *  \param newModel 返回新创建的OccModel的指针.
    *  \return 返回新创建的OccModel的指针.
    */
    bool open(const std::string& path, int& id, OccModel* newModel);

    /*! \fn OccModel* getModel(const int& id) const
    *  \brief 根据id获取OccModel.
    *  \param id OccModel的唯一标识.
    *  \return 查找到的OccModel的指针.
    */
    OccModel* getModel(const int& id) const;

    /*! \fn TDocStd_Application* getApp() const
    *  \brief 获取成员TDocStd_Application的指针.
    *  \return TDocStd_Application的指针.
    */
    TDocStd_Application* getApp() const { return m_app; }

    /*! \fn void setApp(TDocStd_Application* val)
    *  \brief 给成员变量赋值.
    *  \param val TDocStd_Application指针.
    */
    void setApp(TDocStd_Application* val) { m_app = val; }

    /*! \fn OccModel* getCurrentModel() const
    *  \brief 获取当前OccModel指针.
    *  \return 当前OccModel指针.
    */
	OccModel* getCurrentModel() const 
	{
		return m_pCurrentModel;
	}

    /*! \fn void setCurrentModel(GKModel* curModel)
    *  \brief 设定当前OccModel.
    *  \param curModel GKModel的指针，实际为OccModel.
    */
	void setCurrentModel(GKModel* curModel)
	{
		m_pCurrentModel = (OccModel*)curModel;
	}

	/*! \fn GKAlgo* getAlgo() const
	*  \brief 获取当前GKAlgo指针.
	*  \return 当前GKAlgo指针.
	*/
	GKAlgo* getAlgo() const;

protected:
    /*! \fn OccApplication()
    *  \brief OccApplication的构造函数.
    */
	OccApplication();

	TDocStd_Application* m_app;

    std::map<int, OccModel*> m_models;

	OccModel* m_pCurrentModel;
};

#ifdef TheOCCApp
#undef TheOCCApp
#endif 

#define  TheOCCApp (*(OccApplication::instance()))

#endif