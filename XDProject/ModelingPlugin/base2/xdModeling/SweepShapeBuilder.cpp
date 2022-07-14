#include "SweepShapeBuilder.h"
#include "ShapeUtilities.h"
#include "TransformShapeBuilder.h"
#include "EdgeShapeBuilder.h"
#include "BRepOffsetAPI_MakeThickSolid_Extension.hxx"

#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepOffsetAPI_MakeDraft.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <BRep_Tool.hxx>
#include <gp_Circ.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Lin2d.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <GeomLProp_CLProps.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax1.hxx>
#include <gp_XYZ.hxx>

#define PI 3.1415926


SweepShapeBuilder::SweepShapeBuilder()
{
    m_sweepAlongPathResolution = 401;
    m_sweepAlongPathPrecision = Precision::Confusion() * 10000;
}

SweepShapeBuilder::~SweepShapeBuilder()
{
}

bool SweepShapeBuilder::extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec)
{
    try
    {
		if (m_builder)
		{
			delete m_builder;
		}

        m_builder = new BRepPrimAPI_MakePrism(shape, gp_Vec(xVec, yVec, zVec));
        m_builder->Build();
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

bool SweepShapeBuilder::extrudeByDraft(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec
    , const double& draftangle, const BRepBuilderAPI_TransitionMode mode)
{
    try
    {
        gp_Vec vec(xVec, yVec, zVec);
        double angle = draftangle * M_PI / 180.0;
        if (angle > M_PI / 6)
            angle = M_PI / 6;
        BRepOffsetAPI_MakeDraft bmd(shape, vec, angle);
        bmd.SetOptions(mode);
        //调整方向
        if (angle < 0)
            bmd.SetDraft(true);
        bmd.Perform(vec.Magnitude());

        TopoDS_Shape resultshape;
        if (shape.ShapeType() == TopAbs_WIRE)
            resultshape = bmd.Shape();
        //面变成体需要管，因为makedraft方法做出来的不是个封闭的体
        else if (shape.ShapeType() == TopAbs_SHELL)
        {
            resultshape = bmd.Shape();
            //选出未封闭线框
            TopExp_Explorer edgeex;
            std::vector<TopoDS_Shape> edgelist;
            TopTools_ListOfShape neededges;
            for (edgeex.Init(resultshape, TopAbs_EDGE); edgeex.More(); edgeex.Next())
            {
                edgelist.push_back(edgeex.Current());
            }
            for (int i = 0; i < edgelist.size(); i++)
            {
                TopoDS_Shape shape1 = edgelist[i];
                int count = 0;
                for (int j = 0; j < edgelist.size(); j++)
                {
                    TopoDS_Shape shape2 = edgelist[j];
                    if (ShapeUtilities::IsShapeGeomSame(shape1, shape2, TopAbs_EDGE))
                        count++
                }
                if (count == 1)
                    neededges.Append(shape1);
            }
            BRepBuilderAPI_MakeWire bmw;
            bmw.Add(neededges);
            if (!bmw.IsDone())
                return false;
            TopoDS_Wire unclosewire = bmw.Wire();
            TopoDS_Face uncloseface = BRepBuilderAPI_MakeFace(unclosewire).Face();

            TopoDS_Shape uncloseshell;
            ShapeUtilities::shapeCover(uncloseface, uncloseshell);

            BRepBuilderAPI_MakeSolid bms;
            bms.Add(TopoDS::Shell(resultshape));
            bms.Add(TopoDS::Shell(uncloseshell));

            resultshape = bms.Solid();
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SweepShapeBuilder::extrudeByPrism(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec
    , const double& draftangle, const BRepBuilderAPI_TransitionMode mode)
{
    try
    {
        int methodtype = 1;
        if (methodtype == 1)
        {
            gp_Vec vec(xVec, yVec, zVec);
            TopTools_ListOfShape shapearray;

            double angle = draftangle * M_PI / 180.0;
            if (angle > M_PI / 6)
                angle = M_PI / 6;
            bool topcover = false;
            TopoDS_Shape shapewire, extrudewire, offsetwire;

            BRepBuilderAPI_MakeWire aMakeWire(TopoDS::Wire(shapewire));
            if (shape.ShapeType() < TopAbs_WIRE)
            {
                ShapeUtilities::shapeFilter(shape, TopAbs_WIRE, shapewire);
                topcover = true;
            }
            else if (shape.ShapeType() > TopAbs_WIRE)
                return false;
            else
                shapewire = TopoDS::Wire(shape);
        }
    }
    catch (...)
    {
        return false;
    }
}

bool SweepShapeBuilder::extrudeDraftOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec, const double& draftAngle, const BRepBuilderAPI_TransitionMode mode, const bool& topCover)
{
    try
    {
        gp_Vec vec(xVec, yVec, zVec);

		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepOffsetAPI_MakeDraft(shape, vec, draftAngle/180.*PI);
        BRepOffsetAPI_MakeDraft* builder = dynamic_cast<BRepOffsetAPI_MakeDraft*>(m_builder);
        builder->SetOptions(mode);
        if (draftAngle < 0)
        {
            builder->SetDraft(true);
        }
        if (topCover == true)
        {
			//build stop face
			gp_XYZ bary;
			if (!ShapeUtilities::PlanarBaryCalculate(shape, bary))
			{
				return false;
			}

			gp_Pnt topFacePnt(bary);
			topFacePnt.Translate(gp_Vec(xVec, yVec, zVec));
			gp_Ax3 ax3(topFacePnt, gp_Dir(xVec, yVec, zVec));
			Handle(Geom_Plane) plane = new Geom_Plane(ax3);

			builder->Perform(plane, true);
        }
        else
        {
            builder->Perform(vec.Magnitude());
        }

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

bool SweepShapeBuilder::revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle /*= 360.0*/)
{
    try
    {

		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepPrimAPI_MakeRevol(shape, gp_Ax1(gp_Pnt(xStart, yStart, zStart), gp_Vec(xVec, yVec, zVec)), angle/180.*PI);
        m_builder->Build();
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

bool SweepShapeBuilder::sweepPipeOperation(TopoDS_Shape shape, TopoDS_Shape path)
{
    try
    {
        if (path.ShapeType() != TopAbs_WIRE)
        {
            return false;
        }

		if (m_builder)
		{
			delete m_builder;
		}
		m_builder = new BRepOffsetAPI_MakePipe(TopoDS::Wire(path), shape);
        m_builder->Build();
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

bool SweepShapeBuilder::sweepPipeShellOperation(TopoDS_Shape profile, TopoDS_Shape path, const double& twistAngle, const BRepBuilderAPI_TransitionMode mode)
{
    try
    {
        bool makeSolid = false;
        TopoDS_Shape profileWire = profile;
        if (profile.ShapeType() <= TopAbs_WIRE)
        {
            if (!ShapeUtilities::shapeFilter(profile, TopAbs_WIRE, profileWire))
            {
                return false;
            }
        }
        else if (profile.ShapeType() == TopAbs_EDGE)
        {
            if (!ShapeUtilities::shapeCover(profile, profileWire))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        if (profile.ShapeType() <= TopAbs_FACE)
        {
            makeSolid = true;
        }

        if (twistAngle == 0)//normal sweep along path
        {
            if (path.ShapeType() != TopAbs_WIRE)
            {
                return false;
            }

			if (m_builder)
			{
				delete m_builder;
			}
            m_builder = new BRepOffsetAPI_MakePipeShell(TopoDS::Wire(path));
            BRepOffsetAPI_MakePipeShell* builder = dynamic_cast<BRepOffsetAPI_MakePipeShell*>(m_builder);
            builder->SetTransitionMode(mode);
            builder->SetMaxDegree(180);
//             builder->SetMode(false);
            builder->SetDiscreteMode();
            builder->Add(profileWire, false, true);

            builder->Build();
            if (!m_builder->IsDone()) {
                return false;
            }
            if (makeSolid)
            {
                builder->MakeSolid();
            }
            if (!m_builder->IsDone()) {
                return false;
            }

            setShape(m_builder->Shape());
        }
        else
        {
            TopoDS_Shape edgePath;
            if (path.ShapeType() <= TopAbs_EDGE)
            {
                if (!ShapeUtilities::shapeFilter(path, TopAbs_EDGE, edgePath))
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

            //找到路径所在面法向（扫掠中的BiNormal）和起始位置处的方向（扫掠中的Normal）
			gp_Pln plane;
			if (!ShapeUtilities::isShapePlannar(path, plane))
			{
				return false;
			}
// 
// 
            gp_Dir pathFaceNormal, startBiNormalDir, startNormalDir;
//             Handle(Geom_Plane) geomPlane = ShapeUtilities::findPlane(path);
//             if (geomPlane.IsNull())
//             {
//                 return false;
//             }
            gp_Ax1 ax1 = plane.Axis();
            pathFaceNormal = ax1.Direction().XYZ();
            Standard_Real First, Last;
            Handle(Geom_Curve) splineCurve = BRep_Tool::Curve(TopoDS::Edge(edgePath), First, Last);

            gp_Pnt StartPos;
            gp_Vec StartDir;
            splineCurve->D1(First, StartPos, StartDir);
            startNormalDir = gp_Dir(StartDir);
//             GeomLProp_CLProps props1(splineCurve, First, 1, 0.0001);
//             props1.Tangent(startNormalDir);
            startBiNormalDir = startNormalDir.Crossed(pathFaceNormal);

            //找到path的起始点
//             TopoDS_Vertex pathStartVertex(StartPos);
//             TopExp_Explorer exp(path, TopAbs_VERTEX);
//             for (; exp.More(); exp.Next())
//             {
//                 pathStartVertex = TopoDS::Vertex(exp.Value());
//                 break;
//             }
//             gp_Pnt centerPnt = BRep_Tool::Pnt(pathStartVertex);
            gp_Pnt guideStartPnt = StartPos.Translated(gp_Vec(startBiNormalDir)*m_sweepAlongPathPrecision);

            //构造螺旋线
            Standard_Real Udiff = (Last - First) / m_sweepAlongPathResolution;
            Standard_Real Anglediff = twistAngle*M_PI / 180.0 / m_sweepAlongPathResolution;
            std::vector<std::vector<double>> guidePoints;
            for (int i = 1; i <= m_sweepAlongPathResolution; i++)
            {      
                gp_Dir curNormalDir, curBiNormalDir;
                gp_Pnt pathCurrentPnt;
                gp_Vec curVec;

                Standard_Real Ucurrent = First + (i - 1)*Udiff;

                gp_Pnt StartPos;
                splineCurve->D1(Ucurrent, pathCurrentPnt, curVec);
                curNormalDir = gp_Dir(curVec);
                curBiNormalDir = curNormalDir.Crossed(pathFaceNormal);

                Standard_Real Anglecurrent = (i - 1)*Anglediff;
                gp_Ax1 ax1(pathCurrentPnt, curNormalDir);
                curBiNormalDir.Rotate(ax1, Anglecurrent);

                pathCurrentPnt.Translate(gp_Vec(curBiNormalDir)*m_sweepAlongPathPrecision);

                std::vector<double> point;
                point.push_back(pathCurrentPnt.X());
                point.push_back(pathCurrentPnt.Y());
                point.push_back(pathCurrentPnt.Z());
                guidePoints.push_back(point);
            }

            EdgeShapeBuilder guideBuilder;
            if (!guideBuilder.createInterpolateBSpline(guidePoints))
            {
                return false;
            }
            TopoDS_Shape guide = guideBuilder.getShape();

            ShapeUtilities::shapeCover(guide, guide);

            //方法暂时被舍弃
            //延连续path造圆柱面,
            //circle
//             gp_Dir dir(xNormal, yNormal, zNormal);
//             gp_Ax2 axes(centerPnt, dir);
//             gp_Circ circle(axes, radius);
//             BRepBuilderAPI_MakeEdge edgeBuilder(circle);
//             edgeBuilder.Build();
//             if (!edgeBuilder.IsDone())
//             {
//                 return false;
//             }
// 
//             //splinesurface
//             SweepShapeBuilder pathSurfaceBuilder;
//             if (!pathSurfaceBuilder.sweepPipeOperation(edgeBuilder.Shape(), path))
//             {
//                 return false;
//             }
// 
//             TopoDS_Face splineFace = TopoDS::Face(pathSurfaceBuilder.getShape());
//             Handle(Geom_Surface) splineSurface = BRep_Tool::Surface(TopoDS::Face(splineFace));
// 
//             //bend helix curve
//             Standard_Real U1, U2, V1, V2;
//             splineSurface->Bounds(U1, U2, V1, V2);
// 
//             double rightHandedFactor = twistAngle >0 ? -1 : 1;
//             gp_Lin2d aLine2d = gp_Lin2d(gp_Pnt2d(0.0, V1), gp_Dir2d(rightHandedFactor*twistAngle*M_PI/180.0, V2));
//             Handle_Geom2d_TrimmedCurve aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();
//             BRepBuilderAPI_MakeEdge helixBuilder;
//             helixBuilder = BRepBuilderAPI_MakeEdge(aSegment, splineSurface, 0.0, turns *2.0 * M_PI);
//             helixBuilder.Build();
//             BRepLib::BuildCurve3d(helixBuilder.Edge(), 1e-5, GeomAbs_C1, 14, turns * 2);
//             if (!helixBuilder.IsDone())
//             {
//                 return false;
//             }

			if (m_builder)
			{
				delete m_builder;
			}
            m_builder = new BRepOffsetAPI_MakePipeShell(TopoDS::Wire(path));
            BRepOffsetAPI_MakePipeShell* builder = dynamic_cast<BRepOffsetAPI_MakePipeShell*>(m_builder);
            builder->SetTransitionMode(mode);
            builder->SetMode(TopoDS::Wire(guide), false, BRepFill_NoContact);
            builder->Add(profileWire, false, true);
            m_builder->Build();

            if (makeSolid)
            {
                builder->MakeSolid();
            }
            if (!m_builder->IsDone()) {
                return false;
            }

            setShape(m_builder->Shape());

        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SweepShapeBuilder::loftOperation(TopTools_ListOfShape shapeArray, const bool& genSolid)
{
    try
    {
        if (shapeArray.Size() < 2)
        {
            return false;
        }


		if (m_builder)
		{
			delete m_builder;
		}
        m_builder = new BRepOffsetAPI_ThruSections(genSolid);
        BRepOffsetAPI_ThruSections* builder = dynamic_cast<BRepOffsetAPI_ThruSections*>(m_builder);

		TopTools_ListOfShape::Iterator iter(shapeArray);
        TopoDS_Shape curShape = iter.Value();
        if (curShape.ShapeType() == TopAbs_VERTEX)
        {
            builder->AddVertex(TopoDS::Vertex(curShape));
        }
        else if (curShape.ShapeType() == TopAbs_WIRE)
        {
            builder->AddWire(TopoDS::Wire(curShape));
        }
        else
        {
            return false;
        }
        iter.Next();

        for (int i = 1; i<shapeArray.Size()-1; iter.Next(), i++)
        {
            curShape = iter.Value();
            if(curShape.ShapeType() == TopAbs_WIRE)
            {
                builder->AddWire(TopoDS::Wire(curShape));
            }
            else
            {
                return false;
            }
        }

        curShape = iter.Value();
        if (curShape.ShapeType() == TopAbs_VERTEX)
        {
            builder->AddVertex(TopoDS::Vertex(curShape));
        }
        else if (curShape.ShapeType() == TopAbs_WIRE)
        {
            builder->AddWire(TopoDS::Wire(curShape));
        }
        else
        {
            return false;
        }

        m_builder->Build();
        if (!m_builder->IsDone()) {
            return false;
        }

        setShape(m_builder->Shape());

        return true;
    }
    catch (...)
    {
    }
}