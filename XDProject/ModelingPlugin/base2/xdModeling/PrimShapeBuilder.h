/*=========================================================================
Program:        XDP Toolkit
Module:         xdOccGeom
Name:           PrimShapeBuilder.h
Description:    ���ڶ�shape�������졢��ת��ɨ�ԡ������������
=========================================================================*/

#ifndef PRIMSHAPEBUILDER_H
#define PRIMSHAPEBUILDER_H

#include "OccShapeBuilder.h"
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <NCollection_DataMap.hxx>

class PrimShapeBuilder :
    public OccShapeBuilder
{
public:
    PrimShapeBuilder();
    ~PrimShapeBuilder();

    void* getBuilder() const { return m_builder; }
    void setBuilder(void* val) { m_builder = val; }
public:
    // ���촦��
    bool extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec);

    // ���촦��
    bool revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle = 360.0);

protected:
    void*                                   m_builder;

    BRepPrimAPI_MakePrism*                  m_extrude;
    BRepPrimAPI_MakeRevol*                  m_revol;
};

#endif