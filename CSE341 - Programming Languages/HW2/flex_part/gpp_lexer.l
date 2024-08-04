
%{	
	#include <stdio.h>
	#include <string.h>
   int flag = 0; 
   void parse(char str[]);  
%}


%%

[;]{2}.* {
	printf(";; : COMMENT\n");
}


and { printf("%s: KW_AND\n",yytext); }
or { printf("%s: KW_OR\n",yytext); }
not { printf("%s: KW_NOT\n",yytext); }
equal { printf("%s: KW_EQUAL\n",yytext); }
less { printf("%s: KW_LESS\n",yytext); }
nil { printf("%s: KW_NIL\n",yytext); }
list { printf("%s: KW_LIST\n",yytext); }
append { printf("%s: KW_APPEND\n",yytext); }
concat { printf("%s: KW_CONCAT\n",yytext); }
set { printf("%s: KW_SET\n",yytext); }
deffun { printf("%s: KW_DEFFUN\n",yytext); }
for { printf("%s: KW_FOR\n",yytext); }
if { printf("%s: KW_IF\n",yytext); }
exit { printf("%s: KW_EXIT\n",yytext); }
load { printf("%s: KW_LOAD\n",yytext); }
disp { printf("%s: KW_DISP\n",yytext); }
true { printf("%s: KW_TRUE\n",yytext); }
false { printf("%s: KW_FALSE\n",yytext); }

[+] { printf("%s: OP_PLUS\n",yytext); }
[-] { printf("%s: OP_MINUS\n",yytext); }
[/] { printf("%s: OP_DIV\n",yytext); }
[*] { printf("%s: OP_MULT\n",yytext); }
[(] { printf("%s: OP_OP\n",yytext); }
[)] { printf("%s: OP_CP\n",yytext); }
[*][*] { printf("%s: OP_DBLMULT\n",yytext); }
[,] { printf("%s: OP_COMMA\n",yytext); }


[0][0-9]+ {
	printf("%s: INVALID VALUE\n",yytext); // starts with 0
	return 0;
}

[01-9][0-9]* {
	printf("%s: VALUEI\n",yytext);
}

[0-9]+f{1}[0][0-9]* {
	printf("%s: INVALID VALUE\n",yytext); // 3f0 or 3f00302
	return 0;
}


[0-9]+f{1}[0-9]+ {
	printf("%s: VALUEF\n",yytext);
}

[0-9]+[a-zA-Z0-9_]+ {
	printf("%s: INVALID IDENTIFIER\n",yytext); //22ddd
	return 0;
}


[a-zA-Z][a-zA-Z0-9_]* {
	printf("%s: IDENTIFIER\n",yytext);
}


["]{1} {
	if(flag==1){
		printf("%s: OP_CC\n",yytext);
		flag = 0;
	}
	else{
		printf("%s: OP_OC\n",yytext);
		flag = 1;
	}
	
}

["].*["] {

	parse(yytext);
	
}

[\n\r\t] { }

. { } 


%%

void parse(char str[]){

	char *parse_str = str+1;
	parse_str[strlen(parse_str)-1] = '\0';

	printf("\": OP_OC\n%s: VALUESTR\n\": OP_CC\n",parse_str);
}

int yywrap(){}

int main(int argc, char *argv[]) {

	if(argc == 1){
		printf("Enter the code: ");
	}

	else if(argc == 2) {
		yyin = fopen(argv[1], "r");
	}

	yylex();

	return 0;
}