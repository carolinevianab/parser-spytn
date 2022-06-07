#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include "../Lexer/lexer.h"

char *programPointer;
int debug;
int openedParentesis = 0;
ht_t *ht = NULL;

// ------------------------- Chamadas ao lexer -------------------------

// <identificador> ::= ([a-z] | [A-Z])+
void identificadorGet(int isCalculusContext) {
  if (debug) printf("Begin <identificador>\n");
    if (!runLexer(IDENTIFIER_GET, &programPointer, &ht)) logError("Símbolo não permitido para o identificador");
  if (debug) printf("End <identificador>\n");
}

// <identificador> ::= ([a-z] | [A-Z])+
void identificadorSet(void) {
  if (debug) printf("Begin <identificador>\n");
  if (!runLexer(IDENTIFIER_SET, &programPointer, &ht)) logError("Símbolo não permitido para o identificador");
  if (debug) printf("End <identificador>\n");
}

// <closeP> ::= ")"
void closeP(void) {
  if (debug) printf("Begin <Placeholder>\n");
  if (!runLexer(RESERVED_CLOSETOKEN, &programPointer, &ht)) logError("Símbolo faltando: )");
  if (debug) printf("End <Placeholder>\n");
}

// <mainRes> ::= "main()"
void mainRes(void) {
  if (debug) printf("Begin <mainRes>\n");
  if (!runLexer(RESERVED_MAIN, &programPointer, &ht)) logError("Faltando função inicial do programa.");
  if (debug) printf("End <mainRes>\n");
}

// <linebreak> ::= "\n"
void linebreak(void) {
  if (debug) printf("Begin <linebreak>\n");
  if (!runLexer(RESERVED_LINEBREAK, &programPointer, &ht)) logError("Faltando quebra de linha");
  if (debug) printf("End <linebreak>\n");
}

// <end> ::= "}"
void end(void) {
  if (debug) printf("Begin <end>\n");
  if (!runLexer(RESERVED_END, &programPointer, &ht)) logError("Símbolo faltando: }");
  if (debug) printf("End <end>\n");
}

// <whitespace> ::= " "
void whitespace(void) {
  if (debug) printf("Begin <whitespace>\n");
  if (!runLexer(RESERVED_WHITESPACE, &programPointer, &ht)) logError("Espaço em branco faltando.");
  if (debug) printf("End <whitespace>\n");
}

// <doubleDotRes> ::= ":"
void doubleDotRes(void) {
  if (debug) printf("Begin <doubleDotRes>\n");
  if (!runLexer(RESERVED_DOUBLE_DOT, &programPointer, &ht)) logError("Símbolo faltando: :");
  if (debug) printf("End <doubleDotRes>\n");
}

// <tipoInt> ::= "int"
void tipoInt(void) {
  if (debug) printf("Begin <tipoInt>\n");
  if (!runLexer(RESERVED_INTDEF, &programPointer, &ht)) logError("Erro na definição de int");
  if (debug) printf("End <tipoInt>\n");
}

// <tipoFloat> ::= "float"
void tipoFloat(void) {
  if (debug) printf("Begin <tipoFloat>\n");
  if (!runLexer(RESERVED_FLOATDEF, &programPointer, &ht)) logError("Erro na definição de float");
  if (debug) printf("End <tipoFloat>\n");
}

// <tipoString> ::= "string"
void tipoString(void) {
  if (debug) printf("Begin <tipoString>\n");
  if (!runLexer(RESERVED_STRINGDEF, &programPointer, &ht)) logError("Erro na definição de string");
  if (debug) printf("End <tipoString>\n");
}

// <tipoBool> ::= "bool"
void tipoBool(void) {
  if (debug) printf("Begin <tipoBool>\n");
  if (!runLexer(RESERVED_BOOLDEF, &programPointer, &ht)) logError("Erro na definição de bool");
  if (debug) printf("End <tipoBool>\n");
}

// <equalSignRes> ::= "="
void equalSignRes(void) {
  if (debug) printf("Begin <equalSignRes>\n");
  if (!runLexer(RESERVED_EQUAL_RES, &programPointer, &ht)) logError("Símbolo faltando: =");
  if (debug) printf("End <equalSignRes>\n");
}

// <quotesRes> ::= "\""
void quotesRes(void) {
  if (debug) printf("Begin <quotesRes>\n");
  if (!runLexer(RESERVED_QUOTE_RES, &programPointer, &ht)) logError("Símbolo faltando: \"");
  if (debug) printf("End <quotesRes>\n");
}

// <comparisonOp> ::= ">" | "<" | "==" | "!="
void comparisonOp(void) {
  if (debug) printf("Begin <comparisonOp>\n");
  int result = runLexer(RESERVED_COMP_OP, &programPointer, &ht);
  if (!result) logError("Símbolo de comparação faltando");
  else {
    char myOperator[5];
    
    switch (result) {
      case 1:
        strcpy(myOperator, ">");
        break;
      case 2:
        strcpy(myOperator, "<");
        break;
      case 3:
        strcpy(myOperator, "==");
        break;
      case 4:
        strcpy(myOperator, "!=");
        break;
      default:
        break;
    }
  }
  if (debug) printf("End <comparisonOp>\n");
}

// <openP> ::= "("
void openP(void) {
  if (debug) printf("Begin <openP>\n");
  if (!runLexer(RESERVED_OPENTOKEN, &programPointer, &ht)) logError("Símbolo faltando: (");
  if (debug) printf("End <openP>\n");
}

/* ---- Ignoring cases ---- */

void ignoreSpace(void) {
  if (debug) printf("Ignoring space\n");
  runLexer(RESERVED_WHITESPACE, &programPointer, &ht);
}

void ignoreLinebreak(void) {
  if (debug) printf("ignoring linebreak\n");
  while(runLexer(RESERVED_LINEBREAK, &programPointer, &ht)){};
}

void ignoreTabs(void) {
  if (debug) printf("Ignoring Tabs\n");
  while (runLexer(SKIP_TAB, &programPointer, &ht)){};
}

// ------------------------------ Parser ------------------------------

// <float> ::= <int>? (<dotSignRes> ([0-9]+)?  <expFloat> | <dotSignRes> [0-9]+  <expFloat>?)
void myFloat(int type) {
  switch (type) {
    case FLT_DEFINITION:
      runLexer(SET_FLOAT_VALUE, &programPointer, &ht);
      break;
    case FLT_ATRIBUTION:
      runLexer(ATTR_FLOAT_SEQUENCE, &programPointer, &ht);
      break;
    default:
      break;
  }
}

// <int> ::= (<plusSignRes> | <minusSignRes>+)? [0-9]+
void myInt(int type) {
  if (debug) printf("Begin <int>\n");
  // Variações:
  // Definição de int
  // Atribuição de int
  // Primeira parte de definição de float
  // Primeira parte de atribuição de float
  switch (type) {
    case INT_DEFINITION:
      if (debug) printf("INT_DEFINITION\n");
      runLexer(SET_INTEGER_SEQUENCE, &programPointer, &ht);
      break;
    case INT_ATRIBUTION:
      if (debug) printf("INT_ATRIBUTION\n");
      runLexer(ATTR_INTEGER_SEQUENCE, &programPointer, &ht);
      break;
    case FLT_DEFINITION:
      if (debug) printf("FLT_DEFINITION\n");
      myFloat(type);
      break;
    case FLT_ATRIBUTION:
      if (debug) printf("FLT_ATRIBUTION\n");
      myFloat(type);
      break;
    default:
      logError("Erro definido em tipo Int");
  }
  if (debug) printf("End <int>\n");
}

// <number> ::= <int> | <float>
void number(int definition, int isFloat) {
  if (definition) { // Eu sei que pelo menos é uma definição
    if (isFloat == FALSE) {
      myInt(INT_DEFINITION);
    }
    else {
      // Ser float ou não saber
      if (!runLexer(GET_INTEGER_SEQUENCE, &programPointer, &ht)) {
        // Pode ser um float que comece a partir do ponto (.14)
        if (runLexer(RESERVED_DOT_SIGN, &programPointer, &ht)) {
          // Agora tenho certeza que é float
          myInt(FLT_DEFINITION);
        }
        else {
          logError("Sem matches");
        }
      }
      else {
        // Se tem sequência, pode ser um int ou float
        if (runLexer(RESERVED_DOT_SIGN, &programPointer, &ht)) {
          // Agora tenho certeza que é float
          myInt(FLT_DEFINITION);
        }
        else {
          // é um int
          myInt(INT_DEFINITION);
        }
      }
    }
  }
  else { // Tem que ser uma atribuição
    if (isFloat == FALSE) {
      myInt(INT_ATRIBUTION);
    }
    else {
      // Ser float ou não saber
      if (!runLexer(GET_INTEGER_SEQUENCE, &programPointer, &ht)) {
        // Pode ser um float que comece a partir do ponto (.14)
        if (runLexer(RESERVED_DOT_SIGN, &programPointer, &ht)) {
          // Agora tenho certeza que é float
          myInt(FLT_ATRIBUTION);
        }
        else {
          logError("Sem matches");
        }
      }
      else {
        // Se tem sequência, pode ser um int ou float
        if (runLexer(RESERVED_DOT_SIGN, &programPointer, &ht)) {
          // Agora tenho certeza que é float
          myInt(FLT_ATRIBUTION);
        }
        else {
          // é um int
          myInt(INT_ATRIBUTION);
        }
      }
    }
  }
}

// <string> ::= "\"" ([A-Z] | [a-z] | [0-9] | " " | "." | ",")* "\""
void myString(int definition) {
  if (definition == TRUE) {
    quotesRes();
    runLexer(SET_STRING_SEQUENCE, &programPointer, &ht);
    quotesRes();
  }
  else if (definition == 3) {
    quotesRes();
    runLexer(ATTR_STRING_SEQUENCE, &programPointer, &ht);
    quotesRes();
  }
  else {
    quotesRes();
    runLexer(GET_STRING_SEQUENCE, &programPointer, &ht);
    quotesRes();
  }
  
}

// <bool> ::= "true" | "false"
void myBool(int definition) {
  if (definition) runLexer(SET_BOOL_VALUE, &programPointer, &ht);
  else runLexer(ATTR_BOOL_SEQUENCE, &programPointer, &ht);
}

// <value> ::= <number> | <identificador>
void value(int definition, int isFloat) {
  if (debug) printf("Begin <value>\n");
  if (*programPointer >= 'A' && *programPointer <= 'z') identificadorGet(TRUE);
  else number(definition, isFloat);
  if (debug) printf("End <value>\n");
}

// <NegFac> ::= <minusSignRes> <value> | <value> <factSignRes> | <value>
void NegFac(int definition, int isFloat) {
  if (debug) printf("Begin <NegFac>\n");
  ignoreSpace();
  if (*programPointer == '-') {
    value(definition, isFloat);
    ignoreSpace();
  }
  else {
    value(definition, isFloat);

    // O fatorial pode ou não existir
    // Se existir, equivale a "<value> <factSignRes>"
    // Senão, equivale a "<value>"
    runLexer(RESERVED_FACT_SIGN, &programPointer, &ht);
    ignoreSpace();
  }
  
  if (debug) printf("End <NegFac>\n");
}

// <Parenteses> ::= <openP> <NegFac> | <NegFac> | <closeP>
void Parenteses(int definition, int isFloat) {
  if (debug) printf("Begin <Parenteses>\n");
  ignoreSpace();
  
  if (runLexer(RESERVED_OPENTOKEN, &programPointer, &ht)) {    
    NegFac(definition, isFloat);
    openedParentesis++;
  }
  else {
    NegFac(definition, isFloat);
  }

  if (runLexer(RESERVED_CLOSETOKEN, &programPointer, &ht)) openedParentesis--;
  ignoreSpace();
  
  if (debug) printf("End <Parenteses>\n");
}

// <Pot> ::= <Parenteses> <PotSignRes> <Pot> | <Parenteses>
void Pot(int definition, int isFloat) {
  if (debug) printf("Begin <Pot>\n");
  ignoreSpace();
  Parenteses(definition, isFloat);

  if (runLexer(RESERVED_POT_SIGN, &programPointer, &ht)) {
    Pot(definition, isFloat);
    ignoreSpace();
  }

  if (debug) printf("End <Pot>\n");
}

// <MultDiv> ::= <Pot> (<timesSignRes> | <dividerSignRes>) <MultDiv> | <Pot>
void MultDiv(int definition, int isFloat) {
  if (debug) printf("Begin <MultDiv>\n");
  ignoreSpace();
  Pot(definition, isFloat);

  int isTimes = runLexer(RESERVED_TIMES_SIGN, &programPointer, &ht);
  int isDivide = runLexer(RESERVED_DIVIDE_SIGN, &programPointer, &ht);

  if (isTimes || isDivide) {
    MultDiv(definition, isFloat);
    ignoreSpace();
  }
  
  if (debug) printf("End <MultDiv>\n");
}

// <somaSub> ::= <MultDiv> (<plusSignRes> | <minusSignRes>) <somaSub> | <MultDiv>
void somaSub(int definition, int isFloat) {
  if (debug) printf("Begin <somaSub>\n");
  ignoreSpace();
  MultDiv(definition, isFloat);

  int isPlus = runLexer(RESERVED_PLUS_SIGN, &programPointer, &ht);
  int isMinus =  runLexer(RESERVED_MINUS_SIGN, &programPointer, &ht);

  if (isPlus || isMinus) {
    somaSub(definition, isFloat);
    ignoreSpace();
  }
  
  if (debug) printf("End <somaSub>\n");
}

// <calculo> ::= <somaSub>
void calculo(int definition, int isFloat) {
  if (debug) printf("Begin <calculo>\n");
  somaSub(definition, isFloat);
  ignoreSpace();
  if (debug) printf("End <calculo>\n");
}

// <begin> ::= "{" <linebreak>
void begin(void) {
  if (!runLexer(RESERVED_BEGIN, &programPointer, &ht)) logError("Símbolo faltando: {");
  linebreak();
}

// <attrInt> ::=  <whitespace> <equalSignRes> <whitespace> <calculo>
void attrInt(void) {
  whitespace();
  equalSignRes();
  whitespace();
  calculo(TRUE, FALSE);
}

// <attrFloat> ::=  <whitespace> <equalSignRes> <whitespace> <calculo>
void attrFloat(void) {
  whitespace();
  equalSignRes();
  whitespace();
  calculo(TRUE, TRUE);
}

// <attrString> ::=  <whitespace> <equalSignRes> <whitespace> <string>
void attrString(void) {
  whitespace();
  equalSignRes();
  whitespace();
  myString(TRUE);
}

// <attrBool> ::=  <whitespace> <equalSignRes> <whitespace> <bool>
void attrBool(void) {
  whitespace();
  equalSignRes();
  whitespace();
  myBool(TRUE);
}

// <tipagemVar> ::= <doubleDotRes> (<tipoInt> <attrInt> | <tipoFloat> <attrFloat> | <tipoString> <attrString> | <tipoBool> <attrBool>)
void tipagemVar(void) {
  doubleDotRes();

  switch (*programPointer) {
    case 'i':
      tipoInt();
      attrInt();
      break;
    case 'f':
      tipoFloat();
      attrFloat();
      break;
    case 's':
      tipoString();
      attrString();
      break;
    case 'b':
      tipoBool();
      attrBool();
      break;
    break;
    default:
    logError("Tipo não encontrado.");
  }
}

// <atribuicao> ::= <identificador> <whitespace> <equalSignRes> <whitespace> (<bool> | <calculo> | <string>)
void atribuicao(void) {
  if (debug) printf("Begin <atribuicao>\n");
  identificadorGet(TRUE);
  whitespace();
  equalSignRes();
  whitespace();
  
  if (*programPointer == '\"') myString(3);
  else if (*programPointer == 't' || (*programPointer == 'f')) myBool(FALSE);
  else calculo(FALSE, -1);
  
  if (debug) printf("End <atribuicao>\n");
}

// <variavel> ::= <varRes> <whitespace> <identificador> <tipagemVar>  <linebreak>
void variavel(void) {
  if (debug) printf("Begin <variavel>\n");
  // Nessa etapa, já validei "<varRes>"
  whitespace();
  identificadorSet();
  tipagemVar();
  linebreak();
  if (debug) printf("End <variavel>\n");
}

// <printing> ::= <string> | <identificador>
void printing(void) {
  if(*programPointer == '\"') myString(FALSE);
  else identificadorGet(TRUE);
}

// <printParameters> ::= <printing> (<separator> <printing>)*
void printParameters(void) {
  if (debug) printf("Begin <printParameters>\n");
  printing();
  while (runLexer(RESERVED_SEPARATOR, &programPointer, &ht)){
    ignoreSpace();
    printing();
  }
  if (debug) printf("End <printParameters>\n");
}

// <print> ::= <printRes> <openP> <printParameters> <closeP>
void myPrint(void) {
  if (debug) printf("Begin <print>\n");
  // Nessa etapa, já validei <printRes>
  openP();
  printParameters();
  closeP();
  if (debug) printf("End <print>\n");
}

// <input> ::= <inputRes> <openP> <identificador> <closeP>
void input(void) {
  if (debug) printf("Begin <dado>\n");
  // Já validei <inputRes>
  openP();
  identificadorGet(TRUE);
  closeP();
  if (debug) printf("End <dado>\n");
}

// <dado> ::= <identificador>
void dado(void) {
  if (debug) printf("Begin <dado>\n");
  identificadorGet(TRUE);
  if (debug) printf("End <dado>\n");
}

// <condicao> ::= <dado> <whitespace> <comparisonOp> <whitespace> <dado>
void condicao(void) {
  if (debug) printf("Begin <condicao>\n");
  dado();
  whitespace();
  comparisonOp();
  whitespace();
  dado();
  if (debug) printf("End <condicao>\n");
}

// <elseStatement> ::= <elseRes> <whitespace> <begin> <bloco> <end>
void elseStatement(void) {
  if (debug) printf("Begin <elseStatement>\n");
  // Nessa etapa já validei <elseRes>
  whitespace();
  begin();

  if (debug) printf("Begin else <bloco>\n");
  ignoreTabs();
  while (*programPointer != '}') {
    ignoreTabs();
    
    if (runLexer(RESERVED_VAR, &programPointer, &ht)) variavel();
    else if (runLexer(RESERVED_IF, &programPointer, &ht)) logError("Um else não pode ter um if dentro");
    else if (runLexer(RESERVED_PRINT, &programPointer, &ht)) myPrint();
    else if (runLexer(RESERVED_INPUT, &programPointer, &ht)) input();
    else atribuicao();
      
    ignoreLinebreak();
    ignoreTabs();
  }
  if (debug) printf("End else <bloco>\n");
  
  end();
  if (debug) printf("End <elseStatement>\n");
}

// <ifStatement> ::= <ifRes> <whitespace> <condicao> <whitespace> <begin> <bloco> <end> <linebreak> <elseStatement>?
void ifStatement(void) {
  if (debug) printf("Begin <ifStatement>\n");
  // Nessa etapa, já validei "<ifRes>"
  whitespace();
  condicao();
  whitespace();

  begin();
  if (debug) printf("Begin if <bloco>\n");
  ignoreTabs();
  while (*programPointer != '}') {
    ignoreTabs();
    
    if (runLexer(RESERVED_VAR, &programPointer, &ht)) variavel();
    else if (runLexer(RESERVED_IF, &programPointer, &ht)) ifStatement();
    else if (runLexer(RESERVED_PRINT, &programPointer, &ht)) myPrint();
    else if (runLexer(RESERVED_INPUT, &programPointer, &ht)) input();
    else atribuicao();
      
    ignoreLinebreak();
    ignoreTabs();
  }
  if (debug) printf("End if <bloco>\n");
  
  end();
  ignoreLinebreak();
  ignoreTabs();
  if (runLexer(RESERVED_ELSE, &programPointer, &ht)) elseStatement();
  
  if (debug) printf("End <ifStatement>\n");
}

// <whileStatement> ::= <whileRes> <whitespace> <condicao> <whitespace> <begin> <bloco> <end> <linebreak>
void whileStatement(void) {
  if (debug) printf("Begin <whileStatement>\n");
  // Nessa etapa, já validei <whileRes>
  whitespace();
  if (!runLexer(GET_BOOL_VALUE, &programPointer, &ht)) {
    condicao();
  }
  whitespace();
  begin();
  
  if (debug) printf("Begin while <bloco>\n");
  ignoreTabs();
  while (*programPointer != '}') {
    ignoreTabs();
    
    if (runLexer(RESERVED_VAR, &programPointer, &ht)) variavel();
    else if (runLexer(RESERVED_IF, &programPointer, &ht)) ifStatement();
    else if (runLexer(RESERVED_WHILE, &programPointer, &ht)) whileStatement();
    else if (runLexer(RESERVED_PRINT, &programPointer, &ht)) myPrint();
    else if (runLexer(RESERVED_INPUT, &programPointer, &ht)) input();
    else if (runLexer(RESERVED_BREAK, &programPointer, &ht)) {}
    else atribuicao();
      
    ignoreLinebreak();
    ignoreTabs();
  }
  if (debug) printf("End while <bloco>\n");
  
  end();
  if (debug) printf("End <whileStatement>\n");
}

// <programa> ::= <mainRes> <begin> <bloco> <end>
// <bloco> ::= (<variavel> | <ifStatement> | <whileStatement> | <myPrint> | <input> | <atribuicao>)+
void programa(void) {
  if (debug) printf("Begin <programa>\n");
  mainRes();
  begin();
  
  if (debug) printf("Begin main <bloco>\n");
  while (*programPointer != '}') {
    ignoreTabs();
    ignoreLinebreak();

    if (runLexer(RESERVED_VAR, &programPointer, &ht)) variavel();
    else if (runLexer(RESERVED_IF, &programPointer, &ht)) ifStatement();
    else if (runLexer(RESERVED_WHILE, &programPointer, &ht)) whileStatement();
    else if (runLexer(RESERVED_PRINT, &programPointer, &ht)) myPrint();
    else if (runLexer(RESERVED_INPUT, &programPointer, &ht)) input();
    else atribuicao();
    
    ignoreLinebreak();
  }
  if (debug) printf("End main <bloco>\n");
  
  end(); 
  if (debug) printf("End <programa>\n");
}

// Setup do parser
void runParser(char program[], int isDebugOn) {
  ht = ht_create();
  programPointer = program;
    debug = isDebugOn;
    defineIsDebug(isDebugOn);

  programa();

  if (openedParentesis > 0) printf(ANSI_RED "Erro: há %d parentesis abertos sem fechamento.\n" ANSI_RESET, openedParentesis);
  else if (openedParentesis < 0) printf(ANSI_RED "Erro: há %d parentesis fechados extras.\n" ANSI_RESET, -openedParentesis);
    
    printf("Variáveis declaradas:\n");
  ht_dump(ht);
  printf("\nFim da execução do parser.\n");

}

