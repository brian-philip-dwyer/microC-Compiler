//Brian Dwyer
//CSE 109
//bpd219
//Parser.cc file

#include "parser.h"
#include "lexer.h"
#include "symbolTable.h"
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>


using namespace std;


const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",
		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",
		      "AND", "OR",
		      "LOADL", "LOADV", "STOREV",
		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",
			      "PRINTF", "FUNC", "PARAM",
		      "LABEL", "SEQ" };




Parser::Parser(Lexer& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1), symboltable() {  
  token = lexer.nextToken();
}

Parser::~Parser() {
};

void Parser::error(string message) {
  cerr << message << " Found " << token->getLexeme() << " at line " << token->getLine() << " position " << token->getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token->getTtype() != tokenType) {
    error(message);
  }
}





Parser::TreeNode* Parser::printfStatement() {

  TreeNode* paramList = NULL;
  int nparams = 0;
  
  token = lexer.nextToken();
  
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();

  check(Token::STRINGLIT, "Expecting string literal");
  string formatString = token->getLexeme();
  token = lexer.nextToken();

  if(token->getTtype() == Token::COMMA) {
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while(token->getTtype() == Token::COMMA) {
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }
  
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  
  TreeNode* node = new TreeNode(PRINTF, itos(nparams) + formatString);
  TreeNode* printStatement = new TreeNode(SEQ, paramList, node);

  return printStatement;
}





void emit(string s) {
  cout << s << endl;
}





int jindex = 0;
string jlabel() { string tmp = "J"; stringstream ss; ss << ++jindex; string res = ss.str(); tmp = tmp + res; return tmp;}





void conditionJump(string s) {

  string j1 = jlabel();
  string j2 = jlabel();

  emit("  pop        rbx                 ;;Relational Operation");
  emit("  pop        rax");
  emit("  cmp rax,   rbx");
  emit("  "+s+"" + j1);
  emit("  mov rax,   0");
  emit("  jmp        " + j2);
  emit("  "+j1+":    ");
  emit("  mov        rax,1");
  emit("  "+j2+":");
  emit("  push       rax");

}





string currentFunction;
int nfmts;
string fmts[100];
int varcnt = 0;
void Parser::geninst(TreeNode *node) {
  
  string fmt = "";
  int nparams = 0;
  const int MAXVARBYTES = 100;
  if(node != NULL) {
    geninst(node->leftChild);
    geninst(node->rightChild);

    switch(node->op) {
    case SEQ:
      break;
    case ADD:
      emit("\n  pop        rbx                 ;;Addition");
      emit("  pop        rax");
      emit("  add rax,   rbx");
      emit("  push       rax");
      break;
    case SUB:
      emit("\n  pop        rbx                 ;;Subtraction");
      emit("  pop        rax");
      emit("  sub rax,   rbx");
      emit("  push       rax");
      break;
    case AND:
      emit("  pop        rbx                 ;;AND Operation");
      emit("  pop        rax");
      emit("  and rax,   rbx");
      emit("  push       rax");
      break;
    case OR:
      emit("  pop        rbx                 ;;OR Operation");
      emit("  pop        rax");
      emit("  or rax,    rbx");
      emit("  push       rax");
      break;
    case MULT:
      emit("\n  pop        rbx                 ;;Multiplication");
      emit("  pop        rax");
      emit("  imul       rbx");
      emit("  push       rax");
      break;
    case DIV:
      emit("\n  mov        rdx,0               ;;Division");
      emit("  pop        rbx");
      emit("  pop        rax");
      emit("  idiv       rbx");
      emit("  push       rax");
      break;
    case LABEL:
      emit("\n  "+node->val);
      break;
    case ISEQ:
      conditionJump("je         ");
      break;
    case ISNE:
      conditionJump("jne        ");
      break;
    case ISLT:
      conditionJump("jl         ");
      break;
    case ISLE:
      conditionJump("jle        ");
      break;
    case ISGT:
      conditionJump("jg         ");
      break;
    case ISGE:
      conditionJump("jge        ");
      break;
    case JUMP:
      emit("  jmp        " + node->val + "                  ;;JUMP");
      break;
    case JUMPF:
      emit("  pop        rax                 ;;JUMPF");
      emit("  cmp        rax,0");
      emit("  je         " + node->val);
      break;
    case JUMPT:
      emit("  pop        rax                  JUMPT");
      emit("  cmp        rax,0");
      emit("  jne        " + node->val);
      break;
    case LOADV:
      emit("  push       rbp-" + node->val +"               ;;Load Variable");
      break;
    case LOADL:
      emit("  mov        rax," + node->val +"              ;;Load Literal");
      emit("  push       rax\n");
      break;
    case STOREV:
      emit("\n  pop        qword[rbp-" + node->val + "]        ;;Store Value");
      break;
    case CALL:
      currentFunction = node->val;
      emit("\n  call       " + currentFunction + "                ;;Function Call" );
      emit("  push       rbx");
      emit("  add        rsp, rbx");
      emit("  push       rax\n");
      break;
    case FUNC:
      currentFunction = node->val;
      emit("\n\n" + currentFunction);
      emit("\n  push       rbp                 ;;Function Operation");
      emit("  mov        rbp,rsp");
      emit("  sub        rsp," + itos(MAXVARBYTES) +"\n");
      varcnt = 0;
      break;
    case PARAM:
      ++varcnt;
      emit("  mov        rsi,qword[rbp+"+ node->val + "]    ;;Parameter");
      emit("  mov        qword[rbp-" + itos(varcnt*8) + "],rsi\n");
      break;
    case RET:
      emit("  pop        rax                 ;;Return");
      emit("  add        rsp," + itos(MAXVARBYTES));
      emit("  pop        rbp");
      emit("  ret");
      break;  
    case PRINTF:
      fmt = node->val;
      nparams = fmt.at(0) - '0';
      fmt = "`" + fmt.substr(1) + "`";
      fmts[nfmts++] = fmt;
      emit("\n  mov        rdi,fmt" + itos(nfmts) + "            ;;Print");
      if (nparams == 5) {
	emit("     pop        r9");
	--nparams;
      }
      if (nparams == 4) {
	emit("  pop        r8");
	--nparams;
      }
      if (nparams == 3) {
	emit("   pop        rcx");
	--nparams;
      }
      if (nparams == 2) {
	emit("  pop        rdx");
	--nparams;
      }
      if (nparams == 1) {
	emit("  pop        rsi");
      }
      emit("  mov        rax,0");
      emit("  push       rbp");
      emit("  call       printf");
      emit("  pop        rbp\n");
      break;      
    default:
      cerr << "In geninst: Unknown operation " << ops[node->op] << endl;
      exit(1);
      break;
    }
  }
}





void Parser::genasm(TreeNode *node) {

  emit("\n  global main");
  emit("  extern printf");
  emit("  section .text\n");
  geninst(node);
  cout << endl << "\n  section .data\n" << endl;
  for(int i = 0; i < nfmts; ++i) {
    cout << "  fmt" << i+1 << ": db " << fmts[i] << ", 0" << endl;
  }
}





Parser::TreeNode* Parser:: funcall(string functionName) {

  TreeNode* newNode = NULL;
  TreeNode* funcallNode = NULL;
  TreeNode* argsNode = NULL;
  
  int n;
  int type = token->getTtype(); 

  //If the type is an LPAREN, it means that there is a function call.
  if(type == Token::LPAREN) {
    token = lexer.nextToken();
    type = token->getTtype();
    //If the next type is an RPAREN, return a function call with no args.
    if(type == Token::RPAREN) {
      n = 0;
      TreeNode* loadLNode = new TreeNode(LOADL, itos(n));
      //Sequence the two together, then sequence that onto the funcall node.
      funcallNode = new TreeNode(CALL, functionName + ":", loadLNode, argsNode);
      return funcallNode;
    }
    //Else if the token is an IDENT, process it.
    else if(type != Token::RPAREN) {
      varcnt++;
      argsNode = expression();
      type = token->getTtype();
      while(type != Token::RPAREN) {
	token = lexer.nextToken();
	varcnt++;
	newNode = expression();
	argsNode = new TreeNode(SEQ, argsNode, newNode);
	type = token->getTtype();
      }
    
      check(Token::RPAREN, "Expected ')' in funcall()...");
      token = lexer.nextToken();
      n = (varcnt * 8);
      TreeNode* loadLNode = new TreeNode(LOADL, itos(n));
      //Sequence the two together, then sequence that onto the funcall node. 
      funcallNode = new TreeNode(CALL, functionName, loadLNode, argsNode);
    }
  }
  
  return funcallNode;
}





Parser::TreeNode* Parser::factor() {

  TreeNode* newNode;
  string functionName;
  int type = token->getTtype();

  if(type == Token::LPAREN) {
    token = lexer.nextToken();
    newNode = expression();
    check(Token::RPAREN, "Exepecting ')' in factor()...");
    token = lexer.nextToken();
  }
  else if(type == Token::INTLIT) {
    newNode = new TreeNode(LOADL, token->getLexeme());
    token = lexer.nextToken();
  }
  else if(type == Token::IDENT) {
    functionName = token->getLexeme();
    token = lexer.nextToken();
    type = token->getTtype();
    if(type == Token::LPAREN) {
      newNode = funcall(functionName);
    }
    else {
      string n = itos(symboltable.getUniqueSymbol(functionName) * 8);
      newNode = new TreeNode(LOADV, n);
    }
  }
  else {
    error("**ERROR** Could not parse factor.");
  }

  return newNode;
}






Parser::TreeNode* Parser::term() {

  TreeNode* root = factor();
  TreeNode* newNode;
  int type = token->getTtype();

  while(type == Token::TIMES || type == Token::DIVIDE) {
    if(type == Token::TIMES) {
      token = lexer.nextToken();
      newNode = factor();
      root = new TreeNode(MULT, root, newNode);
      type = token->getTtype();
    }
    else if(type == Token::DIVIDE) {
      token = lexer.nextToken();
      newNode = factor();
      root = new TreeNode(DIV, root, newNode);
      type = token->getTtype();
    }
    else {
      error("**ERROR** Could not parse term.");
    }
  }

  return root;
}







Parser::TreeNode* Parser::expression() {

  TreeNode* root = term();
  TreeNode* newNode;
  int type = token->getTtype();

  while(type == Token::PLUS || type == Token::MINUS) {
    if(type == Token::PLUS) {
      token = lexer.nextToken();
      newNode = term();
      root = new TreeNode(ADD, root, newNode);
      type = token->getTtype();
    }
    else if(type == Token::MINUS) {
      token = lexer.nextToken();
      newNode = term();
      root = new TreeNode(SUB, root, newNode);
      type = token->getTtype();
    }
    else {
      error("**ERROR** Could not parse expression.");
    }
  }
  
  return root;
}






Parser::TreeNode* Parser::relationalExpression() {

  TreeNode* root = expression();
  TreeNode* newNode;
  int type = token->getTtype();

  while(type == Token::EQ || type == Token::LT || type == Token::LE ||
	type == Token::GT || type == Token::GE || type == Token::NE) {

    if(type == Token::EQ) {
      token = lexer.nextToken();
      newNode = expression();
      root = new TreeNode(ISEQ, root, newNode);
      type = token->getTtype();
    }
    else if(type == Token::LT) {
      token = lexer.nextToken();
      newNode = expression();
      root = new TreeNode(ISLT, root, newNode);
      type = token->getTtype();
    }
    else if(type == Token::LE) {
      token = lexer.nextToken();
      newNode = expression();
      root = new TreeNode(ISLE, root, newNode);
      type = token->getTtype();
    }
    else if(type == Token::GT) {
      token = lexer.nextToken();
      newNode = expression();
      root = new TreeNode(ISGT, root, newNode);
      type = token->getTtype();
    }
    else if(type == Token::GE) {
      token = lexer.nextToken();
      newNode = expression();
      root = new TreeNode(ISGE, root, newNode);
      type = token->getTtype();
    }
    else if(type == Token::NE) {
      token = lexer.nextToken();
      newNode = expression();
      root = new TreeNode(ISNE, root, newNode);
      type = token->getTtype();
    }
    else {
      error("**ERROR** Could not parse relational expression.");
    }
  }

  return root;
}






Parser::TreeNode* Parser::logicalExpression() {

  TreeNode* logExpressionNode = relationalExpression();
  TreeNode* newNode;
  int type = token->getTtype();

  while(type == Token::AND || type == Token::OR) {
    if(type == Token::AND) {
      token = lexer.nextToken();
      newNode = relationalExpression();
      logExpressionNode = new TreeNode(AND, logExpressionNode, newNode);
      type = token->getTtype();
    }
    else if(type == Token::OR) {
      token = lexer.nextToken();
      newNode = relationalExpression();
      logExpressionNode = new TreeNode(OR, logExpressionNode, newNode);
      type = token->getTtype();
    }
    else {
      error("**ERROR** Could not parse logical expression");
    }
  }
  
  return logExpressionNode;
}






Parser::TreeNode* Parser::assignStatement() {

  TreeNode* varNode = NULL;
  TreeNode* valueNode;
  TreeNode* root;

  string variable = token->getLexeme();
  int uniqueSymbol = (symboltable.getUniqueSymbol(variable) * 8);
  string n = itos(uniqueSymbol);

  if(n == "") {
    error("**ERROR** Unique symbol returned a 0.");
  }

  check(Token::IDENT, "Expecting IDENT in assignStatement...");
  token = lexer.nextToken();
  varNode = new TreeNode(LOADV, n);
  check(Token::ASSIGN, "Expecting '=' in assignStatement()...");
  token = lexer.nextToken();
  valueNode = logicalExpression();
  check(Token::SEMICOLON, "Expecting ';' in assignStatement()...");
  token = lexer.nextToken();

  root = new TreeNode(SEQ, varNode, valueNode);
  return root;
}





Parser::TreeNode* Parser::whileStatement() {

  
  string L1 = makeLabel();  
  string L2 = makeLabel();
  
  TreeNode* root = NULL;
  TreeNode* blockNode = NULL;
  TreeNode* whileCondition = NULL; 
  TreeNode* jumpfNode = new TreeNode(JUMPF, L1);
  TreeNode* jumpNode = new TreeNode(JUMP, L2);
  TreeNode* L1Node = new TreeNode(LABEL, L1 + ":");
  TreeNode* L2Node = new TreeNode(LABEL, L2 + ":");
  //Get method for token type.
  int type = token->getTtype();

  //If token is "while", the inside of the parens is a logical expression.
  if(type == Token::WHILE) {
    token = lexer.nextToken();
    //Checks for left paren.
    check(Token::LPAREN, "Expecting '(' in whileStatement()...");
    token = lexer.nextToken();
    whileCondition = logicalExpression();
    type = token->getTtype();
    while(type != Token:: RPAREN) {
      TreeNode* newCondition = logicalExpression();
      whileCondition = new TreeNode(SEQ, whileCondition, newCondition);
      token = lexer.nextToken();
      type = token->getTtype();
    }
    //Checks for right paren.
    check(Token::RPAREN, "Expecting ')' in whileStatement()...");
    token = lexer.nextToken();
    symboltable.enterScope();
    blockNode = block();
    symboltable.exitScope();
  }
  else {
    error("**ERROR** Could not parse while statement.");
    token = lexer.nextToken();
  }

  //Runs the tree that executes the while statement.
  root = new TreeNode(SEQ, L1Node, whileCondition);
  root = new TreeNode(SEQ, root, L2Node);
  root = new TreeNode(SEQ, root, jumpfNode);
  root = new TreeNode(SEQ, root, jumpNode);
  root = new TreeNode(SEQ, root, blockNode);

  return root;
}





Parser::TreeNode* Parser::ifStatement() {

  string L1 = makeLabel();  
  string L2 = makeLabel();
  //Separate nodes needed for the if statement.
  TreeNode* root = NULL;
  TreeNode* then = NULL;
  TreeNode* elseNode = NULL;
  TreeNode* logExpression = NULL;
  TreeNode* jumpfNode = new TreeNode(JUMPF, L1);
  TreeNode* jumpNode = new TreeNode(JUMP, L2);
  TreeNode* L1Node = new TreeNode(LABEL, L1 + ":");
  TreeNode* L2Node = new TreeNode(LABEL, L2 + ":");
  //Get method for token type.
  int type = token->getTtype();
  
  //If there is an if statement, parse it accordingly.
  if(type == Token::IF) {
    token = lexer.nextToken();    
    check(Token::LPAREN, "Expecting '(' in ifStatement()...");
    token = lexer.nextToken();
    //Parse the logical expression inside the if statement's parens
    //and generate a tree for it.  
    logExpression = logicalExpression();
    type = token->getTtype();
    while(type != Token:: RPAREN) {
      TreeNode* newNode = logicalExpression();
      logExpression = new TreeNode(SEQ, newNode, logExpression);
      token = lexer.nextToken();
      type = token->getTtype();
    }
    check(Token::RPAREN, "Expecting ')' in ifStatement()...");
    token = lexer.nextToken();
    symboltable.enterScope();
    then = block();
    symboltable.exitScope();

    type = token->getTtype();
    if(type == Token::ELSE) {
      token = lexer.nextToken();
      symboltable.enterScope();
      elseNode = block();
      symboltable.exitScope();
    }
  }
  else {
    //Throws an error if there is no else.
    error("**ERROR** Could not parse else statement.");
  }

  //Runs the tree that executes the if-else statement. 
  root = new TreeNode(SEQ, root, jumpfNode);
  root = new TreeNode(SEQ, root, then);
  root = new TreeNode(SEQ, root, jumpNode);
  root = new TreeNode(SEQ, root, L1Node);
  root = new TreeNode(SEQ, root, elseNode);
  root = new TreeNode(SEQ, root, L2Node);

  return root;
}





Parser::TreeNode* Parser::returnStatement() {

  TreeNode* returnNode;
  TreeNode* nullNode = NULL;
  int type = token->getTtype();

  if(type == Token::RETURN) { 
    token = lexer.nextToken();
    returnNode = logicalExpression();
    check(Token::SEMICOLON, "Expecting ';' in returnStatement()...");
    token = lexer.nextToken();
  }
  else {
    error("**ERROR** Could not parse return statement.");
  }
 
  return new TreeNode(RET, returnNode, nullNode);
}





Parser::TreeNode* Parser::vardefStatement() {

  string variable;
  int verifyNewSymbol;
  int type;
  
  check(Token::VAR, "Expecting VAR...");
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting an IDENT in vardefStatement()...");
  variable = token->getLexeme();

  //Add variable to the Symbol Table.
  verifyNewSymbol = symboltable.addSymbol(variable);
  if(verifyNewSymbol == 0) {
    error("**ERROR** " + variable + " already defined in this scope.");
  }
  token = lexer.nextToken();
  type = token->getTtype();
  
  while(type == Token::COMMA) {
    token = lexer.nextToken();
    check(Token::IDENT, "Expecting an IDENT in vardefStatement()...");
    variable = token->getLexeme();
    verifyNewSymbol = symboltable.addSymbol(variable);
    if(verifyNewSymbol == 0) {
      error("**ERROR** " + variable + " already defined in this scope.");
    }
    token = lexer.nextToken();
    type = token->getTtype();
  }
  check(Token::SEMICOLON, "Expecting ';' in vardefStatement()...");
  token = lexer.nextToken();
  
  return new TreeNode(SEQ);
}





Parser::TreeNode* Parser::statement() {

  TreeNode* root = NULL;
  int type = token->getTtype();
  
  if(type == Token::VAR) {
    TreeNode* newNode = vardefStatement();
    root = new TreeNode(SEQ, root, newNode);
  }
  else if(type == Token::WHILE) {
    TreeNode* newNode = whileStatement();
    root = new TreeNode(SEQ, root, newNode);
  }
  else if(type == Token::IF) {
    TreeNode* newNode = ifStatement();
    root = new TreeNode(SEQ, root, newNode);
  }
  else if(type== Token::RETURN) {
    TreeNode* newNode = returnStatement();
    root = new TreeNode(SEQ, root, newNode);
  }
  else if(type == Token::IDENT) {
    string currentLexeme = token->getLexeme();
    if(currentLexeme == "printf") {
      TreeNode* newNode = printfStatement();
      root = new TreeNode(SEQ, root, newNode);
    }
    else {
      TreeNode* newNode = assignStatement();
      root = new TreeNode(SEQ, root, newNode);
    }
  }
  else {
    error("**ERROR** Could not parse statement.");
  }

  return root;
}




 
Parser::TreeNode* Parser::block() {

  TreeNode* blockNode;
  TreeNode* newStatement = NULL;
  
  int type = token->getTtype();

  //If the token is a left brace, it means there is a statement body.
  if(type == Token::LBRACE) {
    token = lexer.nextToken();
    blockNode = statement();
    type = token->getTtype();
    //While there are still statements in the block:
    while(type != Token::RBRACE) {
      newStatement = statement();
      blockNode = new TreeNode(SEQ, blockNode, newStatement);
      type = token->getTtype();
    }
    
    check(Token::RBRACE, "Expecting '}' in block()...");
    token = lexer.nextToken();
  }
  else {
    error("**ERROR** Could not parse the statement block.");
    token = lexer.nextToken();
  }
  
  return blockNode;
}




Parser::TreeNode* Parser::parameterdef() {

  TreeNode* paramNode;
  TreeNode* paramSymbol;

  Stack<string> parameterStack;
  string param;
  int verifySym;
  int uniqueSymbol;
  int type;
  int n = 0;
  
  check(Token::IDENT, "Expecting a PARAM in parameterdef()...");
  param = token->getLexeme();
  parameterStack.push(param);
  n++;
  paramNode = new TreeNode(PARAM, itos(n*8));
  token = lexer.nextToken();
  type = token->getTtype();
  
  while(type == Token::COMMA) {
    token = lexer.nextToken();
    check(Token::IDENT, "Expecting an IDENT in parameterdefs()...");
    param = token->getLexeme();

    parameterStack.push(param);
    n++;
    TreeNode* newParam = new TreeNode(PARAM, itos(n*8));
    paramNode = new TreeNode(SEQ, paramNode, newParam);
    token = lexer.nextToken();
    type = token->getTtype();
  }

  check(Token::RPAREN, "Expecting ')' in parameterdefs()...");
  token = lexer.nextToken();

  //Pop params off the stack and adds them Symbol Table.
  while(!parameterStack.isEmpty()) {
    param = parameterStack.pop();
    verifySym = symboltable.addSymbol(param);
    if(verifySym == 0) {
      error("**ERROR** " + param + " already defined in this scope.");
    }
    //Int associated with the parameter in the Symbol Table.
    uniqueSymbol = symboltable.getUniqueSymbol(param);

    //Stores the symbol of the argument in a pointer to the fuction
    //node's memory address, and sequences it to the function node.
    paramSymbol = new TreeNode(STOREV, itos(uniqueSymbol*8));
    paramNode = new TreeNode(SEQ, paramNode, paramSymbol);
  }
  
  return paramNode;
}





Parser::TreeNode* Parser::parameterdefs() {

  TreeNode* funcParams;
  
  check(Token::IDENT, "Expecting a PARAM in parameterdefs()...");
  funcParams = parameterdef();

  return funcParams;
}





Parser::TreeNode* Parser::function() {

  TreeNode* functionNode;
  TreeNode* paramNode = NULL;
  string funcName;
  int type;
  
  check(Token::FUNCTION, "Expecting 'function' in function()...");
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting an IDENT in function()...");
  funcName = token->getLexeme();
  functionNode = new TreeNode(FUNC, funcName + ":");
  token = lexer.nextToken();  
  check(Token::LPAREN, "Expecting '(' ...");
  token = lexer.nextToken();
  type = token->getTtype();
  symboltable.enterScope();
  
  if(type == Token::IDENT) { 
    paramNode = parameterdefs();
  }
  else {
    check(Token::RPAREN, "Expecting ')' in function()...");
    token = lexer.nextToken();
  }
  
  check(Token::LBRACE, "Expecting '{' in function()...");

  functionNode = new TreeNode(SEQ, functionNode, paramNode);
  TreeNode* blockNode = block();
  symboltable.exitScope();
  
  functionNode = new TreeNode(SEQ, functionNode, blockNode);
  return functionNode;
}




Parser::TreeNode* Parser::compilationunit() {

  TreeNode* programRoot = function();
  TreeNode* newFunction = NULL;
  int type = token->getTtype();

  while(type == Token::FUNCTION) {
    newFunction = function();
    programRoot = new TreeNode(SEQ, programRoot, newFunction);
    type = token->getTtype();
  }

  return programRoot;
}
