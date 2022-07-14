// Created on: 1993-03-10
// Created by: JCV
// Copyright (c) 1993-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _Geom_BoundedMathSurface_HeaderFile
#define _Geom_BoundedMathSurface_HeaderFile

#include "exprtk.hpp"

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <Geom_BoundedSurface.hxx>
#include <Geom_Curve.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <GeomAbs_Shape.hxx>
#include <Standard_Integer.hxx>
class Standard_RangeError;
class Standard_NoSuchObject;
class Geom_UndefinedDerivative;
class Geom_UndefinedValue;
class gp_Trsf;
class gp_GTrsf2d;
class gp_Pnt;
class gp_Vec;

typedef exprtk::symbol_table<Standard_Real>            symbol_table_t;
typedef exprtk::expression<Standard_Real>              expression_t;
typedef exprtk::parser<Standard_Real>                  parser_t;

class Geom_BoundedMathSurface;
DEFINE_STANDARD_HANDLE(Geom_BoundedMathSurface, Geom_BoundedSurface)

//! Describes the common behavior of surfaces in 3D
//! space. The Geom package provides many
//! implementations of concrete derived surfaces, such as
//! planes, cylinders, cones, spheres and tori, surfaces of
//! linear extrusion, surfaces of revolution, Bezier and
//! BSpline surfaces, and so on.
//! The key characteristic of these surfaces is that they
//! are parameterized. Geom_BoundedMathSurface demonstrates:
//! - how to work with the parametric equation of a
//! surface to compute the point of parameters (u,
//! v), and, at this point, the 1st, 2nd ... Nth derivative,
//! - how to find global information about a surface in
//! each parametric direction (for example, level of
//! continuity, whether the surface is closed, its
//! periodicity, the bounds of the parameters and so on), and
//! - how the parameters change when geometric
//! transformations are applied to the surface, or the
//! orientation is modified.
//! Note that all surfaces must have a geometric
//! continuity, and any surface is at least "C0". Generally,
//! continuity is checked at construction time or when the
//! curve is edited. Where this is not the case, the
//! documentation makes this explicit.
//! Warning
//! The Geom package does not prevent the construction of
//! surfaces with null areas, or surfaces which self-intersect.
class Geom_BoundedMathSurface : public Geom_BoundedSurface
{

public:

	Standard_EXPORT Geom_BoundedMathSurface(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress,
		const std::string UName, const std::string VName, const Standard_Real startU, const Standard_Real endU, const Standard_Real startV, const Standard_Real endV);

	std::string getXFuncExpress() const { return m_xFuncExpress; }
	void setXFuncExpress(std::string val) { m_xFuncExpress = val; }
	std::string getYFuncExpress() const { return m_yFuncExpress; }
	void setYFuncExpress(std::string val) { m_yFuncExpress = val; }
	std::string getZFuncExpress() const { return m_zFuncExpress; }
	void setZFuncExpress(std::string val) { m_zFuncExpress = val; }

	Standard_Boolean getReverseUSymbol() const { return m_reverseUSymbol; }
	void setReverseUSymbol(Standard_Boolean val) { m_reverseUSymbol = val; }
	Standard_Boolean getReverseVSymbol() const { return m_reverseVSymbol; }
	void setReverseVSymbol(Standard_Boolean val) { m_reverseVSymbol = val; }

	//! Reverses the U direction of parametrization of <me>.
	//! The bounds of the surface are not modified.
	Standard_EXPORT virtual void UReverse();

	//! Returns the  parameter on the  Ureversed surface for
	//! the point of parameter U on <me>.
	//!
	//! me->UReversed()->Value(me->UReversedParameter(U),V)
	//!
	//! is the same point as
	//!
	//! me->Value(U,V)
	Standard_EXPORT virtual Standard_Real UReversedParameter(const Standard_Real U) const;


	//! Reverses the V direction of parametrization of <me>.
	//! The bounds of the surface are not modified.
	Standard_EXPORT virtual void VReverse();

	//! Returns the  parameter on the  Vreversed surface for
	//! the point of parameter V on <me>.
	//!
	//! me->VReversed()->Value(U,me->VReversedParameter(V))
	//!
	//! is the same point as
	//!
	//! me->Value(U,V)
	Standard_EXPORT virtual Standard_Real VReversedParameter(const Standard_Real V) const;

	//! Returns the parametric bounds U1, U2, V1 and V2 of this surface.
	//! If the surface is infinite, this function can return a value
	//! equal to Precision::Infinite: instead of Standard_Real::LastReal.
	Standard_EXPORT virtual void Bounds(Standard_Real& U1, Standard_Real& U2, Standard_Real& V1, Standard_Real& V2) const;

	//! Checks whether this surface is closed in the u
	//! parametric direction.
	//! Returns true if, in the u parametric direction: taking
	//! uFirst and uLast as the parametric bounds in
	//! the u parametric direction, for each parameter v, the
	//! distance between the points P(uFirst, v) and
	//! P(uLast, v) is less than or equal to gp::Resolution().
	Standard_EXPORT virtual Standard_Boolean IsUClosed() const;

	//! Checks whether this surface is closed in the u
	//! parametric direction.
	//! Returns true if, in the v parametric
	//! direction: taking vFirst and vLast as the
	//! parametric bounds in the v parametric direction, for
	//! each parameter u, the distance between the points
	//! P(u, vFirst) and P(u, vLast) is less than
	//! or equal to gp::Resolution().
	Standard_EXPORT virtual Standard_Boolean IsVClosed() const;

	//! Checks if this surface is periodic in the u
	//! parametric direction. Returns true if:
	//! - this surface is closed in the u parametric direction, and
	//! - there is a constant T such that the distance
	//! between the points P (u, v) and P (u + T,
	//! v) (or the points P (u, v) and P (u, v +
	//! T)) is less than or equal to gp::Resolution().
	//! Note: T is the parametric period in the u parametric direction.
	Standard_EXPORT virtual Standard_Boolean IsUPeriodic() const;

	//! Checks if this surface is periodic in the v
	//! parametric direction. Returns true if:
	//! - this surface is closed in the v parametric direction, and
	//! - there is a constant T such that the distance
	//! between the points P (u, v) and P (u + T,
	//! v) (or the points P (u, v) and P (u, v +
	//! T)) is less than or equal to gp::Resolution().
	//! Note: T is the parametric period in the v parametric direction.
	Standard_EXPORT virtual Standard_Boolean IsVPeriodic() const;

	//! Computes the U isoparametric curve.
	Standard_EXPORT virtual Handle(Geom_Curve) UIso(const Standard_Real U) const;

	//! Computes the V isoparametric curve.
	Standard_EXPORT virtual Handle(Geom_Curve) VIso(const Standard_Real V) const;


	//! Returns the Global Continuity of the surface in direction U and V :
	//! C0 : only geometric continuity,
	//! C1 : continuity of the first derivative all along the surface,
	//! C2 : continuity of the second derivative all along the surface,
	//! C3 : continuity of the third derivative all along the surface,
	//! G1 : tangency continuity all along the surface,
	//! G2 : curvature continuity all along the surface,
	//! CN : the order of continuity is infinite.
	//! Example :
	//! If the surface is C1 in the V parametric direction and C2
	//! in the U parametric direction Shape = C1.
	Standard_EXPORT virtual GeomAbs_Shape Continuity() const;

	//! Returns the order of continuity of the surface in the
	//! U parametric direction.
	//! Raised if N < 0.
	Standard_EXPORT virtual Standard_Boolean IsCNu(const Standard_Integer N) const;

	//! Returns the order of continuity of the surface in the
	//! V parametric direction.
	//! Raised if N < 0.
	Standard_EXPORT virtual Standard_Boolean IsCNv(const Standard_Integer N) const;

	//! Computes the point of parameter U,V on the surface.
	//!
	//! Raised only for an "OffsetSurface" if it is not possible to
	//! compute the current point.
	Standard_EXPORT virtual void D0(const Standard_Real U, const Standard_Real V, gp_Pnt& P) const;


	//! Computes the point P and the first derivatives in the
	//! directions U and V at this point.
	//! Raised if the continuity of the surface is not C1.
	Standard_EXPORT virtual void D1(const Standard_Real U, const Standard_Real V, gp_Pnt& P, gp_Vec& D1U, gp_Vec& D1V) const;


	//! Computes the point P, the first and the second derivatives in
	//! the directions U and V at this point.
	//! Raised if the continuity of the surface is not C2.
	Standard_EXPORT virtual void D2(const Standard_Real U, const Standard_Real V, gp_Pnt& P, gp_Vec& D1U, gp_Vec& D1V, gp_Vec& D2U, gp_Vec& D2V, gp_Vec& D2UV) const;


	//! Computes the point P, the first,the second and the third
	//! derivatives in the directions U and V at this point.
	//! Raised if the continuity of the surface is not C2.
	Standard_EXPORT virtual void D3(const Standard_Real U, const Standard_Real V, gp_Pnt& P, gp_Vec& D1U, gp_Vec& D1V, gp_Vec& D2U, gp_Vec& D2V, gp_Vec& D2UV, gp_Vec& D3U, gp_Vec& D3V, gp_Vec& D3UUV, gp_Vec& D3UVV) const;

	//! ---Purpose ;
	//! Computes the derivative of order Nu in the direction U and Nv
	//! in the direction V at the point P(U, V).
	//!
	//! Raised if the continuity of the surface is not CNu in the U
	//! direction or not CNv in the V direction.
	//! Raised if Nu + Nv < 1 or Nu < 0 or Nv < 0.
	Standard_EXPORT virtual gp_Vec DN(const Standard_Real U, const Standard_Real V, const Standard_Integer Nu, const Standard_Integer Nv) const;


	//! Computes the point of parameter U on the surface.
	//!
	//! It is implemented with D0
	//!
	//! Raised only for an "OffsetSurface" if it is not possible to
	//! compute the current point.
	Standard_EXPORT gp_Pnt Value(const Standard_Real U, const Standard_Real V) const;


	//! Applies the transformation T to this trimmed curve.
	//! Warning The basis curve is also modified.
	Standard_EXPORT void Transform(const gp_Trsf& T) Standard_OVERRIDE;

	//! Creates a new object which is a copy of this trimmed curve.
	Standard_EXPORT Handle(Geom_Geometry) Copy() const Standard_OVERRIDE;


	DEFINE_STANDARD_RTTIEXT(Geom_BoundedMathSurface, Geom_BoundedSurface)

protected:
	std::string strFuncMultiplyValue(const std::string origFunc, const Standard_Real value);

	Standard_Real                                   m_startU;
	Standard_Real                                   m_endU;
	Standard_Real                                   m_startV;
	Standard_Real                                   m_endV;



	GeomAbs_Shape                                   m_smooth;

	Standard_Boolean                                m_reverseUSymbol;
	Standard_Boolean                                m_reverseVSymbol;

	parser_t                                        m_parser;

	//normal
	symbol_table_t                                  m_symbolTable;
	expression_t                                    m_xExpression;
	expression_t                                    m_yExpression;
	expression_t                                    m_zExpression;
	std::string                                     m_xFuncExpress;
	std::string                                     m_yFuncExpress;
	std::string                                     m_zFuncExpress;
	std::string                                     m_varUName;
	std::string                                     m_varVName;
	// 
	// 	//Urev, V
	// 	symbol_table_t                                  m_symbolTable_UrevV;
	// 	expression_t                                    m_xExpression_UrevV;
	// 	expression_t                                    m_yExpression_UrevV;
	// 	expression_t                                    m_zExpression_UrevV;
	// 	std::string                                     m_xFuncExpress_UrevV;
	// 	std::string                                     m_yFuncExpress_UrevV;
	// 	std::string                                     m_zFuncExpress_UrevV;
	// 	std::string                                     m_varUName_UrevV;
	// 	std::string                                     m_varVName_UrevV;
	// 
		//U, Vrev
	// 	symbol_table_t                                  m_symbolTable_UVrev;
	// 	expression_t                                    m_xExpression_UVrev;
	// 	expression_t                                    m_yExpression_UVrev;
	// 	expression_t                                    m_zExpression_UVrev;
	// 	std::string                                     m_xFuncExpress_UVrev;
	// 	std::string                                     m_yFuncExpress_UVrev;
	// 	std::string                                     m_zFuncExpress_UVrev;
	// 	std::string                                     m_varUName_UVrev;
	// 	std::string                                     m_varVName_UVrev;
	// 
		//Urev, Vrev
	// 	symbol_table_t                                  m_symbolTable_doubleRev;
	// 	expression_t                                    m_xExpression_doubleRev;
	// 	expression_t                                    m_yExpression_doubleRev;
	// 	expression_t                                    m_zExpression_doubleRev;
	// 	std::string                                     m_xFuncExpress_doubleRev;
	// 	std::string                                     m_yFuncExpress_doubleRev;
	// 	std::string                                     m_zFuncExpress_doubleRev;
	// 	std::string                                     m_varUName_doubleRev;
	// 	std::string                                     m_varVName_doubleRe};

};

#endif // _Geom_BoundedMathSurface_HeaderFile