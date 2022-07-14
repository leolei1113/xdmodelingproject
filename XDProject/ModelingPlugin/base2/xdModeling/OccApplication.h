/*! \file OccApplication.h
*   \brief OccApplication���ͷ�ļ�
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
*   \brief ���ڴ���Ψһ��OccApplication�ࣨ����ģʽ����ͨ��������Դ�����ͬ��OccModel��ÿ��OccModel��Ӧһ�����̣����Ե�������ģ�͹��������
 */
class XDMODELING_API OccApplication
{
public:
    /*! \fn static OccApplication* instance()
    *  \brief ��ȡOccApplication�ľ�ָ̬��.
    *  \return OccApplication�ľ�ָ̬��.
    */
	static OccApplication* instance();

    /*! \fn ~OccApplication()
    *  \brief OccApplication����������.
    */
	~OccApplication();

    /*! \fn OccModel* createNewDoc(int& id)
    *  \brief �����µ�OccModel.
    *  \param id �����´�����OccModel��id��id��Ψһ��.
    *  \return �����´�����OccModel��ָ��.
    */
	OccModel* createNewDoc(int& id);

    /*! \fn bool saveAs(const int& id, const std::string& path)
    *  \brief ��OccModel�����������ⲿ�ļ�.
    *  \param id OccModel��Ӧ��Ψһid.
    *  \param path ��Ҫ������ⲿ�ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool saveAs(const int& id, const std::string& path);

    /*! \fn bool saveAs(OccModel* model, const std::string& path)
    *  \brief ��OccModel�����������ⲿ�ļ�.
    *  \param model OccModel��ָ��.
    *  \param path ��Ҫ������ⲿ�ļ�·��.
    *  \return �Ƿ�ɹ�����.
    */
    bool saveAs(OccModel* model, const std::string& path);

    /*! \fn bool open(const std::string& path, int& id, OccModel* newModel)
    *  \brief ���ⲿ�ļ����µ�OccModel.
    *  \param path ��Ҫ�򿪵��ⲿ�ļ�·��.
    *  \param id �����´�����OccModel��id��id��Ψһ��.
    *  \param newModel �����´�����OccModel��ָ��.
    *  \return �����´�����OccModel��ָ��.
    */
    bool open(const std::string& path, int& id, OccModel* newModel);

    /*! \fn OccModel* getModel(const int& id) const
    *  \brief ����id��ȡOccModel.
    *  \param id OccModel��Ψһ��ʶ.
    *  \return ���ҵ���OccModel��ָ��.
    */
    OccModel* getModel(const int& id) const;

    /*! \fn TDocStd_Application* getApp() const
    *  \brief ��ȡ��ԱTDocStd_Application��ָ��.
    *  \return TDocStd_Application��ָ��.
    */
    TDocStd_Application* getApp() const { return m_app; }

    /*! \fn void setApp(TDocStd_Application* val)
    *  \brief ����Ա������ֵ.
    *  \param val TDocStd_Applicationָ��.
    */
    void setApp(TDocStd_Application* val) { m_app = val; }

    /*! \fn OccModel* getCurrentModel() const
    *  \brief ��ȡ��ǰOccModelָ��.
    *  \return ��ǰOccModelָ��.
    */
	OccModel* getCurrentModel() const 
	{
		return m_pCurrentModel;
	}

    /*! \fn void setCurrentModel(GKModel* curModel)
    *  \brief �趨��ǰOccModel.
    *  \param curModel GKModel��ָ�룬ʵ��ΪOccModel.
    */
	void setCurrentModel(GKModel* curModel)
	{
		m_pCurrentModel = (OccModel*)curModel;
	}

	/*! \fn GKAlgo* getAlgo() const
	*  \brief ��ȡ��ǰGKAlgoָ��.
	*  \return ��ǰGKAlgoָ��.
	*/
	GKAlgo* getAlgo() const;

protected:
    /*! \fn OccApplication()
    *  \brief OccApplication�Ĺ��캯��.
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