#pragma once

// global functions applicable to raw resbuf usage but also used as helper
// functions for ArxDbgRbList class
int			dxfCodeToDataType(int resType);
void        dxfToStr(const resbuf* rb, CString& dxfCodeStr, CString& valueStr);
// Unicode: Leaving as char type because it is a buffer
LPCTSTR     bytesToHexStr(char* buffer, int len, CString& hexStr);
resbuf*     duplicateResbufNode(resbuf* rb);
resbuf*     duplicateResbufChain(resbuf* rb);
resbuf*     duplicateResbufChain(resbuf* startPtr, resbuf* endPtr);
resbuf*     ptArrayToResbuf(const AcGePoint3dArray& ptArray);
resbuf*     tailOfResbufChain(resbuf* const rb);
void        printResbufChain(resbuf* const rb);