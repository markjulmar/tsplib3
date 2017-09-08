#include "stdafx.h"
#include "parser.h"

CParser::CParser(const CMapStringToString& convMap, const char* pszBuff) : _convMap(convMap)
{
	_isWriting = true;
	_pszCurr = pszBuff;
	_inLoop = false;
	_loopCount = 0;
}

CParser::Token CParser::GetNextToken(const char*& pszBuff, CString& strData)
{
	while (*pszBuff != '\0')
	{
		// End of line.
		if (*pszBuff == '\n')
		{
			if (strData.IsEmpty())
			{
				++pszBuff;
				return EOL;
			}
			else
				return LITERAL;
		}
		else if (*pszBuff == '\r')
			++pszBuff;

		// Starting a command?
		else if (*pszBuff == '$' && *(pszBuff+1) == '$')
		{
			if (!strData.IsEmpty())
				return LITERAL;

			if (!strnicmp(pszBuff, "$$IF(", 5))
			{
				pszBuff += 5;
				while (*pszBuff != '\0' && *pszBuff != ')')
					strData += *pszBuff++;
				LookFor(pszBuff, '\n');	++pszBuff;
				return IF;
			}
			else if (!strnicmp(pszBuff, "$$ELSE", 6))
			{
				pszBuff += 6;
				return ELSE;
			}
			else if (!strnicmp(pszBuff, "$$ENDIF", 7))
			{
				LookFor(pszBuff, '\n');	pszBuff++;
				return ENDIF;
			}
			else if (!strnicmp(pszBuff, "$$BEGINLOOP(", 12))
			{
				pszBuff += 12;
				while (*pszBuff != ')')
				{
					strData += *pszBuff;
					pszBuff++;
				}
				LookFor(pszBuff, '\n'); pszBuff++;
				return BEGINLOOP;
			}
			else if (!strnicmp(pszBuff, "$$ENDLOOP", 9))
			{
				LookFor(pszBuff, '\n'); pszBuff++;
				return ENDLOOP;
			}
			else if (!strnicmp(pszBuff, "$$//", 3))
			{
				LookFor(pszBuff, '\n');	pszBuff++;
				return COMMENT;
			}
			else if (!strnicmp(pszBuff, "$$", 2))
			{
				pszBuff += 2;
				while (*pszBuff != '\0' && !(*pszBuff == '$' && *(pszBuff+1) == '$'))
					strData += *pszBuff++;
				if (*pszBuff != '\0')
					pszBuff += 2;
				return PLACEHOLDER;
			}
		}

		// In a literal.
		else
		{
			strData += *pszBuff;
			++pszBuff;
		}
	}
	return ENDOFFILE;
}

void CParser::LookFor(const char*& pszBuff, char ch)
{
	while (*pszBuff != '\0' && *pszBuff != ch)
		pszBuff++;
}

bool CParser::EvalCondition(CString strData)
{
	strData.Trim();
	int pos = strData.Find(' ');
	if (pos == -1)
	{
		CString keyData;
		if (strData.Left(1) == '!') // not operator
			return (_convMap.Lookup(strData.Mid(1), keyData) == FALSE);
		else
			return (_convMap.Lookup(strData, keyData) == TRUE);
	}

	CString leftCond = strData.Left(pos);

	// Get the conditional
	strData = strData.Mid(pos+1); strData.TrimLeft();
	if (strData.GetLength() > 2 && (strData.Left(2) != "||" && strData.Left(2) != "&&"))
		return false;

	CString cond = strData.Left(2);
	strData = strData.Mid(3); strData.TrimLeft();

	bool firstCond = EvalCondition(leftCond);
	if (cond == "||" && firstCond)
		return true;

	return EvalCondition(strData);
}

bool CParser::GetNext(CString& strData)
{
	strData.Empty();

	while ((*_pszCurr != '\0') && strData.IsEmpty())
	{
		CString strInfo;
		Token token = GetNextToken(_pszCurr, strInfo);
		if (_inLoop)
		{
			if (token != ENDLOOP)
			{
				_loopInfo.Add(TokenPair(token, strInfo));
			}
			else
			{
				_inLoop = false;
				CString strLines;
				for (int i = 0; i < _loopCount; i++)
				{
					for (int x = 0; x < static_cast<int>(_loopInfo.GetCount()); x++)
					{
						strData.Empty();
						CString keyData = _loopInfo[x].strData;
						if (_loopInfo[x].token == PLACEHOLDER)
						{
							CString checkData;
							checkData.Format("_%d",i);
							keyData +=  checkData;
							if (_convMap.Lookup(keyData, checkData) == FALSE)
								keyData = _loopInfo[x].strData;
						}
						ProcessOneToken(strData, _loopInfo[x].token, keyData);
						strLines += strData;
					}
				}
				strData = strLines;
			}
		}
		else
		{
			ProcessOneToken(strData, token, strInfo);
		}
	}

	return (!strData.IsEmpty());
}

void CParser::ProcessOneToken(CString& strData, Token token, CString& strInfo)
{
	CString keyData;
	switch (token)
	{
		case LITERAL:
			if (!strInfo.IsEmpty() && _isWriting) 
				strData = strInfo;
			break;
		case EOL:
			if (_isWriting)
				strData = "\n";
			break;
		case ENDOFFILE:
		case COMMENT:
			break;
		case PLACEHOLDER:
			if (_isWriting)
			{
				if (_convMap.Lookup(strInfo, keyData) == TRUE)
					strData = keyData;
				else
					strData = strInfo;
			}
			break;
		case BEGINLOOP:
			_loopInfo.RemoveAll();
			if (_convMap.Lookup(strInfo, keyData) == TRUE)
				_loopCount = atol(keyData);
			else
				_loopCount = atol(strInfo);
			_inLoop = true;
			break;
		case ENDLOOP:
			_inLoop = false;
				break;
		case IF:
			_ifStack.Add(_isWriting);
			if (_isWriting && !EvalCondition(strInfo))
				_isWriting = false;
			break;
		case ELSE:
			_isWriting = !_isWriting;
			if (_isWriting == true)
			{
				// Get the previous state -- that way we ignore embedded $$IF statements.
				_isWriting = _ifStack[(int) _ifStack.GetCount()-1];
			}
			break;
		case ENDIF:
			{
				int count = (int) _ifStack.GetCount()-1;
				_isWriting = _ifStack[count];
				_ifStack.RemoveAt(count, 1);
			}
			break;
		default:
			break;
	}
}