#include "LabelUtilities.h"
#include "OccApplication.h"
#include "OccModel.h"

#include <TDocStd_XLinkTool.hxx>
#include <TDF_CopyLabel.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDataStd_Name.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TDataStd_Real.hxx>
#include <TNaming_UsedShapes.hxx>
#include <TNaming_RefShape.hxx>

LabelUtilities::LabelUtilities()
{
}


LabelUtilities::~LabelUtilities()
{
}

bool LabelUtilities::copyLabelWithoutFather(const TDF_Label& source, const TDF_Label& target)
{
    try
    {
        TDocStd_XLinkTool copyTool;
        copyTool.Copy(target, source);
        if (!copyTool.IsDone())
        {
            return false;
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool LabelUtilities::copyLabelWithFather(const TDF_Label& source, const TDF_Label& target)
{
    try
    {
        TDF_CopyLabel copyLabel(source, target);
        copyLabel.Perform();

        if (!copyLabel.IsDone())
        {
            return false;
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool LabelUtilities::copyLabelWithFather(const TDocStd_Document* doc, const std::string& sourceEntry, const std::string& targetEntry)
{
    try
    {
        Handle(TDF_Data) data = doc->GetData();
        TDF_Label sourceLabel, targetLabel;
        TDF_Tool::Label(data, TCollection_AsciiString(sourceEntry.c_str()), sourceLabel, false);
        TDF_Tool::Label(data, TCollection_AsciiString(targetEntry.c_str()), targetLabel, false);

        return LabelUtilities::copyLabelWithFather(sourceLabel, targetLabel);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool LabelUtilities::copyLabelWithOnlyPropertyChild(const TDF_Label& source, const TDF_Label& target)
{
    try
    {
        TDF_CopyLabel copyLabel(source, target);
        copyLabel.Perform();

        if (!copyLabel.IsDone())
        {
            return false;
        }

        TDF_ChildIterator childIter(target);
        for (; childIter.More(); childIter.Next())
        {
            TDF_Label child = childIter.Value();
            Handle(TDataStd_Name) nameAttri;
            if (!child.FindAttribute(TDataStd_Name::GetID(), nameAttri))
            {
                 //child.ForgetAllAttributes();
            }
            else
            {
                if (nameAttri->Get().IsDifferent(TCollection_ExtendedString("property")))
                {
                    child.ForgetAllAttributes();
                }
            }
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool LabelUtilities::copyLabelProperty(const TDF_Label& source, const TDF_Label& target)
{
    try
    {
        TDF_Label sourcePropLabel, targetPropLabel;
        if (findNameLabel(source, "property", sourcePropLabel) && findNameLabel(target, "property", targetPropLabel))
        {
            targetPropLabel.ForgetAllAttributes();
            return copyLabelWithFather(sourcePropLabel, targetPropLabel);
        }
    }
    catch (...)
    {
    }

    return false;
}

bool LabelUtilities::copyLabelName(const TDF_Label& source, const TDF_Label& target)
{
    try
    {
        Handle(TDataStd_Name) namedData;
        if (!source.FindAttribute(TDataStd_Name::GetID(), namedData))
        {
            return false;
        }
        TDataStd_Name::Set(target, namedData->Get());

        //old method
//         std::string sourceName;
//         if (!getLabelName(source, sourceName))
//         {
//             return false;
//         }
//         setLabelName(target, sourceName);
    }
    catch (...)
    {
    }

    return false;
}

bool LabelUtilities::findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, TDF_Label& returnLabel)
{
    try
    {
        Handle(TNaming_UsedShapes) US;
        Standard_Boolean MapExist = label.FindAttribute(TNaming_UsedShapes::GetID(), US);
        TNaming_DataMapOfShapePtrRefShape* M = &US->Map();
        const TNaming_PtrRefShape* shape = M->Seek(compareShape);
        if (shape == nullptr/* || (*shape)->Shape().IsNotEqual(compareShape)*/)
        {
            return false;
        }

        returnLabel = (*shape)->Label();

        return true;


//         Handle(TNaming_NamedShape) namedShape;
//         if (label.FindAttribute(TNaming_NamedShape::GetID(), namedShape))
//         {
//             TopoDS_Shape shape = namedShape->Get();
//             if (!shape.IsNull() && compareShape.IsSame(shape))
//             {
//                 returnLabel = label;
//                 return true;
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             if (findShapeLabelFromRoot(childIter.Value(), compareShape, returnLabel))
//             {
//                 return true;
//             }
//         }
    }
    catch (...)
    {
    }

    return false;
}

bool LabelUtilities::findShapeLabelFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape)
{
    try
    {
        Handle(TNaming_UsedShapes) US;
        Standard_Boolean MapExist = label.FindAttribute(TNaming_UsedShapes::GetID(), US);
        TNaming_DataMapOfShapePtrRefShape* M = &US->Map();
        const TNaming_PtrRefShape* shape = M->Seek(compareShape);
        if (shape == nullptr/* || (*shape)->Shape().IsNotEqual(compareShape)*/)
        {
            return false;
        }
        else
        {
            return true;
        }

//         Handle(TNaming_NamedShape) namedShape;
//         if (label.FindAttribute(TNaming_NamedShape::GetID(), namedShape))
//         {
//             TopoDS_Shape shape = namedShape->Get();
//             if (!shape.IsNull() && compareShape.IsSame(shape))
//             {
//                 return true;
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             if (findShapeLabelFromRoot(childIter.Value(), compareShape))
//             {
//                 return true;
//             }
//         }
    }
    catch (...)
    {
    }

    return false;
}

bool LabelUtilities::findShapePIDFromRoot(const TDF_Label& label, const TopoDS_Shape& compareShape, std::string& strPID)
{
    try
    {
        TDF_Label returnLabel;
        if (findShapeLabelFromRoot(label, compareShape, returnLabel))
        {
            strPID = getLabelPID(returnLabel);
            return true;
        }
        else
        {
            return false;
        }
//         TopAbs_ShapeEnum type = compareShape.ShapeType();
//         Handle(TNaming_NamedShape) namedShape;
//         if (label.FindAttribute(TNaming_NamedShape::GetID(), namedShape))
//         {
//             TopoDS_Shape shape = namedShape->Get();
// 
//             TopExp_Explorer exp(shape, type);
//             for (; exp.More(); exp.Next())
//             {
//                 TopoDS_Shape faceShape = exp.Value();
//                 if (faceShape.IsSame(compareShape))
//                 {
//                     break;
//                 }
//             }
// 
//             TopAbs_ShapeEnum type2 = shape.ShapeType();
// 
//             if (!shape.IsNull() && compareShape.IsSame(shape))
//             {
//                 TCollection_AsciiString entryInstance;
//                 TDF_Tool::Entry(label, entryInstance);
//                 strPID = entryInstance.ToCString();
//                 return true;
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             if (findShapePIDFromRoot(childIter.Value(), compareShape, strPID))
//             {
//                 return true;
//             }
//         }
    }
    catch (...)
    {
    }
    return false;
}

bool LabelUtilities::findSubShapePIDsFromRoot(const TDF_Label& label, const TopAbs_ShapeEnum& shapeType, std::vector<std::string>& strPIDs)
{
	try
	{
		TDF_Label rootLabel = label.Root();
		TopoDS_Shape origShape = getLabelShape(label);
		if (origShape.IsNull())
		{
			return false;
		}

		if (origShape.ShapeType() >= shapeType)
		{
			return false;
		}

		TopExp_Explorer exp(origShape, shapeType);
		for (; exp.More(); exp.Next())
		{
			TopoDS_Shape subShape = exp.Value();
// 			TDF_Label returnLabel;
			std::string strPID;
			if (findShapePIDFromRoot(rootLabel, subShape, strPID))
			{
				strPIDs.push_back(strPID);
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool LabelUtilities::findNameLabel(const TDF_Label& sourceRootLabel, const std::string& name, TDF_Label& retuanLabel)
{
	try
	{
		Handle(TDataStd_Name) nameData;
		if (sourceRootLabel.FindAttribute(TDataStd_Name::GetID(), nameData))
		{
			if (!nameData.IsNull() && !nameData->Get().IsDifferent(TCollection_ExtendedString(name.c_str())))
			{
				retuanLabel = sourceRootLabel;
				return true;
			}
		}

		TDF_ChildIterator childIter(sourceRootLabel);
		for (; childIter.More(); childIter.Next())
		{
			if (findNameLabel(childIter.Value(), name, retuanLabel))
			{
				return true;
			}
		}
	}
	catch (...)
	{
	}

	return false;
}

TDF_Label LabelUtilities::getOwnerLabel(const TDF_Label& childLabel)
{
    int depth = childLabel.Depth();
    TDF_Label fatherLabel;

    if (depth > 1)
    {
        fatherLabel = childLabel.Father();
        depth--;
    }
	depth--;

    for (int i = depth; i > 0; --i)
    {
        fatherLabel = fatherLabel.Father();
    }
    return fatherLabel;
}

bool LabelUtilities::getLastEntry(const TDocStd_Document* doc, std::string& entry)
{
    try
    {
        Handle(TDF_Data) data = doc->GetData();
        TDF_Label rootLabel = data->Root();

        TCollection_AsciiString asciiEntry;
        TDF_ChildIterator iter(rootLabel);
        for (; iter.More(); iter.Next())
        {
            TDF_Tool::Entry(iter.Value(), asciiEntry);
        }
        if (asciiEntry.IsEmpty())
        {
            return false;
        }
        entry = asciiEntry.ToCString();

        return true;
    }
    catch (...)
    {
    }

    return false;
}

std::string LabelUtilities::getLabelPID(const TDF_Label& label)
{
    TCollection_AsciiString entryInstance;
    TDF_Tool::Entry(label, entryInstance);
    return entryInstance.ToCString();
}

TDF_LabelList LabelUtilities::findEdgeOwnerFaces(const TDF_Label& ownerLabel, const TopoDS_Shape& edge)
{
    TDF_LabelList faceList;

    TDF_ChildIterator childIter(ownerLabel);
    for (; childIter.More(); childIter.Next())
    {
        TopoDS_Shape shape = getLabelShape(childIter.Value());
        if (shape.IsNull())
        {
            continue;
        }
        else
        {
            if (shape.ShapeType() == TopAbs_FACE)
            {
                TopExp_Explorer edgeExp(edge, TopAbs_EDGE);
                for (; edgeExp.More(); edgeExp.Next())
                {
                    if (edgeExp.Value().IsSame(edge))
                    {
                        faceList.Append(childIter.Value());
                        break;
                    }
                }
            }
        }

		const TDF_Label& child = childIter.Value();
		TDF_LabelList findList = findEdgeOwnerFaces(child, edge);
        faceList.Append(findList);
    }

    return faceList;
}

TDF_Label LabelUtilities::createNewChild(const TDF_Label& parent, const std::string& name)
{
    TDF_Label newLabel = TDF_TagSource::NewChild(parent);
    // set name
    TDataStd_Name::Set(newLabel, name.c_str());
    // create property label
    TDF_Label propLabel = TDF_TagSource::NewChild(newLabel);
    TDataStd_Name::Set(propLabel, "property");

    return newLabel;
}

TDF_Label LabelUtilities::createNewChild(const TDF_Label& parent)
{
    TDF_Label newLabel = TDF_TagSource::NewChild(parent);
    // create property label
    TDF_Label propLabel = TDF_TagSource::NewChild(newLabel);
    TDataStd_Name::Set(propLabel, "property");

    return newLabel;
}

void LabelUtilities::createLabelNameWithShapeType(TopoDS_Shape shape, const int& id, std::string& rootName)
{
	// add shape to top label
	std::string typeName;
	// add name to top label
	switch (shape.ShapeType())
	{
	case TopAbs_COMPOUND:
	{
		typeName = "compound";
		break;
	}
	case TopAbs_COMPSOLID:
	{
		typeName = "compsolid";
		break;
	}
	case TopAbs_SOLID:
	{
		typeName = "solid";
		break;
	}
	case TopAbs_SHELL:
	{
		typeName = "shell";
		break;
	}
	case TopAbs_FACE:
	{
		typeName = "face";
		break;
	}
	case TopAbs_WIRE:
	{
		typeName = "wire";
		break;
	}
	case TopAbs_EDGE:
	{
		typeName = "edge";
		break;
	}
	case TopAbs_VERTEX:
	{
		typeName = "vertex";
		break;
	}
	default:
		break;
	}
	rootName.append("_" + typeName + "_" + std::to_string(id));
}

bool LabelUtilities::getLabelProperty(const TDF_Label& label, const std::string& propertyName, int& value)
{
    bool found = false;
    try
    {
        if (!label.IsNull())
        {
            // find property label
            TDF_ChildIterator childIter(label);
            for (; childIter.More(); childIter.Next())
            {
                TDF_Label child = childIter.Value();
                Handle(TDataStd_Name) name;
                if (child.FindAttribute(TDataStd_Name::GetID(), name))
                {
                    if (!name->Get().IsDifferent(TCollection_ExtendedString("property")))
                    {
                        // find label in name of 'propertyName'
                        TDF_ChildIterator grandsonIter(child);
                        for (; grandsonIter.More(); grandsonIter.Next())
                        {
                            TDF_Label grandson = grandsonIter.Value();
                            Handle(TDataStd_Name) grandsonName;
                            if (grandson.FindAttribute(TDataStd_Name::GetID(), grandsonName))
                            {
                                if (!grandsonName->Get().IsDifferent(TCollection_ExtendedString(Standard_CString(propertyName.c_str()))))
                                {
                                    // find property value
                                    Handle(TDataStd_Integer) targetValue;
                                    if (grandson.FindAttribute(TDataStd_Integer::GetID(), targetValue))
                                    {
                                        value = targetValue->Get();
                                        found = true;

                                        return found;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch (...)
    {
    }

    return found;
}

bool LabelUtilities::getLabelProperty(const TDF_Label& label, const std::string& propertyName, std::string& value)
{
    bool found = false;
    try
    {
        if (!label.IsNull())
        {
            if (propertyName == "ATTRIB_XACIS_NAME")
            {
                Handle(TDataStd_Name) namedData;
                if (label.FindAttribute(TDataStd_Name::GetID(), namedData))
                {
                    Standard_Integer aLen = namedData->Get().LengthOfCString();
                    Standard_PCharacter aCstr = (Standard_PCharacter)Standard::Allocate(aLen + 1);
                    namedData->Get().ToUTF8CString(aCstr);
                    value.assign(aCstr);
                    found = true;
                }
            }
            else // find other property label
            {
                TDF_ChildIterator childIter(label);
                for (; childIter.More(); childIter.Next())
                {
                    TDF_Label child = childIter.Value();
                    Handle(TDataStd_Name) name;
                    if (child.FindAttribute(TDataStd_Name::GetID(), name))
                    {
                        if (!name->Get().IsDifferent(TCollection_ExtendedString("property")))
                        {
                            // find label in name of 'propertyName'
                            TDF_ChildIterator grandsonIter(child);
                            for (; grandsonIter.More(); grandsonIter.Next())
                            {
                                TDF_Label grandson = grandsonIter.Value();
                                Handle(TDataStd_Name) grandsonName;
                                if (grandson.FindAttribute(TDataStd_Name::GetID(), grandsonName))
                                {
                                    if (!grandsonName->Get().IsDifferent(TCollection_ExtendedString(Standard_CString(propertyName.c_str()))))
                                    {
                                        // find property value
                                        Handle(TDataStd_AsciiString) targetValue;
                                        if (grandson.FindAttribute(TDataStd_AsciiString::GetID(), targetValue))
                                        {
                                            value = targetValue->Get().ToCString();
                                            found = true;

                                            return found;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch (...)
    {
    }

    return found;
}

bool LabelUtilities::getLabelProperty(const TDF_Label& label, const std::string& propertyName, double& value)
{
    bool found = false;
    try
    {
        if (!label.IsNull())
        {
            // find property label
            TDF_ChildIterator childIter(label);
            for (; childIter.More(); childIter.Next())
            {
                TDF_Label child = childIter.Value();
                Handle(TDataStd_Name) name;
                if (child.FindAttribute(TDataStd_Name::GetID(), name))
                {
                    if (!name->Get().IsDifferent(TCollection_ExtendedString("property")))
                    {
                        // find label in name of 'propertyName'
                        TDF_ChildIterator grandsonIter(child);
                        for (; grandsonIter.More(); grandsonIter.Next())
                        {
                            TDF_Label grandson = grandsonIter.Value();
                            Handle(TDataStd_Name) grandsonName;
                            if (grandson.FindAttribute(TDataStd_Name::GetID(), grandsonName))
                            {
                                if (!grandsonName->Get().IsDifferent(TCollection_ExtendedString(Standard_CString(propertyName.c_str()))))
                                {
                                    // find property value
                                    Handle(TDataStd_Real) targetValue;
                                    if (grandson.FindAttribute(TDataStd_Real::GetID(), targetValue))
                                    {
                                        value = targetValue->Get();
                                        found = true;

                                        return found;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    catch (...)
    {
    }

    return found;
}

bool LabelUtilities::addLabelProperty(const TDF_Label& label, const std::string& propertyName, const int& value)
{
    bool success = false;
    try
    {
        if (!label.IsNull())
        {
            // find property label
            TDF_ChildIterator childIter(label);
            for (; childIter.More(); childIter.Next())
            {
                TDF_Label child = childIter.Value();
                Handle(TDataStd_Name) name;
                if (child.FindAttribute(TDataStd_Name::GetID(), name))
                {
                    if (!name->Get().IsDifferent(TCollection_ExtendedString("property")))
                    {
                        // find label in name of 'propertyName'
                        TDF_ChildIterator grandsonIter(child);
                        for (; grandsonIter.More(); grandsonIter.Next())
                        {
                            TDF_Label grandson = grandsonIter.Value();
                            Handle(TDataStd_Name) grandsonName;
                            if (grandson.FindAttribute(TDataStd_Name::GetID(), grandsonName))// 
                            {
                                if (!grandsonName->Get().IsDifferent(TCollection_ExtendedString(Standard_CString(propertyName.c_str()))))// property already exists
                                {
                                    //reset
                                    TDataStd_Integer::Set(grandson, value);
                                    success = true;
                                    break;
                                }
                            }
                        }

                        if (!success)
                        {
                            TDF_Label grandson = TDF_TagSource::NewChild(child);
                            TDataStd_Name::Set(grandson, TCollection_AsciiString(propertyName.c_str()));
                            TDataStd_Integer::Set(grandson, value);
                            success = true;
                        }
                    }
                }

                if (success)
                {
                    break;
                }
            }
        }
    }
    catch (...)
    {
    }

    return success;
}

bool LabelUtilities::addLabelProperty(const TDF_Label& label, const std::string& propertyName, const std::string& value)
{
    bool success = false;
    try
    {
        if (!label.IsNull())
        {
            if (propertyName == "ATTRIB_XACIS_NAME")
            {
                TDataStd_Name::Set(label, TCollection_AsciiString(value.c_str()));
            }
            else // add other property label
            {
                TDF_ChildIterator childIter(label);
                for (; childIter.More(); childIter.Next())
                {
                    TDF_Label child = childIter.Value();
                    Handle(TDataStd_Name) name;
                    if (child.FindAttribute(TDataStd_Name::GetID(), name))
                    {
                        if (!name->Get().IsDifferent(TCollection_ExtendedString("property")))
                        {
                            // find label in name of 'propertyName'
                            TDF_ChildIterator grandsonIter(child);
                            for (; grandsonIter.More(); grandsonIter.Next())
                            {
                                TDF_Label grandson = grandsonIter.Value();
                                Handle(TDataStd_Name) grandsonName;
                                if (grandson.FindAttribute(TDataStd_Name::GetID(), grandsonName))// 
                                {
                                    if (!grandsonName->Get().IsDifferent(TCollection_ExtendedString(Standard_CString(propertyName.c_str()))))// property already exists
                                    {
                                        //reset
                                        TDataStd_AsciiString::Set(grandson, TCollection_AsciiString(value.c_str()));
                                        success = true;
                                        break;
                                    }
                                }
                            }

                            if (!success)
                            {
                                TDF_Label grandson = TDF_TagSource::NewChild(child);
                                TDataStd_Name::Set(grandson, TCollection_AsciiString(propertyName.c_str()));
                                TDataStd_AsciiString::Set(grandson, TCollection_AsciiString(value.c_str()));
                                success = true;
                            }
                        }
                    }

                    if (success)
                    {
                        break;
                    }
                }
            }
        }
    }
    catch (...)
    {
    }

    return success;
}

bool LabelUtilities::addLabelProperty(const TDF_Label& label, const std::string& propertyName, const double& value)
{
    bool success = false;
    try
    {
        if (!label.IsNull())
        {
            // find property label
            TDF_ChildIterator childIter(label);
            for (; childIter.More(); childIter.Next())
            {
                TDF_Label child = childIter.Value();
                Handle(TDataStd_Name) name;
                if (child.FindAttribute(TDataStd_Name::GetID(), name))
                {
                    if (!name->Get().IsDifferent(TCollection_ExtendedString("property")))
                    {
                        // find label in name of 'propertyName'
                        TDF_ChildIterator grandsonIter(child);
                        for (; grandsonIter.More(); grandsonIter.Next())
                        {
                            TDF_Label grandson = grandsonIter.Value();
                            Handle(TDataStd_Name) grandsonName;
                            if (grandson.FindAttribute(TDataStd_Name::GetID(), grandsonName))// 
                            {
                                if (!grandsonName->Get().IsDifferent(TCollection_ExtendedString(Standard_CString(propertyName.c_str()))))// property already exists
                                {
                                    //reset
                                    TDataStd_Real::Set(grandson, value);
                                    success = true;
                                    break;
                                }
                            }
                        }

                        if (!success)
                        {
                            TDF_Label grandson = TDF_TagSource::NewChild(child);
                            TDataStd_Name::Set(grandson, TCollection_AsciiString(propertyName.c_str()));
                            TDataStd_Real::Set(grandson, value);
                            success = true;
                        }
                    }
                }

                if (success)
                {
                    break;
                }
            }
        }
    }
    catch (...)
    {
    }

    return success;
}

bool LabelUtilities::removeLabelProperty(const TDF_Label& label, const std::string& propertyName)
{
    bool found = false;
    try
    {
        if (!label.IsNull())
        {
            // find property label
            TDF_ChildIterator childIter(label);
            for (; childIter.More(); childIter.Next())
            {
                TDF_Label child = childIter.Value();
                Handle(TDataStd_Name) name;
                if (child.FindAttribute(TDataStd_Name::GetID(), name))
                {
                    if (!name->Get().IsDifferent(TCollection_ExtendedString("property")))
                    {
                        // find label in name of 'propertyName'
                        TDF_ChildIterator grandsonIter(child);
                        for (; grandsonIter.More(); grandsonIter.Next())
                        {
                            TDF_Label grandson = grandsonIter.Value();
                            grandson.ForgetAllAttributes();
                            found = true;
                            return found;
                        }
                    }
                }
            }
        }
    }
    catch (...)
    {
    }

    return found;
}

bool LabelUtilities::getLabelName(const TDF_Label& label, std::string& name)
{
    try
    {
        Handle(TDataStd_Name) namedData;
        if (!label.FindAttribute(TDataStd_Name::GetID(), namedData))
        {
            return false;
        }

        int a = 0;
        Standard_Integer aLen = namedData->Get().LengthOfCString();
        Standard_PCharacter aCstr = (Standard_PCharacter)Standard::Allocate(aLen + 1);
        namedData->Get().ToUTF8CString(aCstr);
        name.assign(aCstr);

        return true;
    }
    catch (...)
    {
    }

    return false;
}

bool LabelUtilities::setLabelName(const TDF_Label& label, const std::string& name)
{
    try
    {
        TDataStd_Name::Set(label, name.c_str());
        return true;
    }
    catch (...)
    {
    }

    return false;
}

bool LabelUtilities::removeLabelName(const TDF_Label& label)
{
    try
    {
        label.ForgetAttribute(TDataStd_Name::GetID());

        return true;
    }
    catch (...)
    {
    }

    return false;
}

TopoDS_Shape LabelUtilities::getLabelShape(const TDF_Label& label)
{
    TopoDS_Shape shape;
    Handle(TNaming_NamedShape) namedData;
    if (!label.FindAttribute(TNaming_NamedShape::GetID(), namedData))
    {
        return shape;
    }

    return namedData->Get();
}

bool LabelUtilities::findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape, LabelInfo& info)
{
	bool found = false;
    try
    {
	    NCollection_DataMap<TopoDS_Shape, LabelInfo>::Iterator iter(shapeMap);
	    for (;iter.More();iter.Next())
	    {
		    if (iter.Key().IsSame(shape))
		    {
			    info = iter.Value();
			    found = true;
			    break;
		    }
	    }
    }
    catch (...)
    {
    }

	return found;
}

bool LabelUtilities::findShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape)
{
    bool found = false;

    try
    {
        NCollection_DataMap<TopoDS_Shape, LabelInfo>::Iterator iter(shapeMap);
        for (; iter.More(); iter.Next())
        {
            if (iter.Key().IsSame(shape))
            {
                found = true;
                break;
            }
        }
    }
    catch (...)
    {
    }

    return found;
}

void LabelUtilities::removeShapeInMap(NCollection_DataMap<TopoDS_Shape, LabelInfo>& shapeMap, TopoDS_Shape shape)
{
    NCollection_DataMap<TopoDS_Shape, LabelInfo>::Iterator iter(shapeMap);
    for (;iter.More();iter.Next())
    {
        if (iter.Key().IsSame(shape))
        {
            shapeMap.UnBind(iter.Key());
            break;
        }
    }
}

void LabelUtilities::recursiveRenameChildLabelWithSameName(const TDF_Label& fatherLabel)
{
    renameChildLabelWithSameName(fatherLabel);

    TDF_ChildIterator childIter(fatherLabel);
    for (; childIter.More(); childIter.Next())
    {
        renameChildLabelWithSameName(childIter.Value());
    }
}

void LabelUtilities::renameChildLabelWithSameName(const TDF_Label& fatherLabel)
{
    std::map<std::string, int> totalNameCalledTimes;
    // collect name info.
    TDF_ChildIterator childIter(fatherLabel);
    for (; childIter.More(); childIter.Next())
    {
        std::string name;
        if (getLabelName(childIter.Value(), name))
        {
            if (totalNameCalledTimes.find(name) != totalNameCalledTimes.end())
            {
                name.append("_" + std::to_string(totalNameCalledTimes.at(name)++));
                TDataStd_Name::Set(childIter.Value(), name.c_str());
            }
            else
            {
                totalNameCalledTimes.insert(std::pair<std::string, int>(name, 1));
            }
        }
    }
}

bool LabelUtilities::findFatherLabelAccordingToType(TDF_Label currentlabel,
    TopAbs_ShapeEnum type, TDF_Label& findlabel)
{
    TopoDS_Shape thisshape = getLabelShape(currentlabel);
    if (thisshape.ShapeType() > type)
    {
        TDF_Label fatherlabel = currentlabel.Father();
        findFatherLabelAccordingToType(fatherlabel, type, findlabel);
    }
    else if (thisshape.ShapeType() == type)
        findlabel = currentlabel;
    return true;
}

bool LabelUtilities::FindTypeLevelFatherLabel(TDF_Label& inputlabel, TopAbs_ShapeEnum type, TopoDS_Shape& currentshape)
{
	TopoDS_Shape testshape = getLabelShape(inputlabel);
	TopAbs_ShapeEnum currenttype = testshape.ShapeType();
	if (currenttype > type)
	{
		inputlabel = inputlabel.Father();
		FindTypeLevelFatherLabel(inputlabel, type, currentshape);
		// ÷∂Ø÷’÷πµ›πÈ
		if (!currentshape.IsNull())
			return true;
		currenttype = testshape.ShapeType();
	}
	else if (currenttype == type)
	{
		currentshape = getLabelShape(inputlabel);
		return true;
	}
	return false;
}