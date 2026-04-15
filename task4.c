/**
 * ============================================================================
 * TACHE 4 : CODAGE PAR DEGRE DECROISSANT
 * ============================================================================
 * Objectif : Modifier le programme de la question 2 de sorte qu'un polynôme
 *           soit représenté par une liste chaînée rangée par ordre décroissant
 *           des degrés de ses monômes.
 * ============================================================================
 */

#include "polynome.h"

/* ==================== VARIABLES GLOBALES ==================== */

char input[1000];
int pos = 0;

/* Liste chaînée représentant le polynôme (triée par degré décroissant) */
static polynome* teteListe = NULL;

/* ==================== FONCTIONS UTILITAIRES ==================== */

char courant(void) {
    return input[pos];
}

void avancer(void) {
    if (input[pos] != '\0') pos++;
}

void ignorer_espaces(void) {
    while (courant() == ' ' || courant() == '\t') avancer();
}

void erreur(const char* msg) {
    printf("ERREUR DE SYNTAXE : %s\n", msg);
    exit(1);
}

int estChiffre(char c) {
    return (c >= '0' && c <= '9');
}

/* ==================== ANALYSEURS AVEC EXTRACTION ==================== */

unsigned int lireNaturel(void) {
    if (!estChiffre(courant())) erreur("Naturel attendu");
    unsigned int val = 0;
    while (estChiffre(courant())) {
        val = val * 10 + (courant() - '0');
        avancer();
    }
    return val;
}

double lireNombre(void) {
    unsigned int partieEntiere = lireNaturel();
    double val = (double)partieEntiere;
    if (courant() == '.') {
        avancer();
        double frac = 0.0, div = 10.0;
        while (estChiffre(courant())) {
            frac += (courant() - '0') / div;
            div *= 10.0;
            avancer();
        }
        val += frac;
    }
    return val;
}

unsigned int lireXpuissance(void) {
    if (courant() != 'X') erreur("'X' attendu");
    avancer();
    if (courant() == '^') {
        avancer();
        return lireNaturel();
    }
    return 1;
}

int analyserMonome(int signe, double* coeff, unsigned int* exp) {
    ignorer_espaces();
    
    if (estChiffre(courant())) {
        *coeff = lireNombre();
        if (courant() == '*') {
            avancer();
            *exp = lireXpuissance();
        } else {
            *exp = 0;
        }
    } else if (courant() == 'X') {
        *coeff = 1.0;
        *exp = lireXpuissance();
    } else {
        return 0;
    }
    
    *coeff *= signe;
    return 1;
}

/* ==================== GESTION DE LA LISTE TRIEE ==================== */

polynome* creerMaillon(double coeff, unsigned int exp) {
    polynome* nouveau = (polynome*)malloc(sizeof(polynome));
    if (nouveau == NULL) erreur("Memoire insuffisante");
    nouveau->m.coefficient = coeff;
    nouveau->m.exposant = exp;
    nouveau->suivant = NULL;
    return nouveau;
}

/**
 * insererTriee : insère un monôme dans la liste triée par exposant décroissant.
 * 
 * Algorithme :
 *   1. Si coefficient nul → ignorer
 *   2. Parcourir la liste pour trouver la position (exposant décroissant)
 *   3. Si même exposant trouvé → additionner les coefficients
 *   4. Si somme nulle → supprimer le maillon
 *   5. Sinon → insérer nouveau maillon à la bonne position
 */
void insererTriee(polynome** tete, double coeff, unsigned int exp) {
    if (coeff == 0.0) return;
    
    polynome* courant = *tete;
    polynome* precedent = NULL;
    
    /* Recherche de la position (exposant décroissant) */
    while (courant != NULL && courant->m.exposant > exp) {
        precedent = courant;
        courant = courant->suivant;
    }
    
    /* Même exposant trouvé → fusion */
    if (courant != NULL && courant->m.exposant == exp) {
        courant->m.coefficient += coeff;
        
        if (courant->m.coefficient == 0.0) {
            /* Supprimer le maillon */
            if (precedent == NULL) {
                *tete = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }
            free(courant);
        }
        return;
    }
    
    /* Insertion d'un nouveau maillon */
    polynome* nouveau = creerMaillon(coeff, exp);
    
    if (precedent == NULL) {
        /* Insertion en tête */
        nouveau->suivant = *tete;
        *tete = nouveau;
    } else {
        /* Insertion au milieu ou en fin */
        nouveau->suivant = courant;
        precedent->suivant = nouveau;
    }
}

/* ==================== ANALYSEUR PRINCIPAL ==================== */

/**
 * analyserPolynome : analyse la chaîne et construit la liste triée.
 * 
 * polynôme → [ '-' ] monôme { ( '+' | '-' ) monôme }
 */
void analyserPolynome(void) {
    int signe = 1;
    double coeff;
    unsigned int exp;
    
    ignorer_espaces();
    
    /* Signe optionnel devant le premier monôme */
    if (courant() == '-') {
        signe = -1;
        avancer();
    }
    
    ignorer_espaces();
    
    /* Premier monôme */
    if (!analyserMonome(signe, &coeff, &exp)) {
        erreur("Monome invalide");
    }
    insererTriee(&teteListe, coeff, exp);
    
    ignorer_espaces();
    
    /* Monômes suivants */
    while (courant() == '+' || courant() == '-') {
        signe = (courant() == '+') ? 1 : -1;
        avancer();
        
        ignorer_espaces();
        
        if (!analyserMonome(signe, &coeff, &exp)) {
            erreur("Monome invalide");
        }
        insererTriee(&teteListe, coeff, exp);
        
        ignorer_espaces();
    }
    
    /* Vérification qu'on a tout consommé */
    if (courant() != '\0') {
        erreur("Caractere inattendu en fin de chaine");
    }
}