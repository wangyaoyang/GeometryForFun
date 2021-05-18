#include "stdafx.h"
#include <math.h>
#include <float.h>
#include "Calculat.h"

#define	MAX_ERROR_NO	16
static LPCSTR	szErrorMessage[] = 
{
	"合法的表达式",
	"表达式首出现了除 ‘-’或‘(’ 以外的任何一种运算符",
	"表达式中'('和')'数量不一致或不匹配",
	"表达式中'('和')'数量不一致或不匹配",
	"表达式尾出现了除 '=' 以外的任何一种运算符",
	"非右括号运算符后又出现了运算符",
	"运算符后出现了非左括号运算符",
	"左括号前出现了数值(变量)或右括号",
	"左括号后出现了非左括号的运算符",
	"右括号前出现了非右括号的运算符",
	"右括号后出现了数值(变量)或左括号",
	"等号前可以出现的运算符只有右括号",
	"等号后不应再出现表达式了",
	"表达式中出现了未知的运算符",
	"数值(变量)前出现了另一个数值(变量)或右括号",
	"数值(变量)后出现了另一个数值(变量)或左括号",
	///////////////////////////////////
	"未知错误",				//Last one
};

CStackElement::CStackElement(int Type)
{
	switch(Type)
	{
		case TYPE_CHAR:		m_pData = (char*)	new char;	*(char*)	m_pData = 0;	break;
		case TYPE_INT:		m_pData = (int*)	new int;	*(int*)		m_pData = 0;	break;
		case TYPE_FLOAT:	m_pData = (double*)	new double;	*(double*)	m_pData = 0.0f;	break;
		case TYPE_BOOL:		m_pData = (BOOL*)	new BOOL;	*(BOOL*)	m_pData = 0;	break;
		default:m_pData = (char*)	new char;	*(char*)	m_pData = 0;
	}
	m_pUp = NULL;
	m_pDn = NULL;
}

CStackElement::~CStackElement(void)
{
	delete m_pData;
	m_pData = NULL;
	m_pUp = NULL;
	m_pDn = NULL;
}

CStackOperation::CStackOperation(int Type)
{
	m_ElementType = Type;
	m_pStackTop = NULL;
	Initialize();
}

CStackOperation::~CStackOperation(void)
{
	Initialize();
}

BOOL CStackOperation::Initialize(void)
{
	CStackElement*	pTempElement = NULL;

	while(m_pStackTop) {
		pTempElement = m_pStackTop;
		m_pStackTop = m_pStackTop->m_pDn;
		delete pTempElement;
		if(m_pStackTop) m_pStackTop->m_pUp = pTempElement = NULL;
	}
	return TRUE;
}
		
CStackElement* CStackOperation::NewElement(void)
{
	if(m_pStackTop == NULL) {
		m_pStackTop = (CStackElement*) new CStackElement(m_ElementType);
	}
	else {
		m_pStackTop->m_pUp = (CStackElement*) new CStackElement(m_ElementType);
		m_pStackTop->m_pUp->m_pDn = m_pStackTop;
		m_pStackTop = m_pStackTop->m_pUp;
	}
	return m_pStackTop;
}

CStackElement* CStackOperation::DeleteElement(void)
{
	CStackElement*	pTempElement = NULL;

	if(m_pStackTop == NULL) return NULL;
	else {
		pTempElement = m_pStackTop;
		m_pStackTop = m_pStackTop->m_pDn;
		delete pTempElement;
		if(m_pStackTop) m_pStackTop->m_pUp = pTempElement = NULL;
		return m_pStackTop;
	}
	return NULL;
}

BOOL CStackOperation::PushStack(void* pData)
{
	m_pStackTop = NewElement();
	switch(m_ElementType)
	{
		case TYPE_CHAR:		*(char*   )	(m_pStackTop->m_pData) = *(char*  )	pData;	break;
		case TYPE_INT:		*(int*    )	(m_pStackTop->m_pData) = *(int*   )	pData;	break;
		case TYPE_FLOAT:	*(double*  )	(m_pStackTop->m_pData) = *(double* )	pData;	break;
		case TYPE_BOOL:		*(BOOL*   )	(m_pStackTop->m_pData) = *(BOOL*  )	pData;	break;
		default:	return FALSE;
	}
	return TRUE;
}

BOOL CStackOperation::GetStackTop(void* pData)
{
	switch(m_ElementType) {
		case TYPE_CHAR:		*(char*   )	pData = *(char*   )	m_pStackTop->m_pData;	break;
		case TYPE_INT:		*(int*    )	pData = *(int*    )	m_pStackTop->m_pData;	break;
		case TYPE_FLOAT:	*(double*  )	pData = *(double*  )	m_pStackTop->m_pData;	break;
		case TYPE_BOOL:		*(BOOL*   )	pData = *(BOOL*   )	m_pStackTop->m_pData;	break;
		default:	return FALSE;
	}
	return TRUE;
}

BOOL CStackOperation::PopStack(void* pData)
{
	if(pData) GetStackTop(pData);
	m_pStackTop = DeleteElement();
	return TRUE;
}

CCalculateElement::CCalculateElement(int Property)
{
	m_Property = Property;
	switch(m_Property) {
		case PROPERTY_OPT:		m_pData = (char*)	new char;	*(char*) 	m_pData = 0;	break;
		case PROPERTY_INT_VAR:
		case PROPERTY_INT:		m_pData = (int*)	new int;	*(int*)		m_pData = 0;	break;
		case PROPERTY_FLT_VAR:
		case PROPERTY_FLOAT:	m_pData = (double*)	new double;	*(double*)	m_pData = 0.0f;	break;
		case PROPERTY_BOOL_VAR:
		case PROPERTY_BOOL:		m_pData = (BOOL*)	new BOOL;	*(BOOL*)	m_pData = 0;	break;
		default:				m_pData = (char*)	new char;	*(char*)	m_pData = 0;
	}
	m_pPrev = NULL;
	m_pNext = NULL;
}

CCalculateElement::~CCalculateElement(void)
{
	delete m_pData;
	m_pPrev = NULL;
	m_pNext = NULL;
	m_Property = 0;
}

CCalculater::CCalculater(int Type)
{
	m_Type = Type;
	m_pCurrElement = &m_HeadElement;
}

CCalculater::~CCalculater(void)
{
	CCalculateElement*	pTempElement = NULL;

	for(m_pCurrElement = m_HeadElement.m_pNext;m_pCurrElement;) {
		pTempElement = m_pCurrElement;
		m_pCurrElement = m_pCurrElement->m_pNext;
		delete pTempElement;
	}
}

CCalculateElement* CCalculater::NewElement(int Type)
{
	m_pCurrElement->m_pNext = (CCalculateElement*) new CCalculateElement(Type);
	m_pCurrElement->m_pNext->m_pPrev = m_pCurrElement;
	m_pCurrElement = m_pCurrElement->m_pNext;
	return m_pCurrElement;
}

BOOL CCalculater::TranslateStringToQueue(void)
{
	char	CurrChar = 0;
	char	NumStr[128];
	int		SourceLength = 0;
	BOOL	JustReadaNumber = FALSE;
	BOOL	ReadEqualSymbol = FALSE;

	memset(NumStr, 0, 128);
	{
		int	nIndex = m_SourceString.Find('=');
		if( nIndex >=0 ) m_SourceString = m_SourceString.Left(nIndex);
		m_SourceString.Remove( ' ' );
		m_SourceString += '=';
		SourceLength = m_SourceString.GetLength();
	}
	for(int i = 0; i < SourceLength && !ReadEqualSymbol; i ++) {
		CurrChar = m_SourceString.GetAt(i);
		if( CurrChar == '[' || CurrChar == '{' ) CurrChar = '(';
		if( CurrChar == ']' || CurrChar == '}' ) CurrChar = ')';
		switch(CurrChar)
		{
		case '+':	case '-':	case '*':	case '/':
		case '^':	case '(':	case ')':	case '=':
			if( JustReadaNumber ) {
				NewElement( m_Type );
				switch(m_Type)
				{
				case TYPE_INT:		*(int*)		m_pCurrElement->m_pData = atoi(NumStr);			break;
				case TYPE_FLOAT:	*(double*)	m_pCurrElement->m_pData = (double)atof(NumStr);	break;
				case TYPE_BOOL:		*(BOOL*)	m_pCurrElement->m_pData = atoi(NumStr);			break;
				case TYPE_ELSE:
				default:;
				}
				memset(NumStr,0,128);
				JustReadaNumber = FALSE;
			}
			NewElement();
			*(char*) m_pCurrElement->m_pData = CurrChar;
			if( CurrChar == '=' ) ReadEqualSymbol = TRUE;
		break;
		case '1':	case '2':	case '3':	case '4':	case '5':	
		case '6':	case '7':	case '8':	case '9':	case '0':
		case '.':	sprintf(NumStr,"%s%c",NumStr,CurrChar);		JustReadaNumber = TRUE;
		break;
		default:	NewElement( -CurrChar );
		}
	}
	return TRUE;
}

int CCalculater::DetectQueue(void)
{
	CCalculateElement*	pTempElement = NULL;	
	m_pCurrElement = m_HeadElement.m_pNext;
	//	表达式首出现了除 ‘-’或‘(’ 以外的任何一种运算符
	if(	m_pCurrElement->m_Property == 0	&&
		*(char*)m_pCurrElement->m_pData != '-'	&&
		*(char*)m_pCurrElement->m_pData != '('	) return SYNTEX_ERR_FIRST_OPERATOR;
	//	表达式中'('和')'数量不一致或不匹配
	int IncludeNum = 0;
	for(IncludeNum = 0;	m_pCurrElement;	m_pCurrElement = m_pCurrElement->m_pNext) {
		if( m_pCurrElement->m_Property == 0 ) {
			if( *(char*)m_pCurrElement->m_pData == '(' ) IncludeNum ++;
			if( *(char*)m_pCurrElement->m_pData == ')' ) IncludeNum --;
			if( IncludeNum < 0 ) return SYNTEX_ERR_BRACKET_NOT_MATCH1;
		}
		pTempElement = m_pCurrElement;
	}
	if( IncludeNum != 0 ) return SYNTEX_ERR_BRACKET_NOT_MATCH2;
	//	表达式尾出现了除 '=' 以外的任何一种运算符
	if( !pTempElement || pTempElement->m_Property != 0 ||
		*(char*)pTempElement->m_pData != '=') return SYNTEX_ERR_NOT_A_EQUAL_SIGN;
	///////////////////////////////////////////////////////////////
	////////////////到此为止完成第一次扫描/////////////////////////
	///////////////////////////////////////////////////////////////
	m_pCurrElement = m_HeadElement.m_pNext;
	for(; m_pCurrElement; m_pCurrElement = m_pCurrElement->m_pNext) {
		if( m_pCurrElement->m_Property != 0 ) continue;
		switch( *(char*)m_pCurrElement->m_pData ) {
			case '+' :	case '-':	case '*':	case '/':	case '^':
				if( m_pCurrElement->m_pPrev->m_Property == 0	&&
					*(char*)m_pCurrElement->m_pPrev->m_pData != ')'	) return SYNTEX_ERR_OPERATOR_TWICE1;
					//	非右括号运算符后又出现了运算符
				if( m_pCurrElement->m_pNext->m_Property == 0	&&
					*(char*)m_pCurrElement->m_pNext->m_pData != '('	) return SYNTEX_ERR_OPERATOR_TWICE2;
					//	运算符后出现了非左括号运算符
			break;
			case '(':
				if( m_pCurrElement->m_pPrev->m_Property != 0	||
					*(char*)m_pCurrElement->m_pPrev->m_pData == ')'	) return SYNTEX_ERR_BEFORE_L_BRACKET;
					//	左括号前出现了非运算符或右括号
				if( m_pCurrElement->m_pNext->m_Property == 0	&&
					*(char*)m_pCurrElement->m_pNext->m_pData != '('	) return SYNTEX_ERR_AFTER_L_BRACKET;
					//	左括号后出现了非左括号的运算符
			break;
			case ')':
				if( m_pCurrElement->m_pPrev->m_Property == 0	&&
					*(char*)m_pCurrElement->m_pPrev->m_pData != ')'	) return SYNTEX_ERR_BEFORE_R_BRACKET;
					//	右括号前出现了非右括号的运算符
				if( m_pCurrElement->m_pNext->m_Property != 0	||
					*(char*)m_pCurrElement->m_pNext->m_pData == '('	) return SYNTEX_ERR_AFTER_R_BRACKET;
					//	右括号后出现了非运算符或左括号
			break;
			case '=':
				if( m_pCurrElement->m_pPrev->m_Property == 0	&&
					*(char*)m_pCurrElement->m_pPrev->m_pData != ')'	) return SYNTEX_ERR_BEFORE_EQUAL_SIGN;
					//	等号前可以出现的运算符只有右括号
				if( m_pCurrElement->m_pNext ) return SYNTEX_ERR_AFTER_EQUAL_SIGN;
					//	等号后不应再出现表达式了
			break;
			default:	return SYNTEX_ERR_UNKNOWN_OPERATOR;
					//	表达式中出现了未知的运算符
		}
	}
	///////////////////////////////////////////////////////////////
	////////////////到此为止完成第二次扫描/////////////////////////
	///////////////////////////////////////////////////////////////
	m_pCurrElement = m_HeadElement.m_pNext;
	for(; m_pCurrElement; m_pCurrElement = m_pCurrElement->m_pNext) {
		if( m_pCurrElement->m_Property == 0 ) continue;
		if( m_pCurrElement->m_pPrev->m_Property != 0	||
			*(char*)m_pCurrElement->m_pPrev->m_pData == ')'	) return SYNTEX_ERR_BEFORE_VALUE;
			//	数值前出现了另一个数值或右括号
		if( m_pCurrElement->m_pNext->m_Property != 0	||
			*(char*)m_pCurrElement->m_pNext->m_pData == '('	) return SYNTEX_ERR_AFTER_VALUE;
			//	数值后出现了另一个数值或左括号
	}
	///////////////////////////////////////////////////////////////
	////////////////到此为止完成第三次扫描/////////////////////////
	///////////////////////////////////////////////////////////////
	m_pCurrElement = m_HeadElement.m_pNext;
	return SYNTEX_OK;
}

char CCalculater::Precede(char left,char right)
{	//比较优先级
	char ls = 0,rs = 0;

	switch(left) {
		case '+' :	case '-':	ls = 21;	break;
		case '*' :	case '/':	ls = 31;	break;
		case '^' :				ls = 40;	break;
		case '(' :				ls = 10;	break;
		case ')' :				ls = 100;	break;
		case '=' :				ls = 0;		break;
		default:				ls = 0;
	}
	switch(right) {
		case '+' :	case '-':	rs = 20;	break;
		case '*' :	case '/':	rs = 30;	break;
		case '^' :				rs = 41;	break;
		case '(' :				rs = 100;	break;
		case ')' :				rs = 10;	break;
		case '=' :				rs = 0;		break;
		default:				rs = 0;
	}
	if( ls >  rs ) return '>';
	if( ls == rs ) return '=';
	if( ls <  rs ) return '<';
	return 0;
}

double CCalculater::DoubleTagOperate(double a,char operater,double b)
{
	switch(operater) {
		case '+' :	return( a+b );
		case '-' :	return( a-b );
		case '*' :	return( a*b );
		case '/' :	return( a/b );
		case '^' :	return( (double)pow(a,b) );
		default:	return( 0.0f );
	}
}
CString CCalculater::ErrorMessage(int errorNo)
{
	if( errorNo < 0 ) errorNo = 0;
	if( errorNo > MAX_ERROR_NO ) errorNo = MAX_ERROR_NO;
	return CString( szErrorMessage[errorNo] );
}

INT CCalculater::Calculate(double* Result,double X,double Y,double Z)
{
	int		QueueResult = 0;
	char	loperater = 0;
	char	roperater = 0;
	double	ltempdata = 0.0f;
	double	rtempdata = 0.0f;
	double	tmpresult = 0.0f;

	if(!TranslateStringToQueue()) return -1;
	if( (QueueResult= DetectQueue()) != 0 ) return QueueResult;
	
	m_pStackOptr = new CStackOperation;
	m_pStackData = new CStackOperation(2);

	loperater = '=';	m_pStackOptr->PushStack(&loperater);
	ltempdata = 0.0f;	m_pStackData->PushStack(&ltempdata);
	for(;m_pCurrElement;) {
		if( m_pCurrElement->m_Property != 0 ) {
			if( m_pCurrElement->m_Property < 0 )
			{
				switch( -m_pCurrElement->m_Property )
				{
				case 'a':	case 'A':	case 'x':	case 'X':	ltempdata = X;	break;
				case 'b':	case 'B':	case 'y':	case 'Y':	ltempdata = Y;	break;
				case 'c':	case 'C':	case 'z':	case 'Z':	ltempdata = Z;	break;
				default:										ltempdata = X;
				}
			}
			else switch( m_pCurrElement->m_Property )
			{
				case PROPERTY_INT:		ltempdata = (double)(*(int*)m_pCurrElement->m_pData);	break;
				case PROPERTY_FLOAT:	ltempdata = *(double*)m_pCurrElement->m_pData;			break;
				case PROPERTY_BOOL:		ltempdata = (double)(*(BOOL*)m_pCurrElement->m_pData);	break;
				case PROPERTY_OPT:
				default:;
			}
			m_pStackData->PushStack( &ltempdata );
			m_pCurrElement = m_pCurrElement->m_pNext;
		}
		else {
			m_pStackOptr->GetStackTop(&loperater);
			roperater = *(char*) m_pCurrElement->m_pData;
			switch( Precede( loperater, roperater ) ) {
				case '<' :
					m_pStackOptr->PushStack( &roperater );
					m_pCurrElement = m_pCurrElement->m_pNext;
				break;
				case '=' ://计算完成
					if( roperater == '=' ) {
						m_pStackData->PopStack(&tmpresult);
						*Result = tmpresult;
						delete m_pStackOptr;
						delete m_pStackData;
						if( _finite(tmpresult) ) return 0;
						else return SYNTEX_ERR_INFINITY_VALUE;
					}
					m_pStackOptr->PopStack();
					m_pCurrElement = m_pCurrElement->m_pNext;
				break;
				case '>' :
					m_pStackOptr->PopStack(&loperater);	
					m_pStackData->PopStack(&rtempdata);
					m_pStackData->PopStack(&ltempdata);
					tmpresult = DoubleTagOperate( ltempdata, loperater, rtempdata );
					m_pStackData->PushStack(&tmpresult);
				break;
				default:
					delete m_pStackOptr;
					delete m_pStackData;
					return 16;
			}
		}
	}
	delete m_pStackOptr;
	delete m_pStackData;
	return 17;
}
