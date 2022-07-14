#ifndef TEST_H
#define TEST_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

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
#include <TDF_Data.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_XLinkTool.hxx> 
#include <CDF_Session.hxx> 
#include <AIS_InteractiveContext.hxx>

#include <vector>


class OccView;
class OccModel;
class DrawModelDlg;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void updateOccView();

    void showInView(void* entity);

	void eraseTopodsShape(TopoDS_Shape shape);

    public slots:
        void slotPushButton();
	void on_toolButton_point_clicked();
	void on_toolButton_line_clicked();
    void on_toolButton_polyline_clicked();
    void on_toolButton_spline_clicked();
    void on_toolButton_interSpline_clicked();
    void on_toolButton_threeArc_clicked();
    void on_toolButton_angArc_clicked();
    void on_toolButton_hyperbola_clicked();
    void on_toolButton_parabola_clicked();
    void on_toolButton_ellipse_clicked();
    void on_toolButton_spiral_clicked();
    void on_toolButton_helix_clicked();
    void on_toolButton_FuncCurve_clicked();


    void on_toolButton_rectFace_clicked();
    void on_toolButton_circleFace_clicked();
    void on_toolButton_ellipseFace_clicked();
    void on_toolButton_coneFace_clicked();
    void on_toolButton_cylinderFace_clicked();
    void on_toolButton_sectorFace_clicked();
    void on_toolButton_regPolyhedronFace_clicked();
    void on_toolButton_closeFace_clicked();
	void on_toolButton_nurbsFace_clicked();

    void on_toolButton_box_clicked();
    void on_toolButton_sphere_clicked();
    void on_toolButton_cylinder_clicked();
    void on_toolButton_cone1_clicked();
    void on_toolButton_torus_clicked();
    void on_toolButton_prism_clicked();
    void on_toolButton_flare_clicked();
    void on_toolButton_helixSolid_clicked();

    //old
    void on_toolButton_extrude_clicked();
    void on_toolButton_revol_clicked();
    void on_toolButton_sweep_clicked();
    void on_toolButton_loft_clicked();
    void on_toolButton_thicken_clicked();
    void on_toolButton_fillet_clicked();
    void on_toolButton_chamfer_clicked();

    void on_toolButton_translate_clicked();
    void on_toolButton_rotate_clicked();
    void on_toolButton_mirror_clicked();
    void on_toolButton_scale_clicked();

    void on_toolButton_cut_clicked();
	void on_toolButton_common_clicked();
	void on_toolButton_fuse_clicked();
//     void on_toolButton_nonreg_clicked();
//     void on_toolButton_fuzzy_clicked();

	void on_toolButton_edgeOffset_clicked();
	void on_toolButton_edgeExtend_clicked();
	void on_toolButton_edgeCombination_clicked();
	void on_toolButton_edgeSplit_clicked();
	void on_toolButton_edgeIntersection_clicked();

	void on_toolButton_faceSplit_clicked();
	void on_toolButton_faceExtend_clicked();
	void on_toolButton_faceOffset_clicked();
	void on_toolButton_faceIntersection_clicked();
	void on_toolButton_stitch_clicked();

	void on_toolButton_errorShape1_clicked();
	void on_toolButton_fix_clicked();

	void on_toolButton_Measurement_clicked();

    void on_toolButton_inModelXDE_clicked();
    void on_toolButton_inModel_clicked();
    void on_toolButton_outModel_clicked();

	void on_toolButton_undo_clicked();
	void on_toolButton_redo_clicked();

    void on_toolButton_saveas_clicked();
    void on_toolButton_open_clicked();

    void on_toolButton_eraseModel_clicked();

    void onCreateNewModel(DrawModelDlg* lineDlg);

private:
    Ui::MainWindowClass ui;

    OccView*            m_occ;
    TDocStd_Application* m_app;
    int                 m_docID = -1;
    OccModel*           m_model;
    Handle(AIS_InteractiveContext) aContext;


    Handle(TDF_Data)    m_DF;
    TDF_Label           m_rootLabel;
    Handle(TDocStd_Document) m_doc;

    QList<TDF_Label> labelList;
    std::vector<void*> m_entityList;

    float*              m_wcsMatrix;

};

#endif // TEST_H
