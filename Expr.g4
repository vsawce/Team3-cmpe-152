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

program: statement+ ;  // at least one statement

statement : expr NEWLINE                
          | IDENTIFER '=' expr NEWLINE        
          | NEWLINE                   
          ;

expr: expr ('*'|'/') expr   
    | expr ('+'|'-') expr   
    | INTEGER                    
    | IDENTIFER                  
    | '(' expr ')'         
    ;

NEWLINE     : '\r'? '\n' ;
STRING_LIT  : '\'' ('\'\'' | ~ ('\''))* '\'';

AND             : 'and';
ARRAY           : 'array';
ASM             : 'asm';
BEGIN           : 'begin';
BREAK           : 'break';
CASE            : 'case';
CONST           : 'const';
CONSTRUCTOR     : 'constructor';
CONTINUE        : 'continue';
DESTRUCTOR      : 'destructor';
DIV             : 'div';
DO              : 'do';
DOWNTO          : 'downto';
ELSE            : 'else';
END             : 'end';
FALSE           : 'FALSE';
FILE            : 'file';
FOR             : 'for';
FUNCTION        : 'function';
GOTO            : 'goto';
IF              : 'if';
IMPLEMENTATION  : 'implementation';
IN              : 'in';
INLINE          : 'inline';
INTERFACE       : 'interface';
LABEL           : 'label';
MOD             : 'mod';
NIL             : 'nil';
NOT             : 'not';
OBJECT          : 'object';
OF              : 'of';
ON              : 'on';
OPERATOR        : 'operator';
OR              : 'or';
PACKED          : 'packed';
PROCEDURE       : 'procedure';
PROGRAM         : 'program';
RECORD          : 'record';
REPEAT          : 'repeat';
SET             : 'set';
SHL             : 'shl';
STRING          : 'string';
THEN            : 'then';
TO              : 'to';
TRUE            : 'true';
TYPE            : 'type';
UNIT            : 'unit';
UNTIL           : 'until';
USES            : 'uses';
VAR             : 'var';
WHILE           : 'while';
WITH            : 'with';
XOR             : 'xor';
INTEGER         : [0-9]+ ;
REAL            : ('0' .. '9') + (('.' ('0' .. '9') + (EXPONENT)?)? | EXPONENT);
IDENTIFER       : [a-zA-Z]+ ; 
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
LBRACE          : '{';
RBRACE          : '}';
LCOMMENT        : '(*';
RCOMMENT        : '*)';

DOT             : '.';

fragment EXPONENT
   : ('E') ('+' | '-')? ('0' .. '9') +
   ;

WS : [ \t]+ -> skip ; 