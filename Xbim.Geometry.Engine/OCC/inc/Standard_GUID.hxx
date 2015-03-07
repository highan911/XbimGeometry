// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Standard_GUID_HeaderFile
#define _Standard_GUID_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Integer.hxx>
#include <Standard_ExtCharacter.hxx>
#include <Standard_Byte.hxx>
#include <Standard_Storable.hxx>
#include <Standard_CString.hxx>
#include <Standard_ExtString.hxx>
#include <Standard_UUID.hxx>
#include <Standard_PCharacter.hxx>
#include <Standard_PExtCharacter.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_OStream.hxx>
#include <Standard_PrimitiveTypes.hxx>


Standard_EXPORT const Handle(Standard_Type)& STANDARD_TYPE(Standard_GUID);


class Standard_GUID 
{

public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT Standard_GUID();
  
  //! build a GUID from an ascii string with the
  //! following format:
  //! Length : 36 char
  //! "00000000-0000-0000-0000-000000000000"
  Standard_EXPORT Standard_GUID(const Standard_CString aGuid);
  
  //! build a GUID from an unicode string with the
  //! following format:
  //!
  //! "00000000-0000-0000-0000-000000000000"
  Standard_EXPORT Standard_GUID(const Standard_ExtString aGuid);
  
  Standard_EXPORT Standard_GUID(const Standard_Integer a32b, const Standard_ExtCharacter a16b1, const Standard_ExtCharacter a16b2, const Standard_ExtCharacter a16b3, const Standard_Byte a8b1, const Standard_Byte a8b2, const Standard_Byte a8b3, const Standard_Byte a8b4, const Standard_Byte a8b5, const Standard_Byte a8b6);
  
  Standard_EXPORT Standard_GUID(const Standard_UUID& aGuid);
  
  Standard_EXPORT Standard_GUID(const Standard_GUID& aGuid);
  
  Standard_EXPORT   Standard_UUID ToUUID()  const;
  
  //! translate the GUID into ascii string
  //! the aStrGuid is allocated by user.
  //! the guid have the following format:
  //!
  //! "00000000-0000-0000-0000-000000000000"
  Standard_EXPORT   void ToCString (const Standard_PCharacter aStrGuid)  const;
  
  //! translate the GUID into unicode string
  //! the aStrGuid is allocated by user.
  //! the guid have the following format:
  //!
  //! "00000000-0000-0000-0000-000000000000"
  Standard_EXPORT   void ToExtString (const Standard_PExtCharacter aStrGuid)  const;
  
  Standard_EXPORT   Standard_Boolean IsSame (const Standard_GUID& uid)  const;
  Standard_Boolean operator == (const Standard_GUID& uid)  const
{
  return IsSame(uid);
}
  
  Standard_EXPORT   Standard_Boolean IsNotSame (const Standard_GUID& uid)  const;
  Standard_Boolean operator != (const Standard_GUID& uid)  const
{
  return IsNotSame(uid);
}
  
  Standard_EXPORT   void Assign (const Standard_GUID& uid) ;
  void operator = (const Standard_GUID& uid) 
{
  Assign(uid);
}
  
  Standard_EXPORT   void Assign (const Standard_UUID& uid) ;
  void operator = (const Standard_UUID& uid) 
{
  Assign(uid);
}
  
  //! Display the GUID with the following format:
  //!
  //! "00000000-0000-0000-0000-000000000000"
  Standard_EXPORT   void ShallowDump (Standard_OStream& aStream)  const;
  
  //! Check the format of a GUID string.
  //! It checks the size, the position of the '-' and the correct size of fields.
  Standard_EXPORT static   Standard_Boolean CheckGUIDFormat (const Standard_CString aGuid) ;
  
  //! Hash function for GUID.
  Standard_EXPORT   Standard_Integer Hash (const Standard_Integer Upper)  const;
  
  //! H method used by collections.
  Standard_EXPORT static   Standard_Integer HashCode (const Standard_GUID& aguid, const Standard_Integer Upper) ;
  
  //! Returns True  when the two GUID are the same.
  Standard_EXPORT static   Standard_Boolean IsEqual (const Standard_GUID& string1, const Standard_GUID& string2) ;
    Standard_Integer _CSFDB_GetStandard_GUIDmy32b() const { return my32b; }
    void _CSFDB_SetStandard_GUIDmy32b(const Standard_Integer p) { my32b = p; }
    Standard_ExtCharacter _CSFDB_GetStandard_GUIDmy16b1() const { return my16b1; }
    void _CSFDB_SetStandard_GUIDmy16b1(const Standard_ExtCharacter p) { my16b1 = p; }
    Standard_ExtCharacter _CSFDB_GetStandard_GUIDmy16b2() const { return my16b2; }
    void _CSFDB_SetStandard_GUIDmy16b2(const Standard_ExtCharacter p) { my16b2 = p; }
    Standard_ExtCharacter _CSFDB_GetStandard_GUIDmy16b3() const { return my16b3; }
    void _CSFDB_SetStandard_GUIDmy16b3(const Standard_ExtCharacter p) { my16b3 = p; }
    Standard_Byte _CSFDB_GetStandard_GUIDmy8b1() const { return my8b1; }
    void _CSFDB_SetStandard_GUIDmy8b1(const Standard_Byte p) { my8b1 = p; }
    Standard_Byte _CSFDB_GetStandard_GUIDmy8b2() const { return my8b2; }
    void _CSFDB_SetStandard_GUIDmy8b2(const Standard_Byte p) { my8b2 = p; }
    Standard_Byte _CSFDB_GetStandard_GUIDmy8b3() const { return my8b3; }
    void _CSFDB_SetStandard_GUIDmy8b3(const Standard_Byte p) { my8b3 = p; }
    Standard_Byte _CSFDB_GetStandard_GUIDmy8b4() const { return my8b4; }
    void _CSFDB_SetStandard_GUIDmy8b4(const Standard_Byte p) { my8b4 = p; }
    Standard_Byte _CSFDB_GetStandard_GUIDmy8b5() const { return my8b5; }
    void _CSFDB_SetStandard_GUIDmy8b5(const Standard_Byte p) { my8b5 = p; }
    Standard_Byte _CSFDB_GetStandard_GUIDmy8b6() const { return my8b6; }
    void _CSFDB_SetStandard_GUIDmy8b6(const Standard_Byte p) { my8b6 = p; }



protected:




private: 


  Standard_Integer my32b;
  Standard_ExtCharacter my16b1;
  Standard_ExtCharacter my16b2;
  Standard_ExtCharacter my16b3;
  Standard_Byte my8b1;
  Standard_Byte my8b2;
  Standard_Byte my8b3;
  Standard_Byte my8b4;
  Standard_Byte my8b5;
  Standard_Byte my8b6;


};







#endif // _Standard_GUID_HeaderFile
