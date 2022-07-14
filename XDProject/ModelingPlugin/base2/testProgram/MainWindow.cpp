#include "MainWindow.h"
#include "occView.h"
#include "OccApplication.h"  
#include "OccModel.h"  
#include "OccModelBuilder.h"
#include "DrawModelDlg.h"
#include "EraseModelDlg.h"  
#include "ExportModelDlg.h"            
// #include "OccShapeStorage.h"
// #include "OccDocShape.h"
   
#include <QtWidgets/QApplication>     
#include <QtWidgets/QMainWindow> 
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries> 
#include <QBoxLayout>   
#include <QFileDialog>  
#include <QMessageBox>
#include <QTextStream>

//occ
#include <AIS_InteractiveContext.hxx>

#include <BRep_Tool.hxx> 

#include <BRepAlgoAPI_Fuse.hxx>  

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepLib.hxx>

#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepPrimAPI_MakeCylinder.hxx> 
#include <BRepPrimAPI_MakePrism.hxx>

#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include "IGESControl_Reader.hxx"
#include "TopTools_HSequenceOfShape.hxx"
#include "BRepBuilderAPI_Sewing.hxx"
#include "IGESControl_Controller.hxx"
#include "IGESControl_Writer.hxx"

#include <TopTools_ListOfShape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <AIS_Shape.hxx>
#include "StlAPI_Writer.hxx"
#include "BRepAlgoAPI_Section.hxx"
#include "BRepMesh_IncrementalMesh.hxx"
#include "BRepBuilderAPI_Copy.hxx"
#include "occView.h"
#include "Standard_CString.hxx"
#include "RWStl.hxx"
#include "ShapeFixWidget.h"

#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pnt.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_MapOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopExp_Explorer.hxx>

#include <TopOpeBRepBuild_HBuilder.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepAlgo_Cut.hxx>
#include <BRepAlgo.hxx>

#include <TDF_Data.hxx>
#include <TDF_Label.hxx>
#include <TDF_LabelMap.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_MapIteratorOfLabelMap.hxx>

#include <TNaming_NamedShape.hxx>
#include <TNaming_Selector.hxx>
#include <TNaming_Tool.hxx>
#include <TNaming_Builder.hxx>
#include <TNaming.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <TDF_Tool.hxx>
#include <TNaming_UsedShapes.hxx>

#include <TDataStd_Name.hxx>
#include <XCAFDoc_Color.hxx>


#include <iostream>
#include <fstream>
#include <BRepTools.hxx>

#include <OSD_Environment.hxx>
#include <BinDrivers.hxx>
#include <XmlDrivers.hxx>
#include <StdDrivers.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_HShape.hxx>
#include <QDebug>

#include "EdgeShapeBuilder.h"
#include "SolidShapeBuilder.h" 
#include "LabelUtilities.h"
#include "LabelShapeAdapter.h"

#include <TDataStd_AsciiString.hxx>
  
// std::string xmlPath = "E:/SVNproject/FakeCAESES/trunk/test/box1.xml";
// std::string brepPath = "E:/SVNproject/FakeCAESES/trunk/test/box1.brep";
 
#define TestProgram

#define transparency 0.5 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
     
//     qputenv("CSF_ResourcesDefaults", "D:\\softwareLists\\OCCT_7.4.0_VTK8.0\\src\\StdResource\\Standard");
//     qputenv("CSF_PluginDefaults", "D:\\softwareLists\\OCCT_7.4.0_VTK8.0\\src\\StdResource\\Plugin");

    QString exeDir = QApplication::applicationDirPath(); 
    exeDir += "/Shaders";   

    //Add occ environment setting 
    TCollection_AsciiString envSDValue = TCollection_AsciiString((Standard_CString)(exeDir.toStdString().c_str()));
    OSD_Environment aSDR(TCollection_AsciiString("CSF_ShadersDirectory"), envSDValue);  
    aSDR.Build();

    m_occ = new OccView(this);  
    QVBoxLayout *layout = new QVBoxLayout(ui.widget);
    layout->setMargin(0);
    layout->addWidget(m_occ);    
     
      
    //...Creating the new document (document conatins a framework) 
    m_app = OccApplication::instance()->getApp();
    m_model = OccApplication::instance()->createNewDoc(m_docID);
     
    m_doc = m_model->getDoc();
//     m_app = new TDocStd_Application;
//     m_app->NewDocument("XmlOcaf", m_doc);
    m_DF = m_doc->GetData();
    m_rootLabel = m_DF->Root();
    //setting
    m_doc->SetUndoLimit(10);
    int m = m_doc->GetUndoLimit();  
    TPrsStd_AISViewer::New(m_rootLabel, m_occ->getViewer());
       
    //OCC VIEW
//     TPrsStd_AISViewer::New(m_model->getDoc()->Main(), m_occ->getViewer());
    TPrsStd_AISViewer::Find(m_rootLabel, aContext);
    aContext->SetDisplayMode(AIS_Shaded, Standard_True);   
    m_occ->setContext(aContext);
      
       
    m_wcsMatrix = new float[16];
    m_wcsMatrix[0] = 1;
    m_wcsMatrix[1] = 0;
    m_wcsMatrix[2] = 0;
    m_wcsMatrix[3] = 0;

    m_wcsMatrix[4] = 0; 
    m_wcsMatrix[5] = 1; 
    m_wcsMatrix[6] = 0;
    m_wcsMatrix[7] = 0;

    m_wcsMatrix[8] = 0;
    m_wcsMatrix[9] = 0;
    m_wcsMatrix[10] = 1;
    m_wcsMatrix[11] = 0;

    m_wcsMatrix[12] = 0;
    m_wcsMatrix[13] = 0;
    m_wcsMatrix[14] = 0;
    m_wcsMatrix[15] = 1; 

    connect(ui.pushButton, &QPushButton::clicked, this, &MainWindow::slotPushButton);
}


MainWindow::~MainWindow()
{
}   

void MainWindow::slotPushButton() 
{
    QString name;
    std::string strPID;
    void* entity = NULL;

    double* wcsMatrix = new double[16];
    wcsMatrix[0] = 1;
    wcsMatrix[1] = 0;
    wcsMatrix[2] = 0;
    wcsMatrix[3] = 0;
    wcsMatrix[4] = 0;
    wcsMatrix[5] = 1;
    wcsMatrix[6] = 0;
    wcsMatrix[7] = 0;
    wcsMatrix[8] = 0;
    wcsMatrix[9] = 0;
    wcsMatrix[10] = 1;
    wcsMatrix[11] = 0;
    wcsMatrix[12] = 0;
    wcsMatrix[13] = 0;
    wcsMatrix[14] = 0;
    wcsMatrix[15] = 0;

    std::string strFile = "C:\\Users\\DELL\\Documents\\WeChat Files\\wxid_tmwypojkb8f022\\FileStorage\\File\\2021-12\\dongye(1).geomTurbo";
	ifstream ifile;
	ifile.open(strFile, ifstream::in);
	if (!ifile)
		cerr << "open failed" << endl;
    string line, word;
    std::vector<std::vector<double>> points;
    while (getline(ifile, line))
    {
		std::vector<string> text;
		istringstream istrm(line);//创建字符串流
		while (istrm >> word)     //逐单词读
			text.push_back(word); //将单词都放入容器
        std::vector<double> vecx;
        if (text.size() == 3)
        {
			double dTempValue = atof(text[0].c_str());
            vecx.push_back(dTempValue);
			dTempValue = atof(text[1].c_str());
			vecx.push_back(dTempValue);
			dTempValue = atof(text[2].c_str());
			vecx.push_back(dTempValue);
            points.push_back(vecx);
        }
    }
	/* points.push_back({-0.0011116620153189,0.10041125118732,0.035626299679279});
	 points.push_back({ -0.0008332640863955,0.10043898224831,0.035467721521854 });
	 points.push_back({ -0.00051561964210123,0.10044814646244,0.035423006862402 });
	 points.push_back({ -0.00051561964210123,0.10044814646244,0.035423006862402 });*/

    std::vector<PolylineSegment> segments;
    PolylineSegment seg;
    seg.StartIndex = 0;
    seg.Type = MULTISEGMENTTYPE::Spline;
    seg.NoOfPoints = 108;
    seg.CenterAxis = Z;
    segments.push_back(seg);

    CrossSection crossSection;

	OccModelBuilder builder;
	if (!builder.createPolyline(wcsMatrix, points, segments, True, False, crossSection, name.toStdString().c_str(), strPID, entity))
		return;
}

void MainWindow::on_toolButton_point_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnPointWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
} 

void MainWindow::on_toolButton_line_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMultiLinesWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}  

void MainWindow::on_toolButton_polyline_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMultiLinesWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_spline_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMultiLinesWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_interSpline_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnInterPSplineWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_threeArc_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMultiLinesWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_angArc_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMultiLinesWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_hyperbola_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnHyperbolaWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_parabola_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnParabolaWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_ellipse_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnEllipseWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_spiral_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnSpiralWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_helix_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnHelixWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_FuncCurve_clicked()
{
    COMMAND_BEGIN;
    EdgeShapeBuilder builder;
    std::string newEntry;
    void* newShapeEntry = nullptr;

    std::string xFuncExpress = "5*sin(t)";
    std::string yFuncExpress = "10*cos(t)";

//     std::string xFuncExpress = "t";
//     std::string yFuncExpress = "5*t^2";
    std::string zFuncExpress = "0.0";
    std::string varName = "t";
    double startU = 0;
    double endU = 2.0*3.1415926;

    if (!builder.createFuncCurve(xFuncExpress, yFuncExpress, zFuncExpress, varName, startU, endU))
    {
    }
    COMMAND_END;
    TopoDS_HShape* hShape = new TopoDS_HShape(builder.getShape());

    showInView(hShape);
    m_entityList.push_back(hShape);

    Standard_Boolean ok = BRepTools::Write(builder.getShape(), "C:\\Users\\Administrator\\Desktop\\test.brep");
    if (!ok)
    {
        return;
    }
}

void MainWindow::on_toolButton_rectFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnRectWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_circleFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnEllipseWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_ellipseFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnEllipseWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_coneFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnConeWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_cylinderFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnCylinderWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_sectorFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMultiLinesWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_regPolyhedronFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnRegPolyhedronFaceWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_closeFace_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMultiLinesWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_nurbsFace_clicked() 
{
	DrawModelDlg* dlg = new DrawModelDlg(EnNurbsFaceWidget); 
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_box_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnCuboidWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_sphere_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnSphereWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_cylinder_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnCylinderWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_cone1_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnConeWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_torus_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnTorusWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_prism_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnPrismWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_flare_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnFlareWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_helixSolid_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnHelixSolidWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_extrude_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnExtrudeWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_revol_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnRevolWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_sweep_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnSweepWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_loft_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnLoftWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_thicken_clicked()  
{
    DrawModelDlg* dlg = new DrawModelDlg(EnThickenWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_fillet_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnFilletWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_chamfer_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnChamferWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_translate_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnTranslateWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_rotate_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnRotateWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_mirror_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnMirrorWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_scale_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnScaleWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_cut_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnCutWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_common_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnCommonWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_fuse_clicked()
{
    DrawModelDlg* dlg = new DrawModelDlg(EnFuseWidget);
    dlg->setPanel();
    dlg->show();
    connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_edgeOffset_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnEdgeOffsetWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_edgeExtend_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnEdgeExtendWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_edgeCombination_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnEdgeCombinationWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_edgeSplit_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnEdgeSplitWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_edgeIntersection_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnEdgeIntersectionWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_faceSplit_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnFaceSplitWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_faceExtend_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnFaceExtendWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_faceOffset_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnFaceOffsetWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_faceIntersection_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnFaceIntersectionWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_stitch_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnFaceStitchWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_errorShape1_clicked()
{
	TopoDS_Shape badShape;

#include <gp_Pln.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

	//shape1
// 	BRep_Builder B;
// 	TopoDS_Wire wire;
// 	B.MakeWire(wire);
// 
// 	gp_Pln plane(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
// 	BRepBuilderAPI_MakeFace faceBuilder(plane, 0.0, 1.0, 0.0, 1.0);
// 	faceBuilder.Build();
// 	if (!faceBuilder.IsDone())
// 	{
// 		return;
// 	}
// 	TopoDS_Shape targetShape = faceBuilder.Shape();
// 	int i = 0;
// 	for (TopExp_Explorer anExp(targetShape, TopAbs_EDGE); anExp.More(); anExp.Next())
// 	{
// 		++i;
// 		targetShape = anExp.Current();
// 		if (i % 2 == 0)
// 		{
// 			targetShape.Reverse();
// 		}
// 		B.Add(wire, targetShape);
// 	}
// 	badShape = wire;

	//shape2
// 	BRep_Builder B;
// 	TopoDS_Wire wire;
// 	B.MakeWire(wire);
// 
// 	gp_Pln plane(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
// 	BRepBuilderAPI_MakeFace faceBuilder(plane, 0.0, 1.0, 0.0, 1.0);
// 	faceBuilder.Build();
// 	if (!faceBuilder.IsDone())
// 	{
// 		return;
// 	}
// 	TopoDS_Shape targetShape = faceBuilder.Shape();
// 	int i = 0;
// 	for (TopExp_Explorer anExp(targetShape, TopAbs_EDGE); anExp.More(); anExp.Next())
// 	{
// 		++i;
// 		targetShape = anExp.Current();
// 		if (i % 2 == 0)
// 		{
// 			targetShape.Reverse();
// 		}
// 		B.Add(wire, targetShape);
// 	}
// 	TopoDS_Shape freeEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(0.1,0,0), gp_Pnt(0.2, 0, 0));
// 
// 	B.Add(wire, freeEdge);
// 	badShape = wire;

	//shape3
// 	BRep_Builder B;
// 	TopoDS_Shell shell;
// 	B.MakeShell(shell);
// 
// 	BRepPrimAPI_MakeBox faceBuilder(gp_Pnt(0, 0, 0), gp_Pnt(1, 1, 1));
// 	faceBuilder.Build();
// 	if (!faceBuilder.IsDone())
// 	{
// 		return;
// 	}
// 	TopoDS_Shape targetShape = faceBuilder.Shape();
// 	int i = 0;
// 	for (TopExp_Explorer anExp(targetShape, TopAbs_FACE); anExp.More(); anExp.Next())
// 	{
// 		++i;
// 		targetShape = anExp.Current();
// 		if (i % 2 == 0)
// 		{
// 			targetShape.Reverse();
// 		}
// 		B.Add(shell, targetShape);
// 	}
// 	badShape = shell;

	
	//shape4
// 	BRep_Builder B;
// 	TopoDS_Shell shell;
// 	B.MakeShell(shell);
// 
// 	BRepPrimAPI_MakeBox boxBuilder(gp_Pnt(0, 0, 0), gp_Pnt(1, 1, 1));
// 	boxBuilder.Build();
// 	if (!boxBuilder.IsDone())
// 	{
// 		return;
// 	}
// 	TopoDS_Shape targetShape = boxBuilder.Shape();
// 	int i = 0;
// 	for (TopExp_Explorer anExp(targetShape, TopAbs_FACE); anExp.More(); anExp.Next())
// 	{
// 		++i;
// 		targetShape = anExp.Current();
// 		if (i % 2 == 0)
// 		{
// 			targetShape.Reverse();
// 		}
// 		B.Add(shell, targetShape);
// 	}
// 	gp_Pln plane(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
// 	BRepBuilderAPI_MakeFace faceBuilder(plane, 0.0, 0.1, 0.0, 0.1);
// 	faceBuilder.Build();
// 	if (!faceBuilder.IsDone())
// 	{
// 		return;
// 	}
// 	
// 	badShape = shell;

	//shape5
// 	BRep_Builder B;
// 	TopoDS_Shell shell;
// 	B.MakeShell(shell);
// 	
// 	BRepPrimAPI_MakeBox boxBuilder(gp_Pnt(0, 0, 0), gp_Pnt(1, 1, 1));
// 	boxBuilder.Build();
// 	if (!boxBuilder.IsDone())
// 	{
// 		return;
// 	}
// 	TopoDS_Shape targetShape = boxBuilder.Shape();
// 	int i = 0;
// 	for (TopExp_Explorer anExp(targetShape, TopAbs_FACE); anExp.More(); anExp.Next())
// 	{
// 		++i;
// 		targetShape = anExp.Current();
// 		if (i % 2 == 0)
// 		{
// 			targetShape.Reverse();
// 		}
// 		B.Add(shell, targetShape);
// 	}
// 	gp_Pln plane(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
// 	BRepBuilderAPI_MakeFace faceBuilder(plane, 0.0, 0.1, 0.0, 0.1);
// 	faceBuilder.Build();
// 	if (!faceBuilder.IsDone())
// 	{
// 		return;
// 	}
// 	
// 	TopoDS_Shape freeFace = faceBuilder.Shape();
// 	B.Add(shell, freeFace);
// 	badShape = shell;

	//shape6
// 	BRep_Builder B;
// 	TopoDS_Wire wire;
// 	B.MakeWire(wire);
// 	TopoDS_Shape edge1 = BRepBuilderAPI_MakeEdge(gp_Pnt(1, 0, 0), gp_Pnt(0, 0, 0));
// 	TopoDS_Shape edge2 = BRepBuilderAPI_MakeEdge(gp_Pnt(-0.1, 0, 0), gp_Pnt(-0.1, 1, 0));
// 	B.Add(wire, edge1);
// 	B.Add(wire, edge2);
// 	badShape = wire;

	//shape7
	//build by xdModeling

	//shape8
	BRep_Builder B;
	TopoDS_Shell shell;
	BRepPrimAPI_MakeBox boxBuilder(gp_Pnt(0, 0, 0), gp_Pnt(1, 1, 1));
	boxBuilder.Build();
	if (!boxBuilder.IsDone())
	{
		return;
	}
	TopoDS_Shape targetShape = boxBuilder.Shape();
	for (TopExp_Explorer anExp(targetShape, TopAbs_SHELL); anExp.More(); anExp.Next())
	{
		shell = TopoDS::Shell(anExp.Value());
		break;
	}

	TopoDS_Shape theOldFace;
	int i = 0;
	for (TopExp_Explorer anExp(shell, TopAbs_FACE); anExp.More(); anExp.Next(),i++)
	{
		if (i == 4)
		{
			theOldFace = TopoDS::Face(anExp.Value());
			break;
		}
	}
	gp_Pln plane(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1));
	BRepBuilderAPI_MakeFace faceBuilder(plane, 0.0, 0.9, 0.0, 0.9);
	faceBuilder.Build();
	if (!faceBuilder.IsDone())
	{
		return;
	}

	TopoDS_Shape freeFace = faceBuilder.Shape();

	Handle(ShapeBuild_ReShape) rebuild = new ShapeBuild_ReShape;
	rebuild->Replace(theOldFace, freeFace);
	badShape = rebuild->Apply(shell, TopAbs_FACE);











	Standard_CString aFile = "D:/badShape.brep";

	Standard_Boolean ok = BRepTools::Write(badShape, aFile);
	if (!ok)
	{
		return;
	}
}

void MainWindow::on_toolButton_fix_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnFixWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_Measurement_clicked()
{
	DrawModelDlg* dlg = new DrawModelDlg(EnMeasurementWidget);
	dlg->setPanel();
	dlg->show();
	connect(dlg, &DrawModelDlg::startCreate, this, &MainWindow::onCreateNewModel);
}

void MainWindow::on_toolButton_inModelXDE_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "", "", "model (*.step *.stp *.iges *.igs)");
    if (path.isEmpty())
        return;

    std::vector<void*> newEntities;
    int numModel;
    TheOCCApp.getCurrentModel()->ImportGeomModel(NULL, path.toStdString(), numModel, newEntities);

    COMMAND_BEGIN

#ifdef TestProgram
        //method1 for show
        TDocStd_Document* doc = TheOCCApp.getCurrentModel()->getDoc();
    Handle(TDF_Data) data = doc->GetData();


    for (int i = 0; i < newEntities.size(); ++i)
    {
        std::string strPID;
        if (!TheOCCApp.getCurrentModel()->findShapePID(newEntities[i], strPID))
        {
            continue;
        }

        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);

        if (!label.IsNull())
        {
            Handle(TNaming_NamedShape) ObjectNS;
            if (label.FindAttribute(TNaming_NamedShape::GetID(), ObjectNS))
            {
                Handle(TPrsStd_AISPresentation) prs;
                prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
                prs->SetTransparency(transparency);
                prs->Display(1);
            }
        }
    }

    OccView* view = m_occ;
    if (view)
    {
        Handle(AIS_InteractiveContext) aContext = view->getContext();
        aContext->UpdateCurrentViewer();
    }
    view->fitAll();

#else
        //method2 for show
        OccView* view = m_occ;
    if (view)
    {
        Handle(AIS_InteractiveContext) aContext = view->getContext();
        for (auto shape : newEntities)
        {
            //method2
            TopoDS_Shape realShape = ((TopoDS_HShape*)shape)->Shape();
            Handle(AIS_Shape) displayedShape = new AIS_Shape(realShape);
            aContext->Display(displayedShape, true);
        }
        view->fitAll();
    }
#endif
    COMMAND_END
}

void MainWindow::on_toolButton_inModel_clicked()
{
    QStringList pathList = QFileDialog::getOpenFileNames(this, "", "", "model (*.step *.stp *.iges *.igs *.brep)");
    if (pathList.size() == 0)
    {
        return;
    }

    COMMAND_BEGIN

        QStringList strPIDList;
    for (QString path : pathList)
    {
        QFileInfo fileInfo(path);
        OccModelBuilder::EnFileType type;
        if (fileInfo.suffix() == "step" || fileInfo.suffix() == "stp")
        {
            type = OccModelBuilder::ENSTP;
        }
        else if (fileInfo.suffix() == "iges" || fileInfo.suffix() == "igs")
        {
            type = OccModelBuilder::ENIGS;
        }
        else if (fileInfo.suffix() == "brep")
        {
            type = OccModelBuilder::ENBREP;
        }
        else
        {
            return;
        }

        std::string name = "", strPID;
        OccModelBuilder builder;
        if (!builder.importModel(path.toStdString(), name.c_str(), type, strPID))
        {
            return;
        }
        strPIDList << QString::fromStdString(strPID);
    }

#ifdef TestProgram
    //method1 for show
    TDocStd_Document* doc = TheOCCApp.getCurrentModel()->getDoc();
    Handle(TDF_Data) data = doc->GetData();

    for (QString strPID : strPIDList)
    {
        TDF_Label label;
        TDF_Tool::Label(data, TCollection_AsciiString(strPID.toStdString().c_str()), label, false);

        if (!label.IsNull())
        {
            Handle(TNaming_NamedShape) ObjectNS;
            if (label.FindAttribute(TNaming_NamedShape::GetID(), ObjectNS))
            {
                Handle(TPrsStd_AISPresentation) prs;
                prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
                prs->SetTransparency(transparency);
                prs->Display(1);
            }
        }

        OccView* view = m_occ;
        if (view)
        {
            Handle(AIS_InteractiveContext) aContext = view->getContext();
            aContext->UpdateCurrentViewer();
        }
        view->fitAll();

#else
    //method2 for show
    OccView* view = m_occ;
    if (view)
    {
        Handle(AIS_InteractiveContext) aContext = view->getContext();
        for (auto shape : newEntities)
        {
            //method2
            TopoDS_Shape realShape = ((TopoDS_HShape*)shape)->Shape();
            Handle(AIS_Shape) displayedShape = new AIS_Shape(realShape);
            aContext->Display(displayedShape, true);
        }
        view->fitAll();
    }
#endif
    }
    COMMAND_END


//     QString path = QFileDialog::getOpenFileName(this, "", "", "model (*.step *.stp *.iges *.igs *.brep)");
//     if (path.isEmpty())
//         return;
// 
//     QFileInfo fileInfo(path);
//     OccModelBuilder::EnFileType type;
//     if (fileInfo.suffix() == "step" || fileInfo.suffix() == "stp")
//     {
//         type = OccModelBuilder::ENSTP;
//     }
//     else if (fileInfo.suffix() == "iges" || fileInfo.suffix() == "igs")
//     {
//         type = OccModelBuilder::ENIGS;
//     }
//     else if (fileInfo.suffix() == "brep")
//     {
//         type = OccModelBuilder::ENBREP;
//     }
//     else
//     {
//         return;
//     }
// 
//     std::string name = "", strPID;
//     OccModelBuilder builder;
//     if (!builder.importModel(path.toStdString(), name.c_str(), type, strPID))
//     {
//         return;
//     }
// 
// 
//     COMMAND_BEGIN
// 
// #ifdef TestProgram
//         //method1 for show
//         TDocStd_Document* doc = TheOCCApp.getCurrentModel()->getDoc();
//     Handle(TDF_Data) data = doc->GetData();
// 
// 
//     TDF_Label label;
//     TDF_Tool::Label(data, TCollection_AsciiString(strPID.c_str()), label, false);
// 
//     if (!label.IsNull())
//     {
//         Handle(TNaming_NamedShape) ObjectNS;
//         if (label.FindAttribute(TNaming_NamedShape::GetID(), ObjectNS))
//         {
//             Handle(TPrsStd_AISPresentation) prs;
//             prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
//             prs->SetTransparency(transparency);
//             prs->Display(1);
//         }
//     }
// 
//     OccView* view = m_occ;
//     if (view)
//     {
//         Handle(AIS_InteractiveContext) aContext = view->getContext();
//         aContext->UpdateCurrentViewer();
//     }
//     view->fitAll();
// 
// #else
//         //method2 for show
//         OccView* view = m_occ;
//     if (view)
//     {
//         Handle(AIS_InteractiveContext) aContext = view->getContext();
//         for (auto shape : newEntities)
//         {
//             //method2
//             TopoDS_Shape realShape = ((TopoDS_HShape*)shape)->Shape();
//             Handle(AIS_Shape) displayedShape = new AIS_Shape(realShape);
//             aContext->Display(displayedShape, true);
//         }
//         view->fitAll();
//     }
// #endif
//     COMMAND_END
}

void MainWindow::on_toolButton_outModel_clicked()
{
    ExportModelDlg* dlg = new ExportModelDlg;
    dlg->show();  
}

void MainWindow::on_toolButton_undo_clicked()
{
#ifdef TestProgram
    //method 1
    TDocStd_Document* doc = TheOCCApp.getCurrentModel()->getDoc();
    doc->Undo();

#else
    //method 2
    TheOCCApp.getCurrentModel()->undo();
#endif

    OccView* view = m_occ;
    if (view)
    {
        Handle(AIS_InteractiveContext) aContext = view->getContext();
        aContext->UpdateCurrentViewer();
    }
}

void MainWindow::on_toolButton_redo_clicked()
{
#ifdef TestProgram
    //method 1
    TDocStd_Document* doc = TheOCCApp.getCurrentModel()->getDoc();
    doc->Redo();

#else
    //method 2
    TheOCCApp.getCurrentModel()->redo();
#endif

    OccView* view = m_occ;
    if (view)
    {
        Handle(AIS_InteractiveContext) aContext = view->getContext();
        aContext->UpdateCurrentViewer();
    }
}

void MainWindow::on_toolButton_saveas_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "save OCAF file", "C:\\Users\\Administrator\\Desktop\\testProgram.cbf", "*.cbf");
    if (!filePath.isEmpty())
    {
        OccApplication::instance()->saveAs(m_docID, filePath.toStdString().c_str());
    }
}

void MainWindow::on_toolButton_open_clicked()
{
//     m_doc->NewCommand();
// 
//     TDF_Label CylinderLabel = TDF_TagSource::NewChild(m_rootLabel);
// 
//     int m = CylinderLabel.Tag();
//     BRepPrimAPI_MakeBox FaireBox(gp_Pnt(1, 6, 1), 5, 10, 3);
//     TopoDS_Solid cylinder_shape = FaireBox.Solid();
// 
//     TopExp_Explorer Ex;
//     for (Ex.Init(cylinder_shape, TopAbs_FACE); Ex.More(); Ex.Next())
//     {
//         TDF_Label faceLabel = CylinderLabel.FindChild(1);
//         TNaming_Builder Box1Ins(faceLabel);
//         Box1Ins.Generated(Ex.Value());
//     }
// 
//     TNaming_Builder Box1Ins(CylinderLabel);
//     Box1Ins.Generated(cylinder_shape);
// 
//     Handle(TPrsStd_AISPresentation) prs;
//     prs = TPrsStd_AISPresentation::Set(CylinderLabel, TNaming_NamedShape::GetID());
// 
//     //  Displaying the box 
//     prs->Display(1);
//     aContext->UpdateCurrentViewer();
// 
//     m_doc->CommitCommand();
// 
//     m_occ->fitAll();
// 
//     labelList.push_back(CylinderLabel);
}

void MainWindow::on_toolButton_eraseModel_clicked()    
{
    COMMAND_BEGIN      
    EraseModelDlg* dlg = new EraseModelDlg;        
    dlg->show(); 
    m_occ->UpdateView();       
    COMMAND_END         
} 

void MainWindow::updateOccView() 
{
	OccModel* occModel = TheOCCApp.getCurrentModel();
	occModel->updateShapeListOverUndo();      

// 	std::list<TopoDS_Shape> addedShapeList = occModel->getAddedShapeList(); 
    std::list<void*> addedShapeList = occModel->getReturnAddedShapeList();
	for (auto shape : addedShapeList)
	{
        TopoDS_Shape realShape = ((TopoDS_HShape*)shape)->Shape();
        Handle(AIS_Shape) displayedShape = new AIS_Shape(realShape);
//         Handle(AIS_Shape) displayedShape = new AIS_Shape(shape);
		aContext->Display(displayedShape, true);
	}

	//遍历删除列表，获取所有删除的lable，进行渲染数据的删除
// 	std::list<TopoDS_Shape> deletedShapeList = occModel->getDeletedShapeList();
    std::list<void*> deletedShapeList = occModel->getReturnDeletedShapeList();
	AIS_ListOfInteractive objects;
	aContext->ObjectsInside(objects);
	AIS_ListIteratorOfListOfInteractive it;
	for (auto shape : deletedShapeList) 
	{
		for (it.Init(objects); it.More(); it.Next())
		{
            TopoDS_Shape realShape = ((TopoDS_HShape*)shape)->Shape();
			Handle(AIS_InteractiveObject) obj = it.Value();
			Handle(AIS_Shape) viewShape = Handle(AIS_Shape)::DownCast(obj);
            if (viewShape->Shape().IsSame(realShape))
			{
				aContext->Erase(obj, Standard_True); 
			}
		} 
	}

	aContext->UpdateCurrentViewer();
}

void MainWindow::showInView(void* entity)
{
    TopoDS_Shape realShape = ((TopoDS_HShape*)entity)->Shape();
    Handle(AIS_Shape) displayedShape = new AIS_Shape(realShape);
    aContext->Display(displayedShape, true);
    aContext->UpdateCurrentViewer();
    m_occ->fitAll();
}

void MainWindow::eraseTopodsShape(TopoDS_Shape shape)
{
	OccModel* occModel = TheOCCApp.getCurrentModel();

	AIS_ListOfInteractive objects;
	aContext->ObjectsInside(objects);
	AIS_ListIteratorOfListOfInteractive it;
	for (it.Init(objects); it.More(); it.Next())
	{
		Handle(AIS_InteractiveObject) obj = it.Value();
		Handle(AIS_Shape) viewShape = Handle(AIS_Shape)::DownCast(obj);
		if (viewShape->Shape().IsSame(shape))
		{
			aContext->Erase(obj, Standard_True);
		}
	}

	aContext->UpdateCurrentViewer();
}

void MainWindow::onCreateNewModel(DrawModelDlg* lineDlg)
{
    if (lineDlg)
    {
        COMMAND_BEGIN

            if (lineDlg->buildModel())
            {

                QStringList newNames = lineDlg->getNewNames();
                QStringList newPIDs = lineDlg->getNewPIDs();
                QVector<void*> newEntities = lineDlg->getNewEntities();

//                 TheOCCApp.getCurrentModel()->AddEntityAttribute(newEntities[0], "test", "aaaa");

#ifdef TestProgram
                //method1 for show
                TDocStd_Document* doc = TheOCCApp.getCurrentModel()->getDoc();
                Handle(TDF_Data) data = doc->GetData();

                for (int i = 0; i < newPIDs.size(); ++i)
                {
                    TDF_Label label;
                    TDF_Tool::Label(data, TCollection_AsciiString(newPIDs[i].toStdString().c_str()), label, false);

                    if (!label.IsNull())
                    {
                        Handle(TNaming_NamedShape) ObjectNS;
                        if (label.FindAttribute(TNaming_NamedShape::GetID(), ObjectNS))
                        {
                            Handle(TPrsStd_AISPresentation) prs;
                            prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
                            prs->SetTransparency(transparency);
                            prs->Display(1);
                        }
                    }
                }

                OccView* view = m_occ;
                if (view)
                {
                    Handle(AIS_InteractiveContext) aContext = view->getContext();
                    aContext->UpdateCurrentViewer();
                }
                view->fitAll();

#else
                //method2 for show
                OccView* view = m_occ;
                if (view)
                {
                    Handle(AIS_InteractiveContext) aContext = view->getContext();
                    for (auto shape : newEntities)
                    {
                        //method2
                        TopoDS_Shape realShape = ((TopoDS_HShape*)shape)->Shape();
                        Handle(AIS_Shape) displayedShape = new AIS_Shape(realShape);
                        aContext->Display(displayedShape, true);
                    }
                }
                view->fitAll();
#endif
				lineDlg->accept();
				if (lineDlg->getType() == EnMeasurementWidget)
				{
					if (newPIDs.size() > 0)
					{
						QMessageBox::information(nullptr, "Minimal Distance", lineDlg->getNewNames()[0]);
					}
					else
					{
						QMessageBox::information(nullptr, "Angle", lineDlg->getNewNames()[0]+" deg");
					}
				}
				else if (lineDlg->getType() == EnFixWidget)
				{
					ShapeFixWidget* fixWidget = dynamic_cast<ShapeFixWidget*>(lineDlg->getElemWidget());
					if (fixWidget)
					{
						QMessageBox::information(nullptr, "Analysis of shape before and after fixing", QString::fromStdString(fixWidget->getMessage()));
					}
				}
            }
        COMMAND_END

//         lineDlg->deleteLater();
    }
}
