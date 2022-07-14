#include "IOShapeBuilder.h"
#include "OccApplication.h"
#include "LabelUtilities.h"
#include "LabelShapeAdapter.h"
#include "OccModel.h"

#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Controller.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TDF_Tool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TopoDS_Iterator.hxx>
#include <IGESCAFControl_Reader.hxx>
#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>
#include <TDataStd_TreeNode.hxx>
#include <XCAFDoc.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDataStd_Integer.hxx>

#include <regex>

std::vector<std::string> regexsplit(const std::string& input)
{
    std::regex re(",");

    std::sregex_token_iterator p(input.begin(), input.end(), re, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec;
    while (p != end)
        vec.emplace_back(*p++);

    return vec;
}

IOShapeBuilder::IOShapeBuilder()
    :m_fileType(IGES)
{
}


IOShapeBuilder::~IOShapeBuilder()
{
}

bool IOShapeBuilder::importIGESModelViaXDE(const std::string& path)
{
    try
    {
        m_fileType = IGES;
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        //build XDE tree
        IGESCAFControl_Reader reader;
        IFSelect_ReturnStatus readstat = reader.ReadFile(aFile);
        if (readstat != IFSelect_RetDone)
        {
            return false;
        }

        Handle(TDocStd_Document) xdeDoc;
        TheOCCApp.getApp()->NewDocument("XmlOcaf", xdeDoc);
        if (!reader.Transfer(xdeDoc)) {
            return false;
        }

        Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(xdeDoc->Main());
        Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(xdeDoc->Main());

        //build normal tree
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        //         Handle(XCAFDoc_ShapeTool) shapeTool2 = XCAFDoc_DocumentTool::ShapeTool(curDoc->Main());
        //         Handle(XCAFDoc_ColorTool) colorTool2 = XCAFDoc_DocumentTool::ColorTool(curDoc->Main());
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();
        TDF_ChildIterator childIter(xdeDoc->GetData()->Root());
        for (; childIter.More(); childIter.Next())
        {
            TDF_Label shapeLabel = childIter.Value().FindChild(1);
            TDF_ChildIterator solidLabelIter(shapeLabel);
            for (; solidLabelIter.More(); solidLabelIter.Next())
            {
                TDF_Label childLabel = solidLabelIter.Value();
                TopoDS_Shape topShape = LabelUtilities::getLabelShape(childLabel);
                if (topShape.IsNull())
                {
                    continue;
                }

                transferSolidLevelShapeToLabel(topShape, rootLabel, topShape.Location(), shapeTool);
            }
        }
        
        TheOCCApp.getApp()->SaveAs(xdeDoc, "C:\\Users\\Administrator\\Desktop\\testPUMPIGS.xml");
        TheOCCApp.getApp()->SaveAs(curDoc, "C:\\Users\\Administrator\\Desktop\\testPUMPIGS.cbf");

        return true;
    }
    catch (...)
    {
        return false;
    }


//     try
//     {
//         char* curFile = (char*)(path.c_str());
//         Standard_CString aFile = (Standard_CString)curFile;
// 
//         IGESControl_Reader reader;
//         IFSelect_ReturnStatus status = reader.ReadFile(aFile);
//         if (status != IFSelect_RetDone)
//         {
//             return false;
//         }
// 
//         if (status == IFSelect_RetDone)
//         {
//             reader.TransferRoots();
//             m_shape = reader.OneShape();
//             if (m_shape.IsNull())
//             {
//                 // Collecting resulting entities
//                 Standard_Integer nbs = reader.NbShapes();
//                 BRep_Builder builder;
//                 TopoDS_Compound cmp;
//                 builder.MakeCompound(cmp);
//                 for (Standard_Integer i = 1; i <= nbs; i++)
//                 {
//                     builder.Add(cmp, reader.Shape(i));
//                 }
//                 m_shape = cmp;
//             }
// 
//             Handle(IGESData_IGESModel) modelData = reader.IGESModel();
//         }
// 
//         return true;
//     }
//     catch (...)
//     {
//         return false;
//     }
}

bool IOShapeBuilder::importIGESModel(const std::string& path)
{
    try
    {
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        IGESControl_Reader reader;
        IFSelect_ReturnStatus status = reader.ReadFile(aFile);
        if (status != IFSelect_RetDone)
        {
            return false;
        }

        if (status == IFSelect_RetDone)
        {
            reader.TransferRoots();
            m_shape = reader.OneShape();
            if (m_shape.IsNull())
            {
                // Collecting resulting entities
                Standard_Integer nbs = reader.NbShapes();
                BRep_Builder builder;
                TopoDS_Compound cmp;
                builder.MakeCompound(cmp);
                for (Standard_Integer i = 1; i <= nbs; i++)
                {
                    builder.Add(cmp, reader.Shape(i));
                }
                m_shape = cmp;
            }
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool IOShapeBuilder::exportIGESModel(const std::string& path)
{
    try
    {
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        IGESControl_Controller::Init();
        IGESControl_Writer ICW("MM", 0);
        //creates a writer object for writing in Face mode with  millimeters 
        ICW.AddShape(m_shape);
        //adds shape sh to IGES model 
        ICW.ComputeModel();
        Standard_Boolean ok = ICW.Write(aFile);
        if (!ok)
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

bool IOShapeBuilder::importSTEPModelViaXDE(const std::string& path)
{
    try
    {
        m_fileType =STEP;

        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        //build XDE tree
        STEPCAFControl_Reader reader;
        reader.SetSourceCodePage(Resource_FormatType_GB);
        IFSelect_ReturnStatus readstat = reader.ReadFile(aFile);
        if (readstat != IFSelect_RetDone)
        {
            return false;
        }

        Handle(TDocStd_Document) xdeDoc;
        TheOCCApp.getApp()->NewDocument("XmlOcaf", xdeDoc);
        if (!reader.Transfer(xdeDoc)) {
            return false;
        }
//         TheOCCApp.getApp()->SaveAs(xdeDoc, "C:\\Users\\Administrator\\Desktop\\testPUMPIGS.xml");

        Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(xdeDoc->Main());
        Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(xdeDoc->Main());

        //build normal tree
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
//         Handle(XCAFDoc_ShapeTool) shapeTool2 = XCAFDoc_DocumentTool::ShapeTool(curDoc->Main());
//         Handle(XCAFDoc_ColorTool) colorTool2 = XCAFDoc_DocumentTool::ColorTool(curDoc->Main());
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();
        TDF_ChildIterator childIter(xdeDoc->GetData()->Root());
        for (; childIter.More(); childIter.Next())
        {
            TDF_Label sourceTopLabel = childIter.Value().FindChild(1).FindChild(1);
            TopoDS_Shape topShape = LabelUtilities::getLabelShape(sourceTopLabel);

            transferSolidLevelShapeToLabel(topShape, rootLabel, topShape.Location(), shapeTool);
        }

        TheOCCApp.getApp()->SaveAs(xdeDoc, "C:\\Users\\Administrator\\Desktop\\testAGV1.xml");
        TheOCCApp.getApp()->SaveAs(curDoc, "C:\\Users\\Administrator\\Desktop\\testAGV2.cbf");

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool IOShapeBuilder::importSTEPModel(const std::string& path)
{
    try
    {
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        STEPControl_Reader Reader;
        Standard_Integer status = Reader.ReadFile(aFile);
        if (status != IFSelect_RetDone)
        {
            return false;
        }

        if (status == IFSelect_RetDone)
        {
            Reader.TransferRoots();
            m_shape = Reader.OneShape();
            if (m_shape.IsNull())
            {
                // Collecting resulting entities
                Standard_Integer nbs = Reader.NbShapes();
                BRep_Builder builder;
                TopoDS_Compound cmp;
                builder.MakeCompound(cmp);
                for (Standard_Integer i = 1; i <= nbs; i++)
                {
                    builder.Add(cmp, Reader.Shape(i));
                }
                m_shape = cmp;
            }
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}
bool IOShapeBuilder::exportSTEPModelViaXDE(const std::string& path)
{
    try
    {
        m_fileType = STEP;

        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        //build XDE tree
        Handle(TDocStd_Document) xdeDoc;
        TheOCCApp.getApp()->NewDocument("XmlOcaf", xdeDoc);
        Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(xdeDoc->Main());
        Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(xdeDoc->Main());

        TDF_LabelList::Iterator iter(m_expLabelList);
        TopoDS_Compound cmp;
        BRep_Builder B;
        B.MakeCompound(cmp);
        for (; iter.More(); iter.Next())
        {
            TopoDS_Shape curShape = LabelUtilities::getLabelShape(iter.Value());
            B.Add(cmp, curShape);
        }
        TDF_Label cmpShapeLabel = shapeTool->AddShape(cmp);

        iter.Init(m_expLabelList);
        for (; iter.More(); iter.Next())
        {        
            TopoDS_Shape curShape = LabelUtilities::getLabelShape(iter.Value());
            TDF_Label shapeLabel = shapeTool->FindShape(curShape);
            if (shapeLabel.IsNull())
            {
                continue;
            }
            std::string name;
            if (LabelUtilities::getLabelName(iter.Value(), name))
            {
                Handle(TDataStd_TreeNode) treenode;
                if (shapeLabel.FindAttribute(XCAFDoc::ShapeRefGUID(), treenode))//no child treenode
                {
                    Handle(TDataStd_TreeNode) childTreenode = treenode->First();
                    Handle(TDataStd_Integer) calledMark;

                    if (childTreenode)
                    {
                        TDataStd_ChildNodeIterator treeIter(treenode);
                        for (; treeIter.More(); treeIter.Next())
                        {
                            TDF_Label linkChildLabel = treeIter.Value()->Label();
                            if (linkChildLabel.FindAttribute(TDataStd_Integer::GetID(), calledMark))//the label has been used before
                            {
                                continue;
                            }

                            LabelUtilities::setLabelName(linkChildLabel, name);
                            TDataStd_Integer::Set(linkChildLabel, 0);
                            break;
                        }
                    }
                }
            }

            std::string color;
            if (LabelUtilities::getLabelProperty(iter.Value(), "COLOR", color))
            {
                std::vector<std::string> values = regexsplit(color);
                if (values.size()!=4)
                {
                    continue;
                }
                for (int i = 0; i < values.size(); ++i)
                {
                    std::string value = values.at(i);
                    remove_if(value.begin(), value.end(), ::isspace);
                }

                Quantity_ColorRGBA rgba(Quantity_Color(stod(values.at(0)), stod(values.at(1)), stod(values.at(2)), Quantity_TOC_RGB), stod(values.at(3)));
                TDF_Label colorLabel = colorTool->AddColor(rgba);
                Handle(TDataStd_TreeNode) treenode, colorTreeNode;
                if (!shapeLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), treenode))
                {
                    treenode = TDataStd_TreeNode::Set(shapeLabel, XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen));
                }
                if (!colorLabel.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), colorTreeNode))
                {
                    colorTreeNode = TDataStd_TreeNode::Set(colorLabel, XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen));
                }
                treenode->Remove(); // abv: fix against bug in TreeNode::Append()
                colorTreeNode->Append(treenode);
            }
        }

        // 
//         TheOCCApp.getApp()->SaveAs(xdeDoc, "C:\\Users\\Administrator\\Desktop\\testExpAGV.xml");
        //         TheOCCApp.getApp()->SaveAs(curDoc, "C:\\Users\\Administrator\\Desktop\\testAGV2.cbf");
        STEPCAFControl_Writer writer;
        if (!writer.Transfer(xdeDoc)) {
            return false;
        }
        IFSelect_ReturnStatus writestat = writer.Write(aFile);
        if (writestat != IFSelect_RetDone)
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

bool IOShapeBuilder::exportSTEPModel(const std::string& path)
{
    try
    {
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        STEPControl_Controller::Init();
        STEPControl_Writer aWriter;
        IFSelect_ReturnStatus retStatus = aWriter.Transfer(m_shape, STEPControl_AsIs);
        IFSelect_ReturnStatus status = aWriter.Write(aFile);
        if (status != IFSelect_RetDone)
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

bool IOShapeBuilder::importBREPModel(const std::string& path)
{
    try
    {
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        BRep_Builder b;
        Standard_Boolean ok = BRepTools::Read(m_shape, aFile, b);
        if (!ok)
        {
            return false;
        }
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        TDF_Label targetlabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetlabel, anEntry);
        std::string strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetlabel);
        adapter.allocateLabelWithTopShape(targetlabel, m_shape);
        TDataStd_AsciiString::Set(targetlabel, "top_level");

        void* entity = NULL;
        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
        m_entityList.push_back(entity);
        return true;

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool IOShapeBuilder::exportBREPModel(const std::string& path)
{
    try
    {
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        Standard_Boolean ok = BRepTools::Write(m_shape, aFile);
        if (!ok)
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

void IOShapeBuilder::transferSolidLevelShapeToLabel(TopoDS_Shape shape, TDF_Label rootLabel, TopLoc_Location locationBefore, Handle(XCAFDoc_ShapeTool) shapeTool)
{
    TopLoc_Location Location = shape.Location();
    TopLoc_Location curLoc = /*locationBefore * */Location;

    bool removeTopLevel = true;
    if (shape.ShapeType() < TopAbs_SOLID)
    {
        TopoDS_Iterator iter(shape);
        for (; iter.More(); iter.Next())
        {
            if (iter.Value().ShapeType() > TopAbs_SOLID)
            {
                removeTopLevel = false;
                break;
            }
        }
    }
    else
    {
        removeTopLevel = false;
    }

    if (!removeTopLevel)
    {
        shape.Location(curLoc);

        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, shape);
        if (m_fileType == IGES)
        {
//             adapter.transferColorAndName4ImportingIges(targetLabel, shapeTool);
            adapter.transferColorAndName4ImportingStep(targetLabel, shapeTool);
        }
        else if (m_fileType == STEP)
        {
            adapter.transferColorAndName4ImportingStep(targetLabel, shapeTool);
        }
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        std::string strPID = anEntry.ToCString();
        void* entity = nullptr;
        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
        m_entityList.push_back(entity);
    }
    else
    {
        TopoDS_Iterator iter(shape);
        for (; iter.More(); iter.Next())
        {
            transferSolidLevelShapeToLabel(iter.Value(), rootLabel, curLoc, shapeTool);
        }
    }
}

bool IOShapeBuilder::exportSATModel(const std::string& path)
{
    try
    {
        char* curFile = (char*)(path.c_str());
        Standard_CString aFile = (Standard_CString)curFile;

        SatControl_Controller::Init();
        SatControl_Wirter awriter;
        IFSelect_ReturnStatus retStatus = awriter.TransferShape(m_shape);
        IFSelect_ReturnStatus status = awriter.WriteFile(aFile);
        if (status != IFSelect_RetDone)
            return false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}