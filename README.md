# Hacking_Time
Le programme suivant utilise l’heure actuelle comme graine pour générer une clé de 128 bits :

```c
unsigned char* generateKey(int timeSeed) {
    // Allouer mémoire pour la clé
    unsigned char* key = (unsigned char*) malloc(KEYSIZE * sizeof(unsigned char));
    if (key == NULL) {
        // Erreur d’allocation mémoire
        fprintf(stderr, "Échec de l’allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    // Initialiser le générateur aléatoire
    srand(timeSeed);
    // Générer la clé
    for (int i = 0; i < KEYSIZE; i++) {
        key[i] = rand() % 256;
    }
    return key;
}
````
Le ransomware a chiffré les fichiers de la victime à l’aide de AES-128-CBC en utilisant le programme précédent comme algorithme de génération des clés. Parmi ces fichiers se trouvent des fichiers PDF, PNG, JPG, etc. Prenons un fichier PDF comme exemple. Le fichier PDF a un en-tête connu, qui commence par %PDF-(version du PDF), soit 8 octets de données. Les 8 octets suivants des données sont également assez faciles à prévoir.

Par conséquent, la victime a facilement obtenu les 16 premiers octets du texte en clair. Ainsi, la victime connaît un bloc de texte en clair et son texte chiffré correspondant. De plus, la victime connaît également le vecteur d’initialisation (IV) du fichier chiffré (IV n’est jamais chiffré).

En prenant les valeurs suivantes comme exemple :
````
Plaintext : 255044462d312e350a25d0d4c5d80a34
Ciphertext : d06bf9d0daba8e8ef880660d2af65aa82
IV : 09080706050403020100A2B2C2D2E2F2`
````
La victime sait que le chiffrement a eu lieu vers le 17 avril 2018 à 22h. La victime a créé le programme suivant pour trouver sa clé en utilisant la fonction generateKey :

python
Copy code
### Conversion de la chaîne de caractères représentant une date en timestamp Unix~
```python
date_time = "2018-04-17 22:00:00"
unix_timestamp = int(time.mktime(time.strptime(date_time, "%Y-%m-%d %H:%M:%S")))
````

### Boucle parcourant une plage de timestamps
```python
for i in range(0, 1000000):
    # Appeler la fonction generateKey
    key_ptr = my_functions.generateKey(unix_timestamp + i)
    key = bytes(key_ptr.contents)
    cypher = encrypt(pl, key, iv)
    
    # Vérifier si le texte chiffré correspond au texte chiffré attendu
    if cypher == ct:
        # Si une correspondance est trouvée, afficher la clé et terminer
        print("Clé trouvée", key.hex())
        break
````
Dans cette partie du programme, la génération de clés itère sur une plage de timestamps Unix, en augmentant d’une seconde à chaque itération, essayant ainsi toutes les clés qui auraient pu être générées au cours des 28 prochaines heures (100000 / 3600 = 27.77). Si une correspondance est trouvée entre le texte chiffré généré et le texte chiffré cible, cela indique que la clé correcte a été trouvée. Le programme affiche la clé et se termine. Sinon, il continue la recherche jusqu’à ce que toutes les clés possibles dans la plage de temps spécifiée aient été épuisées.

En moins d’une seconde, la victime a trouvé la clé symétrique.
````
Plaintext : b'%PDF-1.5\n%\xd0\xd4\xc5\xd8\n4'
Ciphertext : b'\xd0k\xf9\xd0\xda\xb8\xe8\xef\x88\x06\xd2\xaf\xeaa\x82'
IV : b'\t\x08\x07\x06\x05\x04\x03\x02\x01\x00\xa2\xb2\xc2\xd2\xe2\x0f2'
Unix Timestamp : 1523995309
Key found : 95fa2030e73e3f8da761b4eb805dfd7
Time taken : 0.620751142501831 seconds
````
En utilisant le temps comme graine (seed) pour générer les clés, la victime a pu facilement trouver la clé pour déchiffrer tous ses fichiers sans payer la rançon.