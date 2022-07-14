#include "OccModelBuilder.h"
#include "OccModel.h"
#include "OccApplication.h"
#include "LabelUtilities.h"
#include "EnumAndStruct.h"
#include "LabelShapeAdapter.h"
#include "VertexShapeBuilder.h"
#include "EdgeShapeBuilder.h"
#include "FaceShapeBuilder.h"
#include "SolidShapeBuilder.h"
#include "IOShapeBuilder.h"
#include "BooleanShapeBuilder.h"
#include "FilletShapeBuilder.h"
#include "SweepShapeBuilder.h"
#include "ShapeUtilities.h"
#include "TransformShapeBuilder.h"
#include "SweepShapeBuilder.h"
#include "OffsetShapeBuilder.h"
#include "OCCBasicTools.h"
#include "FaceOperationBuilder.h"
#include "EdgeOperationBuilder.h"
#include "OCCFixTool.h"

#include <BRepBuilderAPI_Transform.hxx>
#include <gce_MakePln.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <ShapeUpgrade_ShapeDivide.hxx>
#include <TDF_Data.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TDocStd_Document.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TDataStd_Name.hxx>
#include <TNaming_Builder.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgo.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDF_Attribute.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TDF_Tool.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_ListOfShape.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopoDS.hxx>
#include <TDF_Label.hxx>
#include <BOPAlgo_CellsBuilder.hxx>
#include <gp_Pln.hxx>
#include <TDocStd_XLinkTool.hxx>
#include <TNaming_Tool.hxx>
#include <TDocStd_XLink.hxx>
#include <TDF_CopyLabel.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Controller.hxx>
#include <BRepTools.hxx>
#include <xdTopoDS_HShape.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TDocStd_Application.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <BOPAlgo_MakerVolume.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopoDS_HShape.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <BRepTools_ReShape.hxx>

OccModelBuilder::OccModelBuilder()
{
}

OccModelBuilder::~OccModelBuilder()
{

}

void OccModelBuilder::builderBegin()
{
	TheOCCApp.getCurrentModel()->BeginCommand();
}

void OccModelBuilder::builderEnd()
{
	bool success = TheOCCApp.getCurrentModel()->EndCommand();
}

bool OccModelBuilder::addModelAttribute(void* modelEntity, const std::string& propertyName, const int& value)
{
	bool suc = true;
	std::string strPID;
	suc = TheOCCApp.getCurrentModel()->AddEntityAttribute(modelEntity, propertyName, value);
	return suc;
}

bool OccModelBuilder::addModelAttribute(void* modelEntity, const std::string& propertyName, const std::string& value)
{
	bool suc = true;
	std::string strPID;
    suc = TheOCCApp.getCurrentModel()->AddEntityAttribute(modelEntity, propertyName, value);
    return suc;
}

bool OccModelBuilder::addModelAttribute(void* modelEntity, const std::string& propertyName, const double& value)
{
	bool suc = true;
	std::string strPID;
    suc = TheOCCApp.getCurrentModel()->AddEntityAttribute(modelEntity, propertyName, value);
    return suc;
}

bool OccModelBuilder::addModelAttribute(void* modelEntity, double dRed, double dGreen, double dBlue)
{
	bool suc = true;
	//rgb_color defColor(dRed, dGreen, dBlue);
	//api_rh_set_entity_rgb((ENTITY*)modelEntity, defColor);
	return suc;
}

double OccModelBuilder::tolerance()
{
    return Precision::Confusion();
}

void OccModelBuilder::HandlePrimitiveAnnotations()
{

}

void OccModelBuilder::HandleCopyAnnotations()
{

}

void OccModelBuilder::HandleSplitAnnotations()
{

}

std::string OccModelBuilder::CreateEdgeNameFromEntity(void* pSelEdge)
{
    std::string strPID;
    if (TheOCCApp.getCurrentModel()->findShapePID(pSelEdge, strPID))
    {

    }
    return strPID;
}

bool OccModelBuilder::createVertex(const double& xPos, const double& yPos, const double& zPos, const char* name, std::string& strPID, void*& entity)
{
    return true;
}

bool OccModelBuilder::createLine(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2, const char* name, std::string& strPID, void*& entity)
{
    return true;
}

bool OccModelBuilder::createPlane(const double& xPos1, const double& yPos1, const double& zPos1, const double& xPos2, const double& yPos2, const double& zPos2, const char* name, std::string& strPID, void*& entity)
{
    return true;
}

bool OccModelBuilder::createUnusedEntity(const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN
    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //create shape
        EdgeShapeBuilder shapeBuilder;
        if (!shapeBuilder.createLine(0,0,0,1,0,0))
        {
            COMMAND_END

            return false;
        }

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, shapeBuilder.getShape());
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }

    COMMAND_END
}

bool OccModelBuilder::createPolyline(double* wcsMatrix, std::vector<std::vector<double>>& points, std::vector<PolylineSegment>& segments, const LOGICAL& isClosed, const LOGICAL& isCovered, const CrossSection& crossSection, const char * name, std::string & strPID, void *& resEntity)
{
    try
    {
        COMMAND_BEGIN

        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        std::vector<std::vector<double>> globalPoints = points;

        std::vector<std::vector<double>>::const_iterator first;
        std::vector<std::vector<double>>::const_iterator last;

        //create shape
        BRepBuilderAPI_MakeWire wireBuilder;

        EdgeShapeBuilder shapeEdgeBuilder;

		TopTools_ListOfShape edges;

        for (int i = 0; i < segments.size(); ++i)
        {
            //get part points
            PolylineSegment& segment = segments.at(i);
            first = globalPoints.begin() + segment.StartIndex;
            last = globalPoints.begin() + segment.StartIndex + segment.NoOfPoints;
            std::vector<std::vector<double>> segPnts(first, last);

            switch (segment.Type)
            {
            case Line:
            {
                if (!shapeEdgeBuilder.createPolyLine(segPnts))
                {
                    COMMAND_END
                    return false;
                }
                else
                {
					TopoDS_Shape curWire = shapeEdgeBuilder.getShape();
                    wireBuilder.Add(TopoDS::Wire(curWire));
					TopExp_Explorer exp(curWire, TopAbs_EDGE);
					std::vector<TopoDS_Shape> edgelist;
					for (; exp.More(); exp.Next())
					{
						edgelist.push_back(exp.Value());
					}
					for (int i = 0; i < edgelist.size(); i++)
					{
						edges.Append(edgelist[i]);
					}
                }
                break;
            }
            case Spline:
            {
                //if (!shapeEdgeBuilder.createMultiTypeSpline(segPnts,2))
                 if (!shapeEdgeBuilder.createBSpline(segPnts))
                {
                    COMMAND_END
                    return false;
                }
                else
                {
                    wireBuilder.Add(TopoDS::Edge(shapeEdgeBuilder.getShape()));
					edges.Append(shapeEdgeBuilder.getShape());
                }
                break;
            }
            case ThreePointArc:
            {
                if (!shapeEdgeBuilder.createThreePointArc(segPnts))
                {
                    COMMAND_END
						return false;
                }
                else
                {
                    wireBuilder.Add(TopoDS::Edge(shapeEdgeBuilder.getShape()));
					edges.Append(shapeEdgeBuilder.getShape());
                }

                break;
            }
            case AngularArc:
            {
                double xAxis, yAxis, zAxis;
                ShapeUtilities::getAxis(segment.CenterAxis, xAxis, yAxis, zAxis);

                std::vector<double> newEndPnt;
                if (!shapeEdgeBuilder.createAngularArc(segPnts, segment.ArcAngle, xAxis, yAxis, zAxis, newEndPnt))
                {
                    COMMAND_END
						return false;
                }
                else
                {
                    PolylineSegment& segment = segments.at(i);
                    first = globalPoints.begin() + segment.StartIndex;
                    last = globalPoints.begin() + segment.StartIndex + segment.NoOfPoints;
                    std::vector<std::vector<double>> segPnts(first, last);

                    globalPoints[segment.StartIndex + +segment.NoOfPoints-1][0] = newEndPnt[0];
                    globalPoints[segment.StartIndex + +segment.NoOfPoints-1][1] = newEndPnt[1];
                    globalPoints[segment.StartIndex + +segment.NoOfPoints-1][2] = newEndPnt[2];
                    wireBuilder.Add(TopoDS::Edge(shapeEdgeBuilder.getShape()));
					edges.Append(shapeEdgeBuilder.getShape());
                }
                break;
            }
            default:
                break;
            }
        }

        //如果点没有封闭，帮他封闭
        if ((isClosed == LOGICAL::True || isCovered == LOGICAL::True) && globalPoints.size() > 2)
        {
			gp_Pnt endPnt(globalPoints.at(0).at(0), globalPoints.at(0).at(1), globalPoints.at(0).at(2));
            gp_Pnt startPnt(globalPoints.at(globalPoints.size()-1).at(0), globalPoints.at(globalPoints.size() - 1).at(1), globalPoints.at(globalPoints.size() - 1).at(2));
            if (!startPnt.IsEqual(endPnt, tolerance()))
            {
                std::vector<std::vector<double>> segPnts;
                segPnts.push_back(globalPoints.at(globalPoints.size() - 1));
                segPnts.push_back(globalPoints.at(0));

                if (!shapeEdgeBuilder.createPolyLine(segPnts))
                {
                    COMMAND_END
                        return false;
                }

                wireBuilder.Add(TopoDS::Wire(shapeEdgeBuilder.getShape()));
				TopExp_Explorer exp(shapeEdgeBuilder.getShape(), TopAbs_EDGE);
				for (; exp.More(); exp.Next())
					edges.Append(exp.Value());
            }
        }

        wireBuilder.Build();
        if (!wireBuilder.IsDone())
        {
            COMMAND_END
				return false;
        }

        TopoDS_Shape targetShape = wireBuilder.Shape();

		//check line
		bool nocover = false;
		if (segments.size() == 1 && segments[0].Type() == Line)
			nocover = true;

        if (isCovered == LOGICAL::True && !nocover)//wire
        {
			FaceOperationBuilder faceop;
			if (!faceop.faceCover(edges))
			{
				COMMAND_END
					return false;
			}
			targetShape = faceop.getShape();
			if (targetShape.IsNull())
			{
				COMMAND_END
					return false;
			}
        }

		if (!targetShape.IsNull() && crossSection.SectionType != SECTIONTYPE::NONE)
		{
			//获取section局部坐标系
			cmtPoint startpt(points[segments[0].StartIndex][0],
				points[segments[0].StartIndex][1],
				points[segments[0].StartIndex][2]);
			cmtVector tangent;
			TopoDS_Shape startedge = edges.First();

			double start, end;
			Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(startedge), start, end);
			double t = start;
			gp_Pnt pnt;
			gp_Vec vec;
			curve->D1(t, pnt, vec);
			gp_Dir zaxis(vec), xaxis, yaxis;

			if (crossSection.Orientation == XDIRECTION)
			{
				if (zaxis.IsEqual(gp_Dir(1, 0, 0), Precision::Angular()))
				{
					xaxis = gp_Dir(1 / 2, -0.5 * sqrt(3), 0);
				}
				else
				{
					xaxis = zaxis.Crossed(gp_Dir(1, 0, 0));
				}
			}
			if (crossSection.Orientation == YDIRECTION)
			{
				if(zaxis.IsEqual(gp_Dir(0, 1, 0), Precision::Angular()))
				{
					xaxis = gp_Dir(1 / 2, -0.5 * sqrt(3), 0);
				}
				else
				{
					xaxis = zaxis.Crossed(gp_Dir(0, 1, 0));
				}
			}
			if (crossSection.Orientation == ZDIRECTION || crossSection.Orientation == AUTO)
			{
				if (zaxis.IsEqual(gp_Dir(0, 0, 1), Precision::Angular()))
				{
					xaxis = gp_Dir(-1 / 2, -0.5 * sqrt(3), 0);
				}
				else
				{
					xaxis = zaxis.Crossed(gp_Dir(0, 0, 1));
				}
			}

			//求出y轴
			yaxis = zaxis.Crossed(xaxis);
			double* localwcs = new double[16];
			localwcs[0] = xaxis.X();
			localwcs[1] = xaxis.Y();
			localwcs[2] = xaxis.Z();
			localwcs[3] = 0;
			localwcs[4] = yaxis.X();
			localwcs[5] = yaxis.Y();
			localwcs[6] = yaxis.Z();
			localwcs[7] = 0;
			localwcs[8] = zaxis.X();
			localwcs[9] = zaxis.Y();
			localwcs[10] = zaxis.Z();
			localwcs[11] = 0;
			localwcs[12] = startpt.X();
			localwcs[13] = startpt.Y();
			localwcs[14] = startpt.Z();
			localwcs[15] = 1;

			TopoDS_Shape profile;
			if (crossSection.SectionType == SECTIONTYPE::LINE && crossSection.width != 0)
			{
				EdgeShapeBuilder esbuilder;
				if (!esbuilder.createLine(-crossSection.width / 2, 0, 0, crossSection.width / 2, 0, 0))
				{
					COMMAND_END
						return false;
				}
				profile = esbuilder.getShape();
			}
			if (crossSection.SectionType == SECTIONTYPE::CIRCULAR && crossSection.width != 0)
			{
				FaceShapeBuilder fsbuilder;
				if (crossSection.segnum > 2)
				{
					if (!fsbuilder.createRegularPolygon(0, 0, 0, crossSection.width, 0, 0, crossSection.segnum, 0, 0, 1))
					{
						COMMAND_END
							return false;
					}
				}
				else
				{
					if (!fsbuilder.createCircleFace(0, 0, 0, 0, 0, 1, crossSection.width))
					{
						COMMAND_END
							return false;
					}
				}
				profile = fsbuilder.getShape();

				if (crossSection.bendType == BENDTYPE::Curved)
				{

				}
			}
			if (crossSection.SectionType == SECTIONTYPE::RECTANGLE && crossSection.width != 0)
			{
				void* ent = nullptr;
				double width = crossSection.width;
				double height = crossSection.Height;
				if (height == 0)
				{
					EdgeShapeBuilder esbuilder;
					if(!esbuilder.createLine(-crossSection.width / 2, 0, 0, crossSection.width / 2, 0, 0))
					{
						COMMAND_END
							return false;
					}
					profile = esbuilder.getShape();
				}
				else
				{
					FaceShapeBuilder fsbuilder;
					if(!fsbuilder.createRectFace(0, 0, 0, 0, 0, 1, width, height))
					{
						COMMAND_END
							return false;
					}
					profile = fsbuilder.getShape();
				}
			}
			if (crossSection.SectionType == SECTIONTYPE::ISOSECLESTRAPEZOID &&
				(crossSection.width != 0 || crossSection.TopWidth != 0) && crossSection.Height != 0)
			{
				void* ent = nullptr;
				double width = crossSection.width;
				double topwidth = crossSection.TopWidth;
				double height = crossSection.Height;

				if (!createIsoTrapezoid(wcsMatrix, topwidth, width, height, LOGICAL::True, ent))
				{
					COMMAND_END
						return false;
				}
				profile = ((TopoDS_HShape*)ent)->Shape();
				ent = NULL;
				delete ent;
			}

			//transform
			profile = ShapeUtilities::shapeTransformFromLocalToGlobal(localwcs, profile);

			//sweep
			BRepBuilderAPI_TransitionMode mode = BRepBuilderAPI_RightCorner;
			SweepShapeBuilder ssbuilder;
			if (!ssbuilder.sweepPipeShellOperation(profile, targetShape, 0, mode))
			{
				COMMAND_END
					return false;
			}
			targetShape = ssbuilder.getShape();
			if (targetShape.IsNull())
			{
				COMMAND_END
					return false;
			}
			if (targetShape.ShapeType() == TopAbs_FACE)
			{
				ShapeUtilities::shapeCover(targetShape, targetShape);
			}
		}

        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

			return true;
    }
    catch (...)
    {
        return false;
    }
}

bool OccModelBuilder::createIsoTrapezoid(double* wcsMatrix, double topwidth, double width, double height,
	LOGICAL iscovered, void*& ent)
{
	try
	{
		TopoDS_Shape finalshape;
		gp_Pnt pt1(-topwidth / 2, height / 2, 0),
			pt2(topwidth / 2, height / 2, 0),
			pt3(width / 2, -height / 2, 0),
			pt4(-width / 2, -height / 2, 0);
		BRep_Builder B;
		TopoDS_Wire closewire;
		B.MakeWire(closewire);

		EdgeShapeBuilder tsbuilder;
		if (!pt1.IsEqual(pt2, tolerance()))
		{
			TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pt1, pt2);
			B.Add(closewire, edge);
		}
		if (!pt2.IsEqual(pt3, tolerance()))
		{
			TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pt2, pt3);
			B.Add(closewire, edge);
		}
		if (!pt3.IsEqual(pt4, tolerance()))
		{
			TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pt3, pt4);
			B.Add(closewire, edge);
		}
		if (!pt4.IsEqual(pt1, tolerance()))
		{
			TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(pt4, pt1);
			B.Add(closewire, edge);
		}

		if (iscovered == LOGICAL::True)
		{
			finalshape = BRepBuilderAPI_MakeFace(closewire);
		}
		else
		{
			finalshape = closewire;
		}

		ent = new TopoDS_HShape(finalshape);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createHyperbola(double* wcsMatrix, std::vector<std::vector<double>>& points,
    const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
{
    return false;
}

bool OccModelBuilder::createParabola(double* wcsMatrix, std::vector<std::vector<double>>& points,
    const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
{
    return false;
}

bool OccModelBuilder::createParaboloid(double* wcsMatrix, cmtPoint point, double depth, double radius, int axis,
	char* name, std::string& newEntry, void*& newShapeEntry)
{
	try
	{
		TDocStd_Document* curdoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curdoc->GetData();
		TDF_Label rootlabel = data->Root();

		double xaxis, yaxis, zaxis;
		ShapeUtilities::getAxis((DATUMAXISTYPE)axis, xaxis, yaxis, zaxis, depth);

		std::vector<double> centerp;
		centerp.push_back(point.x());
		centerp.push_back(point.y());
		centerp.push_back(point.z());

		double focus = radius * radius / (4 * depth);
		FaceShapeBuilder facebuilder;
		if (!facebuilder.createParabolaFace(centerp, depth, radius, xaxis, yaxis, zaxis))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape targetshape = facebuilder.getShape();
		targetshape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetshape);
		ShapeUtilities::shapeCover(targetshape, targetshape);

		TDF_Label targetlabel = LabelUtilities::createNewChild(rootlabel);
		TCollection_AsciiString anentry;
		TDF_Tool::Entry(targetlabel, anentry);
		newEntry = anentry.ToCString();

		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetlabel);
		adapter.allocateLabelWithTopShape(targetlabel, targetshape);
		TDataStd_AsciiString::Set(targetlabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(newEntry, newShapeEntry);

		COMMAND_END
			return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool OccModelBuilder::createParabolaFace(double* wcsMatrix, std::vector<double> points, double tstart, double tend, double fdistance,
	DATUMAXISTYPE axis, char* name, std::string& strPID, void*& resEntity)
{
	try
	{
		TDocStd_Document* curdoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curdoc->GetData();
		TDF_Label rootlabel = data->Root();

		double xaxis, yaxis, zaxis;
		ShapeUtilities::getAxis(axis, xaxis, yaxis, zaxis, depth);

		FaceShapeBuilder fsbuilder;
		if (!fsbuilder.createParabolaFace(points, fdistance, tstart, tend, xaxis, yaxis, zaxis))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape targetshape = fsbuilder.getShape();
		targetshape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetshape);

		TDF_Label targetlabel = LabelUtilities::createNewChild(rootlabel);
		TCollection_AsciiString anentry;
		TDF_Tool::Entry(targetlabel, anentry);
		strPID = anentry.ToCString();

		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetlabel);
		adapter.allocateLabelWithTopShape(targetlabel, targetshape);
		TDataStd_AsciiString::Set(targetlabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END
			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createHyperbola(double* wcsMatrix, std::vector<double>& points, const double& tStart, const double& tEnd,
    const double& majorR, const double& minorR, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
{
    try
    {
        COMMAND_BEGIN

        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        EdgeShapeBuilder edgeBuilder;
        if (!edgeBuilder.createHyperbola(points, majorR, minorR, tStart, tEnd, xAxis, yAxis, zAxis))
        {
            COMMAND_END

                return false;
        }
        TopoDS_Shape targetShape = edgeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool OccModelBuilder::createParabola(double* wcsMatrix, std::vector<double>& points, const double& tStart, const double& tEnd,
    const double& focalDistance, const DATUMAXISTYPE& axis, const char * name, std::string & strPID, void *& resEntity)
{
    try
    {
        COMMAND_BEGIN

        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        EdgeShapeBuilder edgeBuilder;
        if (!edgeBuilder.createParabola(points, focalDistance, tStart, tEnd, xAxis, yAxis, zAxis))
        {
            COMMAND_END

                return false;
        }
        TopoDS_Shape targetShape = edgeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool OccModelBuilder::createHelix(double* wcsMatrix, 
	const double& centerX, const double& centerY, const double& centerZ,
	const double& lowerRadius, const double& upperRadius, const double& height, const double& turns, 
	const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
	try
	{
        COMMAND_BEGIN

		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		gp_Pnt fakept(-9999.01, 0, 0);
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);
		
		//create shape
		EdgeShapeBuilder edgeBuilder;
		if (!edgeBuilder.createHelix(fakept,
			centerX, centerY, centerZ, 
			lowerRadius, upperRadius, height, turns, rightHanded, 
			xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}

        TopoDS_Shape targetShape = edgeBuilder.getShape();
        ShapeUtilities::shapeCover(targetShape, targetShape);

        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createHelix(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, /*const double& lowerRadius, */const double& upperRadius, const double& height, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
{
	try
	{
		COMMAND_BEGIN

			TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		//create shape
		EdgeShapeBuilder edgeBuilder;
		if (!edgeBuilder.createHelix(
			centerX, centerY, centerZ,
			startX, startY, startZ, upperRadius, height, turns, rightHanded,
			xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = edgeBuilder.getShape();
		ShapeUtilities::shapeCover(targetShape, targetShape);

		targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& minRadius, const double& maxRadius, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
	try
	{
        COMMAND_BEGIN

		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		//create shape
		EdgeShapeBuilder edgeBuilder;
		if (!edgeBuilder.createSpiral(centerX, centerY, centerZ, minRadius, maxRadius, turns, rightHanded, xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}

        TopoDS_Shape targetShape = edgeBuilder.getShape();
        if (!ShapeUtilities::shapeFilter(targetShape, TopAbs_WIRE, targetShape))//remove compounds level above wire
        {
            COMMAND_END
                return false;
        }
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createSpiral(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& startX, const double& startY, const double& startZ, /*const double& minRadius, */const double& maxRadius, const double& turns, const double& rightHanded, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& resEntity)
{
	try
	{
		COMMAND_BEGIN

			TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		//create shape
		EdgeShapeBuilder edgeBuilder;
		if (!edgeBuilder.createSpiral(centerX, centerY, centerZ, startX, startY, startZ, maxRadius, turns, rightHanded, xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = edgeBuilder.getShape();
		if (!ShapeUtilities::shapeFilter(targetShape, TopAbs_WIRE, targetShape))//remove compounds level above wire
		{
			COMMAND_END
				return false;
		}
		targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createEquationCurve(double* wcsMatrix, const char* xtFunction, const char* ytFunction, const char* ztFunction, const double& startT, const double& strEndT, const char* name, std::string& strPID, void*& resEntity)
{
	try
	{
		COMMAND_BEGIN

			TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//create shape
		EdgeShapeBuilder edgeBuilder;
		if (!edgeBuilder.createFuncCurve(std::string(xtFunction), std::string(ytFunction), std::string(ztFunction), "x", startT, strEndT))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = edgeBuilder.getShape();
		if (!ShapeUtilities::shapeCover(targetShape, targetShape))//remove compounds level above wire
		{
			COMMAND_END
				return false;
		}
		targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

#include <BRep_Tool.hxx>
//need to check
bool OccModelBuilder::createRectangle(double* wcsMatrix, const double& startX, const double& startY, const double& startZ, const double& width, const double& height, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
		double finalH = height;
		double finalW = width;
		if (axis == X)
		{
			double swap = finalH;
			finalH = -finalW;
			finalW = swap;
		}
        FaceShapeBuilder shapeBuilder;
        if (!shapeBuilder.createRectFace(startX, startY, startZ, xAxis, yAxis, zAxis, finalW, finalH))
        {
            COMMAND_END

            return false;
        }

        TopoDS_Shape targetShape = shapeBuilder.getShape();
        if (isCovered == LOGICAL::False)//wire
        {
            if (targetShape.NbChildren() != 1)
            {
                COMMAND_END

                return false;
            }

            for (TopExp_Explorer anExp(targetShape, TopAbs_WIRE); anExp.More(); anExp.Next())
            {
                targetShape = anExp.Current();
                break;
            }
        }
        else
        {
            ShapeUtilities::shapeCover(targetShape, targetShape);
        }
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

       TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }

}

bool OccModelBuilder::createCircle(double* wcsMatrix, const double& centerX, const double& centerY, 
	const double& centerZ, const double& radius, const DATUMAXISTYPE& axis, const LOGICAL& isCovered,
	int segment, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
		double xAxis, yAxis, zAxis, startx, starty, startz;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);
		startx = startx * radius + centerX;
		starty = starty * radius + centerY;
		startz = startz * radius + centerZ;

        //create shape
        FaceShapeBuilder shapeBuilder;
		if (segment > 2)
		{
			if (!shapeBuilder.createRegularPolygon(centerX, centerY, centerZ, startx, starty, startz, segment, xAxis, yAxis, zAxis))
			{
				COMMAND_END

					return false;
			}
		}
		else
		{
			if (!shapeBuilder.createCircleFace(centerX, centerY, centerZ, xAxis, yAxis, zAxis, radius))
			{
				COMMAND_END

					return false;
			}
		}

        TopoDS_Shape targetShape = shapeBuilder.getShape();
        if (isCovered == LOGICAL::False)//wire
        {
            if (targetShape.NbChildren() != 1)
            {
                COMMAND_END

                return false;
            }

            for (TopExp_Explorer anExp(targetShape, TopAbs_WIRE); anExp.More(); anExp.Next())
            {
                targetShape = anExp.Current();
                break;
            }
        }
        else
        {
            ShapeUtilities::shapeCover(targetShape, targetShape);
        }

        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createSector(double* wcsMatrix, 
	const double& centerX, const double& centerY, const double& centerZ, 
	const double& radius, const double& startAng, const double& endAng,
	const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& entity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		//create shape
		FaceShapeBuilder shapeBuilder;
		if (!shapeBuilder.createSectorFace(
			centerX, centerY, centerZ,
			xAxis, yAxis, zAxis, radius, startAng, endAng))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = shapeBuilder.getShape();
		if (isCovered == LOGICAL::False)//wire
		{
			if (targetShape.NbChildren() != 1)
			{
				COMMAND_END

					return false;
			}

			for (TopExp_Explorer anExp(targetShape, TopAbs_WIRE); anExp.More(); anExp.Next())
			{
				targetShape = anExp.Current();
				break;
			}
		}
		else
		{
            ShapeUtilities::shapeCover(targetShape, targetShape);
        }
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::createRegularPolygon(double* wcsMatrix,
	const double& centerX, const double& centerY, const double& centerZ, 
	const double& startX, const double& startY, const double& startZ, const int& numSides,
	const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& entity)
{
	COMMAND_BEGIN

	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		//create shape
		FaceShapeBuilder shapeBuilder;
		if (!shapeBuilder.createRegularPolygon(
			centerX, centerY, centerZ,
			startX, startY, startZ,
			numSides, xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = shapeBuilder.getShape();
		if (isCovered == LOGICAL::False)//wire
		{
			if (targetShape.NbChildren() != 1)
			{
				COMMAND_END

					return false;
			}

			for (TopExp_Explorer anExp(targetShape, TopAbs_WIRE); anExp.More(); anExp.Next())
			{
				targetShape = anExp.Current();
				break;
			}
		}
		else
		{
            ShapeUtilities::shapeCover(targetShape, targetShape);
        }
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

		return true;
	}
	catch (...)
	{
		COMMAND_END

		return false;
	}
}

bool OccModelBuilder::createEllipse(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const LOGICAL& isCovered, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        FaceShapeBuilder shapeBuilder;
        if (!shapeBuilder.createEllipseFace(centerX, centerY, centerZ, xAxis, yAxis, zAxis,
			abs(majRadius), abs(majRadius*ratio)))
        {
            COMMAND_END

            return false;
        }

        TopoDS_Shape targetShape = shapeBuilder.getShape();
        if (isCovered == LOGICAL::False)//wire
        {
            if (targetShape.NbChildren() != 1)
            {
                COMMAND_END

                return false;
            }

            for (TopExp_Explorer anExp(targetShape, TopAbs_WIRE); anExp.More(); anExp.Next())
            {
                targetShape = anExp.Current();
                break;
            }
        }
        else
        {
            ShapeUtilities::shapeCover(targetShape, targetShape);
        }
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //create shape
        FaceShapeBuilder shapeBuilder;
        if (!shapeBuilder.createCylinderFace(xBottomCenter, yBottomCenter, zBottomCenter, xAxis, yAxis, zAxis, raidus, height))
        {
            COMMAND_END

            return false;
        }

        TopoDS_Shape targetShape = shapeBuilder.getShape();
        ShapeUtilities::shapeCover(targetShape, targetShape);

        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createCylinderFace(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const DATUMAXISTYPE& axis, const double& raidus, const double& height, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        FaceShapeBuilder shapeBuilder;
        if (!shapeBuilder.createCylinderFace(xBottomCenter, yBottomCenter, zBottomCenter, xAxis, yAxis, zAxis, raidus, height))
        {
            COMMAND_END

                return false;
        }

        TopoDS_Shape targetShape = shapeBuilder.getShape();
        ShapeUtilities::shapeCover(targetShape, targetShape);
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::createConeFace(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerRadius, const double& upperRadius, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        FaceShapeBuilder shapeBuilder;
        if (!shapeBuilder.createConeFace(botCenterX, botCenterY, botCenterZ, lowerRadius, upperRadius, height, xAxis, yAxis, zAxis))
        {
            COMMAND_END

            return false;
        }

        TopoDS_Shape targetShape = shapeBuilder.getShape();
        ShapeUtilities::shapeCover(targetShape, targetShape);
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createEquationSurface(double* wcsMatrix, const char* xuvFunction, const char* yuvFunction, const char* zuvFunction, const double& startU, const double& strEndU, const double& startV, const double& strEndV, const char* name, std::string& strPID, void*& resEntity)
{
	try
	{
		COMMAND_BEGIN

			TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//create shape
		FaceShapeBuilder faceBuilder;
		if (!faceBuilder.createFuncSurface(std::string(xuvFunction), std::string(yuvFunction), std::string(zuvFunction), "x", "y",startU, strEndU, startV, strEndV))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = faceBuilder.getShape();
		if (!ShapeUtilities::shapeCover(targetShape, targetShape))//remove compounds level above wire
		{
			COMMAND_END
				return false;
		}
		targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createNURBSSurface(double* wcsMatrix, NURBSTYPE type, int numPtsU, int numPtsV,
	std::vector<std::vector<double>>& points, char* name, std::string& newEntry, void*& newShapeEntry)
{
	try
	{
		int ptcount = points.size();
		if (numPtsU * numPtsV != ptcount)
			return false;
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		TColgp_Array2OfPnt polearray(1, numPtsU, 1, numPtsV);
		TColStd_Array2OfReal weights(1, numPtsU, 1, numPtsV);

		for (int i = 0; i < numPtsU; i++)
		{
			for (int j = 0; j < numPtsV; j++)
			{
				std::vector<double> tmp = points[numPtsV * i + j];
				gp_Pnt pnt(tmp[0], tmp[1], tmp[2]);
				polearray.SetValue(i + 1, j + 1, pnt);
				weights.SetValue(i + 1, j + 1, tmp[3]);
			}
		}

		FaceShapeBuilder shapebuilder;
		if (type == InterpolatedPoints)
		{
			if (!shapebuilder.createInterpolateBSplineSurface(polearray))
			{
				COMMAND_END
					return false;
			}
		}
		else
		{
			int degreeu = numPtsU - 1 > 3 ? 3 : numPtsU - 1;
			int knotsucount = numPtsU + degreeu + 1;
			int degreev = numPtsV - 1 > 3 ? 3 : numPtsV - 1;
			int knotsvcount = numPtsV + degreev + 1;

			std::vector<double> uknots, vknots;
			int i = 0;
			for (; i < degreeu; i++)
			{
				uknots.push_back(0.0);
			}
			for (; i < knotsucount - degreeu; i++)
			{
				uknots.push_back(i - degreeu);
			}
			for (; i < knotsucount; i++)
			{
				uknots.push_back(knotsucount - 2 * degreeu - 1);
			}
			i = 0;
			for (; i < degreeu; i++)
			{
				vknots.push_back(0.0);
			}
			for (; i < knotsvcount - degreev; i++)
			{
				vknots.push_back(i - degreev);
			}
			for (; i < knotsvcount; i++)
			{
				vknots.push_back(knotsvcount - 2 * degreev - 1);
			}

			//检查knots,计算节点数
			TColStd_Array1OfReal uknotsarray, vknotsarray;
			TColStd_Array1OfInteger umultsarray, vmultsarray;

			OCCBasicTools::ComputeNURBSKnotsMults(uknots, uknotsarray, umultsarray);
			OCCBasicTools::ComputeNURBSKnotsMults(vknots, vknotsarray, vmultsarray);

			if (!shapebuilder.createNurbsSurface(polearray, weights, uknotsarray, vknotsarray, umultsarray, vmultsarray,
				degreeu, degreev))
			{
				COMMAND_END
					return false;
			}
		}

		TopoDS_Shape targetshape = shapebuilder.getShape();
		ShapeUtilities::shapeCover(targetshape, targetshape);
		targetshape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetshape);
		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		newEntry = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetshape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(newEntry, newShapeEntry);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END
			return false;
	}
}

bool OccModelBuilder::createCuboid(double* wcsMatrix, const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //create shape
        SolidShapeBuilder shapeBuilder;
        if (!shapeBuilder.createCuboid(xStart, yStart, zStart, xSize, ySize, zSize))
        {
            COMMAND_END

            return false;
        }
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createCylinder(double* wcsMatrix, const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& raidus, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        SolidShapeBuilder shapeBuilder;
        if (!shapeBuilder.createCylinder(xBottomCenter, yBottomCenter, zBottomCenter, xAxis, yAxis, zAxis, raidus, height))
        {
            COMMAND_END

            return false;
        }
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createSphere(double* wcsMatrix, const double& xCenter, const double& yCenter, const double& zCenter, 
    const double& raidus, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

		//create shape
		SolidShapeBuilder shapeBuilder;
		if (!shapeBuilder.createSphere(xCenter, yCenter, zCenter, raidus))
		{
            COMMAND_END

			return false;
		}
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createCone(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, 
    const double& lowerRadius, const double& upperRadius, const double& height, 
    const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        SolidShapeBuilder shapeBuilder;
        if (!shapeBuilder.createCone(botCenterX, botCenterY, botCenterZ, 
			abs(lowerRadius), abs(upperRadius), height, xAxis, yAxis, zAxis))
        {
            COMMAND_END

            return false;
        }
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createRegularPolyhedron(double* wcsMatrix, 
	const double& botCenterX, const double& botCenterY, const double& botCenterZ, 
	const double& startX, const double& startY, const double& startZ, 
	const double& height, const int& numSides, const DATUMAXISTYPE& axis, 
	const char* name, std::string& strPID, void*& entity)
{
	COMMAND_BEGIN

	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);
	
		//create shape
		SolidShapeBuilder shapeBuilder;
		if (!shapeBuilder.createRegularPolyhedron(
			botCenterX, botCenterY, botCenterZ, 
			startX, startY, startZ,
			height, numSides, xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::createTorus(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, 
    const double& minorRadius, const double& majorRadius, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double xAxis, yAxis, zAxis;
        ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

        //create shape
        SolidShapeBuilder shapeBuilder;
        if (!shapeBuilder.createTorus(centerX, centerY, centerZ, minorRadius, majorRadius, xAxis, yAxis, zAxis))
        {
            COMMAND_END

            return false;
        }
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //create child label
        LabelShapeAdapter adapter;
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::createFlare(double* wcsMatrix, const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& lowerWidth, const double& lowerDepth, const double& upperWidth, const double& upperDepth, const double& height, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
	COMMAND_BEGIN

	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		//create shape
		SolidShapeBuilder shapeBuilder;
		if (!shapeBuilder.createFlare(botCenterX, botCenterY, botCenterZ, 
			abs(lowerWidth), abs(lowerDepth), 
			abs(upperWidth), abs(upperDepth), height,
			xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter::allocateLabelWithTopShape(targetLabel, targetShape, name, 1);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::createSpindle(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ,
	double majradius, double ratio, DATUMAXISTYPE axis, char* name, std::string& strPID, void* entity)
{
	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		double xaxis, yaxis, zaxis;
		ShapeUtilities::getAxis(axis, xaxis, yaxis, zaxis);

		SolidShapeBuilder sdbuilder;
		if (!sdbuilder.createSpindle(centerX, centerY, centerZ, abs(majradius), abs(ratio), xaxis, yaxis, zaxis))
		{
			COMMAND_END
				return false;
		}

		TopoDS_Shape targetShape = sdbuilder.getShape();
		targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter::allocateLabelWithTopShape(targetLabel, targetShape, name, 1);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::createEllipsoid(double* wcsMatrix, const double& centerX, const double& centerY, const double& centerZ, const double& majRadius, const double& ratio, const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
	COMMAND_BEGIN

	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);
	
		//create shape
		SolidShapeBuilder shapeBuilder;
		if (!shapeBuilder.createEllipsoid(centerX, centerY, centerZ,
			abs(majRadius), abs(ratio),
			xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter::allocateLabelWithTopShape(targetLabel, targetShape, name, 1);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

		return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::createHelixSolid(double* wcsMatrix, 
	const double& centerX, const double& centerY, const double& centerZ, 
	const double& profileRadius, const double& lowerRadius, const double& upperRadius,
	const double& height, const double& turns, const double& rightHanded,
	const DATUMAXISTYPE& axis, const char* name, std::string& strPID, void*& entity)
{
	COMMAND_BEGIN

	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//parameters transformation
		double xAxis, yAxis, zAxis;
		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		//create shape
		SolidShapeBuilder shapeBuilder;
		;
		if (!shapeBuilder.createHelixSolid(
			centerX, centerY, centerZ,
			profileRadius, lowerRadius, upperRadius, height, turns, rightHanded, 
			xAxis, yAxis, zAxis))
		{
			COMMAND_END

				return false;
		}
        TopoDS_Shape targetShape = shapeBuilder.getShape();
        targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter::allocateLabelWithTopShape(targetLabel, targetShape, name, 1);
		//         TDataStd_Name::Set(targetLabel, name);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		COMMAND_END

		return true;
	}
	catch (...)
	{
		COMMAND_END

		return false;
	}
}

// bool OccModelBuilder::createExtrude(const std::string& sourceEntry, const double& xVec, const double& yVec, const double& zVec, const char* name, std::string& strPID, void*& entity)
// {
//     try
//     {
//         TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
//         Handle(TDF_Data) data = curDoc->GetData();
//         TDF_Label rootLabel = data->Root();
// 
//         // start
//         // get solid shape
//         TDF_Label sourceLabel;
//         TDF_Tool::Label(data, TCollection_AsciiString(sourceEntry.c_str()), sourceLabel, false);
//         if (sourceLabel.IsNull())
//         {
//             return false;
//         }
// 
//         //get main label shape
//         TopoDS_Shape sourceShape;
//         Handle(TNaming_NamedShape) sourceNamedShape;
//         if (sourceLabel.FindAttribute(TNaming_NamedShape::GetID(), sourceNamedShape))
//         {
//             sourceShape = sourceNamedShape->Get();
//             if (sourceShape.IsNull()||sourceShape.ShapeType()<TopAbs_FACE)
//             {
//                 return false;
//             }
//         }
//         else
//         {
//             return false;
//         }
// 
//         //start operation
//         PrimShapeBuilder shapeBuilder;
//         if (!shapeBuilder.extrudeOperation(sourceShape, xVec, yVec, zVec))
//         {
//             return false;
//         }
// 
//         TDF_Label oldLabel;
//         LabelUtilities::findNameLabel(rootLabel, name, oldLabel);
//         TDF_Label targetLabel = TDF_TagSource::NewChild(rootLabel);
//         LabelUtilities::copyLabelWithOnlyPropertyChild(sourceLabel, targetLabel);
// 
//         TopoDS_Shape newShape = shapeBuilder.getShape();
//         LabelShapeAdapter adapter;
//         adapter.setName(name);
//         adapter.clearModifiedShapes();
//         adapter.addModifiedShapes(newShape, targetLabel);
//         adapter.collectLabelShapes(sourceLabel, EnExtrude, shapeBuilder.getBuilder());
//         adapter.collectLabelShapesGeneratedByEdgeAndVertex(sourceShape, EnExtrude, shapeBuilder.getBuilder());
// 
//         //allocate
//         adapter.setCompoundLabel(targetLabel);
//         adapter.allocateLabelWithTopShape(targetLabel, newShape);
//         LabelShapeAdapter::replaceChildLabelWithCopiedShape(targetLabel, newShape);
// 
//         TDataStd_AsciiString::Set(targetLabel, "top_level");
//         TDataStd_Name::Set(targetLabel, name);
// 
//         // some infos for return;
//         LabelUtilities::recursiveRenameChildLabelWithSameName(targetLabel);
//         TCollection_AsciiString anEntry;
//         TColStd_ListOfInteger aTagList;
//         TDF_Tool::Entry(targetLabel, anEntry);
//         strPID = anEntry.ToCString();
//         if (!oldLabel.IsNull())
//         {
//             oldLabel.ForgetAllAttributes();
//         }
//         TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
//         entity = TheOCCApp.getCurrentModel()->findTopodsShapeFromMap(strPID);
// 
//         return true;
//     }
//     catch (...)
//     {
//         return false;
//     }
// }

bool OccModelBuilder::makeBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
{
    if (entryList.size() < 2)
    {
        return false;
    }

    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find entity PID; collect shape and information
        TopTools_ListOfShape args, tools, total;
        LabelShapeAdapter adapter;
        TransformShapeBuilder transformBuilder;
        for (std::list<void*>::const_iterator iter = entryList.begin(); iter != entryList.end(); ++iter)
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

                //copy shape to avoid influence of boolean operation upon origin shape
                if (!transformBuilder.shapeCopy(foundShape))
                {
                    return false;
                }
                adapter.labelDiscreteToMap(foundLabel);
                adapter.collectRelatedLabelAndShapes(foundShape, EnCopy, transformBuilder.getBuilder());
                foundShape = transformBuilder.getShape();

                if (iter == entryList.begin())
                {
                    args.Append(foundShape);
                }
                else
                {
                    tools.Append(foundShape);
                }

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

        BOPAlgo_Operation type;
        EnAdapatType adaptType;
        if (boolType == Substract|| boolType == Unite || boolType == Intersect)
        {
			bool regularboolean = true;

			TopTools_ListOfShape cellsargs, cellstools, cellstotall;
			TopTools_ListOfShape::Iterator iter(args);
			for (; iter.More(); iter.Next())
			{
				ShapeUtilities::GetAllNonCompoundShapes(iter.Value(), cellsargs);
				ShapeUtilities::GetAllNonCompoundShapes(iter.Value(), cellstotall);
			}
			iter.Init(tools);
			for (; iter.More(); iter.Next())
			{
				ShapeUtilities::GetAllNonCompoundShapes(iter.Value(), cellstools);
				ShapeUtilities::GetAllNonCompoundShapes(iter.Value(), cellstotall);
			}

			TopAbs_ShapeEnum lower, upper;
			ShapeUtilities::GetShapeTypeRange(cellstotall, lower, upper);
			if (lower >= TopAbs_SHELL && upper <= TopAbs_FACE)
				regularboolean = false;
			else
				regularboolean = true;

			if (boolType == Unite)
				regularboolean = false;

			if (!regularboolean)
			{
				switch (boolType)
				{
				case Substract:
				{
					type = BOPAlgo_CUT;
					adaptType = EnCellsBuilderCut;
					break;
				}
				case Unite:
				{
					type = BOPAlgo_FUSE;
					adaptType = EnCellsBuilderFuse;
					break;
				}
				case Intersect:
				{
					type = BOPAlgo_COMMON;
					adaptType = EnCellsBuilderCommon;
					break;
				}
				}

				if (boolType == Substract)
				{
					TopTools_ListOfShape substractShapes;
					TopTools_ListOfShape::Iterator argIter(cellsargs);
					for (; argIter.More(); argIter.Next())
					{
						TopoDS_Shape shapetosubstract = argIter.Value();
						TopTools_ListOfShape::Iterator toolIter(cellstools);
						for (; toolIter.More(); toolIter.Next())
						{
							if (!shapeBuilder.cellsBuilderOperation(shapetosubstract, toolIter.Value(), adaptType, false, tolerance))
							{
								COMMAND_END
									return false;
							}
							TopoDS_Shape substractshape = shapeBuilder.getShape();
							if (substractshape.IsNull())
							{
								COMMAND_END
									return false;
							}

							adapter.collectRelatedLabelAndShapes(shapetosubstract, adaptType, shapeBuilder.getBuilder());
							adapter.collectRelatedLabelAndShapes(toolIter.Value(), adaptType, shapeBuilder.getBuilder());

							shapetosubstract = substractshape;
						}

						substractShapes.Append(shapetosubstract);
					}

					if (substractShapes.Size() == 1)
						targetShape = substractShapes.First();
					else
					{
						if (!shapeBuilder.cellsBuilderOperation(substractShapes, EnCellsBuilderFuse, 0, true))
						{
							COMMAND_END
								return false;
						}

						targetShape = shapeBuilder.getShape();
						if (targetShape.IsNull())
						{
							COMMAND_END
								return false;
						}

						//找联系
						TopTools_ListOfShape::Iterator shapeIter(substractShapes);
						for (; shapeIter.More(); shapeIter.Next())
							adapter.collectRelatedLabelAndShapes(shapeIter.Value(), adaptType, shapeBuilder.getBuilder());

						ShapeUpgrade_UnifySameDomain sus(targetShape);
						targetShape = sus.Shape();
					}
				}
				else
				{
					//面相交
					if (boolType == Intersect && (upper <= TopAbs_FACE && upper >= TopAbs_SHELL
						&& lower <= TopAbs_FACE && lower >= TopAbs_SHELL))
					{
						if (!shapeBuilder.faceCommon(cellsargs, cellstools, &adapter, 0))
						{
							COMMAND_END
								return false;
						}
						targetShape = shapeBuilder.getShape();
					}
					else
					{
						//处理法向相反的面fuse时无法成功的情况
						if (upper <= TopAbs_FACE && upper >= TopAbs_SHELL && lower <= TopAbs_FACE && lower >= TopAbs_SHELL)
						{
							TopTools_ListOfShape newcellsargs, newcellstools;
							if (ShapeUtilities::ReverseFaceNormal4CellsBuild(cellsargs, cellstools, newcellsargs, newcellstools));
							{
								cellsargs = newcellsargs;
								cellstools = newcellstools;
							}
						}

						if (!shapeBuilder.cellsBuilderOperation(cellsargs, cellstools, adaptType, true, tolerance()))
						{
							COMMAND_END
								return false;
						}

						targetShape = shapeBuilder.getShape();

						if (targetShape.IsNull())
						{
							COMMAND_END
								return false;
						}
						TopTools_ListOfShape::Iterator shapeiter(total);
						for (; shapeiter.More(); shapeiter.Next())
							adapter.collectRelatedLabelAndShapes(shapeiter.Value(), adaptType, shapeBuilder.getBuilder());
						if (adaptType == EnCellsBuilderFuse)
						{
							FixShapeBuilder fsbuilder;
						}
					}
				}
			}
			else
			{
				//判断是否存在共面实体
				double fuzzyvalue = tolerance();
				if (OCCBasicTools::IsListShapesCoplane(total, fuzzyvalue))
					fuzzyvalue *= 10;
				if (boolType == Substract || boolType == Unite)
				{
					if (boolType == Substract)
					{
						type = BOPAlgo_CUT;
						adaptType = EnCut;
					}
					else
					{
						type = BOPAlgo_FUSE;
						adaptType = EnFuse;
					}

					if (args.Size() == 1 && tools.Size() == 1)
					{
						if (!shapeBuilder.booleanOperation(args.First(), tools.First(), type, fuzzyvalue))
						{
							COMMAND_END
								return false;
						}
						targetShape = shapeBuilder.getShape();
					}
					else
					{
						if (!shapeBuilder.booleanOperation(args, tools, type, fuzzyvalue))
						{
							COMMAND_END
								return false;
						}
					}
					targetShape = shapeBuilder.getShape();
					if (targetShape.IsNull())
					{
						COMMAND_END
							return false;
					}

					//找联系。。。
				}
				else if (boolType == Intersect)
				{
					type = BOPAlgo_COMMON;
					adaptType = EnCommon;

					TopTools_ListOfShape::Iterator shapeiter(total);
					targetShape = shapeiter.Value();
					shapeiter.Next();
					for (; shapeiter.More(); shapeiter.Next())
					{
						if (!shapeBuilder.booleanOperation(targetShape, shapeiter.Value(), type, fuzzyvalue))
							return false;
						TopoDS_Shape newshape = shapeBuilder.getShape();
						if (newshape.IsNull())
							return false;

						//找联系。。
					}
				}
			}
        }
		else if (boolType == Imprint)
		{
			if (args.Size() > 1)
			{
				COMMAND_END
					return false;
			}
			if (!shapeBuilder.shapeImprintOperation(args.First(), tools, &adapter, tolerance()))
			{
				COMMAND_END
					m_errorMessage += "imprint shape failed";
				return false;
			}
			targetShape = shapeBuilder.getShape();
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
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);


        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

        return false;
    }
}

bool OccModelBuilder::translate(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ, const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double globalVectorX, globalVectorY, globalVectorZ;
        ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, vectorX, vectorY, vectorZ, globalVectorX, globalVectorY, globalVectorZ);

        //find origin shape
		std::string origPID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(entry, origPID))
        {
            COMMAND_END
            return false;
        }
        TDF_Label origLabel;
        TDF_Tool::Label(data, origPID.c_str(), origLabel);
        if (origLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

        //start
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TopoDS_Shape targetShape;
        TransformShapeBuilder shapeBuilder;
        if (isAttach == LOGICAL::True)
        {
            TopoDS_Compound cmp;
            BRep_Builder B;
            B.MakeCompound(cmp);
//             B.Add(cmp, origShape);

            for (int i = 0; i < totalNumber; ++i)
            {
                if (!shapeBuilder.shapeTranslate(origShape, globalVectorX*i, globalVectorY*i, globalVectorZ*i, true))
                {
                    COMMAND_END

                        return false;
                }
                TDF_Label copiedChildLabel = LabelUtilities::createNewChild(targetLabel);

                TopoDS_Shape copiedShape = shapeBuilder.getShape();

                LabelUtilities::copyLabelWithFather(origLabel, copiedChildLabel);
                LabelShapeAdapter::replaceChildLabelWithCopiedShape(copiedChildLabel, copiedShape);
                B.Add(cmp, copiedShape);

                //remove "top_level"
                Handle(TDataStd_AsciiString) asciiString;
                if (copiedChildLabel.FindAttribute(TDataStd_AsciiString::GetID(), asciiString)&& asciiString->Get() == "top_level")
                {
                    copiedChildLabel.ForgetAttribute(TDataStd_AsciiString::GetID());
                }
            }
            targetShape = cmp;
            TNaming_Builder topNamingBuilder(targetLabel);
            topNamingBuilder.Generated(targetShape);
        }
        else
        {
            //transform
            if (!shapeBuilder.shapeTranslate(origShape, globalVectorX, globalVectorY, globalVectorZ, true))
            {
                targetLabel.ForgetAllAttributes();
                COMMAND_END
                    return false;
            }

            targetShape = shapeBuilder.getShape();

            LabelUtilities::copyLabelWithFather(origLabel, targetLabel);
            LabelShapeAdapter::replaceChildLabelWithCopiedShape(targetLabel, targetShape);
        }

        //get label entry
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

        return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::rotate(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const int& totalNumber, const LOGICAL& isAttach, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //parameters transformation
        double globalAxisPosX, globalAxisPosY, globalAxisPosZ, globalVectorX, globalVectorY, globalVectorZ;
        ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, axisPosX, axisPosY, axisPosZ, globalAxisPosX, globalAxisPosY, globalAxisPosZ);
        ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, axisVecX, axisVecY, axisVecZ, globalVectorX, globalVectorY, globalVectorZ);

        //find origin shape
        if (!TheOCCApp.getCurrentModel()->findShapePID(entry, strPID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label origLabel;
        TDF_Tool::Label(data, strPID.c_str(), origLabel);
        if (origLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

        //start
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TopoDS_Shape targetShape;
        TransformShapeBuilder shapeBuilder;
        if (isAttach == LOGICAL::True)
        {
            TopoDS_Compound cmp;
            BRep_Builder B;
            B.MakeCompound(cmp);
//             B.Add(cmp, origShape);

            for (int i = 0; i < totalNumber; ++i)
            {
                if (!shapeBuilder.shapeRotate(origShape, globalAxisPosX, globalAxisPosY, globalAxisPosZ,
                    globalVectorX, globalVectorY, globalVectorZ, angle*i, true))
                {
                    COMMAND_END

                        return false;
                }
                TDF_Label copiedChildLabel = LabelUtilities::createNewChild(targetLabel);

                TopoDS_Shape copiedShape = shapeBuilder.getShape();

                LabelUtilities::copyLabelWithFather(origLabel, copiedChildLabel);
                LabelShapeAdapter::replaceChildLabelWithCopiedShape(copiedChildLabel, copiedShape);
                B.Add(cmp, copiedShape);

                //remove "top_level"
                Handle(TDataStd_AsciiString) asciiString;
                if (copiedChildLabel.FindAttribute(TDataStd_AsciiString::GetID(), asciiString) && asciiString->Get() == "top_level")
                {
                    copiedChildLabel.ForgetAttribute(TDataStd_AsciiString::GetID());
                }
            }
            targetShape = cmp;
            TNaming_Builder topNamingBuilder(targetLabel);
            topNamingBuilder.Generated(targetShape);
        }
        else
        {
            //transform
            if (!shapeBuilder.shapeRotate(origShape, globalAxisPosX, globalAxisPosY, globalAxisPosZ,
                globalVectorX, globalVectorY, globalVectorZ, angle, true))
            {
                targetLabel.ForgetAllAttributes();
                COMMAND_END
                    return false;
            }

            targetShape = shapeBuilder.getShape();

            LabelUtilities::copyLabelWithFather(origLabel, targetLabel);
            LabelShapeAdapter::replaceChildLabelWithCopiedShape(targetLabel, targetShape);
        }

        //get label entry
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::mirror(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const LOGICAL& isCopy, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        double globalBasePosX, globalBasePosY, globalBasePosZ, globalNormalX, globalNormalY, globalNormalZ;
        ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, basePosX, basePosY, basePosZ, globalBasePosX, globalBasePosY, globalBasePosZ);
        ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, normalVecX, normalVecY, normalVecZ, globalNormalX, globalNormalY, globalNormalZ);

        //find origin shape
        if (!TheOCCApp.getCurrentModel()->findShapePID(entry, strPID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label origLabel;
        TDF_Tool::Label(data, strPID.c_str(), origLabel);
        if (origLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

        //start
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TopoDS_Shape targetShape;
        TransformShapeBuilder shapeBuilder;

		//transform
        if (!shapeBuilder.shapeMirror(origShape, globalBasePosX, globalBasePosY, globalBasePosZ,
            globalNormalX, globalNormalY, globalNormalZ, true))
        {
            COMMAND_END
                return false;
        }
        targetShape = shapeBuilder.getShape();

        LabelUtilities::copyLabelWithFather(origLabel, targetLabel);
        LabelShapeAdapter::replaceChildLabelWithCopiedShape(targetLabel, targetShape);

        //get label entry
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");


        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::scale(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& scaleX, const double& scaleY, const double& scaleZ, const LOGICAL& isCopy, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        double globalBasePosX, globalBasePosY, globalBasePosZ;
        ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, basePosX, basePosY, basePosZ, globalBasePosX, globalBasePosY, globalBasePosZ);

        //find origin shape
        if (!TheOCCApp.getCurrentModel()->findShapePID(entry, strPID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label origLabel;
        TDF_Tool::Label(data, strPID.c_str(), origLabel);
        if (origLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

        //start
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TopoDS_Shape targetShape;
        TransformShapeBuilder shapeBuilder;

		//transform
        if (!shapeBuilder.shapeScale(origShape, basePosX, basePosY, basePosZ,
            scaleX, scaleY, scaleZ, true))
        {
            COMMAND_END
                return false;
        }
        targetShape = shapeBuilder.getShape();

        LabelUtilities::copyLabelWithFather(origLabel, targetLabel);
        LabelShapeAdapter::replaceChildLabelWithCopiedShape(targetLabel, targetShape);

        //get label entry
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::sweepAlongVector(void* entry, double* wcsMatrix, const double& vectorX, const double& vectorY, const double& vectorZ, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find origin shape
        std::string instancePID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label origLabel;
        TDF_Tool::Label(data, instancePID.c_str(), origLabel);
        if (origLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);
        
        //tranform
        double globalXVec, globalYVec, globalZVec;
        ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, vectorX, vectorY, vectorZ, globalXVec, globalYVec, globalZVec);

        //start
        LabelShapeAdapter adapter;
        //preparation
        adapter.labelDiscreteToMap(origLabel);
        BRepBuilderAPI_TransitionMode mode;
        switch (draftType)
        {
        case Sweep_Extended:
        {
            mode = BRepBuilderAPI_Transformed;
            break;
        }
        case Sweep_Round:
        {
            mode = BRepBuilderAPI_RoundCorner;
            break;
        }
        case Sweep_Natural:
        {
            mode = BRepBuilderAPI_RightCorner;
            break;
        }
        default:
            break;
        }

        TopoDS_Shape targetShape, filterShape;
        SweepShapeBuilder shapeBuilder;
        TopAbs_ShapeEnum type = origShape.ShapeType();
        if (draftAngle == 0)
        {
            if (type <= TopAbs_SHELL)
            {
                if (!ShapeUtilities::shapeFilter(origShape, TopAbs_SHELL, filterShape))
                {
                    COMMAND_END
                        return false;
                }
            }
            else
            {
                filterShape = origShape;
            }

            if (!shapeBuilder.extrudeOperation(filterShape, globalXVec, globalYVec, globalZVec))
            {
                COMMAND_END
                    return false;
            }

            targetShape = shapeBuilder.getShape();
            if (targetShape.IsNull())
            {
                return false;
            }

            // 找联系
            adapter.collectRelatedLabelAndShapes(filterShape, EnExtrude, shapeBuilder.getBuilder());
        }
        else
        {
            if (type <= TopAbs_SHELL)
            {
                if (!ShapeUtilities::shapeFilter(origShape, TopAbs_SHELL, filterShape))
                {
                    COMMAND_END
                        return false;
                }
            }
            else if (type > TopAbs_WIRE)
            {
                return false;
            }
            else
            {
                filterShape = origShape;
            }

			bool topCover = true;
			if (filterShape.ShapeType() == TopAbs_WIRE)
			{
				topCover = false;
			}

            if (!shapeBuilder.extrudeDraftOperation(filterShape, globalXVec, globalYVec, globalZVec, draftAngle, mode, topCover))
            {
                COMMAND_END
                    return false;
            }

            targetShape = shapeBuilder.getShape();
            if (targetShape.IsNull())
            {
                return false;
            }

            // 找联系，在draft的情况下会失败
            adapter.collectRelatedLabelAndShapes(filterShape, EnExtrudeDraft, shapeBuilder.getBuilder());
        }

        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        adapter.linkRelatedLabels(targetLabel);

        // some infos for return;
        TCollection_AsciiString anEntry;
        TColStd_ListOfInteger aTagList;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::sweepAroundAxis(void* entry, double* wcsMatrix, const double& axisPosX, const double& axisPosY, const double& axisPosZ, const double& axisVecX, const double& axisVecY, const double& axisVecZ, const double& angle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find origin shape
        std::string instancePID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label origLabel;
        TDF_Tool::Label(data, instancePID.c_str(), origLabel);
        if (origLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

        //tranform
        double globalAxisPosX, globalAxisPosY, globalAxisPosZ, globalVecPosX, globalVecPosY, globalVecPosZ;
        ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, axisPosX, axisPosY, axisPosZ, globalAxisPosX, globalAxisPosY, globalAxisPosZ);
        ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, axisVecX, axisVecY, axisVecZ, globalVecPosX, globalVecPosY, globalVecPosZ);

        //start
        LabelShapeAdapter adapter;
        //preparation
        adapter.labelDiscreteToMap(origLabel);
        BRepBuilderAPI_TransitionMode mode;
        switch (draftType)
        {
        case Sweep_Extended:
        {
            mode = BRepBuilderAPI_Transformed;
            break;
        }
        case Sweep_Round:
        {
            mode = BRepBuilderAPI_RoundCorner;
            break;
        }
        case Sweep_Natural:
        {
            mode = BRepBuilderAPI_RightCorner;
            break;
        }
        default:
            break;
        }

        TopoDS_Shape targetShape, filterShape;
        SweepShapeBuilder shapeBuilder;
        if (!shapeBuilder.revolOperation(origShape, globalAxisPosX, globalAxisPosY, globalAxisPosZ, globalVecPosX, globalVecPosY, globalVecPosZ, angle))
        {
            COMMAND_END
                return false;
        }

        targetShape = shapeBuilder.getShape();
        if (targetShape.IsNull())
        {
            return false;
        }

        // 找联系
        adapter.collectRelatedLabelAndShapes(filterShape, EnRevol, shapeBuilder.getBuilder());

        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        adapter.linkRelatedLabels(targetLabel);

        // some infos for return;
        TCollection_AsciiString anEntry;
        TColStd_ListOfInteger aTagList;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::sweepAlongPath(void* profileEntry, void* pathEntry, const double& twistAngle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find profile shape
		std::string instancePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(profileEntry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label profileLabel;
		TDF_Tool::Label(data, instancePID.c_str(), profileLabel);
		if (profileLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape profileShape = LabelUtilities::getLabelShape(profileLabel);

		//find path shape
		if (!TheOCCApp.getCurrentModel()->findShapePID(pathEntry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label pathLabel;
		TDF_Tool::Label(data, instancePID.c_str(), pathLabel);
		if (pathLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape pathShape = LabelUtilities::getLabelShape(pathLabel);


		//start
		LabelShapeAdapter adapter;
		//preparation
		adapter.labelDiscreteToMap(profileLabel);
		adapter.labelDiscreteToMap(pathLabel);
		BRepBuilderAPI_TransitionMode mode;
		switch (draftType)
		{
		case Sweep_Extended:
		{
			mode = BRepBuilderAPI_Transformed;
			break;
		}
		case Sweep_Round:
		{
			mode = BRepBuilderAPI_RoundCorner;
			break;
		}
		case Sweep_Natural:
		{
			mode = BRepBuilderAPI_RightCorner;
			break;
		}
		default:
			break;
		}

		TopoDS_Shape targetShape, filterProfileShape, filterPathShape;
		if (pathShape.ShapeType() <= TopAbs_WIRE)
		{
			if (!ShapeUtilities::shapeFilter(pathShape, TopAbs_WIRE, filterPathShape))
			{
				return false;
			}
		}
		else if (pathShape.ShapeType() == TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeCover(pathShape, filterPathShape))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		SweepShapeBuilder shapeBuilder;
		//         if (twistAngle == 0)
		//         {
		//             if (profileShape.ShapeType() <= TopAbs_FACE)
		//             {
		//                 if (!ShapeUtilities::shapeFilter(profileShape, TopAbs_FACE, filterProfileShape))
		//                 {
		//                     COMMAND_END
		//                         return false;
		//                 }
		//             }
		//             else
		//             {
		//                 filterProfileShape = profileShape;
		//             }
		// 
		//             if (!shapeBuilder.sweepPipeOperation(filterProfileShape, filterPathShape))
		//             {
		//                 COMMAND_END
		//                     return false;
		//             }
		//         }
		//         else
		{

			TopoDS_Shape profileWire = profileShape;
			if (!shapeBuilder.sweepPipeShellOperation(profileWire, filterPathShape, twistAngle, mode))
			{
				COMMAND_END
					return false;
			}
		}

		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		adapter.collectRelatedLabelAndShapes(filterProfileShape, EnSweep, shapeBuilder.getBuilder());
		adapter.collectRelatedLabelAndShapes(filterPathShape, EnSweep, shapeBuilder.getBuilder());

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}

	return true;

	return sweepAlongPathPipeshell(profileEntry, pathEntry, twistAngle, draftAngle, draftType, name, strPID, resEntity);

    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find profile shape
        std::string instancePID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(profileEntry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label profileLabel;
        TDF_Tool::Label(data, instancePID.c_str(), profileLabel);
        if (profileLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape profileShape = LabelUtilities::getLabelShape(profileLabel);

        //find path shape
        if (!TheOCCApp.getCurrentModel()->findShapePID(pathEntry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label pathLabel;
        TDF_Tool::Label(data, instancePID.c_str(), pathLabel);
        if (pathLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape pathShape = LabelUtilities::getLabelShape(pathLabel);


        //start
        LabelShapeAdapter adapter;
        //preparation
        adapter.labelDiscreteToMap(profileLabel);
        adapter.labelDiscreteToMap(pathLabel);
        BRepBuilderAPI_TransitionMode mode;
        switch (draftType)
        {
        case Sweep_Extended:
        {
            mode = BRepBuilderAPI_Transformed;
            break;
        }
        case Sweep_Round:
        {
            mode = BRepBuilderAPI_RoundCorner;
            break;
        }
        case Sweep_Natural:
        {
            mode = BRepBuilderAPI_RightCorner;
            break;
        }
        default:
            break;
        }

        TopoDS_Shape targetShape, filterProfileShape, filterPathShape;
        if (pathShape.ShapeType() <= TopAbs_WIRE)
        {
            if (!ShapeUtilities::shapeFilter(pathShape, TopAbs_WIRE, filterPathShape))
            {
                return false;
            }
        }
        else if (pathShape.ShapeType() == TopAbs_EDGE)
        {
            if (!ShapeUtilities::shapeCover(pathShape, filterPathShape))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        SweepShapeBuilder shapeBuilder;
        if (twistAngle == 0)
        {
            if (profileShape.ShapeType() <= TopAbs_FACE)
            {
                if (!ShapeUtilities::shapeFilter(profileShape, TopAbs_FACE, filterProfileShape))
                {
                    COMMAND_END
                        return false;
                }
            }
            else
            {
                filterProfileShape = profileShape;
            }

            if (!shapeBuilder.sweepPipeOperation(filterProfileShape, filterPathShape))
            {
                COMMAND_END
                    return false;
            }
        }
        else
        {
            TopoDS_Shape profileWire = profileShape;
            if (!shapeBuilder.sweepPipeShellOperation(profileWire, filterPathShape, twistAngle, mode))
            {
                COMMAND_END
                    return false;
            }
        }

        targetShape = shapeBuilder.getShape();
        if (targetShape.IsNull())
        {
            return false;
        }

        // 找联系
        adapter.collectRelatedLabelAndShapes(filterProfileShape, EnSweep, shapeBuilder.getBuilder());
        adapter.collectRelatedLabelAndShapes(filterPathShape, EnSweep, shapeBuilder.getBuilder());

        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        adapter.linkRelatedLabels(targetLabel);

        // some infos for return;
        TCollection_AsciiString anEntry;
        TColStd_ListOfInteger aTagList;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::sweepAlongPathPipeshell(void* profileEntry, void* pathEntry, const double& twistAngle, const double& draftAngle, const SWEEPDRAFTTYPE& draftType, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find profile shape
        std::string instancePID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(profileEntry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label profileLabel;
        TDF_Tool::Label(data, instancePID.c_str(), profileLabel);
        if (profileLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape profileShape = LabelUtilities::getLabelShape(profileLabel);

        //find path shape
        if (!TheOCCApp.getCurrentModel()->findShapePID(pathEntry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label pathLabel;
        TDF_Tool::Label(data, instancePID.c_str(), pathLabel);
        if (pathLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape pathShape = LabelUtilities::getLabelShape(pathLabel);


        //start
        LabelShapeAdapter adapter;
        //preparation
        adapter.labelDiscreteToMap(profileLabel);
        adapter.labelDiscreteToMap(pathLabel);
        BRepBuilderAPI_TransitionMode mode;
        switch (draftType)
        {
        case Sweep_Extended:
        {
            mode = BRepBuilderAPI_Transformed;
            break;
        }
        case Sweep_Round:
        {
            mode = BRepBuilderAPI_RoundCorner;
            break;
        }
        case Sweep_Natural:
        {
            mode = BRepBuilderAPI_RightCorner;
            break;
        }
        default:
            break;
        }

        TopoDS_Shape targetShape, filterProfileShape, filterPathShape;
        if (pathShape.ShapeType() <= TopAbs_WIRE)
        {
            if (!ShapeUtilities::shapeFilter(pathShape, TopAbs_WIRE, filterPathShape))
            {
                return false;
            }
        }
        else if (pathShape.ShapeType() == TopAbs_EDGE)
        {
            if (!ShapeUtilities::shapeCover(pathShape, filterPathShape))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        SweepShapeBuilder shapeBuilder;
        //         if (twistAngle == 0)
        //         {
        //             if (profileShape.ShapeType() <= TopAbs_FACE)
        //             {
        //                 if (!ShapeUtilities::shapeFilter(profileShape, TopAbs_FACE, filterProfileShape))
        //                 {
        //                     COMMAND_END
        //                         return false;
        //                 }
        //             }
        //             else
        //             {
        //                 filterProfileShape = profileShape;
        //             }
        // 
        //             if (!shapeBuilder.sweepPipeOperation(filterProfileShape, filterPathShape))
        //             {
        //                 COMMAND_END
        //                     return false;
        //             }
        //         }
        //         else
        {

            TopoDS_Shape profileWire = profileShape;
            if (!shapeBuilder.sweepPipeShellOperation(profileWire, filterPathShape, twistAngle, mode))
            {
                COMMAND_END
                    return false;
            }
        }

        targetShape = shapeBuilder.getShape();
        if (targetShape.IsNull())
        {
            return false;
        }

        // 找联系
        adapter.collectRelatedLabelAndShapes(filterProfileShape, EnSweep, shapeBuilder.getBuilder());
        adapter.collectRelatedLabelAndShapes(filterPathShape, EnSweep, shapeBuilder.getBuilder());

        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        adapter.linkRelatedLabels(targetLabel);

        // some infos for return;
        TCollection_AsciiString anEntry;
        TColStd_ListOfInteger aTagList;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::thicken(void* entry, const double& thickness, const LOGICAL& isDoubleSides, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN

    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find origin shape
        std::string instancePID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label origLabel;
        TDF_Tool::Label(data, instancePID.c_str(), origLabel);
        if (origLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

        //start
        LabelShapeAdapter adapter;
        //preparation
        adapter.labelDiscreteToMap(origLabel);

        TopoDS_Shape targetShape;
        OffsetShapeBuilder shapeBuilder;
        if (isDoubleSides == True)
        {
			if (!shapeBuilder.faceOffsetOperation(origShape, -0.5*thickness, true))
			{
				COMMAND_END
					return false;
			}
			TopoDS_Shape thicken1 = shapeBuilder.getShape();
			if (thicken1.IsNull())
			{
				return false;
			}

			// 找联系
			adapter.collectRelatedLabelAndShapes(thicken1, EnThicken, shapeBuilder.getBuilder());

			if (!shapeBuilder.faceOffsetOperation(origShape, 0.5*thickness, true))
			{
				COMMAND_END
					return false;
			}
			TopoDS_Shape thicken2 = shapeBuilder.getShape();
			if (thicken2.IsNull())
			{
				return false;
			}

			// 找联系
			adapter.collectRelatedLabelAndShapes(thicken2, EnThicken, shapeBuilder.getBuilder());

			BooleanShapeBuilder bsBuilder;
			if (!bsBuilder.booleanOperation(thicken1, thicken2, BOPAlgo_FUSE, tolerance()))
			{
				COMMAND_END
				return false;
			}

			targetShape = bsBuilder.getShape();
			if (targetShape.IsNull())
			{
				return false;
			}

			// 找联系
			adapter.collectRelatedLabelAndShapes(targetShape, EnFuse, bsBuilder.getBuilder());

//             if (!shapeBuilder.faceOffsetOperation(origShape, -0.5*thickness, false))
//             {
//                 COMMAND_END
//                     return false;
//             }
//             TopoDS_Shape offsetShape = shapeBuilder.getShape();
//             if (offsetShape.IsNull())
//             {
//                 return false;
//             }
//             // 找联系
//             adapter.collectRelatedLabelAndShapes(origShape, EnOffset, shapeBuilder.getBuilder());
// 
//             if (!shapeBuilder.faceOffsetOperation(offsetShape, thickness, true))
//             {
//                 COMMAND_END
//                     return false;
//             }
// 
//             targetShape = shapeBuilder.getShape();
//             if (targetShape.IsNull())
//             {
//                 return false;
//             }
// 
//             // 找联系
//             adapter.collectRelatedLabelAndShapes(offsetShape, EnThicken, shapeBuilder.getBuilder());
        }
        else
        {
            if (!shapeBuilder.faceOffsetOperation(origShape, thickness, true))
            {
                COMMAND_END
                    return false;
            }

            targetShape = shapeBuilder.getShape();
            if (targetShape.IsNull())
            {
                return false;
            }

            // 找联系
            adapter.collectRelatedLabelAndShapes(origShape, EnThicken, shapeBuilder.getBuilder());
        }

        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        adapter.linkRelatedLabels(targetLabel);

        // some infos for return;
        TCollection_AsciiString anEntry;
        TColStd_ListOfInteger aTagList;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }
}

bool OccModelBuilder::detachEdges(void* entry, const std::list<void*>& edgeList, LOGICAL isCreated, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//prepare
		LabelShapeAdapter adapter;

		//find orig shape and label
		TopoDS_Shape origWire;
		TDF_Label origLabel;
		std::string origPID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, origPID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, origPID.c_str(), origLabel);
		if (origLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		origWire = LabelUtilities::getLabelShape(origLabel);
		adapter.labelDiscreteToMap(origLabel);

		//find tool shape and label
		TopTools_ListOfShape toolEdges;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = edgeList.begin(); ptrIter != edgeList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			adapter.labelDiscreteToMap(foundLabel);
			adapter.directRelateShapes(foundShape, foundShape);
			toolEdges.Append(foundShape);
		}

		//start		
		BooleanShapeBuilder shapeBuilder;
		if (!shapeBuilder.detachEdges(origWire, toolEdges, isCreated == LOGICAL::True ? true : false))
		{
			COMMAND_END
				return false;
		}

		TopoDS_Shape targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		adapter.collectRelatedLabelAndShapes(origWire, EnDetachEdges, shapeBuilder.getBuilder());
		TopTools_ListOfShape::Iterator shapeIter(toolEdges);
		for (; shapeIter.More(); shapeIter.Next())
		{
			adapter.collectRelatedLabelAndShapes(shapeIter.Value(), EnDetachEdges, shapeBuilder.getBuilder());
		}

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::createPoint(double* wcsMatrix, const double& dCoordX, const double& dCoordY, const double& dCoordZ, const char* name, std::string& strPID, void*& resEntity)
{
	try
	{
		COMMAND_BEGIN

		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//transform
		double globalBasePosX, globalBasePosY, globalBasePosZ;
		ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, dCoordX, dCoordY, dCoordZ, globalBasePosX, globalBasePosY, globalBasePosZ);

		//create shape
		VertexShapeBuilder vertexBuilder;
		if (!vertexBuilder.createVertex(globalBasePosX, globalBasePosY, globalBasePosZ))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = vertexBuilder.getShape();

		//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::fillet(void* solidEntry, const std::list<std::string>& edgeorvertexlist, const double& radius, const double& setback, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN
		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find solid shape
		std::string instancePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(solidEntry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label solidLabel;
		TDF_Tool::Label(data, instancePID.c_str(), solidLabel);
		if (solidLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(solidLabel);

		TopTools_ListOfShape edgeOrVertexShapeList;
		for (std::list<std::string>::const_iterator iter = edgeorvertexlist.begin(); iter != edgeorvertexlist.end(); ++iter)
		{
			//instance 
			std::string edgeVetexPID = *iter;
			TDF_Label foundLabel;
			TDF_Tool::Label(data, edgeVetexPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			TopoDS_Shape shape = LabelUtilities::getLabelShape(foundLabel);
			if (shape.IsNull())
			{
				continue;
			}
			edgeOrVertexShapeList.Append(shape);
		}

		//start operation
		LabelShapeAdapter adapter;
		//preparation
		adapter.labelDiscreteToMap(solidLabel);

		//fillet
		FilletShapeBuilder shapeBuilder;
		TopoDS_Shape targetShape;
		EnAdapatType operatorType;
		gp_Pln plane;
		if (ShapeUtilities::isShapePlannar(ownerShape, plane))
		{
			//remove shell or compound
			TopoDS_Shape filterShape;
			if (!ShapeUtilities::shapeFilter(ownerShape, TopAbs_FACE, filterShape))
			{
				if (!ShapeUtilities::shapeFilter(ownerShape, TopAbs_WIRE, filterShape))
				{
					COMMAND_END
						return false;
				}
			}
			adapter.directRelateShapes(ownerShape, filterShape);

			//copy shape 
			TransformShapeBuilder tsBuilder;
			tsBuilder.shapeCopy(filterShape);
			filterShape = tsBuilder.getShape();
			adapter.collectRelatedLabelAndShapes(filterShape, EnCopy, tsBuilder.getBuilder());

			//fillet
			TopTools_ListOfShape::Iterator verticeIter(edgeOrVertexShapeList);
			TopTools_ListOfShape updateVerticeList;
			for (; verticeIter.More(); verticeIter.Next())
			{
				TopoDS_Shape vertex = verticeIter.Value();
				if (vertex.ShapeType() != TopAbs_VERTEX)
				{
					COMMAND_END
						return false;
				}

				//update vertex list	
				BRepBuilderAPI_MakeShape* copyBuilder = (BRepBuilderAPI_MakeShape*)tsBuilder.getBuilder();
				const TopTools_ListOfShape& modifiedShapes = copyBuilder->Modified(vertex);
				TopoDS_Shape updateVertex = modifiedShapes.First();
				updateVerticeList.Append(updateVertex);
			}

			//operation
			if (!shapeBuilder.vertexFillet2DOperation(filterShape, updateVerticeList, radius, 0, true, true))
			{
				COMMAND_END

					return false;
			}
			operatorType = EnFillet2D;
			filterShape = shapeBuilder.getShape();
// 			OCCFixToolInstance->fixTopoValidityOp(filterShape);
			adapter.collectRelatedLabelAndShapes(filterShape, operatorType, shapeBuilder.getBuilder());

			targetShape = filterShape;
		}
		else
		{
			if (!shapeBuilder.edgeFilletOperation(ownerShape, edgeOrVertexShapeList, radius))
			{
				COMMAND_END
					return false;
			}
			operatorType = EnFillet;
			// 找联系
			targetShape = shapeBuilder.getShape();
			adapter.collectRelatedLabelAndShapes(ownerShape, operatorType, shapeBuilder.getBuilder());
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
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}

	return true;
}

// bool OccModelBuilder::fillet(void* solidEntry, const std::list<std::string>& edgeorvertexlist, const double& radius, const double& setback, const char* name, std::string& strPID, void*& resEntity)
// {
// 	COMMAND_BEGIN
// 		try
// 	{
// 		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
// 		Handle(TDF_Data) data = curDoc->GetData();
// 		TDF_Label rootLabel = data->Root();
// 
// 		//find solid shape
// 		std::string instancePID;
// 		if (!TheOCCApp.getCurrentModel()->findShapePID(solidEntry, instancePID))
// 		{
// 			COMMAND_END
// 				return false;
// 		}
// 		TDF_Label solidLabel;
// 		TDF_Tool::Label(data, instancePID.c_str(), solidLabel);
// 		if (solidLabel.IsNull())
// 		{
// 			COMMAND_END
// 				return false;
// 		}
// 		TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(solidLabel);
// 
// 		TopTools_ListOfShape edgeOrVertexShapeList;
// 		for (std::list<std::string>::const_iterator iter = edgeorvertexlist.begin(); iter != edgeorvertexlist.end(); ++iter)
// 		{
// 			//instance 
// 			std::string edgeVetexPID = *iter;
// 			TDF_Label foundLabel;
// 			TDF_Tool::Label(data, edgeVetexPID.c_str(), foundLabel);
// 			if (foundLabel.IsNull())
// 			{
// 				COMMAND_END
// 					return false;
// 			}
// 			TopoDS_Shape shape = LabelUtilities::getLabelShape(foundLabel);
// 			if (shape.IsNull())
// 			{
// 				continue;
// 			}
// 			edgeOrVertexShapeList.Append(shape);
// 		}
// 
// 		//start operation
// 		LabelShapeAdapter adapter;
// 		//preparation
// 		adapter.labelDiscreteToMap(solidLabel);
// 
// 		//fillet
// 		FilletShapeBuilder shapeBuilder;
// 		TopoDS_Shape targetShape;
// 		EnAdapatType operatorType;
// 		gp_Pln plane;
// 		if (ShapeUtilities::isShapePlannar(ownerShape,plane))
// 		{
// 			//remove shell or compound
// 			TopoDS_Shape filterShape;
// 			if (!ShapeUtilities::shapeFilter(ownerShape, TopAbs_FACE, filterShape))
// 			{
// 				if (!ShapeUtilities::shapeFilter(ownerShape, TopAbs_WIRE, filterShape))
// 				{
// 					COMMAND_END
// 						return false;
// 				}
// 			}
// 			adapter.directRelateShapes(ownerShape, filterShape);
// 
// 			//copy shape 
// 			TransformShapeBuilder tsBuilder;
// 			tsBuilder.shapeCopy(filterShape);
// 			filterShape = tsBuilder.getShape();
// 			adapter.collectRelatedLabelAndShapes(filterShape, EnCopy, tsBuilder.getBuilder());
// 
// 			//fillet
// 			TopTools_ListOfShape::Iterator verticeIter(edgeOrVertexShapeList);
// 			for (; verticeIter.More(); verticeIter.Next())
// 			{
// 				TopoDS_Shape vertex = verticeIter.Value();
// 				if (vertex.ShapeType() != TopAbs_VERTEX)
// 				{
// 					COMMAND_END
// 						return false;
// 				}
// 
// 				//update vertex list	
// 				BRepBuilderAPI_MakeShape* copyBuilder = (BRepBuilderAPI_MakeShape*)tsBuilder.getBuilder();
// 				const TopTools_ListOfShape& modifiedShapes = copyBuilder->Modified(vertex);
// 				TopoDS_Shape updateVertex = modifiedShapes.First();
// 
// 				TopTools_ListOfShape adjEdges;
// 				bool found = ShapeUtilities::FindConnectedEdges(filterShape, TopoDS::Vertex(updateVertex), adjEdges);
// 				int num = ShapeUtilities::getNumTopoType(filterShape, TopAbs_VERTEX);
// 				//operation
// 				if (!shapeBuilder.vertexFillet2DOperation(filterShape, updateVertex, radius, 0, true, true))
// 				{
// 					COMMAND_END
// 
// 						return false;
// 				}
// 				operatorType = EnFillet2D;
// 				filterShape = shapeBuilder.getShape();
// 				OCCFixToolInstance->fixTopoValidityOp(filterShape);
// 
// 				adapter.collectRelatedLabelAndShapes(filterShape, operatorType, shapeBuilder.getBuilder());
// 			}
// 			targetShape = filterShape;
// 		}
// 		else
// 		{
// 			if (!shapeBuilder.edgeFilletOperation(ownerShape, edgeOrVertexShapeList, radius))
// 			{
// 				COMMAND_END
// 					return false;
// 			}
// 			operatorType = EnFillet;
// 			// 找联系
// 			targetShape = shapeBuilder.getShape();
// 			adapter.collectRelatedLabelAndShapes(ownerShape, operatorType, shapeBuilder.getBuilder());
// 		}
// 
// 		// 根据布尔运算后的模型自动分配子节点到各个label
// 		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
// 		adapter.setTopLabel(targetLabel);
// 		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
// 		adapter.linkRelatedLabels(targetLabel);
// 
// 		// some infos for return;
// 		TCollection_AsciiString anEntry;
// 		TColStd_ListOfInteger aTagList;
// 		TDF_Tool::Entry(targetLabel, anEntry);
// 		strPID = anEntry.ToCString();
// 		TDataStd_AsciiString::Set(targetLabel, "top_level");
// 
// 		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);
// 
// 		COMMAND_END
// 
// 			return true;
// 	}
// 	catch (...)
// 	{
// 		COMMAND_END
// 
// 			return false;
// 	}
// 
// 	return true;
// }

bool OccModelBuilder::chamfer(void* solidEntry, const std::list<std::string>& edgeorvertexlist, const double& setback, const double& lDistance, const double& rDistance, const CHAMFERTYPE& chamferType, const char* name, std::string& strPID, void*& resEntity)
{
    COMMAND_BEGIN
        try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find solid shape
        std::string instancePID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(solidEntry, instancePID))
        {
            COMMAND_END
                return false;
        }
        TDF_Label solidLabel;
        TDF_Tool::Label(data, instancePID.c_str(), solidLabel);
        if (solidLabel.IsNull())
        {
            COMMAND_END
                return false;
        }
        TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(solidLabel);

        NCollection_List<TopoDS_Shape> edgeOrVertexShapeList;
        for (std::list<std::string>::const_iterator iter = edgeorvertexlist.begin(); iter != edgeorvertexlist.end(); ++iter)
        {
            //instance 
            std::string edgeVetexPID = *iter;
            TDF_Label foundLabel;
            TDF_Tool::Label(data, edgeVetexPID.c_str(), foundLabel);
            if (foundLabel.IsNull())
            {
                COMMAND_END
                    return false;
            }
            TopoDS_Shape shape = LabelUtilities::getLabelShape(foundLabel);
            if (shape.IsNull())
            {
                continue;
            }
            edgeOrVertexShapeList.Append(shape);
        }

		//start operation
		LabelShapeAdapter adapter;
		//preparation
		adapter.labelDiscreteToMap(solidLabel);

		//fillet
		FilletShapeBuilder shapeBuilder;
		TopoDS_Shape targetShape;
		EnAdapatType operatorType;
		gp_Pln plane;
		if (ShapeUtilities::isShapePlannar(ownerShape, plane))
		{
			//remove shell or compound
			TopoDS_Shape filterShape;
			if (!ShapeUtilities::shapeFilter(ownerShape, TopAbs_FACE, filterShape))
			{
				if (!ShapeUtilities::shapeFilter(ownerShape, TopAbs_WIRE, filterShape))
				{
					COMMAND_END
						return false;
				}
			}
			adapter.directRelateShapes(ownerShape, filterShape);

			//copy shape 
			TransformShapeBuilder tsBuilder;
			tsBuilder.shapeCopy(filterShape);
			filterShape = tsBuilder.getShape();
			adapter.collectRelatedLabelAndShapes(filterShape, EnCopy, tsBuilder.getBuilder());

			//fillet
			TopTools_ListOfShape::Iterator verticeIter(edgeOrVertexShapeList);
			for (; verticeIter.More(); verticeIter.Next())
			{
				TopoDS_Shape vertex = verticeIter.Value();
				if (vertex.ShapeType() != TopAbs_VERTEX)
				{
					COMMAND_END
						return false;
				}

				//update vertex list	
				BRepBuilderAPI_MakeShape* copyBuilder = (BRepBuilderAPI_MakeShape*)tsBuilder.getBuilder();
				const TopTools_ListOfShape& modifiedShapes = copyBuilder->Modified(vertex);
				TopoDS_Shape updateVertex = modifiedShapes.First();

				TopTools_ListOfShape adjEdges;
				bool found = ShapeUtilities::FindConnectedEdges(filterShape, TopoDS::Vertex(updateVertex), adjEdges);
				int num = ShapeUtilities::getNumTopoType(filterShape, TopAbs_VERTEX);
				//operation
				if (!shapeBuilder.vertexFillet2DOperation(filterShape, updateVertex, lDistance, rDistance, false, chamferType == Symmetric))
				{
					COMMAND_END

						return false;
				}
				operatorType = EnFillet2D;
				filterShape = shapeBuilder.getShape();
				OCCFixToolInstance->fixTopoValidityOp(filterShape);

				adapter.collectRelatedLabelAndShapes(filterShape, operatorType, shapeBuilder.getBuilder());
			}
			targetShape = filterShape;
		}
		else
		{
			if (!shapeBuilder.edgeChamferOperation(ownerShape, edgeOrVertexShapeList, lDistance, rDistance, chamferType == Symmetric))
			{
				COMMAND_END
					return false;
			}
			operatorType = EnFillet;
			// 找联系
			targetShape = shapeBuilder.getShape();
			adapter.collectRelatedLabelAndShapes(ownerShape, operatorType, shapeBuilder.getBuilder());
		}

//         // 找联系
//         adapter.collectRelatedLabelAndShapes(solidShape, operatorType, shapeBuilder.getBuilder());

        // 根据布尔运算后的模型自动分配子节点到各个label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        adapter.setTopLabel(targetLabel);
        adapter.allocateLabelWithTopShape(targetLabel, targetShape);
        adapter.linkRelatedLabels(targetLabel);

        // some infos for return;
        TCollection_AsciiString anEntry;
        TColStd_ListOfInteger aTagList;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        COMMAND_END

            return true;
    }
    catch (...)
    {
        COMMAND_END

            return false;
    }

    return true;
}

bool OccModelBuilder::fillet(const std::list<void*>& edgesOrVertices,
	const double& radius, const double& setback,
	const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN
		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();
		//find shape and label
		LabelShapeAdapter adapter;

		//find all edge and vertex;
		TopTools_ListOfShape edgesOrVerticesArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = edgesOrVertices.begin(); ptrIter != edgesOrVertices.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			if (foundShape.ShapeType() == TopAbs_EDGE || foundShape.ShapeType() == TopAbs_VERTEX)
			{
				adapter.labelDiscreteToMap(foundLabel);
				edgesOrVerticesArray.Append(foundShape);
			}
		}

		//find owner label
		TDF_Label ownerLabel = LabelUtilities::getOwnerLabel(foundLabel);
		TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(ownerLabel);
		adapter.labelDiscreteToMap(ownerLabel);

		//start
		FilletShapeBuilder shapeBuilder;
		TopoDS_Shape targetShape;
		EnAdapatType operatorType;
		gp_Pln plane;
// 		double normalX, normalY, normalZ;
// 		bool hasNoraml;
		if (!ShapeUtilities::isShapePlannar(ownerShape, plane))//3d fillet
		{
			TopTools_ListOfShape edgeArray;
			TopTools_ListOfShape::Iterator shapeIter(edgesOrVerticesArray);
			for (; shapeIter.More(); shapeIter.Next())
			{
				if (shapeIter.Value().ShapeType() == TopAbs_EDGE)
				{
					edgeArray.Append(shapeIter.Value());
				}
				else
				{
					TopTools_ListOfShape instanceArray;
					ShapeUtilities::FindConnectedEdges(ownerShape, TopoDS::Vertex(shapeIter.Value()), instanceArray);
					TopTools_ListOfShape::Iterator shapeIter2(instanceArray);
					for (; shapeIter2.More(); shapeIter2.Next())
					{
						if (!edgeArray.Contains(shapeIter2.Value()))
						{
							edgeArray.Append(shapeIter2.Value());
							TDF_Label edgeLabel;
							if (!LabelUtilities::findShapeLabelFromRoot(data->Root(), shapeIter2.Value(), edgeLabel))
							{
								COMMAND_END
								return false;
							}

							adapter.labelDiscreteToMap(edgeLabel);
						}
					}
				}
			}

			if (!shapeBuilder.edgeFilletOperation(ownerShape, edgeArray, radius))
			{
				COMMAND_END
					return false;
			}
			operatorType = EnFillet;
		}
		else//2d fillet
		{
			if (!shapeBuilder.vertexFillet2DOperation(ownerShape, edgesOrVerticesArray, radius, 0, true, true))
			{
				COMMAND_END

					return false;
			}
			operatorType = EnFillet2D;
		}

		// 找联系
		targetShape = shapeBuilder.getShape();
		adapter.collectRelatedLabelAndShapes(ownerShape, operatorType, shapeBuilder.getBuilder());

		// 根据布尔运算后的模型自动分配子节点到各个label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}

	return true;
}

bool OccModelBuilder::chamfer(const std::list<void*>& edgesOrVertices,
	const double& setback, const double& lDistance, const double& rDistance,
	const CHAMFERTYPE& chamferType,
	const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN
		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();
		//find shape and label
		LabelShapeAdapter adapter;

		//find all edge and vertex;
		TopTools_ListOfShape edgesOrVerticesArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = edgesOrVertices.begin(); ptrIter != edgesOrVertices.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			if (foundShape.ShapeType() == TopAbs_EDGE || foundShape.ShapeType() == TopAbs_VERTEX)
			{
				adapter.labelDiscreteToMap(foundLabel);
				edgesOrVerticesArray.Append(foundShape);
			}
		}

		//find owner label
		TDF_Label ownerLabel = LabelUtilities::getOwnerLabel(foundLabel);
		TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(ownerLabel);
		adapter.labelDiscreteToMap(ownerLabel);

		//start
		FilletShapeBuilder shapeBuilder;
		TopoDS_Shape targetShape;
		EnAdapatType operatorType;
		gp_Pln plane;
// 		double normalX, normalY, normalZ;
// 		bool hasNoraml;
		if (!ShapeUtilities::isShapePlannar(ownerShape, plane))//3d fillet
		{
			TopTools_ListOfShape edgeArray;
			TopTools_ListOfShape::Iterator shapeIter(edgesOrVerticesArray);
			for (; shapeIter.More(); shapeIter.Next())
			{
				if (shapeIter.Value().ShapeType() == TopAbs_EDGE)
				{
					edgeArray.Append(shapeIter.Value());
				}
				else
				{
					TopTools_ListOfShape instanceArray;
					ShapeUtilities::FindConnectedEdges(ownerShape, TopoDS::Vertex(shapeIter.Value()), instanceArray);
					TopTools_ListOfShape::Iterator shapeIter2(instanceArray);
					for (; shapeIter2.More(); shapeIter2.Next())
					{
						if (!edgeArray.Contains(shapeIter2.Value()))
						{
							edgeArray.Append(shapeIter2.Value());
							TDF_Label edgeLabel;
							if (!LabelUtilities::findShapeLabelFromRoot(data->Root(), shapeIter2.Value(), edgeLabel))
							{
								COMMAND_END
									return false;
							}

							adapter.labelDiscreteToMap(edgeLabel);
						}
					}
				}
			}

			if (!shapeBuilder.edgeChamferOperation(ownerShape, edgeArray, lDistance, rDistance, chamferType == Symmetric))
			{
				COMMAND_END
					return false;
			}
			operatorType = EnChamfer;
		}
		else//2d fillet
		{
			if (!shapeBuilder.vertexFillet2DOperation(ownerShape, edgesOrVerticesArray, lDistance, rDistance, false, chamferType == Symmetric))
			{
				COMMAND_END

					return false;
			}
			operatorType = EnFillet2D;
		}

		// 找联系
		targetShape = shapeBuilder.getShape();
		adapter.collectRelatedLabelAndShapes(ownerShape, operatorType, shapeBuilder.getBuilder());

		// 根据布尔运算后的模型自动分配子节点到各个label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}

	return true;
}


bool OccModelBuilder::moveFaces(void* entry, double* wcsMatrix, const std::list<std::string>& edgeNames, const double& vectorX, const double& vectorY, const double& vectorZ, const char* name, std::string& strPID, void*& resEntity)
{
    return true;
}

bool OccModelBuilder::moveFaces(double* wcsMatrix, const std::list<void*>& entryList, const double& vectorX, const double& vectorY, const double& vectorZ, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//transform
		double  globalVectorX, globalVectorY, globalVectorZ;
		ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, vectorX, vectorY, vectorZ, globalVectorX, globalVectorY, globalVectorZ);

		//find shape and label
		LabelShapeAdapter adapter;

		TopTools_ListOfShape shapeArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = entryList.begin(); ptrIter != entryList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			if (foundShape.ShapeType() <= TopAbs_FACE)
			{
				TopoDS_Shape filterWire;
				if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_WIRE, filterWire))
				{
					COMMAND_END
						return false;
				}

				adapter.labelDiscreteToMap(foundLabel);
				adapter.directRelateShapes(foundShape, filterWire);
				shapeArray.Append(filterWire);
			}
		}

		//get shape on top level
		TDF_Label ownerLabel = LabelUtilities::getOwnerLabel(foundLabel);
		TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(ownerLabel);
		adapter.labelDiscreteToMap(ownerLabel);

		//extrude
		TopTools_ListOfShape extrudeShapeArray;
		TopTools_ListOfShape::Iterator iter(shapeArray);
		SweepShapeBuilder extrudeBuilder;
		for (; iter.More(); iter.Next())
		{
			if (!extrudeBuilder.extrudeOperation(iter.Value(), globalVectorX, globalVectorY, globalVectorZ))
			{
				COMMAND_END
				return false;
			}

			adapter.collectRelatedLabelAndShapes(iter.Value(), EnExtrude, extrudeBuilder.getBuilder());
			extrudeShapeArray.Append(extrudeBuilder.getShape());
		}

		//fuse
		TopTools_ListOfShape args;
		args.Append(ownerShape);
		BooleanShapeBuilder booleanBuilder;
		if (!booleanBuilder.booleanOperation(args, extrudeShapeArray, BOPAlgo_FUSE, tolerance()))
		{
			COMMAND_END
			return false;
		}
		TopoDS_Shape targetShape = booleanBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		adapter.collectRelatedLabelAndShapes(ownerShape, EnFuse, booleanBuilder.getBuilder());
		iter.Init(extrudeShapeArray);
		for (; iter.More(); iter.Next())
		{
			adapter.collectRelatedLabelAndShapes(iter.Value(), EnFuse, booleanBuilder.getBuilder());
		}

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

// bool OccModelBuilder::createFillet(const std::string& solidEntry, const std::vector<std::string>& faceNames, const double& radius, const char* name, std::string& strPID, void*& entity)
// {
// 	try
// 	{
// 		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
// 		Handle(TDF_Data) data = curDoc->GetData();
// 		TDF_Label rootLabel = data->Root();
// 
// 		//get souce label and shape
//         TDF_Label sourceLabel;
//         TDF_Tool::Label(data, TCollection_AsciiString(solidEntry.c_str()), sourceLabel, false);
// 
// 		Handle(TNaming_NamedShape) namedSourceShape;
// 		TopoDS_Shape sourceShape;
// 		if (sourceLabel.FindAttribute(TNaming_NamedShape::GetID(), namedSourceShape))
// 		{
// 			sourceShape = namedSourceShape->Get();
// 			if (sourceShape.IsNull())
// 			{
// 				return false;
// 			}
// 		}
// 		else
// 		{
// 			return false;
// 		}
// 
// 		//get two faces
// 		TopoDS_ListOfShape faceList;
// 		for (std::vector<std::string>::const_iterator nameIter = faceNames.begin(); nameIter != faceNames.end(); ++nameIter)
// 		{
// 			TDF_Label faceLabel;
// 			if (LabelUtilities::findNameLabel(sourceLabel, *nameIter, faceLabel))
// 			{
// 				Handle(TNaming_NamedShape) namedShape;
// 				if (faceLabel.FindAttribute(TNaming_NamedShape::GetID(), namedShape))
// 				{
// 					TopoDS_Shape faceShape = namedShape->Get();
// 					if (!faceShape.IsNull())
// 					{
// 						faceList.Append(faceShape);
// 					}
// 				}
// 				else
// 				{
// 					return false;
// 				}
// 			}
// 			else
// 			{
// 				return false;
// 			}
// 		}
// 		if (faceList.Size() != 2)
// 		{
// 			return false;
// 		}
// 
// 		// find edge
//         NCollection_DataMap<TopoDS_Shape, double> edgeInfo;
// 
//         //test code: all edge fillet
// //         TopExp_Explorer edge_exp1(sourceShape, TopAbs_EDGE);
// //         for (; edge_exp1.More(); edge_exp1.Next())
// //         {
// //             edgeInfo.Bind(edge_exp1.Value(), radius);
// //         }
// 
//         // real code
// 		TopExp_Explorer edge_exp1, edge_exp2;
// 		edge_exp1.Init(faceList.First(), TopAbs_EDGE);
// 		edge_exp2.Init(faceList.Last(), TopAbs_EDGE);
// 		bool found = false;
// 		for (; edge_exp1.More(); edge_exp1.Next())
// 		{
// 			if (found)
// 			{
// 				break;
// 			}
// 			for (; edge_exp2.More(); edge_exp2.Next())
// 			{
// 				if (edge_exp1.Value().IsSame(edge_exp2.Value()))
// 				{
//                     edgeInfo.Bind(edge_exp1.Value(), radius);
// 					found = true;
// 					break;
// 				}
// 			}
// 		}
//         if (edgeInfo.Size() == 0)
//         {
//             return false;
//         }
// 
// 		// make fillet
//         FilletShapeBuilder shapeBuilder;
//         if (!shapeBuilder.singleFilletOperation(sourceShape, edgeInfo))
//         {
//             return false;
//         }
// 
//         TopoDS_Shape newShape = shapeBuilder.getShape();
// 		newShape = LabelUtilities::shapeFilter(newShape);
// 
// 		// try to find label with same name 
// 		TDF_Label oldLabel;
// 		LabelUtilities::findNameLabel(rootLabel, name, oldLabel);
// 		TDF_Label targetLabel = TDF_TagSource::NewChild(rootLabel);
// 		LabelUtilities::copyLabelWithOnlyPropertyChild(sourceLabel, targetLabel);
// 		LabelShapeAdapter adapter;
// 		adapter.setName(name);
// 		adapter.clearModifiedShapes();
// 		adapter.addModifiedShapes(newShape, targetLabel);
//         adapter.collectLabelShapes(sourceLabel, EnFillet, shapeBuilder.getBuilder());
//         adapter.collectLabelShapesGeneratedByEdgeAndVertex(sourceShape, EnFillet, shapeBuilder.getBuilder());
// 
// 		//allocate
// 		adapter.setCompoundLabel(targetLabel);
// 		adapter.allocateLabelWithTopShape(targetLabel, newShape);
// 		LabelShapeAdapter::replaceChildLabelWithCopiedShape(targetLabel, newShape);
// 
// 		TDataStd_AsciiString::Set(targetLabel, "top_level");
// 		TDataStd_Name::Set(targetLabel, name);
// 
// 		// some infos for return;
// 		LabelUtilities::recursiveRenameChildLabelWithSameName(targetLabel);
// 		TCollection_AsciiString anEntry;
// 		TColStd_ListOfInteger aTagList;
// 		TDF_Tool::Entry(targetLabel, anEntry);
// 		strPID = anEntry.ToCString();
// 		if (!oldLabel.IsNull())
// 		{
// 			oldLabel.ForgetAllAttributes();
// 		}
// 		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
// 		entity = TheOCCApp.getCurrentModel()->findTopodsShapeFromMap(strPID);
// 
// 		return true;
// 	}
// 	catch (...)
// 	{
// 		return false;
// 	}
// }
// 
// bool OccModelBuilder::createChamfer(const std::string& solidEntry, const std::vector<std::string>& faceNames, const double& distance, const char* name, std::string& strPID, void*& entity)
// {
//     try
//     {
//         TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
//         Handle(TDF_Data) data = curDoc->GetData();
//         TDF_Label rootLabel = data->Root();
// 
//         //get souce label and shape
//         TDF_Label sourceLabel;
//         TDF_Tool::Label(data, TCollection_AsciiString(solidEntry.c_str()), sourceLabel, false);
// 
//         Handle(TNaming_NamedShape) namedSourceShape;
//         TopoDS_Shape sourceShape;
//         if (sourceLabel.FindAttribute(TNaming_NamedShape::GetID(), namedSourceShape))
//         {
//             sourceShape = namedSourceShape->Get();
//             if (sourceShape.IsNull())
//             {
//                 return false;
//             }
//         }
//         else
//         {
//             return false;
//         }
// 
//         //get two faces
//         TopoDS_ListOfShape faceList;
//         for (std::vector<std::string>::const_iterator nameIter = faceNames.begin(); nameIter != faceNames.end(); ++nameIter)
//         {
//             TDF_Label faceLabel;
//             if (LabelUtilities::findNameLabel(sourceLabel, *nameIter, faceLabel))
//             {
//                 Handle(TNaming_NamedShape) namedShape;
//                 if (faceLabel.FindAttribute(TNaming_NamedShape::GetID(), namedShape))
//                 {
//                     TopoDS_Shape faceShape = namedShape->Get();
//                     if (!faceShape.IsNull())
//                     {
//                         faceList.Append(faceShape);
//                     }
//                 }
//                 else
//                 {
//                     return false;
//                 }
//             }
//             else
//             {
//                 return false;
//             }
//         }
//         if (faceList.Size() != 2)
//         {
//             return false;
//         }
// 
//         // find edge
//         NCollection_DataMap<TopoDS_Shape, double> edgeInfo;
// 
//         //test code: all edge fillet
//         TopExp_Explorer edge_exp1(sourceShape, TopAbs_EDGE);
//         for (; edge_exp1.More(); edge_exp1.Next())
//         {
//             edgeInfo.Bind(edge_exp1.Value(), distance);
//         }
// 
//         // real code
//         // 		TopExp_Explorer edge_exp1, edge_exp2;
//         // 		edge_exp1.Init(faceList.First(), TopAbs_EDGE);
//         // 		edge_exp2.Init(faceList.Last(), TopAbs_EDGE);
//         // 		bool found = false;
//         // 		for (; edge_exp1.More(); edge_exp1.Next())
//         // 		{
//         // 			if (found)
//         // 			{
//         // 				break;
//         // 			}
//         // 			for (; edge_exp2.More(); edge_exp2.Next())
//         // 			{
//         // 				if (edge_exp1.Value().IsSame(edge_exp2.Value()))
//         // 				{
//         //                     edgeInfo.Bind(edge_exp1.Value(), distance);
//         // 					found = true;
//         // 					break;
//         // 				}
//         // 			}
//         // 		}
//         if (edgeInfo.Size() == 0)
//         {
//             return false;
//         }
// 
//         // make fillet
//         FilletShapeBuilder shapeBuilder;
//         if (!shapeBuilder.singleSymmetricChamferOperation(sourceShape, edgeInfo))
//         {
//             return false;
//         }
// 
//         TopoDS_Shape newShape = shapeBuilder.getShape();
// 		newShape = LabelUtilities::shapeFilter(newShape);
// 
//         // try to find label with same name 
//         TDF_Label oldLabel;
//         LabelUtilities::findNameLabel(rootLabel, name, oldLabel);
//         TDF_Label targetLabel = TDF_TagSource::NewChild(rootLabel);
//         LabelUtilities::copyLabelWithOnlyPropertyChild(sourceLabel, targetLabel);
//         LabelShapeAdapter adapter;
//         adapter.setName(name);
//         adapter.clearModifiedShapes();
//         adapter.addModifiedShapes(newShape, targetLabel);
//         adapter.collectLabelShapes(sourceLabel, EnFillet, shapeBuilder.getBuilder());
//         adapter.collectLabelShapesGeneratedByEdgeAndVertex(sourceShape, EnFillet, shapeBuilder.getBuilder());
// 
//         //allocate
//         adapter.setCompoundLabel(targetLabel);
//         adapter.allocateLabelWithTopShape(targetLabel, newShape);
//         LabelShapeAdapter::replaceChildLabelWithCopiedShape(targetLabel, newShape);
// 
//         TDataStd_AsciiString::Set(targetLabel, "top_level");
//         TDataStd_Name::Set(targetLabel, name);
// 
//         // some infos for return;
//         LabelUtilities::recursiveRenameChildLabelWithSameName(targetLabel);
//         TCollection_AsciiString anEntry;
//         TColStd_ListOfInteger aTagList;
//         TDF_Tool::Entry(targetLabel, anEntry);
//         strPID = anEntry.ToCString();
//         if (!oldLabel.IsNull())
//         {
//             oldLabel.ForgetAllAttributes();
//         }
//         TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
//         entity = TheOCCApp.getCurrentModel()->findTopodsShapeFromMap(strPID);
// 
//         return true;
//     }
//     catch (...)
//     {
//         return false;
//     }
// }

bool OccModelBuilder::offsetWire(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity)
{
	return wireOffset(entry, distance, name, strPID, resEntity);
}

bool OccModelBuilder::wireOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN
		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find ori shape
		std::string instancePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel);
		if (origLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

		//start
		LabelShapeAdapter adapter;
		//preparation
		adapter.labelDiscreteToMap(origLabel);

		TopoDS_Shape targetShape;
		OffsetShapeBuilder shapeBuilder;
		if (!shapeBuilder.edgeOffsetOperation(origShape, distance))
		{
			COMMAND_END
				return false;
		}

		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			//直线
			TopExp_Explorer exl;
			std::vector<gp_Pnt> Pntlist;
			for (exl.Init(origShape, TopAbs_VERTEX); exl.More(); exl.Next())
			{
				TopoDS_Vertex xvtx = TopoDS::Vertex(exl.Current());
				gp_Pnt xpnt = BRep_Tool::Pnt(xvtx);
				Pntlist.push_back(xpnt);
			}
			if (Pntlist.size() != 2)
				return false;
			double length = Pntlist[0].Distance(Pntlist[1]);
			gp_Vec vec1(Pntlist[1].X() - Pntlist[0].X(), Pntlist[1].Y() - Pntlist[0].Y(), Pntlist[1].Z() - Pntlist[0].Z());
			gp_Vec vecZ(0, 0, 1);
			gp_Vec vec2 = length * vecZ;
			gp_Vec finalvec = vec1.Crossed(vec2);
			gp_Trsf tf;
			tf.SetTranslation(finalvec * distance);

			BRepBuilderAPI_Transform transform(tf);
			transform.Perform(origShape);
			targetShape = transform.ModifiedShape(origShape);
		}
		if (targetShape.IsNull())
			return false;

		//找联系
		adapter.collectRelatedLabelAndShapes(origShape, EnThicken, shapeBuilder.getBuilder());

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		//some infos for return
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END
			return true;
	}
	catch (...)
	{
		COMMAND_END
			return false;
	}
}

bool OccModelBuilder::connect(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN
		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		// find shape and label
		LabelShapeAdapter adapter;
		bool genSolid = true;

		TopTools_ListOfShape shapeArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		TransformShapeBuilder tsBuilder;
		for (std::list<void*>::const_iterator ptrIter = entryList.begin(); ptrIter != entryList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					continue;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					continue;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);
			if (foundShape.ShapeType() == TopAbs_WIRE || foundShape.ShapeType() == TopAbs_VERTEX)
			{
				genSolid = false;
				adapter.labelDiscreteToMap(foundLabel);
				//copy
				tsBuilder.shapeCopy(foundShape);
				adapter.collectRelatedLabelAndShapes(foundShape, EnCopy, tsBuilder.getBuilder());
				foundShape = tsBuilder.getShape();

				shapeArray.Append(foundShape);
			}
			else if (foundShape.ShapeType() < TopAbs_WIRE)
			{
				TopoDS_Shape filterWire;
				if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_WIRE, filterWire))
				{
					COMMAND_END
						continue;
				}

				adapter.labelDiscreteToMap(foundLabel);
				//copy
				tsBuilder.shapeCopy(filterWire);
				adapter.collectRelatedLabelAndShapes(filterWire, EnCopy, tsBuilder.getBuilder());
				filterWire = tsBuilder.getShape();
				shapeArray.Append(filterWire);
			}
		}

		//start
		SweepShapeBuilder ssb;
		if (!ssb.loftOperation(shapeArray, genSolid))
		{
			COMMAND_END
				return false;
		}

		TopoDS_Shape targetShape = ssb.getShape();
		if (targetShape.IsNull())
			return false;

		adapter.collectRelatedLabelAndShapes(targetShape, EnLoft, ssb.getBuilder());

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END
			return false;
	}
}

bool OccModelBuilder::offsetFaces(void* entry, const std::list<std::string>& edgeNames, const double& distance, const char* name, std::string& strPID, void*& resEntity)
{
    return true;
}

bool OccModelBuilder::offsetFaces(const std::list<void*>& entryList, const double& distance, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		TopTools_ListOfShape shapeArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = entryList.begin(); ptrIter != entryList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			if (foundShape.ShapeType() <= TopAbs_FACE)
			{
				TopoDS_Shape filterWire;
				if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_FACE, filterWire))
				{
					COMMAND_END
						return false;
				}

				adapter.labelDiscreteToMap(foundLabel);
				adapter.directRelateShapes(foundShape, filterWire);
				shapeArray.Append(filterWire);
			}
		}

		//get shape on top level
		TDF_Label ownerLabel = LabelUtilities::getOwnerLabel(foundLabel);
		TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(ownerLabel);
		adapter.labelDiscreteToMap(ownerLabel);

		//solid face for thickening
// 		TopTools_ListOfShape args;
// 		args.Append(ownerShape);
		OffsetShapeBuilder offsetBuilder;
		if (!offsetBuilder.solidFaceOffsetOperation(ownerShape, shapeArray, distance))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape targetShape = offsetBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		adapter.collectRelatedLabelAndShapes(ownerShape, EnOffset, offsetBuilder.getBuilder());
		TopTools_ListOfShape::Iterator iter(shapeArray);
		for (; iter.More(); iter.Next())
		{
			adapter.collectRelatedLabelAndShapes(iter.Value(), EnOffset, offsetBuilder.getBuilder());
		}

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::copyFaces(void* entry, const std::string& faceName, const char* name, std::string& strPID, void*& resEntity)
{
    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find top label
        xdTopoDS_HShape* hshape = static_cast<xdTopoDS_HShape*>(entry);
        TopoDS_Shape searchShape = hshape->Shape();

        std::string origPID;
        if (!LabelUtilities::findShapePIDFromRoot(rootLabel, searchShape, strPID))
        {
            return false;
        }

        TDF_Label origLabel;
        TDF_Tool::Label(data, TCollection_AsciiString(origPID.c_str()), origLabel, false);

        //find face label
        TDF_Label faceLabel;
        if (origLabel.IsNull()|| !LabelUtilities::findNameLabel(origLabel, faceName, faceLabel)|| faceLabel.IsNull())
        {
            return false;
        }

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //copy label
        LabelUtilities::copyLabelWithFather(faceLabel, targetLabel);
        //get face shape
        Handle(TNaming_NamedShape) faceNamedShape;
        if (!targetLabel.FindAttribute(TNaming_NamedShape::GetID(), faceNamedShape))
        {
            return false;
        }
        TopoDS_Shape faceShape = faceNamedShape->Get();

        //create child label
        LabelShapeAdapter::replaceChildLabelWithCopiedShape2(targetLabel, faceShape);
//         TDataStd_Name::Set(targetLabel, name);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        return true;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool OccModelBuilder::copyFaces(void* entity, const char* name, std::string& strPID, void*& resEntity)
{
	return createCloneEntity(entity, name, strPID, resEntity);
}

bool OccModelBuilder::sliceByFaces(void* entry, const std::list<void*>& toolFaceList, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		LabelShapeAdapter adapter;

		//find origin shape on top level
		TDF_Label origLabel;
		TopoDS_Shape origShape;
		std::string origPID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, origPID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, origPID.c_str(), origLabel);
		if (origLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		origShape = LabelUtilities::getLabelShape(origLabel);
		adapter.labelDiscreteToMap(origLabel);

		//find shape and label
		TopTools_ListOfShape shapeArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = toolFaceList.begin(); ptrIter != toolFaceList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			if (foundShape.ShapeType() <= TopAbs_FACE)
			{
				TopoDS_Shape filterFace;
				if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_FACE, filterFace))
				{
					COMMAND_END
						return false;
				}

				adapter.labelDiscreteToMap(foundLabel);
				adapter.directRelateShapes(foundShape, filterFace);
				shapeArray.Append(filterFace);
			}
		}

		//slid
		
		TopTools_ListOfShape args;
		args.Append(origShape);
		BooleanShapeBuilder offsetBuilder;
		if (!offsetBuilder.shapeSplitOperation2(origShape, shapeArray))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape targetShape = offsetBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		adapter.collectRelatedLabelAndShapes(origShape, EnNormal, offsetBuilder.getBuilder());
		TopTools_ListOfShape::Iterator iter(shapeArray);
		for (; iter.More(); iter.Next())
		{
			adapter.collectRelatedLabelAndShapes(iter.Value(), EnNormal, offsetBuilder.getBuilder());
		}

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::createEntityPIDForEntity(void* curBdy, std::string& newTag)
{
    return TheOCCApp.getCurrentModel()->findShapePID(curBdy, newTag);
}

bool OccModelBuilder::createCloneEntity(void* curBdy, const char* name, std::string& strPID, void*& resEntity)
{
    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        //find top label
        xdTopoDS_HShape* hshape = static_cast<xdTopoDS_HShape*>(curBdy);
        TopoDS_Shape searchShape = hshape->Shape();

        std::string origPID;
        if (!LabelUtilities::findShapePIDFromRoot(rootLabel, searchShape, origPID))
        {
            return false;
        }

        TDF_Label origLabel;
        TDF_Tool::Label(data, TCollection_AsciiString(origPID.c_str()), origLabel, false);
        if (origLabel.IsNull())
        {
            return false;
        }

        //create label
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
        TCollection_AsciiString anEntry;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();

        //copy label
        LabelUtilities::copyLabelWithFather(origLabel, targetLabel);
        //get shape
        Handle(TNaming_NamedShape) targetNamedShape;
        if (!targetLabel.FindAttribute(TNaming_NamedShape::GetID(), targetNamedShape))
        {
            return false;
        }
        TopoDS_Shape targetShape = targetNamedShape->Get();

        //create child label
        LabelShapeAdapter::replaceChildLabelWithCopiedShape2(targetLabel, targetShape);
//         TDataStd_Name::Set(targetLabel, name);
        TDataStd_AsciiString::Set(targetLabel, "top_level");

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

        return true;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool OccModelBuilder::splitFaceByPoints(std::vector<void*> entities, std::vector<std::tuple<double, double>> UVList, LOGICAL isClosed,
	const char* name, std::string& strPID, void*& resEntity)
{
	try
	{
		if (entities.size() < 2 || entities.size() != UVList.size())
			return false;
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		LabelShapeAdapter adapter;
		std::string edgePID;
		TDF_Label edgeLabel;

		if (!TheOCCApp.getCurrentModel()->findShapePID(entities[0], edgePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, edgePID.c_str(), edgeLabel);
		if (edgeLabel.IsNull())
		{
			COMMAND_END
				return false;
		}

		TDF_Label ownerLabel = LabelUtilities::getOwnerLabel(edgeLabel);
		TopoDS_Shape ownerShape = LabelUtilities::getLabelShape(ownerLabel);
		adapter.labelDiscreteToMap(ownerLabel);

		TransformShapeBuilder tsbuilder;
		tsbuilder.shapeCopy(ownerShape);
		adapter.collectRelatedLabelAndShapes(ownerShape, EnCopy, tsbuilder.getBuilder());
		TopoDS_Shape copiedshape = tsbuilder.getShape();

		//闭合
		bool needClose = false;
		if (isClosed == LOGICAL::True && entities.size() > 2 && entities[0] != entities[entities.size() - 1] && UVList[0] != UVList[UVList.size() - 1])
		{
			needClose = true;
			entities.push_back(entities[0]);
			UVList.push_back(UVList[0]);
		}

		//find subshape
		BRepBuilderAPI_MakeShape* api = (BRepBuilderAPI_MakeShape*)tsbuilder.getBuilder();
		std::tuple<double, double> preValue, curValue;
		preValue = UVList[0];
		TopoDS_Shape preshape, curshape;
		TopoDS_Edge targetedge;
		TopoDS_Wire wire;
		BRep_Builder B;
		B.MakeWire(wire);
		TopTools_ListOfShape tools;
		for (int i = 1; i < entities.size(); i++)
		{
			//pre
			void* prevoid = entities[i - 1];
			TopoDS_HShape* prehshape = static_cast<TopoDS_HShape*>(prevoid);
			TopoDS_Shape orgshape = prehshape->Shape();
			{
				const TopTools_ListOfShape& modifiedShapes = api->Modified(orgshape);
				for (auto iter : modifiedShapes)
					preshape = iter;
			}

			//cur
			void* curvoid = entities[i];
			TopoDS_HShape* curhshape = static_cast<TopoDS_HShape*>(curvoid);
			orgshape = curhshape->Shape();
			{
				const TopTools_ListOfShape& modifiedShapes = api->Modified(orgshape);
				for (auto iter : modifiedShapes)
					curshape = iter;
			}

			preValue = UVList[i - 1];
			curValue = UVList[i];

			if (!ShapeUtilities::BuildSurfaceEdge(copiedshape, preshape, curshape, preValue,
				curValue, targetedge))
			{
				COMMAND_END
					return false;
			}

			if (i != 0)
			{
				tools.Append(targetedge);
			}
		}
		BooleanShapeBuilder bsbuilder;
		if (!bsbuilder.shapeImprintOperation(copiedshape, tools, &adapter, 0, 0))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape targetShape = bsbuilder.getShape();
		if (targetShape.NbChildren() == 0)
		{
			COMMAND_END
				return false;
		}
		TDF_Label targetlabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetlabel);
		adapter.alloateLabelWithTopShape(tartlabel, targetshape);
		adapter.linkRelateLabels(targetlabel);

		TCollection_AsciiString anentry;
		TColStd_ListOfInteger ataglist;
		TDF_Tool::Entry(targetlabel, anentry);
		strPID = anentry.ToCString();
		TDataStd_AsciiString::Set(targetlabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);
		COMMAND_END
			return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::split(void* entry, double* wcsMatrix, const double& basePosX, const double& basePosY, const double& basePosZ, const double& normalVecX, const double& normalVecY, const double& normalVecZ, const KEEPSIDE& keepSide, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//transform
		double globalBasePosX, globalBasePosY, globalBasePosZ, globalVectorX, globalVectorY, globalVectorZ;
		ShapeUtilities::pntTransformFromLocalToGlobal(wcsMatrix, basePosX, basePosY, basePosZ, globalBasePosX, globalBasePosY, globalBasePosZ);
		ShapeUtilities::vectorTransformFromLocalToGlobal(wcsMatrix, normalVecX, normalVecY, normalVecZ, globalVectorX, globalVectorY, globalVectorZ);


		//find shape and label
		LabelShapeAdapter adapter;

		std::string srcPID;
		TDF_Label srcLabel;
		TopoDS_Shape srcShape;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, srcPID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, srcPID.c_str(), srcLabel);
		if (srcLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		srcShape = LabelUtilities::getLabelShape(srcLabel);
		adapter.labelDiscreteToMap(srcLabel);


		//start		
		bool normalVecSide = keepSide == KEEPSIDE::Positive ? true : false;
		BooleanShapeBuilder shapeBuilder;
		if (!shapeBuilder.shapeSplitOperation(srcShape, globalBasePosX, globalBasePosY, globalBasePosZ, globalVectorX, globalVectorY, globalVectorZ, normalVecSide))
		{
			COMMAND_END
				return false;
		}

		TopoDS_Shape targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		if (normalVecSide)
		{
			adapter.collectRelatedLabelAndShapes(srcShape, EnCommon, shapeBuilder.getBuilder());
		}
		else
		{
			adapter.collectRelatedLabelAndShapes(srcShape, EnCut, shapeBuilder.getBuilder());
		}

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

// 			TheOCCApp.saveAs(TheOCCApp.getCurrentModel(), "d:/text.cbf");

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::stitch(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		TopTools_ListOfShape shapeArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		TransformShapeBuilder tsBuilder;
		for (std::list<void*>::const_iterator ptrIter = entryList.begin(); ptrIter != entryList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

// 			if (foundShape.ShapeType() == TopAbs_SHELL|| foundShape.ShapeType() == TopAbs_FACE)
// 			{
				adapter.labelDiscreteToMap(foundLabel);

				tsBuilder.shapeCopy(foundShape);
				TopoDS_Shape newShape = tsBuilder.getShape();
				adapter.collectRelatedLabelAndShapes(newShape, EnCopy, tsBuilder.getBuilder());
				shapeArray.Append(newShape);
// 			}
// 			else if (foundShape.ShapeType() == TopAbs_SHELL)
// 			{
// 				TopoDS_Shape filterFace;
// 				if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_SHELL, filterFace))
// 				{
// 					COMMAND_END
// 						return false;
// 				}
// 
// 				adapter.labelDiscreteToMap(foundLabel);
// 				adapter.directRelateShapes(foundShape, filterFace);
// 				shapeArray.Append(filterFace);
// 			}
// 			else
// 			{
// 				return false;
// 			}

// 			if (foundShape.ShapeType() == TopAbs_FACE)
// 			{
// 				adapter.labelDiscreteToMap(foundLabel);
// 				shapeArray.Append(foundShape);
// 			}
// 			else if (foundShape.ShapeType() == TopAbs_SHELL)
// 			{
// 				TopoDS_Shape filterFace;
// 				if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_FACE, filterFace))
// 				{
// 					COMMAND_END
// 						return false;
// 				}
// 
// 				adapter.labelDiscreteToMap(foundLabel);
// 				adapter.directRelateShapes(foundShape, filterFace);
// 				shapeArray.Append(filterFace);
// 			}
		}

		//start
// 		TopoDS_Shape cmp;
// 		if (!OCCBasicTools::createCompound(shapeArray, cmp))
// 		{
// 			COMMAND_END
// 			return false;
// 		}
	
		TopoDS_Shape targetShape;
		BooleanShapeBuilder shapeBuilder;
// 		
// 		if (!shapeBuilder.volumeMarker(cmp))
		if (!shapeBuilder.volumeMarker(shapeArray))
		{
			if (!shapeBuilder.shapeSewing(shapeArray, true))
			{
				COMMAND_END
					return false;
			}
			targetShape = shapeBuilder.getShape();
			if (targetShape.IsNull())
			{
				COMMAND_END
				return false;
			}

			// 找联系
			TopTools_ListOfShape::Iterator shapeIter(shapeArray);
			for (; shapeIter.More(); shapeIter.Next())
			{
				adapter.collectRelatedLabelAndShapes(shapeIter.Value(), EnSew1, shapeBuilder.getBuilder());
			}
		}
		else
		{
			targetShape = shapeBuilder.getShape();
			if (targetShape.IsNull())
			{
				COMMAND_END
					return false;
			}
			// 找联系
			TopTools_ListOfShape::Iterator shapeIter(shapeArray);
			for (; shapeIter.More(); shapeIter.Next())
			{
				adapter.collectRelatedLabelAndShapes(shapeIter.Value(), EnSew2, shapeBuilder.getBuilder());
			}

// 			BOPAlgo_MakerVolume* api = (BOPAlgo_MakerVolume*)shapeBuilder.getBuilder();
// 			TopTools_ListOfShape::Iterator shapeIter(shapeArray);
// 			for (; shapeIter.More(); shapeIter.Next())
// 			{
// 				TopTools_ListOfShape modifiedShapes = api->Modified(shapeIter.Value());
// 				int size = modifiedShapes.Size();
// 				bool del = api->IsDeleted(shapeIter.Value());
// 			}

// 			//暂时无法进行历史追踪
// 			targetShape = shapeBuilder.getShape();
// 			if (targetShape.IsNull())
// 			{
// 				COMMAND_END
// 				return false;
// 			}
		}
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

// 			TheOCCApp.saveAs(TheOCCApp.getCurrentModel(), "d:/text.cbf");

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::cover(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		TopTools_ListOfShape shapeArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = entryList.begin(); ptrIter != entryList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			if (foundShape.ShapeType() == TopAbs_WIRE)
			{
				adapter.labelDiscreteToMap(foundLabel);
				shapeArray.Append(foundShape);
			}
			else if (foundShape.ShapeType() < TopAbs_WIRE)
			{
				TopoDS_Shape filterWire;
				if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_WIRE, filterWire))
				{
					COMMAND_END
						return false;
				}

				adapter.labelDiscreteToMap(foundLabel);
				adapter.directRelateShapes(foundShape, filterWire);
				shapeArray.Append(filterWire);
			}
		}

		//start	
		FaceOperationBuilder faceOperator;
		if (!faceOperator.faceCover(shapeArray))
		{
			COMMAND_END
				return false;
		}

		TopoDS_Shape targetShape = faceOperator.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		TopTools_ListOfShape::Iterator shapeIter(shapeArray);
		for (; shapeIter.More(); shapeIter.Next())
		{
			adapter.collectRelatedLabelAndShapes(shapeIter.Value(), EnNormal, faceOperator.getBuilder());
		}

		if (!ShapeUtilities::shapeCover(targetShape, targetShape))
		{
			COMMAND_END
				return false;
		}

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::trimCurve(void* edgeEntry, const double& startT, const double& endT, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		if (!TheOCCApp.getCurrentModel()->findShapePID(edgeEntry, foundPID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
		if (foundLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		foundShape = LabelUtilities::getLabelShape(foundLabel);

		if (foundShape.ShapeType() == TopAbs_EDGE)
		{
			adapter.labelDiscreteToMap(foundLabel);
		}
		else if (foundShape.ShapeType() == TopAbs_WIRE)
		{
			TopoDS_Shape filterShape;
			if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_EDGE, filterShape))
			{
				COMMAND_END
					return false;
			}
			adapter.directRelateShapes(foundShape, filterShape);
			foundShape = filterShape;
		}
		else
		{
			return false;
		}

		//start
		EdgeOperationBuilder esBuilder;
		if (!esBuilder.trimCurve(foundShape, startT, endT))
		{
			COMMAND_END
			return false;
		}
		TopoDS_Shape targetShape = esBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}
		// 找联系
		adapter.directRelateShapes(foundShape, targetShape);

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::edgeExtendLength(void* edgeEntry, double startParam, double length, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		if (!TheOCCApp.getCurrentModel()->findShapePID(edgeEntry, foundPID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
		if (foundLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		foundShape = LabelUtilities::getLabelShape(foundLabel);

		if (foundShape.ShapeType() <= TopAbs_EDGE)
		{
			adapter.labelDiscreteToMap(foundLabel);

			TopoDS_Shape filterShape;
			if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_EDGE, filterShape))
			{
				COMMAND_END
				return false;
			}
			adapter.directRelateShapes(foundShape, filterShape);
			foundShape = filterShape;
		}
		else
		{
			return false;
		}

		//start
		EdgeOperationBuilder eoBuilder;
		if (!eoBuilder.trimCurveAdaptEnd(foundShape, startParam))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape trimmedCurve = eoBuilder.getShape();
		if (trimmedCurve.IsNull())
		{
			return false;
		}
		// 找联系
		adapter.directRelateShapes(foundShape, trimmedCurve);

		if (!eoBuilder.edgeExtendOperation(trimmedCurve, length, 1, true))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape targetShape = eoBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}
		// 找联系
		adapter.directRelateShapes(trimmedCurve, targetShape);


		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::closedEdgeFace(const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		TopTools_ListOfShape shapeArray;
		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		for (std::list<void*>::const_iterator ptrIter = entryList.begin(); ptrIter != entryList.end(); ++ptrIter)
		{
			if (!TheOCCApp.getCurrentModel()->findShapePID(*ptrIter, foundPID))
			{
				COMMAND_END
					return false;
			}
			TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
			if (foundLabel.IsNull())
			{
				COMMAND_END
					return false;
			}
			foundShape = LabelUtilities::getLabelShape(foundLabel);

			if (foundShape.ShapeType() == TopAbs_EDGE)
			{
				adapter.labelDiscreteToMap(foundLabel);
				shapeArray.Append(foundShape);
			}
			else
			{
				return false;
			}
		}

		//start
		//build wire
		EdgeOperationBuilder eoBuilder;
		eoBuilder.edgeClosedWire(shapeArray);
		TopoDS_Shape wire = eoBuilder.getShape();
		if (wire.IsNull())
		{
			return false;
		}
		// 找联系
		TopTools_ListOfShape::Iterator shapeIter(shapeArray);
		for (; shapeIter.More(); shapeIter.Next())
		{
			adapter.collectRelatedLabelAndShapes(shapeIter.Value(), EnNormal, eoBuilder.getBuilder());
		}

		//rebuild wire
		eoBuilder.wireRebuild(TopoDS::Wire(wire));
		TopoDS_Shape rebuildWire = eoBuilder.getShape();
		if (rebuildWire.IsNull())
		{
			return false;
		}
		// 找联系
		adapter.collectRelatedLabelAndShapes(wire, EnNormal, eoBuilder.getBuilder());

		//build face
		TopTools_ListOfShape wireList;
		wireList.Append(rebuildWire);
		FaceOperationBuilder foBuilder;
		foBuilder.faceCover(wireList);
		TopoDS_Shape targetShape = foBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}
		// 找联系
		adapter.collectRelatedLabelAndShapes(rebuildWire, EnNormal, foBuilder.getBuilder());

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}


bool OccModelBuilder::faceOffset(void* entry, const double& distance, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find origin shape
		std::string instancePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel);
		if (origLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);

		//start
		LabelShapeAdapter adapter;
		//preparation
		adapter.labelDiscreteToMap(origLabel);

		TopoDS_Shape targetShape;
		OffsetShapeBuilder shapeBuilder;
		if (!shapeBuilder.faceOffsetOperation(origShape, distance, false))
		{
			COMMAND_END
				return false;
		}

		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		// 找联系
		adapter.collectRelatedLabelAndShapes(origShape, EnThicken, shapeBuilder.getBuilder());

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::faceExtendLength(void* edgeEntry, double length, const bool& inU, const bool& forward, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		if (!TheOCCApp.getCurrentModel()->findShapePID(edgeEntry, foundPID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
		if (foundLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		foundShape = LabelUtilities::getLabelShape(foundLabel);

		if (foundShape.ShapeType() <= TopAbs_FACE)
		{
			adapter.labelDiscreteToMap(foundLabel);

			TopoDS_Shape filterShape;
			if (!ShapeUtilities::shapeFilter(foundShape, TopAbs_FACE, filterShape))
			{
				COMMAND_END
					return false;
			}
			adapter.directRelateShapes(foundShape, filterShape);
			foundShape = filterShape;
		}
		else
		{
			return false;
		}

		//start
		FaceOperationBuilder eoBuilder;
		if (!eoBuilder.faceExtendOperation(foundShape, length, 1, inU, forward))
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape targetShape = eoBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}
		// 找联系
		adapter.directRelateShapes(foundShape, targetShape);


		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::faceIntersection(void* entry, void* entry2, const int& method, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

// 		TopoDS_Edge face1, face2;
		//find origin shape
		std::string instancePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel);
		if (origLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);
// 		if (origShape.ShapeType() <= TopAbs_FACE)
// 		{
// 			if (!ShapeUtilities::shapeFilter(origShape, TopAbs_FACE, face1))
// 			{
// 				COMMAND_END
// 					return false;
// 			}
// 		}
// 		else
// 		{
// 			COMMAND_END
// 				return false;
// 		}

		//find origin shape
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry2, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel2;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel2);
		if (origLabel2.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape2 = LabelUtilities::getLabelShape(origLabel2);
// 		if (origShape2.ShapeType() <= TopAbs_FACE)
// 		{
// 			if (!ShapeUtilities::shapeFilter(origShape2, TopAbs_FACE, face2))
// 			{
// 				COMMAND_END
// 					return false;
// 			}
// 		}
// 		else
// 		{
// 			COMMAND_END
// 				return false;
// 		}

		TopoDS_Shape targetShape;
		FaceOperationBuilder shapeBuilder;
		if (!shapeBuilder.faceIntersectionOperation(origShape, origShape2, method))
		{
			COMMAND_END
				return false;
		}

		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		LabelShapeAdapter adapter;
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::edgeCombination(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		TopoDS_Edge edge1, edge2;
		//find origin shape
		std::string instancePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel);
		if (origLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);
		if (origShape.ShapeType() <= TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeFilter(origShape, TopAbs_EDGE, edge1))
			{
				COMMAND_END
				return false;
			}
		}
		else
		{
			COMMAND_END
			return false;
		}

		//find origin shape
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry2, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel2;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel2);
		if (origLabel2.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape2 = LabelUtilities::getLabelShape(origLabel2);
		if (origShape2.ShapeType() <= TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeFilter(origShape2, TopAbs_EDGE, edge2))
			{
				COMMAND_END
				return false;
			}
		}
		else
		{
			COMMAND_END
			return false;
		}

		//start
		LabelShapeAdapter adapter;
		//preparation
		adapter.labelDiscreteToMap(origLabel);
		adapter.labelDiscreteToMap(origLabel2);
// 		adapter.directRelateShapes(origShape, edge1);
// 		adapter.directRelateShapes(origShape2, edge2);


		TopoDS_Shape targetShape;
		EdgeOperationBuilder shapeBuilder;
		if (!shapeBuilder.edgesCombinationOperation(edge1, edge2, tolerance()))
		{
			COMMAND_END
				return false;
		}

		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}
		adapter.directRelateShapes(origShape, targetShape);

// 		// 找联系
// 		adapter.collectRelatedLabelAndShapes(edge1, EnThicken, shapeBuilder.getBuilder());
// 		adapter.collectRelatedLabelAndShapes(edge2, EnThicken, shapeBuilder.getBuilder());

		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::edgeIntersection(void* entry, void* entry2, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		TopoDS_Edge edge1, edge2;
		//find origin shape
		std::string instancePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel);
		if (origLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape = LabelUtilities::getLabelShape(origLabel);
		if (origShape.ShapeType() <= TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeFilter(origShape, TopAbs_EDGE, edge1))
			{
				COMMAND_END
					return false;
			}
		}
		else
		{
			COMMAND_END
				return false;
		}

		//find origin shape
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry2, instancePID))
		{
			COMMAND_END
				return false;
		}
		TDF_Label origLabel2;
		TDF_Tool::Label(data, instancePID.c_str(), origLabel2);
		if (origLabel2.IsNull())
		{
			COMMAND_END
				return false;
		}
		TopoDS_Shape origShape2 = LabelUtilities::getLabelShape(origLabel2);
		if (origShape2.ShapeType() <= TopAbs_EDGE)
		{
			if (!ShapeUtilities::shapeFilter(origShape2, TopAbs_EDGE, edge2))
			{
				COMMAND_END
					return false;
			}
		}
		else
		{
			COMMAND_END
				return false;
		}

		TopoDS_Shape targetShape;
		EdgeOperationBuilder shapeBuilder;
		if (!shapeBuilder.edgesIntersectionOperation(edge1, edge2, tolerance()))
		{
			COMMAND_END
				return false;
		}

		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			return false;
		}

		LabelShapeAdapter adapter;
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::makeFuzzyBoolean(const BOOLTYPE& boolType, const LOGICAL& isKeep, const std::list<void*>& entryList, const double& fuzzyValue, const char* name, std::string& strPID, void*& resEntity)
{
	if (entryList.size() < 2)
	{
		return false;
	}

	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find entity PID; collect shape and information
		TopTools_ListOfShape args, tools, total;
		LabelShapeAdapter adapter;
		TransformShapeBuilder transformBuilder;
		for (std::list<void*>::const_iterator iter = entryList.begin(); iter != entryList.end(); ++iter)
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

				//copy shape to avoid influence of boolean operation upon origin shape
				if (!transformBuilder.shapeCopy(foundShape))
				{
					return false;
				}
				adapter.labelDiscreteToMap(foundLabel);
				adapter.collectRelatedLabelAndShapes(foundShape, EnCopy, transformBuilder.getBuilder());
				foundShape = transformBuilder.getShape();

				if (iter == entryList.begin())
				{
					args.Append(foundShape);
				}
				else
				{
					tools.Append(foundShape);
				}

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

		BOPAlgo_Operation type;
		EnAdapatType adaptType;
		if (boolType == Substract || boolType == Unite)
		{
			if (boolType == Substract)
			{
				type = BOPAlgo_CUT;
				adaptType = EnCut;
			}
			else
			{
				type = BOPAlgo_FUSE;
				adaptType = EnFuse;
			}

			if (!shapeBuilder.booleanOperation(args, tools, type, fuzzyValue))
			{
				COMMAND_END
					return false;
			}
			targetShape = shapeBuilder.getShape();
			if (targetShape.IsNull())
			{
				COMMAND_END
					return false;
			}

			// 找联系
			TopTools_ListOfShape::Iterator shapeIter(total);
			for (; shapeIter.More(); shapeIter.Next())
			{
				adapter.collectRelatedLabelAndShapes(shapeIter.Value(), adaptType, shapeBuilder.getBuilder());
			}
		}
		else if (boolType == Intersect)
		{
			type = BOPAlgo_COMMON;
			adaptType = EnCommon;

			TopTools_ListOfShape::Iterator shapeIter(total);
			targetShape = shapeIter.Value();
			shapeIter.Next();
			for (; shapeIter.More(); shapeIter.Next())
			{
				if (!shapeBuilder.booleanOperation(targetShape, shapeIter.Value(), type, fuzzyValue))
				{
					return false;
				}
				TopoDS_Shape newShape = shapeBuilder.getShape();
				if (newShape.IsNull())
				{
					return false;
				}

				// 找联系
				adapter.collectRelatedLabelAndShapes(targetShape, adaptType, shapeBuilder.getBuilder());
				adapter.collectRelatedLabelAndShapes(shapeIter.Value(), adaptType, shapeBuilder.getBuilder());

				targetShape = newShape;
			}
		}
// 		else if (boolType == Imprint)
// 		{
// 			TopoDS_Shape mainShape = args.First();
// 			//get main shape faces;
// 			TopTools_ListOfShape::Iterator toolIter(tools);
// 			for (; toolIter.More(); toolIter.Next())
// 			{
// 				TopoDS_Shape curTool = toolIter.Value();
// 
// 				//先用母体的所有face与tool求交，结果可能是线框或者shell
// 				TopoDS_Compound cmp;
// 				BRep_Builder B;
// 				B.MakeCompound(cmp);
// 				TopExp_Explorer exp(mainShape, TopAbs_FACE);
// 				for (; exp.More(); exp.Next())
// 				{
// 					B.Add(cmp, exp.Value());
// 				}
// 
// 				type = BOPAlgo_SECTION;
// 				adaptType = EnSection;
// 				if (!shapeBuilder.booleanOperation(tools.First(), cmp, type, tolerance()))
// 				{
// 					type = BOPAlgo_COMMON;
// 					adaptType = EnCommon;
// 					if (!shapeBuilder.booleanOperation(tools.First(), cmp, type, tolerance()))
// 					{
// 						COMMAND_END
// 							return false;
// 					}
// 				}
// 
// 				TopoDS_Shape commonShape = shapeBuilder.getShape();
// 				if (commonShape.IsNull())
// 				{
// 					COMMAND_END
// 						return false;
// 				}
// 
// 				// 找联系
// 				TopTools_ListOfShape::Iterator shapeIter(tools);
// 				for (; shapeIter.More(); shapeIter.Next())
// 				{
// 					adapter.collectRelatedLabelAndShapes(shapeIter.Value(), adaptType, shapeBuilder.getBuilder());
// 				}
// 				adapter.collectRelatedLabelAndShapes(cmp, adaptType, shapeBuilder.getBuilder());
// 
// 
// 				//do first imprint try
// 				//method3
// 				adaptType = EnNormal;
// 				TopTools_ListOfShape splitTool;
// 				splitTool.Append(commonShape);
// 				if (!shapeBuilder.shapeSplitOperation2(mainShape, splitTool))
// 				{
// 					//do second imprint 
// 					//imprint
// 					//method1
// 					adaptType = EnCellsBuilderFuse;
// 					args.Append(commonShape);
// 					if (!shapeBuilder.cellsBuilderOperation(args, adaptType))
// 					{
// 						COMMAND_END
// 							return false;
// 					}
// 
// 					mainShape = shapeBuilder.getShape();
// 					if (mainShape.IsNull())
// 					{
// 						COMMAND_END
// 							return false;
// 					}
// 
// 					// 找联系
// 					shapeIter.Init(args);
// 					for (; shapeIter.More(); shapeIter.Next())
// 					{
// 						adapter.collectRelatedLabelAndShapes(shapeIter.Value(), adaptType, shapeBuilder.getBuilder());
// 					}
// 				}
// 				else
// 				{
// 					mainShape = shapeBuilder.getShape();
// 					if (mainShape.IsNull())
// 					{
// 						COMMAND_END
// 							return false;
// 					}
// 
// 					// 找联系
// 					shapeIter.Init(args);
// 					for (; shapeIter.More(); shapeIter.Next())
// 					{
// 						adapter.collectRelatedLabelAndShapes(shapeIter.Value(), adaptType, shapeBuilder.getBuilder());
// 					}
// 					adapter.collectRelatedLabelAndShapes(commonShape, adaptType, shapeBuilder.getBuilder());
// 				}
// 			}
// 			targetShape = mainShape;
// 		}
		else
		{
			return false;
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
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		// 		IOShapeBuilder ioBuilder;
		// 		ioBuilder.setShape(targetShape);
		// 		ioBuilder.exportBREPModel("D:\\imprint.brep");

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::makeNonRegBoolean(const EnAdapatType& boolType, const std::list<void*>& entryList, const char* name, std::string& strPID, void*& resEntity)
{
	if (entryList.size() < 2)
	{
		return false;
	}

	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find entity PID; collect shape and information
		TopTools_ListOfShape args, tools, total;
		LabelShapeAdapter adapter;
		TransformShapeBuilder transformBuilder;
		for (std::list<void*>::const_iterator iter = entryList.begin(); iter != entryList.end(); ++iter)
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

				//copy shape to avoid influence of boolean operation upon origin shape
				if (!transformBuilder.shapeCopy(foundShape))
				{
					return false;
				}
				adapter.labelDiscreteToMap(foundLabel);
				adapter.collectRelatedLabelAndShapes(foundShape, EnCopy, transformBuilder.getBuilder());
				foundShape = transformBuilder.getShape();

				if (iter == entryList.begin())
				{
					args.Append(foundShape);
				}
				else
				{
					tools.Append(foundShape);
				}

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
		if (!shapeBuilder.cellsBuilderOperation(total, boolType))
		{
			COMMAND_END
				return false;
		}
		targetShape = shapeBuilder.getShape();
		if (targetShape.IsNull())
		{
			COMMAND_END
				return false;
		}

		// 找联系
		TopTools_ListOfShape::Iterator shapeIter(total);
		for (; shapeIter.More(); shapeIter.Next())
		{
			adapter.collectRelatedLabelAndShapes(shapeIter.Value(), boolType, shapeBuilder.getBuilder());
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
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		// 		IOShapeBuilder ioBuilder;
		// 		ioBuilder.setShape(targetShape);
		// 		ioBuilder.exportBREPModel("D:\\imprint.brep");

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::createNurbsFace(const std::vector<std::vector<std::vector<double>>>& poles, const std::vector<std::vector<double>>& weight, const std::vector<double> &uValue, const std::vector<double>& vValue, const std::vector<int> &uMul, const std::vector<int>& vMul, const int& uDeg, const int& vDeg, const char* name, std::string& strPID, void*& resEntity)
{
	COMMAND_BEGIN

		try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		// 		//parameters transformation
		// 		double xAxis, yAxis, zAxis;
		// 		ShapeUtilities::getAxis(axis, xAxis, yAxis, zAxis);

		TColgp_Array2OfPnt PoleArray(0, poles.size() - 1, 0, poles[0].size() - 1);
		TColStd_Array2OfReal Weights(0, weight.size() - 1, 0, weight[0].size() - 1);
		TColStd_Array1OfReal UKnots(0, uValue.size() - 1), VKnots(0, vValue.size() - 1);
		TColStd_Array1OfInteger UMults(0, uMul.size() - 1), VMults(0, vMul.size() - 1);
		// 		Standard_Integer UDegree, VDegree;

		for (int i = 0; i < poles.size(); ++i)
		{
			for (int j = 0; j < poles[0].size(); ++j)
			{
				gp_Pnt pnt(poles[i][j][0], poles[i][j][1], poles[i][j][2]);
				PoleArray.SetValue(i, j, pnt);
				Weights.SetValue(i, j, weight[i][j]);
			}
		}

		for (int i = 0; i < uValue.size(); ++i)
		{
			UKnots.SetValue(i, uValue[i]);
		}

		for (int i = 0; i < vValue.size(); ++i)
		{
			VKnots.SetValue(i, vValue[i]);
		}

		for (int i = 0; i < uMul.size(); ++i)
		{
			UMults.SetValue(i, uMul[i]);
		}

		for (int i = 0; i < vMul.size(); ++i)
		{
			VMults.SetValue(i, vMul[i]);
		}

		//create shape
		FaceShapeBuilder shapeBuilder;
		if (!shapeBuilder.createNurbsSurface(PoleArray, Weights, UKnots, VKnots, UMults, VMults, uDeg, vDeg))
		{
			COMMAND_END

				return false;
		}

		TopoDS_Shape targetShape = shapeBuilder.getShape();
		ShapeUtilities::shapeCover(targetShape, targetShape);

		// 		targetShape = ShapeUtilities::shapeTransformFromLocalToGlobal(wcsMatrix, targetShape);

				//create label
		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		TCollection_AsciiString anEntry;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();

		//create child label
		LabelShapeAdapter adapter;
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::shapeFix(void* entry, const double& precision, const double& minTol, const double& maxTol, const EnFixType& fixType, const char* name, std::string& strPID, void*& resEntity, std::string& message)
{
	COMMAND_BEGIN

	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		//find shape and label
		LabelShapeAdapter adapter;

		std::string foundPID;
		TDF_Label foundLabel;
		TopoDS_Shape foundShape;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, foundPID))
		{
			COMMAND_END
				return false;
		}
		TDF_Tool::Label(data, foundPID.c_str(), foundLabel);
		if (foundLabel.IsNull())
		{
			COMMAND_END
				return false;
		}
		foundShape = LabelUtilities::getLabelShape(foundLabel);

		//copy
		TransformShapeBuilder tsBuilder;
		tsBuilder.shapeCopy(foundShape);
		adapter.collectRelatedLabelAndShapes(foundShape, EnCopy, tsBuilder.getBuilder());
		TopoDS_Shape copiedShape = tsBuilder.getShape();

		//start
		OCCFixToolInstance->setPrecision(precision);
		OCCFixToolInstance->setMinTol(minTol);
		OCCFixToolInstance->setMaxTol(maxTol);

		message += "Analysis before fixing:\n\n";
		OCCFixToolInstance->shapeValidityAnalysis(copiedShape, copiedShape.ShapeType(), message);
		message += "\n";

		switch (fixType)
		{
		case EnFixWhole:
		{
			OCCFixToolInstance->fixTopoValidityOp(copiedShape, true);
			break;
		}
		case EnFixOrder:
		{
			OCCFixToolInstance->FixShellOrientationOp(copiedShape);
			OCCFixToolInstance->FixFaceOrientationOp(copiedShape);
			OCCFixToolInstance->FixWireOrientationOp(copiedShape);
			OCCFixToolInstance->FixEdgeOrientationOp(copiedShape);

			break;
		}
		case EnFixEdgeConnect:
		{
			OCCFixToolInstance->FixEdgeConnection(copiedShape);
			OCCFixToolInstance->FixWire(copiedShape);
			break;
		}
		case EnFixFaceConnect:
		{
			OCCFixToolInstance->FixFaceConnection(copiedShape);
			break;
		}
		default:
			break;
		}

		message += "Analysis after fixing:\n\n";
		OCCFixToolInstance->shapeValidityAnalysis(copiedShape, copiedShape.ShapeType(), message);
		message += "\n";

		TopoDS_Shape targetShape = copiedShape;
		if (targetShape.IsNull())
		{
			return false;
		}
		// 找联系
// 		adapter.directRelateShapes(foundShape, targetShape);


		TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
		adapter.setTopLabel(targetLabel);
		adapter.allocateLabelWithTopShape(targetLabel, targetShape);
// 		adapter.linkRelatedLabels(targetLabel);

		// some infos for return;
		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(targetLabel, anEntry);
		strPID = anEntry.ToCString();
		TDataStd_AsciiString::Set(targetLabel, "top_level");

		TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, resEntity);

		COMMAND_END

			return true;
	}
	catch (...)
	{
		COMMAND_END

			return false;
	}
}

bool OccModelBuilder::importModel(const std::string& path, const char* name, const EnFileType& type, std::string& strPID)
{
	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

        IOShapeBuilder ioBuilder;
		switch (type)
		{
		case ENIGS:
		{
            if (!ioBuilder.importIGESModel(path))
            {
                return false;
            }
			break;
		}
		case ENSTP:
		{
            if (!ioBuilder.importSTEPModel(path))
            {
                return false;
            }
            break;
		}
		case ENBREP:
		{
            if (!ioBuilder.importBREPModel(path))
            {
                return false;
            }
            break;
		}
		default:
			break;
		}

        TopoDS_Shape ResultShape = ioBuilder.getShape();
		if (ResultShape.IsNull())
		{
			return false;
		}

		TDF_Label inModelLabel = LabelUtilities::createNewChild(rootLabel);
        LabelShapeAdapter adapter;
        adapter.setTopLabel(inModelLabel);
        adapter.allocateLabelWithTopShape(inModelLabel, ResultShape);
		TDataStd_AsciiString::Set(inModelLabel, "top_level");

		TCollection_AsciiString anEntry;
		TColStd_ListOfInteger aTagList;
		TDF_Tool::Entry(inModelLabel, anEntry);
		strPID = anEntry.ToCString();

        void* entity;
        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::exportModel(const std::string& path, const EnFileType& type, const std::string& entry)
{
	try
	{
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		TDF_Label label;
		TDF_Tool::Label(data, TCollection_AsciiString(entry.c_str()), label, false);
		Handle(TNaming_NamedShape) nameShape;
		TopoDS_Shape shape;
		if (label.FindAttribute(TNaming_NamedShape::GetID(), nameShape))
		{
			shape = nameShape->Get();
			if (shape.IsNull())
				return false;
		}
		else
		{
			return false;
		}
		
        IOShapeBuilder ioBuilder;
        ioBuilder.setShape(shape);
        switch (type)
		{
		case ENIGS:
		{
            if (!ioBuilder.exportIGESModel(path))
            {
                return false;
            }
            break;
		}
		case ENSTP:
		{
            if (!ioBuilder.exportSTEPModel(path))
            {
                return false;
            }
			break;
		}
		case ENBREP:
		{
            if (!ioBuilder.exportBREPModel(path))
            {
                return false;
            }
            break;
		}
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

bool OccModelBuilder::deleteLabel(const std::string& entry)
{
    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();

        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(entry.c_str()), label, false);

        if (!label.IsNull())
        {
            label.ForgetAllAttributes(true);
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool OccModelBuilder::exportEntityList(std::vector<void *>entList, std::string& path)
{
	try
	{
		TopoDS_Compound cmp;
		BRep_Builder cmpBuilder;
		cmpBuilder.MakeCompound(cmp);
		for (int i = 0; i < entList.size(); ++i)
		{
			xdTopoDS_HShape* pCurShape = (xdTopoDS_HShape*)entList[i];
			cmpBuilder.Add(cmp, pCurShape->Shape());
		}
		BRepTools::Write(cmp, path.c_str());
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

/************************************************************************/
/*                         protected                                    */
/************************************************************************/
bool OccModelBuilder::createBooleanCut(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& entity)
{
    //     try
    //     {
    //         TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
    //         Handle(TDF_Data) data = curDoc->GetData();
    //         TDF_Label rootLabel = data->Root();
    // 
    //         // start
    //         if (entryList.size() < 2)
    //         {
    //             return false;
    //         }
    //         //get main label
    //         std::vector<std::string>::const_iterator iter = entryList.begin();
    //         TDF_Label mainLabel;
    //         TDF_Tool::Label(data, TCollection_AsciiString((*iter).c_str()), mainLabel, false);
    //         TopoDS_Shape mainShape = LabelUtilities::getLabelShape(mainLabel);
    //         if (mainShape.IsNull())
    //         {
    //             return false;
    //         }
    // 
    //         ++iter;
    // 
    //         //start operation
    //         LabelShapeAdapter adapter;
    //         //preparation
    //         adapter.labelDiscreteToMap(mainLabel);
    //         TransformShapeBuilder transformBuilder;
    //         if (!transformBuilder.shapeCopy(mainShape))
    //         {
    //             return false;
    //         }
    //         adapter.collectRelatedLabelAndShapes(mainShape, EnCopy, transformBuilder.getBuilder());
    //         mainShape = transformBuilder.getShape();
    // 
    //         BooleanShapeBuilder shapeBuilder;
    // //         TDF_Label instanceMainLabel = mainLabel;
    //         for (; iter != entryList.end(); ++iter)
    //         {
    //             TDF_Label toolLabel;
    //             TDF_Tool::Label(data, TCollection_AsciiString((*iter).c_str()), toolLabel, false);
    //             if (toolLabel.IsNull())
    //             {
    //                 continue;
    //             }
    // 
    //             //copy tool shape
    //             TopoDS_Shape toolShape = LabelUtilities::getLabelShape(toolLabel);
    //             if (toolShape.IsNull())
    //             {
    //                 return false;
    //             }
    //             adapter.labelDiscreteToMap(toolLabel);
    //             if (!transformBuilder.shapeCopy(toolShape))
    //             {
    //                 return false;
    //             }
    //             adapter.collectRelatedLabelAndShapes(toolShape, EnCopy, transformBuilder.getBuilder());
    //             toolShape = transformBuilder.getShape();
    // 
    //             if (!shapeBuilder.singleCutOperation(mainShape, toolShape))
    //             {
    //                 return false;
    //             }
    //             TopoDS_Shape newShape = shapeBuilder.getShape();
    //             //                 newShape = ShapeUtilities::shapeFilter(newShape);
    //             if (newShape.IsNull())
    //             {
    //                 return false;
    //             }
    // 
    //             // 找联系
    //             adapter.collectRelatedLabelAndShapes(mainShape, EnCut, shapeBuilder.getBuilder());
    //             adapter.collectRelatedLabelAndShapes(toolShape, EnCut, shapeBuilder.getBuilder());
    //             mainShape = newShape;
    //         }
    // 
    //         // 根据布尔运算后的模型自动分配子节点到各个label
    //         TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
    //         TopoDS_Shape targetShape = shapeBuilder.getShape();
    //         adapter.setTopLabel(targetLabel);
    //         adapter.allocateLabelWithTopShape(targetLabel, targetShape);
    //         adapter.linkRelatedLabels(targetLabel);
    // 
    //         // some infos for return;
    //         TCollection_AsciiString anEntry;
    //         TColStd_ListOfInteger aTagList;
    //         TDF_Tool::Entry(targetLabel, anEntry);
    //         strPID = anEntry.ToCString();
    //         TDataStd_AsciiString::Set(targetLabel, "top_level");
    // 
    //         TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
    // //         TheOCCApp.getApp()->SaveAs(curDoc, "C:\\Users\\Administrator\\Desktop\\testBoolean.xml");
    // 
    //         return true;
    //     }
    //     catch (...)
    //     {
    //         return false;
    //     }

    return false;

}

bool OccModelBuilder::createBooleanCommon(const std::string& mainEntry, const std::string& toolEntry, const char* name, std::string& strPID, void*& entity)
{
//     try
//     {
//         TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
//         Handle(TDF_Data) data = curDoc->GetData();
//         TDF_Label rootLabel = data->Root();
// 
//         //get main label
//         TDF_Label mainLabel;
//         TDF_Tool::Label(data, TCollection_AsciiString(mainEntry.c_str()), mainLabel, false);
//         TopoDS_Shape mainShape = LabelUtilities::getLabelShape(mainLabel);
//         if (mainShape.IsNull())
//         {
//             return false;
//         }
// 
//         TDF_Label toolLabel;
//         TDF_Tool::Label(data, TCollection_AsciiString(toolEntry.c_str()), toolLabel, false);
//         if (toolLabel.IsNull())
//         {
//             return false;
//         }
//         TopoDS_Shape toolShape = LabelUtilities::getLabelShape(toolLabel);
//         if (toolShape.IsNull())
//         {
//             return false;
//         }
// 
//         //start operation
//         LabelShapeAdapter adapter;
//         //preparation
//         adapter.labelDiscreteToMap(mainLabel);
//         TransformShapeBuilder transformBuilder;
//         if (!transformBuilder.shapeCopy(mainShape))
//         {
//             return false;
//         }
//         adapter.collectRelatedLabelAndShapes(mainShape, EnCopy, transformBuilder.getBuilder());
//         mainShape = transformBuilder.getShape();
// 
//         //copy tool shape
//         adapter.labelDiscreteToMap(toolLabel);
//         if (!transformBuilder.shapeCopy(toolShape))
//         {
//             return false;
//         }
//         adapter.collectRelatedLabelAndShapes(toolShape, EnCopy, transformBuilder.getBuilder());
//         toolShape = transformBuilder.getShape();
// 
// 
//         BooleanShapeBuilder shapeBuilder;
//         if (!shapeBuilder.singleCommonOperation(mainShape, toolShape))
//         {
//             return false;
//         }
//         TopoDS_Shape newShape = shapeBuilder.getShape();
//         if (newShape.IsNull())
//         {
//             return false;
//         }
// 
//         // 找联系
//         adapter.collectRelatedLabelAndShapes(mainShape, EnCommon, shapeBuilder.getBuilder());
//         adapter.collectRelatedLabelAndShapes(toolShape, EnCommon, shapeBuilder.getBuilder());
// 
//         // 根据布尔运算后的模型自动分配子节点到各个label
//         TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
//         TopoDS_Shape targetShape = shapeBuilder.getShape();
//         adapter.setTopLabel(targetLabel);
//         adapter.allocateLabelWithTopShape(targetLabel, targetShape);
//         adapter.linkRelatedLabels(targetLabel);
// 
//         // some infos for return;
//         TCollection_AsciiString anEntry;
//         TColStd_ListOfInteger aTagList;
//         TDF_Tool::Entry(targetLabel, anEntry);
//         strPID = anEntry.ToCString();
//         TDataStd_AsciiString::Set(targetLabel, "top_level");
//         TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
// //         TheOCCApp.getApp()->SaveAs(curDoc, "C:\\Users\\Administrator\\Desktop\\testBoolean.xml");
// 
//         return true;
//     }
//     catch (...)
//     {
//         return false;
//     }

    return true;
}

bool OccModelBuilder::createBooleanFuse(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& entity)
{
     try
     {
         TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
         Handle(TDF_Data) data = curDoc->GetData();
         TDF_Label rootLabel = data->Root();
 
         // start
         if (entryList.size() < 2)
         {
             return false;
         }
         //get main label
         std::vector<std::string>::const_iterator iter = entryList.begin();
         TDF_Label mainLabel;
         TDF_Tool::Label(data, TCollection_AsciiString((*iter).c_str()), mainLabel, false);
         TopoDS_Shape mainShape = LabelUtilities::getLabelShape(mainLabel);
         if (mainShape.IsNull())
         {
             return false;
         }
 
         ++iter;
 
         //start operation
         LabelShapeAdapter adapter;
         //preparation
         adapter.labelDiscreteToMap(mainLabel);
         TransformShapeBuilder transformBuilder;
         if (!transformBuilder.shapeCopy(mainShape))
         {
             return false;
         }
         adapter.collectRelatedLabelAndShapes(mainShape, EnCopy, transformBuilder.getBuilder());
         mainShape = transformBuilder.getShape();
 
         BooleanShapeBuilder shapeBuilder;
         //         TDF_Label instanceMainLabel = mainLabel;
         for (; iter != entryList.end(); ++iter)
         {
             TDF_Label toolLabel;
             TDF_Tool::Label(data, TCollection_AsciiString((*iter).c_str()), toolLabel, false);
             if (toolLabel.IsNull())
             {
                 continue;
             }
 
             //copy tool shape
             TopoDS_Shape toolShape = LabelUtilities::getLabelShape(toolLabel);
             if (toolShape.IsNull())
             {
                 return false;
             }
             adapter.labelDiscreteToMap(toolLabel);
             if (!transformBuilder.shapeCopy(toolShape))
             {
                 return false;
             }
             adapter.collectRelatedLabelAndShapes(toolShape, EnCopy, transformBuilder.getBuilder());
             toolShape = transformBuilder.getShape();
 
             if (!shapeBuilder.singleFuseOperation(mainShape, toolShape))
             {
                 return false;
             }
             TopoDS_Shape newShape = shapeBuilder.getShape();
             //                 newShape = ShapeUtilities::shapeFilter(newShape);
             if (newShape.IsNull())
             {
                 return false;
             }
 
             // 找联系
             adapter.collectRelatedLabelAndShapes(mainShape, EnFuse, shapeBuilder.getBuilder());
             adapter.collectRelatedLabelAndShapes(toolShape, EnFuse, shapeBuilder.getBuilder());
             mainShape = newShape;
         }
 
         // 根据布尔运算后的模型自动分配子节点到各个label
         TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
         TopoDS_Shape targetShape = shapeBuilder.getShape();
         adapter.setTopLabel(targetLabel);
         adapter.allocateLabelWithTopShape(targetLabel, targetShape);
         adapter.linkRelatedLabels(targetLabel);
 
         // some infos for return;
         TCollection_AsciiString anEntry;
         TColStd_ListOfInteger aTagList;
         TDF_Tool::Entry(targetLabel, anEntry);
         strPID = anEntry.ToCString();
         TDataStd_AsciiString::Set(targetLabel, "top_level");
 
         TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
 
 //         TheOCCApp.getApp()->SaveAs(curDoc, "C:\\Users\\Administrator\\Desktop\\testBoolean.xml");
 
         return true;
     }
     catch (...)
     {
         return false;
     }
    return true;
}

bool OccModelBuilder::createBooleanNonReg(const std::vector<std::string>& entryList, const char* name, std::string& strPID, void*& entity)
{
    try
    {
        TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
        Handle(TDF_Data) data = curDoc->GetData();
        TDF_Label rootLabel = data->Root();

        // start
        if (entryList.size() < 2)
        {
            return false;
        }
        //get main label
        std::vector<std::string>::const_iterator iter = entryList.begin();
        TDF_Label mainLabel;
        TDF_Tool::Label(data, TCollection_AsciiString((*iter).c_str()), mainLabel, false);
        Handle(TNaming_NamedShape) mainNamedShape;
        TopoDS_Shape mainShape;
        if (mainLabel.FindAttribute(TNaming_NamedShape::GetID(), mainNamedShape))
        {
            mainShape = mainNamedShape->Get();
            if (mainShape.IsNull())
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        ++iter;

        // try to find label with same name 

        //start operation
//         TDF_Label oldLabel;
//         LabelUtilities::findNameLabel(rootLabel, name, oldLabel);
        TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);;
        LabelUtilities::copyLabelWithOnlyPropertyChild(mainLabel, targetLabel);

        BooleanShapeBuilder shapeBuilder;
        LabelShapeAdapter adapter;
//         adapter.setName(name);
        for (; iter != entryList.end(); ++iter)
        {
            TDF_Label toolLabel;
            TDF_Tool::Label(data, TCollection_AsciiString((*iter).c_str()), toolLabel, false);
            if (toolLabel.IsNull())
            {
                continue;
            }

            Handle(TNaming_NamedShape) toolNamedShape;
            if (toolLabel.FindAttribute(TNaming_NamedShape::GetID(), toolNamedShape))
            {
                TopoDS_Shape toolShape = toolNamedShape->Get();
                if (toolShape.IsNull())
                {
                    targetLabel.ForgetAllAttributes();
                    return false;
                }

                if (!shapeBuilder.singleNonRegFuseOperation(mainShape, toolShape))
                {
                    targetLabel.ForgetAllAttributes();
                    return false;
                }
                TopoDS_Shape newShape = shapeBuilder.getShape();

                adapter.clearRelatedShapes();
                adapter.addModifiedShapes(newShape, targetLabel);
                adapter.collectLabelShapes(mainLabel, EnCellsBuilderCut, shapeBuilder.getBuilder());
                adapter.collectLabelShapes(toolLabel, EnCellsBuilderCut, shapeBuilder.getBuilder());

                //allocate
                adapter.setTopLabel(targetLabel);
                adapter.allocateLabelWithTopShape(targetLabel, newShape);
                mainLabel = targetLabel;
            }
            else
            {
                targetLabel.ForgetAllAttributes();
                return false;
            }
        }

        //
        //         TDF_Label targetLabel = TDF_TagSource::NewChild(rootLabel);
        //         LabelUtilities::copyLabelWithFather(mainLabel, targetLabel);
        TDataStd_AsciiString::Set(targetLabel, "top_level");
//         TDataStd_Name::Set(targetLabel, name);
        if (targetLabel.FindAttribute(TNaming_NamedShape::GetID(), mainNamedShape))
        {
            mainShape = mainNamedShape->Get();
            if (mainShape.IsNull())
            {
                targetLabel.ForgetAllAttributes();
                return false;
            }
            LabelShapeAdapter::replaceChildLabelWithCopiedShape2(targetLabel, mainShape);
        }
        else
        {
            targetLabel.ForgetAllAttributes();
            return false;
        }

        // some infos for return;
        LabelUtilities::recursiveRenameChildLabelWithSameName(targetLabel);
        TCollection_AsciiString anEntry;
        TColStd_ListOfInteger aTagList;
        TDF_Tool::Entry(targetLabel, anEntry);
        strPID = anEntry.ToCString();
//         if (!oldLabel.IsNull())
//         {
//             oldLabel.ForgetAllAttributes();
//         }

        TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
        TheOCCApp.getApp()->SaveAs(curDoc, "C:\\Users\\Administrator\\Desktop\\testBoolean.xml");

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool OccModelBuilder::insertHardPoints(void* entry, void* edge, std::vector<cmtPoint> points, const char* name,
	std::string& strPID, void*& entity)
{
	try
	{
		if (!entry || !edge || points.size() < 1)
			return false;
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		LabelShapeAdapter adapter;
		TDF_Label shapelabel;
		std::string shapePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, shapePID))
			return false;
		TDF_Tool::Label(data, shapePID.c_str(), shapelabel);
		TopoDS_Shape ownershape = LabelUtilities::getLabelShape(shapelabel);
		adapter.labelDiscreteToMap(shapelabel);

		//copy owner
		TransformShapeBuilder tsbuilder;
		tsbuilder.shapeCopy(ownershape);
		adapter.collectRelatedLabelAndShapes(ownershape, EnCopy, tsbuilder.getBuilder());
		ownershape = tsbuilder.getShape();

		//find copy edge
		TopoDS_Shape orgEdge = ((TopoDS_HShape*)edge)->Shape();
		TopoDS_Shape copyEdge;
		if (orgEdge.ShapeType() != TopAbs_EDGE)
			return false;
		BRepBuilderAPI_MakeShape* api = (BRepBuilderAPI_MakeShape*)tsbuilder.getBuilder();
		TopTools_ListOfShape modifiedshapes = api->Modified(orgEdge);
		TopTools_ListIteratorOfListOfShape shapesiter(modifiedshapes);
		for (; shapesiter.More(); shapesiter.Next())
			copyEdge = shapesiter.Value();

		//divide
		double uvalue = 0.0, normalizeu = 0.0, startu, endu;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(TopoDS::Edge(copyEdge), startu, endu);

		xdShapeUpgrade_ShapeDivideEdge shapedivide(ownershape);
		std::vector<double> normTVec;
		for (int i = 0; i < points.size(); i++)
		{
			gp_Pnt point = gp_Pnt(points[i].x(), points[i].y(), points[i].z());
			ShapeUtilities::GetPntUonEdge(TopoDS::Edge(copyEdge), point, uvalue);
			normalizeu = (uvalue - startu) / (endu - startu);
			normTVec.push_back(normalizeu);
		}
		std::sort(normTVec.begin(), normTVec.end());
		shapedivide.add
	}
	catch (...)
	{

	}
}

bool OccModelBuilder::splitWireBypoint(void* entry, std::vector<double> insertTs, const char* name, std::string& strPID, void*& entity)
{
	try
	{
		if (!entry)
			return false;
		TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
		Handle(TDF_Data) data = curDoc->GetData();
		TDF_Label rootLabel = data->Root();

		LabelShapeAdapter adapter;
		TDF_Label shapelabel;
		std::string shapePID;
		if (!TheOCCApp.getCurrentModel()->findShapePID(entry, shapePID))
			return false;
		TDF_Tool::Label(data, shapePID.c_str(), shapelabel);
		TopoDS_Shape wireshape = LabelUtilities::getLabelShape(shapelabel);

		//solid level
		TDF_Label solidLabel;
		solidLabel = shapelabel.Father

		if (wireshape.ShapeType() != TopAbs_WIRE && wireshape.ShapeType() != TopAbs_EDGE)
			return false;
		TopoDS_Edge operateedge;
		if (wireshape.ShapeType() == TopAbs_WIRE)
		{
			TopExp_Explorer edgeex;
			for (edgeex.Init(wireshape, TopAbs_EDGE); edgeex.More(); edgeex.Next())
			{
				operateedge = TopoDS::Edge(edgeex.Current());
			}
		}
		else
		{
			operateedge = TopoDS::Edge(wireshape);
		}
		BRepAdaptor_Curve curve(operateedge);
		std::vector<gp_Pnt> pntlist;
		gp_Pnt pt1 = curve.Value(curve.FirstParameter());
		pntlist.push_back(pt1);
		for (int i = 0; i < insertTs.size(); i++)
		{
			gp_Pnt ptx = curve.Value(insertTs[i]);
			pntlist.push_back(ptx);
		}
		gp_Pnt pt2 = curve.Value(curve.LastParameter());
		pntlist.push_back(pt2);

		BRepBuilderAPI_MakeWire mkwire;
		for (int i = 0; i < pntlist.size() - 1; i++)
		{
			TopoDS_Edge egx = BRepBuilderAPI_MakeEdge(pntlist[i], pntlist[i + 1]).Edge();
			mkwire.Add(egx);
		}

		TopoDS_Wire newwire = mkwire.Wire();

		BRepTools_ReShape brs;
		brs.Replace(wireshape, newwire);
		brs.Apply(ownershape);
		ownershape = brs.Value(ownershape);

		// 根据布尔运算后的模型自动分配子节点到各个label
	    TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
	    adapter.setTopLabel(targetLabel);
	    adapter.allocateLabelWithTopShape(targetLabel, ownershape);
	    adapter.linkRelatedLabels(targetLabel);
	 
	    // some infos for return;
	    TCollection_AsciiString anEntry;
	    TColStd_ListOfInteger aTagList;
	    TDF_Tool::Entry(targetLabel, anEntry);
	    strPID = anEntry.ToCString();
	    TDataStd_AsciiString::Set(targetLabel, "top_level");
	 
	    TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
	}
	catch (...)
	{
		return false;
	}
}

bool OccModelBuilder::splitFaceByEdge(void* entry, SplitLineCreationInfos infos, const char* name,
	std::string& strPID, void*& entity)
{
	if (!entry)
		return false;
	TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
	Handle(TDF_Data) data = curDoc->GetData();
	TDF_Label rootLabel = data->Root();

	LabelShapeAdapter adapter;
	TDF_Label shapelabel;
	std::string shapePID;
	if (!TheOCCApp.getCurrentModel()->findShapePID(entry, shapePID))
		return false;
	TDF_Tool::Label(data, shapePID.c_str(), shapelabel);
	TopoDS_Shape faceshape = LabelUtilities::getLabelShape(shapelabel);

	//0:1找不到shape
	//TDF_Label ownerlabel = LabelUtilities::getOwnerLabel(shapelabel);
	TDF_Label fatherlabel;
	fatherlabel = shapelabel.Father();
	fatherlabel = fatherlabel.Father();//往上找两层
	TCollection_AsciiString checklabelnum;//查看label的编号方法
	TDF_Tool::Entry(fatherlabel, checklabelnum);
	TopoDS_Shape ownershape = LabelUtilities::getLabelShape(fatherlabel);
	adapter.labelDiscreteToMap(fatherlabel);

	TopoDS_Edge divideedge;
	TopoDS_Wire dividewire;
	if (infos.twopointtype == true)
	{
		TopoDS_HShape* voidpt1 = static_cast<TopoDS_HShape*>(infos.pt1);
		TopoDS_Shape vtx1 = voidpt1->Shape();
		TopoDS_HShape* voidpt2 = static_cast<TopoDS_HShape*>(infos.pt2);
		TopoDS_Shape vtx2 = voidpt2->Shape();
		divideedge = BRepBuilderAPI_MakeEdge(TopoDS::Vertex(vtx1), TopoDS::Vertex(vtx2)).Edge();
	}
	else if (infos.pointedgetype == true)
	{
		TopoDS_HShape* voidpt = static_cast<TopoDS_HShape*>(infos.pt3);
		TopoDS_Shape vtx = voidpt->Shape();
		TopoDS_HShape* voidedge = static_cast<TopoDS_HShape*>(infos.edge);
		TopoDS_Shape edge = voidedge->Shape();

		gp_Pnt point1 = BRep_Tool::Pnt(TopoDS::Vertex(vtx));
		BRepAdaptor_Curve adcurve(TopoDS::Edge(edge));
		Handle(Geom_Curve) gecurve = adcurve.Curve().Curve();
		GeomAPI_ProjectPointOnCurve gpp(point1, gecurve);
		if (gpp.NbPoints() < 1)
			return false;
		gp_Pnt point2 = gpp.Point(1);
		divideedge = BRepBuilderAPI_MakeEdge(point1, point2).Edge();
	}
	dividewire = BRepBuilderAPI_MakeWire(divideedge).Wire();

	//makeface添加线的方法，如果完全贯穿面的话，面会被分割，且只保留一边部分
	//BRepBuilderAPI_MakeFace mkface(TopoDS::Face(faceshape));
	//mkface.Add(dividewire);
	//TopoDS_Face newface = mkface.Face();
	BOPAlgo_CellsBuilder cellbd;
	cellbd.AddArgument(faceshape);
	cellbd.AddArgument(divideedge);
	cellbd.Perform();
	cellbd.AddAllToResult();
	TopoDS_Shape cellshape = cellbd.Shape();
	BRep_Builder b;
	TopoDS_Shell shell;
	b.MakeShell(shell);
	TopExp_Explorer faceex(cellshape,TopAbs_FACE);
	for (; faceex.More(); faceex.Next())
	{
		b.Add(shell, TopoDS::Face(faceex.Current()));
	}

	BRepTools_ReShape brs;
	brs.Replace(faceshape, shell);
	brs.Apply(ownershape);
	//下一步必须进行，否则ownershape还是原来的样子
	ownershape = brs.Value(ownershape);

	// 根据布尔运算后的模型自动分配子节点到各个label
	TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
	adapter.setTopLabel(targetLabel);
	adapter.allocateLabelWithTopShape(targetLabel, ownershape);
	adapter.linkRelatedLabels(targetLabel);

	// some infos for return;
	TCollection_AsciiString anEntry;
	TColStd_ListOfInteger aTagList;
	TDF_Tool::Entry(targetLabel, anEntry);
	strPID = anEntry.ToCString();
	TDataStd_AsciiString::Set(targetLabel, "top_level");

	TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
}

bool OccModelBuilder::splitBodyByFace(void* entry, SplitFaceCreationInfos infos, const char* name,
	std::string& strPID, void*& entity)
{
	if (!entry)
		return false;
	TDocStd_Document* curDoc = TheOCCApp.getCurrentModel()->getDoc();
	Handle(TDF_Data) data = curDoc->GetData();
	TDF_Label rootLabel = data->Root();

	LabelShapeAdapter adapter;
	TDF_Label shapelabel;
	std::string shapePID;
	if (!TheOCCApp.getCurrentModel()->findShapePID(entry, shapePID))
		return false;
	TDF_Tool::Label(data, shapePID.c_str(), shapelabel);
	TopoDS_Shape bodyshape = LabelUtilities::getLabelShape(shapelabel);

	TDF_Label ownerlabel = LabelUtilities::getOwnerLabel(shapelabel);
	TopoDS_Shape ownershape = LabelUtilities::getLabelShape(ownerlabel);
	adapter.labelDiscreteToMap(ownerlabel);

	gp_Pln plane;
	TopoDS_Face divideface;
	TopoDS_Wire dividewire;
	if (infos.threepointtype == true)
	{
		TopoDS_HShape* voidpt1 = static_cast<TopoDS_HShape*>(infos.pt1);
		TopoDS_Shape vtx1 = voidpt1->Shape();
		gp_Pnt pnt1 = BRep_Tool::Pnt(TopoDS::Vertex(vtx1));
		TopoDS_HShape* voidpt2 = static_cast<TopoDS_HShape*>(infos.pt2);
		TopoDS_Shape vtx2 = voidpt2->Shape();
		gp_Pnt pnt2 = BRep_Tool::Pnt(TopoDS::Vertex(vtx2));
		TopoDS_HShape* voidpt3 = static_cast<TopoDS_HShape*>(infos.pt3);
		TopoDS_Shape vtx3 = voidpt3->Shape();
		gp_Pnt pnt3 = BRep_Tool::Pnt(TopoDS::Vertex(vtx3));
		gce_MakePln mkpln(pnt1, pnt2, pnt3);
		plane = mkpln.Value();
	}
	else if (infos.pointdirtype == true)
	{
		TopoDS_HShape* voidpt = static_cast<TopoDS_HShape*>(infos.pt3);
		TopoDS_Shape vtx = voidpt->Shape();
		gp_Pnt pnt = BRep_Tool::Pnt(TopoDS::Vertex(vtx));
		gp_Dir dir(infos.dir[0], infos.dir[1], infos.dir[2]);

		gce_MakePln mkpln(pnt, dir);
		plane = mkpln.Value();
	}
	else if (infos.isxoy)
	{
		gce_MakePln mkpln(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
		plane = mkpln.Value();
	}
	else if (infos.isxoz)
	{
		gce_MakePln mkpln(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0));
		plane = mkpln.Value();
	}
	else if (infos.isyoz)
	{
		gce_MakePln mkpln(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0));
		plane = mkpln.Value();
	}
	divideface = BRepBuilderAPI_MakeFace(plane).Face();

	//非正则布尔运算
	TopoDS_CompSolid coms;
	BRep_Builder b;
	b.MakeCompSolid(coms);
	BOPAlgo_CellsBuilder cellbd;
	cellbd.AddArgument(divideface);
	cellbd.AddArgument(bodyshape);
	cellbd.Perform();
	cellbd.AddAllToResult();
	TopoDS_Shape cellshape = cellbd.Shape();
	TopoDS_Iterator iter(cellshape);
	for (; iter.More(); iter.Next())
	{
		if (iter.Value().ShapeType() == TopAbs_SOLID)
		{
			TopoDS_Shape cur = iter.Value();
			b.Add(cur, coms);
		}	
	}

	TopoDS_Solid res = BRepBuilderAPI_MakeSolid(coms).Solid();

	BRepTools_ReShape brs;
	brs.Replace(bodyshape, res);
	brs.Apply(ownershape);
	ownershape = brs.Value(ownershape);

	// 根据布尔运算后的模型自动分配子节点到各个label
	TDF_Label targetLabel = LabelUtilities::createNewChild(rootLabel);
	adapter.setTopLabel(targetLabel);
	adapter.allocateLabelWithTopShape(targetLabel, ownershape);
	adapter.linkRelatedLabels(targetLabel);

	// some infos for return;
	TCollection_AsciiString anEntry;
	TColStd_ListOfInteger aTagList;
	TDF_Tool::Entry(targetLabel, anEntry);
	strPID = anEntry.ToCString();
	TDataStd_AsciiString::Set(targetLabel, "top_level");

	TheOCCApp.getCurrentModel()->addShapeToCacheByEntryOnSecondLevel(strPID, entity);
}