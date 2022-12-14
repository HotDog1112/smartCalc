#include "s21_calc_polish.h"

lex_t* initLex(lex_t* lex) {
  lex = malloc(sizeof(lex_t));
  return lex;
}

node_t* initNode(lex_t* lex) {
  node_t* node = malloc(sizeof(node_t));
  if (node != NULL) {
    node->next = NULL;
    node->lexema = lex;
  }
  return node;
}

node_t* addNode(node_t* A, lex_t* lex) {
  node_t* new = NULL;
  if (A == NULL) {
    new = NULL;
  } else {
    new = initNode(lex);
    node_t* B = A->next;
    A->next = new;
    new->next = B;
  }
  return new;
}

node_t* addLastNode(node_t* head, lex_t* lex) {
  if (head != NULL) {
    node_t* tmp_head = head;
    while (tmp_head->next != NULL) {
      tmp_head = tmp_head->next;
    }
    addNode(tmp_head, lex);
  } else {
    head = initNode(lex);
  }
  return head;
}

node_t* deleteNode(node_t* head, node_t* find) {
  if (find != NULL && head != NULL) {
    node_t* tmp_head = head;
    node_t* before = NULL;
    while (before == NULL && tmp_head != NULL) {
      if (tmp_head->next != find) {
        tmp_head = tmp_head->next;
      } else {
        before = tmp_head;
      }
    }
    if (before == NULL) {
      if (head == find) {
        node_t* result = head->next;
        free(find);
        return result;
      } else {
        return head;
      }
    } else {
      before->next = find->next;
      free(find);
      return head;
    }
  } else {
    return head;
  }
}

int deleteListOfNodes(node_t* head) {
  int res = ERROR;
  node_t* tmp_head = head;
  if (head != NULL) {
    while (tmp_head != NULL) {
      node_t* A = tmp_head;
      free(A->lexema);
      free(A);
      tmp_head = tmp_head->next;
    }
    res = OK;
  }
  return res;
}

int deleteListOfNodes_2(node_t* head) {
  int res = ERROR;
  node_t* tmp_head = head;
  if (head != NULL) {
    while (tmp_head != NULL) {
      node_t* A = tmp_head;
      free(A);
      tmp_head = tmp_head->next;
    }
    res = OK;
  }
  return res;
}

tstack_t* initStack() {
  tstack_t* s = malloc(sizeof(tstack_t));
  s->peak = initNode(NULL);
  return s;
}

void Push(lex_t* lex, tstack_t* s) { addNode(s->peak, lex); }

lex_t* Pop(tstack_t* s) {
  lex_t* lexema = NULL;
  node_t* node = s->peak->next;
  if (node) {
    lexema = node->lexema;
    s->peak = deleteNode(s->peak, node);
  }
  return lexema;
}

lex_t* checkStackNode(tstack_t* s) {
  node_t* node = s->peak->next;
  return node->lexema;
}

void deleteStack(tstack_t* stack) {
  if (stack != NULL) {
    if (stack->peak != NULL) {
      deleteListOfNodes_2(stack->peak);
    }
    free(stack);
  }
}

int isFuncNameCorrect(char* str) {
  int res = ERROR;
  if (strcmp(str, "cos") == 0 || strcmp(str, "sin") == 0 ||
      strcmp(str, "tan") == 0 || strcmp(str, "acos") == 0 ||
      strcmp(str, "asin") == 0 || strcmp(str, "atan") == 0 ||
      strcmp(str, "sqrt") == 0 || strcmp(str, "ln") == 0 ||
      strcmp(str, "log") == 0 || strcmp(str, "mod") == 0) {
    res = OK;
  }
  return res;
}

int checkPriority(char a) {
  int pr = 0;
  switch (a) {
    case '+':
      pr = 1;
      break;
    case '-':
      pr = 1;
      break;
    case '*':
      pr = 2;
      break;
    case '/':
      pr = 2;
      break;
    case '^':
      pr = 3;
      break;
    case '(':
      pr = 4;
      break;
    case ')':
      pr = 4;
      break;
  }
  return pr;
}

int checkType(char a) {
  int type_of_node = NO;
  char is_func[14] = "cosintaqrglmd";
  if (strchr(is_func, (int)a) == NULL) {
    if ((int)a >= 48 && (int)a <= 57) {
      type_of_node = NUMBER;
    } else if (a == 'x') {
      type_of_node = X;
    } else {
      type_of_node = OPERATOR;
    }
  } else {
    type_of_node = FUNCTION;
  }
  return type_of_node;
}

int isOperator(char c) {
  int res = ERROR;
  if (c == '-' || c == '+' || c == '*' || c == '/' || c == '^' || c == '(' ||
      c == ')') {
    res = OK;
  }
  return res;
}

lex_t* fillLex(lex_t* lexema, char a, char* value, double num) {
  int type = checkType(a);
  if (type == OPERATOR) {
    switch (a) {
      case '(':
        lexema->type = '(';
        lexema->oper = a;
        break;
      case ')':
        lexema->type = ')';
        lexema->oper = a;
        break;
      default:
        lexema->type = 'o';
        lexema->oper = a;
        break;
    }
    lexema->priority = checkPriority(a);
    lexema->func_name[0] = '\0';
    lexema->number = 0.0;
  } else if (type == NUMBER) {
    lexema->type = 'n';
    lexema->func_name[0] = '\0';
    lexema->number = atof(value);
  } else if (type == FUNCTION) {
    if (strcmp(value, "mod") == 0) {
      lexema->type = 'o';
      lexema->oper = '%';
      lexema->priority = 2;
      lexema->number = 0.0;
    } else {
      lexema->type = 'f';
      strncat(lexema->func_name, value, strlen(value));
      lexema->func_name[4] = '\0';
      lexema->priority = 6;
      lexema->number = 0.0;
    }
  } else {
    lexema->type = 'x';
    lexema->func_name[0] = '\0';
    lexema->number = num;
  }
  return lexema;
}

/*_________POLISH NOTATION_________*/

double reversePolishNotation(node_t* node) {
  tstack_t* st = initStack();
  node_t* res = NULL;
  double result = 0.0;
  int flag_for_brackets = 0;
  int count_brackets = 0;
  node_t* inode = node;
  tstack_t* tmpst = st;
  if (node != NULL) {
    while (inode != NULL) {
      if (inode->lexema->type == 'f' || inode->lexema->type == 'u') {
        Push(inode->lexema, st);
      } else if (inode->lexema->type == 'n' || inode->lexema->type == 'x') {
        res = addLastNode(res, inode->lexema);
      } else if (inode->lexema->oper == '(') {
        Push(inode->lexema, st);
        count_brackets++;
      } else if (inode->lexema->oper == ')') {
        flag_for_brackets = 0;
        count_brackets++;
        lex_t* o2 = NULL;
        while (st->peak->next != NULL) {
          o2 = checkStackNode(st);
          if (o2->oper == '(') {
            Pop(st);
            flag_for_brackets = 1;
            count_brackets++;
            break;
          } else {
            Pop(st);
            res = addLastNode(res, o2);
          }
        }
        if (flag_for_brackets == 0) {
          result = 0;
          break;
        }
        if (st->peak->next != NULL) {
          o2 = checkStackNode(st);
          if (o2->type == 'f' && o2 != NULL) {
            Pop(st);
            res = addLastNode(res, o2);
          }
        }
      } else if (inode->lexema->type == 'o') {
        lex_t* o1 = inode->lexema;
        if (inode->lexema->type == 'o') {
          while (st->peak->next != NULL) {
            lex_t* o2 = checkStackNode(st);
            if ((o2->type != 'o' && o2->type != 'u') ||
                (o1->priority == 3 && o2->priority != 3) ||
                (o1->priority == 2 && o2->priority == 1)) {
              break;
            } else {
              Pop(st);
              res = addLastNode(res, o2);
            }
          }
        }
        Push(o1, st);
      }
      inode = inode->next;
    }
  }
  tmpst = st;
  while (st->peak->next != NULL) {
    res = addLastNode(res, st->peak->next->lexema);
    free(tmpst->peak);
    st->peak = st->peak->next;
  }
  deleteStack(tmpst);
  if (flag_for_brackets == 0 && count_brackets != 0) {
    deleteListOfNodes_2(res);
  } else {
    result = calcResult(res);
  }
  return result;
}

double calcResult(node_t* inode) {
  tstack_t* st = initStack();
  node_t* res_node = NULL;
  node_t* tmp_res_node = res_node;
  node_t* node = inode;
  double result = 0.0;
  int operations_counter = 0;
  int number_counter = 0;
  node_t* tmp_node = node;
  while (node != NULL) {
    lex_t* value = node->lexema;
    res_node = initNode(value);
    tmp_res_node = res_node;
    if (value->type == 'n' || value->type == 'x') {
      number_counter++;
      Push(value, st);
    } else if (value->type == 'o' || value->type == 'u' || value->type == 'f') {
      operations_counter++;
      if (st->peak->next != NULL) {
        node_t* num1 = st->peak->next;
        node_t* num2 = num1->next;
        if (value->type == 'o' && number_counter > 1) {
          if (value->oper == '/' && num1->lexema->number == 0) {
            num1->lexema->number = 0.0;
          }
          switch (value->oper) {
            case '+':
              res_node->lexema->number =
                  num2->lexema->number + num1->lexema->number;
              break;
            case '-':
              res_node->lexema->number =
                  num2->lexema->number - num1->lexema->number;
              break;
            case '*':
              res_node->lexema->number =
                  num2->lexema->number * num1->lexema->number;
              break;
            case '/':
              res_node->lexema->number =
                  num2->lexema->number / num1->lexema->number;
              break;
            case '^':
              res_node->lexema->number =
                  pow(num2->lexema->number, num1->lexema->number);
              break;
            case '%':
              res_node->lexema->number =
                  fmod(num2->lexema->number, num1->lexema->number);
              break;
          }
          number_counter--;
          Pop(st);
          Pop(st);
        } else if (value->type == 'f') {
          if (strcmp(value->func_name, "sin") == 0) {
            res_node->lexema->number = sin(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "cos") == 0) {
            res_node->lexema->number = cos(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "tan") == 0) {
            res_node->lexema->number = tan(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "acos") == 0) {
            res_node->lexema->number = acos(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "asin") == 0) {
            res_node->lexema->number = asin(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "atan") == 0) {
            res_node->lexema->number = atan(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "log") == 0) {
            res_node->lexema->number = log10(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "ln") == 0) {
            res_node->lexema->number = log(st->peak->next->lexema->number);
          } else if (strcmp(value->func_name, "sqrt") == 0) {
            res_node->lexema->number = sqrt(st->peak->next->lexema->number);
          }
          Pop(st);
        } else if (value->type == 'u') {
          if (value->oper == '-') {
            res_node->lexema->number = st->peak->next->lexema->number * (-1);
          } else {
            res_node->lexema->number = st->peak->next->lexema->number;
          }
          Pop(st);
        }
        Push(res_node->lexema, st);
      }
    }
    free(tmp_res_node);
    node = node->next;
  }
  result = res_node->lexema->number;

  if (operations_counter == 0 ||
      (number_counter > 1 && operations_counter == 0) ||
      (operations_counter > 1 && number_counter == 0)) {
    result = 0;
  }
  deleteListOfNodes_2(tmp_node);
  deleteStack(st);
  return result;
}

void isUnary(node_t* head) {
  lex_t* prev = NULL;
  node_t* tmp = head;
  while (tmp != NULL) {
    if (prev == NULL || prev->type == 'o' || prev->type == 'u' ||
        prev->oper == '(') {
      if (tmp->lexema->oper == '-' || tmp->lexema->oper == '+') {
        tmp->lexema->type = 'u';
        tmp->lexema->priority = 3;
      }
    }
    prev = tmp->lexema;
    tmp = tmp->next;
  }
}

double startCalc(char* str, double x_value) {
  node_t* node = NULL;
  double result = 0.0;
  char buffer_tmp[2] = "";
  char* tmp = NULL;
  int pc = 0, i = 0, flag = OK;
  lex_t* lexema = NULL;
  node = initNode(lexema);
  tmp = calloc(20, sizeof(char));
  while (*str != '=') {
    lexema = initLex(NULL);
    if (*str == ' ') {
      ++str;
    }
    if ((*str >= '0' && *str <= '9') || (*str == '.' && pc == 1)) {
      i++;
      while ((*str >= '0' && *str <= '9') || *str == '.') {
        if (*str == '.') {
          pc++;
        }
        if (pc <= 1) {
          buffer_tmp[0] = *str;
          strncat(tmp, buffer_tmp, 1);
          str++;
        } else {
          free(tmp);
          deleteListOfNodes(node);
          free(lexema);
          return result;
        }
      }
      node = addLastNode(node, fillLex(lexema, *buffer_tmp, tmp, 0.0));
      buffer_tmp[0] = '\0';
    } else if (checkType(*str) == OPERATOR) {
      if (isOperator(*str) == OK) {
        node = addLastNode(node, fillLex(lexema, *str, '\0', 0.0));
        i++;
      } else {
        result = 0;
        free(lexema);
        flag = ERROR;
        break;
      }
      ++str;
    } else if (checkType(*str) == X) {
      node = addLastNode(node, fillLex(lexema, *str, '\0', x_value));
      i++;
      ++str;
    } else if (checkType(*str) == FUNCTION) {
      while (checkType(*str) == FUNCTION) {
        buffer_tmp[0] = *str;
        strncat(tmp, buffer_tmp, 1);
        str++;
      }
      if (isFuncNameCorrect(tmp) == OK) {
        node = addLastNode(node, fillLex(lexema, *buffer_tmp, tmp, 0.0));
        i++;
      } else {
        result = 0;
        free(lexema);
        flag = ERROR;
        break;
      }
      buffer_tmp[0] = '\0';
    }
    pc = 0;
    tmp[0] = '\0';
  }
  if (i > 0 && flag == OK) {
    node_t* n = node->next;
    isUnary(n);
    result = reversePolishNotation(n);
    deleteListOfNodes(node);
    free(tmp);
  } else if (flag == ERROR) {
    deleteListOfNodes(node);
    free(tmp);
  } else {
    free(tmp);
    free(node);
    result = 0;
  }
  return result;
}

/*_________BONUS PART_________*/

int creditAnn_Calc(double credit_amount, int term, double rate,
                   double* mounth_sum, double* total_payment,
                   double* overpayment) {
  int res = ERROR;
  double mount_rate = 0, an_coff = 0;
  if (credit_amount > 0 && term > 0 && rate > 0 && rate <= 100) {
    mount_rate = rate / 12 / 100;
    an_coff = mount_rate * pow((1 + mount_rate), term) /
              (pow((1 + mount_rate), term) - 1);
    *mounth_sum = an_coff * credit_amount;
    *total_payment = (*mounth_sum) * term;
    *overpayment = term * (*mounth_sum) - credit_amount;
    res = OK;
  }
  return res;
}

int getDaysInMounth(int mounth) {
  return mounth == 2 ? 28 : 30 + (mounth > 7 ? mounth + 1 : mounth) % 2;
}

int getYearDays(int year) { return year % 4 == 0 ? 366 : 365; }

int creditDiff_Calc(double credit_amount, int term, double rate,
                    double* mounth_sum, double* total_payment,
                    double* overpayment, int mounth) {
  int res = ERROR;
  double mounth_payment = credit_amount / term;
  int current_year = 2022;
  if (mounth <= term) {
    int m_count = 10;
    if (mounth == 0) {
      mounth = term;
    }
    double tmp_sum = credit_amount;
    double tmp_mounth_sum = 0.0;
    for (int counter = 1; counter <= term; counter++) {
      if (m_count > 12) {
        m_count = 1;
      }
      double tmp = tmp_sum * (rate / 100) * getDaysInMounth(m_count) /
                   getYearDays(current_year);
      *total_payment += tmp;
      tmp_sum -= mounth_payment;
      tmp_mounth_sum = mounth_payment + tmp;
      if (mounth == counter) {
        *mounth_sum = tmp_mounth_sum;
      }
      m_count++;
    }
  }
  *total_payment += credit_amount;
  *overpayment = *total_payment - credit_amount;
  return res;
}

int takeCreditInfo(double credit_amount, int term, double rate, int type,
                   double* mounth_sum, double* total_payment,
                   double* overpayment, int mounth) {
  /* type 1 = ann, 2 = diff */
  int res = ERROR;
  if (type == 1 || type == 2) {
    res = OK;
    if (type == 1) {
      creditAnn_Calc(credit_amount, term, rate, mounth_sum, total_payment,
                     overpayment);
    } else if (type == 2) {
      creditDiff_Calc(credit_amount, term, rate, mounth_sum, total_payment,
                      overpayment, mounth);
    }
  }
  return res;
}

double taxCalc(double* percents, double* tax_sum, double tax,
               int mounths) {  // mounth
  if (tax == 0) {
    tax = 7.5;
  }
  int res = ERROR;
  if (tax > 0 && tax <= 100) {
    double tax_fix = 1000000.0 * mounths * (tax / 100) / 12.0;
    double summa_ndfl = *percents - tax_fix;
    if (summa_ndfl > 0) {
      *tax_sum = summa_ndfl * 0.13;
    } else {
      *tax_sum = 0;
    }
    res = OK;
  }
  return res;
}

double whatTypeOfPeriod(int payments_periodicity) {
  double period = 0.0;
  if (payments_periodicity == 3) {
    period = 12.0;                        /* year */
  } else if (payments_periodicity == 2) { /* quart */
    period = 3.0;
  } else if (payments_periodicity == 1) { /* mounth */
    period = 1.0;
  }
  return period;
}

int parserDatesAndAmounts(char* list, int* date, double* sum,
                          int current_mounth, int* flag) {
  int res = ERROR;
  int n = 0;
  char* tmp = list;
  int f = 0;
  while (*tmp) {
    if (((int)*tmp < 48 || (int)*tmp > 58) && (int)*tmp != 32) {
      f = 1;
    }
    *tmp++;
  }

  while (*list != '\0' && res != 0 && flag != 0 && f != 1) {
    res = sscanf(list, "%d:%lf%n", date, sum, &n);
    if (n != 0) {
      if (*date == current_mounth && res == 2) {
        break;
      } else if (res < 2) {
        *sum = 0;

        break;
      }
      if (res < 2) {
        sum = 0;
        break;
      }
      list += n;
    } else {
      sum = 0;
      *flag = 1;
      break;
    }
  }
  return res;
}

int depositCalc(double deposit_amount, double term, double rate,
                int payments_period, double* percents, double* amount,
                char* replenishment_list, char* withdrawals_list,
                int is_capitalization) {
  double get_term_days = 0.0;
  int current_mounth = 1, flag = 0, *date = malloc(sizeof(int)), res = OK,
      *flag_for_lists = malloc(sizeof(int));
  double new_period_of_pay = whatTypeOfPeriod(payments_period);
  double new_term = 0.0;
  int current_year = 2022;
  double* sum = malloc(sizeof(double));
  double tmp_term = term, one_time_percents = 0.0;
  int tmp_flag_m = 0;
  if (term > 12) {
    flag = 1;
  }
  for (int i = 1; i <= term / new_period_of_pay; i++) {
    for (int m = 1; m <= (int)new_period_of_pay; m++) {
      if (current_mounth >= 13) {
        tmp_flag_m++;
        current_mounth = 1;
        current_year += 1;
      }
      int new_counter = tmp_flag_m * 12 + current_mounth;
      if (tmp_term > 0 && *flag_for_lists != 1) {
        get_term_days = getDaysInMounth(current_mounth);
        new_term += get_term_days;
        if (strlen(replenishment_list) > 1) {
          parserDatesAndAmounts(replenishment_list, date, sum, new_counter,
                                flag_for_lists);
          if (*date == new_counter && *flag_for_lists != 1) {
            deposit_amount += *sum;
          }
        }
        if (strlen(withdrawals_list) > 1) {
          parserDatesAndAmounts(withdrawals_list, date, sum, new_counter,
                                flag_for_lists);
          if (*date == new_counter && *sum < deposit_amount &&
              *flag_for_lists != 1) {
            deposit_amount -= *sum;
          }
        }
        current_mounth++;
      }
      tmp_term--;
    }
    if (flag == 1) {
      i -= 1;
      flag = 0;
    }
    one_time_percents =
        deposit_amount * rate / 100 * (new_term / getYearDays(current_year));
    *percents += one_time_percents;
    new_term = 0.0;
    if (is_capitalization == 1) {
      deposit_amount += one_time_percents;
    }
  }
  *amount = deposit_amount;
  free(sum);
  free(date);
  free(flag_for_lists);
  return res;
}

int takeDepositInfo(double deposit_amount, double term, double rate, double tax,
                    int payments_periodicity, int capitalization,
                    char* replenishment_list, char* withdrawals_list,
                    double* percents, double* tax_sum, double* amount) {
  int res = ERROR;
  *percents = 0;
  *tax_sum = 0;
  *amount = 0;
  if (deposit_amount > 0 && term > 0 && rate > 0 && rate <= 100 &&
      payments_periodicity > 0 &&
      (capitalization == 0 || capitalization == 1)) {
    res = depositCalc(deposit_amount, term, rate, payments_periodicity,
                      percents, amount, replenishment_list, withdrawals_list,
                      capitalization);
    if (res == OK) {
      taxCalc(percents, tax_sum, tax, term);
    }
  }
  return res;
}
