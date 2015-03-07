// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _gce_MakeElips2d_HeaderFile
#define _gce_MakeElips2d_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <gp_Elips2d.hxx>
#include <gce_Root.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
class StdFail_NotDone;
class gp_Ax2d;
class gp_Ax22d;
class gp_Pnt2d;
class gp_Elips2d;


//! This class implements the following algorithms used to
//! create Elips2d from gp.
//!
//! * Create an ellipse from its center, and two points:
//! one on the ciconference giving the major radius, the
//! other giving the value of the small radius.
//! * Create an ellipse from its major axis and its major
//! radius and its minor radius.
class gce_MakeElips2d  : public gce_Root
{
public:

  DEFINE_STANDARD_ALLOC

  

  //! Creates an ellipse with the major axis, the major and the
  //! minor radius. The location of the MajorAxis is the center
  //! of the  ellipse.
  //! The sense of parametrization is given by Sense.
  //! It is possible to create an ellipse with MajorRadius = MinorRadius.
  //! the status is "InvertRadius" if MajorRadius < MinorRadius or
  //! "NegativeRadius" if MinorRadius < 0.0
  Standard_EXPORT gce_MakeElips2d(const gp_Ax2d& MajorAxis, const Standard_Real MajorRadius, const Standard_Real MinorRadius, const Standard_Boolean Sense = Standard_True);
  

  //! Axis defines the Xaxis and Yaxis of the ellipse which defines
  //! the origin and the sense of parametrization.
  //! Creates an ellipse with the AxisPlacement the major and the
  //! minor radius. The location of Axis is the center
  //! of the  ellipse.
  //! It is possible to create an ellipse with MajorRadius = MinorRadius.
  //! the status is "InvertRadius" if MajorRadius < MinorRadius or
  //! "NegativeRadius" if MinorRadius < 0.0
  Standard_EXPORT gce_MakeElips2d(const gp_Ax22d& A, const Standard_Real MajorRadius, const Standard_Real MinorRadius);
  
  //! Makes an Elips2d with its center and two points.
  //! The sense of parametrization is given by S1, S2,
  //! and Center.
  //! Depending on the constructor, the  implicit orientation of the ellipse is:
  //! -   the sense defined by A,
  //! -   the sense defined by points Center, S1 and S2,
  //! -   the trigonometric sense if Sense is not given or is true, or
  //! -   the opposite if Sense is false.
  //! It is possible to construct an ellipse where the major
  //! and minor radii are equal.
  //! Warning
  //! If an error occurs (that is, when IsDone returns
  //! false), the Status function returns:
  //! -   gce_InvertRadius if MajorRadius is less than MinorRadius,
  //! -   gce_NegativeRadius if MajorRadius or
  //! MinorRadius is less than 0.0,
  //! -   gce_NullAxis if points S1, S2 and Center are collinear, or
  //! -   gce_InvertAxis if the major radius computed with
  //! Center and S1 is less than the minor radius
  //! computed with Center, S1 and S2.
  Standard_EXPORT gce_MakeElips2d(const gp_Pnt2d& S1, const gp_Pnt2d& S2, const gp_Pnt2d& Center);
  
  //! Returns the constructed ellipse.
  //! Exceptions StdFail_NotDone if no ellipse is constructed.
  Standard_EXPORT  const  gp_Elips2d& Value()  const;
  
  Standard_EXPORT  const  gp_Elips2d& Operator()  const;
Standard_EXPORT operator gp_Elips2d() const;




protected:





private:



  gp_Elips2d TheElips2d;


};







#endif // _gce_MakeElips2d_HeaderFile
