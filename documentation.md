# Documentation — Manipulation de polynômes en C

Les polynômes sont représentés par des listes chaînées de monômes, définies dans `polynome.h`.
Chaque nœud contient un `monome` (coefficient + exposant) et un pointeur vers le suivant.
Les listes sont toujours triées par **exposant décroissant**.

```
2*X^3 + X^2 + 5

[coeff=2, exp=3] --> [coeff=1, exp=2] --> [coeff=5, exp=0] --> NULL
```

---

## affichage.c

### Rôle
Afficher un polynôme de façon lisible dans le terminal.

### Fonctions

#### `afficher_monome(double c, unsigned int e)`
Affiche un seul monôme selon son exposant :

| Cas | Exemple | Affichage |
|-----|---------|-----------|
| `e == 0` | constante | `5` |
| `e == 1`, `c == 1` | | `X` |
| `e == 1`, `c == -1` | | `-X` |
| `e == 1`, autre | | `3*X` |
| `e >= 2`, `c == 1` | | `X^4` |
| `e >= 2`, `c == -1` | | `-X^4` |
| `e >= 2`, autre | | `2*X^3` |

#### `afficher_signe(double c)`
Affiche le séparateur entre deux monômes consécutifs.
- `c >= 0` → affiche ` + `
- `c < 0`  → affiche ` - `

Le signe est séparé de l'affichage du monôme pour que `afficher_monome` travaille toujours avec la valeur absolue du coefficient (sauf pour le premier monôme).

#### `afficher(polynome *p)`
Parcourt la liste chaînée et affiche le polynôme complet.

```
p == NULL          → affiche "0"
premier nœud       → afficher_monome avec le vrai coefficient (peut être négatif)
nœuds suivants     → afficher_signe + afficher_monome avec valeur absolue
```

Exemple de sortie : `-4.5*X^5 + 2*X^4 + X^3 - X + 123`

---

## addition.c

### Rôle
Additionner deux polynômes `a` et `b` et retourner un nouveau polynôme résultat.

### Fonction `plus(polynome *a, polynome *b)`

Les deux listes étant triées, on les fusionne comme deux listes triées en parallèle.

**Boucle principale** — tant que `a` et `b` ont des éléments :

```
exposant(a) > exposant(b)  →  on copie le nœud de a dans le résultat
exposant(a) < exposant(b)  →  on copie le nœud de b dans le résultat
exposant(a) == exposant(b) →  on additionne les coefficients
                               si la somme == 0 : les termes s'annulent, on jette le nœud
```

**Recopie des restes** — quand une liste est épuisée, on recopie les nœuds restants de l'autre tels quels.

La variable `queue` pointe toujours sur le dernier nœud inséré pour éviter de reparcourir la liste à chaque insertion.

Exemple :
```
A = 2*X^3 + X^2 + 5
B = X^3 - X^2 + 3*X
A + B = 3*X^3 + 3*X + 5   (les X^2 s'annulent)
```

---

## soustraction.c

### Rôle
Soustraire `b` de `a` et retourner un nouveau polynôme résultat.

### Fonction `moins(polynome *a, polynome *b)`

Même structure que `plus`, avec une seule différence : **quand on prend un monôme de `b`, on inverse son coefficient**, car soustraire `b` revient à additionner `-b`.

```
exposant(a) > exposant(b)  →  on copie le nœud de a tel quel
exposant(a) < exposant(b)  →  on copie le nœud de b avec coefficient inversé
exposant(a) == exposant(b) →  diff = coeff(a) - coeff(b)
                               si diff == 0 : les termes s'annulent, on jette le nœud
```

**Recopie des restes :**
- Restes de `a` → copiés tels quels
- Restes de `b` → copiés avec coefficient inversé

Exemple :
```
A = 2*X^3 + X^2 + 5
B = X^3 - X^2 + 3*X
A - B = X^3 + 2*X^2 - 3*X + 5
```

---

## multiplication.c

### Rôle
Multiplier deux polynômes `a` et `b` et retourner un nouveau polynôme résultat.

### Fonction `fois(polynome *a, polynome *b)`

On applique la distributivité : chaque monôme de `a` est multiplié par tous les monômes de `b`.

**Pour chaque monôme `i` de `a` :**
1. On construit une liste intermédiaire `inter` en multipliant `i` par chaque monôme `j` de `b` :
   - `coefficient = i.coefficient * j.coefficient`
   - `exposant    = i.exposant + j.exposant`
2. On additionne `inter` au résultat avec `plus()`.

On réutilise `plus` pour gérer automatiquement le tri et la fusion des termes de même degré.

Exemple :
```
A = 2*X^2 + 3
B = X + 1

tour 1 : 2*X^2 * (X + 1) = 2*X^3 + 2*X^2   →  res = 2*X^3 + 2*X^2
tour 2 : 3     * (X + 1) = 3*X   + 3        →  res = 2*X^3 + 2*X^2 + 3*X + 3

A * B = 2*X^3 + 2*X^2 + 3*X + 3
```

---

## division.c

### Rôle
Effectuer la division euclidienne de `a` par `b`.
Retourne le quotient et fournit le reste via un pointeur de pointeur `**reste`.

On cherche `q` et `r` tels que : **`a = b * q + r`** avec `deg(r) < deg(b)`

### Fonction `quotient(polynome *a, polynome *b, polynome **reste)`

**Initialisation :** on copie `a` dans `rem` (remainder = reste courant) pour ne pas modifier `a`.

**Boucle** — tant que `deg(rem) >= deg(b)` :

```
étape 1 : c = coeff dominant de rem / coeff dominant de b
          e = exposant dominant de rem - exposant dominant de b
          → c*X^e est le prochain terme du quotient

étape 2 : on ajoute c*X^e au quotient

étape 3 : rem = rem - (c*X^e * b)
          on utilise fois() et moins() déjà écrits
```

À chaque tour `rem` perd son terme dominant, son degré diminue jusqu'à passer sous `deg(b)`.

**Fin :** `rem` est le reste final, on le renvoie via `*reste`.

Exemple :
```
A = 2*X^3 + 3*X^2 + X + 1
B = X + 1

tour 1 : 2*X^3 / X = 2*X^2  →  rem = rem - 2*X^2*(X+1) = X^2 + X + 1
tour 2 : X^2   / X = X      →  rem = rem - X*(X+1)      = 1
deg(rem) = 0 < deg(b) = 1   →  stop

quotient = 2*X^2 + X
reste    = 1

Vérification : (X+1)*(2*X^2 + X) + 1 = 2*X^3 + 3*X^2 + X + 1 ✓
```
