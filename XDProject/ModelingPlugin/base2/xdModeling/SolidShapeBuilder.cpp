#include "SolidShapeBuilder.h"
#include "EdgeShapeBuilder.h"
#include "OCCBasicTools.h"
#include "ShapeUtilities.h"
#include "OCCFixTool.h"
#include "FaceShapeBuilder.h"
#include "SweepShapeBuilder.h"
#include "TransformShapeBuilder.h"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Elips.hxx>
#include <gp_Circ.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepTools.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepFill_Pipe.hxx>
#include <gp_Lin2d.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom2d_Line.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <BRepLib.hxx>
#include <Geom_Circle.hxx>

SolidShapeBuilder::SolidShapeBuilder()
{
}


SolidShapeBuilder::~SolidShapeBuilder()
{
}


bool SolidShapeBuilder::createRegularPolyhedron(double XbotCenter, double YbotCenter, double ZbotCenter, double XbotStart, double YbotStart, double ZbotStart, const double& height, const int& numSides, double XAxis, double YAxis, double ZAxis)
{
    try
    {
        //create RegularPolyhedron
        gp_Pnt center(XbotCenter, YbotCenter, ZbotCenter);
        gp_Pnt start(XbotStart, YbotStart, ZbotStart);
        gp_Dir dir(XAxis, YAxis, ZAxis);

        if (gp_Dir(gp_Vec(center, start)).Dot(dir) != 0)
        {
            return false;
        }

        if (numSides < 3)
        {
            return false;
        }

        std::vector<gp_Pnt> PntList;
        gp_Trsf rotate;
        for (int i = 0; i < numSides; ++i)
        {

            PntList.push_back(start.Rotated(gp_Ax1(center, dir), 2 * i * M_PI / numSides));
        }

        TopoDS_Face Face = OCCBasicTools::GetFaceFromPntList(PntList);
        TopoDS_Edge PathEdge = BRepBuilderAPI_MakeEdge(center, center.Translated(gp_Vec(dir) * height));

        if (Face.IsNull() || PathEdge.IsNull())
        {
            return false;
        }
        TopoDS_Wire PathWire = BRepBuilderAPI_MakeWire(PathEdge);
        BRepOffsetAPI_MakePipe aBuilder(PathWire, Face);
        aBuilder.Build();
        if (!aBuilder.IsDone())
        {
            return false;
        }

        setShape(aBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createSpindle(double xbotcenter, double ybotcenter, double zbotcenter,
    double majradius, double ratio, double xaxis, double yaxis, double zaxis)
{
    try
    {
        double minradius = majradius * ratio;
        gp_Pnt pnt(xbotcenter, ybotcenter, zbotcenter);
        gp_Dir dir(xaxis, yaxis, zaxis);
        gp_Ax2 axes;
        if (dir.IsParallel(gp_Dir(0, 0, 1), Precision::Angular()))
        {
            axes = gp_Ax2(pnt, dir, gp_Dir(1, 0, 0));
        }
        else if (dir.IsParallel(gp_Dir(0, 1, 0), Precision::Angular()))
        {
            axes = gp_Ax2(pnt, dir, gp_Dir(0, 0, 1));
        }
        else if (dir.IsParallel(gp_Dir(1, 0, 0), Precision::Angular()))
        {
            axes = gp_Ax2(pnt, dir, gp_Dir(0, 1, 0));
        }

        TopoDS_Edge arc;
        if (majradius >= minradius)
        {
            gp_Elips ellipse(axes, majradius, minradius);
            arc = BRepBuilderAPI_MakeEdge(ellipse, 0, M_PI);
        }
        if (majradius < minradius)
        {
            gp_Elips ellipse(axes, minradius, majradius);
            arc = BRepBuilderAPI_MakeEdge(ellipse, -M_PI_2, M_PI_2);
        }
        gp_Pnt startpt, endpt;
        double startp, endp;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(arc, startp, endp);
        startpt = curve->Value(startp);
        endpt = curve->Value(endp);
        TopoDS_Edge line = BRepBuilderAPI_MakeEdge(startpt, endpt);

        BRepBuilderAPI_MakeWire wirebd;
        wirebd.Add(arc);
        wirebd.Add(line);
        wirebd.Build();
        if (!wirebd.IsDone())
            return false;

        TopoDS_Face ellipseface = BRepBuilderAPI_MakeFace(wirebd.Wire());
        gp_Ax1 majoraxis(pnt, gp_Dir(gp_Vec(endpt, startpt)));

        BRepPrimAPI_MakeRevol revolbd(ellipseface, majoraxis);

        revolbd.Build();
        if (!revolbd.IsDone())
            return false;

        if (majradius < minradius)
        {
            gp_Trsf rotate;
            rotate.SetRotation(gp_Ax1(pnt, dir), M_PI_2);
            setShape(BRepBuilderAPI_Transform(revolbd.Shape(),rotate));   
        }
        else
        {
            setShape(revolbd.Shape());
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createCuboid(const double& xStart, const double& yStart, const double& zStart, const double& xSize, const double& ySize, const double& zSize)
{
    try
    {
        //create box
        BRepPrimAPI_MakeBox boxBuilder(gp_Pnt(xStart, yStart, zStart), xSize, ySize, zSize); // creating Box1
        boxBuilder.Build();
        if (!boxBuilder.IsDone())
        {
            return false;
        }

        setShape(boxBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createCylinder(const double& xBottomCenter, const double& yBottomCenter, const double& zBottomCenter, const double& xAxis, const double& yAxis, const double& zAxis, const double& raidus, const double& height)
{
    try
    {
        //create cylinder
        gp_Pnt pnt(xBottomCenter, yBottomCenter, zBottomCenter);
        double xRealAxis = xAxis, yRealAxis = yAxis, zRealAxis = zAxis;
        if (height < 0)
        {
            xRealAxis *= -1.;
            yRealAxis *= -1.;
            zRealAxis *= -1.;
        }

        gp_Dir dir(xRealAxis, yRealAxis, zRealAxis);
        gp_Ax2 axes(pnt, dir);

        BRepPrimAPI_MakeCylinder cylinderBuilder(axes, raidus, abs(height));
        cylinderBuilder.Build();
        if (!cylinderBuilder.IsDone())
        {
            return false;
        }

        setShape(cylinderBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createCone(const double& botCenterX, const double& botCenterY, const double& botCenterZ,
    const double& lowerRadius, const double& upperRadius, const double& height,
    const double& xAxis, const double& yAxis, const double& zAxis)
{
    try
    {
        //create cylinder
        gp_Pnt pnt(botCenterX, botCenterY, botCenterZ);
        gp_Dir dir(xAxis, yAxis, zAxis);
        gp_Ax2 axes(pnt, dir);

        if (height < 0)
        {
            axes = gp_Ax2(pnt, -dir);
        }
        TopoDS_Shape Solid;
        if (lowerRadius == upperRadius)
        {
            BRepPrimAPI_MakeCylinder cylinderBuidler(axes, lowerRadius, abs(height));
            cylinderBuidler.Build();
            if (!cylinderBuidler.IsDone())
            {
                return false;
            }

            Solid = cylinderBuidler.Shape();
        }
        else
        {
            BRepPrimAPI_MakeCone coneBuilder(axes, lowerRadius, upperRadius, abs(height));
            coneBuilder.Build();
            if (!coneBuilder.IsDone())
            {
                return false;
            }

            Solid = coneBuilder.Shape();
        }

        setShape(Solid);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createSphere(const double& xCenter, const double& yCenter, const double& zCenter, const double& raidus)
{
    try
    {
        //create sphere
        gp_Pnt pnt(xCenter, yCenter, zCenter);
        BRepPrimAPI_MakeSphere sphereBuilder(pnt, raidus);
        sphereBuilder.Build();
        if (!sphereBuilder.IsDone())
        {
            return false;
        }

        setShape(sphereBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createTorus(const double& botCenterX, const double& botCenterY, const double& botCenterZ, const double& minorRadius, const double& majorRadius, const double& xAxis, const double& yAxis, const double& zAxis)
{
    try
    {
        //create cylinder
        gp_Pnt pnt(botCenterX, botCenterY, botCenterZ);
        gp_Dir dir(xAxis, yAxis, zAxis);
        gp_Ax2 axes(pnt, dir);

        BRepPrimAPI_MakeTorus torusBuilder(axes, majorRadius, minorRadius);
        torusBuilder.Build();
        if (!torusBuilder.IsDone())
        {
            return false;
        }

        setShape(torusBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createPrism(const std::vector<std::vector<double>>& pPos, const double& xVec, const double& yVec, const double& zVec)
{
    try
    {
        std::vector<std::vector<double>> newpPos = pPos;
        TopoDS_Shape bottomWire;

        BRepBuilderAPI_MakeWire wireBuilder;
        EdgeShapeBuilder shapeEdgeBuilder;
        if (!shapeEdgeBuilder.createPolyLine(newpPos))
        {
            return false;
        }
        else
        {
            wireBuilder.Add(TopoDS::Wire(shapeEdgeBuilder.getShape()));
//             bottomWire = TopoDS::Wire(shapeEdgeBuilder.getShape());
        }

        gp_Pnt endPnt(newpPos.at(0).at(0), newpPos.at(0).at(1), newpPos.at(0).at(2));
        gp_Pnt startPnt(newpPos.at(newpPos.size() - 1).at(0), newpPos.at(newpPos.size() - 1).at(1), newpPos.at(newpPos.size() - 1).at(2));
        if (!startPnt.IsEqual(endPnt, Precision::Confusion()))
        {
            std::vector<std::vector<double>> segPnts;
            segPnts.push_back(newpPos.at(newpPos.size() - 1));
            segPnts.push_back(newpPos.at(0));

            if (!shapeEdgeBuilder.createPolyLine(segPnts))
            {
                return false;
            }

            wireBuilder.Add(TopoDS::Wire(shapeEdgeBuilder.getShape()));
        }
        wireBuilder.Build();
        if (!wireBuilder.IsDone())
        {
            return false;
        }
        bottomWire = wireBuilder.Shape();
        OCCFixToolInstance->fixTopoValidityOp(bottomWire);

        FaceShapeBuilder faceBuilder;
        if (!faceBuilder.createClosedFace(TopoDS::Wire(bottomWire)))
        {
            return false;
        }
        TopoDS_Shape bottomFace = faceBuilder.getShape();

        BRepPrimAPI_MakePrism prismBuilder(bottomFace, gp_Vec(xVec, yVec, zVec));
        prismBuilder.Build();
        if (!prismBuilder.IsDone())
        {
            return false;
        }

        setShape(prismBuilder.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }

}

bool SolidShapeBuilder::createFrustrumPrism(const std::vector<std::vector<double>>& pPos, const double& scale, const double& xVec, const double& yVec, const double& zVec)
{
    try
    {
        std::vector<std::vector<double>> newpPos = pPos;
        TopoDS_Shape bottomWire;

        BRepBuilderAPI_MakeWire wireBuilder;
        EdgeShapeBuilder shapeEdgeBuilder;
        if (!shapeEdgeBuilder.createPolyLine(newpPos))
        {
            return false;
        }
        else
        {
            wireBuilder.Add(TopoDS::Wire(shapeEdgeBuilder.getShape()));
            //             bottomWire = TopoDS::Wire(shapeEdgeBuilder.getShape());
        }

        gp_Pnt endPnt(newpPos.at(0).at(0), newpPos.at(0).at(1), newpPos.at(0).at(2));
        gp_Pnt startPnt(newpPos.at(newpPos.size() - 1).at(0), newpPos.at(newpPos.size() - 1).at(1), newpPos.at(newpPos.size() - 1).at(2));
        if (!startPnt.IsEqual(endPnt, Precision::Confusion()))
        {
            std::vector<std::vector<double>> segPnts;
            segPnts.push_back(newpPos.at(newpPos.size() - 1));
            segPnts.push_back(newpPos.at(0));

            if (!shapeEdgeBuilder.createPolyLine(segPnts))
            {
                return false;
            }

            wireBuilder.Add(TopoDS::Wire(shapeEdgeBuilder.getShape()));
        }
        wireBuilder.Build();
        if (!wireBuilder.IsDone())
        {
            return false;
        }
        bottomWire = wireBuilder.Shape();

        gp_XYZ bary;
        if (!ShapeUtilities::PlanarBaryCalculate(bottomWire, bary))
        {
            return false;
        }

        gp_Trsf T;
        T.SetScale(gp_Pnt(bary), scale);
        BRepBuilderAPI_Transform transformer(T);
        transformer.Perform(bottomWire, true);
        if (!transformer.IsDone())
        {
            return false;
        }
        TopoDS_Shape scaledBottomWire = transformer.Shape();

        TransformShapeBuilder transformBuilder;
        if (!transformBuilder.shapeTranslate(scaledBottomWire, xVec, yVec, zVec, false))
        {
            return false;
        }
        TopoDS_Shape topWire = transformBuilder.getShape();

        BRepOffsetAPI_ThruSections generator(Standard_True);
        generator.AddWire(TopoDS::Wire(bottomWire));
        generator.AddWire(TopoDS::Wire(topWire));
        generator.Build();
        if (!generator.IsDone())
        {
            return false;
        }

        setShape(generator.Shape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createFlare(
    const double& XbotCenter, const double& YbotCenter, const double& ZbotCenter,
    const double& lowerWidth, const double& lowerDepth,
    const double& upperWidth, const double& upperDepth, const double& Height,
    const double& XAxis, const double& YAxis, const double& ZAxis)
{
    try
    {
        //create wedge
        double dx = lowerWidth, dy = abs(Height), dz = lowerDepth;
        double xmin = lowerWidth / 2. - upperWidth / 2.;
        double xmax = lowerWidth / 2. + upperWidth / 2.;
        double zmin = lowerDepth / 2. - upperDepth / 2.;
        double zmax = lowerDepth / 2. + upperDepth / 2.;

        gp_Trsf translate_origin;
        translate_origin.SetTranslation(gp_Vec(-dx / 2., 0, -dz / 2.));
        gp_Trsf translate_center;
        translate_center.SetTranslation(gp_Vec(XbotCenter, YbotCenter, ZbotCenter));
        gp_Trsf rotate_trsf;

        gp_Pnt globlecenter(XbotCenter, YbotCenter, ZbotCenter);
        gp_Dir dir(XAxis, YAxis, ZAxis);
        if (dir.IsEqual(gp::DX(), 0.001))
        {
            rotate_trsf.SetRotation(gp_Ax1(gp::Origin(), gp::DZ()), -M_PI_2);
        }
        else if (dir.IsEqual(gp::DY(), 0.001))
        {
        }
        else if (dir.IsEqual(gp::DZ(), 0.001))
        {
            rotate_trsf.SetRotation(gp_Ax1(gp::Origin(), gp::DX()), M_PI_2);
        }
        else
        {
            return false;
        }

        gp_Ax2 Axes(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1), gp_Dir(1 * Height, 0, 0));

        BRepPrimAPI_MakeWedge wedgeBuilder(Axes, dx, dy, dz, xmin, zmin, xmax, zmax);
        wedgeBuilder.Build();
        if (!wedgeBuilder.IsDone())
        {
            return false;
        }

        TopoDS_Shape Wedge;
        Wedge = BRepBuilderAPI_Transform(wedgeBuilder.Shape(), translate_origin);
        Wedge = BRepBuilderAPI_Transform(Wedge, rotate_trsf);
        Wedge = BRepBuilderAPI_Transform(Wedge, translate_center);
        setShape(Wedge);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createEllipsoid(
    const double& XbotCenter, const double& YbotCenter, const double& ZbotCenter,
    const double& majRadius, const double& ratio,
    const double& XAxis, const double& YAxis, const double& ZAxis)
{
    try
    {
        //create Ellipsoid

        double minRadius = majRadius * ratio;
        gp_Pnt pnt(XbotCenter, YbotCenter, ZbotCenter);
        gp_Dir dir(XAxis, YAxis, ZAxis);
        gp_Ax2 axes(pnt, dir);

        TopoDS_Edge Arc;
        if (majRadius > minRadius)
        {
            gp_Elips ellipse(axes, majRadius, minRadius);
            Arc = BRepBuilderAPI_MakeEdge(ellipse, 0, M_PI);

        }
        if (majRadius < minRadius)
        {
            gp_Elips ellipse(axes, minRadius, majRadius);
            Arc = BRepBuilderAPI_MakeEdge(ellipse, 0, M_PI);
        }
        if (majRadius == minRadius)
        {
            gp_Circ circle(axes, majRadius);
            Arc = BRepBuilderAPI_MakeEdge(circle, 0, M_PI);
        }

        gp_Pnt StartPnt, EndPnt;
        double startP, endP;
        Handle(Geom_Curve) Curve = BRep_Tool::Curve(Arc, startP, endP);
        StartPnt = Curve->Value(startP);
        EndPnt = Curve->Value(endP);
        TopoDS_Edge Line = BRepBuilderAPI_MakeEdge(StartPnt, EndPnt);

        BRepBuilderAPI_MakeWire wireBuilder;
        wireBuilder.Add(Arc);
        wireBuilder.Add(Line);
        wireBuilder.Build();
        if (!wireBuilder.IsDone())
        {
            return false;
        }
        TopoDS_Face EllipseFace = BRepBuilderAPI_MakeFace(wireBuilder.Wire());
        gp_Ax1 majoraxis(pnt, gp_Dir(gp_Vec(EndPnt, StartPnt)));

        BRepPrimAPI_MakeRevol revolBuilder(EllipseFace, majoraxis);

        revolBuilder.Build();
        if (!revolBuilder.IsDone())
        {
            return false;
        }

        gp_Trsf rotate;
        if (majRadius < minRadius)
        {
            rotate.SetRotation(gp_Ax1(pnt, dir), M_PI_2);
        }
        setShape(BRepBuilderAPI_Transform(revolBuilder.Shape(), rotate));
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool SolidShapeBuilder::createHelixSolid(
    const double& Xcenter, const double& Ycenter, const double& Zcenter,
    const double& profileRadius, const double& lowerRadius, const double& upperRadius,
    const double& height, const double& turns, const double& rightHanded,
    const double& XAxis, const double& YAxis, const double& ZAxis)
{

    try
    {
//         //通用设置
//         double maxRadius = upperRadius > lowerRadius ? upperRadius : lowerRadius;
//         gp_Pnt center(Xcenter, Ycenter, Zcenter);
//         gp_Dir dir(XAxis, YAxis, ZAxis);
//         gp_Ax2 axes = gp_Ax2(center, dir);
//         double angle = atan(abs(upperRadius - lowerRadius) / abs(height));
//         //         double angle = atan(abs(height) / abs(turns *2.0 * M_PI));
//         double slope = cos(angle);
// 
//         //造线
//         double rightHandedFactor = rightHanded == 1 ? -1 : 1;
//         Handle(Geom2d_Curve) aLine2d = new Geom2d_Line(gp_Pnt2d(0.0, 0.0), gp_Dir2d(rightHandedFactor*2.0 * M_PI, height / turns / slope));
//         Handle(Geom2d_Curve) aSegment = new Geom2d_TrimmedCurve(aLine2d, 0.0, M_PI * 2.0);
// 
//         //造面
//         BRepBuilderAPI_MakeEdge helixBuilder;
//         Handle(Geom_CylindricalSurface) aCylinder = new Geom_CylindricalSurface(axes, lowerRadius);
//         helixBuilder = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, turns *2.0 * M_PI);
//         TopoDS_Edge edge = helixBuilder.Edge();
// 
//         BRepLib::BuildCurves3d(edge, 0.001);
// 
//         BRepBuilderAPI_MakeWire MW;
//         MW.Add(TopoDS::Edge(edge));
//         TopoDS_Wire helixWire = TopoDS::Wire(MW.Shape());
// 
//         double StartP, EndP;
//         Handle(Geom_Curve) helixCurve = BRep_Tool::Curve(edge, StartP, EndP);
// 
//         //profile face
//         gp_Pnt StartPos;
//         gp_Vec StartDir;
//         helixCurve->D1(StartP, StartPos, StartDir);
// 
//         Handle(Geom_Circle) circle = new Geom_Circle(gp_Ax2(gp_Pnt(StartPos.X(), StartPos.Y(), StartPos.Z()),
//             gp_Dir(StartDir.X(), StartDir.Y(), StartDir.Z())),
//             profileRadius);
//         helixBuilder = BRepBuilderAPI_MakeEdge(circle);
//         TopoDS_Shape edge2 = helixBuilder.Shape();
//         BRepBuilderAPI_MakeWire MW2;
//         MW2.Add(TopoDS::Edge(edge2));
//         TopoDS_Wire circleWire = TopoDS::Wire(MW2.Shape());
// 
// 
//         Standard_Boolean OnlyPlane = Standard_False;
//         BRepBuilderAPI_MakeFace aMF(TopoDS::Wire(circleWire), OnlyPlane);
// 
// 
// //         FaceShapeBuilder faceBuilder;
// //         if (!faceBuilder.createCircleFace(StartPos.X(), StartPos.Y(), StartPos.Z(), StartDir.X(), StartDir.Y(), StartDir.Z(), profileRadius))
// //         {
// //             return false;
// //         }
//         TopoDS_Shape profileShape = aMF.Shape();
// 
//         GeomFill_Trihedron Mode = GeomFill_IsCorrectedFrenet;
//         Standard_Boolean ForceApproxC1 = Standard_False;
//         BRepOffsetAPI_MakePipe PipeBuilder(helixWire, profileShape, Mode, ForceApproxC1);
//         TopoDS_Shape S = PipeBuilder.Shape();
//         setShape(S);




        //helix curve
        EdgeShapeBuilder edgeBuilder;
        if (!edgeBuilder.createHelix(Xcenter, Ycenter, Zcenter,
            lowerRadius, upperRadius, height, turns, rightHanded,
            XAxis, YAxis, ZAxis))
        {
            return false;
        }

        TopoDS_Edge heilxEdge = TopoDS::Edge(edgeBuilder.getShape());
        double StartP, EndP;
        TopoDS_Wire helixWire = BRepBuilderAPI_MakeWire(heilxEdge);
        Handle(Geom_Curve) helixCurve = BRep_Tool::Curve(heilxEdge, StartP, EndP);

        //profile face
        gp_Pnt StartPos;
        gp_Vec StartDir;
        helixCurve->D1(StartP, StartPos, StartDir);

        FaceShapeBuilder faceBuilder;
        if (!faceBuilder.createCircleFace(StartPos.X(), StartPos.Y(), StartPos.Z(), StartDir.X(), StartDir.Y(), StartDir.Z(), profileRadius))
        {
            return false;
        }
        TopoDS_Shape profileShape = faceBuilder.getShape();
        TopoDS_Shape profileWire = profileShape;
        if (profileShape.ShapeType() <= TopAbs_WIRE)
        {
            if (!ShapeUtilities::shapeFilter(profileShape, TopAbs_WIRE, profileWire))
            {
                return false;
            }
        }

        BRepOffsetAPI_MakePipeShell* builderSweep = new BRepOffsetAPI_MakePipeShell(TopoDS::Wire(helixWire));
        builderSweep->SetTransitionMode(BRepBuilderAPI_RoundCorner);
//         builderSweep->SetMaxDegree(180);

//         builderSweep->SetMode(false);
        builderSweep->SetDiscreteMode();
        builderSweep->SetForceApproxC1(true);
        builderSweep->Add(profileWire);

        builderSweep->Build();
        if (!builderSweep->IsDone()) {
            return false;
        }
        builderSweep->MakeSolid();
        if (!builderSweep->IsDone()) {
            return false;
        }
        setShape(builderSweep->Shape());


//         SweepShapeBuilder sweepBuilder;
//         if (!sweepBuilder.sweepPipeShellOperation(profileShape, helixWire, 0, BRepBuilderAPI_RoundCorner))
//         {
//             return false;
//         }
//         setShape(sweepBuilder.getShape());

        return true;
    }
    catch (...)
    {
        return false;
    }
}
