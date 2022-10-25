grammar Expr;

fragment A:[aA];
fragment B:[bB];
fragment C:[cC];
fragment D:[dD];
fragment E:[eE];
fragment F:[fF];
fragment G:[gG];
fragment H:[hH];
fragment I:[iI];
fragment J:[jJ];
fragment K:[kK];
fragment L:[lL];
fragment M:[mM];
fragment N:[nN];
fragment O:[oO];
fragment P:[pP];
fragment Q:[qQ];
fragment R:[rR];
fragment S:[sS];
fragment T:[tT];
fragment U:[uU];
fragment V:[vV];
fragment W:[wW];
fragment X:[xX];
fragment Y:[yY];
fragment Z:[zZ];

program: programHeader compStatement DOT EOF;  // at least one statement

programHeader: PROGRAM identifier SEMICOLON;

block : (labelDeclarationPart | constantDefinitionPart | typeDefinitionPart | variableDeclarationPart | procedureAndFunctionDeclarationPart | usesUnitsPart | IMPLEMENTATION)* compoundStatement;

compStatement : BEGIN statements END;

assignStatement : identifier ASSIGN expr;

ifStatement : IF expr THEN statement (: ELSE statement)?;

whileStatement : WHILE expr DO statement;

forStatement : FOR identifier ASSIGN startValue (TO | DOWNTO) endValue DO statement;

repeatStatement : REPEAT statements UNTIL expression;

conditionalStatement : ifStatement
                     | caseStatement
                     ;
                    
repetitiveStatement : whileStatement
                    | repeatStatement
                    | forStatement
                    ;

withStatement : WITH recordVariableList DO statement;

statement : compStatement
          | assignStatement                   
          | ifStatement
          | whileStatement
          | forStatement
          | repeatStatement
          | conditionalStatement
          | repetitiveStatement
          | withStatement
          ;

statements : statement (SEMICOLON (statement)?)*;

expr: expr ('*'|'/') expr   
    | expr ('+'|'-') expr
    | identifier relationalOperator expr
    | INTEGER
    | REAL                    
    | identifier                  
    | '(' expr ')'         
    ;

constant : unsignedNumber
         | sign unsignedNumber
         | IDENTIFIER
         | sign IDENTIFIER
         | LITERAL
         | constantChr
         ;

unsignedNumber : INTEGER
               | REAL
               ;

constantChr : CHR LPAREN REAL RPAREN;

relationalOperator: EQUAL
                  | NE
                  | LTEQ
                  | GTEQ
                  | LT
                  | GT;

type : simpleType
     | structuredTyped
     | pointerType
     ;

simpleType : scalarType
           | subrangeType
           | typeIdentifier
           | stringType
           ;

scalarType : LPAREN identifierList RPAREN;

subrangeType : costant DOTDOT constant;

typeIdentifier : identifier
               | (CHARACTER | BOOLEAN | INTEGER | REAL | STRING)
               ;

stringType : STRING LBRACKET (identifier | unsignedNumber) RBRACKET;

structuredTyped : PACKED unpackedStructuredType
                | unpackedStructuredType
                ;

unpackedStructuredType : arrayType
                       | recordType
                       | setType
                       | fileType
                       ;

arrayType : ARRAY LBRACK typeList RBRACK OF componentType
          | ARRAY LBRACK2 typeList RBRACK2 OF componentType
          ;

typeList : indexType (COMMA indexType)*;

indexType : simpleType;

componentType : type;

recordType : RECORD fieldList? END;

fieldList : fixedPart (SEMI variantPart)?
          | variantPart
          ;

fixedPart : recordSection (SEMI recordSection)*;

recordSection : identifierList COLON type;

variantPart : CASE tag OF variant (SEMI variant)*;

tag : identifier COLON typeIdentifier
    | typeIdentifier
    ;

variant : constList COLON LPAREN fieldList RPAREN;

setType : SET OF baseType;

baseType : simpleType;

fileType : FILE OF type
         | FILE
         ;

pointerType : POINTER typeIdentifier;

identifierList
   : identifier (COMMA identifier)*
   ;

startValue : expr;
endValue   : expr;

identifier : IDENTIFIER;

STRING_LIT  : '\'' ('\'\'' | ~ ('\''))* '\'';

AND             : A N D;
ARRAY           : A R R A Y;
ASM             : A S M;
BEGIN           : B E G I N;
BOOLEAN         : B O O L E A N;
BREAK           : B R E A K;
CASE            : C A S E;
CONST           : C O N S T;
CONSTRUCTOR     : C O N S T R U C T O R;
CONTINUE        : C O N T I N U E;
DESTRUCTOR      : D E S T R U C T O R;
DIV             : D I V;
DO              : D O;
DOWNTO          : D O W N T O;
ELSE            : E L S E;
END             : E N D;
FALSE           : F A L S E;
FILE            : F I L E;
FOR             : F O R;
FUNCTION        : F U N C T I O N;
GOTO            : G O T O;
IF              : I F;
IMPLEMENTATION  : I M P L E M E N T A T I O N;
IN              : I N;
INLINE          : I N L I N E;
INTERFACE       : I N T E R F A C E;
LABEL           : L A B E L;
MOD             : M O D;
NIL             : N I L;
NOT             : N O T;
OBJECT          : O B J E C T;
OF              : O F;
ON              : O N;
OPERATOR        : O P E R A T O R;
OR              : O R;
PACKED          : P A C K E D;
PROCEDURE       : P R O C E D U R E;
PROGRAM         : P R O G R A M;
RECORD          : R E C O R D;
REPEAT          : R E P E A T;
SET             : S E T;
SHL             : S H L;
STRING          : S T R I N G;
THEN            : T H E N;
TO              : T O;
TRUE            : T R U E;
TYPE            : T Y P E;
UNIT            : U N I T;
UNTIL           : U N T I L;
USES            : U S E S;
VAR             : V A R;
WHILE           : W H I L E;
WITH            : W I T H;
XOR             : X O R;
INTEGER         : (MINUSOP)? [0-9]+ ;
REAL            : (MINUSOP)? [0-9]* '.' [0-9]+;
IDENTIFIER      : [a-zA-Z]+ ; 
PLUSOP          : '+';
MINUSOP         : '-';
MULTOP          : '*';
DIVOP           : '/';
ASSIGN          : ':=';
EQUAL           : '=';
NE              : '<>';
LTEQ            : '<=';
GTEQ            : '>=';
LT              : '<';
GT              : '>';
PLUSEQUAL       : '+=';
MINUSEQUAL      : '-=';
MULTEQUAL       : '*=';
DIVEQUAL        : '/=';
CARAT           : '^';
SEMICOLON       : ';';
COMMA           : ',';
LPAREN          : '(';
RPAREN          : ')';
LBRACKET        : '[';
RBRACKET        : ']';
LBRACKET2       : '(.';
RBRACKET2       : '.)';
LBRACE          : '{';
RBRACE          : '}';
LCOMMENT        : '(*';
RCOMMENT        : '*)';
AT              : '@';
DOT             : '.';
DOTDOT          : '..';

COMMENT_1       : '(*' .*? '*)' -> skip;
COMMENT_2       : '{*' .*? '*}' -> skip;
LITERAL         : '\'' ('\'\'' | ~ ('\''))* '\'';
fragment EXPONENT : ('E') ('+" | '-')? ('0' .. '9') +;
WS : [ \t\r\n]+ -> skip ; 