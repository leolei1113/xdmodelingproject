#pragma once

#include "xdModelingHeader.h"

class AcisModel;
class GKModel;
class GKAlgo;

class XDMODELING_API AcisApplication
{
public:
	static AcisApplication* instance();

	~AcisApplication();

	AcisModel* createNewDoc(int& id);

	// 获取当前OccModel
	AcisModel* getCurrentModel() const
	{
		return m_pCurrentModel;
	}

	void setCurrentModel(GKModel* curModel)
	{
		m_pCurrentModel = (AcisModel*)curModel;
	}

	GKAlgo* getAlgo() const;

protected:
	AcisApplication();

	std::map<int, AcisModel*>* m_models;

	AcisModel* m_pCurrentModel;
};

#ifdef TheACISApp
#undef TheACISApp
#endif 

#define  TheACISApp (*(AcisApplication::instance()))

