/*	see copyright notice in squirrel.h */
#ifndef _SQFUNCTION_H_
#define _SQFUNCTION_H_

#include "sqopcodes.h"

enum SQOuterType {
	otLOCAL = 0,
	otOUTER = 1
};

struct SQOuterVar
{

	SQOuterVar(){}
	SQOuterVar(const SQObjectPtr &name,const SQObjectPtr &src,SQOuterType t)
	{
		_name = name;
		_src=src;
		_type=t;
	}
	SQOuterVar(const SQOuterVar &ov)
	{
		_type=ov._type;
		_src=ov._src;
		_name=ov._name;
	}
	SQOuterType _type;
	SQObjectPtr _name;
	SQObjectPtr _src;
};

#define _VAR_ANY			0x00000001
#define _VAR_INTEGER		0x00000002
#define _VAR_FLOAT			0x00000004
#define _VAR_BOOL			0x00000008
#define _VAR_STRING			0x00000010
#define _VAR_TABLE			0x00000020
#define _VAR_ARRAY			0x00000040
#define _VAR_CLOSURE		0x00000080
#define _VAR_CLASS			0x00000100
#define _VAR_REFERENCE  	0x00000200
#define _VAR_CONST  		0x00000400

struct SQLocalVarInfo
{
	SQLocalVarInfo():_start_op(0),_end_op(0),_pos(0),
        _type(_VAR_ANY),_scope(0) {}
	SQLocalVarInfo(const SQLocalVarInfo &lvi)
	{
		_name=lvi._name;
		_type_name=lvi._type_name;
		_start_op=lvi._start_op;
		_end_op=lvi._end_op;
		_pos=lvi._pos;
		_type=lvi._type;
		_scope=lvi._scope;
	}
	SQObjectPtr _name;
	SQObjectPtr _type_name;
	SQUnsignedInteger _start_op;
	SQUnsignedInteger _end_op;
	SQUnsignedInteger _pos;
	unsigned short _type;
	unsigned short _scope;
};

struct SQLineInfo { SQInteger _line;SQInteger _op; };
struct SQGotoLabelsInfo { SQObjectPtr name; SQInt32 pos; SQInt16 line; SQInt16 traps;};

typedef sqvector<SQOuterVar> SQOuterVarVec;
typedef sqvector<SQLocalVarInfo> SQLocalVarInfoVec;
typedef sqvector<SQLineInfo> SQLineInfoVec;
typedef sqvector<SQGotoLabelsInfo> SQGotoLabelsInfoVec;

#define _FUNC_SIZE(ni,nl,nparams,nfuncs,nouters,nlineinf,localinf,defparams) \
        (sizeof(SQFunctionProto) \
		+((ni-1)*sizeof(SQInstruction))+(nl*sizeof(SQObjectPtr)) \
		+(nparams*sizeof(SQObjectPtr))+(/*param_types*/nparams*sizeof(SQObjectPtr)) \
        +(nfuncs*sizeof(SQObjectPtr)) \
		+(nouters*sizeof(SQOuterVar))+(nlineinf*sizeof(SQLineInfo)) \
		+(localinf*sizeof(SQLocalVarInfo))+(defparams*sizeof(SQInteger)))


struct SQFunctionProto : public CHAINABLE_OBJ
{
private:
	SQFunctionProto(SQSharedState *ss);
	~SQFunctionProto();

public:
	static SQFunctionProto *Create(SQSharedState *ss,SQInteger ninstructions,
		SQInteger nliterals,SQInteger nparameters,
		SQInteger nfunctions,SQInteger noutervalues,
		SQInteger nlineinfos,SQInteger nlocalvarinfos,SQInteger ndefaultparams)
	{
		SQFunctionProto *f;
		//I compact the whole class and members in a single memory allocation
		f = (SQFunctionProto *)sq_vm_malloc(_FUNC_SIZE(ninstructions,nliterals,nparameters,nfunctions,noutervalues,nlineinfos,nlocalvarinfos,ndefaultparams));
		new (f) SQFunctionProto(ss);
		f->_ninstructions = ninstructions;
		//literals start at then end of _instructions
		f->_literals = (SQObjectPtr*)&f->_instructions[ninstructions];
		f->_nliterals = nliterals;
		//parameters start at then end of _literals
		f->_parameters = (SQObjectPtr*)&f->_literals[nliterals];
		f->_nparameters = nparameters;
		//the same as above repeat till the end
		f->_parameters_type = (SQObjectPtr*)&f->_parameters[nparameters];
		f->_functions = (SQObjectPtr*)&f->_parameters_type[nparameters];
		f->_nfunctions = nfunctions;
		f->_outervalues = (SQOuterVar*)&f->_functions[nfunctions];
		f->_noutervalues = noutervalues;
		f->_lineinfos = (SQLineInfo *)&f->_outervalues[noutervalues];
		f->_nlineinfos = nlineinfos;
		f->_localvarinfos = (SQLocalVarInfo *)&f->_lineinfos[nlineinfos];
		f->_nlocalvarinfos = nlocalvarinfos;
		f->_defaultparams = (SQInteger *)&f->_localvarinfos[nlocalvarinfos];
		f->_ndefaultparams = ndefaultparams;

		_CONSTRUCT_VECTOR(SQObjectPtr,f->_nliterals,f->_literals);
		_CONSTRUCT_VECTOR(SQObjectPtr,f->_nparameters,f->_parameters);
		_CONSTRUCT_VECTOR(SQObjectPtr,f->_nparameters,f->_parameters_type);
		_CONSTRUCT_VECTOR(SQObjectPtr,f->_nfunctions,f->_functions);
		_CONSTRUCT_VECTOR(SQOuterVar,f->_noutervalues,f->_outervalues);
		//_CONSTRUCT_VECTOR(SQLineInfo,f->_nlineinfos,f->_lineinfos); //not required are 2 integers
		_CONSTRUCT_VECTOR(SQLocalVarInfo,f->_nlocalvarinfos,f->_localvarinfos);
		return f;
	}
	void Release(){
		_DESTRUCT_VECTOR(SQObjectPtr,_nliterals,_literals);
		_DESTRUCT_VECTOR(SQObjectPtr,_nparameters,_parameters);
		_DESTRUCT_VECTOR(SQObjectPtr,_nparameters,_parameters_type);
		_DESTRUCT_VECTOR(SQObjectPtr,_nfunctions,_functions);
		_DESTRUCT_VECTOR(SQOuterVar,_noutervalues,_outervalues);
		//_DESTRUCT_VECTOR(SQLineInfo,_nlineinfos,_lineinfos); //not required are 2 integers
		_DESTRUCT_VECTOR(SQLocalVarInfo,_nlocalvarinfos,_localvarinfos);
		SQInteger size = _FUNC_SIZE(_ninstructions,_nliterals,_nparameters,_nfunctions,_noutervalues,_nlineinfos,_nlocalvarinfos,_ndefaultparams);
		this->~SQFunctionProto();
		sq_vm_free(this,size);
	}

	const SQChar* GetLocal(SQVM *v,SQUnsignedInteger stackbase,SQUnsignedInteger nseq,SQUnsignedInteger nop);
	SQInteger GetLine(SQInstruction *curr);
	bool Save(SQVM *v,SQUserPointer up,SQWRITEFUNC write);
	bool SaveAsSource(SQVM *v,SQUserPointer up,SQWRITEFUNC write);
	static bool Load(SQVM *v,SQUserPointer up,SQREADFUNC read,SQObjectPtr &ret);

#ifdef SQ_JIT_LLVM
	void JitCompile();
#endif

#ifndef NO_GARBAGE_COLLECTOR
	void Mark(SQCollectable **chain);
	void Finalize(){ _NULL_SQOBJECT_VECTOR(_literals,_nliterals); }
	SQObjectType GetType() {return OT_FUNCPROTO;}
#endif
	SQObjectPtr _sourcename;
	SQObjectPtr _name;
	SQObjectPtr _return_type;
    SQInteger _stacksize;
	bool _bgenerator;
	SQInteger _varparams;

#ifdef SQ_JIT_LLVM
	llvm::Function* _jitfunction;
#endif

	SQInteger _nlocalvarinfos;
	SQLocalVarInfo *_localvarinfos;

	SQInteger _nlineinfos;
	SQLineInfo *_lineinfos;

	SQInteger _nliterals;
	SQObjectPtr *_literals;

	SQInteger _nparameters;
	SQObjectPtr *_parameters;
	SQObjectPtr *_parameters_type;

	SQInteger _nfunctions;
	SQObjectPtr *_functions;

	SQInteger _noutervalues;
	SQOuterVar *_outervalues;

	SQInteger _ndefaultparams;
	SQInteger *_defaultparams;

	SQInteger _ninstructions;
	SQInstruction _instructions[1];
};

#endif //_SQFUNCTION_H_
