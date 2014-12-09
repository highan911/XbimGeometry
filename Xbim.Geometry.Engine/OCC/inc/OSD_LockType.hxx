// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _OSD_LockType_HeaderFile
#define _OSD_LockType_HeaderFile

//! locks for files. <br>
//!          NoLock is the default value when opening a file. <br>
//! <br>
//!          ReadLock allows only one reading of the file at a time. <br>
//! <br>
//!          WriteLock prevents others writing into a file(excepted the user <br>
//!          who puts the lock)but allows everybody to read. <br>
//! <br>
//!          ExclusiveLock prevents reading and writing except for the <br>
//!          current user of the file. <br>
//!          So ExclusiveLock means only one user on the file and this <br>
//!          user is the one who puts the lock. <br>
enum OSD_LockType {
OSD_NoLock,
OSD_ReadLock,
OSD_WriteLock,
OSD_ExclusiveLock
};

#ifndef _Standard_PrimitiveTypes_HeaderFile
#include <Standard_PrimitiveTypes.hxx>
#endif

#endif