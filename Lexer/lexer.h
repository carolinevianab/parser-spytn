#include "../Hashmap/hashmap.h"

#define FALSE 0
#define TRUE 1

#define ANSI_RED "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

enum {
  RESERVED_NULL,         // Não quero usar o valor 0 do enum
  RESERVED_VAR,          // <varRes> ::= "var"
  RESERVED_IF,           // <ifRes> ::= "if"
  RESERVED_ELSE,         // <elseRes> ::= "else"
  RESERVED_WHILE,        // <whileRes> ::= "while"
  RESERVED_MAIN,         // <mainRes> ::= "main()"
  RESERVED_PRINT,        // <printRes> ::= "print"
  RESERVED_INPUT,        // <inputRes> ::= "input"
  RESERVED_BREAK,        // <timesSignRes> ::= "break"
  RESERVED_INTDEF,       // <tipoInt> ::= "int"
  RESERVED_FLOATDEF,     // <tipoFloat> ::= "float"
  RESERVED_STRINGDEF,    // <tipoString> ::= "string"
  RESERVED_BOOLDEF,      // <tipoBool> ::= "bool"
  RESERVED_E,            // <eSignRes> ::= "e" | "E"
  RESERVED_COMP_OP,      // <comparisonOp> ::= ">" | "<" | "==" | "!="
  RESERVED_OPENTOKEN,    // <openP> ::= "("
  RESERVED_CLOSETOKEN,   // <closeP> ::= ")"
  RESERVED_SEPARATOR,    // <separator> ::= ","
  RESERVED_WHITESPACE,   // <whitespace> ::= " "
  RESERVED_LINEBREAK,    // <linebreak> ::= "\n"
  RESERVED_BEGIN,        // <begin> ::= "{"
  RESERVED_END,          // <end> ::= "}"
  RESERVED_EQUAL_RES,    // <equalSignRes> ::= "="
  RESERVED_DOUBLE_DOT,   // <doubleDotRes> ::= ":"
  RESERVED_PLUS_SIGN,    // <plusSignRes> ::= "+"
  RESERVED_MINUS_SIGN,   // <minusSignRes> ::= "-"
  RESERVED_TIMES_SIGN,   // <timesSignRes> ::= "*"
  RESERVED_DIVIDE_SIGN,  // <dividerSignRes> ::= "/"
  RESERVED_FACT_SIGN,    // <factSignRes> ::= "!"
  RESERVED_DOT_SIGN,     // <dotSignRes> ::= "."
  RESERVED_POT_SIGN,     // <PotSignRes> ::= "^"
  RESERVED_QUOTE_RES,    // <quotesRes> ::= "\""
    
  IDENTIFIER_GET,        // Definição de um identificado
  IDENTIFIER_SET,        // Real definição
    
  GET_INTEGER_SEQUENCE,  // Definição de um valor inteiro
  SET_INTEGER_SEQUENCE,  // Real definição
  ATTR_INTEGER_SEQUENCE, // Atribuição de um valor inteiro
    
  GET_STRING_SEQUENCE,   // Definição de uma string
  SET_STRING_SEQUENCE,   // Real definição
  ATTR_STRING_SEQUENCE,  // Atribuição de um valor String
    
  GET_BOOL_VALUE,        // Definição de um bool
  SET_BOOL_VALUE,        // Real definição
  ATTR_BOOL_SEQUENCE,    // Atribuição de um valor bool
    
  SET_FLOAT_VALUE,       // Real definição de um float
  ATTR_FLOAT_SEQUENCE,   // Atribuição de um valor float
    
  SKIP_TAB               // Pros tabs
};

enum {
  INT_DEFINITION,
  INT_ATRIBUTION,
  FLT_DEFINITION,
  FLT_ATRIBUTION
};


int saveDefToHash(ht_t **hashmap, char type[], char toSave[]);
int compareToReserveds(void);
int detectIdentifier(void);
int processReserved(char reservedWord[], int n);
int reservedCopOp(void);
int reservedTab(void);
int reservedE(void);
int getInteger(void);
int setInteger(ht_t **hashmap);
int atributeInt(ht_t **hashmap);
int getFloat(void);
int setFloat(ht_t **hashmap);
int getString(void);
int setString(ht_t **hashmap);
int getBool(void);
int atributeBool(ht_t **hashmap);
int runLexer(int wordCode, char **pointer, ht_t **hashmap);
void logError(char error[]);
void defineIsDebug(int isDebugMode);
