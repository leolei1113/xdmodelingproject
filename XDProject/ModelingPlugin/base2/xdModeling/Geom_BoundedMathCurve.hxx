#ifndef _Geom_BoundedMathCurve_HeaderFile
#define _Geom_BoundedMathCurve_HeaderFile

#include "exprtk.hpp"

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <Standard_Real.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Standard_Boolean.hxx>
#include <GeomAbs_Shape.hxx>
#include <Standard_Integer.hxx>
class Geom_Curve;
class Standard_ConstructionError;
class Standard_RangeError;
class Standard_NoSuchObject;
class Geom_UndefinedDerivative;
class Geom_UndefinedValue;
class gp_Pnt;
class gp_Vec;
class gp_Trsf;
class Geom_Geometry;


typedef exprtk::symbol_table<Standard_Real>            symbol_table_t;
typedef exprtk::expression<Standard_Real>              expression_t;
typedef exprtk::parser<Standard_Real>                  parser_t;


class Geom_BoundedMathCurve;
DEFINE_STANDARD_HANDLE(Geom_BoundedMathCurve, Geom_BoundedCurve)

class Geom_BoundedMathCurve : public Geom_BoundedCurve
{

public:

    Standard_EXPORT Geom_BoundedMathCurve(const std::string xFuncExpress, const std::string yFuncExpress, const std::string zFuncExpress, 
        const std::string varName, const Standard_Real startU, const Standard_Real endU);

    std::string getXFuncExpress() const { return m_xFuncExpress; }
    void setXFuncExpress(std::string val) { m_xFuncExpress = val; }
    std::string getYFuncExpress() const { return m_yFuncExpress; }
    void setYFuncExpress(std::string val) { m_yFuncExpress = val; }
    std::string getZFuncExpress() const { return m_zFuncExpress; }
    void setZFuncExpress(std::string val) { m_zFuncExpress = val; }
    std::string getVarName() const { return m_varName; }
    void setVarName(std::string val) { m_varName = val; }
    Standard_Boolean getReverseSymbol() const { return m_reverseSymbol; }
    void setReverseSymbol(Standard_Boolean val) { m_reverseSymbol = val; }

    //! Changes the orientation of this trimmed curve.
    //! As a result:
    //! - the basis curve is reversed,
    //! - the start point of the initial curve becomes the
    //! end point of the reversed curve,
    //! - the end point of the initial curve becomes the
    //! start point of the reversed curve,
    //! - the first and last parameters are recomputed.
    //! If the trimmed curve was defined by:
    //! - a basis curve whose parameter range is [ 0., 1. ],
    //! - the two trim values U1 (first parameter) and U2 (last parameter),
    //! the reversed trimmed curve is defined by:
    //! - the reversed basis curve, whose parameter range is still [ 0., 1. ],
    //! - the two trim values 1. - U2 (first parameter) and 1. - U1 (last parameter).
    Standard_EXPORT void Reverse() Standard_OVERRIDE;

    //! Computes the parameter on the reversed curve for
    //! the point of parameter U on this trimmed curve.
    Standard_EXPORT Standard_Real ReversedParameter(const Standard_Real U) const Standard_OVERRIDE;

    //! Returns the value of the first parameter of <me>.
    //! The first parameter is the parameter of the "StartPoint"
    //! of the trimmed curve.
    Standard_EXPORT Standard_Real FirstParameter() const Standard_OVERRIDE;

    //! Returns the value of the last parameter of <me>.
    //! The last parameter is the parameter of the "EndPoint" of the
    //! trimmed curve.
    Standard_EXPORT Standard_Real LastParameter() const Standard_OVERRIDE;

    //! Returns the start point of <me>.
    //! This point is the evaluation of the curve from the
    //! "FirstParameter".
    //! value and derivatives
    //! Warnings :
    //! The returned derivatives have the same orientation as the
    //! derivatives of the basis curve even if the trimmed curve
    //! has not the same orientation as the basis curve.
    Standard_EXPORT gp_Pnt StartPoint() const Standard_OVERRIDE;

    //! Returns the end point of <me>. This point is the
    //! evaluation of the curve for the "LastParameter".
    Standard_EXPORT gp_Pnt EndPoint() const Standard_OVERRIDE;

    //! Always returns FALSE
    Standard_EXPORT Standard_Boolean IsClosed() const Standard_OVERRIDE;

    //! Always returns FALSE
    Standard_EXPORT Standard_Boolean IsPeriodic() const Standard_OVERRIDE;

    //! Returns the continuity of the curve :
    //! C0 : only geometric continuity,
    //! C1 : continuity of the first derivative all along the Curve,
    //! C2 : continuity of the second derivative all along the Curve,
    //! C3 : continuity of the third derivative all along the Curve,
    //! CN : the order of continuity is infinite.
    Standard_EXPORT GeomAbs_Shape Continuity() const Standard_OVERRIDE;

    //! Returns true if the degree of continuity of the basis
    //! curve of this trimmed curve is at least N. A trimmed
    //! curve is at least "C0" continuous.
    //! Warnings :
    //! The continuity of the trimmed curve can be greater than
    //! the continuity of the basis curve because you consider
    //! only a part of the basis curve.
    //! Raised if N < 0.
    Standard_EXPORT Standard_Boolean IsCN(const Standard_Integer N) const Standard_OVERRIDE;

    //! Returns in P the point of parameter U.
    //!
    //! If the basis curve is an OffsetCurve sometimes it is not
    //! possible to do the evaluation of the curve at the parameter
    //! U (see class OffsetCurve).
    Standard_EXPORT void D0(const Standard_Real U, gp_Pnt& P) const Standard_OVERRIDE;

    //! Raised if the continuity of the curve is not C1.
    Standard_EXPORT void D1(const Standard_Real U, gp_Pnt& P, gp_Vec& V1) const Standard_OVERRIDE;

    //! Raised if the continuity of the curve is not C2.
    Standard_EXPORT void D2(const Standard_Real U, gp_Pnt& P, gp_Vec& V1, gp_Vec& V2) const Standard_OVERRIDE;

    //! Raised if the continuity of the curve is not C3.
    Standard_EXPORT void D3(const Standard_Real U, gp_Pnt& P, gp_Vec& V1, gp_Vec& V2, gp_Vec& V3) const Standard_OVERRIDE;

    //! N is the order of derivation.
    //! Raised if the continuity of the curve is not CN.
    //! Raised if N < 1.
    //! geometric transformations
    Standard_EXPORT gp_Vec DN(const Standard_Real U, const Standard_Integer N) const Standard_OVERRIDE;

    //! Applies the transformation T to this trimmed curve.
    //! Warning The basis curve is also modified.
    Standard_EXPORT void Transform(const gp_Trsf& T) Standard_OVERRIDE;

    //! Creates a new object which is a copy of this trimmed curve.
    Standard_EXPORT Handle(Geom_Geometry) Copy() const Standard_OVERRIDE;




    DEFINE_STANDARD_RTTIEXT(Geom_BoundedMathCurve, Geom_BoundedCurve)

protected:
    std::string strFuncMultiplyValue(const std::string origFunc, const Standard_Real value);


private:
    Standard_Real                                   m_startU;
    Standard_Real                                   m_endU;



    GeomAbs_Shape                                   m_smooth;

    Standard_Boolean                                m_reverseSymbol;

    parser_t                                        m_parser;

    symbol_table_t                                  m_symbolTable;
    expression_t                                    m_xExpression;
    expression_t                                    m_yExpression;
    expression_t                                    m_zExpression;
    std::string                                     m_xFuncExpress;
    std::string                                     m_yFuncExpress;
    std::string                                     m_zFuncExpress;
    std::string                                     m_varName;

//     symbol_table_t                                  m_revSymbolTable;
//     expression_t                                    m_xRevExpression;
//     expression_t                                    m_yRevExpression;
//     expression_t                                    m_zRevExpression;
//     std::string                                     m_xRevFuncExpress;
//     std::string                                     m_yRevFuncExpress;
//     std::string                                     m_zRevFuncExpress;
//     std::string                                     m_revVarName;
};







#endif // _Geom_BoundedMathCurve_HeaderFile
