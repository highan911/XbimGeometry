// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TColStd_ListOfInteger_HeaderFile
#define _TColStd_ListOfInteger_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Address.hxx>
#include <Standard_Integer.hxx>
#include <Handle_TColStd_ListNodeOfListOfInteger.hxx>
#include <Standard_Boolean.hxx>
class Standard_NoSuchObject;
class TColStd_ListIteratorOfListOfInteger;
class TColStd_ListNodeOfListOfInteger;



class TColStd_ListOfInteger 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT TColStd_ListOfInteger();
  
  Standard_EXPORT TColStd_ListOfInteger(const TColStd_ListOfInteger& Other);
  
  Standard_EXPORT   void Assign (const TColStd_ListOfInteger& Other) ;
  void operator= (const TColStd_ListOfInteger& Other) 
{
  Assign(Other);
}
  
  Standard_EXPORT   Standard_Integer Extent()  const;
  
  Standard_EXPORT   void Clear() ;
~TColStd_ListOfInteger()
{
  Clear();
}
  
      Standard_Boolean IsEmpty()  const;
  
  Standard_EXPORT   void Prepend (const Standard_Integer& I) ;
  
  Standard_EXPORT   void Prepend (const Standard_Integer& I, TColStd_ListIteratorOfListOfInteger& theIt) ;
  
  Standard_EXPORT   void Prepend (TColStd_ListOfInteger& Other) ;
  
  Standard_EXPORT   void Append (const Standard_Integer& I) ;
  
  Standard_EXPORT   void Append (const Standard_Integer& I, TColStd_ListIteratorOfListOfInteger& theIt) ;
  
  Standard_EXPORT   void Append (TColStd_ListOfInteger& Other) ;
  
  Standard_EXPORT   Standard_Integer& First()  const;
  
  Standard_EXPORT   Standard_Integer& Last()  const;
  
  Standard_EXPORT   void RemoveFirst() ;
  
  Standard_EXPORT   void Remove (TColStd_ListIteratorOfListOfInteger& It) ;
  
  Standard_EXPORT   void InsertBefore (const Standard_Integer& I, TColStd_ListIteratorOfListOfInteger& It) ;
  
  Standard_EXPORT   void InsertBefore (TColStd_ListOfInteger& Other, TColStd_ListIteratorOfListOfInteger& It) ;
  
  Standard_EXPORT   void InsertAfter (const Standard_Integer& I, TColStd_ListIteratorOfListOfInteger& It) ;
  
  Standard_EXPORT   void InsertAfter (TColStd_ListOfInteger& Other, TColStd_ListIteratorOfListOfInteger& It) ;


friend class TColStd_ListIteratorOfListOfInteger;


protected:





private:



  Standard_Address myFirst;
  Standard_Address myLast;


};

#define Item Standard_Integer
#define Item_hxx <Standard_Integer.hxx>
#define TCollection_ListNode TColStd_ListNodeOfListOfInteger
#define TCollection_ListNode_hxx <TColStd_ListNodeOfListOfInteger.hxx>
#define TCollection_ListIterator TColStd_ListIteratorOfListOfInteger
#define TCollection_ListIterator_hxx <TColStd_ListIteratorOfListOfInteger.hxx>
#define Handle_TCollection_ListNode Handle_TColStd_ListNodeOfListOfInteger
#define TCollection_ListNode_Type_() TColStd_ListNodeOfListOfInteger_Type_()
#define TCollection_List TColStd_ListOfInteger
#define TCollection_List_hxx <TColStd_ListOfInteger.hxx>

#include <TCollection_List.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_ListNode
#undef TCollection_ListNode_hxx
#undef TCollection_ListIterator
#undef TCollection_ListIterator_hxx
#undef Handle_TCollection_ListNode
#undef TCollection_ListNode_Type_
#undef TCollection_List
#undef TCollection_List_hxx




#endif // _TColStd_ListOfInteger_HeaderFile
