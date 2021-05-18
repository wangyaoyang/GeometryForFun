#ifndef		__CALCULATER_CLASS_

#define	TYPE_CHAR			0
#define	TYPE_INT			1
#define	TYPE_FLOAT			2
#define	TYPE_BOOL			3
#define	TYPE_ELSE			4
#define	PROPERTY_OPT		0
#define	PROPERTY_INT		1
#define	PROPERTY_FLOAT		2
#define	PROPERTY_BOOL		3
#define	PROPERTY_ELSE		4
#define	PROPERTY_INT_VAR	-1
#define	PROPERTY_FLT_VAR	-2
#define	PROPERTY_BOOL_VAR	-3
#define	PROPERTY_ELSE_VAR	-4

#define	SYNTEX_OK						0	//	�Ϸ��ı��ʽ
#define	SYNTEX_ERR_FIRST_OPERATOR		1	//	���ʽ�׳����˳� ��-����(�� ������κ�һ�������
#define	SYNTEX_ERR_BRACKET_NOT_MATCH1	2	//	���ʽ��'('��')'������һ�»�ƥ��
#define	SYNTEX_ERR_BRACKET_NOT_MATCH2	3	//	���ʽ��'('��')'������һ�»�ƥ��
#define	SYNTEX_ERR_NOT_A_EQUAL_SIGN		4	//	���ʽβ�����˳� '=' ������κ�һ�������
#define	SYNTEX_ERR_OPERATOR_TWICE1		5	//	����������������ֳ����������
#define	SYNTEX_ERR_OPERATOR_TWICE2		6	//	�����������˷������������
#define	SYNTEX_ERR_BEFORE_L_BRACKET		7	//	������ǰ�����˷��������������
#define	SYNTEX_ERR_AFTER_L_BRACKET		8	//	�����ź�����˷��������������
#define	SYNTEX_ERR_BEFORE_R_BRACKET		9	//	�����ź�����˷��������������
#define	SYNTEX_ERR_AFTER_R_BRACKET		10	//	�����ź�����˷��������������
#define	SYNTEX_ERR_BEFORE_EQUAL_SIGN	11	//	�Ⱥ�ǰ���Գ��ֵ������ֻ��������
#define	SYNTEX_ERR_AFTER_EQUAL_SIGN		12	//	�Ⱥź�Ӧ�ٳ��ֱ��ʽ��
#define	SYNTEX_ERR_UNKNOWN_OPERATOR		13	//	���ʽ�г�����δ֪�������
#define	SYNTEX_ERR_BEFORE_VALUE			14	//	��ֵǰ��������һ����ֵ��������
#define	SYNTEX_ERR_AFTER_VALUE			15	//	��ֵ���������һ����ֵ��������
#define	SYNTEX_ERR_INFINITY_VALUE		18	//	��ֵ���������һ����ֵ��������

//##ModelId=39C1EC0B02F6
class CStackElement
{
public:
	//##ModelId=39C1EC0B033C
	void*			m_pData;
	//##ModelId=39C1EC0B0334
	CStackElement*	m_pUp;
	//##ModelId=39C1EC0B032A
	CStackElement*	m_pDn;
	//##ModelId=39C1EC0B0348
	CStackElement(int = 0);
	//##ModelId=39C1EC0B0346
	~CStackElement(void);
};

//##ModelId=39C1EC260146
class CStackOperation
{
private:
	//##ModelId=39C1EC26015A
	int				m_ElementType;	//	0---char
									//	1---int
									//	2---double
									//	3---BOOL
									//	4---else
	//##ModelId=39C1EC260152
	CStackElement*	m_pStackTop;
	//##ModelId=39C1EC2601AC
	CStackElement*	NewElement(void);
	//##ModelId=39C1EC2601AA
	CStackElement*	DeleteElement(void);
public:
	//##ModelId=39C1EC2601A3
	CStackOperation(int = 0);
	//##ModelId=39C1EC2601A1
	~CStackOperation(void);
	//##ModelId=39C1EC26019A
	BOOL	Initialize(void);
	//##ModelId=39C1EC260198
	BOOL	PushStack(void*);
	//##ModelId=39C1EC260196
	BOOL	GetStackTop(void*);
	//##ModelId=39C1EC26015B
	BOOL	PopStack(void* = NULL);
};

//##ModelId=39C1EC26001A
class CCalculateElement
{
public:
	//##ModelId=39C1EC260042
	int					m_Property;		//	0---�����
										//	1---int		������	-1---int	����
										//	2---double	������	-2---double	����
										//	3---BOOL	������	-3---BOOL	����
										//	4---else	������	-4---else	����
	//##ModelId=39C1EC260039
	void*				m_pData;
	//##ModelId=39C1EC260030
	CCalculateElement*	m_pPrev;
	//##ModelId=39C1EC260026
	CCalculateElement*	m_pNext;
	//##ModelId=39C1EC26004E
	CCalculateElement(int = 0);
	//##ModelId=39C1EC26004C
	~CCalculateElement(void);
};

//##ModelId=39C1EC1300F9
class CCalculater
{
private:
	//##ModelId=39C1EC13012E
	int					m_Type;	
	//##ModelId=39C1EC13012B
	CCalculateElement	m_HeadElement;
	//##ModelId=39C1EC130123
	CCalculateElement*	m_pCurrElement;
	//##ModelId=39C1EC130119
	CStackOperation*	m_pStackOptr;
	//##ModelId=39C1EC130110
	CStackOperation*	m_pStackData;
private:
	//##ModelId=39C1EC130171
	CCalculateElement*	NewElement(int = 0);
	//##ModelId=39C1EC130169
	BOOL				TranslateStringToQueue(void);
	//##ModelId=39C1EC130167
	int					DetectQueue(void);
	//##ModelId=39C1EC13015D
	char				Precede(char,char);
	//##ModelId=39C1EC13014B
	double				DoubleTagOperate(double,char,double);
public:
	//##ModelId=39C1EC13010D
	CString				m_SourceString;
public:
	//##ModelId=39C1EC130149
	CCalculater(int = 1);
	//##ModelId=39C1EC130141
	~CCalculater(void);
	//##ModelId=39C1EC130135
	CString				ErrorMessage(int errorNo);
	BOOL				Calculate(double*,double = 0,double = 0,double = 0);
};

#define		__CALCULATER_CLASS__
#endif
