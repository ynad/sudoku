/*
 * =======================================================================================
 *
 *      Algoritmi e programmazione
 *      Progetto: Generatore di SUDOKU
 *      Versione:       14  -  2018.03.02
 *
 * =======================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/*
 * da fare:
 * - stampa
 */

#define D 9     //dim matrice sudoku
#define TRIES 50
#define LM 20+2 //stringa nome file
#define EMPTY 0

void gen_matrix(int [][D]);
int is_valid(int, int [][D], int, int);
void biancatura (int [][D], int [][D], int, int*);
int stampa(int [][D], int [][D], char *, int);
void rand_points(int *, int *);
void clean_mat(int [][D]);

int rn;

int main(int argc, char **argv)
{
    int i, n_c, diff;
    int matrix_s[D][D]; //matrice soluta
    int matrix_v[D][D]; //matrice solvenda
    char nome_f[LM];
    char *level[]={"difficile", "medio", "facile"};
    int levdiff[]={20, 25, 30};
    FILE *reset;

    /* controllo che il numero di input sia corretto e che la stringa contenente il nome file sia della lunghezza appropriata */
    if(argc != 2 || strlen(argv[1]) > LM) {
        fprintf(stderr, "\n  Errore in input!\n  Sintassi:\n\t%10s <file-output>\n", argv[0]);
        return 1;
    }
    /* richiesta per il numero e la difficoltà dei sudoku da generare */
    printf("\n Quanti Sudoku vuoi generare?\t");
    if (scanf("%d", &n_c) == EOF) {
        fprintf(stderr, "\n Input errato!\t");
        return 1;
    }

    printf(" Seleziona difficolta':\n\t(1) Difficile\t(2) Medio\t(3) Facile\n");
    if (scanf("%d", &diff) == EOF) {
        fprintf(stderr, "\n Input errato!\t");
        return 1;
    }
        

    while(diff != 1 && diff != 2 && diff != 3) {
        printf(" Errore nell'immisione, valori ammessi:\n\t(1) Difficile\t(2) Medio\t(3) Facile\n");
        if (scanf("%d", &diff) == EOF) {
            fprintf(stderr, "\n Input errato!\t");
            return 1;
        }
    }

    /* roba */
    srand(time(NULL));
    strcpy(nome_f, argv[1]);
    reset = fopen(nome_f, "w"); //deleta il file se già scritto
    fclose(reset);
    rn = (int)sqrt(D);

    /* ripeto il codice finche non raggiungo le n_c volte */
    for(i=0; i<n_c; i++) {
        /* richiamo la funzione per generare la matrice */
        gen_matrix(matrix_s);

        /* genero matrix_v di difficoltà diff */
        biancatura(matrix_s, matrix_v, diff, levdiff);

        while(stampa(matrix_s, matrix_v, nome_f, i)) {
            printf("\n Errore file di output!\n Inserire un nuovo nome file:\t");
            if (scanf("%s", nome_f) == EOF) {
                fprintf(stderr, "\n Input errato!\t");
                return 1;
            }
        }
    }
    printf("\n %d Sudoku generati di livello %s e scritti sul file %s!\n", n_c, level[diff-1], nome_f);

    return 0;
}


void gen_matrix(int matrix[][D])
{
    int i, j, tmp, giri, check;
    int verif[D+1];

    /* inizializzo matrice */
    clean_mat(matrix);

    giri = 0;
    /* i primi 2 cicli for servono per spostarsi all'interno della matrice del sudoku */
    for(i=0; i<D; i++) {
        for(j=0; j<10; j++)  verif[j] = 0;  //backtrack
        for(j=0; j<D; j++) {

        /* se esegue più di TRIES prove nel cercare di completare una riga, azzera la riga in corso e quella
        prima, modificando indice riga/colonna e tenendo conto dei giri con contatori */
            if(giri >= TRIES) {
                j = 0;
                i--;
                giri = 0;
                for(tmp=0; tmp<10; tmp++)  verif[tmp] = 0;
            } else {
                check = 0;
                giri = 0;
            }
            while( !check && giri<TRIES) {
                giri++;

                /* così facendo genero numeri tra 0 e 8, incrementandoli sempre di 1 ottengo n tra 1 e 9 */
                tmp = rand() % D +1;
                while(verif[tmp] == 1)
                    tmp = rand() % D +1;

                /* scrive il numero se i controlli hanno avuto succeso */
                if(is_valid(tmp, matrix, i, j)) {
                    matrix[i][j] = tmp;
                    verif[tmp] = 1;
                    check = 1;
                }
            }
        }
    }
}

/* funzione di verifica di un dato numero */
int is_valid(int tmp, int matrix[][D], int i, int j)
{
    int k, n, m, p, q;

    /* controllo che non ci sia lo stesso numero sulla colonna */
    for(k=0; k<i; k++) {
        if(tmp == matrix[k][j])
            return 0;
    }
    /* controllo che non ci sia lo stesso numero sulla riga */
    for(k=0; k<j; k++) {
        if(tmp == matrix[i][k])
            return 0;
    }
    n = m = -1;
    //ricavo indici del quadrato minore
    for(k=D/rn; k<=D && (n==-1 || m==-1); k+=D/rn) {
        if(i < k && n == -1)
            n = k - rn;
        if(j < k && m == -1)
            m = k -rn;
    }
    //identificato il quadrato minore cerco ripetizioni del numero
    for(p=n; p<n+rn; p++) {
        for(q=m; q<m+rn; q++)   //se si termina a i e j rispettivamente, non controlla tutto il minore?
            if(i!=p && j!=q && tmp == matrix[p][q])
                return 0;
    }
    //tutto OK
    return 1;
}

void biancatura(int matrix_s[][D], int matrix_v[][D], int diff, int *levdiff)
{
    int i, px, py;

    clean_mat(matrix_v);

    for(i=0; i<levdiff[diff-1]; i++) {
        rand_points(&px, &py);
        /* se la matrice in tali coordinate è già scritta genero nuovi punti finchè vanno bene */
        while(matrix_v[px][py] != EMPTY ) {
            rand_points(&px, &py);
        }
        matrix_v[px][py] = matrix_s[px][py];
    }
}

/*
*   Nuova versione in sviluppo
*/
int stampa(int matrix_s[][D], int matrix_v[][D], char *autput, int num)
{
    int i, j;
    FILE *fyle;
    char layout[38]={"-------------------------------------"};  //stringa per layout nella stampa

    if((fyle = fopen(autput, "a")) == NULL) {   //scrittura in aggiunta al file già esistente
        return(-2);
    }

    /* TEMPORANEO: stampa le matrice con il vecchio stile */

    fprintf(fyle, "\nSUDOKU DA RISOLVERE NUMERO %d:\n", num+1);
    fprintf(fyle, "%37s\n", layout);
    for(i=0; i<D; i++) {
        fprintf(fyle, "| ");
        for(j=0; j<D; j++) {
            if(matrix_v[i][j] != EMPTY)
                fprintf(fyle, "%2d ", matrix_v[i][j]);
            else
                fprintf(fyle, " . ");
            if((j+1)%3==0)
                fprintf(fyle, " | ");
        }
        if((i+1)%3==0)
            fprintf(fyle, "\n%37s", layout);
        fprintf(fyle, "\n");
    }

    fprintf(fyle, "\nSUDOKU RISOLTO NUMERO %d:\n", num+1);
    fprintf(fyle, "%37s\n", layout);
    for(i=0; i<D; i++) {
        fprintf(fyle, "| ");
        for(j=0; j<D; j++) {
            fprintf(fyle, "%2d ", matrix_s[i][j]);
            if((j+1)%3==0)
                fprintf(fyle, " | ");
        }
        if((i+1)%3==0)
            fprintf(fyle, "\n%37s", layout);
        fprintf(fyle, "\n");
    }
    fprintf(fyle, "\n\n=========================================================\n\n");
    fclose(fyle);

    return 0;
}

/* genera coppia di punti casuale */
void rand_points(int *x, int *y)
{
    *x = rand() % D;
    *y = rand() % D;
}

/* inizializza matrice */
void clean_mat(int matrix[][D])
{
    int i, j;

    for(i=0; i<D; i++)
        for(j=0; j<D; j++)
            matrix[i][j] = EMPTY;
}
