#include "LabelShapeAdapter.h"
#include "LabelUtilities.h"

#include <string>

#include <TNaming_NamedShape.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TNaming_Builder.hxx>
#include <TopExp_Explorer.hxx>
#include <TDataStd_Name.hxx>
#include <Standard_PCharacter.hxx>
#include <Standard_ExtString.hxx>
#include <TopOpeBRepBuild_ShapeSet.hxx>
#include <TopoDS_Iterator.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDF_Reference.hxx>
#include <TDF_TagSource.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDataStd_Integer.hxx>
#include <TNaming_Tool.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BOPAlgo_MakerVolume.hxx>
	

using namespace std;

// #define noUseVertex



LabelShapeAdapter::LabelShapeAdapter()
    :m_supportType(TopAbs_SHAPE)
{
}


LabelShapeAdapter::~LabelShapeAdapter()
{
}

void LabelShapeAdapter::allocateLabelWithTopShape(const TDF_Label& fatherLabel, const TopoDS_Shape& shape, const int& numBrothers, const bool& topLevel)
{
	if (topLevel)
    {
        TNaming_Builder topNamingBuilder(fatherLabel);
        topNamingBuilder.Generated(shape);
//         TDataStd_Name::Set(fatherLabel, m_name.c_str());
		for (TopoDS_Iterator iter(shape); iter.More(); iter.Next())
		{
			allocateLabelWithTopShape(fatherLabel, iter.Value(), shape.NbChildren() - 1, false);
		}
    }
    else
    {
		if (shape.ShapeType() == TopAbs_COMPSOLID || shape.ShapeType() == TopAbs_SHELL || shape.ShapeType() == TopAbs_WIRE)
		{
			for (TopoDS_Iterator iter(shape); iter.More(); iter.Next())
			{
				allocateLabelWithTopShape(fatherLabel, iter.Value(), shape.NbChildren() - 1, false);
			}
		}
#ifdef noUseVertex
        else if (shape.ShapeType() > TopAbs_VERTEX)
        {
            return;
        }
#endif // noUseVertex
		else
		{
			if (numBrothers > 0)
			{
				LabelInfo foundLabel;
				if (LabelUtilities::findShapeInMap(m_modifiedShapes,shape, foundLabel)&&!LabelUtilities::findShapeLabelFromRoot(m_topLabel,shape))
				{
					TDF_Label childLabel;
					if (foundLabel.label.IsNull())
					{
						childLabel = LabelUtilities::createNewChild(fatherLabel);
						TNaming_Builder topNamingBuilder(childLabel);
						topNamingBuilder.Generated(shape);
						std::string newName = m_name;
						LabelUtilities::createLabelNameWithShapeType(shape, foundLabel.calledTimes, newName);
// 						TDataStd_Name::Set(childLabel, newName.c_str());
					}
					else
					{
						childLabel = LabelUtilities::createNewChild(fatherLabel);
						LabelUtilities::copyLabelWithOnlyPropertyChild(foundLabel.label, childLabel);
						TNaming_Builder topNamingBuilder(childLabel);
						topNamingBuilder.Generated(shape);
					}

					++foundLabel.calledTimes;
					for (TopoDS_Iterator iter(shape); iter.More(); iter.Next())
					{
						allocateLabelWithTopShape(childLabel, iter.Value(), shape.NbChildren() - 1, false);
					}
				}
			}
			else
			{
				if (shape.ShapeType() == TopAbs_SOLID)
				{
					for (TopoDS_Iterator iter(shape); iter.More(); iter.Next())
					{
						allocateLabelWithTopShape(fatherLabel, iter.Value(), shape.NbChildren() - 1, false);
					}
				}
				else
				{
					LabelInfo foundLabel;
					if (LabelUtilities::findShapeInMap(m_modifiedShapes, shape, foundLabel) && !LabelUtilities::findShapeLabelFromRoot(m_topLabel, shape))
					{
						TDF_Label childLabel;
						if (foundLabel.label.IsNull())
						{
							childLabel = LabelUtilities::createNewChild(fatherLabel);
							TNaming_Builder topNamingBuilder(childLabel);
							topNamingBuilder.Generated(shape);
							std::string newName = m_name;
							LabelUtilities::createLabelNameWithShapeType(shape, foundLabel.calledTimes, newName);
// 							TDataStd_Name::Set(childLabel, newName.c_str());
						}
						else
						{
							childLabel = LabelUtilities::createNewChild(fatherLabel);
							LabelUtilities::copyLabelWithOnlyPropertyChild(foundLabel.label, childLabel);
							TNaming_Builder topNamingBuilder(childLabel);
							topNamingBuilder.Generated(shape);
						}

						++foundLabel.calledTimes;
						for (TopoDS_Iterator iter(shape); iter.More(); iter.Next())
						{
							allocateLabelWithTopShape(childLabel, iter.Value(), shape.NbChildren() - 1, false);
						}
					}
				}
			}
		}
    }
}

void LabelShapeAdapter::allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape, const std::string& name, const int& id)
{
    // add shape to top label
    TNaming_Builder topNamingBuilder(label);
    topNamingBuilder.Generated(shape);
//     // add name to top label
// 	std::string selfName = name;
// 	LabelUtilities::createLabelNameWithShapeType(shape, id, selfName);
//     TDataStd_Name::Set(label, selfName.c_str());

    std::string subName;
    int number = 1;
    for (TopoDS_Iterator iter(shape); iter.More(); iter.Next())
    {
        TopoDS_Shape subShape = iter.Value();
        TopAbs_ShapeEnum subShapeType = subShape.ShapeType();
        if (subShapeType == TopAbs_COMPSOLID || subShapeType == TopAbs_SHELL || subShapeType == TopAbs_WIRE)//ignore compsolid, shell and wire
        {
            for (TopoDS_Iterator iter2(subShape); iter2.More(); iter2.Next())
            {
                TDF_Label childLabel = LabelUtilities::createNewChild(label);
                allocateLabelWithTopShape(childLabel, iter2.Value(), name, number++);
            }
        }
#ifdef noUseVertex
        else if (subShapeType >TopAbs_VERTEX)//ignore edge and vertex except its father is compound
        {
            if (shape.ShapeType() == TopAbs_COMPOUND)
            {
                TDF_Label childLabel = LabelUtilities::createNewChild(label);
                allocateLabelWithTopShape(childLabel, iter.Value(), name, number++);
            }
            else
            {
                return;
            }
        }
#endif
        else // normal allocation
        {
            TDF_Label childLabel = LabelUtilities::createNewChild(label);
            allocateLabelWithTopShape(childLabel, subShape, name, number++);
        }
    }
}

void LabelShapeAdapter::allocateLabelWithTopShape(const TDF_Label& label, const TopoDS_Shape& shape)
{
    // add shape to top label
    TopAbs_ShapeEnum type2 = shape.ShapeType();
    TNaming_Builder topNamingBuilder(label);
    topNamingBuilder.Generated(shape);

    TopoDS_Iterator iter(shape);
    for (; iter.More(); iter.Next())
    {
#ifdef noUseVertex
        if (iter.Value().ShapeType() == TopAbs_VERTEX)
        {
            continue;
        }
#endif
		TDF_Label matchedLabel;
        if (!LabelUtilities::findShapeLabelFromRoot(label.Root(), iter.Value(), matchedLabel))
        {
            TDF_Label childLabel = LabelUtilities::createNewChild(label);
            allocateLabelWithTopShape(childLabel, iter.Value());
        }
// 		else//link to exist 
// 		{
// 			Handle(TDataStd_TreeNode) fatherTreenode;
// 			Handle(TDataStd_TreeNode) childTreenode;
// 
// 			if (!label.FindAttribute(XCAFDoc::ShapeRefGUID(), fatherTreenode))//no child treenode
// 			{
// 				fatherTreenode = new TDataStd_TreeNode;
// 			}
// 			if (!matchedLabel.FindAttribute(XCAFDoc::ShapeRefGUID(), childTreenode))//no child treenode
// 			{
// 				childTreenode = new TDataStd_TreeNode;
// 			}
// 			fatherTreenode->Append(childTreenode);
// 		}
    }
}

void LabelShapeAdapter::replaceLabelPropertyWithName(const TDF_Label& sourceLabel, const TDF_Label& targetRootLabel)
{
	std::string name;
	if (LabelUtilities::getLabelName(sourceLabel, name))
	{
		if (name == "property")
		{
			return;
		}
		
		TDF_Label targetLabel;
		if (LabelUtilities::findNameLabel(targetRootLabel, name, targetLabel))
		{
			LabelUtilities::copyLabelProperty(sourceLabel, targetLabel);
		}
	}

	TDF_ChildIterator childIter(sourceLabel);
	for (; childIter.More(); childIter.Next())
	{
		replaceLabelPropertyWithName(childIter.Value(), targetRootLabel);
	}
}

void LabelShapeAdapter::replaceLabelWithChangedShape(const TDF_Label& label, const TopoDS_Shape& changedShape, const std::string& name, const int& id)
{
    // add shape to top label
    TNaming_Builder topNamingBuilder(label);
    topNamingBuilder.Generated(changedShape);

    std::string subName;
    int number = 1;
    TDF_ChildIterator childIter(label);
    TopoDS_Iterator shapeIter(changedShape);
    for (; shapeIter.More(); shapeIter.Next())
    {
        TopoDS_Shape subShape = shapeIter.Value();
        TDF_Label childLabel = childIter.Value();
        TopAbs_ShapeEnum subShapeType = subShape.ShapeType();
        if (subShapeType == TopAbs_COMPSOLID || subShapeType == TopAbs_SHELL || subShapeType == TopAbs_WIRE)//ignore compsolid, shell and wire
        {
            for (TopoDS_Iterator iter2(subShape); iter2.More(); iter2.Next())
            {
                replaceLabelWithChangedShape(childLabel, iter2.Value(), name, number++);
            }
        }
#ifdef noUseVertex
        else if (subShapeType > TopAbs_VERTEX)//ignore edge and vertex except its father is compound
        {
            if (changedShape.ShapeType() == TopAbs_COMPOUND)
            {
                replaceLabelWithChangedShape(childLabel, shapeIter.Value(), name, number++);
            }
            else
            {
                return;
            }
        }
#endif
        else // normal allocation
        {
            replaceLabelWithChangedShape(childLabel, subShape, name, number++);
        }

        if (childIter.More())//能匹配上
        {

            childIter.Next();
        }
        else//label比shape少
        {
            break;
        }
    }

    if (!shapeIter.More()&& !childIter.More())//label和shape一样多
    {
        return;
    }
    else if (shapeIter.More())//label比shape少
    {
        for (; shapeIter.More(); shapeIter.Next())
        {
            TopoDS_Shape subShape = shapeIter.Value();
            TopAbs_ShapeEnum subShapeType = subShape.ShapeType();
            if (subShapeType == TopAbs_COMPSOLID || subShapeType == TopAbs_SHELL || subShapeType == TopAbs_WIRE)//ignore compsolid, shell and wire
            {
                for (TopoDS_Iterator iter2(subShape); iter2.More(); iter2.Next())
                {
                    TDF_Label childLabel = LabelUtilities::createNewChild(label);
                    allocateLabelWithTopShape(childLabel, iter2.Value(), name, number++);
                }
            }
#ifdef noUseVertex
            else if (subShapeType > TopAbs_VERTEX)//ignore edge and vertex except its father is compound
            {
                if (changedShape.ShapeType() == TopAbs_COMPOUND)
                {
                    TDF_Label childLabel = LabelUtilities::createNewChild(label);
                    allocateLabelWithTopShape(childLabel, shapeIter.Value(), name, number++);
                }
                else
                {
                    return;
                }
            }
#endif
            else // normal allocation
            {
                TDF_Label childLabel = LabelUtilities::createNewChild(label);
                allocateLabelWithTopShape(childLabel, subShape, name, number++);
            }
        }
    }
    else //label比shape多
    {
        for (; childIter.More(); childIter.Next())
        {
            childIter.Value().ForgetAllAttributes();
        }
    }
}

void LabelShapeAdapter::replaceChildLabelWithCopiedShape(const TDF_Label& label, const TopoDS_Shape& copiedShape)
{
    // add shape to top label
//     TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
    TNaming_Builder topNamingBuilder(label);
    topNamingBuilder.Generated(copiedShape);
//     topNamingBuilder.Modify(origShape, copiedShape);

    TDF_ChildIterator childIter(label);
    TopoDS_Iterator shapeIter(copiedShape);
    for (; shapeIter.More(); shapeIter.Next())
    {
        //确定没有重复shape
        TopoDS_Shape subShape = shapeIter.Value();
        if (LabelUtilities::findShapeLabelFromRoot(label.Root(), subShape))
        {
            continue;
        }
//         TDF_Label childLabel = childIter.Value();
        std::string labelName;
        if (LabelUtilities::getLabelName(childIter.Value(), labelName) && labelName == "property")//跳过属性节点
        {
            childIter.Next();
        }

        if (!childIter.More())
        {
            return;
        }

        replaceChildLabelWithCopiedShape(childIter.Value(), subShape);
        childIter.Next();
    }
}

void LabelShapeAdapter::replaceChildLabelWithCopiedShape1(const TDF_Label& label, const TopoDS_Shape& copiedShape)
{
    Handle(TNaming_NamedShape) namedShape;
    if (label.FindAttribute(TNaming_NamedShape::GetID(), namedShape))
    {
        TopoDS_Shape shape = namedShape->Get();
        if (shape.IsNull())
        {
            return;
        }

        TopAbs_ShapeEnum shapeType = shape.ShapeType();
        TopExp_Explorer exp(copiedShape, shapeType);
        for (; exp.More(); exp.Next())
        {
            if (exp.Value().IsPartner(shape))
            {
                TNaming_Builder newBuilder(label);
                newBuilder.Generated(exp.Value());
                break;
            }
        }
    }
    else
    {
        return;
    }

    TDF_ChildIterator childIter(label);
    for (; childIter.More(); childIter.Next())
    {
        replaceChildLabelWithCopiedShape1(childIter.Value(), copiedShape);
    }
}

void LabelShapeAdapter::replaceChildLabelWithCopiedShape2(const TDF_Label& label, const TopoDS_Shape& copiedShape)
{
    Handle(TNaming_NamedShape) namedShape;
    if (label.FindAttribute(TNaming_NamedShape::GetID(), namedShape))
    {
        TopoDS_Shape shape = namedShape->Get();
        if (shape.IsNull())
        {
            return;
        }

        TopAbs_ShapeEnum shapeType = shape.ShapeType();
        TopExp_Explorer exp(copiedShape, shapeType);
        for (; exp.More(); exp.Next())
        {
            if (exp.Value().IsSame(shape))
            {
                TNaming_Builder newBuilder(label);
                newBuilder.Generated(exp.Value());
                break;
            }
        }
    }
    else
    {
        return;
    }

    TDF_ChildIterator childIter(label);
    for (; childIter.More(); childIter.Next())
    {
        replaceChildLabelWithCopiedShape2(childIter.Value(), copiedShape);
    }
}

void LabelShapeAdapter::buildEntityAttributeName(const TDF_Label& orgLabel, const TDF_Label& curLabel, bool bIsNewCreate, bool bIsCloneCS, bool bNeedCopyFaceName, int& iNextFaceID, int& iNextEdgeID)
{
    if (curLabel.IsNull())
    {
        return;
    }

    bool bCopyName;
    if (!orgLabel.IsNull()&&bIsNewCreate)//继承
    {
        bCopyName = true;
    }
    else//不继承
    {

    }

    std::string curLabelName, orgLabelName, finalName;
    bool nameFound = LabelUtilities::getLabelName(curLabel, curLabelName);
    bool orgNameFound = LabelUtilities::getLabelName(orgLabel, orgLabelName);
    Handle(TNaming_NamedShape) namedShapeData;
    if (curLabel.FindAttribute(TNaming_NamedShape::GetID(), namedShapeData))

    //属性继承
    if (nameFound&&orgNameFound&&curLabelName == "property"&&orgLabelName == "property")
    {
        LabelUtilities::copyLabelProperty(orgLabel, curLabel);
    }
    if (namedShapeData->Get().ShapeType() == TopAbs_FACE)//只有面和线才继承和新建名称
    {
        //子节点名称处理
        if (nameFound != false && !bIsCloneCS)
        {
            finalName = curLabelName;
            LabelUtilities::removeLabelName(curLabel);
        }
        //针对Clone的对象，如果面上存在名称，通需要继承，进行面名称继承
        else if (nameFound != false && bNeedCopyFaceName)
        {
            finalName = curLabelName;
            LabelUtilities::removeLabelName(curLabel);
        }
        else if (bCopyName)
        {
            if (orgNameFound)
            {
                finalName = orgLabelName;
            }
            else
            {
                finalName = to_string(iNextFaceID);
            }
        }
        else
        {
            finalName = to_string(iNextFaceID);
        }
        LabelUtilities::setLabelName(curLabel, finalName);
    }
    else if (namedShapeData->Get().ShapeType() == TopAbs_EDGE)//只有面和线才继承和新建名称
    {
        //针对Clone的对象，如果面上存在名称，通需要继承，进行面名称继承
        if (nameFound != false && bNeedCopyFaceName)
        {
            finalName = curLabelName;
            LabelUtilities::removeLabelName(curLabel);
        }
        else if (bCopyName)
        {
            if (orgNameFound)
            {
                finalName = orgLabelName;
            }
            else
            {
                finalName = to_string(iNextEdgeID);
            }
        }
        else
        {
            finalName = to_string(iNextEdgeID);
        }
        LabelUtilities::setLabelName(curLabel, finalName);
    }

    //递归拷贝
    TDF_ChildIterator curFaceIter(curLabel);
    if (orgLabel.IsNull())
    {
        for (; curFaceIter.More(); curFaceIter.Next())
        {
            buildEntityAttributeName(orgLabel, curFaceIter.Value(), bIsNewCreate, bIsCloneCS, bNeedCopyFaceName, iNextFaceID, iNextEdgeID);
        }
    }
    else
    {
        TDF_ChildIterator orgFaceIter(orgLabel);
        for (; curFaceIter.More(); curFaceIter.Next(), orgFaceIter.Next())
        {
            buildEntityAttributeName(orgFaceIter.Value(), curFaceIter.Value(), bIsNewCreate, bIsCloneCS, bNeedCopyFaceName, iNextFaceID, iNextEdgeID);
        }
    }
}

void LabelShapeAdapter::collectRelatedLabelAndShapes(const TDF_Label& label, const EnAdapatType& type, void* operation)
{
    TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
    if (origShape.IsNull())
    {
        return;
    }

    LabelInfo foundLabel;

    //new method
    BRepBuilderAPI_MakeShape* api = (BRepBuilderAPI_MakeShape*)operation;
    if (!api)
    {
        return;
    }

    bool supportType = true;
    if (type >= EnCut && type <= EnFuse && !isSupportedType(origShape))
    {
        supportType = false;
    }

    if (supportType)
    {
        //find modified shape
        const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
        TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
        for (; ShapesIterator.More(); ShapesIterator.Next())
        {
            const TopoDS_Shape& newShape = ShapesIterator.Value();
            if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
            {
                foundLabel.label = label;
                foundLabel.calledTimes = 0;
                m_modifiedShapes.Bind(newShape, foundLabel);
            }
        }

        //find modified shape
        const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
        ShapesIterator.Init(generatedShapes);
        for (; ShapesIterator.More(); ShapesIterator.Next())
        {
            const TopoDS_Shape& newShape = ShapesIterator.Value();
            if (!LabelUtilities::findShapeInMap(m_generatedShapes, newShape))
            {
                foundLabel.label = label;
                foundLabel.calledTimes = 0;
                m_generatedShapes.Bind(newShape, foundLabel);
            }
        }
    }

    //find self in map
    if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape)
        && !LabelUtilities::findShapeInMap(m_generatedShapes, origShape)
        && !LabelUtilities::findShapeInMap(m_constantShapes, origShape))
    {
        TopoDS_Shape targetRootShape = LabelUtilities::getLabelShape(m_topLabel);
        TopExp_Explorer exp(targetRootShape, origShape.ShapeType());
        for (; exp.More(); exp.Next())
        {
            if (exp.Value() == origShape)
            {
                foundLabel.label = label;
                foundLabel.calledTimes = 0;
                m_constantShapes.Bind(exp.Value(), foundLabel);
            }
        }
    }

    //old method
//     switch (type)
//     {
//     case EnExtrude:
//     case EnRevol:
//     case EnSweep:
//     case EnLoft:
//     {
//         BRepPrimAPI_MakePrism* api = (BRepPrimAPI_MakePrism*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape, foundLabel))
//         {
//             foundLabel.label = label;
//             foundLabel.calledTimes = 0;
//             m_modifiedShapes.Bind(origShape, foundLabel);
// 
//             // 				Utilities::getLabelName(label, m_name);
//         }
// 
//         const TopTools_ListOfShape& Shapes = api->Modified(origShape);
//         TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
//         //             int splitIndex = 0;
//         for (; ShapesIterator.More(); ShapesIterator.Next())
//         {
//             const TopoDS_Shape& newShape = ShapesIterator.Value();
//             // TNaming_Evolution == MODIFY
//             if (!origShape.IsSame(newShape))
//             {
//                 if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape, foundLabel))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_modifiedShapes.Bind(newShape, foundLabel);
//                 }
//             }
//         }
// 
//         break;
//     }
//     case EnCut:
//     case EnCommon:
//     case EnFuse:
//     {
//         BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         if (isSupportedType(origShape))
//         {
//             //find modified shape
//             const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
//             TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_modifiedShapes.Bind(newShape, foundLabel);
//                 }
//             }
// 
//             //find modified shape
//             const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
//             ShapesIterator.Init(generatedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_generatedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_generatedShapes.Bind(newShape, foundLabel);
//                 }
//             }
//         }
// 
//         //find self in map
//         if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape)
//             && !LabelUtilities::findShapeInMap(m_generatedShapes, origShape)
//             && !LabelUtilities::findShapeInMap(m_constantShapes, origShape))
//         {
//             foundLabel.label = label;
//             foundLabel.calledTimes = 0;
//             m_constantShapes.Bind(origShape, foundLabel);
//         }
// 
//         break;
//     }
//     case EnFillet:
//     {
//         BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         if (isSupportedType(origShape))
//         {
//             //find modified shape
//             const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
//             TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_modifiedShapes.Bind(newShape, foundLabel);
//                 }
//             }
// 
//             //find modified shape
//             const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
//             ShapesIterator.Init(generatedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_generatedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_generatedShapes.Bind(newShape, foundLabel);
//                 }
//             }
//         }
// 
//         break;
//     }
//     case EnChamfer:
//     {
//         BRepFilletAPI_MakeChamfer* api = (BRepFilletAPI_MakeChamfer*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         if (isSupportedType(origShape))
//         {
//             //find modified shape
//             const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
//             TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_modifiedShapes.Bind(newShape, foundLabel);
//                 }
//             }
// 
//             //find modified shape
//             const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
//             ShapesIterator.Init(generatedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_generatedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_generatedShapes.Bind(newShape, foundLabel);
//                 }
//             }
//         }
// 
//         break;
//     }
//     case EnFillet2D:
//     {
//         BRepFilletAPI_MakeFillet2d* api = (BRepFilletAPI_MakeFillet2d*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         if (isSupportedType(origShape))
//         {
//             //find modified shape
//             const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
//             TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_modifiedShapes.Bind(newShape, foundLabel);
//                 }
//             }
// 
//             //find modified shape
//             const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
//             ShapesIterator.Init(generatedShapes);
//             for (; ShapesIterator.More(); ShapesIterator.Next())
//             {
//                 const TopoDS_Shape& newShape = ShapesIterator.Value();
//                 if (!LabelUtilities::findShapeInMap(m_generatedShapes, newShape))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_generatedShapes.Bind(newShape, foundLabel);
//                 }
//             }
//         }
// 
//         break;
//     }
//     case EnCellsBuilder:
//     {
//         BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         LabelInfo foundLabel;
//         if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape, foundLabel))
//         {
//             foundLabel.label = label;
//             foundLabel.calledTimes = 0;
//             m_modifiedShapes.Bind(origShape, foundLabel);
// 
//             // 				Utilities::getLabelName(label, m_name);
//         }
// 
//         const TopTools_ListOfShape& Shapes = api->Modified(origShape);
//         TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
//         //             int splitIndex = 0;
//         for (; ShapesIterator.More(); ShapesIterator.Next())
//         {
//             const TopoDS_Shape& newShape = ShapesIterator.Value();
//             // TNaming_Evolution == MODIFY
//             if (newShape.ShapeType() == origShape.ShapeType())
//             {
//                 if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape, foundLabel))
//                 {
//                     foundLabel.label = label;
//                     foundLabel.calledTimes = 0;
//                     m_modifiedShapes.Bind(newShape, foundLabel);
//                 }
//             }
//         }
// 
//         break;
//     }
//     default:
//         break;
//     }

    TDF_ChildIterator childIter(label);
    for (; childIter.More(); childIter.Next())
    {
        TDF_Label child = childIter.Value();
        collectRelatedLabelAndShapes(child, type, operation);
    }
}

void LabelShapeAdapter::labelDiscreteToMap(const TDF_Label& label)
{
    TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
    if (origShape.IsNull())
    {
        return;
    }

    LabelInfo foundLabel;
    foundLabel.label = label;
    foundLabel.calledTimes = 0;
    m_shapeHistoryMap.Bind(origShape, foundLabel);

    TDF_ChildIterator childIter(label);
    for (; childIter.More(); childIter.Next())
    {
        TDF_Label child = childIter.Value();
        labelDiscreteToMap(child);
    }
}

void LabelShapeAdapter::collectRelatedLabelAndShapes(const TopoDS_Shape& origShape,
    const EnAdapatType& type, void* operation)
{
    if (operation == nullptr)
    {
        return;
    }

    LabelInfo foundLabel;
//     if (m_shapeHistoryMap.Find(origShape, foundLabel))
    if (LabelUtilities::findShapeInMap(m_shapeHistoryMap, origShape, foundLabel))
    {
		if (type == EnSew1)
		{
			BRepBuilderAPI_Sewing* api = (BRepBuilderAPI_Sewing*)operation;
			if (!api)
			{
				return;
			}

			bool supportType = true;
			if (supportType)
			{
				//find modified shape
				const TopoDS_Shape& modifiedShape = api->Modified(origShape);
				if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, modifiedShape, foundLabel))
				{
					LabelInfo newFoundLabel = foundLabel;
					newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnModified ? EnModified : EnNotChanged;
					m_shapeHistoryMap.Bind(modifiedShape, newFoundLabel);
				}
			}

			TopExp_Explorer topoExp(origShape, TopAbs_ShapeEnum(origShape.ShapeType() + 1));
			for (; topoExp.More(); topoExp.Next())
			{
				collectRelatedLabelAndShapes(topoExp.Value(), type, operation);
			}
		}
		else if (type == EnSew2)
		{
			BOPAlgo_MakerVolume* api = (BOPAlgo_MakerVolume*)operation;

// 			if (supportType)
// 			{
				//find modified shape
			const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
			TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
			for (; ShapesIterator.More(); ShapesIterator.Next())
			{
				const TopoDS_Shape& newShape = ShapesIterator.Value();
				if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, newShape, foundLabel))
				{
					LabelInfo newFoundLabel = foundLabel;
					newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnModified ? EnModified : EnNotChanged;
					m_shapeHistoryMap.Bind(newShape, newFoundLabel);
				}
			}

			//find modified shape
			const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
			ShapesIterator.Init(generatedShapes);
			for (; ShapesIterator.More(); ShapesIterator.Next())
			{
				const TopoDS_Shape& newShape = ShapesIterator.Value();
				if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, newShape, foundLabel))
				{
					LabelInfo newFoundLabel = foundLabel;
					newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnGenerated ? EnGenerated : EnNotChanged;
					m_shapeHistoryMap.Bind(newShape, newFoundLabel);
				}
			}
// 			}

			TopExp_Explorer topoExp(origShape, TopAbs_ShapeEnum(origShape.ShapeType() + 1));
			for (; topoExp.More(); topoExp.Next())
			{
				collectRelatedLabelAndShapes(topoExp.Value(), type, operation);
			}
		}
		else if (type == EnCellsBuilderCommon || type == EnCellsBuilderCut || type == EnCellsBuilderFuse)
		{
			BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
			if (!api)
			{
				return;
			}

			bool supportType = true;
// 			if (type >= EnCut && type <= EnFuse && !isSupportedType(origShape))
// 			{
// 				supportType = false;
// 			}
// 			else if (type == EnFillet2D)
// 			{
// 				m_supportType = TopAbs_EDGE;
// 				if (!isSupportedType(origShape))
// 				{
// 					supportType = false;
// 				}
// 			}
// 			else if (type == EnExtrudeDraft)
// 			{
// 				if (origShape.ShapeType() != TopAbs_EDGE)
// 				{
// 					supportType = false;
// 				}
// 			}

			if (supportType)
			{
				//find modified shape
				const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
				TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
				for (; ShapesIterator.More(); ShapesIterator.Next())
				{
					const TopoDS_Shape& newShape = ShapesIterator.Value();
					if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, newShape, foundLabel))
					{
						LabelInfo newFoundLabel = foundLabel;
						newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnModified ? EnModified : EnNotChanged;
						m_shapeHistoryMap.Bind(newShape, newFoundLabel);
					}
				}

				//find modified shape
				const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
				ShapesIterator.Init(generatedShapes);
				for (; ShapesIterator.More(); ShapesIterator.Next())
				{
					const TopoDS_Shape& newShape = ShapesIterator.Value();
					if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, newShape, foundLabel))
					{
						LabelInfo newFoundLabel = foundLabel;
						newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnGenerated ? EnGenerated : EnNotChanged;
						m_shapeHistoryMap.Bind(newShape, newFoundLabel);
					}
				}
			}

			TopExp_Explorer topoExp(origShape, TopAbs_ShapeEnum(origShape.ShapeType() + 1));
			for (; topoExp.More(); topoExp.Next())
			{
				collectRelatedLabelAndShapes(topoExp.Value(), type, operation);
			}

		}
		else
		{
// 			TopTools_ListOfShape& (*Generated)(const TopoDS_Shape& S);
// 			TopTools_ListOfShape& (*Modified)(const TopoDS_Shape& S);
// 
// 			if (type == EnCellsBuilderCommon || type == EnCellsBuilderCut || type == EnCellsBuilderFuse)
// 			{
// 			}
// 			else
// 			{
// 			}
			BRepBuilderAPI_MakeShape* api = (BRepBuilderAPI_MakeShape*)operation;
			if (!api)
			{
			    return;
			}

			bool supportType = true;
			if (type >= EnCut && type <= EnFuse && !isSupportedType(origShape))
			{
				supportType = false;
			}
			else if (type == EnFillet2D)
			{
				m_supportType = TopAbs_EDGE;
				if (!isSupportedType(origShape))
				{
					supportType = false;
				}
			}
			else if (type == EnExtrudeDraft)
			{
				if (origShape.ShapeType() != TopAbs_EDGE)
				{
					supportType = false;
				}
			}

			if (supportType)
			{
				//find modified shape
				const TopTools_ListOfShape& modifiedShapes = api->Modified(origShape);
				TopTools_ListIteratorOfListOfShape ShapesIterator(modifiedShapes);
				for (; ShapesIterator.More(); ShapesIterator.Next())
				{
					const TopoDS_Shape& newShape = ShapesIterator.Value();
					if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, newShape, foundLabel))
					{
						LabelInfo newFoundLabel = foundLabel;
						newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnModified ? EnModified : EnNotChanged;
						m_shapeHistoryMap.Bind(newShape, newFoundLabel);
					}
				}

				//find modified shape
				const TopTools_ListOfShape& generatedShapes = api->Generated(origShape);
				ShapesIterator.Init(generatedShapes);
				for (; ShapesIterator.More(); ShapesIterator.Next())
				{
					const TopoDS_Shape& newShape = ShapesIterator.Value();
					if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, newShape, foundLabel))
					{
						LabelInfo newFoundLabel = foundLabel;
						newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnGenerated ? EnGenerated : EnNotChanged;
						m_shapeHistoryMap.Bind(newShape, newFoundLabel);
					}
				}
			}

			TopExp_Explorer topoExp(origShape, TopAbs_ShapeEnum(origShape.ShapeType() + 1));
			for (; topoExp.More(); topoExp.Next())
			{
				collectRelatedLabelAndShapes(topoExp.Value(), type, operation);
			}	
		}
    }
    else
    {
        return;
    }
}

void LabelShapeAdapter::directRelateShapes(const TopoDS_Shape& origShape, const TopoDS_Shape& newShape)
{
	LabelInfo foundLabel;
	//     if (m_shapeHistoryMap.Find(origShape, foundLabel))
	if (LabelUtilities::findShapeInMap(m_shapeHistoryMap, origShape, foundLabel))
	{
		if (!LabelUtilities::findShapeInMap(m_shapeHistoryMap, newShape, foundLabel))
		{
			LabelInfo newFoundLabel = foundLabel;
			newFoundLabel.shapeHistory = newFoundLabel.shapeHistory <= EnModified ? EnModified : EnNotChanged;
			m_shapeHistoryMap.Bind(newShape, newFoundLabel);
		}
	}
	else
	{
		return;
	}
}

void LabelShapeAdapter::linkRelatedLabels(const TDF_Label& label)
{
    TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
    if (origShape.IsNull())
    {
        return;
    }

    LabelInfo foundLabel;
    if (LabelUtilities::findShapeInMap(m_shapeHistoryMap, origShape, foundLabel))
//     if (m_shapeHistoryMap.Find(origShape, foundLabel))
    {
        if (foundLabel.shapeHistory < EnGenerated)
        {
            LabelUtilities::copyLabelProperty(foundLabel.label, label);
            LabelUtilities::copyLabelName(foundLabel.label, label);
        }
        Handle(TDF_Reference) meRef = TDF_Reference::Set(label, foundLabel.label);
    }

    TDF_ChildIterator childIter(label);
    for (; childIter.More(); childIter.Next())
    {
        TDF_Label child = childIter.Value();
        linkRelatedLabels(child);
    }
}

void LabelShapeAdapter::labelLinkMakeup(const TDF_Label& label)
{
    TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
    if (origShape.IsNull())
    {
        return;
    }

    LabelInfo foundLabel;
    //现在编辑、生成和不变找那个查找
    if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape, foundLabel)
        &&!LabelUtilities::findShapeInMap(m_generatedShapes, origShape, foundLabel)
        &&!LabelUtilities::findShapeInMap(m_constantShapes, origShape, foundLabel)
        && origShape.ShapeType() == TopAbs_SOLID)
    {
        bool found = false;
        NCollection_DataMap<TopoDS_Shape, LabelInfo>::Iterator iter(m_constantShapes);
        for (; iter.More(); iter.Next())
        {
            if (iter.Key().ShapeType() == TopAbs_SOLID)
            {
                foundLabel = iter.Value();
                found = true;
                break;
            }
        }
        if (!found)
        {
            return;
        }

        LabelUtilities::copyLabelProperty(foundLabel.label, label);
        LabelUtilities::copyLabelName(foundLabel.label, label);

        Handle(TDF_Reference) meRef = TDF_Reference::Set(label, foundLabel.label);

        m_constantShapes.UnBind(iter.Key());

        return;
    }

    TDF_ChildIterator childIter(label);
    for (; childIter.More(); childIter.Next())
    {
        TDF_Label child = childIter.Value();
        labelLinkMakeup(child);
    }
}

void LabelShapeAdapter::collectLabelShapes(const TDF_Label& label, const EnAdapatType& type, void* operation)
{
    if (label.IsNull())
    {
        return;
    }

    switch (type)
    {
    case EnExtrude:
    case EnRevol:
    case EnSweep:
    case EnLoft:
    {
        BRepPrimAPI_MakePrism* api = (BRepPrimAPI_MakePrism*)operation;
        if (!api)
        {
            return;
        }

        TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
        if (origShape.IsNull())
        {
            return;
        }
        else
        {
            LabelInfo foundLabel;
            if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape, foundLabel))
            {
                foundLabel.label = label;
                foundLabel.calledTimes = 0;
                m_modifiedShapes.Bind(origShape, foundLabel);

                // 				Utilities::getLabelName(label, m_name);
            }

            const TopTools_ListOfShape& Shapes = api->Modified(origShape);
            TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
            //             int splitIndex = 0;
            for (; ShapesIterator.More(); ShapesIterator.Next())
            {
                const TopoDS_Shape& newShape = ShapesIterator.Value();
                // TNaming_Evolution == MODIFY
                if (!origShape.IsSame(newShape))
                {
                    if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape, foundLabel))
                    {
                        foundLabel.label = label;
                        foundLabel.calledTimes = 0;
                        m_modifiedShapes.Bind(newShape, foundLabel);
                    }
                }
            }
        }

        TDF_ChildIterator childIter(label);
        for (; childIter.More(); childIter.Next())
        {
            TDF_Label child = childIter.Value();
            collectLabelShapes(child, type, operation);
        }

        break;
    }
    case EnCut:
    case EnCommon:
    case EnFuse:
    {
        BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
        if (!api)
        {
            return;
        }

        TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
        if (origShape.IsNull())
        {
            return;
        }
        else
        {
            LabelInfo foundLabel;
			if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape, foundLabel))
            {
                foundLabel.label = label;
                foundLabel.calledTimes = 0;
                m_modifiedShapes.Bind(origShape, foundLabel);

// 				Utilities::getLabelName(label, m_name);
            }

            const TopTools_ListOfShape& Shapes = api->Modified(origShape);
            TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
//             int splitIndex = 0;
            for (; ShapesIterator.More(); ShapesIterator.Next())
            {
                const TopoDS_Shape& newShape = ShapesIterator.Value();
                // TNaming_Evolution == MODIFY
                if (newShape.ShapeType() == origShape.ShapeType())
                {
					if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape, foundLabel))
                    {
                        foundLabel.label = label;
                        foundLabel.calledTimes = 0;
                        m_modifiedShapes.Bind(newShape, foundLabel);
                    }
                }
            }
        }

        TDF_ChildIterator childIter(label);
        for (; childIter.More(); childIter.Next())
        {
            TDF_Label child = childIter.Value();
            collectLabelShapes(child, type, operation);
        }

        break;
    }
    case EnFillet:
    {
		BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
		if (!api)
		{
			return;
		}

		TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
		if (origShape.IsNull())
		{
			return;
		}
		else
		{
			LabelInfo foundLabel;
			if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape, foundLabel))
			{
				foundLabel.label = label;
				foundLabel.calledTimes = 0;
				m_modifiedShapes.Bind(origShape, foundLabel);

				// 				Utilities::getLabelName(label, m_name);
			}

			const TopTools_ListOfShape& Shapes = api->Modified(origShape);
			TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
			for (; ShapesIterator.More(); ShapesIterator.Next())
			{
				const TopoDS_Shape& newShape = ShapesIterator.Value();
				if (newShape.ShapeType() == origShape.ShapeType())
				{
					if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape, foundLabel))
					{
						foundLabel.label = label;
						foundLabel.calledTimes = 0;
						m_modifiedShapes.Bind(newShape, foundLabel);
					}
				}
			}
		}

		TDF_ChildIterator childIter(label);
		for (; childIter.More(); childIter.Next())
		{
			TDF_Label child = childIter.Value();
			collectLabelShapes(child, type, operation);
		}
        break;
    }
    case EnCellsBuilderCut:
    {
		BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
		if (!api)
		{
			return;
		}

		TopoDS_Shape origShape = LabelUtilities::getLabelShape(label);
		if (origShape.IsNull())
		{
			return;
		}
		else
		{
			LabelInfo foundLabel;
			if (!LabelUtilities::findShapeInMap(m_modifiedShapes, origShape, foundLabel))
			{
				foundLabel.label = label;
				foundLabel.calledTimes = 0;
				m_modifiedShapes.Bind(origShape, foundLabel);

				// 				Utilities::getLabelName(label, m_name);
			}

			const TopTools_ListOfShape& Shapes = api->Modified(origShape);
			TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
			//             int splitIndex = 0;
			for (; ShapesIterator.More(); ShapesIterator.Next())
			{
				const TopoDS_Shape& newShape = ShapesIterator.Value();
				// TNaming_Evolution == MODIFY
				if (newShape.ShapeType() == origShape.ShapeType())
				{
					if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape, foundLabel))
					{
						foundLabel.label = label;
						foundLabel.calledTimes = 0;
						m_modifiedShapes.Bind(newShape, foundLabel);
					}
				}
			}
		}

		TDF_ChildIterator childIter(label);
		for (; childIter.More(); childIter.Next())
		{
			TDF_Label child = childIter.Value();
			collectLabelShapes(child, type, operation);
		}
        break;
    }
    default:
        break;
    }
}

void LabelShapeAdapter::collectLabelShapesGeneratedByEdgeAndVertex(TopoDS_Shape sourceShape, const EnAdapatType& type, void* operation)
{
	switch (type)
	{
    case EnExtrude:
    {
        BRepPrimAPI_MakePrism* api = (BRepPrimAPI_MakePrism*)operation;
        if (!api)
        {
            return;
        }

        LabelInfo foundLabel;
        foundLabel.label = TDF_Label();
        foundLabel.calledTimes = 0;
        // get face generated by translate
        {
            const TopoDS_Shape& newShape = api->LastShape();
            if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
            {
                ++foundLabel.calledTimes;
                m_modifiedShapes.Bind(newShape, foundLabel);
            }
        }

        // get face generated by edge
        TopExp_Explorer shapeExp(sourceShape, TopAbs_EDGE);
        for (; shapeExp.More(); shapeExp.Next())
        {
            const TopTools_ListOfShape& generated = api->Generated(shapeExp.Value());
            if (!generated.IsEmpty())
            {
                TopTools_ListIteratorOfListOfShape ShapesIterator(generated);
                for (; ShapesIterator.More(); ShapesIterator.Next())
                {
                    const TopoDS_Shape& newShape = ShapesIterator.Value();
                    if (newShape.ShapeType() == TopAbs_FACE)
                    {
                        if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
                        {
                            ++foundLabel.calledTimes;
                            m_modifiedShapes.Bind(newShape, foundLabel);
                        }
                    }
                }
            }
        }

        //get face generated by vertex
        shapeExp.Init(sourceShape, TopAbs_VERTEX);
        for (; shapeExp.More(); shapeExp.Next())
        {
            const TopTools_ListOfShape& generated = api->Generated(shapeExp.Value());
            if (!generated.IsEmpty())
            {
                TopTools_ListIteratorOfListOfShape ShapesIterator(generated);
                for (; ShapesIterator.More(); ShapesIterator.Next())
                {
                    const TopoDS_Shape& newShape = ShapesIterator.Value();
                    if (newShape.ShapeType() == TopAbs_FACE)
                    {
                        if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
                        {
                            ++foundLabel.calledTimes;
                            m_modifiedShapes.Bind(newShape, foundLabel);
                        }
                    }
                }
            }
        }

        break;
    }
	case EnFillet:
	{
		BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
		if (!api)
		{
			return;
		}

		// get face generated by edge
		LabelInfo foundLabel;
		foundLabel.label = TDF_Label();
		foundLabel.calledTimes = 0;
		TopExp_Explorer edgeExp(sourceShape, TopAbs_EDGE);
		for (; edgeExp.More();edgeExp.Next())
		{
			const TopTools_ListOfShape& generated = api->Generated(edgeExp.Value());
			if (!generated.IsEmpty())
			{
				TopTools_ListIteratorOfListOfShape ShapesIterator(generated);
				for (; ShapesIterator.More(); ShapesIterator.Next())
				{
					const TopoDS_Shape& newShape = ShapesIterator.Value();
					if (newShape.ShapeType() == TopAbs_FACE)
					{
						if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
						{
							++foundLabel.calledTimes;
							m_modifiedShapes.Bind(newShape, foundLabel);
						}
					}
				}
			}
		}

		//get face generated by vertex
		edgeExp.Init(sourceShape, TopAbs_VERTEX);
		for (; edgeExp.More(); edgeExp.Next())
		{
			const TopTools_ListOfShape& generated = api->Generated(edgeExp.Value());
			if (!generated.IsEmpty())
			{
				TopTools_ListIteratorOfListOfShape ShapesIterator(generated);
				for (; ShapesIterator.More(); ShapesIterator.Next())
				{
					const TopoDS_Shape& newShape = ShapesIterator.Value();
					if (newShape.ShapeType() == TopAbs_FACE)
					{
						if (!LabelUtilities::findShapeInMap(m_modifiedShapes, newShape))
						{
							++foundLabel.calledTimes;
							m_modifiedShapes.Bind(newShape, foundLabel);
						}
					}
				}
			}
		}

		break;
	}
	default:
		break;
	}
}

//old method
// void LabelShapeAdapter::transferColorAndName4ImportingStep(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool)
// {
//     TopoDS_Shape shape = LabelUtilities::getLabelShape(targetLabel);
//     if (shape.IsNull())
//     {
//         return;
//     }
// 
//     TDF_Label foundLabel;
//     if (shapeTool->FindShape(shape, foundLabel))//can't find coresponding shape of face
// //     if (shapeTool->Search(shape, foundLabel, true, true, true)||shapeTool->SearchUsingMap(shape, foundLabel, true, true))
//     {
//         Handle(TDataStd_TreeNode) treenode;
//         Handle(TDataStd_TreeNode) fatherTreenode;
//         Handle(TDataStd_TreeNode) childTreenode;
// 
//         Handle(TDataStd_Name) nameData;
//         Handle(TDataStd_Integer) calledMark;
// 
//         if (!foundLabel.FindAttribute(XCAFDoc::ShapeRefGUID(), treenode))//no child treenode
//         {
//             if (foundLabel.FindAttribute(TDataStd_Name::GetID(), nameData))
//             {
//                 std::string name;
//                 Standard_Integer aLen = nameData->Get().LengthOfCString();
//                 Standard_PCharacter aCstr = (Standard_PCharacter)Standard::Allocate(aLen + 1);
//                 nameData->Get().ToUTF8CString(aCstr);
//                 name.assign(aCstr);
//                 TDataStd_Name::Set(targetLabel, name.c_str());
//             }
//         }
//         else
//         {
//             std::string name;
// 
//             fatherTreenode = treenode->Father();
//             childTreenode = treenode->First();
//             if (fatherTreenode)
//             {
//                 TDF_Label linkLabel = fatherTreenode->Label();
//                 if (!linkLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
//                 {
//                     if (LabelUtilities::getLabelName(linkLabel, name))
//                     {
//                         TDataStd_Name::Set(targetLabel, name.c_str());
//                         TDataStd_Integer::Set(linkLabel, 0);
//                     }
//                 }
//             }
//             else if (childTreenode)
//             {
//                 TDataStd_ChildNodeIterator treeIter(treenode);
//                 for (; treeIter.More(); treeIter.Next())
//                 {
//                     TDF_Label linkChildLabel = treeIter.Value()->Label();
//                     if (linkChildLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
//                     {
//                         continue;
//                     }
// 
//                     if (LabelUtilities::getLabelName(linkChildLabel, name))
//                     {
//                         TDataStd_Name::Set(targetLabel, name.c_str());
//                         TDataStd_Integer::Set(linkChildLabel, 0);
//                         break;
//                     }
//                 }
//             }
//         }
// 
//         if (foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), treenode)
//             || foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorSurf), treenode)
//             || foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorCurv), treenode))
//         {
//             fatherTreenode = treenode->Father();
//             childTreenode = treenode->First();
//             if (fatherTreenode)
//             {
//                 TDF_Label linkFatherLabel = fatherTreenode->Label();
//                 std::string sourceColor;
//                 if (LabelUtilities::getLabelName(linkFatherLabel, sourceColor))
//                 {
//                     int start = sourceColor.find_first_of("(");
//                     int end = sourceColor.find_first_of(")");
//                     sourceColor = sourceColor.substr(start+1, end-start-1);
//                     LabelUtilities::addLabelProperty(targetLabel, "COLOR", sourceColor);
//                     TDataStd_Integer::Set(linkFatherLabel, 0);
//                 }              
//             }
//             else if (childTreenode)
//             {
//                 TDF_Label linkFatherLabel = childTreenode->Label();
//                 std::string sourceColor;
//                 if (LabelUtilities::getLabelName(linkFatherLabel, sourceColor))
//                 {
//                     int start = sourceColor.find_first_of("(");
//                     int end = sourceColor.find_first_of(")");
//                     sourceColor = sourceColor.substr(start + 1, end - start - 1);
//                     LabelUtilities::addLabelProperty(targetLabel, "COLOR", sourceColor);
//                     TDataStd_Integer::Set(linkFatherLabel, 0);
//                 }
//             }
//         }
//     }
// 
//     TDF_ChildIterator iter(targetLabel);
//     for (; iter.More(); iter.Next())
//     {
//         transferColorAndName4ImportingStep(iter.Value(), shapeTool);
//     }
// }
//new method
void LabelShapeAdapter::transferColorAndName4ImportingStep(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool)
{
    TopoDS_Shape shape = LabelUtilities::getLabelShape(targetLabel);
    if (shape.IsNull())
    {
        return;
    }

    TDF_Label foundLabel;
    if (shapeTool->FindShape(shape, foundLabel))//can't find coresponding shape of face
//     if (shapeTool->Search(shape, foundLabel, true, true, true)||shapeTool->SearchUsingMap(shape, foundLabel, true, true))
    {
        Handle(TDataStd_TreeNode) treenode;
        Handle(TDataStd_TreeNode) fatherTreenode;
        Handle(TDataStd_TreeNode) childTreenode;

        Handle(TDataStd_Name) nameData;
        Handle(TDataStd_Integer) calledMark;

        if (foundLabel.FindAttribute(TDataStd_Name::GetID(), nameData))
        {
            TCollection_ExtendedString str = nameData->Get();

            if (!foundLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
            {
                TDataStd_Name::Set(targetLabel, str);
                TDataStd_Integer::Set(foundLabel, 0);
            }
            else
            {
                int calledTimes = calledMark->Get() + 1;
                TDataStd_Integer::Set(foundLabel, calledTimes);

                str.Insert(str.Length()+1, "_");
                str.Insert(str.Length()+1, TCollection_ExtendedString(calledTimes));
                TDataStd_Name::Set(targetLabel, str);
            }



//             nameData->Get();    
//             std::string name;
//             Standard_Integer aLen = nameData->Get().LengthOfCString();
//             Standard_PCharacter aCstr = (Standard_PCharacter)Standard::Allocate(aLen + 1);
//             nameData->Get().ToUTF8CString(aCstr);
//             name.assign(aCstr);
//             TDataStd_Name::Set(targetLabel, name.c_str());
        }
//         else
//         {
// 
//         }
// 
// 
//         if (!foundLabel.FindAttribute(XCAFDoc::ShapeRefGUID(), treenode))//no child treenode
//         {
//             if (foundLabel.FindAttribute(TDataStd_Name::GetID(), nameData))
//             {
//                 std::string name;
//                 Standard_Integer aLen = nameData->Get().LengthOfCString();
//                 Standard_PCharacter aCstr = (Standard_PCharacter)Standard::Allocate(aLen + 1);
//                 nameData->Get().ToUTF8CString(aCstr);
//                 name.assign(aCstr);
//                 TDataStd_Name::Set(targetLabel, name.c_str());
//             }
//         }
//         else
//         {
//             std::string name;
// 
//             fatherTreenode = treenode->Father();
//             childTreenode = treenode->First();
//             if (fatherTreenode)
//             {
//                 TDF_Label linkLabel = fatherTreenode->Label();
//                 if (!linkLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
//                 {
//                     if (LabelUtilities::getLabelName(linkLabel, name))
//                     {
//                         TDataStd_Name::Set(targetLabel, name.c_str());
//                         TDataStd_Integer::Set(linkLabel, 0);
//                     }
//                 }
//             }
//             else if (childTreenode)
//             {
//                 TDataStd_ChildNodeIterator treeIter(treenode);
//                 for (; treeIter.More(); treeIter.Next())
//                 {
//                     TDF_Label linkChildLabel = treeIter.Value()->Label();
//                     if (linkChildLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
//                     {
//                         continue;
//                     }
// 
//                     if (LabelUtilities::getLabelName(linkChildLabel, name))
//                     {
//                         TDataStd_Name::Set(targetLabel, name.c_str());
//                         TDataStd_Integer::Set(linkChildLabel, 0);
//                         break;
//                     }
//                 }
//             }
//         }

        if (foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), treenode)
            || foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorSurf), treenode)
            || foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorCurv), treenode))
        {
            fatherTreenode = treenode->Father();
            childTreenode = treenode->First();
            if (fatherTreenode)
            {
                TDF_Label linkFatherLabel = fatherTreenode->Label();
                std::string sourceColor;
                if (LabelUtilities::getLabelName(linkFatherLabel, sourceColor))
                {
                    int start = sourceColor.find_first_of("(");
                    int end = sourceColor.find_first_of(")");
                    sourceColor = sourceColor.substr(start + 1, end - start - 1);
                    LabelUtilities::addLabelProperty(targetLabel, "COLOR", sourceColor);
                    TDataStd_Integer::Set(linkFatherLabel, 0);
                }
            }
            else if (childTreenode)
            {
                TDF_Label linkFatherLabel = childTreenode->Label();
                std::string sourceColor;
                if (LabelUtilities::getLabelName(linkFatherLabel, sourceColor))
                {
                    int start = sourceColor.find_first_of("(");
                    int end = sourceColor.find_first_of(")");
                    sourceColor = sourceColor.substr(start + 1, end - start - 1);
                    LabelUtilities::addLabelProperty(targetLabel, "COLOR", sourceColor);
                    TDataStd_Integer::Set(linkFatherLabel, 0);
                }
            }
        }
    }

    TDF_ChildIterator iter(targetLabel);
    for (; iter.More(); iter.Next())
    {
        transferColorAndName4ImportingStep(iter.Value(), shapeTool);
    }
}


void LabelShapeAdapter::transferColorAndName4ImportingIges(TDF_Label targetLabel, Handle(XCAFDoc_ShapeTool) shapeTool)
{
    TopoDS_Shape shape = LabelUtilities::getLabelShape(targetLabel);
    if (shape.IsNull())
    {
        return;
    }

    TDF_Label foundLabel;
    if (shapeTool->FindShape(shape, foundLabel))//can't find coresponding shape of face
        //     if (shapeTool->Search(shape, foundLabel, true, true, true)||shapeTool->SearchUsingMap(shape, foundLabel, true, true))
    {
        Handle(TDataStd_TreeNode) treenode;
        Handle(TDataStd_TreeNode) fatherTreenode;
        Handle(TDataStd_TreeNode) childTreenode;

        Handle(TDataStd_Name) nameData;
        Handle(TDataStd_Integer) calledMark;

        if (foundLabel.FindAttribute(TDataStd_Name::GetID(), nameData))
        {
            std::string name;
            Standard_Integer aLen = nameData->Get().LengthOfCString();
            Standard_PCharacter aCstr = (Standard_PCharacter)Standard::Allocate(aLen + 1);
            nameData->Get().ToUTF8CString(aCstr);
            name.assign(aCstr);
            TDataStd_Name::Set(targetLabel, name.c_str());
        }
        else if (foundLabel.FindAttribute(XCAFDoc::ShapeRefGUID(), treenode))//no child treenode
        {
            std::string name;

            fatherTreenode = treenode->Father();
            childTreenode = treenode->First();
            if (fatherTreenode)
            {
                TDF_Label linkLabel = fatherTreenode->Label();
                if (!linkLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
                {
                    if (LabelUtilities::getLabelName(linkLabel, name))
                    {
                        TDataStd_Name::Set(targetLabel, name.c_str());
                        TDataStd_Integer::Set(linkLabel, 0);
                    }
                }
            }
            else if (childTreenode)
            {
                TDataStd_ChildNodeIterator treeIter(treenode);
                for (; treeIter.More(); treeIter.Next())
                {
                    TDF_Label linkChildLabel = treeIter.Value()->Label();
                    if (linkChildLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
                    {
                        continue;
                    }

                    if (LabelUtilities::getLabelName(linkChildLabel, name))
                    {
                        TDataStd_Name::Set(targetLabel, name.c_str());
                        TDataStd_Integer::Set(linkChildLabel, 0);
                    }
                }
            }
        }

        if (foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), treenode)
            || foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorSurf), treenode)
            || foundLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorCurv), treenode))
        {
            fatherTreenode = treenode->Father();
            childTreenode = treenode->First();
            if (fatherTreenode)
            {
                TDF_Label linkFatherLabel = fatherTreenode->Label();
                std::string sourceColor;
                if (LabelUtilities::getLabelName(linkFatherLabel, sourceColor))
                {
                    int start = sourceColor.find_first_of("(");
                    int end = sourceColor.find_first_of(")");
                    sourceColor = sourceColor.substr(start + 1, end - start - 1);
                    LabelUtilities::addLabelProperty(targetLabel, "COLOR", sourceColor);
                    TDataStd_Integer::Set(linkFatherLabel, 0);
                }
            }
            else if (childTreenode)
            {
                TDF_Label linkFatherLabel = childTreenode->Label();
                std::string sourceColor;
                if (LabelUtilities::getLabelName(linkFatherLabel, sourceColor))
                {
                    int start = sourceColor.find_first_of("(");
                    int end = sourceColor.find_first_of(")");
                    sourceColor = sourceColor.substr(start + 1, end - start - 1);
                    LabelUtilities::addLabelProperty(targetLabel, "COLOR", sourceColor);
                    TDataStd_Integer::Set(linkFatherLabel, 0);
                }
            }
        }
    }

    TDF_ChildIterator iter(targetLabel);
    for (; iter.More(); iter.Next())
    {
        transferColorAndName4ImportingIges(iter.Value(), shapeTool);
    }
}

bool LabelShapeAdapter::isSupportedType(TopoDS_Shape shape)
{
    switch (m_supportType)
    {
    case TopAbs_SHAPE:
    {
        return BRepTools_History::IsSupportedType(shape);
    }
    default:
        return shape.ShapeType() == TopAbs_SOLID;
    }
}

// void LabelShapeAdapter::labelModify(const TDF_Label& compoundLabel, const EnAdapatType& type, void* operation)
// {
//     if (compoundLabel.IsNull())
//     {
//         return;
//     }
// 
//     switch (type)
//     {
//     case EnCut:
//     case EnCommon:
//     case EnFuse:
//     {
//         BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TDF_ChildIterator childIter(compoundLabel);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelModifyChild(child, type, operation);
//         }
// 
//         break;
//     }
//     case EnFillet:
//     {
//         BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TDF_ChildIterator childIter(compoundLabel);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelModifyChild(child, type, operation);
//         }
// 
//         break;
//     }
//     case EnCellsBuilder:
//     {
//         BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         m_compoundLabel = compoundLabel;
// 
//         TDF_ChildIterator childIter(compoundLabel);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelModifyChild(child, type, operation);
//         }
// 
//         break;    
//     }
//     default:
//         break;
//     }
// }
// 
// void LabelShapeAdapter::labelDelete(const TDF_Label& compoundLabel, const EnAdapatType& type, void* operation)
// {
//     if (compoundLabel.IsNull())
//     {
//         return;
//     }
// 
//     switch (type)
//     {
//     case EnCut:
//     case EnCommon:
//     case EnFuse:
//     {
//         BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TDF_ChildIterator childIter(compoundLabel);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelDeleteChild(child, type, operation);
//         }
//         break;
//     }
//     case EnFillet:
//     {
//         BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TDF_ChildIterator childIter(compoundLabel);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelDeleteChild(child, type, operation);
//         }
// 
//         break;
//     }
//     case EnCellsBuilder:
//     {
//         BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TDF_ChildIterator childIter(compoundLabel);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelDeleteChild(child, type, operation);
//         }
// 
//         break;
//     }
//     default:
//         break;
//     }
// }
// 
// void LabelShapeAdapter::labelGenerate(const TDF_Label& compoundLabel, const TDF_Label& toolLabel, const EnAdapatType& type, void* operation)
// {
//     m_addedNewShape.Clear();
//     if (compoundLabel.IsNull())
//     {
//         return;
//     }
// 
//     switch (type)
//     {
//     case EnCut:
//     case EnCommon:
//     case EnFuse:
//     {
//         BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
// //         //method 1
// //         TopExp_Explorer ShapeExplorer;
// //         int faceID = 1;
// //         std::string  faceName;
// // 
// //         for (TopTools_ListOfShape::iterator iter = aLS.begin(); iter != aLS.end(); ++iter)
// //         {
// //             TopoDS_Shape ToolSh = *iter;
// //             ShapeExplorer.Init(ToolSh, TopAbs_FACE);
// //             for (; ShapeExplorer.More(); ShapeExplorer.Next()) {
// //                 const TopoDS_Shape& F = ShapeExplorer.Current();
// //                 const TopTools_ListOfShape& generated = api->Modified(F);
// //                 if (!generated.IsEmpty())
// //                 {
// //                     TopTools_ListIteratorOfListOfShape itr(generated);
// //                     for (; itr.More(); itr.Next())
// //                     {
// //                         const TopoDS_Shape& newShape = itr.Value();
// //                         bool alreadyAdded = false;
// //                         for (TopTools_ListOfShape::iterator addedIter = addedNewShape.begin(); addedIter != addedNewShape.end(); ++addedIter)
// //                         {
// //                             if (newShape.IsSame(*addedIter))
// //                             {
// //                                 alreadyAdded = true;
// //                                 break;;
// //                             }
// //                         }
// //                         if (alreadyAdded)
// //                         {
// //                             continue;
// //                         }
// //                         addedNewShape.Append(newShape);
// // 
// //                         faceName = std::string(m_name) + "_face_" + std::to_string(faceID++);
// //                         TDF_Label faceLabel = createNewChild(compoundLabel, faceName);
// //                         TNaming_Builder newBuilder(faceLabel);
// //                         newBuilder.Generated(F, newShape);
// //                     }
// //                 }
// //             }
// //         }
// 
//         //method 2 to be approved
// //         // generated from edge
// //         for (TopTools_ListOfShape::iterator iter = aLS.begin(); iter != aLS.end(); ++iter)
// //         {
// //             TopoDS_Shape ToolSh = *iter;
// //             ShapeExplorer.Init(ToolSh, TopAbs_EDGE);
// //             for (; ShapeExplorer.More(); ShapeExplorer.Next()) {
// //                 const TopoDS_Shape& E = ShapeExplorer.Current();
// //                 const TopTools_ListOfShape& generated = api->Generated(E);
// //                 if (!generated.IsEmpty())
// //                 {
// //                     TopTools_ListIteratorOfListOfShape itr(generated);
// //                     for (; itr.More(); itr.Next())
// //                     {
// //                         const TopoDS_Shape& newShape = itr.Value();
// //                         faceName = std::string(name) + "_face_" + std::to_string(faceID++);
// //                         TDF_Label faceLabel = TDF_TagSource::NewChild(compoundLabel);
// //                         TDataStd_Name::Set(faceLabel, faceName.c_str());
// //                         TNaming_Builder newBuilder(faceLabel);
// //                         newBuilder.Generated(E, newShape);
// //                     }
// //                 }
// //             }
// //         }
// // 
// //         // generated from vetex
// //         for (TopTools_ListOfShape::iterator iter = aLS.begin(); iter != aLS.end(); ++iter)
// //         {
// //             TopoDS_Shape ToolSh = *iter;
// //             ShapeExplorer.Init(ToolSh, TopAbs_VERTEX);
// //             for (; ShapeExplorer.More(); ShapeExplorer.Next()) {
// //                 const TopoDS_Shape& E = ShapeExplorer.Current();
// //                 const TopTools_ListOfShape& generated = api->Generated(E);
// //                 if (!generated.IsEmpty())
// //                 {
// //                     TopTools_ListIteratorOfListOfShape itr(generated);
// //                     for (; itr.More(); itr.Next())
// //                     {
// //                         const TopoDS_Shape& newShape = itr.Value();
// //                         faceName = std::string(name) + "_face_" + std::to_string(faceID++);
// //                         TDF_Label faceLabel = TDF_TagSource::NewChild(compoundLabel);
// //                         TDataStd_Name::Set(faceLabel, faceName.c_str());
// //                         TNaming_Builder newBuilder(faceLabel);
// //                         newBuilder.Generated(E, newShape);
// //                     }
// //                 }
// //             }
// //         }
// 
//         // method3
//         m_compoundLabel = compoundLabel;
// 
//         TDF_ChildIterator childIter(toolLabel);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelGenerateChild(child, type, operation);
//         }
// 
//         break;
//     }
//     case EnFillet:
//     {
// //         BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
// //         if (!api)
// //         {
// //             return;
// //         }
// // 
// //         TopExp_Explorer ShapeExplorer;
// //         int faceID = 1;
// //         std::string  faceName;
// // 
// //         // generated from edge
// //         for (TopTools_ListOfShape::iterator iter = aLS.begin(); iter != aLS.end(); ++iter)
// //         {
// //             TopoDS_Shape ToolSh = *iter;
// //             ShapeExplorer.Init(ToolSh, TopAbs_EDGE);
// //             for (; ShapeExplorer.More(); ShapeExplorer.Next()) {
// //                 const TopoDS_Shape& E = ShapeExplorer.Current();
// //                 const TopTools_ListOfShape& generated = api->Generated(E);
// //                 if (!generated.IsEmpty())
// //                 {
// //                     TopTools_ListIteratorOfListOfShape itr(generated);
// //                     for (; itr.More(); itr.Next())
// //                     {
// //                         const TopoDS_Shape& newShape = itr.Value();
// //                         bool alreadyAdded = false;
// //                         for (TopTools_ListOfShape::iterator addedIter = addedNewShape.begin(); addedIter != addedNewShape.end(); ++addedIter)
// //                         {
// //                             if (newShape.IsSame(*addedIter))
// //                             {
// //                                 alreadyAdded = true;
// //                                 break;;
// //                             }
// //                         }
// //                         if (alreadyAdded)
// //                         {
// //                             continue;
// //                         }
// //                         addedNewShape.Append(newShape);
// // 
// //                         faceName = std::string(m_name) + "_face_" + std::to_string(faceID++);
// //                         TDF_Label faceLabel = createNewChild(compoundLabel, faceName);
// //                         TNaming_Builder newBuilder(faceLabel);
// //                         newBuilder.Generated(E, newShape);
// //                     }
// //                 }
// //             }
// //         }
// // 
// //         // generated from vetex
// //         for (TopTools_ListOfShape::iterator iter = aLS.begin(); iter != aLS.end(); ++iter)
// //         {
// //             TopoDS_Shape ToolSh = *iter;
// //             ShapeExplorer.Init(ToolSh, TopAbs_VERTEX);
// //             for (; ShapeExplorer.More(); ShapeExplorer.Next()) {
// //                 const TopoDS_Shape& E = ShapeExplorer.Current();
// //                 const TopTools_ListOfShape& generated = api->Generated(E);
// //                 if (!generated.IsEmpty())
// //                 {
// //                     TopTools_ListIteratorOfListOfShape itr(generated);
// //                     for (; itr.More(); itr.Next())
// //                     {
// //                         const TopoDS_Shape& newShape = itr.Value();
// //                         bool alreadyAdded = false;
// //                         for (TopTools_ListOfShape::iterator addedIter = addedNewShape.begin(); addedIter != addedNewShape.end(); ++addedIter)
// //                         {
// //                             if (newShape.IsSame(*addedIter))
// //                             {
// //                                 alreadyAdded = true;
// //                                 break;;
// //                             }
// //                         }
// //                         if (alreadyAdded)
// //                         {
// //                             continue;
// //                         }
// //                         addedNewShape.Append(newShape);
// // 
// //                         faceName = std::string(m_name) + "_face_" + std::to_string(faceID++);
// //                         TDF_Label faceLabel = createNewChild(compoundLabel, faceName);
// //                         TNaming_Builder newBuilder(faceLabel);
// //                         newBuilder.Generated(E, newShape);
// //                     }
// //                 }
// //             }
// //         }
// 
//         break;
//     }
//     case EnCellsBuilder:
//     {
// //         BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
// //         if (!api)
// //         {
// //             return;
// //         }
// // 
// //         TDF_ChildIterator childIter(compoundLabel);
// //         for (; childIter.More(); childIter.Next())
// //         {
// //             TDF_Label child = childIter.Value();
// //             labelGenerateChild(child, type, operation);
// //         }
// 
//         break;
//     }
//     default:
//         break;
//     }
// }
// 
// void LabelShapeAdapter::labelModifyChild(const TDF_Label& label, const EnAdapatType& type, void* operation)
// {
//     if (label.IsNull())
//     {
//         return;
//     }
// 
//     switch (type)
//     {
//     case EnCut:
//     case EnCommon:
//     case EnFuse:
//     {
// //         BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
// //         if (!api)
// //         {
// //             return;
// //         }
// // 
// //         TopoDS_Shape origShape;
// //         Handle(TNaming_NamedShape) OriginalNShape;
// //         if (!label.FindAttribute(TNaming_NamedShape::GetID(), OriginalNShape))
// //         {
// //             return;
// //         }
// //         else
// //         {
// //             if (!OriginalNShape->IsEmpty())
// //             {
// //                 origShape = OriginalNShape->Get();
// //                 if (!origShape.IsNull())
// //                 {
// //                     const TopTools_ListOfShape& Shapes = api->Modified(origShape);
// //                     TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
// //                     int splitIndex = 0;
// //                     for (; ShapesIterator.More(); ShapesIterator.Next())
// //                     {
// //                         const TopoDS_Shape& newShape = ShapesIterator.Value();
// //                         // TNaming_Evolution == MODIFY
// //                         if (!origShape.IsSame(newShape))
// //                         {
// //                             if (newShape.ShapeType() == origShape.ShapeType())
// //                             {
// //                                 if (splitIndex != 0)//The origin shape has been split into several shapes, so that the shapes excepte the first one should be added as new label
// //                                 {
// //                                     std::string name;
// //                                     Utilities::getLabelName(label, name);
// //                                     TDF_Label childLabel = Utilities::createNewChild(label.Father());
// //                                     allocateLabelWithTopShape(childLabel, newShape, name, splitIndex++, true, true);// add new shape
// //                                 }
// //                                 else
// //                                 {
// //                                     TNaming_Builder ModBuilder(label);
// //                                     ModBuilder.Modify(origShape, newShape);
// //                                     splitIndex++;
// //                                 }
// //                             }
// //                         }
// //                     }
// //                 }
// //             }
// //         }
// // 
// //         TDF_ChildIterator childIter(label);
// //         for (; childIter.More(); childIter.Next())
// //         {
// //             TDF_Label child = childIter.Value();
// //             labelModifyChild(child, type, operation);
// //         }
//         break;
//     }
//     case EnFillet:
//     {
//         BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TopoDS_Shape origShape;
//         Handle(TNaming_NamedShape) OriginalNShape;
//         if (!label.FindAttribute(TNaming_NamedShape::GetID(), OriginalNShape))
//         {
//             return;
//         }
//         else
//         {
//             if (!OriginalNShape->IsEmpty())
//             {
//                 origShape = OriginalNShape->Get();
//                 if (!origShape.IsNull())
//                 {
//                     const TopTools_ListOfShape& Shapes = api->Modified(origShape);
//                     TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
//                     for (; ShapesIterator.More(); ShapesIterator.Next())
//                     {
//                         const TopoDS_Shape& newShape = ShapesIterator.Value();
//                         // TNaming_Evolution == MODIFY
//                         if (!origShape.IsSame(newShape) && newShape.ShapeType() == origShape.ShapeType())
//                         {
//                             TNaming_Builder ModBuilder(label);
//                             ModBuilder.Modify(origShape, newShape);
//                         }
//                     }
//                 }
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelModifyChild(child, type, operation);
//         }
// 
//         break;
//     }
//     case EnCellsBuilder:
//     {
// //         BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
// //         if (!api)
// //         {
// //             return;
// //         }
// // 
// //         TopoDS_Shape origShape;
// //         Handle(TNaming_NamedShape) OriginalNShape;
// //         if (!label.FindAttribute(TNaming_NamedShape::GetID(), OriginalNShape))
// //         {
// //             return;
// //         }
// //         else
// //         {
// //             if (!OriginalNShape->IsEmpty())
// //             {
// //                 origShape = OriginalNShape->Get();
// //                 if (!origShape.IsNull())
// //                 {
// //                     const TopTools_ListOfShape& Shapes = api->Modified(origShape);
// //                     if (Shapes.Size() >= 1)
// //                     {
// //                         TopoDS_Shape newShape = Shapes.First();
// // 
// //                         if (!origShape.IsSame(newShape) && newShape.ShapeType() == origShape.ShapeType())
// //                         {
// //                             TNaming_Builder ModBuilder(label);
// //                             ModBuilder.Modify(origShape, newShape);
// //                         }
// // 
// // 
// //                         if (Shapes.Size() > 1)
// //                         {
// //                             TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
// //                             ShapesIterator.Next();//step over the first shape
// //                             int id = 1;
// //                             for (; ShapesIterator.More(); ShapesIterator.Next())
// //                             {
// //                                 newShape = ShapesIterator.Value();
// //                                 if (!origShape.IsSame(newShape) && newShape.ShapeType() == origShape.ShapeType())
// //                                 {
// //                                     TDF_Label returnedLabel;
// //                                     TDF_Label childLabel = Utilities::createNewChild(label.Father());
// //                                     allocateLabelWithTopShape(childLabel, newShape, m_name, id++, true);// add new shape
// // 
// // //                                     if (!Utilities::findShape(m_compoundLabel, newShape, returnedLabel))
// // //                                     {
// // //                                         TDF_Label childLabel = Utilities::createNewChild(label.Father());
// // //                                         allocateLabelWithTopShape(childLabel, newShape, m_name, id++, true);// add new shape
// // //                                     }
// //                                 }
// //                             }
// //                         }
// //                     }
// //                 }
// //             }
// //         }
// // 
// //         TDF_ChildIterator childIter(label);
// //         for (; childIter.More(); childIter.Next())
// //         {
// //             TDF_Label child = childIter.Value();
// //             labelModifyChild(child, type, operation);
// //         }
// 
//         break;
//     }
//     default:
//         break;
//     }
// }
// 
// void LabelShapeAdapter::labelDeleteChild(const TDF_Label& label, const EnAdapatType& type, void* operation)
// {
//     if (label.IsNull())
//     {
//         return;
//     }
// 
//     switch (type)
//     {
//     case EnCut:
//     case EnCommon:
//     case EnFuse:
//     {
//         BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TopoDS_Shape origShape;
//         Handle(TNaming_NamedShape) OriginalNShape;
//         if (!label.FindAttribute(TNaming_NamedShape::GetID(), OriginalNShape))
//         {
//             return;
//         }
//         else
//         {
//             if (!OriginalNShape->IsEmpty())
//             {
//                 origShape = OriginalNShape->Get();
//                 if (api->IsDeleted(origShape))
//                 {
//                     label.ForgetAllAttributes();
//                 }
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelDeleteChild(child, type, operation);
//         }
//         break;
//     }
//     case EnFillet:
//     {
//         BRepFilletAPI_MakeFillet* api = (BRepFilletAPI_MakeFillet*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TopoDS_Shape origShape;
//         Handle(TNaming_NamedShape) OriginalNShape;
//         if (!label.FindAttribute(TNaming_NamedShape::GetID(), OriginalNShape))
//         {
//             return;
//         }
//         else
//         {
//             if (!OriginalNShape->IsEmpty())
//             {
//                 origShape = OriginalNShape->Get();
//                 if (api->IsDeleted(origShape))
//                 {
//                     label.ForgetAllAttributes();
//                 }
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelDeleteChild(child, type, operation);
//         }
// 
//         break;
//     }
//     case EnCellsBuilder:
//     {
//         BOPAlgo_CellsBuilder* api = (BOPAlgo_CellsBuilder*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TopoDS_Shape origShape;
//         Handle(TNaming_NamedShape) OriginalNShape;
//         if (!label.FindAttribute(TNaming_NamedShape::GetID(), OriginalNShape))
//         {
//             return;
//         }
//         else
//         {
//             if (!OriginalNShape->IsEmpty())
//             {
//                 origShape = OriginalNShape->Get();
//                 if (api->IsDeleted(origShape))
//                 {
//                     label.ForgetAllAttributes();
//                 }
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelDeleteChild(child, type, operation);
//         }
// 
//         break;
//     }
//     default:
//         break;
//     }
// }
// 
// void LabelShapeAdapter::labelGenerateChild(const TDF_Label& label, const EnAdapatType& type, void* operation)
// {
//     if (label.IsNull())
//     {
//         return;
//     }
// 
//     switch (type)
//     {
//     case EnCut:
//     case EnCommon:
//     case EnFuse:
//     {
//         BRepAlgoAPI_BuilderAlgo* api = (BRepAlgoAPI_BuilderAlgo*)operation;
//         if (!api)
//         {
//             return;
//         }
// 
//         TopoDS_Shape origShape;
//         Handle(TNaming_NamedShape) OriginalNShape;
//         if (!label.FindAttribute(TNaming_NamedShape::GetID(), OriginalNShape))
//         {
//             return;
//         }
//         else
//         {
//             if (!OriginalNShape->IsEmpty())
//             {
//                 origShape = OriginalNShape->Get();
//                 if (!origShape.IsNull())
//                 {
//                     const TopTools_ListOfShape& Shapes = api->Modified(origShape);
//                     TopTools_ListIteratorOfListOfShape ShapesIterator(Shapes);
//                     for (; ShapesIterator.More(); ShapesIterator.Next())
//                     {
//                         const TopoDS_Shape& newShape = ShapesIterator.Value();
//                         // TNaming_Evolution == MODIFY
//                         if (!origShape.IsSame(newShape) && newShape.ShapeType() == origShape.ShapeType())
//                         {
//                             TDF_Label fatherLabel;
//                             if (!Utilities::findShape(m_compoundLabel, newShape) && Utilities::findFatherLabel(m_compoundLabel, newShape, true, fatherLabel))
//                             {
//                                 TDF_Label childLabel = Utilities::createNewChild(fatherLabel);
//                                 Utilities::copyLabelWithOnlyPropertyChild(label, childLabel);
//                                 TNaming_Builder ModBuilder(childLabel);
//                                 ModBuilder.Modify(origShape, newShape);
//                             }
//                         }
//                     }
//                 }
//             }
//         }
// 
//         TDF_ChildIterator childIter(label);
//         for (; childIter.More(); childIter.Next())
//         {
//             TDF_Label child = childIter.Value();
//             labelModifyChild(child, type, operation);
//         }
//         break;
//     }
//     case EnFillet:
//     {
//         break;
//     }
//     case EnCellsBuilder:
//     {
//         break;
//     }
//     default:
//         break;
//     }
// }