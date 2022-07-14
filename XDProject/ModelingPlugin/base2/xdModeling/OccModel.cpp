#include "OccModel.h"
#include "LabelUtilities.h"
#include "ShapeUtilities.h"
#include "LabelShapeAdapter.h"
#include "IOShapeBuilder.h"
#include "BooleanShapeBuilder.h"

#include <BRep_Builder.hxx>
#include <TDF_Data.hxx>
#include <TDF_Label.hxx>
#include <TDF_TagSource.hxx>
#include <TopoDS_Shape.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_DeltaOnAddition.hxx>
#include <TDF_DeltaOnForget.hxx>
#include <TDF_DeltaOnRemoval.hxx>
#include <TNaming_NamedShape.hxx>
#include <TDF_AttributeDeltaList.hxx>
#include <TDF_DeltaList.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TNaming_DeltaOnRemoval.hxx>
#include <TDF_DefaultDeltaOnRemoval.hxx>
#include <TNaming_UsedShapes.hxx>
#include <TNaming_Tool.hxx>
#include <TNaming_DeltaOnModification.hxx>
#include <TNaming_DeltaOnRemoval.hxx>
#include <TopTools_HArray1OfShape.hxx>
#include <TDF_Tool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_Real.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TNaming_Builder.hxx>
#include <TDocStd_XLinkTool.hxx>
#include <TDF_CopyTool.hxx>
#include <TDF_CopyLabel.hxx>
#include <TopoDS_HShape.hxx>
#include <TDocStd_Application.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <CDF_Session.hxx>
#include <CDF_DirectoryIterator.hxx>
#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wireframe.hxx>

#include "xd.h"
#include "OccApplication.h"
#include "OCCFixTool.h"

#ifdef WIN32
#include "corecrt_io.h"
#else
#include "sys/io.h"
#endif
/*! \class OccModelData
*   \brief 用于存放OccModel的成员及提供相应接口.
 */
class OccModelData
{
public:
    /*! \fn ~OccModelData()
    *  \brief OccModelData的析构函数.
    */
    ~OccModelData()
    {
        clearAddedShapeList();
        clearDeletedShapeList();
        clearShapeCacheList();
    }

    /*! \fn clearAddedShapeList()
    *  \brief 清除成员中addedShapeList.
    */
    void clearAddedShapeList()
    {
        for (std::list<void*>::iterator iter = addedShapeList.begin(); iter != addedShapeList.end(); ++iter)
        {
            static_cast<TopoDS_HShape*>(*iter)->Delete();
        }
        addedShapeList.clear();
    }

    /*! \fn clearDeletedShapeList()
    *  \brief 清除成员中deletedShapeList.
    */
    void clearDeletedShapeList()
    {
        for (std::list<void*>::iterator iter = deletedShapeList.begin(); iter != deletedShapeList.end(); ++iter)
        {
            static_cast<TopoDS_HShape*>(*iter)->Delete();
        }
        deletedShapeList.clear();
    }

    /*! \fn clearShapeCacheList()
    *  \brief 清除成员中shapeCache.
    */    void clearShapeCacheList()
    {
        for (std::map<std::string, TopoDS_HShape*>::iterator iter = shapeCache.begin(); iter != shapeCache.end(); ++iter)
        {
            iter->second->Delete();
        }
        shapeCache.clear();
    }

public:
    TDocStd_Document*	doc;

    //undo, redo期间顶层shape的变化情况
    std::list<void*> addedShapeList;//TopoDS_HShape*
    std::list<void*> deletedShapeList;

    //存储当前所有shape，包括子节点
    std::map<std::string, TopoDS_HShape*> shapeCache;
};

//recursive add shape
void addShapeCacheRecursive(const TDF_Label& parentLabel, std::map<std::string, TopoDS_HShape*>& shapeCache, void*& topEntity, const bool& topLevel = false)
{
    try
    {
        Handle(TNaming_NamedShape) ObjectNS;
        TCollection_AsciiString entryInstance;
        TDF_Tool::Entry(parentLabel, entryInstance);
        if (parentLabel.FindAttribute(TNaming_NamedShape::GetID(), ObjectNS))
        {
            TopoDS_Shape labelShape = ObjectNS->Get();
            if (!labelShape.IsNull())
            {
                TopoDS_HShape* hShape = new TopoDS_HShape(labelShape);
                shapeCache.insert(std::pair<std::string, TopoDS_HShape*>(entryInstance.ToCString(),hShape));
                if (topLevel)
                {
                    topEntity = hShape;
                }
            }
        }

        TDF_ChildIterator childIter(parentLabel);
        for (; childIter.More(); childIter.Next())
        {
            addShapeCacheRecursive(childIter.Value(), shapeCache, topEntity);
        }
    }
    catch (...)
    {
    }
}

OccModel::OccModel(TDocStd_Document* doc)
{
    m_data = new OccModelData;
    m_data->doc = doc;
    m_data->doc->SetUndoLimit(-1);

    Handle(TDF_Data) data = doc->GetData();
    TDF_Label rootLabel = doc->GetData()->Root();
    TDF_ChildIterator childIter(rootLabel);
    for (; childIter.More(); childIter.Next())
    {
        TDF_Label chilLabel = childIter.Value();
        if (!chilLabel.IsNull())
        {
            TCollection_AsciiString entryInstance;
            TDF_Tool::Entry(chilLabel, entryInstance);
            std::string strPID = entryInstance.ToCString();

            void* entity;
            addShapeToCacheByEntryOnSecondLevel(strPID, entity);
        }
    }
}

OccModel::~OccModel()
{
    delete m_data;
}

void OccModel::undo()
{
    m_data->doc->Undo();

    m_data->doc->CommitCommand();

	//update shape map
	updateShapeListOverRedo();
}

void OccModel::redo()
{
    m_data->doc->Redo();

    m_data->doc->CommitCommand();

	//update shape map
	updateShapeListOverUndo();
}

void OccModel::updateShapeListOverUndo()
{
    m_data->clearAddedShapeList();
    m_data->clearDeletedShapeList();

    TDF_DeltaList undo = m_data->doc->GetUndos();
    if (undo.Size() == 0)
    {
        return;
    }

    Handle(TDF_Delta) command = undo.Last();
    TDF_AttributeDeltaList attributeDeltaList = command->AttributeDeltas();

    //find labels on second level
//     std::list<TDF_Label> solidLabels;
    for (TDF_AttributeDeltaList::iterator attrIter = attributeDeltaList.begin(); attrIter != attributeDeltaList.end(); ++attrIter)
    {
        TDF_AttributeDelta* attributeDelta = attrIter->get();
        Handle(TDF_Attribute) attri = attributeDelta->Attribute();

//         Handle(TDataStd_AsciiString) asciiString = Handle(TDataStd_AsciiString)::DownCast(attri);
// 		if (!asciiString.IsNull() && asciiString->Get() == "top_level")
// 		{
// 			TDF_Label label = attributeDelta->Label();
// 			solidLabels.push_back(label);
// 		}

		Handle(TNaming_NamedShape) namedShape = Handle(TNaming_NamedShape)::DownCast(attri);
		if (!namedShape.IsNull())
		{
			// judgment of root label
			TDF_Label label = attributeDelta->Label();
			Handle(TDataStd_AsciiString) asciiString;
			if (!label.FindAttribute(TDataStd_AsciiString::GetID(), asciiString))
			{
				continue;
			}
			else if (asciiString->Get() != "top_level")
			{
				continue;
			}

			TCollection_AsciiString anEntry;
			TDF_Tool::Entry(label, anEntry);

			// operation type
			Handle(TDF_DeltaOnAddition) delta1 = Handle(TDF_DeltaOnAddition)::DownCast(attributeDelta);
			Handle(TDF_DeltaOnForget) delta2 = Handle(TDF_DeltaOnForget)::DownCast(attributeDelta);
			Handle(TNaming_DeltaOnRemoval) delta3 = Handle(TNaming_DeltaOnRemoval)::DownCast(attributeDelta);

			if (delta1)
			{
				TopoDS_Shape shape = namedShape->Get();
                TopoDS_HShape* hShape = new TopoDS_HShape(shape);
                m_data->addedShapeList.push_back(hShape);
                void* newEntity;
                addShapeToCacheByEntryOnSecondLevel(anEntry.ToCString(), newEntity);
            }
			else if (delta2)
			{
				//nothing to do
				//             m_data->deletedShapeList.push_back(shape);
			}
			else if (delta3)
			{
				Handle(TNaming_DeltaOnModification) modifyDelta = delta3->getMyDelta();

				Handle(TopTools_HArray1OfShape) shapeArray = modifyDelta->getMyNew();
				int size = shapeArray->Size();
				if (size == 1)
				{
					TopoDS_Shape shape = shapeArray->Value(1);
                    TopoDS_HShape* hShape = new TopoDS_HShape(shape);
                    m_data->deletedShapeList.push_back(hShape);
                    deleteShapeToCacheByEntryOnSecond(anEntry.ToCString());
				}
			}
		}
    }

//     for (std::list<TopoDS_Shape>::iterator iter = m_data->addedShapeList.begin(); iter != m_data->addedShapeList.end(); ++iter)
//     {
//         m_data->returnAddedShapeList.push_back(&(*iter));
//     }
// 
//     for (std::list<TopoDS_Shape>::iterator iter = m_data->deletedShapeList.begin(); iter != m_data->deletedShapeList.end(); ++iter)
//     {
//         m_data->returnDeletedShapeList.push_back(&(*iter));
//     }
}

void OccModel::updateShapeListOverRedo()
{
    m_data->clearAddedShapeList();
    m_data->clearDeletedShapeList();

    TDF_DeltaList redo = m_data->doc->GetRedos();
    if (redo.Size() == 0)
    {
        return;
    }

    Handle(TDF_Delta) command = redo.First();
    TDF_AttributeDeltaList attributeDeltaList = command->AttributeDeltas();

    //find labels on second level
    std::list<TDF_Label> solidLabels;
    for (TDF_AttributeDeltaList::iterator attrIter = attributeDeltaList.begin(); attrIter != attributeDeltaList.end(); ++attrIter)
    {
        TDF_AttributeDelta* attributeDelta = attrIter->get();
        Handle(TDF_Attribute) attri = attributeDelta->Attribute();

        Handle(TDataStd_AsciiString) asciiString = Handle(TDataStd_AsciiString)::DownCast(attri);
		if (!asciiString.IsNull() && asciiString->Get() == "top_level")
		{
			TDF_Label label = attributeDelta->Label();
			solidLabels.push_back(label);
		}
	}

    for (TDF_AttributeDeltaList::iterator attrIter = attributeDeltaList.begin(); attrIter != attributeDeltaList.end(); ++attrIter)
    {
        TDF_AttributeDelta* attributeDelta = attrIter->get();
        Handle(TDF_Attribute) attri = attributeDelta->Attribute();

        // judgment of shape
        Handle(TNaming_NamedShape) namedShape = Handle(TNaming_NamedShape)::DownCast(attri);
        if (!namedShape) // we only need the shape
        {
            continue;
        }

        // judgment of root label
        TDF_Label label = attributeDelta->Label();
        bool found = false;
        for (std::list<TDF_Label>::iterator labelIter = solidLabels.begin(); labelIter != solidLabels.end(); ++labelIter)
        {
            if (*labelIter == label)
            {
                found = true;
            }
        }
        if (!found)
        {
            continue;
        }

		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(label, anEntry);

        // operation type
        Handle(TDF_DeltaOnAddition) delta1 = Handle(TDF_DeltaOnAddition)::DownCast(attributeDelta);
        Handle(TDF_DeltaOnForget) delta2 = Handle(TDF_DeltaOnForget)::DownCast(attributeDelta);
        Handle(TNaming_DeltaOnRemoval) delta3 = Handle(TNaming_DeltaOnRemoval)::DownCast(attributeDelta);

        if (!namedShape) // we only need the shape
        {
            continue;
        }
//         shape = namedShape->Get();

        if (delta1)
        {
            TopoDS_Shape shape = namedShape->Get();
            TopoDS_HShape* hShape = new TopoDS_HShape(shape);
            m_data->addedShapeList.push_back(hShape);
            void* newEntity;
            addShapeToCacheByEntryOnSecondLevel(anEntry.ToCString(), newEntity);
        }
        else if (delta2)
        {
            //nothing to do
        }
        else if (delta3)
        {
            Handle(TNaming_DeltaOnModification) modifyDelta = delta3->getMyDelta();

            Handle(TopTools_HArray1OfShape) shapeArray = modifyDelta->getMyNew();
            int size = shapeArray->Size();
            if (size == 1)
            {
                TopoDS_Shape shape = shapeArray->Value(1);
                TopoDS_HShape* hShape = new TopoDS_HShape(shape);
                m_data->deletedShapeList.push_back(hShape);
                deleteShapeToCacheByEntryOnSecond(anEntry.ToCString());
            }
        }
    }

//     for (std::list<TopoDS_Shape>::iterator iter = m_data->addedShapeList.begin(); iter != m_data->addedShapeList.end(); ++iter)
//     {
//         m_data->returnAddedShapeList.push_back(&(*iter));
//     }
// 
//     for (std::list<TopoDS_Shape>::iterator iter = m_data->deletedShapeList.begin(); iter != m_data->deletedShapeList.end(); ++iter)
//     {
//         m_data->returnDeletedShapeList.push_back(&(*iter));
//     }
}

void OccModel::addShapeToCacheByEntryOnSecondLevel(const std::string& strPID, void*& topEntity)
{
    try
    {
        TDF_Label label;
        TDF_Tool::Label(m_data->doc->GetData(), Standard_CString(strPID.c_str()), label);
        if (label.IsNull())
        {
            return;
        }

        addShapeCacheRecursive(label, m_data->shapeCache, topEntity, true);
    }
    catch (...)
    {
    }
}

void OccModel::deleteShapeToCacheByEntryOnSecond(const std::string& strPID)
{
    for (std::map<std::string, TopoDS_HShape*>::iterator iter = m_data->shapeCache.begin(); iter != m_data->shapeCache.end();)
    {
        std::string iterEntry = iter->first;
        bool startwith = iterEntry.compare(0, strPID.size(), strPID) == 0;
        if (startwith)
        {
            iter->second->Delete();
            m_data->shapeCache.erase(iter++);
        }
        else
        {
            iter++;
        }
    }
}

void* OccModel::findEntityFromCacheMap(const std::string& strPID)
{
    for (std::map<std::string, TopoDS_HShape*>::iterator iter = m_data->shapeCache.begin(); iter != m_data->shapeCache.end(); ++iter)
    {
        if (strPID == iter->first&& iter->second)
        {
            return iter->second;
        }
    }

    return nullptr;
}

TDocStd_Document* OccModel::getDoc() const
{
    return m_data->doc;
}

void OccModel::setDoc(TDocStd_Document* curDoc)
{
	if (m_data->doc != NULL)
	{
		m_data->doc->Close();
	}
	m_data->doc = curDoc;
	if (curDoc != NULL)
	{
		m_data->doc->SetUndoLimit(-1);
	}
}

std::list<void *> OccModel::getReturnAddedShapeList() const
{
    return m_data->addedShapeList;
}

void OccModel::setReturnAddedShapeList(std::list<void *> val)
{
    m_data->addedShapeList = val;
}

std::list<void *> OccModel::getReturnDeletedShapeList() const
{
    return m_data->deletedShapeList;
}

void OccModel::setReturnDeletedShapeList(std::list<void *> val)
{
    m_data->deletedShapeList = val;
}

void OccModel::BeginCommand()
{
    getDoc()->OpenCommand();
// 	getDoc()->NewCommand();
}

bool OccModel::EndCommand()
{
	return getDoc()->CommitCommand();
}

bool OccModel::findShapePID(void* curEntity, std::string& strPID)
{
	if (curEntity == NULL) 
	{
		return false;
	}
    try
    {
        TDF_Label rootLabel = m_data->doc->GetData()->Root();
        TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(curEntity);
        TopoDS_Shape searchShape = hshape->Shape();

        return LabelUtilities::findShapePIDFromRoot(rootLabel, searchShape, strPID);
    }
    catch (...)
    {
    }

    return false;
}

void OccModel::ImportGeomModel(FILE* fp, const string& fileName, int& iEntityNum, std::vector<void*>& entityList)
{
    if (fp)
    {

    }
    try
    {
        BeginCommand();
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        std::string path = fileName;
//         std::string path = "C:\\Users\\Administrator\\Desktop\\pump.igs";
//         std::string path = "C:\\Users\\Administrator\\Desktop\\pump.step";

        std::string ext = path.substr(path.find_last_of(".") + 1);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        IOShapeBuilder ioBuilder;
        if (ext == "step" || ext == "stp")
        {
            if (!ioBuilder.importSTEPModelViaXDE(path))
            {
                EndCommand();
                return;
            }
        }
        else if (ext == "iges" || ext == "igs")
        {
            if (!ioBuilder.importIGESModelViaXDE(path))
            {
                EndCommand();
                return;
            }
        }
        else
        {
            EndCommand();
            return;
        }

		std::vector<void *> instanceVector = ioBuilder.getEntityList();
		for (std::vector<void *>::iterator iter = instanceVector.begin(); iter != instanceVector.end(); ++iter)
		{
			entityList.push_back(*iter);
		}

        iEntityNum = entityList.size();
        EndCommand();
        return;
    }
    catch (...)
    {
        EndCommand();
        return;
    }

//     WriteSatFile(entityList, nullptr);
}

bool OccModel::PrepareModel(const std::vector<void*>& inputEntList, const std::vector<int>& idList, std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp)
{
    return true;
}

bool OccModel::PrepareModel(const std::vector<void*>& inputEntList, std::vector<void*>& OutEntList)
{
	if (inputEntList.size() < 2)
	{
		return false;
	}

	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find entity PID; collect shape and information
		TopTools_ListOfShape /*args, tools, */total;
		LabelShapeAdapter adapter;
// 		TransformShapeBuilder transformBuilder;
		for (std::vector<void*>::const_iterator iter = inputEntList.begin(); iter != inputEntList.end(); ++iter)
		{
			std::string strPIDInstance;
			if (TheOCCApp.getCurrentModel()->findShapePID(*iter, strPIDInstance))
			{
				//find label and shape
				TDF_Label foundLabel;
				TDF_Tool::Label(data, TCollection_AsciiString(strPIDInstance.c_str()), foundLabel, false);
				TopoDS_Shape foundShape = LabelUtilities::getLabelShape(foundLabel);
				if (foundShape.IsNull())
				{
					return false;
				}

// 				//copy shape to avoid influence of boolean operation upon origin shape
// 				if (!transformBuilder.shapeCopy(foundShape))
// 				{
// 					return false;
// 				}
				adapter.labelDiscreteToMap(foundLabel);
// 				adapter.collectRelatedLabelAndShapes(foundShape, EnCopy, transformBuilder.getBuilder());
// 				foundShape = transformBuilder.getShape();

// 				if (iter == inputEntList.begin())
// 				{
// 					args.Append(foundShape);
// 				}
// 				else
// 				{
// 					tools.Append(foundShape);
// 				}
// 
				total.Append(foundShape);
			}
			else
			{
				COMMAND_END

					return false;
			}
		}

		//start operation
		BooleanShapeBuilder shapeBuilder;
		TopoDS_Shape targetShape;
		if (!shapeBuilder.cellsBuilderOperation(total, EnCellsBuilderFuse))
		{
			return false;
		}
		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		TopTools_ListOfShape::Iterator shapeIter(total);
		for (; shapeIter.More(); shapeIter.Next())
		{
			adapter.collectRelatedLabelAndShapes(shapeIter.Value(), EnCellsBuilderFuse, shapeBuilder.getBuilder());
		}

		// 根据布尔运算后的模型自动分配子节点到各个label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		std::string strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");
		void* resEntity = nullptr;
		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		OutEntList.push_back(resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModel::PrepareModel(const std::vector<void*>& inputEntList,/* const std::vector<int>& idList,*/ std::vector<void*>& OutEntList, int mshTyp, int mshAlgTyp)
{
	return false;
}

void OccModel::ExportModelFile(const std::vector<void*>& inputEntList, const std::vector<std::string>& entityNameList, FILE* fp)
{
}

void OccModel::RestoreModelArchive(xdArchive &ar, std::vector<void*>& loadedEntList, std::vector<string>& entPIDList)
{
	ar.Flush();
	UINT hFile = ar.File()->m_hFile;
	int64_t iOCCStartPos = tell(hFile);
	int64_t curEndPos;
	ar >> curEndPos;
	ar.Flush();

	std::filebuf fb(fdopen(hFile, "ab+"));
	std::istream os(&fb);

    try
    {
        Handle(TDocStd_Document) openedDoc;

        //start
        TDF_Label origRoot = this->getDoc()->GetData()->Root();

        PCDM_ReaderStatus  status = TheGKApp.getApp()->Open(os, openedDoc);
        if (status != PCDM_RS_OK)
        {
            return;
        }

        TDF_Label source = openedDoc->GetData()->Root();
        TDF_ChildIterator iter(source);
        for (; iter.More(); iter.Next())
        {
            TDF_Label child = iter.Value();

            TDF_Label targetLabel = LabelUtilities::createNewChild(origRoot);
            LabelUtilities::copyLabelWithFather(child, targetLabel);

            TCollection_AsciiString entryInstance;
            TDF_Tool::Entry(targetLabel, entryInstance);
            entPIDList.push_back(entryInstance.ToCString());

            void* resEntity = nullptr;
            this->addShapeToCacheByEntryOnSecondLevel(entryInstance.ToCString(), resEntity);
            loadedEntList.push_back(resEntity);
        }
        //TODO remove opendDoc from app
    }
    catch (...)
    {

    }

	ar.Flush();
	int iOCCEndPos = tell(hFile);
	if (iOCCEndPos != curEndPos)
	{
		ar.File()->Seek(curEndPos, xdFile::begin);
	}
}

void OccModel::SaveModelArchive(const std::vector<void*>& inputEntList, xdArchive &ar)
{
	ar.Flush();
	UINT hFile = ar.File()->m_hFile;
	int64_t iOCCStartPos = ar.File()->Position();
	ar << iOCCStartPos;
	ar.Flush();
	std::filebuf fb(fdopen(hFile, "ab+"));
	std::ostream os(&fb);

	this->getDoc()->ChangeStorageFormat("BinOcaf");
	try
	{
        std::vector<std::string> inputPIDList;
//         OccModel* origModel = TheGKApp.getCurrentModel();

        //find corresponding PID
        for (int i = 0; i < inputEntList.size(); ++i)
        {
            std::string strPID;
            if (this->findShapePID(inputEntList.at(i), strPID))
            {
                inputPIDList.push_back(strPID);
            }
        }

        //remove no referenecd labels
        this->BeginCommand();
        TDocStd_Document* doc1 = this->getDoc();
        TDF_Label source = doc1->GetData()->Root();
        TDF_ChildIterator iter(source);
        for (; iter.More(); iter.Next())
        {
            TDF_Label child = iter.Value();
            TCollection_AsciiString entryInstance;
            TDF_Tool::Entry(child, entryInstance);
            bool exist = false;
            for (int i = 0; i < inputPIDList.size(); ++i)
            {
                if (inputPIDList.at(i) == entryInstance.ToCString())
                {
                    exist = true;
                    break;;
                }
            }

            if (!exist)
            {
                child.ForgetAllAttributes();
            }
        }
        this->EndCommand();
        //save data after remove
        TheGKApp.getApp()->SaveAs(this->getDoc(), os);
        doc1->Undo();
	}
	catch (...)
	{

	}
	ar.Flush();
	int64_t iOCCEndPos = ar.File()->Position();
	ar.File()->Seek(iOCCStartPos, xdFile::begin);
	//iOCCStartPos = tell(hFile);
	ar << iOCCEndPos;
	ar.Flush();
	ar.File()->Seek(iOCCEndPos, xdFile::begin);
	//iOCCEndPos = ar.File()->Position();
}

double OccModel::GetTolerance()
{
    return 10e-6;
}

std::string OccModel::GetStringAttribute(void* pEntity, const std::string& attrName)
{
    std::string value = "";
    GetEntityAttribute(pEntity, attrName, value);
    return value;
}

void OccModel::BuildEntityAttributeName(void* orgEntity, void* curEntity, const std::string& bodyName, bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID)
{
    TopoDS_HShape* cubBdy = (TopoDS_HShape*)curEntity;
    if (cubBdy)
    {
        Handle(TDF_Data) data = m_data->doc->GetData();

        //find PID
        std::string curPID;
        if (!findShapePID(curEntity, curPID))
        {
            return;
        }
        TDF_Label curLabel;
        TDF_Tool::Label(data, TCollection_AsciiString(curPID.c_str()), curLabel, false);

        //find PID
        std::string orgPID;
        TDF_Label orgLabel;
        if (findShapePID(orgEntity, orgPID))
        {
            TDF_Tool::Label(data, TCollection_AsciiString(orgPID.c_str()), orgLabel, false);
        }

        //体的名称默认使用cs的名称，如果本身有名称，名称进行继承
        std::string name;
        if (!LabelUtilities::getLabelName(curLabel, name))
        {
            LabelUtilities::setLabelName(curLabel, bodyName);
        }

        //process face and edge name attribute
        TDF_ChildIterator curFaceIter(curLabel);
        TDF_ChildIterator orgFaceIter(orgLabel);
        if (orgLabel.IsNull())
        {
            for (; curFaceIter.More(); curFaceIter.Next())
            {
                LabelShapeAdapter::buildEntityAttributeName(orgLabel, curFaceIter.Value(), bIsNewCreate, bIsCloneCS, bNeedCopyFaceName, iNextFaceID, iNextEdgeID);
            }
        }
        else
        {
            TDF_ChildIterator orgFaceIter(orgLabel);
            for (; curFaceIter.More(); curFaceIter.Next(), orgFaceIter.Next())
            {
                LabelShapeAdapter::buildEntityAttributeName(orgFaceIter.Value(), curFaceIter.Value(), bIsNewCreate, bIsCloneCS, bNeedCopyFaceName, iNextFaceID, iNextEdgeID);
            }
        }
    }
}

bool OccModel::GetEntityAttribute(void* entity, const std::string &attrName, std::string &attrValue)
{
    std::string strPID;
    if (!findShapePID(entity, strPID))
    {
        return false;
    }

    return getPIDProperty(strPID, attrName, attrValue);
}

bool OccModel::GetEntityAttribute(void* entity, const std::string &attrName, int &attrValue)
{
    std::string strPID;
    if (!findShapePID(entity, strPID))
    {
        return false;
    }

    return getPIDProperty(strPID, attrName, attrValue);
}

bool OccModel::AddEntityAttribute(void* entity, const std::string &attrName, const std::string &attrValue, bool bNeedCopy)
{
    BeginCommand();
    std::string strPID;
    if (!findShapePID(entity, strPID))
    {
        return false;
    }

    if (addPIDProperty(strPID, attrName, attrValue))
    {
        EndCommand();

        return true;
    }
    else
    {
        EndCommand();

        return false;
    }
}

bool OccModel::AddEntityAttribute(void* entity, const std::string &attrName, const int attrValue, bool bNeedCopy)
{
    BeginCommand();

    std::string strPID;
    if (!findShapePID(entity, strPID))
    {
        return false;
    }

    if (addPIDProperty(strPID, attrName, attrValue))
    {
        EndCommand();

        return true;
    }
    else
    {
        EndCommand();

        return false;
    }
}

bool OccModel::HasEntityAttributeValue(void* entity, const std::string &attrName, const int attrValue)
{
	std::string strPID;
	if (!findShapePID(entity, strPID))
	{
		return false;
	}

	int existValue;
	if (!getPIDProperty(strPID, attrName, existValue))
	{
		return false;
	}
	else
	{
		if (existValue != existValue)
		{
			return false;
		}
	}

	return true;
}

bool OccModel::RemoveEntityAttribute(void* entity, const std::string &attrName)
{
    BeginCommand();

    std::string strPID;
    if (!findShapePID(entity, strPID))
    {
        return false;
    }

    if (removeLabelProperty(strPID, attrName))
    {
        EndCommand();

        return true;
    }
    else
    {
        EndCommand();

        return false;
    }
}

bool OccModel::RemoveCellTopology(std::vector<void*>& entity_list)
{
    return true;
}

bool OccModel::DeleteBodies(std::vector<void*>& entity_list)
{
    BeginCommand();

    bool allSuccess = false;
    TDF_Label rootLabel = m_data->doc->GetData()->Root();

    for (int i = 0; i < entity_list.size(); ++i)
    {

        TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entity_list[i]);
        TopoDS_Shape searchShape = hshape->Shape();

        TDF_ChildIterator childIter(rootLabel);
        for (; childIter.More(); childIter.Next())
        {
            TDF_Label childLabel = childIter.Value();
            Handle(TNaming_NamedShape) ObjectNS;
            if (childLabel.FindAttribute(TNaming_NamedShape::GetID(), ObjectNS))
            {
                TopoDS_Shape labelShape = ObjectNS->Get();
                if (!labelShape.IsNull() && labelShape.IsSame(searchShape))
                {
                    childLabel.ForgetAllAttributes();
                    childIter.Initialize(rootLabel);
                    allSuccess = true;
                    break;
                }
            }
        }
    }

    EndCommand();

    return allSuccess;
}

bool OccModel::WriteSatFile(std::vector<void*>& entity_list, FILE* file_prt)
{
    if (file_prt)
    {

    }
    try
    {
        BeginCommand();
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        TDF_LabelList expList;
        for (auto entity : entity_list)
        {
            TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entity);
            TopoDS_Shape searchShape = hshape->Shape();
            TDF_Label foundLabel;
            if (LabelUtilities::findShapeLabelFromRoot(rootLabel, searchShape, foundLabel))
            {
                expList.Append(foundLabel);
            }
        }

        if (expList.Size() < 1)
        {
            return false;
        }

        std::string path = "C:\\Users\\Administrator\\Desktop\\AGVExport2.step";

        std::string ext = path.substr(path.find_last_of(".") + 1);
        IOShapeBuilder ioBuilder;
        ioBuilder.setExpLabelList(expList);
        if (ext == "step" || ext == "stp")
        {
            if (!ioBuilder.exportSTEPModelViaXDE(path))
            {
                EndCommand();
                return false;
            }
        }
        else if (ext == "iges" || ext == "igs")
        {
            if (!ioBuilder.exportIGESModel(path))
            {
                EndCommand();
                return false;
            }
        }
        else
        {
            EndCommand();
            return false;
        }

        EndCommand();
        return true;
    }
    catch (...)
    {
        EndCommand();
        return false;
    }

    return true;
}

std::string OccModel::GetEntityUuid(void* entity, int idx)
{
    Handle(TDF_Data) data = m_data->doc->GetData();

    std::string finalUUID = "";
    std::string shapePID;
    findShapePID(entity, shapePID);
    try
    {
        TopoDS_HShape* hshape = static_cast<TopoDS_HShape*>(entity);
        TopoDS_Shape shape = hshape->Shape();
        if (shape.ShapeType() == TopAbs_SOLID || shape.ShapeType() == TopAbs_SHELL || shape.ShapeType() == TopAbs_WIRE)//body
        {
            std::string pid = idx != XD_INVALID_INDEX ? std::to_string(idx) : shapePID;
            return "Body#" + pid;
        }
        else if (shape.ShapeType() == TopAbs_FACE)
        {
            std::string ownPid;
            TDF_Label faceLabel, ownerLabel;
            TDF_Tool::Label(data, TCollection_AsciiString(shapePID.c_str()), faceLabel, false);
            if (!faceLabel.IsNull())
            {
                if (idx == XD_INVALID_INDEX)
                {
                    ownerLabel = LabelUtilities::getOwnerLabel(faceLabel);
                    if (!ownerLabel.IsNull())
                    {
                        ownPid = LabelUtilities::getLabelPID(ownerLabel);
                    }
                }
                else
                {
                    ownPid = std::to_string(idx);
                }

                std::string faceName;
                LabelUtilities::getLabelName(faceLabel, faceName);

                if (!ownPid.empty() && !faceName.empty())
                {
                    return "Face#" + ownPid + std::string("#") + faceName;
                }
            }
        }
        else if (shape.ShapeType() == TopAbs_EDGE)
        {
            std::string ownPid;
            TDF_Label edgeLabel, ownerLabel;
            TDF_Tool::Label(data, TCollection_AsciiString(shapePID.c_str()), edgeLabel, false);
            if (!edgeLabel.IsNull())
            {
                ownerLabel = LabelUtilities::getOwnerLabel(edgeLabel);
                if (!edgeLabel.IsNull())
                {
                    TDF_LabelList faceList = LabelUtilities::findEdgeOwnerFaces(ownerLabel, shape);

                    std::string edgeNames = "";
                    for (int i = 0; i < faceList.Size(); ++i)
                    {
                        std::string edgeName;
                        LabelUtilities::getLabelName(edgeLabel, edgeName);
                        edgeNames += edgeName;
                        if (i != faceList.Size() - 1)
                        {
                            edgeNames += "&";
                        }
                    }

                    if (idx == XD_INVALID_INDEX)
                    {
                        ownPid = LabelUtilities::getLabelPID(ownerLabel);
                    }
                    else
                    {
                        ownPid = std::to_string(idx);
                    }

                    if (!ownPid.empty() && !edgeNames.empty())
                    {
                        return "Edge#" + ownPid + std::string("#") + edgeNames;
                    }
                }
            }
        }
    }
    catch (...)
    {
    }

    return "";
}

void* OccModel::GetEntityByUuid(void* entity, std::vector<std::string>& pidlist)
{
    if (pidlist.size() < 1)
    {
        return nullptr;
    }

    if (entity && pidlist[0] == "Body")
    {
        return entity;
    }
    else if (entity && pidlist[0] == "Face"&& pidlist.size() == 3)
    {
        return entity;
    }
    else if (entity && pidlist[0] == "Edge"&& pidlist.size() == 3)
    {
        return entity;
    }

    return nullptr;
}

int OccModel::GetBodyMaterial(void* entity)
{
    return 0;
}

int OccModel::GetFaceBoundaryCondition(void* entity)
{
    return 0;
}

bool OccModel::IsPeriodicBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList, std::vector<void*>& faceList)
{
    return true;
}

bool OccModel::IsFEIEBoundary(const std::vector<void*>& entityList, const std::vector<std::string>& faceNameList, std::vector<void*>& faceList)
{
    return true;
}

std::string OccModel::GetPointUuid(void* entity, const std::string& faceName, double u, double v)
{
    return "";
}

std::string OccModel::GetPointUuid(void* face, double u, double v)
{
    return "";
}

std::string OccModel::GetPointUuid(void* entity, const std::string& edgeName, double u)
{
    return "";
}

std::string OccModel::GetPointUuid(void* edge, double u)
{
    return "";
}

bool OccModel::RemoveHolesWithinTolerance(double maxradius)
{
    try
    {
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

        LabelShapeAdapter adapter;
        TDF_Label complabel = rootLabel.FindChild(0);
        TopoDS_Shape compoundshape = LabelUtilities::getLabelShape(complabel);
        adapter.labelDiscreteToMap(complabel);

        TDF_Label solidlabel = complabel.FindChild(0);
        TopoDS_Shape solidshape = LabelUtilities::getLabelShape(solidlabel);
        BRep_Builder b;
        b.Remove(compoundshape, solidshape);

        TopoDS_Shape removeresult;
        TopTools_ListOfShape facesremoved;
        double maxarea = M_PI * pow(maxradius, 2);
        if (!OCCBasicTools::RemoveHolesWithinAreaAndDetectFaces(solidshape, removeresult, facesremoved, maxarea))
            return false;

        b.Add(compoundshape, removeresult);
        adapter.replaceChildLabelWithCopiedShape(solidlabel, removeresult);
        adapter.allocateLabelWithTopShape(complabel, compoundshape);
        adapter.linkRelatedLabels(complabel);

        TCollection_AsciiString entry;
        TDF_Tool::Entry(complabel, entry);
        string strPID = entry.ToCString();

        void* part = nullptr;
        deleteShapeToCacheByEntryOnSecond(strPID);
        addShapeToCacheByEntryOnSecondLevel(strPID, part);
    }
    catch (...)
    {
        return false;
    }
}

bool OccModel::Heal(const std::map<void*, std::string> entnamemap, std::vector<void*>& outentlist, HealOption params, std::string& m_err)
{
    //0:no heal, 1 auto, 2 manual
    try
    {
        if (entnamemap.size() < 1)
            return false;
        for (auto iter : entnamemap)
        {
            void* entry = iter.first;
            TDocStd_Document* curdoc = TheOCCApp.getCurrentModel()->getDoc();
            Handle(TDF_Data) data = curdoc->GetData();
            TDF_Label rootLabel = data->Root();

            std::vector<void*> facelist, edgelist;
            GetFaces(entry, facelist);
            GetEdgeList(entry, edgelist);
            for (int i = 0; i < facelist.size(); i++)
            {
                void* facex = facelist[i];
                std::string PIDx;
                TDF_Label labelx;
                if (!TheOCCApp.getCurrentModel()->findShapePID(facex, PIDx))
                    continue;
                TDF_Tool::Label(data, PIDx.c_str(), labelx);
                if (labelx.IsNull())
                    continue;
                if (!LabelUtilities::addLabelProperty(labelx, "bodyName", iter.second))
                    continue;
            }
            for (int i = 0; i < edgelist.size(); i++)
            {
                void* edgex = edgelist[i];
                std::string PIDx;
                TDF_Label labelx;
                if (!TheOCCApp.getCurrentModel()->findShapePID(edgex, PIDx))
                    continue;
                TDF_Tool::Label(data, PIDx.c_str(), labelx);
                if (labelx.IsNull())
                    continue;
                if (!LabelUtilities::addLabelProperty(labelx, "bodyName", iter.second))
                    continue;
            }
            //find shape
            LabelShapeAdapter adapter;
            std::string foundPID;
            TDF_Label foundLabel;
            TopoDS_Shape foundShape;
            if(!TheOCCApp.getCurrentModel()->findShapePID(entry, foundPID))
                continue;

            TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
            if(foundLabel.IsNull())
                continue;

            foundShape = LabelUtilities::getLabelShape(foundLabel);

            TopoDS_Shape copieshape = foundShape;
            if (params.healingtype == 0)
            {
                outentlist.push_back(entry);
                m_err += "heal done without operation";
            }
            else if (params.healingtype == 1)
            {
                m_err += "analysis before fixing:\n\n";
                OCCFixToolInstance->shapeValidityAnalysis(copieshape, copieshape.ShapeType(), m_err);
                m_err += "\n";

                OCCFixToolInstance->setPrecision(0.0001);
                OCCFixToolInstance->setMaxTol(0.0001);
                OCCFixToolInstance->setMinTol(0.0001);

                //最简单的自动修复
                Handle(ShapeFix_Shape) afixshape = new ShapeFix_Shape();
                afixshape->Init(copieshape);
                afixshape->SetPrecision(0.0001);
                afixshape->SetMaxTolerance(0.0001);
                afixshape->SetMinTolerance(0.0001);
                afixshape->Perform();
                copieshape = afixshape->Shape();

                Handle(ShapeFix_Wireframe) afixwire = new ShapeFix_Wireframe;
                afixwire->SetPrecision(0.01);
                afixwire->SetMaxTolerance(0.01);
                afixwire->ModeDropSmallEdges() = true;
                afixwire->FixSmallEdges();
                afixwire->FixWireGaps();
                copieshape = afixwire->Shape();

                m_err += "analysis after fixing:\n\n";
                OCCFixToolInstance->shapeValidityAnalysis(copieshape, copieshape.ShapeType(), m_err);
                m_err += "\n";
            }
            else if (params.healingtype == 2)
            {
                m_err += "manual healing start successfully.\n";
                if (params.isRmvSliverFaces)
                {
                    std::vector<TopoDS_Shape> sliverFacesVec;
                    double toleancesliver;
                    if (params.UseScalFactor)
                    {
                        double xmin, ymin, zmin, xmax, ymax, zmax;
                        OCCBasicTools::GetBndBoxParams(copieshape, xmin, ymin, zmin, xmax, ymax, zmax);
                        toleancesliver = sqrt(pow(xmax - xmin, 2) + pow(ymax - ymin, 2) + pow(zmax - zmin, 2))
                            * params.SliverFaceTol;
                    }
                    else if (params.UseEdgeWidth)
                    {
                        toleancesliver = params.SliverFaceTol;
                    }

                    OCCBasicTools::DetectSliverFaces(copieshape, params.SliverFaceTol, sliverFacesVec);

                    TopTools_ListOfShape remove_list;
                    for (int i = 0; i < sliverFacesVec.size(); i++)
                    {
                        remove_list.Append((sliverFacesVec[i]);
                    }
                    OCCBasicTools::RemoveFace(copieshape, remove_list, copieshape);

                    int slivernum = remove_list.Size();
                    m_err += "sliver face remove num is:";
                    m_err += std::to_string(slivernum);
                    m_err += "\n";
                }
            }
        }
    }
    catch (...)
    {

    }
}

/************************************************************************/
/*                          protected                                   */
/************************************************************************/
bool OccModel::getPIDProperty(const std::string& strPID, const std::string& propertyName, int& value)
{
    bool found = false;
    try
    {
        Handle(TDF_Data) data = m_data->doc->GetData();
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        found = LabelUtilities::getLabelProperty(label, propertyName, value);
    }
    catch (...)
    {
    }

    return found;
}

bool OccModel::getPIDProperty(const std::string& strPID, const std::string& propertyName, std::string& value)
{
    bool found = false;
    try
    {
        Handle(TDF_Data) data = m_data->doc->GetData();
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        found = LabelUtilities::getLabelProperty(label, propertyName, value);
    }
    catch (...)
    {
    }

    return found;
}

bool OccModel::getPIDProperty(const std::string& strPID, const std::string& propertyName, double& value)
{
    bool found = false;
    try
    {
        Handle(TDF_Data) data = m_data->doc->GetData();
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        found = LabelUtilities::getLabelProperty(label, propertyName, value);
    }
    catch (...)
    {
    }

    return found;
}

bool OccModel::addPIDProperty(const std::string& strPID, const std::string& propertyName, const int& value)
{
    bool success = false;
    try
    {
        Handle(TDF_Data) data = m_data->doc->GetData();
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        success = LabelUtilities::addLabelProperty(label, propertyName, value);
    }
    catch (...)
    {
    }

    return success;
}

bool OccModel::addPIDProperty(const std::string& strPID, const std::string& propertyName, const std::string& value)
{
    bool success = false;
    try
    {
        Handle(TDF_Data) data = m_data->doc->GetData();
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        success = LabelUtilities::addLabelProperty(label, propertyName, value);
    }
    catch (...)
    {
    }

    return success;
}

bool OccModel::addPIDProperty(const std::string& strPID, const std::string& propertyName, const double& value)
{
    bool success = false;
    try
    {
        Handle(TDF_Data) data = m_data->doc->GetData();
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        success = LabelUtilities::addLabelProperty(label, propertyName, value);
    }
    catch (...)
    {
    }

    return success;
}

bool OccModel::removeLabelProperty(const std::string& strPID, const std::string& propertyName)
{
    bool found = false;
    try
    {
        Handle(TDF_Data) data = m_data->doc->GetData();
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        found = LabelUtilities::removeLabelProperty(label, propertyName);
    }
    catch (...)
    {
    }

    return found;
}

void* OccModel::GetOwnerEntity(void* pEntity)
{
	Handle(TDF_Data) data = m_data->doc->GetData();

	std::string shapePID;
	findShapePID(pEntity, shapePID);

	std::string ownPid;
	TDF_Label faceLabel, ownerLabel;
	TDF_Tool::Label(data, TCollection_AsciiString(shapePID.c_str()), faceLabel, false);
	if (!faceLabel.IsNull())
	{
		ownerLabel = LabelUtilities::getOwnerLabel(faceLabel);
		if (!ownerLabel.IsNull())
		{
			ownPid = LabelUtilities::getLabelPID(ownerLabel);
		}
	}

	if (!ownPid.empty())
	{
		return findEntityFromCacheMap(ownPid);
	}
	return pEntity;
}