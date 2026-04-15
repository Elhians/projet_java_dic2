# Documentation — Manipulation de polynômes en Java

Les polynômes sont représentés par des listes chaînées de nœuds (`Noeud`), chaque nœud contenant un coefficient (`double`), un exposant (`int`) et un pointeur vers le suivant.
Les listes sont toujours triées par **exposant décroissant**.

```
2*X^3 + X^2 + 5

[coeff=2, exp=3] --> [coeff=1, exp=2] --> [coeff=5, exp=0] --> null
```

### Différences avec la version C

| C | Java |
|---|------|
| `struct monome` + `struct polynome` | classe `Noeud` (coefficient + exposant + suivant) |
| pointeurs `*` et `**` | références d'objets et tableau `Noeud[]` pour le double pointeur |
| `malloc` / `free` | `new Noeud(...)` / garbage collector |
| `printf("%g", c)` | `String.format` via `formatCoeff()` |

---

## AFFICHAGE

### Rôle
Afficher un polynôme de façon lisible dans le terminal.

### Méthodes

#### `afficherMonome(double c, int e)`
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

#### `afficherSigne(double c)`
Affiche le séparateur entre deux monômes consécutifs.
- `c >= 0` → affiche ` + `
- `c < 0`  → affiche ` - `

#### `afficher(Noeud p)`
Parcourt la liste chaînée et affiche le polynôme complet.

```
p == null          → affiche "0"
premier nœud       → afficherMonome avec le vrai coefficient (peut être négatif)
nœuds suivants     → afficherSigne + afficherMonome avec valeur absolue
```

Exemple de sortie : `-4.5*X^5 + 2*X^4 + X^3 - X + 123`

---

## ADDITION

### Rôle
Additionner deux polynômes `a` et `b` et retourner un nouveau polynôme résultat.

### Méthode `plus(Noeud a, Noeud b)`

Les deux listes étant triées, on les fusionne comme deux listes triées en parallèle.

**Boucle principale** — tant que `a` et `b` ont des éléments :

```
exposant(a) > exposant(b)  →  on copie le nœud de a dans le résultat
exposant(a) < exposant(b)  →  on copie le nœud de b dans le résultat
exposant(a) == exposant(b) →  on additionne les coefficients
                               si la somme == 0 : les termes s'annulent, on jette le nœud
```

**Recopie des restes** — quand une liste est épuisée, on recopie les nœuds restants de l'autre tels quels.

Exemple :
```
A = 2*X^3 + X^2 + 5
B = X^3 - X^2 + 3*X
A + B = 3*X^3 + 3*X + 5   (les X^2 s'annulent)
```

---

## SOUSTRACTION

### Rôle
Soustraire `b` de `a` et retourner un nouveau polynôme résultat.

### Méthode `moins(Noeud a, Noeud b)`

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

## MULTIPLICATION

### Rôle
Multiplier deux polynômes `a` et `b` et retourner un nouveau polynôme résultat.

### Méthode `fois(Noeud a, Noeud b)`

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

## DIVISION

### Rôle
Effectuer la division euclidienne de `a` par `b`.
Retourne le quotient et fournit le reste via un tableau `Noeud[] reste` (équivalent du `**` en C).

On cherche `q` et `r` tels que : **`a = b * q + r`** avec `deg(r) < deg(b)`

### Méthode `quotient(Noeud a, Noeud b, Noeud[] reste)`

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

**Fin :** `rem` est le reste final, stocké dans `reste[0]`.

Exemple :
```
A = 2*X^4 + 3*X^2 + X + 1
B = X + 1

quotient = 2*X^3 - 2*X^2 + 5*X - 4
reste    = 5

Vérification : (X+1) * quotient + 5 = A ✓
```
