// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Extrema_ECC2d_HeaderFile
#define _Extrema_ECC2d_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Real.hxx>
#include <math_Vector.hxx>
#include <Standard_Integer.hxx>
#include <TColStd_SequenceOfReal.hxx>
#include <Standard_Address.hxx>
#include <Standard_Boolean.hxx>
#include <Handle_TColgp_HArray1OfPnt2d.hxx>
class StdFail_InfiniteSolutions;
class StdFail_NotDone;
class Standard_OutOfRange;
class Adaptor2d_Curve2d;
class Extrema_Curve2dTool;
class TColgp_HArray1OfPnt2d;
class Extrema_POnCurv2d;
class gp_Pnt2d;
class gp_Vec2d;



class Extrema_ECC2d 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT Extrema_ECC2d();
  
  Standard_EXPORT Extrema_ECC2d(const Adaptor2d_Curve2d& C1, const Adaptor2d_Curve2d& C2);
  
  Standard_EXPORT Extrema_ECC2d(const Adaptor2d_Curve2d& C1, const Adaptor2d_Curve2d& C2, const Standard_Real Uinf, const Standard_Real Usup, const Standard_Real Vinf, const Standard_Real Vsup);
  
  Standard_EXPORT   void SetParams (const Adaptor2d_Curve2d& C1, const Adaptor2d_Curve2d& C2, const Standard_Real Uinf, const Standard_Real Usup, const Standard_Real Vinf, const Standard_Real Vsup) ;
  
  Standard_EXPORT   void SetTolerance (const Standard_Real Tol) ;
  
  Standard_EXPORT   void Perform() ;
  
  Standard_EXPORT   Standard_Boolean IsDone()  const;
  
  Standard_EXPORT   Standard_Integer NbExt()  const;
  
  Standard_EXPORT   Standard_Real SquareDistance (const Standard_Integer N = 1)  const;
  
  Standard_EXPORT   void Points (const Standard_Integer N, Extrema_POnCurv2d& P1, Extrema_POnCurv2d& P2)  const;




protected:





private:



  Standard_Real myCurveMinTol;
  math_Vector myLowBorder;
  math_Vector myUppBorder;
  Standard_Integer mySolCount;
  TColStd_SequenceOfReal myPoints1;
  TColStd_SequenceOfReal myPoints2;
  Standard_Address myC[2];
  Standard_Boolean myDone;


};







#endif // _Extrema_ECC2d_HeaderFile
