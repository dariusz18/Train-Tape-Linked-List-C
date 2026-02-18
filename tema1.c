//ZAHARESCU DARIUS CRISTIAN - GRUPA 312
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char T;

typedef struct node {
  T value;
  struct node *prev;
  struct node *next;
} TrainNode;

typedef struct Train {
  TrainNode *sentinel;
  TrainNode *mecanic;
} Train;

TrainNode *createNode(T value) {
  TrainNode *node = (TrainNode *)malloc(sizeof(TrainNode));
  if (node == NULL)
    exit(1);
  node->value = value;
  node->prev = NULL;
  node->next = NULL;
  return node;
}

Train *initTrain() {
  Train *train = (Train *)malloc(sizeof(Train));
  if (train == NULL)
    exit(1);

  train->sentinel = createNode('\0');
  train->sentinel->prev = train->sentinel;
  train->sentinel->next = train->sentinel;

  TrainNode *first = createNode('#');
  first->next = train->sentinel;
  first->prev = train->sentinel;
  train->sentinel->next = first;

  train->mecanic = first;

  return train;
}

void addVagon(Train *train, T value) {
  if (train == NULL)
    return;
  TrainNode *newVagon = createNode(value);
  if (newVagon == NULL)
    return;

  TrainNode *iter = train->sentinel;
  while (iter->next != train->sentinel)
    iter = iter->next;

  iter->next = newVagon;
  newVagon->prev = iter;
  newVagon->next = train->sentinel;
  train->sentinel->prev = newVagon;
}

void move(Train *train, char *comanda) {
  if (strcmp(comanda, "MOVE_LEFT") == 0) {
    if (train->mecanic->prev != train->sentinel)
      train->mecanic = train->mecanic->prev;
    else{
      train->mecanic = train->mecanic->prev->prev;
      if(train->mecanic == train->sentinel)
       train->mecanic = train->mecanic->next;
      }
  } else if (strcmp(comanda, "MOVE_RIGHT") == 0) {
    if (train->mecanic->next != train->sentinel)
      train->mecanic = train->mecanic->next;
    else {
      addVagon(train, '#');
      train->mecanic = train->mecanic->next;
    }
  }
}

void inscription(Train *train, T value) {
  if (train == NULL)
    return;
  else train->mecanic->value = value;
   
}

void decuplare(Train *train) {
  if (train == NULL || train->sentinel->next == train->sentinel)
    return;

  TrainNode *iter = train->sentinel->next;

  while (iter->next != train->sentinel) {
    if (iter->prev == train->sentinel && iter == train->mecanic) {
      train->sentinel->next = iter->next;
      iter->next->prev = train->sentinel;
      free(iter);
      train->mecanic = train->sentinel->prev;
      return;
    } else if (iter->next != train->sentinel && iter == train->mecanic) {
      iter->prev->next = iter->next;
      iter->next->prev = iter->prev;
      TrainNode *tmp = iter;
      iter = iter->next;
      free(tmp);
      train->mecanic = iter->prev;
      return;
    }
    iter = iter->next;
  }

  if (iter->next == train->sentinel && iter == train->mecanic) {
    TrainNode *tmp = iter;
    train->sentinel->prev = iter->prev;
    iter = iter->prev;
    iter->next = train->sentinel;
    free(tmp);
    train->mecanic = iter;
    return;
  }
}

void clear(Train *train) {
    if (train == NULL || train->sentinel == NULL || train->sentinel->next == train->sentinel) {
        return;
    }

    TrainNode *current = train->sentinel->next->next;
    while (current != train->sentinel) {
        TrainNode *temp = current;
        current = current->next;
        free(temp);
    }

    train->mecanic = train->sentinel->next;
    train->mecanic->value = '#';
    train->mecanic->next = train->sentinel;
    train->mecanic->prev = train->sentinel;
    train->sentinel->next = train->mecanic;
    train->sentinel->prev = train->mecanic;
}

void cuplare(Train *train, char *comanda, T value, FILE *output) {
  if (train == NULL)
    return;
  TrainNode *iter;
  iter = train->sentinel->next;
  while (iter != train->sentinel) {
    if (strcmp(comanda, "INSERT_LEFT") == 0 && iter == train->mecanic) {

      if (train->mecanic == train->sentinel->next) {
        fprintf(output, "ERROR\n");
        return;
      }

      TrainNode *newVagon = createNode(value);
      newVagon->next = iter;
      newVagon->prev = iter->prev;
      iter->prev->next = newVagon;
      iter->prev = newVagon;
      train->mecanic = newVagon;
      return;
    }

    else if (strcmp(comanda, "INSERT_RIGHT") == 0 && iter == train->mecanic) {
      TrainNode *newVagon = createNode(value);
      newVagon->next = iter->next;
      newVagon->prev = iter;
      iter->next = newVagon;
      if (newVagon->next != train->sentinel)
        newVagon->next->prev = newVagon;
      else
        train->sentinel->prev = newVagon;
      train->mecanic = newVagon;
      return;
    }
    iter = iter->next;
  }
}

int lungime(Train *train) {
  int c = 0;
  TrainNode *iter = train->sentinel->next;
  while (iter != train->sentinel) {
    c++;
    iter = iter->next;
  }
  return c;
}

void search(Train *train, char *sir, FILE *output) {
  TrainNode *iter = train->mecanic;
  TrainNode *pozitie_initiala = train->mecanic;
  int i = 0, contor = 0, ok = 0, cifre_sir = strlen(sir);
  do {
    if (iter == train->sentinel)
      iter = iter->next;

    if (sir[i] == iter->value) {
      i++;
      contor++;
    }

    else if (sir[i] != iter->value) {
      if (i > 0) {
        i = 0;
        contor = 0;
        train->mecanic = train->mecanic->next;
        if (train->mecanic == train->sentinel)
          train->mecanic = train->mecanic->next;

        while (train->mecanic != iter) {
          if (sir[i] == train->mecanic->value) {
            i++;
            contor++;
          } else {
            i = 0;
            contor = 0;
          }
          train->mecanic = train->mecanic->next;
        }

        if (iter->value == sir[i]) {
          i++;
          contor++;
        } else {
          i = 0;
          contor = 0;
        }
      }

      else {
        i = 0;
        contor = 0;
      }
    }

    if (contor == cifre_sir) {
      ok = 1;
      train->mecanic = iter;
      break;
    }

    iter = iter->next;

  } while (iter != pozitie_initiala && ok == 0);

  if (ok == 1)
    for (i = 1; i < contor; i++) {
      train->mecanic = train->mecanic->prev;
      if (train->mecanic == train->sentinel)
        train->mecanic = train->mecanic->prev;
    }

  else if (ok == 0) {
    train->mecanic = pozitie_initiala;
    fprintf(output, "ERROR\n");
  }

}

void search_directie(Train *train, char *sir, char *directie, FILE *output) {
  char *s;
  int ok = 0;
  int i = 0, cnt = 0;
  int l = lungime(train);
  TrainNode *iter = train->mecanic;
  if (strcmp(directie, "SEARCH_RIGHT") == 0) {
    s = malloc(l * sizeof(char));
    s[0] = '\0';
    while (iter != train->sentinel) {
      char temp[2] = {iter->value, '\0'};
      strcat(s, temp);
      iter = iter->next;
    }
    int cifre = strlen(s), cifre_sir = strlen(sir);
    for (i = 0; i < cifre && ok == 0; i++) {
      if (strncmp(s + i, sir, strlen(sir)) == 0) {
        ok = 1;
        cnt++;
      } else
        cnt++;
    }
    if (ok == 1) {
      for (i = 1; i <= cnt + cifre_sir - 2; i++)
        train->mecanic = train->mecanic->next;
    }
  }

  else if (strcmp(directie, "SEARCH_LEFT") == 0) {
    s = malloc(l * sizeof(char));
    s[0] = '\0';
    while (iter != train->sentinel) {
      char temp[2] = {iter->value, '\0'};
      strcat(s, temp);
      iter = iter->prev;
    }
    printf("%s ", s);
    int cifre = strlen(s), cifre_sir = strlen(sir);
    for (i = 0; i < cifre && ok == 0; i++) {
      if (strncmp(s + i, sir, strlen(sir)) == 0) {
        ok = 1;
      } else
        cnt++;
    }
    if (ok == 1) {
      for (i = 1; i <= cnt + cifre_sir - 1; i++)
        train->mecanic = train->mecanic->prev;
    }
  }

  if (ok == 0)
    fprintf(output, "ERROR\n");
  free(s);
}

void showCurrent(Train *train, FILE *output) {
  if (train == NULL || output == NULL)
    return;
  else
    fprintf(output, "%c\n", train->mecanic->value);
}

void printTrain(Train *train, FILE *output) {
  if (train == NULL || train->sentinel == NULL || output == NULL)
    return;

  TrainNode *current = train->sentinel->next;

  do {
    if (current == train->mecanic)
      fprintf(output, "|%c|", current->value);
    else
      fprintf(output, "%c", current->value);
    current = current->next;
  } while (current != train->sentinel);
  fprintf(output, "\n");
}

typedef struct nodecomanda {
  char *value;
  struct nodecomanda *prev;
  struct nodecomanda *next;
} Node;

typedef struct queue {
  Node *head;
  Node *tail;
  int size;
} * TQueue;

Node *initNode(char *value) {
  Node *node = malloc(sizeof(struct nodecomanda));
  node->value = malloc(strlen(value) + 1);
  strcpy(node->value, value);
  node->prev = NULL;
  node->next = NULL;
  return node;
}

TQueue createQueue(void) {
  TQueue queue = (TQueue)malloc(sizeof(struct queue));
  if (queue == NULL)
    return NULL;
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  return queue;
}

Node *freeNode(Node *node) {
  if (node) {
    free(node->value);
    free(node);
  }
  return NULL;
}

int isEmpty(TQueue queue) {
  if (queue == NULL || (queue->head == NULL && queue->size == 0))
    return 1;
  else
    return 0;
}

TQueue enqueue(TQueue queue, char *value) {
  if (queue == NULL) {
    return NULL;
  }

  Node *node = initNode(value);
  if (node == NULL)
    return NULL;

  if (isEmpty(queue)) {
    queue->head = node;
    queue->tail = node;
  } else {
    queue->tail->next = node;
    node->prev = queue->tail;
    queue->tail = node;
  }

  queue->size++;
  return queue;
}

TQueue dequeue(TQueue queue) {

  if (isEmpty(queue))
    return queue;

  Node *tmp = queue->head;

  if (queue->head == queue->tail) {
    queue->head = NULL;
    queue->tail = NULL;
  } else {
    queue->head = queue->head->next;
    if (queue->head != NULL)
      queue->head->prev = NULL;
  }
  freeNode(tmp);

  queue->size--;

  return queue;
}

void switch_q(TQueue q) {
  Node *current = q->head;
  Node *prev = NULL;
  Node *next = NULL;

  while (current != NULL) {
    next = current->next;
    current->next = prev;
    current->prev = next;
    prev = current;
    current = next;
  }
  Node *temp = q->head;
  q->head = q->tail;
  q->tail = temp;
}

void executacomanda(Train *train, char *comanda, FILE *output) {
  char *first = strtok(comanda, " ");
  char *second = strtok(NULL, " ");
  if (first == NULL)
    return;
  if (strcmp(first, "MOVE_RIGHT") == 0)
    move(train, "MOVE_RIGHT");
  else if (strcmp(first, "MOVE_LEFT") == 0)
    move(train, "MOVE_LEFT");
  else if (strcmp(first, "WRITE") == 0) {
    if (second != NULL)
      inscription(train, second[0]);
  } else if (strcmp(first, "CLEAR_CELL") == 0)
    decuplare(train);
  else if (strcmp(first, "CLEAR_ALL") == 0)
    clear(train);

  else if (strcmp(first, "INSERT_RIGHT") == 0) {
    if (second != NULL)
      cuplare(train, "INSERT_RIGHT", second[0], output);
  }

  else if (strcmp(first, "INSERT_LEFT") == 0) {
    if (second != NULL)
      cuplare(train, "INSERT_LEFT", second[0], output);
  }

  else if (strcmp(first, "SEARCH") == 0) {
    if (second != NULL) {
      int i = 0;
      while (second[i] != 10) // cod ascii pt enter
        i++;
      second[i] = '\0';
    }
    search(train, second, output);
  }

  else if (strcmp(first, "SEARCH_LEFT") == 0) {
    if (second != NULL) {
      int i = 0;
      while (second[i] != 10)
        i++;
      second[i] = '\0';
    }

    search_directie(train, second, "SEARCH_LEFT", output);
  }

  else if (strcmp(first, "SEARCH_RIGHT") == 0) {
    if (second != NULL) {
      int i = 0;
      while (second[i] != 10)
        i++;
      second[i] = '\0';
    }
    search_directie(train, second, "SEARCH_RIGHT", output);
  }
}

void freeTrain(Train *train) {
  if (train == NULL)
    return;
  TrainNode *current = train->sentinel->next;
  while (current != train->sentinel) {
    TrainNode *temp = current;
    current = current->next;
    free(temp);
  }
  free(train->sentinel);
  free(train);
}

void freeQueue(TQueue queue) {
  while (!isEmpty(queue)) {
    dequeue(queue);
  }
  free(queue);
}

int main() {
  TQueue queue = createQueue();
  Train *train = initTrain();
  FILE *input;
  FILE *output;
  char s[200];
  char s2[200];
  int n, i;
  input = fopen("tema1.in", "r");
  if (input == NULL)
    exit(1);
  output = fopen("tema1.out", "w");
  if (output == NULL)
    exit(1);

  fscanf(input, "%d ", &n);
  for (i = 0; i < n; i++) {
    fgets(s, sizeof(s), input);

    if (strncmp(s, "MOVE_RIGHT", 10) == 0) {
      strncpy(s2, s, 10);
      s2[10] = '\0';
      enqueue(queue, s2);
    }

    else if (strncmp(s, "MOVE_LEFT", 9) == 0) {
      strncpy(s2, s, 9);
      s2[9] = '\0';
      enqueue(queue, s2);
    }

    else if (strncmp(s, "WRITE", 5) == 0) {
      strncpy(s2, s, 7);
      s2[7] = '\0';
      enqueue(queue, s2);
    }

    else if (strncmp(s, "CLEAR_CELL", 10) == 0) {
      strncpy(s2, s, 10);
      s2[10] = '\0';
      enqueue(queue, s2);
    }

    else if (strncmp(s, "CLEAR_ALL", 9) == 0) {
      strncpy(s2, s, 9);
      s2[9] = '\0';
      enqueue(queue, s2);
    }

    else if (strncmp(s, "INSERT_LEFT", 11) == 0) {
      strncpy(s2, s, 13);
      s2[13] = '\0';
      enqueue(queue, s2);
    }

    else if (strncmp(s, "INSERT_RIGHT", 12) == 0) {
      strncpy(s2, s, 14);
      s2[14] = '\0';
      enqueue(queue, s2);
    } else if (strncmp(s, "SEARCH ", 7) == 0) {
      enqueue(queue, s);
    }

    else if (strncmp(s, "SEARCH_LEFT", 11) == 0) {
      enqueue(queue, s);
    }

    else if (strncmp(s, "SEARCH_RIGHT", 12) == 0) {
      enqueue(queue, s);
    }

    else if (strncmp(s, "EXECUTE", 7) == 0) {
      if(!isEmpty(queue)){
      strcpy(s2, queue->head->value);
      dequeue(queue);
      executacomanda(train, s2, output);}
    } else if (strncmp(s, "SHOW_CURRENT", 12) == 0)
      showCurrent(train, output);
    else if (strncmp(s, "SHOW", 4) == 0)
      printTrain(train, output);
    else if (strncmp(s, "SWITCH", 6) == 0)
      switch_q(queue);
  }
  fclose(input);
  fclose(output);
  freeTrain(train);
  freeQueue(queue);
  return 0;
}
