
## 1. Gegebene Funktion

[
V(l) = \sqrt{(1-r)(1-l)} + \sqrt{l} + C
]

Dabei sind:

* ( r ) eine Konstante
* ( C ) eine Konstante
* Variable: ( l )

---

## 2. Ableitungsregeln, die wir brauchen

1. **Konstantenregel**
   [
   \frac{d}{dl} C = 0
   ]

2. **Kettenregel für die Wurzel**
   [
   \frac{d}{dl} \sqrt{f(l)} = \frac{1}{2\sqrt{f(l)}} \cdot f'(l)
   ]

---

## 3. Ableitung der einzelnen Terme

### **Erster Term**

[
\sqrt{(1-r)(1-l)}
]

Setze:
[
f(l) = (1-r)(1-l)
]

Ableitung von ( f(l) ):
[
f'(l) = (1-r)\cdot(-1) = -(1-r)
]

Jetzt Kettenregel:
[
\frac{d}{dl} \sqrt{(1-r)(1-l)} =
\frac{1}{2\sqrt{(1-r)(1-l)}} \cdot (-(1-r))
]

[
= -\frac{1-r}{2\sqrt{(1-r)(1-l)}}
]

---

### **Zweiter Term**

[
\sqrt{l}
]

[
\frac{d}{dl} \sqrt{l} = \frac{1}{2\sqrt{l}}
]

---

### **Dritter Term**

[
C
]

[
\frac{d}{dl} C = 0
]

---

## 4. Gesamtableitung

[
V'(l) =
-\frac{1-r}{2\sqrt{(1-r)(1-l)}} + \frac{1}{2\sqrt{l}}
]

---

## 5. Vereinfachung

Im ersten Term kürzt sich ( \sqrt{1-r} ):

[
\sqrt{(1-r)(1-l)} = \sqrt{1-r}\sqrt{1-l}
]

[
\frac{1-r}{\sqrt{(1-r)(1-l)}} = \frac{\sqrt{1-r}}{\sqrt{1-l}}
]

Damit:

[
V'(l) =
-\frac{\sqrt{1-r}}{2\sqrt{1-l}} + \frac{1}{2\sqrt{l}}
]

---

## ✅ **Endergebnis (vereinfacht)**

[
\boxed{
V'(l) = \frac{1}{2\sqrt{l}} - \frac{\sqrt{1-r}}{2\sqrt{1-l}}
}
]

---

Wir starten mit der bereits abgeleiteten Funktion:

[
V'(l) = \frac{1}{2\sqrt{l}} - \frac{\sqrt{1-r}}{2\sqrt{1-l}}
]

---

## 1. Erste-Ordnung-Bedingung setzen

[
V'(l) = 0
]

[
\frac{1}{2\sqrt{l}} - \frac{\sqrt{1-r}}{2\sqrt{1-l}} = 0
]

---

## 2. Faktor ( \tfrac{1}{2} ) eliminieren

Multipliziere beide Seiten mit 2:

[
\frac{1}{\sqrt{l}} - \frac{\sqrt{1-r}}{\sqrt{1-l}} = 0
]

---

## 3. Einen Term auf die andere Seite bringen

[
\frac{1}{\sqrt{l}} = \frac{\sqrt{1-r}}{\sqrt{1-l}}
]

---

## 4. Beide Seiten quadrieren

(das ist zulässig, da alle Terme positiv sind für ( l \in (0,1) ))

[
\frac{1}{l} = \frac{1-r}{1-l}
]

---

## 5. Kreuzmultiplikation

[
1 - l = l(1 - r)
]

---

## 6. Nach ( l ) auflösen

[
1 - l = l - lr
]

[
1 = 2l - lr
]

[
1 = l(2 - r)
]

---

## 7. Lösung für ( l )

[
\boxed{
l^*(r) = \frac{1}{2 - r}
}
]

---

## 8. Plausibilitätscheck

Für ( r \in [0,1] ):

* ( r = 0 \Rightarrow l = \frac{1}{2} )
* ( r = 1 \Rightarrow l = 1 )

Das liegt sauber im Intervall ( (0,1] ) und ist ökonomisch konsistent.

---

## ✅ **Endergebnis**

[
\boxed{
l^*(r) = \frac{1}{2 - r}
}
]

