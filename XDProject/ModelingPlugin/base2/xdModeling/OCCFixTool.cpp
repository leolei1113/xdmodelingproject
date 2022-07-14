// #include "stdafx.h"

#ifdef WIN32
#pragma warning( disable : 4786 )
#pragma warning( disable : 4251 ) 
#endif

#include "OCCFixTool.h"

#include <Precision.hxx>
#include <ShapeFix_Shape.hxx>
#include <TColStd_DataMapOfIntegerInteger.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRep_Builder.hxx>
#include <ShapeFix_Wireframe.hxx>
#include <ShapeAnalysis_Shell.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <ShapeFix_EdgeConnect.hxx>
#include <ShapeFix_FaceConnect.hxx>


static ShapeExtend_Status getReorderStatus(const Handle(ShapeFix_Wire)& theFix)
{
    if (theFix->StatusReorder(ShapeExtend_FAIL1)) return ShapeExtend_FAIL1;
    if (theFix->StatusReorder(ShapeExtend_FAIL2)) return ShapeExtend_FAIL2;
    if (theFix->StatusReorder(ShapeExtend_DONE1)) return ShapeExtend_DONE1;
    if (theFix->StatusReorder(ShapeExtend_DONE2)) return ShapeExtend_DONE2;
    if (theFix->StatusReorder(ShapeExtend_DONE3)) return ShapeExtend_DONE3;
    return ShapeExtend_OK;
}

OCCFixTool* OCCFixTool::m_OCCFixTool = NULL;
					
OCCFixTool::OCCFixTool()
{
    m_precision = m_minTol = m_maxTol = Precision::Confusion();
}

OCCFixTool::~OCCFixTool()
{
}

OCCFixTool* OCCFixTool::Instance()
{
	if (m_OCCFixTool == NULL)
	{
		static OCCFixTool aTool;
		m_OCCFixTool = &aTool;
	}
	return m_OCCFixTool;
}

void OCCFixTool::shapeValidityAnalysis(const TopoDS_Shape& aShape, const TopAbs_ShapeEnum& curType, std::string& message)
{
	switch (curType)
	{
	case TopAbs_COMPOUND:
	{
		break;
	}
	case TopAbs_COMPSOLID:
	{
		break;
	}
	case TopAbs_SOLID:
	{
		break;
	}
	case TopAbs_SHELL:
	{
		int orientationError = 0;
		bool freeEdges = false, connectedEdges = false;
		shellValidityAnalysis(aShape, orientationError, freeEdges, connectedEdges);
		message += "Shells check:\n";
		message += "Number of wrong oriented shells: " + std::to_string(orientationError) + ".\n";
		message += "Free edges exist: " + std::to_string(freeEdges) + ".\n";
		message += "Connected edges (edges shared twice or more) exist: " + std::to_string(freeEdges) + ".\n";
		message += "\n";
		break;
	}
	case TopAbs_FACE:
	{
		int spotFace = 0, stripFace = 0, twistedFace = 0;
		faceValidityAnalysis(aShape, spotFace, stripFace, twistedFace);
		message += "Faces check:\n";
		message += "Number of SpotFace: " + std::to_string(spotFace) + ".\n";
		message += "Number of StripFace: " + std::to_string(stripFace) + ".\n";
		message += "Number of TwistedFace: " + std::to_string(twistedFace) + ".\n";
		message += "\n";

		break;
	}
	case TopAbs_WIRE:
	{
		int orientationError = 0, disconnectedEdges = 0, smallEdges = 0, selfIntersection = 0;
		wireValidityAnalysis(aShape, orientationError, disconnectedEdges, smallEdges, selfIntersection);
		message += "Wires check:\n";
		message += "Number of wrong oriented wire: " + std::to_string(orientationError) + ".\n";
		message += "Number of disconnected edges: " + std::to_string(disconnectedEdges) + ".\n";
		message += "Number of small edges: " + std::to_string(smallEdges) + ".\n";
		message += "Number of self-intersected edges: " + std::to_string(selfIntersection) + ".\n";
		message += "\n";

		break;
	}
	case TopAbs_EDGE:
	{
		return;
	}
	case TopAbs_VERTEX:
	{
		return;
	}
	case TopAbs_SHAPE:
	{
		return;
	}
	default:
		break;
	}

	TopAbs_ShapeEnum nextType = TopAbs_ShapeEnum(curType + 1);
	shapeValidityAnalysis(aShape, nextType, message);
}

bool OCCFixTool::fixTopoValidityOp(TopoDS_Shape& aShape, const Standard_Boolean& wireframeRepair /*= Standard_False*/)
{
	try
	{
		Standard_Boolean success;
		Handle(ShapeFix_Shape) aShapeFix = new ShapeFix_Shape(aShape);
		PreCommit(aShapeFix);

		success = aShapeFix->Perform();
		if (aShapeFix->Status(ShapeExtend_FAIL)|| !success)
		{
			return false;
		}
		aShape = aShapeFix->Shape();

		if (wireframeRepair)
		{
			Handle(ShapeFix_Wireframe) SFWF = new ShapeFix_Wireframe(aShape);
			PreCommit(SFWF);

			SFWF->ModeDropSmallEdges() = Standard_True;
			success = SFWF->FixSmallEdges();
			success = SFWF->FixWireGaps();
			aShape = SFWF->Shape();
		}

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void OCCFixTool::FixShellOrientationOp(TopoDS_Shape& aShape)
{
	Handle(ShapeBuild_ReShape) Context = new ShapeBuild_ReShape;
	Context->Apply(aShape);

	TopExp_Explorer Exp(aShape, TopAbs_SHELL);
	for (; Exp.More(); Exp.Next())
	{
		TopoDS_Shell aShell = TopoDS::Shell(Exp.Current());
		//create tools for fixing a face 
		Handle(ShapeFix_Shell)  SFS = new ShapeFix_Shell;
		PreCommit(SFS);
		// create tool for rebuilding a shape and initialize it by shape 
		//set a tool for rebuilding a shape in the tool for fixing 
		SFS->SetContext(Context);
		//initialize the fixing tool by one face 
		SFS->Init(aShell);
		//fix the set face 
		SFS->Perform();

		PostCommit(aShape, SFS);
	}
}

void OCCFixTool::FixFaceOrientationOp(TopoDS_Shape& aShape)
{
	Handle(ShapeBuild_ReShape) Context = new ShapeBuild_ReShape;
	Context->Apply(aShape);

	TopExp_Explorer Exp(aShape, TopAbs_FACE);
	for (; Exp.More(); Exp.Next())
	{
		TopoDS_Face aFace = TopoDS::Face(Exp.Current());
		//create tools for fixing a face 
		Handle(ShapeFix_Face)  SFF = new ShapeFix_Face;
		PreCommit(SFF);
		// create tool for rebuilding a shape and initialize it by shape 
		//set a tool for rebuilding a shape in the tool for fixing 
		SFF->SetContext(Context);
		//initialize the fixing tool by one face 
		SFF->Init(aFace);
		SFF->RemoveSmallAreaFaceMode() = Standard_True;
		//fix the set face 
		SFF->Perform();
		PostCommit(aShape, SFF);
	}
}

void OCCFixTool::FixEdgeOrientationOp(TopoDS_Shape& aShape)
{
	Handle(ShapeFix_Wire) aWireFix = new ShapeFix_Wire;
	PreCommit(aWireFix);

	// map FixStatus - NbSuchStatuses
	TColStd_DataMapOfIntegerInteger aStatusNbDMap;
	// get faces to load fixer
	TopExp_Explorer aFaceExplorer(aShape, TopAbs_FACE);

	for (; aFaceExplorer.More(); aFaceExplorer.Next())
	{
		TopoDS_Shape aFace = aFaceExplorer.Current();
		// loop on wires
		TopoDS_Iterator aWireItr(aFace);
		for (; aWireItr.More(); aWireItr.Next())
		{
			// fix
			aWire = TopoDS::Wire(aWireItr.Value());
			aWireFix->Load(aWire);
			aWireFix->SetFace(TopoDS::Face(aFace));
			aWireFix->FixReorder();
			// keep status

			ShapeExtend_Status aStatus = getReorderStatus(aWireFix);
			if (aStatusNbDMap.IsBound(aStatus))
			{
				aStatusNbDMap(aStatus)++;
			}
			else
			{
				aStatusNbDMap.Bind(aStatus, 1);
			}

			m_theReShape->Replace(aWire, aWireFix->Wire());
		}
	}
	PostCommit(aShape, aWireFix);
}

void OCCFixTool::FixWireOrientationOp(TopoDS_Shape& aShape)
{
	Handle(ShapeFix_Face) aFaceFix = new ShapeFix_Face;
	PreCommit(aFaceFix);

	// parameters
	Standard_Boolean isNaturalAllowed = Standard_True;
	// nb fixed seams, nb failures, nb faces
	Standard_Integer nbFixes = 0, nbFail = 0, nbFaces = 0;

	// get faces to load fixer

	TopExp_Explorer aFaceExplorer(aShape, TopAbs_FACE);
	for (; aFaceExplorer.More(); aFaceExplorer.Next())
	{
		TopoDS_Face aSrcFace = TopoDS::Face(aFaceExplorer.Current());
		nbFaces++;
		// fix
		aFaceFix->Init(aSrcFace);
		if (aFaceFix->FixOrientation())
		{
			nbFixes++;
		}
		else if (aFaceFix->Status(ShapeExtend_FAIL2))
		{
			nbFail++;
		}
	}
	PostCommit(aShape, aFaceFix);
}

void OCCFixTool::FixEdgeConnection(TopoDS_Shape& aShape)
{
	ShapeFix_EdgeConnect aEedgeFix;
	aEedgeFix.Add(aShape);
	aEedgeFix.Build();
}

void OCCFixTool::FixFaceConnection(TopoDS_Shape& aShape)
{
	ShapeFix_FaceConnect aEedgeFix;
	aEedgeFix.Build(TopoDS::Shell(aShape), m_precision, m_precision);
}

void OCCFixTool::FixWire(TopoDS_Shape& aShape)
{
	Handle(ShapeFix_Wire) aWireFix = new ShapeFix_Wire;
	PreCommit(aWireFix);
	Standard_Boolean fixInterct = aWireFix->FixSelfIntersection();
	fixInterct = true;
	aWireFix->Load(TopoDS::Wire(aShape));
	PostCommit(aShape, aWireFix);
}

void OCCFixTool::PreCommit(const Handle(ShapeFix_Root)& theFix)
{
    m_theReShape = new ShapeBuild_ReShape();
    theFix->SetPrecision(m_precision);
    theFix->SetMinTolerance(m_minTol);
    theFix->SetMaxTolerance(m_maxTol);
    theFix->SetContext(m_theReShape);
}

void OCCFixTool::PostCommit(TopoDS_Shape& myShape, const Handle(ShapeFix_Root)& theFix)
{
    m_theReShape = theFix->Context();
    m_aShCtx = new ShapeProcess_ShapeContext(myShape, "");
    m_aShCtx->SetDetalisation(TopAbs_SHAPE);
    m_aShCtx->RecordModification(m_theReShape);
    m_theReShape->ModeConsiderLocation() = Standard_True;
    myShape = m_theReShape->Apply(myShape);
}

void OCCFixTool::shellValidityAnalysis(const TopoDS_Shape& aShape, int& orientationError, bool& freeEdges, bool& connectedEdges)
{
	orientationError = 0;

	ShapeAnalysis_Shell shellAnalysis;
	TopExp_Explorer exp(aShape, TopAbs_SHELL);
	for (; exp.More(); exp.Next())
	{
		if (shellAnalysis.CheckOrientedShells(exp.Value()))
		{
			orientationError++;
		}

		shellAnalysis.LoadShells(exp.Value());
	}

	freeEdges = shellAnalysis.HasFreeEdges();
	connectedEdges = shellAnalysis.HasConnectedEdges();
}

void OCCFixTool::faceValidityAnalysis(const TopoDS_Shape& aShape, int& spotFace, int& stripFace, int& twistedFace)
{
	spotFace = 0;
	stripFace = 0;
	twistedFace = 0;

	ShapeAnalysis_CheckSmallFace smallFaceAnalysis;
	TopExp_Explorer exp(aShape, TopAbs_FACE);
	for (; exp.More(); exp.Next())
	{
		if (smallFaceAnalysis.CheckSpotFace(TopoDS::Face(exp.Value()), m_precision))
		{
			spotFace++;
		}

		if (smallFaceAnalysis.IsStripSupport(TopoDS::Face(exp.Value()), m_precision))
		{
			stripFace++;
		}

		double paramu, paramv;
		if (smallFaceAnalysis.CheckTwisted(TopoDS::Face(exp.Value()), paramu, paramv))
		{
			twistedFace++;
		}
	}
}

void OCCFixTool::wireValidityAnalysis(const TopoDS_Shape& aShape, int& orientationError, int& disconnectedEdges, int& smallEdges, int& selfIntersection)
{
	int i = 0;
	TopExp_Explorer exp(aShape, TopAbs_FACE);
	for (; exp.More(); exp.Next())
	{
		i++;
		TopoDS_Shape face = exp.Value();

		TopExp_Explorer exp2(aShape, TopAbs_WIRE);
		for (; exp2.More(); exp2.Next())
		{
			ShapeAnalysis_Wire wireAnalysis(TopoDS::Wire(exp2.Value()), TopoDS::Face(face), m_precision);
			if (wireAnalysis.CheckOrder())
			{
				orientationError++;
			}
			if (wireAnalysis.CheckConnected(m_precision))
			{
				disconnectedEdges++;
			}
			if (wireAnalysis.CheckSmall(m_precision))
			{
				smallEdges++;
			}
			if (wireAnalysis.CheckSelfIntersection())
			{
				selfIntersection++;
			}
		}
	}

	if (i == 0)
	{
		TopExp_Explorer exp(aShape, TopAbs_WIRE);
		for (; exp.More(); exp.Next())
		{
			ShapeAnalysis_Wire wireAnalysis;
			wireAnalysis.SetPrecision(m_precision);
			wireAnalysis.Load(TopoDS::Wire(exp.Value()));
			if (wireAnalysis.CheckOrder())
			{
				orientationError++;
			}
			if (wireAnalysis.CheckConnected(m_precision))
			{
				disconnectedEdges++;
			}
			if (wireAnalysis.CheckSmall(m_precision))
			{
				smallEdges++;
			}
			if (wireAnalysis.CheckSelfIntersection())
			{
				selfIntersection++;
			}
		}
	}
}

