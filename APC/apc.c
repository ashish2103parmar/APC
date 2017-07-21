#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <apc.h>
#pragma pack(1)
#define precision 50 // precision can be any value between 0 - (2^31 - 1)

/* create double link list */
dll *create_dll(unsigned int data)
{
  /* allocate memory */
  dll *tmp = malloc(sizeof (dll));
  if (tmp)
  {
	/* initialize memory */
	tmp->data = data;
	tmp->left = NULL;
	tmp->right = NULL;
  }
  return tmp;
}

/* create decimal num struct */
apn *create_apn(dll *list, unsigned int dpos, unsigned int nflag)
{
  /* allocate memory */
  apn *tmp = malloc(sizeof (apn));
  if (tmp)
  {
	/* initialize memory */
	tmp->list = list;
	tmp->dpos = dpos;
	tmp->nflag = nflag;
  }
  return tmp;
}

/* insert first in dll */
dll *insert_first_dll(dll *head, dll *node)
{
  if (node)
  {
	/* link head */
  	node->right = head;
	if (head)
		head->left = node;
	/* return new head */
	return node;
  }
  return NULL;
}

/* insert last in dll*/
dll *insert_last_dll(dll *head, dll *node)
{
 dll *ptr = head;
 /* go to last */
 while (ptr && ptr->right)
   ptr = ptr->right;
 /* insert node */
 if (ptr)
 	ptr->right = node;
 else
   return node;
 /* link ptr */
 if (node)
 	node->left = ptr;
 return head;
}

/* add 2 dll */
dll *addlist(dll *va, dll *vb)
{
  dll *result = NULL;
  long long sum = 0;
  
  /* while va or vb */
  while (va || vb)
  {
	/* if va has data then add*/
	if (va && va->data)
	  sum += (long long)va->data;
	/* if vb has data then add*/
	if (vb && vb->data)
	  sum += (long long)vb->data; 
	/* insert in result */
	result = insert_last_dll(result, create_dll((unsigned int)sum));
	/* shift sum */
	sum = sum >> 32;
	/* move to next node */
	if (va)
		va = va->right;
	if (vb)
		vb = vb->right;
  }
  /* if carry insert in result */
  if (sum)
	result = insert_last_dll(result, create_dll((unsigned int)sum));
	
  return result;
}

/* mul 2 dll */
dll *mullist(dll *va, dll *vb)
{
  dll *result = NULL;
  long long sum = 0, count = 0, idx;
  dll *tmp = NULL, *tmpa = va;

  /* till end of vb */
  while (vb)
  {
	tmp = NULL;
	sum = 0;
	
	/* shift tmp */
	for (idx = 0; idx < count; idx++)
		tmp = insert_last_dll(tmp, create_dll(0));
	
	/* till end of va */
	while (va)
	{
	  	/* mul */
	  	sum += (unsigned long long)va->data * vb->data;
		/* insert in tmp */
		tmp = insert_last_dll(tmp, create_dll((unsigned int)sum));
		/* shift sum */
		sum = sum  >> 32;
		va = va->right;
	}
  	
	/* insert carry */
	if (sum)
		tmp = insert_last_dll(tmp, create_dll((unsigned int)sum));
	
	/* add to result */
	result = addlist(result, tmp);
	count++;
	
	va = tmpa; 
	vb = vb->right;
  }
  return result;
}

/* take 2'complement of dll */
dll *comp_2(dll *va)
{
  dll *ptr = va;
  long long sum = 1;
  /* while end of list */
  while (ptr)
  {
	/* take complement and add */
	sum += ~ptr->data;
	/* save data */
	ptr->data = (unsigned int)sum;
	/* shift data */
	sum >>= 32;
	ptr = ptr->right;
  }

  return va;
}

/* sub 2 dll */
dll *sublist(dll *va, dll *vb)
{
  dll *result = NULL;
  long long sum = 0;

  /* while va or vb */
  while (va || vb)
  {
	if (va && va->data)
	/* if va has data then add */
	  sum += (long long)va->data;
	if (vb && vb->data)
	/* if vb has data then sub */
	  sum -= (long long)vb->data; 
	/* insert in result */
	result = insert_last_dll(result, create_dll((unsigned int)(sum)));
	/* shift sum */
	sum = sum >> 32;
	if (va)
		va = va->right;
	if (vb)
		vb = vb->right;
  }
  
  /* if sign then insert */
  if (sum)
	result = insert_last_dll(result, create_dll((unsigned int)sum));

  return result;
}

/* check if va less than vb */
int ltlist(dll *va, dll *vb)
{
  /* move till end of any one list reached */
  while (va && vb && va->right && vb->right)
  {
	va = va->right;
	vb = vb->right;
  }
  /* check which link reached end */
  if ( vb && vb->right)
	return 1;
  else if (va && va->right)
	return 0;
  else
  {
	/* if both are of equal length */
	while (va)
	{
	  /* comare from msword to lsword*/
	  if (va->data < vb->data)
		return 1;
	  else if (va->data > vb->data)
		return 0;

	  va = va->left;
	  vb = vb->left;
	}
  }
  return 0;
}

/* div 2 dll */
dll *divlist(dll *va, dll *vb, dll **mod)
{
  dll *ptr = va, *tmp = create_dll(0), *que = create_dll(0), *d2 = create_dll(2), *tmpv = create_dll(0);
  int idx;

  /* move to end of dividend */
  while (ptr && ptr->right)
	ptr = ptr->right;

  while (ptr)
  {
	/* taking one bit at a time */
	for (idx = sizeof (int) * 8 - 1; idx >= 0; idx--)
	{
	  /* take one bit */
	  tmpv->data = (ptr->data >> idx) & 1;
	  /* append to tmp */
	  tmp = addlist(mullist(tmp, d2), tmpv);	
	  /* shift que by one */
	  que = mullist(que, d2);
	  /* if tmp is greater than vb then sub vb*/
	  if (!ltlist(tmp, vb))
	  {
		tmp = sublist(tmp, vb);
		/* add 1 */
		tmpv->data = 1;
		que = addlist(que, tmpv);
	  }
	}
	ptr = ptr->left;
  }
  /* ger remainder */
  *mod = tmp;
  return que;
}

/* debug print */
/*
void debugprint(apn *tmp)
{
  if (tmp)
  {
  	if (tmp->nflag)
		putchar('-');
 
	dll *ptr = tmp->list;
	while (ptr && ptr->right)
	  ptr = ptr->right;
  	while (ptr)
  	{
		printf("%x ", ptr->data);
		ptr = ptr->left;
  	}
  	printf("(%u)\n", tmp->dpos);
  }
  else
	printf("no num\n");
}
*/

/* string to apn */
apn *str2apn(char *str)
{
  unsigned int nflag = 0, dflag = 0, dpos = 0;
  dll *tmp = create_dll(0), *d10 = create_dll(10), *tmpv = create_dll(0);
  /* check neg sign */
  if (*str == '-')
  {
	nflag = 1;
	str++;
  }
  /* while end of str */
  while (*str != '\0')
  {
	/* check for decimal point */
	if (*str == '.')
	  dflag = 1;
	else if (*str >= '0' && *str <= '9')
	{
	  if (dflag)
		dpos++;
	  /* get element */
	  tmpv->data = *str - '0';
	  /* add to num */
	  tmp = addlist(mullist(tmp, d10), tmpv);
	}
	else if (*str != ' ')
	  return NULL;
	str++;
  }
	/* return apn */
  return create_apn(tmp, dpos, nflag);
}

int process_str(char *str)
{
  int idx = 0;
  char buff[200], op;
  /* get va */
  while (!strchr("+-*/^%", str[++idx]));
  strncpy(buff, str, idx);
  buff[idx] = '\0';
  apn *va = str2apn(buff);
  /* get op */
  op = str[idx];
  /* get vb */
  apn *vb = str2apn(str + idx + 1);

  /* check va and vb */
  if (va && vb)
  {
	/* do operation */
	switch (op)
	{
	  case '+':
		/* add */
		printapn(decimal_add(va, vb));
		break;
	  case '-':
		/* sub */
		printapn(decimal_sub(va, vb));
		break;
	  case '*':
		/* mul */
		printapn(decimal_mul(va, vb));
		break;
	  case '/':
		/* div */
		printapn(decimal_div(va, vb, precision)); //precision can be set to any value 
		break;
	  case '%':
		/* mod */
		printapn(mod(va, vb));
		break;
	  case '^':
		/* power */
		printapn(power(va, vb));
		break;
	  default:
		return 0;
	}
  }
  else
	return 0;

  return 1;
}

/* append zeros */
void append_zeros(apn *va, int dpos)
{
  dll *d10 = create_dll(10);
  /* append till dpos */
  while (dpos)
  {
	/* mul 10 */
	va->list = mullist(va->list, d10);
	va->dpos++;
	dpos--;
  }
}

/* add two apn */
apn *decimal_add(apn *va, apn *vb)
{
  apn *result = create_apn(NULL, 0, 0);
 
  /* make dpos of both va and vb equal */
  if (va->dpos > vb->dpos)
	append_zeros(vb, va->dpos - vb->dpos);
  else if (va->dpos < vb->dpos)
	append_zeros(va, vb->dpos - va->dpos);

  /* get dpos */
  result->dpos = va->dpos;
  /* both have different sign */
  if (va->nflag ^ vb->nflag)
  {	
	/* sub */
	result->list = sublist(va->list, vb->list);
	/* check if va is smaller then vb */
	if (ltlist(va->list, vb->list))
	{	
	  /* take 2's complement */
	  result->list = comp_2(result->list);
	  /* get vb sign */
	  result->nflag = vb->nflag;
	}
	else
	  /* get va sign */
	  result->nflag = va->nflag;
  }
  else
  {
	/* add */
	result->list = addlist(va->list, vb->list);
	/* get va sign */
	result->nflag = va->nflag;
  }
  return result;
}

/* sub 2 apn */
apn *decimal_sub(apn *va, apn *vb)
{
  /* negate vb sign */
  vb->nflag = ~vb->nflag;
  /* add va and vb */
  apn *result = decimal_add(va, vb);
  /* change vb sign */
  vb->nflag = ~vb->nflag;
  return result;
}

/* nul two apn */
apn *decimal_mul(apn *va, apn *vb)
{
  /* return mul result */
  return create_apn(mullist(va->list, vb->list), va->dpos + vb->dpos, va->nflag ^ vb->nflag);
}

/* div two apn with precision (prn) */
apn *decimal_div(apn *va, apn *vb, int prn)
{
  int dpos = va->dpos;
  /* append zeros */
  if (prn > va->dpos - vb->dpos)
 	 append_zeros(va, prn - va->dpos + vb->dpos);
  /* divide */
  dll *mod, *que = divlist(va->list, vb->list, &mod);
  /* remove zeros */
  set_dpos(va, dpos);
  /* return result */
  return create_apn(que, prn, va->nflag ^ vb->nflag);
}

/* set decimal pos of apn */
void set_dpos(apn *va, int dpos)
{
  /* append zeros */
  if (dpos > va->dpos)
	append_zeros(va, dpos - va->dpos);
  /* remove decimal */
  else if (dpos < va->dpos)
  {
  	dll *d10 = create_dll(10), *tmp;
	while (va->dpos != dpos)
	{
	  /* remove */
	  va->list = divlist(va->list, d10, &tmp);
	  va->dpos--;
	}
  }
}

/* get mod of two apn */
apn *mod(apn *va, apn *vb)
{
  dll *mod;
  /* get mod */
  divlist(va->list, vb->list, &mod);
  /* return mod */
  return create_apn(mod, 0, va->nflag);
}

/* get power of (only a ls-word of vb is used)*/
apn *power(apn *va, apn *vb)
{
  unsigned int itr = vb->list->data;
  apn *result = create_apn(create_dll(1), 0, 0);

  /* mul va, vb times*/
  while (itr--)
  {
	result = decimal_mul(result, va);
  }
  /* if vb is neg take reciprocal */
  if (vb->nflag)
	return decimal_div(create_apn(create_dll(1), 0, 0), result, 100);
  else
  	return result;
}

/* check if apn is zero */
int checkzero(dll *va)
{

  /* go through list */
  while(va)
  {
	/* return if non zero */
	if (va->data)
	  return 0;
	va = va->right;
  }
  /* return list is zero */
  return 1;
}

/* print dll using recursion with decimal point */
void printrecdec(dll *va, int num)
{	
  dll *d10 = create_dll(10), *tmp;
  
  /* return condition */
  if (num < 0 && checkzero(va))
	return;	
  
  /* take mod */
  va = divlist(va, d10, &tmp);
  /* get next */
  printrecdec(va, num - 1);
  /* print */
  printf("%u", tmp->data);
  /* print decimal point */
  if (!num)  
	printf(".");
  
}

/* print apn */
void printapn(apn *va)
{
  /* print sign */
  if (va->nflag)
	printf("-");
  /* print number */
  printrecdec(va->list, va->dpos);
  printf("\n");
}
