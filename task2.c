/**
 * ============================================================================
 * TACHE 2 : CODAGE MEMOIRE (ORDRE D'APPARITION)
 * ============================================================================
 * Objectif : Transformer l'analyseur précédent pour qu'il produise le codage
 *           en mémoire du polynôme reconnu. Rien de l'analyseur ne disparaît.
 *
 * Représentation : liste chaînée dont chaque maillon représente un monôme.
 * Polynôme nul = liste vide.
 * ============================================================================
 */

#include "polynome.h"

/* ==================== VARIABLES GLOBALES ==================== */

char input[1000];
int pos = 0;

/* Liste chaînée représentant le polynôme */
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

/**
 * lireNaturel : lit et retourne un entier naturel.
 */
unsigned int lireNaturel(void) {
    if (!estChiffre(courant())) erreur("Naturel attendu");
    unsigned int val = 0;
    while (estChiffre(courant())) {
        val = val * 10 + (courant() - '0');
        avancer();
    }
    return val;
}

/**
 * lireNombre : lit et retourne un nombre réel.
 */
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

/**
 * lireXpuissance : lit et retourne l'exposant d'un X^...
 */
unsigned int lireXpuissance(void) {
    if (courant() != 'X') erreur("'X' attendu");
    avancer();
    if (courant() == '^') {
        avancer();
        return lireNaturel();
    }
    return 1;
}

/**
 * analyserMonome : analyse un monôme et retourne ses caractéristiques.
 * 
 * @param signe  Signe du monôme (+1 ou -1)
 * @param coeff  Pointeur pour stocker le coefficient
 * @param exp    Pointeur pour stocker l'exposant
 * @return       1 si réussi, 0 sinon
 */
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

/* ==================== FONCTIONS DE GESTION DE LA LISTE ==================== */

polynome* creerMaillon(double coeff, unsigned int exp) {
    polynome* nouveau = (polynome*)malloc(sizeof(polynome));
    if (nouveau == NULL) erreur("Memoire insuffisante");
    nouveau->m.coefficient = coeff;
    nouveau->m.exposant = exp;
    nouveau->suivant = NULL;
    return nouveau;
}

void ajouterEnFin(polynome** tete, double coeff, unsigned int exp) {
    if (coeff == 0.0) return;
    
    polynome* nouveau = creerMaillon(coeff, exp);
    
    if (*tete == NULL) {
        *tete = nouveau;
        return;
    }
    
    polynome* courant = *tete;
    while (courant->suivant != NULL) courant = courant->suivant;
    courant->suivant = nouveau;
}

/* ==================== ANALYSEUR PRINCIPAL ==================== */

/**
 * analyserPolynome : analyse la chaîne et construit la liste chaînée.
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
    ajouterEnFin(&teteListe, coeff, exp);
    
    ignorer_espaces();
    
    /* Monômes suivants */
    while (courant() == '+' || courant() == '-') {
        signe = (courant() == '+') ? 1 : -1;
        avancer();
        
        ignorer_espaces();
        
        if (!analyserMonome(signe, &coeff, &exp)) {
            erreur("Monome invalide");
        }
        ajouterEnFin(&teteListe, coeff, exp);
        
        ignorer_espaces();
    }
    
    /* Vérification qu'on a tout consommé */
    if (courant() != '\0') {
        erreur("Caractere inattendu en fin de chaine");
    }
}