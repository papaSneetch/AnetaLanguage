int addExp()
{
currentContext.pushNode( new AstAdd (currentContext.popNode(),currentContext.popNode()));
}
		

int subExp()
{
	currentContext.pushNode( new AstSub (currentContext.popNode(),currentContext.popNode()));
}

int mulExp()
{
	currentContext.pushNode( new AstMul (currentContext.popNode(),currentContext.popNode()));
	
};

int divExp()
{
	currentContext.pushNode( new AstDiv (currentContext.popNode(),currentContext.popNode()));
	
};

int incExp();

int leftSh()
{
	currentContext.pushNode( new AstLeftSh (currentContext.popNode(),currentContext.popNode()));
	
};

int rightSh()
{
	currentContext.pushNode( new AstRightSh (currentContext.popNode(),currentContext.popNode()));
	
};

int andExp()
{
	currentContext.pushNode( new AstAnd (currentContext.popNode(),currentContext.popNode()));
	
};

int orExp()
{
	currentContext.pushNode( new AstOr (currentContext.popNode(),currentContext.popNode()));
	
};

int xorExp()
{
	currentContext.pushNode( new AstXor (currentContext.popNode(),currentContext.popNode()));
	
};

int ltCmp()
{
	currentContext.pushNode( new Astlt (currentContext.popNode(),currentContext.popNode()));
	
};

int gtCmp()
{
		currentContext.pushNode( new Astgt (currentContext.popNode(),currentContext.popNode()));

};

int leqCmp()
{
		currentContext.pushNode( new Astleq (currentContext.popNode(),currentContext.popNode()));

};

int geqCmp()
{
	currentContext.pushNode( new Astgeq (currentContext.popNode(),currentContext.popNode()));
	
};

int eqlCmp()
{
	currentContext.pushNode( new Asteql (currentContext.popNode(),currentContext.popNode()));
	
};

int neqCmp()
		{
			currentContext.pushNode( new Astneq (currentContext.popNode(),currentContext.popNode()));
	
		};

int whileLoop();

int ifStat();

int varDefine();

int varCall();

int asgStat();

int FuncDecl();

int FuncCall();

int intTypeDecl(int val)
{
currentContext.pushNode(new AstInt(val) );
}

int floatTypeDecl(float val)
{
currentContext.pushNode(new AstFloat(val) );
}

int stringTypeDecl(char* val)
{
currentContext.pushNode(new AstString(std::string(val) );
}

int boolTypeDecl(bool val)
{
currentContext.pushNode(new AstBool(val);
}

int blockDecl();



