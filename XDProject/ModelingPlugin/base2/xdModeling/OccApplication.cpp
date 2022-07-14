#include "OccApplication.h"
#include "OccModel.h"
#include "OccAlgo.h"

#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>
#include <BinDrivers.hxx>
#include <XmlDrivers.hxx>

using namespace std;

OccApplication::OccApplication()
{
    m_app = new TDocStd_Application();
    BinDrivers::DefineFormat(m_app);
    XmlDrivers::DefineFormat(m_app);

	m_pCurrentModel = NULL;
}

OccApplication* OccApplication::instance()
{
	static OccApplication manager;
	return &manager;
}

OccApplication::~OccApplication()
{
}

OccModel* OccApplication::createNewDoc(int& id)
{
	Handle(TDocStd_Document) doc;
	m_app->NewDocument("BinOcaf", doc);

    doc->SetNestedTransactionMode(true);
	OccModel* newModel = new OccModel(opencascade::handle<TDocStd_Document>(doc).get());

    if (m_models.size() == 0)
    {
        id = 0;
    }
    else
    {
        id = (--m_models.end())->first;
    }
    m_models.insert(pair<int, OccModel*>(id, newModel));

	m_pCurrentModel = newModel;

	return newModel;
}

bool OccApplication::saveAs(const int& id, const std::string& path)
{
    map<int, OccModel*>::iterator iter = m_models.find(id);

    // OccModel not found
    if (iter == m_models.end())
    {
        return false;
    }

    OccModel* model = iter->second;

    TCollection_ExtendedString TPath;
    TPath.Copy(path.c_str());
    if (TPath.SearchFromEnd(".xml") > 0)
    {
        // The document must be saved in XML format
        model->getDoc()->ChangeStorageFormat("XmlOcaf");
    }
    else if (TPath.SearchFromEnd(".cbf") > 0)
    {
        // The document must be saved in binary format
        model->getDoc()->ChangeStorageFormat("BinOcaf");
    }

    PCDM_StoreStatus status;
    TCollection_ExtendedString theStatusMessage;
//     Standard_PCharacter ss;
    try
    {
        // Saves the document in the current application
        status = m_app->SaveAs(model->getDoc(), TPath, theStatusMessage);
//         theStatusMessage.ToUTF8CString(ss);
        if (status == PCDM_SS_OK)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
}

bool OccApplication::saveAs(OccModel* model, const std::string& path)
{
    if (!model)
    {
        return false;
    }

    TCollection_ExtendedString TPath;
    TPath.Copy(path.c_str());
    if (TPath.SearchFromEnd(".xml") > 0)
    {
        // The document must be saved in XML format
        model->getDoc()->ChangeStorageFormat("XmlOcaf");
    }
    else if (TPath.SearchFromEnd(".cbf") > 0)
    {
        // The document must be saved in binary format
        model->getDoc()->ChangeStorageFormat("BinOcaf");
    }

    PCDM_StoreStatus status;
    TCollection_ExtendedString theStatusMessage;
    //     Standard_PCharacter ss;
    try
    {
        // Saves the document in the current application
        status = m_app->SaveAs(model->getDoc(), TPath, theStatusMessage);
        //         theStatusMessage.ToUTF8CString(ss);
        if (status == PCDM_SS_OK)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
//     int id = -1;
//     for (map<int, OccModel*>::iterator iter = m_models.begin(); iter != m_models.end(); ++iter)
//     {
//         if (iter->second == model)
//         {
//             id = iter->first;
//             break;
//         }
//     }
//     if (id == -1)
//     {
//         return false;
//     }
//     else
//     {
//         return saveAs(id, path);
//     }
}

bool OccApplication::open(const std::string& path, int& id, OccModel* newModel)
{
    PCDM_ReaderStatus status;
    Handle(TDocStd_Document) doc;
    try
    {
        status = m_app->Open(path.c_str(), doc);
        if (status == PCDM_RS_OK)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    newModel = new OccModel(opencascade::handle<TDocStd_Document>(doc).get());

    if (m_models.size() == 0)
    {
        id = 0;
    }
    else
    {
        id = (--m_models.end())->first;
    }
    m_models.insert(pair<int, OccModel*>(id, newModel));
}

OccModel* OccApplication::getModel(const int& id) const
{
    map<int, OccModel*>::const_iterator iter = m_models.find(id);

    // OccModel not found
    if (iter == m_models.end())
    {
        return nullptr;
    }
    else
    {
        return iter->second;
    }
}

GKAlgo* OccApplication::getAlgo()  const
{
// 	return nullptr;
	return OccAlgo::Instance();
}
