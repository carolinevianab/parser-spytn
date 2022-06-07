#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

char *resPointer;           // Pointeiro do programa
int incQuanti = 0;          // Quanto o ponteiro tem que incrementar
int identifierContext = -1;
int isDebug;

char identifierName[100];

int saveDefToHash(ht_t **hashmap, char type[], char toSave[]) {
  char str[100];
  strcpy(str, type);
  
  strcat(str, toSave);
    
  char *key = ht_getFirstNull(*hashmap);
  if (key != NULL){
    ht_set(*hashmap, key, str);
  }
  else {
    logError("Não foi encontrada chave nula no hashmap");
    return FALSE;
  }
  return TRUE;
}

int compareToReserveds(void){
  char reservedsToCompare[] = "+-*/^()!%";
  for (int i = 0; i < 9; i++) {
    if (*resPointer == reservedsToCompare[i]) return TRUE;
  }
  return FALSE;
}

int detectIdentifier(void) {
  if (isDebug) printf("Lexer Processing identifier: ");
  int count = 0;
  while (TRUE) {
    if (!((*resPointer >= 'A' && *resPointer <= 'Z') || (*resPointer >= 'a' && *resPointer <= 'z'))) {
      if (*resPointer == ' ') break;
      else if (*resPointer == '\n') break;
      else if (*resPointer == '\0') break;
      else if (*resPointer == ':') break;
      else if (*resPointer == '{') break;
      else if (*resPointer == ',') break;
        else if (*resPointer == ')') break;
      else if (*resPointer >= '1' && *resPointer <= '9' && count > 0) {}
      else return FALSE;
    }
    if (isDebug) printf("%c", *resPointer);
    identifierName[count] = *resPointer;
    count++;
    resPointer++;
    incQuanti++;
  }
  identifierName[count] = '\0';
  if (isDebug) printf("\n");
  return TRUE;
}

int processReserved(char reservedWord[], int n) {
  for (int i = 0; i <= n-1;i++){
    if (isDebug) printf("Lexer Processing %s: %c\n", reservedWord, *resPointer);
    if (*resPointer != reservedWord[i]) return FALSE;
    resPointer++;
    incQuanti++;
  }
  return TRUE;
}

int reservedCopOp(void) {
  if (processReserved(">", 1)) return 1;
  if (processReserved("<", 1)) return 2;
  if (processReserved("==", 2)) return 3;
  if (processReserved("!=", 2)) return 4;
  return FALSE;
}

int reservedTab(void) {
  char reserved[] = "  ";
  int n = sizeof(reserved) / sizeof(reserved[0]);
  return processReserved(reserved, n);
}

int reservedE(void) {
  int result = 
  processReserved("e", 1)  ||
  processReserved("E", 1);
  
  return result;
}

char numberChar[10] = {0};
int numP = 0;
int result = 1;
int getInteger(void) {
  if (isDebug) printf("Lexer (getInteger) Processing integer: %c\n", *resPointer);
  result = 1;
  processReserved("+", 1);

  while (processReserved("-", 1)) result *= -1;

  for (int i = 0; i < 10; i++) {
    numberChar[i] = 0;
  }

  if ((int)*resPointer == 0) return FALSE;
  if (!(*resPointer >= '(' && *resPointer <= '9'))
    return FALSE;

  while ((*resPointer >= '0' && *resPointer <= '9')){
    numberChar[numP] = *resPointer;
    numP++;
    resPointer++;
    incQuanti++;
  }
  numP = 0;
  if (numberChar[0] != 0) {
    return TRUE;
  }
  return FALSE;
}

int setInteger(ht_t **hashmap) {
  if (isDebug) printf("Lexer (setInteger) Processing integer: %c\n", *resPointer);
  
  if (!getInteger()) {
    logError("Valor não inteiro.");
    return FALSE;
  }
  else {
    char str[100];
    if (result < 0) {
      strcpy(str, "-");
      strcat(str, numberChar);
    }
    else strcpy(str, numberChar);
    saveDefToHash(hashmap, "i:", str);
  }
  result = 1;
  return TRUE;
}

int atributeInt(ht_t **hashmap) {
  if (isDebug) printf("Lexer (atributeInt) Processing integer: %c\n", *resPointer);
  // Nesse ponto, por causa das verificações, o int já foi lido

  char *oldContent = ht_get(*hashmap, identifierName);
  if (*oldContent != 'i'){
    logError("Não é possível atribuir um inteiro a um tipo não inteiro.");
    return FALSE;
  }
  char str[100];
  strcpy(str, "i:");
  strcat(str, numberChar);
  ht_set(*hashmap, identifierName, str);

  return TRUE;
}

char firstPart[100];
int getFloat(void) {
  if (result < 0) {
    strcpy(firstPart, "-");
    strcat(firstPart, numberChar);
  }
  else strcpy(firstPart, numberChar);
  strcat(firstPart, ".");


  if (getInteger()) { // Tem dados dps do ponto (3.14)
    strcat(firstPart, numberChar);

    // Pode ou não ter E
    if (reservedE()) {
      strcat(firstPart, "e");
      getInteger();

      if (result < 0) strcat(firstPart, "-");
      else strcat(firstPart, "+");

      
      strcat(firstPart, numberChar);
    }
  }
  else { // Não tem dados depois do ponto, logo, obriga um e (3.e-15)
    if (!reservedE()){
      logError("Obrigatório uso do expoente nesse caso");
      return FALSE;
    }
    else {
      strcat(firstPart, "e");

      if (result < 0) strcat(firstPart, "-");
      else strcat(firstPart, "+");

      strcat(firstPart, numberChar);
    }
  }
  return TRUE;
}

int setFloat(ht_t **hashmap) {
  if (getFloat()) saveDefToHash(hashmap, "f:", firstPart);
  else {
    logError("Erro na definição de float");
    return FALSE;
  }
  
  return TRUE;
}

int atributeFloat(ht_t **hashmap) {
  if (!getFloat()) {
    logError("Erro na definição de float");
    return FALSE;
  }
  else {
    char *oldContent = ht_get(*hashmap, identifierName);
    if (*oldContent != 'f'){
      logError("Não é possível atribuir um float a um tipo não float.");
      return FALSE;
    }
    char str[100];
    strcpy(str, "f:");
    strcat(str, firstPart);
    ht_set(*hashmap, identifierName, str);
  }
  
  return TRUE;
}

char stringChar[200] = {0};
int stringP = 0;
int getString(void) {
  if (isDebug) printf("Lexer Processing string: ");

  if(*resPointer == '\"') return TRUE;

  for (int i = 0; i < 200; i++) {
    stringChar[i] = 0;
  }
  
  if ((int)*resPointer == 0) return FALSE;
  
  while (*resPointer != '\"'){
    if (*resPointer == '\n') return FALSE;
    if (*resPointer == '\\') return FALSE;

    if (isDebug) printf("%c", *resPointer);

    stringChar[stringP] = *resPointer;
    stringP++;
    resPointer++;
    incQuanti++;
  }

  if (isDebug) printf("\n");
  stringP = 0;
  if (stringChar[0] != 0) return TRUE;
  
  return FALSE;

  
}

int setString(ht_t **hashmap) {

  if (!getString()) {
    logError("Erro na definição de string");
    return FALSE;
  }
  else {
    saveDefToHash(hashmap, "s:", stringChar);
  }
  return TRUE;
}

int atributeString(ht_t **hashmap) {
  if (isDebug) printf("%s", identifierName);
  if (getString()) {
    char *oldContent = ht_get(*hashmap, identifierName);
    printf("%c", *oldContent);
    if (*oldContent != 's'){
      logError("Não é possível atribuir uma string a um tipo não string.");
      return FALSE;
    }
    char str[100];
    strcpy(str, "s:");
    strcat(str, stringChar);
    ht_set(*hashmap, identifierName, str);
    
  }
  else logError("Erro na atribuição da String");
  return TRUE;
}


char boolValue[2] = "2";
int getBool(void) {
  if (!processReserved("true", 4)) {
    if (!processReserved("false", 5)) return FALSE;
    else boolValue[0] = '0';
  }
  else boolValue[0] = '1';
  boolValue[1] = '\0';
  return TRUE;
}

int setBool(ht_t **hashmap) {
  if (!getBool()) {
    logError("Erro na definição de bool");
    return FALSE;
  }
  else {
    saveDefToHash(hashmap, "b:", boolValue);
  }
  return TRUE;
}

int atributeBool(ht_t **hashmap) {
  if (!getBool()) {
    logError("Erro na definição de bool");
    return FALSE;
  }
  else {
    char *oldContent = ht_get(*hashmap, identifierName);
    if (*oldContent != 'b'){
      logError("Não é possível atribuir um bool a um tipo não bool.");
      return FALSE;
    }
    char str[100];
    strcpy(str, "b:");
    strcat(str, boolValue);
    ht_set(*hashmap, identifierName, str);
  }
  
  return TRUE;
}

void logError(char error[]) {
  printf(ANSI_RED "Error: %s\n" ANSI_RESET, error);
}

void defineIsDebug(int isDebugMode) {
    isDebug = isDebugMode;
}


int runLexer(int wordCode, char **pointer, ht_t **hashmap) {
  resPointer = *pointer;
  int evaluate = FALSE;
  switch(wordCode) {
    case RESERVED_NULL:  printf(ANSI_RED "Erro: Token nulo.\n" ANSI_RESET); break;
    case IDENTIFIER_GET:        evaluate = detectIdentifier();              break;
    case IDENTIFIER_SET:        evaluate = detectIdentifier();              break;
    case SKIP_TAB:              evaluate = reservedTab();                   break;
    case RESERVED_VAR:          evaluate = processReserved("var", 3);       break;
    case RESERVED_IF:           evaluate = processReserved("if", 2);        break;
    case RESERVED_ELSE:         evaluate = processReserved("else", 4);      break;
    case RESERVED_WHILE:        evaluate = processReserved("while", 5);     break;
    case RESERVED_MAIN:         evaluate = processReserved("main()", 6);    break;
    case RESERVED_PRINT:        evaluate = processReserved("print", 5);     break;
    case RESERVED_INPUT:        evaluate = processReserved("input", 5);     break;
    case RESERVED_INTDEF:       evaluate = processReserved("int", 3);       break;
    case RESERVED_FLOATDEF:     evaluate = processReserved("float", 5);     break;
    case RESERVED_STRINGDEF:    evaluate = processReserved("string", 6);    break;
    case RESERVED_BOOLDEF:      evaluate = processReserved("bool", 4);      break;
    case RESERVED_BREAK:        evaluate = processReserved("break", 5);     break;
    case RESERVED_OPENTOKEN:    evaluate = processReserved("(", 1);         break;
    case RESERVED_CLOSETOKEN:   evaluate = processReserved(")", 1);         break;
    case RESERVED_SEPARATOR:    evaluate = processReserved(",", 1);         break;
    case RESERVED_WHITESPACE:   evaluate = processReserved(" ", 1);         break;
    case RESERVED_LINEBREAK:    evaluate = processReserved("\n", 1);        break;
    case RESERVED_BEGIN:        evaluate = processReserved("{", 1);         break;
    case RESERVED_END:          evaluate = processReserved("}", 1);         break;
    case RESERVED_EQUAL_RES:    evaluate = processReserved("=", 1);         break;
    case RESERVED_DOUBLE_DOT:   evaluate = processReserved(":", 1);         break;
    case RESERVED_PLUS_SIGN:    evaluate = processReserved("+", 1);         break;
    case RESERVED_MINUS_SIGN:   evaluate = processReserved("-", 1);         break;
    case RESERVED_TIMES_SIGN:   evaluate = processReserved("*", 1);         break;
    case RESERVED_DIVIDE_SIGN:  evaluate = processReserved("/", 1);         break;
    case RESERVED_FACT_SIGN:    evaluate = processReserved("!", 1);         break;
    case RESERVED_DOT_SIGN:     evaluate = processReserved(".", 1);         break;
    case RESERVED_POT_SIGN:     evaluate = processReserved("^", 1);         break;
    case RESERVED_QUOTE_RES:    evaluate = processReserved("\"", 1);        break;
    case RESERVED_COMP_OP:      evaluate = reservedCopOp();                 break;
    case RESERVED_E:            evaluate = reservedE();                     break;
    case GET_INTEGER_SEQUENCE:  evaluate = getInteger();                    break;
    case SET_INTEGER_SEQUENCE:  evaluate = setInteger(hashmap);             break;
    case ATTR_INTEGER_SEQUENCE: evaluate = atributeInt(hashmap);            break;
    case GET_STRING_SEQUENCE:   evaluate = getString();                     break;
    case SET_STRING_SEQUENCE:   evaluate = setString(hashmap);              break;
    case ATTR_STRING_SEQUENCE:  evaluate = atributeString(hashmap);         break;
    case GET_BOOL_VALUE:        evaluate = getBool();                       break;
    case SET_BOOL_VALUE:        evaluate = setBool(hashmap);                break;
    case ATTR_BOOL_SEQUENCE:    evaluate = atributeBool(hashmap);           break;
    case SET_FLOAT_VALUE:       evaluate = setFloat(hashmap);               break;
    case ATTR_FLOAT_SEQUENCE:   evaluate = atributeFloat(hashmap);          break;
    default:
          printf(ANSI_RED "Token Error: Token não reconhecido.\n" ANSI_RESET);
          break;
  }

  if (wordCode == IDENTIFIER_SET && evaluate) {
    ht_set(*hashmap, identifierName, "NULL");
  }

  if (wordCode == IDENTIFIER_GET && evaluate) {
    char *identifier = ht_get(*hashmap, identifierName);
    if (identifier == NULL) {
      logError("Identificador utilizado mas não declarado.");
    }
    else {
      *pointer+=incQuanti;
      incQuanti = 0;
      return (int)ht_getSlotOfKey(*hashmap, identifierName);
    }
  }

  if ((wordCode == RESERVED_VAR || wordCode == RESERVED_IF || wordCode == RESERVED_ELSE) && !evaluate) {
    incQuanti = 0;
  }

  *pointer+=incQuanti;
  incQuanti = 0;
  return evaluate; // 1 é true
}
