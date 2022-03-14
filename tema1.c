#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// IMPLEMENTARE COADA
typedef struct qcell { 
   char *command;
   struct qcell *next;
} QueueCell, *PQueue;

typedef struct Queue {
   PQueue front, rear;
} TQueue;

TQueue init_queue() {  //initializeaza coada
   TQueue q; 
   q.front = q.rear = NULL; 
   return q; 
}

TQueue enqueue(TQueue *q, char *comanda) {  //pune comanda primita in coada
   PQueue p; 
   p = (QueueCell*) malloc(sizeof(QueueCell)); 
   p->command = (char*) malloc((strlen(comanda)+1)*sizeof(char));
   if (p == NULL) {
      printf("Eroare\n"); 
      exit(1); 
   } 
   strcpy(p->command, comanda); 
   p->next = NULL; 
   if (q->front == NULL) 
      q->front = q->rear = p; 
   else { 
      q->rear->next = p; 
      q->rear = p; 
   } 
   return *q;
} 

TQueue dequeue(TQueue q) {  //scoate primul nod din coada
   PQueue p; 
   if (q.front == NULL) { 
      printf("coada vida\n"); 
      exit(1); 
   } 
   if (q.front == q.rear) { 
      free(q.front->command); 
      free(q.front); 
      q.front = q.rear = NULL;
   } 
   else { 
      p = q.front; 
      q.front = q.front->next; 
      free(p->command); 
      free(p); 
   }  
   return q;
}

// IMPLEMENTARE STIVA
typedef struct scell {  
   char *command;
   struct scell *next;
} Stack, *PStack;

Stack* push_stack(Stack **s, char *comanda) {      //pune comanda primita in stiva 
    Stack *nou = (Stack*) malloc(sizeof(Stack));
    nou->command = (char*)malloc((strlen(comanda)+1)*sizeof(char));
    if (nou == NULL)
      return NULL;

    strcpy(nou->command, comanda);
    nou->next = NULL;
    if ((*s) == NULL) {
      *s = nou;
      nou->next = NULL;
    }else {
        nou->next = *s;
        *s = nou;
    }
    return *s;
}

Stack* pop_stack(Stack **s) {    //scoate comanda din varful stivei
    Stack *aux;
    aux = (*s);
    if ((*s) == NULL) {
       perror((char*)"Stiva vida");
       return 0;
    }
    *s = aux->next;
    free(aux->command);
    free(aux);
    return *s;
}

//LISTA DUBLU INLANTUITA
typedef struct node {
 char value ;
 struct node *prev ;
 struct node *next ;
} node_t;

node_t *alloc_node(char value){              // creaza un nod nou
    node_t *node = (node_t*)malloc(sizeof(node_t));
    node->value = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void push_after ( node_t **current , node_t * new_node ){  //pune un nod nou dupa un anumit nod dat la listei
    if((*current) == NULL){
        *current = new_node;
    }else{
     new_node->next = (*current)->next;
     (*current)->next->prev = new_node;
     (*current)->next=new_node;
     new_node->prev= *current;
    }
}

void push_back ( node_t **head , node_t *new_node ){   //pune un nod nou la sfarsitul listei
   if((*head) == NULL){
     (*head) = new_node;
     new_node->next = NULL;
     new_node->prev = NULL;
   }else{
    node_t *n = (*head);
    while( n->next!=NULL)
        n=n->next;

    n->next=new_node;
    new_node->prev=n;
    new_node->next = NULL;
   }
}

void remove_node ( node_t ** node, node_t **head ){  //elimina un nod dat din lista
    if((*node)->prev == NULL){
       (*node)->next->prev=NULL;
       *head=(*node)->next;
       free(*node);
       return;
    }   
    if((*node)->next == NULL){
       (*node)->prev->next=NULL;
       free(*node);
       return;
    }
    (*node)->next->prev=(*node)->prev;
    (*node)->prev->next=(*node)->next;
    free(*node);
}

void save(node_t *head)         //salveaza lista in fisierul "editor.out"
{
    FILE *out = fopen("editor.out", "w");
    while(head != NULL){
        fputc(head->value, out);
        head=head->next;
    }
    fclose(out);    
}

void delete_line(node_t **head, char *s, node_t **curr, node_t **v, int *k, int *curr_line){  //DELETE LINE sterge o linie data

   char *l = strtok(s+3, "\n");
   int line;   //linia de sters
    if( l != NULL)  //daca este data
      line = atoi(l);
   else
      line = *curr_line;  //este linia curenta

   *curr_line = line-1;
   *curr = v[line-1];

   node_t *p = v[line];
   while( p->value != '\n'){ //cat timp nu este sfarsitul liniei
      node_t *aux = p->next;
      remove_node(&p, head);
      p = aux;
   }
   remove_node(&p, head);    //se sterge si sfarsitul de linie
   int i;
   for( i=line; i<(*k); i++)
      v[i]=v[i+1];           //linia stearsa este eliminata din vectorul de pointeri la linii
   (*k)--;
    *curr_line = line;
    *curr = v[line];
}

int goto_line( char *s, node_t **curr, node_t **v,  int *curr_line){   //GOTO LINE muta cursorul la inceputul unei linii date
    int line = atoi(strtok(s+3, "\n")); //linia data
    *curr = v[line];
    *curr_line = line;
}

int goto_character(char *s, node_t **curr, node_t **v,  int *curr_line, int *k){  //GOTO CHARACTER muta cursorul la un caracter dat
      int poz = atoi(strtok(s+3, " "));  //pozitia caracterului in linie
      char *l = strtok(NULL, "\n");     //linia
      int line;
      if( l != NULL)  //daca linis=a este data
        line = atoi(l);
      else
         line = *curr_line; //este linia curenta

      node_t *nod = v[line];     // se muta cursorul la inceputul liniei
      int i;
      for( i=1; i<poz; i++) //se muta cursorul la pozitia caracterului
           nod = nod->next;
      (*curr) = nod;
      (*curr_line) = line;
}

char* delete_character(node_t **head, char *s, node_t **curr, node_t **v,  int *curr_line){ //DELETE CHARACTER(S) sterge un numar de caractere
      char *n = strtok(s+2, "\n");  //numarul de caractere de sters
      int nr;
      if( n != NULL) //daca este dat
        nr = atoi(n);
      else
        nr = 1;   //trebuie sters un singur caracter
     char *del = (char*)calloc((nr+2),sizeof(char));  //retine caracterele ce vor fi sterse
     *curr = (*curr)->next;
     int i;
     for(i=0; i<nr; i++){
         node_t *aux = (*curr)->next;
         del[i] = (*curr)->value;
         if( *curr == v[(*curr_line)])  //daca caracterul de sters este chiar inceputul de linie
            v[(*curr_line)] = (*curr)->next; //inceputul de linie devine urmatorul caracter
         remove_node(curr, head);
         *curr = aux;
      }
      *curr = (*curr)->prev;
      del[nr+1] = '\0';
     return del;
}
         
void insert(node_t **head, char *s, node_t **curr, node_t **v, int *k,  int *curr_line){  //INSERT TEXT insereaza text
   int i;
   for( i=0; i<strlen(s); i++){ 
      node_t *n = alloc_node(s[i]);
      if( (*curr) == NULL || (*curr)->next == NULL) //daca lista este goala sau se insereaza la sfarsitul ei
         push_back( head, n);    //se insereaza in continuare
      else{
         if( n->value != '\n')     //daca caracterul de inserat este sfarsit de linie
            push_after( curr, n);  //se insereaza dupa caracterul curent
         else
             free(n);  //se elibereaza
        }

      *curr=n;
       if( i == 0 && (*curr)->next == NULL){  //daca a fost adaugata o noua linie
         v[++(*k)] = n;  //se adaugata o noua linie si in vectorul de pointeri la linii
         *curr_line = *k;
       }
    }
   if( *curr_line != *k)  //daca linia curenta nu este ultima linie adaugata
    (*curr_line) = *k;
}

void replace_word(node_t **head, char *s, node_t **curr, node_t **v, int *k, int *curr_line){   //REPLACE WORD inlocuieste un cuvant cu un altul
        char *old, *new;
        old = strtok(s+3, " ");  //cuvantul vechi
        new = strtok(NULL, "\n"); //cuvantul nou
        node_t *nod = *curr;
        node_t *p = NULL;   //pointer la pozitia la care incepe cuvantul in lista
        int i=0;
        while( nod->next != NULL){  
              if(nod->value == old[i++]){  //se cauta pozitia de la care caracterele din lista sunt aceleasi cu cele din cuvant 
                  if(i == 1)              // pe o distanta egala cu lungimea cuvantului vechi
                     p = nod;
                  if(i == strlen(old))  //daca s-a ajuns la lungimea cuvantului vechi, a fost gasit
                    break;
               }else
                   i=0;
              nod = nod->next; 
        }
           int m, j;
           if(p != NULL){
                if(strlen(old) <= strlen(new)){ //daca cuv nou este mai mare
                    for(j=0; j<i; j++){           //se inlocuieste pana la sfarsitul celui vechi
                        p->value = new[j];
                        p = p->next;
                    }
                   *curr = p->prev;
                    for(m=j; m<strlen(new); m++){ //se mai adauga noduri in lista pana este adaugat tot cuvantul nou
                        node_t *n;
                        n = alloc_node(new[m]);
                        push_after(curr, n);
                        (*curr) = n;
                    }
               }else{                       //daca cuv cuvantul nou este mai mic
                    for(j=0; j<strlen(new); j++){   //se inlocuieste pana la sfarsitul acestuia
                        p->value = new[j];
                        p = p->next;
                    }
                    *curr = p->prev;
                    for(m=j; m<strlen(old); m++){  //nodurile ramase din vechi sunt sterse
                    node_t *aux = p->next;
                        if( p == v[(*curr_line)])
                            v[(*curr_line)] = p->next;
                        remove_node(&p, head);
                        p = aux;
                    }
                    *curr = p;
                 }   
           }
}

void replace_all (node_t **head, char *s, node_t **curr, node_t **v, int *k, int *curr_line){      //REPLACE ALL inlocuieste toate aparitiile unui cuvant cu un altul
      char *old, *new;
       old = strtok(s+3, " ");  //cuvantul vechi
       new = strtok(NULL, "\n"); //cuvantul nou
       node_t *nod = v[1];
       while( nod != NULL){
           node_t *p = NULL; //pointer la pozitia la care incepe cuvantul in lista
           int i=0;
           while( nod->next != NULL) //cat timp nu s-a ajuns la sfarsitul listei
           {  
              if(nod->value == old[i++]){ //se cauta pozitia de la care caracterele din lista sunt aceleasi cu cele din cuvant 
                  if(i == 1)
                     p = nod;
                  if(i == strlen(old)) //daca s-a ajuns la lungimea cuvantului vechi, a fost gasit
                    break;
               }else
               {
                   i=0;
                   p = NULL;
               }
              nod = nod->next; 
           }
           *curr = p;
           int m, j;
           if(p != NULL && i != 0){
                if(strlen(old) <= strlen(new)){ //daca cuv nou este mai mare
                    for(j=0; j<i; j++){           //se inlocuieste pana la sfarsitul celui vechi
                        p->value = new[j];
                        p = p->next;
                    }
                   *curr = p->prev;
                    for(m=j; m<strlen(new); m++){ //se mai adauga noduri in lista pana este adaugat tot cuvantul nou
                        node_t *n;
                        n = alloc_node(new[m]);
                        push_after(curr, n);
                        (*curr) = n;
                    }
               }else{                       //daca cuv cuvantul nou este mai mic
                    for(j=0; j<strlen(new); j++){   //se inlocuieste pana la sfarsitul acestuia
                        p->value = new[j];
                        p = p->next;
                    }
                    *curr = p->prev;
                    for(m=j; m<strlen(old); m++){  //nodurile ramase din vechi sunt sterse
                    node_t *aux = p->next;
                        if( p == v[(*curr_line)])
                            v[(*curr_line)] = p->next;
                        remove_node(&p, head);
                        p = aux;
                    }
                    *curr = p;
                 } 
           }
           nod = (*curr);
       }  
}

void delete_word(node_t **head, char *s, node_t **curr, node_t **v, int *k, int *curr_line){  //DELETE WORD sterge prima aparitie a unui cuvant dat de dupa curosor
           char *w = strtok(s+3, "\n");  //cuvantul care trebuie sters
           node_t *nod = *curr;
           node_t *p = NULL;  //pointer la pozitia la care incepe cuvantul in lista
           int i=0;
           while( nod->next != NULL)
           { 
              if(nod->value == w[i++]){   //se cauta pozitia de la care caracterele din lista sunt aceleasi cu cele din cuvant 
                  if(i == 1)             // pe o distanta egala cu lungimea cuvantului
                     p = nod;
                  if(i == strlen(w))   //daca s-a ajuns la lungimea cuvantului de sters, a fost gasit
                    break;
               }else
               { i=0;
                 if(nod-> value == '\n')  //daca se intalneste sfarsitul de linie, se trece la linia urmatoare
                    (*curr_line)++;
               }
              nod = nod->next; 
           }
           if(i!=0 ){
               int j;
             for(j=0; j<i; j++){
                node_t *aux = p->next;
                if( p == v[(*curr_line)])   //daca un caracter este chiar inceputul unei linii
                   v[(*curr_line)] = p->next;
                remove_node(&p, head);
                p = aux;
             }
            *curr = p;
           }
}

void delete_all(node_t **head, char *s, node_t **curr, node_t **v, int *k, int *curr_line){   //DELETE ALL sterge toate aparitiile unui cuvant dat
        char *w = strtok(s+3, "\n"); //cuvantul care trebuie sters
        if(v[1] != NULL){
          node_t *nod = v[1];
        *curr_line = 1;
        while(nod != NULL){  
           node_t *p = NULL; //pointer la pozitia la care incepe cuvantul in lista
           int i=0;
           while(  nod != NULL )
           {  
              if(nod->value == w[i++]){    //se cauta pozitia de la care caracterele din lista sunt aceleasi cu cele din cuvant 
                  if(i == 1)              // pe o distanta egala cu lungimea cuvantului
                     p = nod; 
                  if(i == strlen(w))    //daca se intalneste sfarsitul de linie, se trece la linia urmatoare
                    break;
               }else
               { i=0;
                 p = NULL;
                 if(nod-> value == '\n') //daca se intalneste sfarsitul de linie, se trece la linia urmatoare
                    (*curr_line)++;
               }
              nod = nod->next; 
           }
           if(i!=0 ){
             int j;
             for( j=0; j<i; j++){
                node_t *aux = p->next;
                if( p == v[(*curr_line)])    //daca un caracter este chiar inceputul unei linii
                   v[(*curr_line)] = p->next;
                remove_node(&p, head);
                p = aux;
             }
              if(v[*curr_line]->value == '\n' && v[*curr_line] != NULL){ //daca primul nod este sfarsit de linie, acesta este sters, linia curenta fiind stearsa
                remove_node(&v[*curr_line], head);
                int i;
                for( i=(*curr_line); i<(*k); i++)     //linia stearsa este eliminata din vectorul de pointeri la linii
                     v[i]=v[i+1];
                (*k)--;
                *curr = v[*curr_line];
              }
              *curr = p;
            }
            nod = p;
         } 
    }
}

int main()
{
   node_t *head = NULL;  //se initializeaza lista
   FILE *in = fopen("editor.in", "r");  //deschide fisierul "editor.in" din care vor fi citite textul si comenzile
    if(in == NULL){
      printf("Error opening file"); 
    return(1); 
    }

  Stack *stiva_undo = NULL; //se initializeaza stiva
  TQueue coada_redo;
  coada_redo = init_queue(); //se initializeaza coada

 node_t **v;     //vector de pointeri noduri in care va fi retinut primul nod de pe fiecare linie
 v=(node_t**)calloc(100,sizeof(node_t*));
 int k=0;        //numarul total de linii de text inserate
 int ok=0;      //numarul de intrari si iesiri din modul de inserare text
 char s[100];  //sirul de caractere in care va fi retinuta fiecare noua linie citita din fisier
 node_t *curr = NULL;  //nodul curent
 int curr_line=0;     //linia curenta

 while( fgets(s, 100, in) !=NULL || strcmp(s, "q\n") != 0){  

    if(strcmp(s, "::i\n") == 0 || strcmp(s, "::i \n") == 0){       
        ok++;
        if(ok % 2 == 0)    //daca am intrat in modul de inserare text  //EDIT MODE
            push_stack(&stiva_undo, s);
        fgets(s, 100, in);   //se citeste textul de inserat
    }

    if(ok % 2 != 0 && strcmp(s, "::i\n") != 0){  //daca urmeaza sa fie citite comenzi
        char aux[100];
        strcpy(aux, s);
        if(strcmp(s, "q\n") == 0){
            break;
        }
        if( strcmp(s, "s\n") == 0){            //daca comanda este SAVE
            if(head != NULL)
                save(head);
            while(stiva_undo != NULL)
                pop_stack(&stiva_undo);
            while (coada_redo.front != NULL)
                coada_redo = dequeue(coada_redo);
         }
   
        if( strcmp(s, "b\n") == 0){           //daca comanda este BACKSPACE
           push_stack(&stiva_undo, s);
           if(curr->prev != NULL && curr != NULL){
                if(curr->value == '\n'){
                    node_t *n = curr->prev;
                    remove_node(&n, &head);
                } else {
                    node_t *n = curr->prev;
                    remove_node(&curr, &head);
                    curr = n;
                }
            }
        }
        
        if( strcmp(strtok(aux, " "), "dl") == 0 || strcmp(strtok(aux, "\n"), "dl") == 0 ){ // daca comanda este DELETE LINE
            push_stack(&stiva_undo, s);
            delete_line(&head, s, &curr, v, &k, &curr_line);
        }
          
        if( strcmp(strtok(aux, " "), "gl") == 0 ){     // daca comanda este GOTO LINE
           int curr_l = curr_line;
           goto_line(s, &curr, v, &curr_line);
           char aux[10];
           sprintf(aux, "%d", curr_l);
           strcat(s, " ");
           strcat(s, aux);
           strcat(s, "\n");
           push_stack(&stiva_undo, s);
        }
          
        if( strcmp(strtok(aux, " "), "gc") == 0 ){     // daca comanda este GOTO CHARACTER
            int curr_l = curr_line;
            node_t *aux = v[curr_l];
            int poz = 1;
            while(aux != curr && aux != NULL){ //cauta pozitia caracterului curent in linia curenta
              poz++;
              aux = aux->next;
            }
            char sir[100];
            strcpy(sir, s);
            goto_character(s, &curr, v, &curr_line, &k);
            char str1[10]; sprintf(str1, "%d", poz);
            strcat(sir, str1);
            strcat(sir, " ");
            char str2[10]; sprintf(str2, "%d", curr_l);
            strcat(sir, str2);
            strcat(sir, "\n");
            push_stack(&stiva_undo, sir);
        }

        if( strcmp(strtok(aux, " "), "d") == 0 || strcmp(strtok(aux, "\n"), "d") == 0 ){        // daca comanda este DELETE CHARACTER(S)
            char sir[100];
            strcpy(sir, "d ");
            char *del = delete_character(&head, s, &curr, v, &curr_line);
            strcat( sir, del);
            push_stack(&stiva_undo, sir);
            free(del);
        }
          
        if(  strcmp(strtok(aux, " "), "re") == 0 ){      // daca comanda este REPLACE WORD
            char sir[100], saux[100];
            strcpy(sir, s);
            strcpy(saux, s);
            replace_word(&head, s, &curr, v, &k, &curr_line);
            char *p = strtok(saux, " ");
            char *p1 = strtok(saux+strlen(p)+1, " ");
            char *p2 = strtok(saux+strlen(p)+1+strlen(p1)+1, "\n");
            node_t *aux = v[curr_line];
            int poz = 1;
            while(aux != curr &&  aux != NULL){  //cauta pozitia caracterului curent in linia curenta
              poz++;
              aux = aux->next;
            }
            poz = poz - strlen(p2);
            char str1[10]; sprintf(str1, "%d", poz);
            strcat(sir, str1);
            strcat(sir, " ");
            char str2[10]; sprintf(str2, "%d", curr_line);
            strcat(sir, str2);
            strcat(sir, "\n");
            push_stack(&stiva_undo, sir); 
        }
          
        if( strcmp(strtok(aux, " "), "ra") == 0 ){      // daca comanda este REPLACE ALL
           push_stack(&stiva_undo, s);
           replace_all(&head, s, &curr, v, &k, &curr_line);
        }
           
        if( strcmp(strtok(aux, " "), "dw") == 0 ){            //daca comanda este DELETE WORD
           char sir[100];
           strcpy(sir, s);
           delete_word(&head, s, &curr, v, &k, &curr_line);
           node_t *aux = v[curr_line];
           int poz = 1;
           while(aux != curr){
              poz++;
              aux = aux->next;
            }
           char str1[10]; sprintf(str1, "%d", poz-1);
           strcat(sir, str1);
           strcat(sir, " ");
           char str2[10]; sprintf(str2, "%d", curr_line);
           strcat(sir, str2);
           strcat(sir, "\n");
           push_stack(&stiva_undo, sir);  
        }
         
        if( strcmp(strtok(aux, " "), "da") == 0 ){            // daca comanda este DELETE ALL
           push_stack(&stiva_undo, s);
           delete_all(&head, s, &curr, v, &k, &curr_line);  
        }
       
       //************************************************* // daca comanda este UNDO
        if( strcmp(s, "u\n") == 0){        
           char u[100];                  //comanda pentru care se aplica "undo" este retinuta in u si qaux
           strcpy(u, stiva_undo->command);
           char qaux[100];
           strcpy(qaux, u);

           if(strcmp(strtok(qaux, " "), "::i") == 0){         //UNDO INSERT/EDIT MODE
                enqueue(&coada_redo, u);
                int i;
                for( i=0; i<strlen(u)-4; i++){
                    node_t *aux = curr->prev;
                    remove_node(&curr, &head);
                    curr = aux;
               }
            }
        
           if( strcmp(strtok(qaux, " "), "gl") == 0 ){          //UNDO GOTO LINE
                enqueue(&coada_redo, u);
                char aux[10];
                strcpy(aux, "gl ");
                char *p = strtok(u, " ");
                char *p1 = strtok(u+strlen(p)+1, " ");
                char *p2 =strtok(u+strlen(p)+strlen(p1)+2, "\n");
                strcat(aux, p2);
                strcat(aux, "\n");
                goto_line(aux, &curr, v, &curr_line);
            }
          
            if( strcmp(strtok(qaux, " "), "gc") == 0 ){           //UNDO GOTO CHARACTER
                char *p1 = strtok(u, "\n");
                char aux[100];
                strcpy(aux, "gc ");
                char *p2 = strtok(u+strlen(p1)+1, "\n");
                strcat(aux, p2);
                strcat(aux, "\n");
                goto_character(aux, &curr, v, &curr_line, &k);
                strcat(p1, "\n");
                enqueue(&coada_redo, p1);
            }
            
           if(  strcmp(strtok(qaux, " "), "d") == 0 || strcmp(strtok(qaux, "\n"), "d") == 0 ){        //UNDO DELETE CHARACTER(S)
                enqueue(&coada_redo, u);
                insert(&head, u+2, &curr, v, &k, &curr_line);
           }
         
            if(  strcmp(strtok(qaux, " "), "re") == 0 ){         //UNDO REPLACE WORD
                enqueue(&coada_redo, u);
                char saux[100];
                char *p = strtok(u, " ");
                char *p1 = strtok(u+strlen(p)+1, " ");
                char *p2 = strtok(u+strlen(p)+1+strlen(p1)+1, "\n");
                strcpy(saux, p);
                strcat(saux, " ");
                strcat(saux, p2);
                strcat(saux, " ");
                strcat(saux, p1);
                strcat(saux, "\n");
                char *p3 = strtok(u+strlen(p)+1+strlen(p1)+1+strlen(p2)+1, " ");
                char *p4 = strtok(u+strlen(p)+1+strlen(p1)+1+strlen(p2)+1+strlen(p3)+1, "\n");
                char aux[100];
                strcpy(aux, "gc ");
                strcat(aux, p3);
                strcat(aux, " ");
                strcat(aux, p4);
                strcat(aux, "\n");
                goto_character(aux, &curr, v, &curr_line, &k);
                replace_word(&head, saux, &curr, v, &k, &curr_line);   
            }
           
            if( strcmp(strtok(qaux, " "), "dw") == 0 ){               //UNDO DELETE WORD
                enqueue(&coada_redo, u);
                char *p = strtok(u, " ");
                char *p1 = strtok(u+strlen(p)+1, "\n");
                char *p2 = strtok(u+strlen(p)+1+strlen(p1)+1, " ");
                char *p3 = strtok(u+strlen(p)+1+strlen(p1)+1+strlen(p2)+1, "\n");
                char aux[100];
                strcpy(aux, "gc ");
                strcat(aux, p2);
                strcat(aux, " ");
                strcat(aux, p3);
                strcat(aux, "\n");
                printf("D aux %s\n", aux);
                goto_character(aux, &curr, v, &curr_line, &k);
                strcat(p1, "\n");
                insert(&head, p1, &curr, v, &k, &curr_line);
            }
         
            if( strcmp(strtok(qaux, " "), "da") == 0 ){             //UNDO DELETE ALL
                enqueue(&coada_redo, u);
            }

            pop_stack(&stiva_undo);    //elimina comanda din stiva
       }
      //************************************** // daca comanda este REDO
       if(strcmp(s, "r\n") == 0){                         
            if(coada_redo.front !=NULL){
                char *r = (char*)calloc((strlen(coada_redo.front->command)+1),sizeof(char));    //comanda pentru care se aplica "redo" este retinuta in r si qaux
                strcpy(r, coada_redo.front->command);    
                char qaux[100];
                strcpy(qaux, r);

                if(strcmp(strtok(qaux, " "), "::i") == 0){            //REDO INSERT/EDIT MODE
                    insert(&head, r+4, &curr, v, &k, &curr_line);
                    }

                if( strcmp(strtok(qaux, " "), "d") == 0 ){        //REDO DELETE CHARACTER(S)
                    int i;
                    for( i=0; i<strlen(r+2)-1; i++)
                    curr = curr->prev;
                    char *sir = calloc(4, sizeof(char));
                    strcpy(sir, "d ");
                    if( strlen(r+2) == 1)
                    sir[2] = '\0';
                    else{
                    sir[2] = strlen(r+2) + 48;
                    sir[3] = '\0';
                    }
                    char *del = delete_character(&head, sir, &curr, v, &curr_line);
                    free(del);
                    free(sir);
                }
    
                if( strcmp(strtok(qaux, " "), "gl") == 0 ){ // REDO GOTO LINE
                    char aux[100];
                    strncpy(aux, r, 5);
                    aux[6] = '\0';
                    strcat(aux, "\n");
                    goto_line(s, &curr, v, &curr_line);
                }

                if( strcmp(strtok(qaux, " "), "gc") == 0 ){              //REDO GOTO CHARACTER
                    goto_character(r, &curr, v, &curr_line, &k);
                }
                
                if( strcmp(strtok(qaux, " "), "dw") == 0 ){             //REDO DELETE WORD
                    char *del = strtok(r, "\n");
                    char *p2 = strtok(r+strlen(del)+1, " ");
                    char *p3 = strtok(r+strlen(del)+1+strlen(p2)+1, "\n");
                    char aux[100];
                    strcpy(aux, "gc ");
                    strcat(aux, p2);
                    strcat(aux, " ");
                    strcat(aux, p3);
                    strcat(aux, "\n");
                    goto_character(aux, &curr, v, &curr_line, &k);
                    strcat(del, "\n");
                    delete_word(&head, del, &curr, v, &k, &curr_line);
                }

                if(  strcmp(strtok(qaux, " "), "re") == 0 ){         //REDO REPLACE WORD
                    char *p = strtok(r, "\n");
                    char *p3 = strtok(r+strlen(p)+1, " ");
                    char *p4 = strtok(r+strlen(p)+1+strlen(p3)+1, "\n");
                    char aux[100];
                    strcpy(aux, "gc ");
                    strcat(aux, p3);
                    strcat(aux, " ");
                    strcat(aux, p4);
                    strcat(aux, "\n");
                    goto_character(aux, &curr, v, &curr_line, &k);
                    strcat(p, "\n");
                    replace_word(&head, p, &curr, v, &k, &curr_line);   
                }

                free(r);
                coada_redo = dequeue(coada_redo);   //elimina comanda din coada
            }
       }
      
    }else {
        insert(&head, s, &curr, v, &k, &curr_line);     //textul citit este inserat in lista dublu inlantuita
        if(stiva_undo !=NULL){    //textul citit este inserat in stiva ca o comanda alaturi de "::i"
            char *x = (char*)calloc((strlen(s)+5),sizeof(char));
            strncpy(x, stiva_undo->command, 3);
            strcat(x, " ");
            x[5] = '\0';
            strcat(x, s);
            stiva_undo->command = realloc(stiva_undo->command,(strlen(s)+strlen(stiva_undo->command)+2)*sizeof(char));
            strcpy(stiva_undo->command, x);
            free(x);
        }
    }    
}
    while(head != NULL){     //se elibereaza lista
       node_t *aux = head->next;
       free(head);
       head = aux;
    }
    while(stiva_undo != NULL)     //se elibereaza stiva
         pop_stack(&stiva_undo);
    while (coada_redo.front != NULL)    //se elibereaza coada
         coada_redo = dequeue(coada_redo);
    free(v);
    fclose(in);
    return 0;
}