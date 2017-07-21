
#ifndef APC
#define APC

typedef struct D_L_L
{
  unsigned int data;
  struct D_L_L *left, *right;
} dll;

typedef struct
{
  unsigned int dpos:31;
  unsigned int nflag:1;
  dll *list;
} apn;

/* process expr */
int process_str(char *str);

/* create double link list */
dll *create_dll(unsigned int data);

/* create decimal num struct */
apn *create_apn(dll *list, unsigned int dpos, unsigned int nflag);

/* operation on dll */
dll *addlist(dll *va, dll *vb);
dll *mullist(dll *va, dll *vb);
dll *sublist(dll *va, dll *vb);
dll *divlist(dll *va, dll *vb, dll **mod);

/* str 2 apn */
apn *str2apn(char *str);

//void debugprint(apn *tmp);

/* add */
apn *decimal_add(apn *va, apn *vb);

/* sub */
apn *decimal_sub(apn *va, apn *vb);

/* mul */
apn *decimal_mul(apn *va, apn *vb);

/* div */
apn *decimal_div(apn *va, apn *vb, int);

/* set decimal point */
void set_dpos(apn *va, int dpos);

/* power of */
apn *power(apn *va, apn *vb);

/* mod */
apn *mod(apn *va, apn *vb);

/* print apn */
void printapn(apn *va);

#endif
