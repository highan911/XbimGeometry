// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BRepAlgo_DSAccess_HeaderFile
#define _BRepAlgo_DSAccess_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_DefineAlloc_HeaderFile
#include <Standard_DefineAlloc.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Handle_TopOpeBRepDS_HDataStructure_HeaderFile
#include <Handle_TopOpeBRepDS_HDataStructure.hxx>
#endif
#ifndef _TopOpeBRep_DSFiller_HeaderFile
#include <TopOpeBRep_DSFiller.hxx>
#endif
#ifndef _Handle_TopOpeBRepBuild_HBuilder_HeaderFile
#include <Handle_TopOpeBRepBuild_HBuilder.hxx>
#endif
#ifndef _Handle_BRepAlgo_EdgeConnector_HeaderFile
#include <Handle_BRepAlgo_EdgeConnector.hxx>
#endif
#ifndef _TopoDS_Shape_HeaderFile
#include <TopoDS_Shape.hxx>
#endif
#ifndef _TopAbs_State_HeaderFile
#include <TopAbs_State.hxx>
#endif
#ifndef _TopTools_ListOfShape_HeaderFile
#include <TopTools_ListOfShape.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _TopoDS_Wire_HeaderFile
#include <TopoDS_Wire.hxx>
#endif
#ifndef _TColStd_ListOfInteger_HeaderFile
#include <TColStd_ListOfInteger.hxx>
#endif
#ifndef _TopTools_DataMapOfShapeShape_HeaderFile
#include <TopTools_DataMapOfShapeShape.hxx>
#endif
#ifndef _TColStd_SetOfInteger_HeaderFile
#include <TColStd_SetOfInteger.hxx>
#endif
#ifndef _BRepAlgo_CheckStatus_HeaderFile
#include <BRepAlgo_CheckStatus.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _TopOpeBRepDS_Kind_HeaderFile
#include <TopOpeBRepDS_Kind.hxx>
#endif
class TopOpeBRepDS_HDataStructure;
class TopOpeBRepBuild_HBuilder;
class BRepAlgo_EdgeConnector;
class BRepAlgo_BooleanOperations;
class TopoDS_Shape;
class TopTools_ListOfShape;
class TopoDS_Vertex;
class TColStd_ListOfInteger;



class BRepAlgo_DSAccess  {
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT   BRepAlgo_DSAccess();
  //! Clears the internal data structure, including the <br>
  Standard_EXPORT     void Init() ;
  //! Loads the shape in DS. <br>
//! <br>
  Standard_EXPORT     void Load(const TopoDS_Shape& S) ;
  //! Loads two shapes in the DS without intersecting them. <br>
  Standard_EXPORT     void Load(TopoDS_Shape& S1,TopoDS_Shape& S2) ;
  //! Intersects two shapes at input and loads the DS with <br>
//!          their intersection. Clears the TopOpeBRepBuild_HBuilder if <br>
//!          necessary <br>
  Standard_EXPORT     void Intersect() ;
  //! Intersects the faces contained in two given shapes <br>
//!          and loads them in the DS. Clears the TopOpeBRepBuild_HBuilder <br>
//!          if necessary <br>
  Standard_EXPORT     void Intersect(const TopoDS_Shape& S1,const TopoDS_Shape& S2) ;
  //! This method does the same thing as the previous, <br>
//!          but faster. There is no intersection face/face 3D. <br>
//!          The faces have the same support(surface). No test of <br>
//!          tangency (that is why it is faster). Intersects in 2d <br>
//!          the faces tangent F1 anf F2. <br>
  Standard_EXPORT     void SameDomain(const TopoDS_Shape& S1,const TopoDS_Shape& S2) ;
  //! returns compounds of Edge connected with section, which <br>
//!          contains sections between faces contained in S1 and S2. <br>
//!          returns an empty list of Shape if S1 or S2 do not contain <br>
//!          face. <br>
//!          calls GetSectionEdgeSet() if it has not already been done <br>
  Standard_EXPORT    const TopTools_ListOfShape& GetSectionEdgeSet(const TopoDS_Shape& S1,const TopoDS_Shape& S2) ;
  //! returns all compounds of edges connected with section <br>
//!          contained in the DS <br>
  Standard_EXPORT    const TopTools_ListOfShape& GetSectionEdgeSet() ;
  //! NYI <br>
  Standard_EXPORT     Standard_Boolean IsWire(const TopoDS_Shape& Compound) ;
  //! NYI <br>
  Standard_EXPORT    const TopoDS_Shape& Wire(const TopoDS_Shape& Compound) ;
  //! NYI <br>
//!          returns the vertex of section, which contains the section <br>
//!          between face S1 and edge S2 (returns an empty Shape <br>
//!          if S1 is not a face or if S2 is not an edge) <br>
  Standard_EXPORT    const TopTools_ListOfShape& SectionVertex(const TopoDS_Shape& S1,const TopoDS_Shape& S2) ;
  //! Invalidates a complete line of section. All <br>
//!          Edges connected by Vertex or a Wire. Can be <br>
//!          a group of connected Edges, which do not form a <br>
//!          standard Wire. <br>
  Standard_EXPORT     void SuppressEdgeSet(const TopoDS_Shape& Compound) ;
  //!  Modifies a line of section.  <New> -- should be a <br>
//!          Group of Edges connected by Vertex.  -- Can be a <br>
//!          Wire.  Can be a group of connected Edges that do not <br>
//!          form a standard Wire.   <New> should be sub-groupn of <Old> <br>
//! <br>
//! <br>
  Standard_EXPORT     void ChangeEdgeSet(const TopoDS_Shape& Old,const TopoDS_Shape& New) ;
  //! NYI <br>
//!          Make invalid a Vertex of section. The Vertex shoud be <br>
//!          reconstructed from a point. <br>
  Standard_EXPORT     void SuppressSectionVertex(const TopoDS_Vertex& V) ;
  
  Standard_EXPORT    const TopoDS_Shape& Merge(const TopAbs_State state1,const TopAbs_State state2) ;
  
  Standard_EXPORT    const TopoDS_Shape& Merge(const TopAbs_State state1) ;
  //!  NYI   Propagation  of a state starting from the shape <br>
//!          FromShape = edge or vertex of section, face or <br>
//!          Coumpound de section. LoadShape is either S1, <br>
//!          or S2  (see the method Load).   Propagation   from <br>
//!          FromShape, on the states <what> of LoadShape. <br>
//!          Return a Wire in 2d, a Shell in 3d. <br>
//!          Specifications are incomplete, to be redefined for the typologies <br>
//!          correpsonding to  <FromShape> and the result : <br>
//!          exemple :    FromShape        resultat <br>
//!                         vertex           wire (or edge) <br>
//!                    edge of section       face (or shell) <br>
//!                    compound of section   shell <br>
//!                      ...                  ... <br>
  Standard_EXPORT    const TopoDS_Shape& Propagate(const TopAbs_State what,const TopoDS_Shape& FromShape,const TopoDS_Shape& LoadShape) ;
  //! SectionShape est soit un Vertex de section(NYI), soit <br>
//!          une Edge de section. Propagation  des shapes <br>
//!          de section en partant de SectionShape. <br>
//!          return un Compound de section. <br>
  Standard_EXPORT    const TopoDS_Shape& PropagateFromSection(const TopoDS_Shape& SectionShape) ;
  //! Returns the list of the descendant shapes of the shape <S>. <br>
  Standard_EXPORT    const TopTools_ListOfShape& Modified(const TopoDS_Shape& S) ;
  //! Returns the fact that the shape <S> has been deleted or not <br>
//!          by the boolean operation. <br>
  Standard_EXPORT     Standard_Boolean IsDeleted(const TopoDS_Shape& S) ;
  //! NYI <br>
//!          coherence of the internal Data Structure. <br>
  Standard_EXPORT     BRepAlgo_CheckStatus Check() ;
  
  Standard_EXPORT    const Handle_TopOpeBRepDS_HDataStructure& DS() const;
  
  Standard_EXPORT     Handle_TopOpeBRepDS_HDataStructure& ChangeDS() ;
  
  Standard_EXPORT    const Handle_TopOpeBRepBuild_HBuilder& Builder() const;
  
  Standard_EXPORT     Handle_TopOpeBRepBuild_HBuilder& ChangeBuilder() ;


friend class BRepAlgo_BooleanOperations;



protected:





private:

  
  Standard_EXPORT     void Suppress(const TopoDS_Shape& Compound,const TopoDS_Shape& KeepComp) ;
  
  Standard_EXPORT     void RemoveEdgeInterferences(const Standard_Integer iF1,const Standard_Integer iF2,const Standard_Integer iCurve) ;
  
  Standard_EXPORT     void RemoveEdgeInterferences(const Standard_Integer iE1,const Standard_Integer iE2,const TopoDS_Shape& SectEdge) ;
  
  Standard_EXPORT     void RemoveFaceInterferences(const Standard_Integer iF1,const Standard_Integer iF2,const Standard_Integer iE1,const Standard_Integer iE2) ;
  
  Standard_EXPORT     void RemoveFaceInterferences(const Standard_Integer iF1,const Standard_Integer iF2,const Standard_Integer iCurve) ;
  
  Standard_EXPORT     void RemoveEdgeInterferencesFromFace(const Standard_Integer iF1,const Standard_Integer iF2,const Standard_Integer ipv1,const TopOpeBRepDS_Kind kind1,const Standard_Integer ipv2,const TopOpeBRepDS_Kind kind2) ;
  
  Standard_EXPORT     void RemoveEdgeFromFace(const Standard_Integer iF,const Standard_Integer iV) ;
  
  Standard_EXPORT     void PntVtxOnCurve(const Standard_Integer iCurve,Standard_Integer& ipv1,TopOpeBRepDS_Kind& ik1,Standard_Integer& ipv2,TopOpeBRepDS_Kind& ik2) ;
  
  Standard_EXPORT     void PntVtxOnSectEdge(const TopoDS_Shape& SectEdge,Standard_Integer& ipv1,TopOpeBRepDS_Kind& ik1,Standard_Integer& ipv2,TopOpeBRepDS_Kind& ik2) ;
  
  Standard_EXPORT     void RemoveEdgeSameDomain(const Standard_Integer iE1,const Standard_Integer iE2) ;
  
  Standard_EXPORT     void RemoveFaceSameDomain(const TopoDS_Shape& C) ;
  
  Standard_EXPORT     TColStd_ListOfInteger& FindGoodFace(const Standard_Integer iE,Standard_Integer& iF1,Standard_Boolean& b) ;
  
  Standard_EXPORT     void RemoveFaceSameDomain(const Standard_Integer iF1,const Standard_Integer iF2) ;
  
  Standard_EXPORT     Standard_Boolean GoodInterf(const TopoDS_Shape& SectEdge,const TopOpeBRepDS_Kind kind,const Standard_Integer iPointVertex) ;


Handle_TopOpeBRepDS_HDataStructure myHDS;
TopOpeBRep_DSFiller myDSFiller;
Handle_TopOpeBRepBuild_HBuilder myHB;
Handle_BRepAlgo_EdgeConnector myEC;
TopoDS_Shape myS1;
TopoDS_Shape myS2;
TopAbs_State myState1;
TopAbs_State myState2;
TopTools_ListOfShape myListOfCompoundOfEdgeConnected;
TopTools_ListOfShape myCurrentList;
Standard_Boolean myRecomputeBuilderIsDone;
Standard_Boolean myGetSectionIsDone;
TopoDS_Shape myResultShape;
TopoDS_Wire myWire;
TopTools_ListOfShape myListOfVertex;
TopTools_ListOfShape myModified;
TopoDS_Shape myEmptyShape;
TopTools_ListOfShape myEmptyListOfShape;
TColStd_ListOfInteger myEmptyListOfInteger;
TopTools_DataMapOfShapeShape myCompoundWireMap;
TColStd_SetOfInteger mySetOfKeepPoint;


};





// other Inline functions and methods (like "C++: function call" methods)


#endif