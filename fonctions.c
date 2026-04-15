#include "polynome.h"

// ============================================================
//                        AFFICHAGE
// ============================================================

// etape 1 : afficher un seul monome
static void afficher_monome(double c, unsigned int e) {
    if (e == 0) { 
        printf("%g", c); // affichera directement le nombre
    } else if (e == 1) { 
        if (c == 1.0)       printf("X"); // affichera X 
        else if (c == -1.0) printf("-X");// affichera -X
        else                printf("%g*X", c); // affichera nombre*X
    } else {
        if (c == 1.0)       printf("X^%u", e); // affichera X^nombre
        else if (c == -1.0) printf("-X^%u", e); // affichera -X^nombre 
        else                printf("%g*X^%u", c, e); // affichera nombre*X^nombre  
    }
}

// etape 2 : afficher le signe entre deux monomes consecutifs
static void afficher_signe(double c) {
    if (c >= 0.0) printf(" + ");
    else          printf(" - ");
}

// tape 3 : afficher le polynome complet
void afficher(polynome *p) {
    if (p == NULL) {
        printf("0\n");
        return;
    }

    afficher_monome(p->m.coefficient, p->m.exposant);
    p = p->suivant; 

    while (p != NULL) {
        double c       = p->m.coefficient;
        unsigned int e = p->m.exposant;
        afficher_signe(c);
        afficher_monome((c < 0.0) ? -c : c, e);  
        p = p->suivant;
    }

    printf("\n");
}

// ============================================================
//                        ADDITION
// ============================================================

polynome* plus(polynome *a, polynome *b) {
    polynome *res   = NULL;
    polynome *queue = NULL;

    while (a != NULL && b != NULL) {

        // allouer le prochain nœud du résultat
        polynome *n = (polynome*) malloc(sizeof(polynome)); 
        n->suivant  = NULL;

        if (a->m.exposant > b->m.exposant) {
            n->m = a->m; // le nouveau mailon cree va prendre les valeurs du premier maillon du polynome a
            a = a->suivant;

        } else if (a->m.exposant < b->m.exposant) {
            n->m = b->m; //le nouveau mailon cree va prendre les valeurs du premier maillon du polynome a
            b = b->suivant; 

        } else {
            // meme exposant : additionner les coefficients
            double somme = a->m.coefficient + b->m.coefficient;
            if (somme == 0.0) {
                //les termes s'annulent, on jette le noeud
                free(n);
                a = a->suivant;
                b = b->suivant;
                continue;
            }
            n->m.coefficient = somme; // la somme des coef des monomes de meme degre va etre stocke au niveau du polynome n
            n->m.exposant    = a->m.exposant;  // l'exposant du polynome n sera le meme que les exposants des monomes qui ont permis de faire la somme
            a = a->suivant;
            b = b->suivant;
        }

        if (res == NULL) res = n;
        else queue->suivant = n; // va relier le nouveau noeud a liste
        queue = n; // se place sur le dernier noeud 
    }
// 4*X^3 + 2*X^2 + 3*X + 1 + 
//5*X^4 + 6*X^3 + 7*X^2 =>B
//5*X^4 + 10 * X^3  +9 * X^2 
    // recopier les monomes restants
    // cette boucle s'execute quand une des deux listes est vide mais l'autre a encore des monomes
    polynome *reste = (a != NULL) ? a : b;  // on choisit la liste qui n'est pas encore vide

    while (reste != NULL) {

        polynome *n = (polynome*) malloc(sizeof(polynome)); // on cree un nouveau noeud

        
        n->m = reste->m; // on copie le monome (coefficient + exposant) tel quel

        n->suivant = NULL; // ce noeud est le dernier pour l'instant, rien apres lui

        if (res == NULL) res = n; // si le resultat (res) est vide ce noeud devient la tete
        else queue->suivant = n;  // sinon on l'accroche apres le dernier noeud deja insere

        queue = n; // queue avance sur ce nouveau dernier noeud

        reste = reste->suivant;
    }

    return res;
}

// ============================================================
//                       SOUSTRACTION
// ============================================================

polynome* moins(polynome *a, polynome *b) {
    polynome *res   = NULL;
    polynome *queue = NULL;

    while (a != NULL && b != NULL) {

        polynome *n = (polynome*) malloc(sizeof(polynome));
        n->suivant  = NULL;

        if (a->m.exposant > b->m.exposant) {
            // le monome de a a le plus grand degre, on le prend tel quel
            n->m = a->m;
            a = a->suivant;

        } else if (a->m.exposant < b->m.exposant) {
            // le monome de b a le plus grand degre
            // on inverse son coefficient car on soustrait b
            n->m.coefficient = -(b->m.coefficient);
            n->m.exposant    = b->m.exposant;
            b = b->suivant;

        } else {
            // meme exposant : on soustrait les coefficients
            double diff = a->m.coefficient - b->m.coefficient;
            if (diff == 0.0) {
                // les termes s'annulent, on jette le noeud
                free(n);
                a = a->suivant;
                b = b->suivant;
                continue;
            }
            n->m.coefficient = diff;
            n->m.exposant = a->m.exposant;
            a = a->suivant;
            b = b->suivant;
        }

        if (res == NULL) res = n;
        else queue->suivant = n;
        queue = n;
    }

    // recopier les monomes restants de a tels quels
    while (a != NULL) {
        polynome *n      = (polynome*) malloc(sizeof(polynome));
        n->m             = a->m;
        n->suivant       = NULL;
        if (res == NULL) res            = n;
        else             queue->suivant = n;
        queue = n;
        a = a->suivant;
    }
/*
a= x^2 + 2*x
b=3*x^3 + 4*x + 7
a - b = -3*x^3  + 4*x^2 - 2*x -7 

*/
    // recopier les monomes restants de b en inversant leur coefficient
    while (b != NULL) {
        polynome *n   = (polynome*) malloc(sizeof(polynome));
        n->m.coefficient  = -(b->m.coefficient);
        n->m.exposant = b->m.exposant;
        n->suivant = NULL;
        if (res == NULL) res = n;
        else queue->suivant = n;
        queue = n;
        b = b->suivant;
    }

    return res;
}

// ============================================================
//                      MULTIPLICATION
// ============================================================

/* MULTIPLICATION
 
   Pour chaque monome de a :
     1. on multiplie tous les monomes de b par ce monome
        → on obtient une petite liste intermediaire
     2. on additionne cette liste au resultat avec plus()
 
   Ex : (2*X^2 + 3) * (X + 1)
     tour 1 : 2*X^2 * (X + 1) = 2*X^3 + 2*X^2   → res = 2*X^3 + 2*X^2
     tour 2 : 3     * (X + 1) = 3*X   + 3        → res = 2*X^3 + 2*X^2 + 3*X + 3
*/

polynome* fois(polynome *a, polynome *b) {

    polynome *res = NULL; 

    for (polynome *i = a; i != NULL; i = i->suivant) { // on parcourt chaque monome de a 

        // construire la liste intermediaire : monome i * chaque monome de b
        polynome *inter  = NULL;
        polynome *queue  = NULL;

        for (polynome *j = b; j != NULL; j = j->suivant) {

            polynome *n  = (polynome*) malloc(sizeof(polynome));
            // coefficient = produit des deux coefficients
            n->m.coefficient = i->m.coefficient * j->m.coefficient;
            // exposant    = somme des deux exposants
            n->m.exposant    = i->m.exposant + j->m.exposant;
            n->suivant       = NULL;

            // on accroche ce noeud en queue de la liste intermediaire
            if (inter == NULL) inter = n;
            else queue->suivant = n;
            queue = n;
        }

        // on additionne la liste intermediaire au resultat
        polynome *nouveau_res = plus(res, inter);
        free(res);   // l'ancien resultat n'est plus utile
        res = nouveau_res;
    }

    return res;
}

// ============================================================
//                        DIVISION
// ============================================================

/* 
  DIVISION EUCLIDIENNE  —  quotient(a, b, &reste)
 
  On cherche q et r tels que :  a = b * q + r
  avec deg(r) < deg(b)
 
  Algorithme :
  tant que le degre de rem >= degre de b :
    1. prendre le terme dominant de rem
    2. le diviser par le terme dominant de b  → prochain terme du quotient
    3. soustraire (ce terme * b) de rem
 
  rem (remainder) = le reste courant, commence comme copie de a
  et retrecit a chaque tour jusqu'a ce que son degre soit < deg(b)
*/

polynome* quotient(polynome *a, polynome *b, polynome **reste) {

    polynome *q     = NULL;
    polynome *queue = NULL;

    // copie de a dans rem pour ne pas modifier a
    polynome *rem  = NULL;
    polynome *qrem = NULL;
    for (polynome *t = a; t != NULL; t = t->suivant) {
        polynome *n  = (polynome*) malloc(sizeof(polynome));
        n->m = t->m;
        n->suivant = NULL;
        if (rem == NULL) rem = n; 
        else qrem->suivant = n;
        qrem = n;
    }

    while (rem != NULL && rem->m.exposant >= b->m.exposant) {

        // etape 1 : prochain terme du quotient
        double c = rem->m.coefficient / b->m.coefficient;
        unsigned int e = rem->m.exposant  - b->m.exposant;

        // etape 2 : ajouter ce terme au quotient
        polynome *terme      = (polynome*) malloc(sizeof(polynome));
        terme->m.coefficient = c;
        terme->m.exposant    = e;
        terme->suivant       = NULL;
        if (q == NULL) q = terme;
        else queue->suivant = terme;
        queue = terme;

        // etape 3 : rem = rem - (terme * b)
        polynome mono  = {{c, e}, NULL};
        polynome *produit   = fois(&mono, b);
        polynome *nouveau   = moins(rem, produit);
        free(rem);
        rem = nouveau;
    }

    // rem est maintenant le reste final, on le renvoie via le pointeur
    *reste = rem;

    return q;
}
