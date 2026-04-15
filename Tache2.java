/**
 * ============================================================================
 * TACHE 2 : CODAGE MEMOIRE (ORDRE D'APPARITION)
 * ============================================================================
 * Objectif : Transformer l'analyseur précédent pour qu'il produise le codage
 *           en mémoire du polynôme reconnu. Rien de l'analyseur ne disparaît.
 *
 * Représentation : liste chaînée dont chaque maillon représente un monôme.
 * Polynôme nul = liste vide.
 *
 * Compilation : javac polynome/Tache2.java
 * Utilisation : java polynome.Tache2 "polynome"
 * ============================================================================
 */
package polynome;

public class Tache2 {
    
    // Variables globales
    private static String input;
    private static int pos;
    
    // Tête de la liste chaînée (polynôme nul = null)
    private static Polynome teteListe = null;
    
    // ==================== FONCTIONS D'ANALYSE ====================
    
    private static void erreur(String msg) {
        System.out.println("ERREUR DE SYNTAXE : " + msg);
        System.exit(1);
    }
    
    private static void ignorerEspaces() {
        while (pos < input.length() && (input.charAt(pos) == ' ' || input.charAt(pos) == '\t')) {
            pos++;
        }
    }
    
    private static char courant() {
        if (pos >= input.length()) return '\0';
        return input.charAt(pos);
    }
    
    private static void avancer() {
        if (pos < input.length()) pos++;
    }
    
    private static boolean estChiffre(char c) {
        return c >= '0' && c <= '9';
    }
    
    // ==================== ANALYSEURS AVEC EXTRACTION ====================
    
    private static int lireNaturel() {
        if (!estChiffre(courant())) erreur("Naturel attendu");
        int val = 0;
        while (estChiffre(courant())) {
            val = val * 10 + (courant() - '0');
            avancer();
        }
        return val;
    }
    
    private static double lireNombre() {
        int partieEntiere = lireNaturel();
        double val = (double) partieEntiere;
        if (courant() == '.') {
            avancer();
            double frac = 0.0;
            double div = 10.0;
            while (estChiffre(courant())) {
                frac += (courant() - '0') / div;
                div *= 10.0;
                avancer();
            }
            val += frac;
        }
        return val;
    }
    
    private static int lireXpuissance() {
        if (courant() != 'X') erreur("'X' attendu");
        avancer();
        if (courant() == '^') {
            avancer();
            return lireNaturel();
        }
        return 1;
    }
    
    private static Monome lireMonome(int signe) {
        ignorerEspaces();
        double coeff;
        int exp;
        
        if (estChiffre(courant())) {
            coeff = lireNombre();
            if (courant() == '*') {
                avancer();
                exp = lireXpuissance();
            } else {
                exp = 0;
            }
        } else if (courant() == 'X') {
            coeff = 1.0;
            exp = lireXpuissance();
        } else {
            erreur("Monome invalide");
            return null;
        }
        
        coeff *= signe;
        return new Monome(coeff, exp);
    }
    
    // ==================== FONCTIONS DE GESTION DE LA LISTE ====================
    
    private static Polynome creerMaillon(double coeff, int exp) {
        return new Polynome(new Monome(coeff, exp));
    }
    
    /**
     * Ajoute un monôme à la fin de la liste (ordre d'apparition)
     * Les monômes de coefficient nul sont ignorés
     */
    private static void ajouterEnFin(double coeff, int exp) {
        if (coeff == 0.0) {
            return;
        }
        
        Polynome nouveau = creerMaillon(coeff, exp);
        
        if (teteListe == null) {
            teteListe = nouveau;
            return;
        }
        
        Polynome courant = teteListe;
        while (courant.getSuivant() != null) {
            courant = courant.getSuivant();
        }
        courant.setSuivant(nouveau);
    }
    
    // ==================== ANALYSEUR PRINCIPAL ====================
    
    /**
     * parsePolynome : analyse le polynôme complet et construit la liste chaînée
     * polynome → [ '-' ] monome { ( '+' | '-' ) monome }
     */
    private static void parsePolynome() {
        int signe = 1;
        
        ignorerEspaces();
        
        // Signe optionnel devant le premier monôme (seul '-' est autorisé)
        if (courant() == '-') {
            signe = -1;
            avancer();
        }
        
        ignorerEspaces();
        
        // Premier monôme
        Monome m = lireMonome(signe);
        ajouterEnFin(m.getCoefficient(), m.getExposant());
        
        ignorerEspaces();
        
        // Monômes suivants
        while (courant() == '+' || courant() == '-') {
            if (courant() == '+') {
                avancer();
                m = lireMonome(1);
            } else {
                avancer();
                m = lireMonome(-1);
            }
            ajouterEnFin(m.getCoefficient(), m.getExposant());
            ignorerEspaces();
        }
        
        // Vérifier qu'on a tout consommé
        if (courant() != '\0') {
            erreur("Caractere inattendu en fin de chaine");
        }
    }
    
    // ==================== MAIN ====================
    
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java polynome.Tache2 \"polynome\"");
            System.out.println("Exemple: java polynome.Tache2 \"3*X^2 + X - 5\"");
            System.exit(1);
        }
        
        input = args[1];
        pos = 0;
        teteListe = null;
        
        parsePolynome();
        
        // Pas d'affichage - conforme à la tâche 2
        // Le programme se termine silencieusement si tout est correct
    }
}