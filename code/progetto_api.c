#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int dist;
    int *veicoli;
    int n;
    int max;
}Stazione;

typedef struct{
    Stazione *S;
    int dim;
}Autostrada;

typedef struct{
    int dist;
    int cost;
    int prev;
    int m;
}prova;

typedef struct{
    prova *v;
    int dim;
}Array;

Autostrada aggiungi_stazione(Autostrada A, int d,char *token, int n_auto){
    int j = 0;
    int t = (A.dim) - 1;
    for (int i = 0; i < t; i++) {
        if(A.S[i].dist == d){
            fputs("non aggiunta\n",stdout);
            A.dim = t;
            return A;
        }
    }

    Stazione *temp = NULL;
    temp = malloc( A.dim*sizeof(Stazione));


    for (int i = 0; i < (A.dim)-1; i++){
        if(d < A.S[i].dist){
            j = i;
            break;
        }
        if(i == (A.dim)-2 && d > A.S[i].dist){
            j = (A.dim)-1;
        }
    }

    for (int i = 0; i < j; i++){
        temp[i] = A.S[i];
    }
    for (int i = j; i < (A.dim)-1; i++){
        temp[i+1] = A.S[i];
    }


    temp[j].dist = d;
    temp[j].n = n_auto;
    temp[j].max = 0;
    temp[j].veicoli = NULL;
    temp[j].veicoli = malloc(n_auto*sizeof(int));

    for(int i = 0;i< temp[j].n; i++){
        token = strtok(NULL, " ");
        temp[j].veicoli[i] = atoi(token);
        if(temp[j].veicoli[i] > temp[j].max){
            temp[j].max = temp[j].veicoli[i];
        }
    }

    free(A.S);
    A.S = NULL;
    A.S = temp;

    fputs("aggiunta\n",stdout);
    return A;
}

Autostrada demolisci_stazione(Autostrada A, int d){
    int j = -1;
    int h = 0;
    Stazione *temp = NULL;
    for (int i = 0; i < A.dim; i++){
        if(A.S[i].dist == d){
            j = i;
        }
    }
    if(j == -1){
        fputs("non demolita\n",stdout);
        return A;
    }else{
        temp = malloc(((A.dim) - 1)*sizeof(Stazione));
        for (int i = 0; i < j; i++){
            temp[h]=A.S[i];
            h++;
        }
        for (int i = j+1; i < A.dim; i++){
            temp[h]=A.S[i];
            h++;
        }
        free(A.S);
        A.S = NULL;
        A.S = temp;
        A.dim = (A.dim) - 1;
        fputs("demolita\n",stdout);
        return A;
    }
}


void aggiungi_auto(Autostrada A, int d, int new){
    int *temp = NULL;

    for(int i = 0; i<A.dim; i++){
        if(A.S[i].dist == d){
            A.S[i].n = (A.S[i].n) + 1;

            temp = realloc(A.S[i].veicoli, A.S[i].n*sizeof(int));
            A.S[i].veicoli = NULL;
            A.S[i].veicoli = temp;

            A.S[i].veicoli[A.S[i].n-1] = new;
            if(new > A.S[i].max){
                A.S[i].max = new;
            }
            fputs("aggiunta\n",stdout);
            return;
        }
    }
    fputs("non aggiunta\n",stdout);

}


void rottama_auto(Autostrada A, int d, int canc) {
    int *temp = NULL;
    for (int i = 0; i < A.dim; i++) {
        if (A.S[i].dist == d) {
            for (int j = 0; j < A.S[i].n; j++) {
                if (A.S[i].veicoli[j] == canc) {
                    for (int k = j; k < (A.S[i].n)-1; k++) {
                        A.S[i].veicoli[k] = A.S[i].veicoli[k + 1];
                    }
                    A.S[i].n = (A.S[i].n) - 1;

                    temp = malloc(A.S[i].n*sizeof(int));

                    for(int h = 0; h < A.S[i].n; h++){
                         temp[h] = A.S[i].veicoli[h];
                    }

                    free(A.S[i].veicoli);

                    A.S[i].veicoli = NULL;
                    A.S[i].veicoli = temp;

                    if(canc == A.S[i].max){
                        A.S[i].max = 0;
                        for (int k = 0; k < A.S[i].n; k++){
                            if(A.S[i].veicoli[k] > A.S[i].max){
                                A.S[i].max = A.S[i].veicoli[k];
                            }
                        }
                    }
                    fputs("rottamata\n", stdout);
                    return;
                }
            }
        }
    }
    fputs("non rottamata\n", stdout);
}


void pianifica_percorso(int punto1, int punto2, Autostrada A){

    Array D;
    D.dim = 0;
    D.v =NULL;

    int *P = NULL;
    int x = 0, y = 0;
    int k = 0;
    int h = -1, l = 0;
    int r = -1;

    if(punto1 == punto2){
        printf("%d\n",punto1);
        free(P);
        return;
    }

    if(punto1 < punto2){

        P = malloc(100*sizeof(int));
        for (int i = 0; i < A.dim; ++i) {
            if (A.S[i].dist == punto1) {
                x = i;
            }
            if (A.S[i].dist == punto2) {
                y = i;
                break;
            }
        }

        D.dim = y-x+1;
        D.v = malloc(D.dim*sizeof(Array));

        for (int i = x; i < y; i++) {
            r++;
            D.v[r].prev = -1;
            D.v[r].m = 0;
            D.v[r].cost = 0;
            D.v[r].dist = A.S[i].dist;
            //-----找到#nodi adiacenti-----
            for (int j = i + 1; j <= y; j++) {
                if (A.S[i].max >= A.S[j].dist - A.S[i].dist) {
                    D.v[r].m++;
                } else{
                    break;
                }
            }

        }
        r++;
        D.v[r].prev = -1;
        D.v[r].cost = 0;
        D.v[r].dist = A.S[y].dist;

        while (k < D.dim - 1){
            for (int i = k + 1; i <= k + D.v[k].m; i++) {
                //--------se il nodo non è ancora stato visitato----------
                if(D.v[i].cost == 0){
                    D.v[i].cost = D.v[k].cost + 1;
                    D.v[i].prev = k;
                }else  //--------se visitato ma con #archi maggiore----------
                if(D.v[k].cost + 1 < D.v[i].cost){
                    D.v[i].cost = D.v[k].cost + 1;
                    D.v[i].prev = k;
                }else  //--------se visitato con stesso #archi ma ha distanza nodo precedente maggiore----------
                if((D.v[k].cost + 1 == D.v[i].cost) && k < D.v[i].prev){
                    D.v[i].cost = D.v[k].cost + 1;
                    D.v[i].prev = k;
                }
            }
            k++;

        }
    }else if(punto1 > punto2) {

        P = malloc(100*sizeof(int));
        for (int i = 0; i < A.dim; ++i) {
            if (A.S[i].dist == punto1) {
                x = i;
                break;
            }
            if (A.S[i].dist == punto2) {
                y = i;
            }
        }

        D.dim = x-y+1;
        D.v = malloc(D.dim*sizeof(Array));

        for (int i = x; i > y; i--) {
            r++;
            D.v[r].prev = -1;
            D.v[r].m = 0;
            D.v[r].cost = 0;
            D.v[r].dist = A.S[i].dist;
            //-----找到#nodi adiacenti-----
            for (int j = i - 1; j >= y; j--) {
                if (A.S[i].max >= A.S[i].dist - A.S[j].dist) {
                    D.v[r].m++;
                } else {
                    break;
                }
            }

        }

        r++;
        D.v[r].prev = -1;
        D.v[r].cost = 0;
        D.v[r].dist = A.S[y].dist;

        while (k < D.dim - 1) {
            for (int i = k + 1; i <= k + D.v[k].m; i++) {

                //--------se il nodo non è ancora stato visitato----------
                if (D.v[i].cost == 0) {
                    D.v[i].cost = D.v[k].cost + 1;
                    D.v[i].prev = k;
                } else  //--------se visitato ma con #archi maggiore----------
                if (D.v[k].cost + 1 < D.v[i].cost) {
                    D.v[i].cost = D.v[k].cost + 1;
                    D.v[i].prev = k;
                } else  //--------se visitato con stesso #archi ma ha distanza nodo precedente maggiore----------
                if ((D.v[k].cost + 1 == D.v[i].cost) && k > D.v[i].prev) {
                    D.v[i].cost = D.v[k].cost + 1;
                    D.v[i].prev = k;
                }
            }
            k++;
        }
    }

    //--------stampa--------
    l = D.dim -1;
    D.v[0].prev = -2;
    while(l >= 0){
        h++;
        P[h] = D.v[l].dist;
        l = D.v[l].prev;
        if(l == -1 ){
            fputs("nessun percorso\n",stdout);
            free(P);
            free(D.v);
            D.dim = 0;
            return;
        }
    }
    while (h >= 0) {
        if(h == 0){
            printf("%d\n",P[h]);
            h--;
            break;
        }
        printf("%d ",P[h]);
        h--;
    }

    free(P);
    free(D.v);
    D.dim = 0;

}


int main() {
    char *token, input[6000];
    int dist = 0, autonomia, n_auto;
    int partenza, arrivo;

    Autostrada a;
    a.dim = 0;
    a.S = malloc(a.dim*sizeof(Stazione));

    while(fgets(input,6000,stdin) != NULL){
        token = strtok(input, " ");

        if(strcmp(token,"aggiungi-stazione") == 0){
            token = strtok(NULL, " ");
            dist = atoi(token);
            a.dim = a.dim + 1;
            token = strtok(NULL, " ");
            n_auto = atoi(token);
            a = aggiungi_stazione(a,dist ,token, n_auto);

        }else if(strcmp(token,"demolisci-stazione") == 0){
            token = strtok(NULL, " ");
            dist = atoi(token);
            a = demolisci_stazione(a, dist);

        }else if(strcmp(token,"aggiungi-auto") == 0){
            token = strtok(NULL, " ");
            dist = atoi(token);
            token = strtok(NULL, " ");
            autonomia = atoi(token);
            aggiungi_auto(a, dist, autonomia);

        }else if(strcmp(token,"rottama-auto") == 0){
            token = strtok(NULL, " ");
            dist = atoi(token);
            token = strtok(NULL, " ");
            autonomia = atoi(token);
            rottama_auto(a, dist, autonomia);

        }else if(strcmp(token,"pianifica-percorso") == 0){
            token = strtok(NULL, " ");
            partenza = atoi(token);
            token = strtok(NULL, " ");
            arrivo = atoi(token);
            pianifica_percorso(partenza,arrivo,a);

        }
        fflush(stdout);
        memset(input,0, 6000);


    }
    return 0;
}
