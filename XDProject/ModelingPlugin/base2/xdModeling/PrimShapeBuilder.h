/*=========================================================================
Program:        XDP Toolkit
Module:         xdOccGeom
Name:           PrimShapeBuilder.h
Description:    用于对shape进行拉伸、旋转、扫略、放样处理的类
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
    // 拉伸处理
    bool extrudeOperation(TopoDS_Shape shape, const double& xVec, const double& yVec, const double& zVec);

    // 拉伸处理
    bool revolOperation(TopoDS_Shape shape, const double& xStart, const double& yStart, const double& zStart, const double& xVec, const double& yVec, const double& zVec, const double& angle = 360.0);

protected:
    void*                                   m_builder;

    BRepPrimAPI_MakePrism*                  m_extrude;
    BRepPrimAPI_MakeRevol*                  m_revol;
};

#endif