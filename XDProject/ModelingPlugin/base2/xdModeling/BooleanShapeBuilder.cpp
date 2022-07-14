#include "BooleanShapeBuilder.h"
#include "ShapeUtilities.h"
#include "BRepAlgoAPI_IntSS.hxx"
#include "VertexShapeBuilder.h"
#include "OCCBasicTools.h"
#include "SweepShapeBuilder.h"
#include "LabelShapeAdapter.h"

#include <algorithm>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRepFeat_SplitShape.hxx>
#include <TopoDS.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <BRepAlgoAPI_IntCS.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopExp_Explorer.hxx>
#include <gp_Pnt.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAlgoAPI_Splitter.hxx>
#include <BRepTest_Objects.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <gp_Pln.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BOPAlgo_MakerVolume.hxx>



BooleanShapeBuilder::BooleanShapeBuilder()
    :m_nonreg(nullptr), m_operationType(EnNormal)
{
    BOPAlgo_Options::SetParallelMode(true);
}


BooleanShapeBuilder::~BooleanShapeBuilder()
{
}

void* BooleanShapeBuilder::getBuilder() const
{
	if (m_operationType == EnNormal)
	{
		return ShapeOperationBuilder::getBuilder();
	}
	else if (m_operationType == EnCellsBuilderCut || m_operationType == EnCellsBuilderCommon || m_operationType == EnCellsBuilderFuse)
	{
		return m_nonreg;
	}
	else if (m_operationType == EnSew1)
	{
		return m_sewOperator;
	}
	else if (m_operationType == EnSew2)
	{
		return m_mkVolume;
	}
	else
	{
		return nullptr;
	}
}

bool BooleanShapeBuilder::booleanOperation(TopTools_ListOfShape args, TopTools_ListOfShape tools, BOPAlgo_Operation boolType, const double& fuzzyValue)
{
    try
    {
        if (args.IsEmpty() || tools.IsEmpty())
        {
            return false;
        }


		if (m_builder)
		{
			delete m_builder;
		}
        switch (boolType)
        {
        case BOPAlgo_CUT:
            m_builder = new BRepAlgoAPI_Cut;
            break;
        case BOPAlgo_COMMON:
            m_builder = new BRepAlgoAPI_Common;
            break;
        case BOPAlgo_FUSE:
            m_builder = new BRepAlgoAPI_Fuse;
            break;
        default:
            return false;
        }

        BRepAlgoAPI_BooleanOperation* builder = dynamic_cast<BRepAlgoAPI_BooleanOperation*>(m_builder);
        builder->SetArguments(args);
        builder->SetTools(tools);
        builder->SetFuzzyValue(fuzzyValue);
        builder->SetRunParallel(true);

        builder->Build();
        if (!m_builder->IsDone()) {
            return false;
        }

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool BooleanShapeBuilder::booleanOperation(TopoDS_Shape shape1, TopoDS_Shape shape2, BOPAlgo_Operation boolType, const double& fuzzyValue)
{
    try
    {
        if (shape1.IsNull() || shape2.IsNull())
        {
            return false;
        }

		if (m_builder)
		{
			delete m_builder;
		}
        switch (boolType)
        {
        case BOPAlgo_CUT:
            m_builder = new BRepAlgoAPI_Cut(shape1, shape2);
            break;
        case BOPAlgo_COMMON:
            m_builder = new BRepAlgoAPI_Common(shape1, shape2);
            break;
        case BOPAlgo_FUSE:
            m_builder = new BRepAlgoAPI_Fuse(shape1, shape2);
            break;
		case BOPAlgo_SECTION:
			m_builder = new BRepAlgoAPI_Section(shape1, shape2);
			break;
        default:
            return false;
        }

        BRepAlgoAPI_BooleanOperation* builder = dynamic_cast<BRepAlgoAPI_BooleanOperation*>(m_builder);
        builder->SetFuzzyValue(fuzzyValue);
        builder->SetRunParallel(true);

        builder->Build();
        if (!m_builder->IsDone()) {
            return false;
        }

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool BooleanShapeBuilder::singleNonRegFuseOperation(TopoDS_Shape shape1, TopoDS_Shape shape2)//TODO
{
    try
    {
		m_operationType = EnCellsBuilderCut;

		if (m_nonreg)
		{
			delete m_nonreg;
		}
        m_nonreg = new BOPAlgo_CellsBuilder;
        m_nonreg->Clear();
        m_nonreg->AddArgument(shape1);
        m_nonreg->AddArgument(shape2);
        m_nonreg->Perform(); // build splits of all arguments (GF)
        if (m_nonreg->HasErrors()) { // check error status
            return false;
        }
        m_nonreg->AddAllToResult();

        setShape(m_nonreg->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool BooleanShapeBuilder::faceCommon(TopTools_ListOfShape args, TopTools_ListOfShape tools,
	LabelShapeAdapter* adapter, double fuzzyvalue)
{
	TopoDS_Compound result;
	BRep_Builder B;
	B.MakeCompound(result);
	for (TopTools_ListOfShape::Iterator argiter(tools); argiter.More(); argiter.Next())
	{
		TopoDS_Shape curarg = argiter.Value();
		for (TopTools_ListOfShape::Iterator tooliter(tools); tooliter.More(); tooliter.Next())
		{
			TopoDS_Shape curtool = tooliter.Value();
			bool needsection = true;
			TopoDS_Shape curresult;
			if (booleanOperation(curarg, curtool, BOPAlgo_COMMON, 0))
			{
				curresult = m_shape;
				if (curresult.NbChildren() != 0)
					needsection = false;
			}
			if (needsection)
			{
				if (booleanOperation(curarg, curtool, BOPAlgo_SECTION, 0))
					curresult = m_shape;
			}

			if (curresult.NbChildren() == 0)
				continue;

			adapter->collectRelatedLabelAndShapes(curarg, EnCommon, m_builder);
			adapter->collectRelatedLabelAndShapes(curtool, EnCommon, m_builder);

			TopTools_ListOfShape cursubshapes;
			ShapeUtilities::GetAllNonCompoundShapes(curresult, cursubshapes);
			for (TopTools_ListOfShape::Iterator iter(cursubshapes); iter.More(); iter.Next())
				B.Add(result, iter.Value());
		}
	}
	if (result.NbChildren() == 0)
	{
		m_shape = TopoDS_Shape();
		return false;
	}
	else
	{
		m_shape = result;
		return true;
	}
}

bool BooleanShapeBuilder::cellsBuilderOperation(TopTools_ListOfShape args,
	TopTools_ListOfShape tools, EnAdapatType boolType, bool isRemoveInternal,
	double fuzzyValue)
{
	try
	{
		if (m_pBOPAlgo)
		{
			m_pBOPAlgo = NULL;
			delete m_pBOPAlgo;
		}
		if (args.Size() < 1 && tools.Size() < 1)
			return false;
		m_operationType = boolType;

		m_pBOPAlgo = new BOPAlgo_CellsBuilder;
		BOPAlgo_CellsBuilder* nonreg = dynamic_cast<BOPAlgo_CellsBuilder*>(m_pBOPAlgo);
		nonreg->SetFuzzyValue(fuzzyValue);
		nonreg->SetRunParallel(true);
		nonreg->Clear();
		TopTools_ListOfShape::Iterator iter(args);
		for (; iter.More(); iter.Next())
			nonreg->AddArgument(iter.Value());
		iter.Init(tools);
		for (; iter.More(); iter.Next())
			nonreg->AddArgument(iter.Value());

		nonreg->Perform();

		if (nonreg->HasErrors())
			return false;

		TopTools_ListOfShape totake, tovoid;
		switch (boolType)
		{
		case EnNormal:
			break;
		case EnCopy:
			break;
		case EnExtrude:
			break;
		case EnExtrudeDraft:
			break;
		case EnRevol:
			break;
		case EnSweep:
			break;
		case EnLoft:
			break;
		case EnThicken:
			break;
		case EnOffset:
			break;
		case EnCut:
			break;
		case EnCommon:
			break;
		case EnSection:
			break;
		case EnFuse:
			break;
		case EnCellsBuilderCut:
			iter.Init(args);
			for (; iter.More(); iter.Next())
			{
				totake.Append(iter.Value());
			}
			iter.Init(tools);
			for (; iter.More(); iter.Next())
			{
				tovoid.Append(iter.Value());
			}
			nonreg->AddToResult(totake, tovoid, 1);
			if (isRemoveInternal)
				nonreg->RemoveInternalBoundaries();
			break;
		case EnCellsBuilderCommon:
			iter.Init(args);
			for (; iter.More(); iter.Next())
			{
				totake.Append(iter.Value());
			}
			iter.Init(tools);
			for (; iter.More(); iter.Next())
			{
				tovoid.Append(iter.Value());
			}
			nonreg->AddToResult(totake, tovoid, 1);
			if (isRemoveInternal)
				nonreg->RemoveInternalBoundaries();
			break;
		case EnCellsBuilderFuse:
			nonreg->AddAllToResult(1);
			if (isRemoveInternal)
				nonreg->RemoveInternalBoundaries();
			break;
		case EnDetachEdges:
			break;
		case EnSew1:
			break;
		case EnSew2:
			break;
		case EnFillet:
			break;
		case EnChamfer:
			break;
		case EnFillet2D:
			break;
		default:
			break;
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool BooleanShapeBuilder::cellsBuilderOperation(TopTools_ListOfShape args, EnAdapatType boolType)
{
    try
    {
		if (m_nonreg)
		{
			delete m_nonreg;
		}
		m_operationType = boolType;
        m_nonreg = new BOPAlgo_CellsBuilder;
        m_nonreg->SetRunParallel(true);
//         m_nonreg->SetFuzzyValue();
        m_nonreg->Clear();
        TopTools_ListOfShape::Iterator iter(args);
        for (; iter.More(); iter.Next())
        {
            m_nonreg->AddArgument(iter.Value());
        }
        m_nonreg->Perform(); // build splits of all arguments (GF)
        if (m_nonreg->HasErrors()) { // check error status
            return false;
        }

//         m_nonreg->AddAllToResult();
        TopTools_ListOfShape aLSToTake, aLSToAvoid;
        switch (boolType)
        {
        case EnCellsBuilderCut:
            iter.Init(args);
            aLSToTake.Append(iter.Value());
            iter.Next();
            for (; iter.More(); iter.Next())
            {
                aLSToAvoid.Append(iter.Value());
            }
            m_nonreg->AddToResult(aLSToTake, aLSToAvoid);
            break;
        case EnCellsBuilderCommon:
            iter.Init(args);
            for (; iter.More(); iter.Next())
            {
                aLSToTake.Append(iter.Value());
            }
            m_nonreg->AddToResult(aLSToTake, aLSToAvoid);
            break;
        case EnCellsBuilderFuse:
            m_nonreg->AddAllToResult();
            break;
        default:
            return false;
        }

        setShape(m_nonreg->Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool BooleanShapeBuilder::detachEdges(TopoDS_Shape origWire, TopTools_ListOfShape toolEdges, const bool& isCreated)
{
	try
	{
		//check
		TopTools_ListOfShape::Iterator iter(toolEdges);
		for (; iter.More(); iter.Next())
		{
			if (iter.Value().ShapeType() != TopAbs_EDGE)
			{
				return false;
			}
		}

		if (origWire.ShapeType() != TopAbs_WIRE)
		{
			return false;
		}

		if (isCreated)//用工具线组合成新的线框或者混合体
		{
			TopTools_ListOfShape args;
			args.Append(origWire);
			if (!booleanOperation(args, toolEdges, BOPAlgo_COMMON, Precision::Confusion()))
			{
				return false;
			}

			ShapeUtilities::shapeFilter(m_shape, TopAbs_WIRE, m_shape);

// 			TopoDS_Wire finalWire;
// 			m_builder = new BRepBuilderAPI_MakeWire(finalWire);
// 			BRepBuilderAPI_MakeWire* builder = dynamic_cast<BRepBuilderAPI_MakeWire*>(m_builder);
// 			TopTools_ListOfShape::Iterator iter(toolEdges);
// 			for (; iter.More(); iter.Next())
// 			{
// 				builder->Add(TopoDS::Edge(iter.Value()));
// 			}
// 			builder->Build();
// 			if (builder->IsDone())
// 			{
// 				m_shape = builder->Shape();
// 			}
// 			else
// 			{
// 				TopTools_ListOfShape args;
// 				args.Append(origWire);
// 				return booleanOperation(args, toolEdges, BOPAlgo_CUT, Precision::Confusion());
// 			}
		}
		else//用origWire减去工具线，结果如果连续则是线框，否则是线混合
		{
			TopTools_ListOfShape args;
			args.Append(origWire);
			if (!booleanOperation(args, toolEdges, BOPAlgo_CUT, Precision::Confusion()))
			{
				return false;
			}

			ShapeUtilities::shapeFilter(m_shape, TopAbs_WIRE, m_shape);
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool BooleanShapeBuilder::faceSplitOperation(TopoDS_Shape shape1, TopoDS_Shape shape2)
{
    try
    {
        //get face
        TopoDS_Face face1;
        if (shape1.ShapeType() <= TopAbs_FACE)
        {
            if (!ShapeUtilities::shapeFilter(shape1, TopAbs_FACE, face1))
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        if (shape2.ShapeType() <= TopAbs_WIRE)
        {
            if (!ShapeUtilities::shapeFilter(shape2, TopAbs_WIRE, shape2))
            {
                return false;
            }
        }
        else if (shape2.ShapeType() != TopAbs_EDGE)
        {
            return false;
        }

		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepFeat_SplitShape();
        BRepFeat_SplitShape* builder = dynamic_cast<BRepFeat_SplitShape*>(m_builder);
        if (shape2.ShapeType() == TopAbs_WIRE)
        {
            builder->Add(TopoDS::Wire(shape2), face1);
        }
        else
        {
            builder->Add(TopoDS::Edge(shape2), face1);
        }
        builder->Build();
        if (!m_builder->IsDone()) {
            return false;
        }

        BRep_Builder aBuilder;
        TopoDS_Compound aLeftCompound;
        TopoDS_Compound aRightCompound;

        aBuilder.MakeCompound(aLeftCompound);
        aBuilder.MakeCompound(aRightCompound);

        // Left shape.
        TopTools_MapOfShape aLeftShapeMap;
        const TopTools_ListOfShape& aLeftShapes = builder->Left();
        for (auto i = aLeftShapes.cbegin(); i != aLeftShapes.cend(); i++)
        {
            aLeftShapeMap.Add(*i);

            aBuilder.Add(aLeftCompound, *i);
        }

        // Right shape.
        TopTools_IndexedMapOfShape aShapeMap;
        TopExp::MapShapes(builder->Shape(), TopAbs_FACE, aShapeMap);

        for (auto i = aShapeMap.cbegin(); i != aShapeMap.cend(); i++)
        {
            if (!aLeftShapeMap.Contains(*i))
            {
                aBuilder.Add(aRightCompound, *i);
            }
        }

        setShape(aLeftCompound);
        m_restShape = aRightCompound;

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool BooleanShapeBuilder::faceSplitOperation(TopoDS_Shape shape1, TopTools_ListOfShape toolEdges)
{
    try
    {
        //get face
        TopoDS_Face face1;
        if (shape1.ShapeType() <= TopAbs_FACE)
        {
            if (!ShapeUtilities::shapeFilter(shape1, TopAbs_FACE, face1))
            {
                return false;
            }
        }
        else
        {
            return false;
        }

		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepFeat_SplitShape();
        BRepFeat_SplitShape* builder = dynamic_cast<BRepFeat_SplitShape*>(m_builder);
        TopTools_ListOfShape::Iterator iter(toolEdges);
        for (; iter.More(); iter.Next())
        {
            TopoDS_Shape toolShape = iter.Value();
            if (toolShape.ShapeType() != TopAbs_EDGE)
            {
                return false;
            }

            builder->Add(TopoDS::Edge(toolShape), face1);
        }
        builder->Build();
        if (!m_builder->IsDone()) {
            return false;
        }

        BRep_Builder aBuilder;
        TopoDS_Compound aLeftCompound;
        TopoDS_Compound aRightCompound;

        aBuilder.MakeCompound(aLeftCompound);
        aBuilder.MakeCompound(aRightCompound);

        // Left shape.
        TopTools_MapOfShape aLeftShapeMap;
        const TopTools_ListOfShape& aLeftShapes = builder->Left();
        for (auto i = aLeftShapes.cbegin(); i != aLeftShapes.cend(); i++)
        {
            aLeftShapeMap.Add(*i);

            aBuilder.Add(aLeftCompound, *i);
        }

        // Right shape.
        TopTools_IndexedMapOfShape aShapeMap;
        TopExp::MapShapes(builder->Shape(), TopAbs_FACE, aShapeMap);

        for (auto i = aShapeMap.cbegin(); i != aShapeMap.cend(); i++)
        {
            if (!aLeftShapeMap.Contains(*i))
            {
                aBuilder.Add(aRightCompound, *i);
            }
        }

        setShape(aLeftCompound);
        m_restShape = aRightCompound;

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool BooleanShapeBuilder::shapeSplitOperation(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
{
	try
	{
		//get bounding box
		double theXmin, theYmin, theZmin, theXmax, theYmax, theZmax;
		if (!OCCBasicTools::GetBndBoxParams(srcShape, theXmin, theYmin, theZmin, theXmax, theYmax, theZmax))
		{
			return false;
		}

		//calculate length of diagonal line
		double length = sqrt(pow(theXmax - theXmin, 2) + pow(theYmax - theYmin, 2) + pow(theZmax - theZmin, 2));
		gp_Pnt origBasePnt(basePosX, basePosY, basePosZ);
		gp_Pnt closePnt((theXmin+theXmax)*0.5, (theYmin + theYmax)*0.5, (theZmin + theZmax)*0.5);
		gp_Dir faceNormal(normalVecX, normalVecY, normalVecZ);

		//translate base point to suitable position
		gp_Pln cutPlane(origBasePnt, faceNormal);
		gp_Pnt newBasePnt(closePnt);
		double distance = cutPlane.Distance(closePnt);
		if (distance != 0)
		{
			gp_Dir dirTowardsShape(gp_Vec(closePnt, origBasePnt));
			if (dirTowardsShape.Angle(faceNormal) > M_PI_2)//same direction
			{
				distance = -distance;
			}

			newBasePnt = closePnt.Translated(faceNormal.XYZ()*distance);
		}

		//build cut box
		Handle(Geom_Plane) newCutPlane = new Geom_Plane(newBasePnt, faceNormal);
		BRepBuilderAPI_MakeFace MWFace(newCutPlane, -3.0*length, 3.0*length, -3.0*length, 3.0*length, Precision::Confusion());
		if (!MWFace.IsDone())
		{
			return false;
		}
		TopoDS_Shape cutFaceShape = MWFace.Shape();
		SweepShapeBuilder extrude;
		if (!extrude.extrudeOperation(cutFaceShape, 3.0*length*faceNormal.X(), 3.0*length*faceNormal.Y(), 3.0*length*faceNormal.Z()))
		{
			return false;
		}
		TopoDS_Shape cutBox = extrude.getShape();

		//get one side
		if (normalVecSide)
		{
			return booleanOperation(srcShape, cutBox, BOPAlgo_COMMON, Precision::Confusion());
		}
		else
		{
			return booleanOperation(srcShape, cutBox, BOPAlgo_CUT, Precision::Confusion());
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool BooleanShapeBuilder::shapeSplitOperation2(TopoDS_Shape srcShape, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const bool& normalVecSide)
{
	try
	{
		//get bounding box
		double theXmin, theYmin, theZmin, theXmax, theYmax, theZmax;
		if (!OCCBasicTools::GetBndBoxParams(srcShape, theXmin, theYmin, theZmin, theXmax, theYmax, theZmax))
		{
			return false;
		}

		//calculate length of diagonal line
		double length = sqrt(pow(theXmax - theXmin, 2) + pow(theYmax - theYmin, 2) + pow(theZmax - theZmin, 2));
		gp_Pnt origBasePnt(basePosX, basePosY, basePosZ);
		gp_Pnt closePnt(theXmin, theYmin, theZmin);
		gp_Dir faceNormal(normalVecX, normalVecY, normalVecZ);

		//translate base point to suitable position
		gp_Pln cutPlane(origBasePnt, faceNormal);
		gp_Pnt newBasePnt(closePnt);
		double distance = cutPlane.Distance(closePnt);
		if (distance != 0)
		{
			gp_Dir dirTowardsShape(gp_Vec(closePnt, origBasePnt));
			if (dirTowardsShape.Angle(faceNormal) > M_PI_2)//same direction
			{
				distance = -distance;
			}

			newBasePnt = closePnt.Translated(faceNormal.XYZ()*distance);
		}

		//build cut box
		Handle(Geom_Plane) newCutPlane = new Geom_Plane(newBasePnt, faceNormal);
		BRepBuilderAPI_MakeFace MWFace(newCutPlane, -2.0*distance, 2.0*distance, -2.0*distance, 2.0*distance, Precision::Confusion());
		if (!MWFace.IsDone())
		{
			return false;
		}
		TopoDS_Shape cutFaceShape = MWFace.Shape();

		TopTools_ListOfShape toolShapes;
		toolShapes.Append(cutFaceShape);
		return shapeSplitOperation2(srcShape, toolShapes);
// 		//split
// 		TopTools_ListOfShape argShapes, toolShapes;
// 		argShapes.Append(srcShape);
// 		toolShapes.Append(cutFaceShape);
// 
// 		m_builder = new BRepAlgoAPI_Splitter;
// 		BRepAlgoAPI_Splitter* builder = dynamic_cast<BRepAlgoAPI_Splitter*>(m_builder);
// 		builder->SetArguments(argShapes);
// 		builder->SetTools(toolShapes);
// 		builder->Build();
// 
// 		if (!builder->IsDone())
// 		{
// 			return false;
// 		}
// 
// 		setShape(builder->Shape());
// 
// 		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool BooleanShapeBuilder::shapeSplitOperation2(TopoDS_Shape srcShape, TopTools_ListOfShape cutShapes)
{
	try
	{
		//split
		TopTools_ListOfShape argShapes;
		argShapes.Append(srcShape);

		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepAlgoAPI_Splitter;
		BRepAlgoAPI_Splitter* builder = dynamic_cast<BRepAlgoAPI_Splitter*>(m_builder);
		builder->SetArguments(argShapes);
		builder->SetTools(cutShapes);
		builder->Build();

		if (builder->HasErrors())
		{
			return false;
		}

		setShape(builder->Shape());

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool BooleanShapeBuilder::shapeSewing(TopTools_ListOfShape shapes, bool constructCompound)
{
	try
	{
		m_operationType = EnSew1;

		if (m_sewOperator)
		{
			delete m_sewOperator;
		}
		m_sewOperator = new BRepBuilderAPI_Sewing(1.0e-06, Standard_True, Standard_True, Standard_True, Standard_True);
		if (constructCompound)
		{
			TopoDS_Shape cmp;
			if (!OCCBasicTools::createCompound(shapes, cmp))
			{
				return false;
			}
			m_sewOperator->Load(cmp);
		}
		else
		{
			TopTools_ListOfShape::Iterator iter(shapes);
			m_sewOperator->Load(iter.Value());
			iter.Next();
			for (; iter.More(); iter.Next())
			{
				m_sewOperator->Add(iter.Value());
			}
		}
		m_sewOperator->Perform();

		TopoDS_Shape sewedShape = m_sewOperator->SewedShape();
// 		TopExp_Explorer exp(sewedShape, TopAbs_SHELL);
// 		for (; exp.More(); exp.Next())
// 		{
// 			setShape(exp.Value());
// 			break;
// 		}

		setShape(sewedShape);
		if (m_shape.IsNull() || m_shape.NbChildren() == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}

}

bool BooleanShapeBuilder::volumeMarker(TopTools_ListOfShape faces)
{
	try
	{
		m_operationType = EnSew2;


		if (m_mkVolume)
		{
			delete m_mkVolume;
		}
		m_mkVolume = new BOPAlgo_MakerVolume;
		m_mkVolume->SetArguments(faces);
		m_mkVolume->SetGlue(BOPAlgo_GlueFull);
		m_mkVolume->Perform();

		if (m_mkVolume->HasErrors())
		{
			return false;
		}

		setShape(m_mkVolume->Shape());
		if (m_shape.IsNull()|| m_shape.NbChildren() == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
}

bool BooleanShapeBuilder::shapeImprintOperation(TopoDS_Shape mainshape, TopTools_ListOfShape tools, const double& fuzzyvalue, int method)
{
	TopTools_ListOfShape sectionresult;
	//过滤掉不接触的
	for (auto iter : tools)
	{
		if (!booleanOperation(mainshape, iter, BOPAlgo_SECTION, 0))
		{
			continue;
		}
		else
		{
			if (m_shape.NbChildren() == 0)
				continue;

			sectionresult.Append(m_shape);
		}
	}

	//处理有接触的tools
	TopTools_ListOfShape args;
	args.Append(mainshape);
	if (!cellsBuilderOperation(args, sectionresult, EnCellsBuilderFuse, true, 0))
	{
		return false;
	}
	else
	{
		if (m_shape.NbChildren() == 0)
			return false;
	}
	return true;
}

bool BooleanShapeBuilder::splitReshape(TopoDS_Shape blank, TopTools_ListOfShape tools)
{
	BOPAlgo_Splitter splitter;
	splitter.AddArgument(blank);
	splitter.SetTools(tools);
	splitter.Perform();
	if (splitter.HasErrors())
		return false;
	setShape(splitter.Shape());
	return true;
}