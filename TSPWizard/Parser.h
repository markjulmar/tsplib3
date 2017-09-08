// Parser.h - this parses our template files and writes out the project.
//

#pragma once
#include <afxtempl.h>

class CParser
{
// Class data
public:
	enum Token {
		IF,ELSE,ENDIF,PLACEHOLDER,LITERAL,COMMENT,EOL,BEGINLOOP,ENDLOOP,ENDOFFILE
	};
private:
	struct TokenPair
	{
	public:
		Token token;
		CString strData;
		TokenPair() {}
		TokenPair(Token tok, CString& str) : token(tok), strData(str) {}
	};

	const CMapStringToString& _convMap;
	bool _isWriting;
	const char* _pszCurr;
	CArray<bool> _ifStack;
	int _loopCount;
	int _currLoop;
	bool _inLoop;
	CArray<TokenPair> _loopInfo;

public:
	CParser(const CMapStringToString& convMap, const char* pszBuff);

// Methods
public:
	void SetData(const char* pszData) { _pszCurr = pszData; _isWriting = true; _ifStack.RemoveAll(); }
	bool GetNext(CString& strData); 

private:
	bool EvalCondition(CString strData);
	Token GetNextToken(const char*& pszBuff, CString& strData);
	void LookFor(const char*& pszBuff, char ch);
	void ProcessOneToken(CString& strData, Token token, CString& strInfo);
};
