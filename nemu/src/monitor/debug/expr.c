#include "nemu.h"
#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NEQ ,TK_AND ,TK_OR,TK_NO,TK_ADDRESS,TK_NEGATIVE,TK_NUM,TK_16NUM,TK_REG 

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},           //MINUS
  {"/", '/'},           //DIVIDE
  {"\\*", '*'},         // MULITE
  {"\\(", '('},         //left
  {"\\)", ')'},         //right			  
  {"==", TK_EQ},         // equal
  {"[0123456789]{1,31}", TK_NUM}  //NUM

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  int m;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
         case TK_NOTYPE : break;
	 case '+' : tokens[nr_token].type = '+';
		    nr_token++;
                    break;
         case '-' : tokens[nr_token].type = '-';
		    nr_token++;
		    break;
         case '/' : tokens[nr_token].type = '/';
		    nr_token++;
                    break;
         case '*' : tokens[nr_token].type = '*';
		    nr_token++;
		    break;
	 case '(' : tokens[nr_token].type = '(';
		    nr_token++;
		    break;
         case ')' : tokens[nr_token].type = ')';
		    nr_token++;
		    break;
         case TK_NUM : tokens[nr_token].type = TK_NUM; 
                       for( m = 0;m < substr_len;m++)
		       {
			 tokens[nr_token].str[m] = *(substr_start + m);
		       }
		       m = substr_len;
		       tokens[nr_token].str[m] = '\0';
		       nr_token++;
		       break;
          default:    TODO();
		       break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  printf("nr_token : %d\n",nr_token);

  return true;
}
static bool check_parentheses(int p,int q)
{
	if(tokens[p].type!='('||tokens[q].type!=')')
	{
		return false;
	}
	else
	{
		int parnum = 1;
		int m;
		for( m = p+1;m<q;m++)
		{
	            if(tokens[m].type=='(' )
		    {
                          parnum++;
		    }
                    if(tokens[m].type==')' )
		    {
			  parnum--;
		    }
                    if(parnum==0)
		    {
			    return false;
		    }

	        }
               return true;
	}
}
static int op_location(int p,int q)
{
   int parnum = 0;
   int m;
   bool detected1,detected2,possess1,possess2;
   possess1=possess2=false;
   int location1,location2;
   location1=location2= -1;
   for( m = p;m<q+1;m++)
   {
	   detected1=detected2=false;
	   if(tokens[m].type=='('  )
	   {
                  parnum++;
	   }
           if(tokens[m].type==')' )
	   {
		   parnum--;
	   }
	   if(parnum==0)
	   {
		   if(tokens[m].type=='+'||tokens[m].type=='-' )
		   {
			   detected1 = true;
		   }
		   if(tokens[m].type=='*'||tokens[m].type=='/' )
		   {
			   detected2 = true;
		   }
	   }
	   if(detected1 )
	   {
                 possess1 = true;
		 location1 = m;
	   }
	   if(detected2 )
	   {
		 possess2 = true;
		 location2 = m;
	   }
   }
   if(possess1 )
   {
           return location1;
   }
   else
   {
	   if(possess2 )
	   {
                 return location2;
	   }
	   else
	   {
		   return -1;
	   }
   }

}
uint32_t eval(int p,int q)
{
  if(p==q)
  {
    int val = atoi(tokens[p].str);
    return val;
  }
  else
  {
    if(check_parentheses(p, q))
    {
       return eval(p+1,q-1);
    }
    else
    {
       int op = op_location(p,q);
       if (op==-1)
       {
	       return 0;
       }
       int val1 = eval(p,op-1);
       int val2 = eval(op+1,q);
       switch(tokens[op].type )
       {
	   case '+' : return val1+val2;
		      break;
           case '-' : return val1-val2;
		      break;
           case '*' : return val1*val2;
		      break;
           case '/' : return val1/val2;
		      break;
           default  : return 0;
		      break;
       }
    }
  }
}
uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  
  return eval(0,nr_token-1);

}
