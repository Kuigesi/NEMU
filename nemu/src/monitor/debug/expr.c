#include "nemu.h"
#include <stdlib.h>
#include <stdio.h>

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
  {"!=", TK_NEQ},         //NOTEQUAL
  {"&&", TK_AND},         //AND
  {"\\|\\|", TK_OR},      //OR
  {"!",TK_NO},     //NO
  {"\\$(eax|ecx|edx|ebx|esp|ebp|esi|edi|eip)",TK_REG},  //REG
  {"0x([0123456789abcdef]{1,29})",TK_16NUM}, //16NUM
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
         case '-' : if(nr_token!=0)
		    {
		      if(tokens[nr_token-1].type == ')'||tokens[nr_token-1].type == TK_NUM||tokens[nr_token-1].type == TK_16NUM||tokens[nr_token-1].type == TK_REG )
		      {
			    tokens[nr_token].type = '-';  
		      }
		      else
		      {
			    tokens[nr_token].type = TK_NEGATIVE;
		      }
		    }
                    else
		    {
			    tokens[nr_token].type = TK_NEGATIVE;
		    }
		    nr_token++;
		    break;
         case '/' : tokens[nr_token].type = '/';
		    nr_token++;
                    break;
         case '*' : if(nr_token!=0)
		    {
                      if(tokens[nr_token-1].type == ')'||tokens[nr_token-1].type == TK_NUM||tokens[nr_token-1].type == TK_16NUM||tokens[nr_token-1].type == TK_REG )
		      {
			   tokens[nr_token].type = '*';   
		      }
		      else
		      {
			   tokens[nr_token].type = TK_ADDRESS;   
		      }
		    }
		    else
		    {
			  tokens[nr_token].type = TK_ADDRESS;
		    }
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
         case TK_16NUM : tokens[nr_token].type = TK_16NUM;
			 for( m = 0;m < substr_len;m++)
			 {
		           tokens[nr_token].str[m] = *(substr_start + m);
			 }
			 m = substr_len;
			 tokens[nr_token].str[m] = '\0';
                         nr_token++;
			 break;
	case TK_REG : tokens[nr_token].type = TK_REG;
		      for( m = 0;m < substr_len-1;m++)
	              {
			  tokens[nr_token].str[m] = *(substr_start + m + 1);   
		      }
		      m = substr_len-1;
		      tokens[nr_token].str[m] = '\0';
		      nr_token++;
		      break;
       case TK_EQ : tokens[nr_token].type = TK_EQ;
		    nr_token++;
		    break;
       case TK_NEQ : tokens[nr_token].type = TK_NEQ;
		     nr_token++;
		     break;
       case TK_AND : tokens[nr_token].type = TK_AND;
                     nr_token++;
		     break; 
       case TK_OR : tokens[nr_token].type = TK_OR;
		    nr_token++;
		    break;
       case TK_NO : tokens[nr_token].type = TK_NO;
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
   bool detected0,detectedand,detectedor,possess0,possessand,possessor;
   possess1=possess2=false;
   possess0=possessand=possessor=false;
   int location1,location2;
   int location0,locationand,locationor;
   location1=location2=location0=locationand=locationor=-1;
   for( m = p;m<q+1;m++)
   {
	   detected1=detected2=false;
	   detected0=detectedand=detectedor=false;
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
		   if(tokens[m].type==TK_EQ||tokens[m].type==TK_NEQ )
		   {
			   detected0 = true;
		   }
		   if(tokens[m].type==TK_AND)
		   {
			   detectedand = true;
		   }
		   if(tokens[m].type==TK_OR)
		   {
			   detectedor = true;
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
	   if(detected0 )
	   {
		 possess0 = true;
		 location0 = m;
	   }
	   if(detectedand )
	   {
		 possessand = true;
		 locationand = m;
	   }
	   if(detectedor )
	   {
		 possessor = true;
		 locationor = m;
	   } 
   }
   if(possessor )
   {
           return locationor;
   }
   else
   {
	   if(possessand )
	   {
                 return locationand;
	   }
	   else
	   {
		 if(possess0 )
		 {
                       return location0;
		 }
		 else
		 {
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
	   }
   }

}
uint32_t eval(int p,int q)
{
  if(p==q)
  {
    uint32_t val;
   if(tokens[p].type==TK_NUM )
   {
     val = atoi(tokens[p].str);	   
   } 
   if(tokens[p].type==TK_16NUM)
   {
     int mn ;
     mn = sscanf(tokens[p].str,"%x",&val);
     if(mn!=1 )
     {
	mn =1;
     }     
   }
   if(tokens[p].type==TK_REG)
   {
     char *regp = tokens[p].str;
     char regid[3];
     regid[0] = *regp;
     regid[1] = *(regp + 1);
     regid[2] = *(regp + 2);
     if(regid[1]=='a'&&regid[2]=='x')
     {
	     val= cpu.eax;
     }
     if(regid[1]=='b'&&regid[2]=='x')
     {
	     val= cpu.ebx;
     }
     if(regid[1]=='c'&&regid[2]=='x')
     {
	     val= cpu.ecx;
     }
     if(regid[1]=='d'&&regid[2]=='x')
     {
	     val= cpu.edx;
     }
     if(regid[1]=='s'&&regid[2]=='p')
     {
	     val= cpu.esp;
     }
     if(regid[1]=='b'&&regid[2]=='p')
     {
	     val= cpu.ebp;
     }
     if(regid[1]=='s'&&regid[2]=='i')
     {
	     val= cpu.esi;
     }
     if(regid[1]=='d'&&regid[2]=='i')
     {
	     val= cpu.edi;
     }
     if(regid[1]=='i'&&regid[2]=='p')
     {
	     val= cpu.eip;
     }
   }
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
	       if(tokens[p].type==TK_NEGATIVE)
	       { 
                   return -eval(p+1,q);
	       }
               if(tokens[p].type==TK_ADDRESS)
	       {
		   uint32_t result,addr;
		   addr = eval(p+1,q);
		   result = vaddr_read(addr,4);
		   return result;
	       }
	       if(tokens[p].type==TK_NO)
	       {
		   return (!eval(p+1,q));    
	       }

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
           case '/' : if(val2==0)
		      {
		        return 0;
		      }
		      else
		      {
		        return val1/val2;      
		      }
		      break;
           case TK_EQ : return (val1==val2);
		      break;
	   case TK_NEQ : return (val1!=val2);
		       break;
           case TK_AND : return (val1&&val2);
                       break;
           case TK_OR : return (val1||val2);
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
